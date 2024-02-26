/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

/* Includes ------------------------------------------------------------------*/
#include "app_ble.h"
#include "app_conf.h"
#include "ble.h"
#include "dbg_trace.h"
#include "os.h"
#include "ov_config.h"
#include "semaphore.h"
#include "timer.h"
#include "tl.h"

#include "otp.h"
#include "shci.h"
#include "shci_tl.h"
#include "stm32_seq.h"

#include "dis_app.h"
#include "stm32wbxx_ll_system.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/**
 * security parameters structure
 */
typedef struct _tSecurityParams
{
    /**
   * IO capability of the device
   */
    uint8_t ioCapability;

    /**
   * Authentication requirement of the device
   * Man In the Middle protection required?
   */
    uint8_t mitm_mode;

    /**
   * bonding mode of the device
   */
    uint8_t bonding_mode;

    /**
   * this variable indicates whether to use a fixed pin
   * during the pairing process or a passkey has to be
   * requested to the application during the pairing process
   * 0 implies use fixed pin and 1 implies request for passkey
   */
    uint8_t Use_Fixed_Pin;

    /**
   * minimum encryption key size requirement
   */
    uint8_t encryptionKeySizeMin;

    /**
   * maximum encryption key size requirement
   */
    uint8_t encryptionKeySizeMax;

    /**
   * fixed pin to be used in the pairing process if
   * Use_Fixed_Pin is set to 1
   */
    uint32_t Fixed_Pin;

    /**
   * this flag indicates whether the host has to initiate
   * the security, wait for pairing or does not have any security
   * requirements.
   * 0x00 : no security required
   * 0x01 : host should initiate security by sending the slave security
   *        request command
   * 0x02 : host need not send the clave security request but it
   * has to wait for paiirng to complete before doing any other
   * processing
   */
    uint8_t initiateSecurity;
    /* USER CODE BEGIN tSecurityParams*/

    /* USER CODE END tSecurityParams */
} tSecurityParams;

/**
 * global context
 * contains the variables common to all
 * services
 */
typedef struct _tBLEProfileGlobalContext
{
    /**
   * security requirements of the host
   */
    tSecurityParams bleSecurityParam;

    /**
   * gap service handle
   */
    uint16_t gapServiceHandle;

    /**
   * device name characteristic handle
   */
    uint16_t devNameCharHandle;

    /**
   * appearance characteristic handle
   */
    uint16_t appearanceCharHandle;

    /**
   * connection handle of the current active connection
   * When not in connection, the handle is set to 0xFFFF
   */
    uint16_t connectionHandle;

    /**
   * length of the UUID list to be used while advertising
   */
    uint8_t advtServUUIDlen;

    /**
   * the UUID list to be used while advertising
   */
    uint8_t advtServUUID[100];
    /* USER CODE BEGIN BleGlobalContext_t*/

    /* USER CODE END BleGlobalContext_t */
} BleGlobalContext_t;

typedef struct
{
    BleGlobalContext_t   BleApplicationContext_legacy;
    APP_BLE_ConnStatus_t Device_Connection_Status;

    /**
   * ID of the Advertising Timeout
   */
    uint8_t Advertising_mgr_timer_Id;
    /* USER CODE BEGIN PTD_1*/

    /* USER CODE END PTD_1 */
} BleApplicationContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 32
#define FAST_ADV_TIMEOUT (30 * 1000 * 1000 / CFG_TS_TICK_VAL) /**< 30s */
#define INITIAL_ADV_TIMEOUT 60000u                            //(60 * 1000 * 1000 / CFG_TS_TICK_VAL) /**< 60s */

#define BD_ADDR_SIZE_LOCAL 6

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;

static const uint8_t a_MBdAddr[BD_ADDR_SIZE_LOCAL] = {(uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF)),
                                                      (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
                                                      (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
                                                      (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
                                                      (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
                                                      (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40)};

static uint8_t a_BdAddrUdn[BD_ADDR_SIZE_LOCAL];

/**
 *   Identity root key used to derive IRK and DHK(Legacy)
 */
static const uint8_t a_BLE_CfgIrValue[16] = CFG_BLE_IR;

/**
 * Encryption root key used to derive LTK(Legacy) and CSRK
 */
static const uint8_t a_BLE_CfgErValue[16] = CFG_BLE_ER;

/**
 * These are the two tags used to manage a power failure during OTA
 * The MagicKeywordAdress shall be mapped @0x140 from start of the binary image
 * The MagicKeywordvalue is checked in the ble_ota application
 */
PLACE_IN_SECTION("TAG_OTA_END") const uint32_t MagicKeywordValue     = 0x94448A29;
PLACE_IN_SECTION("TAG_OTA_START") const uint32_t MagicKeywordAddress = (uint32_t)&MagicKeywordValue;

static BleApplicationContext_t BleApplicationContext;
static uint16_t                AdvIntervalMin, AdvIntervalMax;

static char    a_LocalName[32u + 1];
static size_t  a_LocalName_size;
static uint8_t a_ManufData[14] = {
    sizeof(a_ManufData) - 1,
    AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01 /*SKD version */,
    0x00 /* Generic*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void           BLE_UserEvtRx(void* p_Payload);
static void           BLE_StatusNot(HCI_TL_CmdStatus_t Status);
static void           Ble_Tl_Init(void);
static void           Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress(void);
static void           Adv_Request(APP_BLE_ConnStatus_t NewStatus);
static void           Add_Advertisment_Service_UUID(uint16_t servUUID);
static void           Adv_Mgr(void);
static void           Adv_Update(void);

/* USER CODE BEGIN PFP */

static void Error_Handler()
{
    __disable_irq();
    while (1)
        ;
}

/* USER CODE END PFP */

/* External variables --------------------------------------------------------*/
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH * 4U * DIVC((sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE), 4U))

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

void        APPE_Tl_Init(void);
static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status);
static void APPE_SysUserEvtRx(void* pPayload);
static void APPE_SysEvtReadyProcessing(void* pPayload);
static void APPE_SysEvtError(void* pPayload);

/* USER CODE BEGIN EV */

/* USER CODE END EV */

class timer_wrapper
{
  public:
    timer_wrapper(HW_TS_pTimerCb_t callback) : m_callback(callback) { }
    void timer_func() { m_callback(); }

  private:
    HW_TS_pTimerCb_t m_callback;
};

timer_wrapper advertising_timer_wrapper(Adv_Mgr);
ov::timer     advertising_timer(ov::timer_func::create<timer_wrapper, &timer_wrapper::timer_func>(advertising_timer_wrapper), "ADV_TIMER");

ov::semaphore hci_cmd_sem(0u, 1u);
ov::semaphore shci_cmd_sem(0u, 1u);

bool s_app_ble_is_ready = false;

/* Functions Definition ------------------------------------------------------*/
static void APP_BLE_Init(void)
{
    // Device name
    auto& ov_config = ov::config::get();
    a_LocalName[0u] = AD_TYPE_COMPLETE_LOCAL_NAME;
    memcpy(&a_LocalName[1u], ov_config.device_name, sizeof(ov_config.device_name));
    a_LocalName_size = strlen(a_LocalName);

    SHCI_CmdStatus_t status;
    /* USER CODE BEGIN APP_BLE_Init_1 */
    WirelessFwInfo_t  wireless_info_instance;
    WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;
    char              BdAddress[20];
    char              StackVersion[23];
    char              StackBranch[20];
    char              FusVersion[20];
    const uint8_t*    bdaddr;
    /* USER CODE END APP_BLE_Init_1 */
    SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet = {{{0, 0, 0}}, /**< Header unused */
                                                         {0,          /** pBleBufferAddress not used */
                                                          0,          /** BleBufferSize not used */
                                                          CFG_BLE_NUM_GATT_ATTRIBUTES,
                                                          CFG_BLE_NUM_GATT_SERVICES,
                                                          CFG_BLE_ATT_VALUE_ARRAY_SIZE,
                                                          CFG_BLE_NUM_LINK,
                                                          CFG_BLE_DATA_LENGTH_EXTENSION,
                                                          CFG_BLE_PREPARE_WRITE_LIST_SIZE,
                                                          CFG_BLE_MBLOCK_COUNT,
                                                          CFG_BLE_MAX_ATT_MTU,
                                                          CFG_BLE_PERIPHERAL_SCA,
                                                          CFG_BLE_CENTRAL_SCA,
                                                          CFG_BLE_LS_SOURCE,
                                                          CFG_BLE_MAX_CONN_EVENT_LENGTH,
                                                          CFG_BLE_HSE_STARTUP_TIME,
                                                          CFG_BLE_VITERBI_MODE,
                                                          CFG_BLE_OPTIONS,
                                                          0,
                                                          CFG_BLE_MAX_COC_INITIATOR_NBR,
                                                          CFG_BLE_MIN_TX_POWER,
                                                          CFG_BLE_MAX_TX_POWER,
                                                          CFG_BLE_RX_MODEL_CONFIG,
                                                          CFG_BLE_MAX_ADV_SET_NBR,
                                                          CFG_BLE_MAX_ADV_DATA_LEN,
                                                          CFG_BLE_TX_PATH_COMPENS,
                                                          CFG_BLE_RX_PATH_COMPENS,
                                                          CFG_BLE_CORE_VERSION,
                                                          CFG_BLE_OPTIONS_EXT}};

    /**
   * Initialize Ble Transport Layer
   */
    Ble_Tl_Init();

    /**
   * Register the hci transport layer to handle BLE User Asynchronous Events
   */
    UTIL_SEQ_RegTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, hci_user_evt_proc);

    /**
   * Starts the BLE Stack on CPU2
   */
    status = SHCI_C2_BLE_Init(&ble_init_cmd_packet);
    if (status != SHCI_Success)
    {
        APP_DBG_MSG("  Fail   : SHCI_C2_BLE_Init command, result: 0x%02x\n\r", status);
        /* if you are here, maybe CPU2 doesn't contain STM32WB_Copro_Wireless_Binaries, see Release_Notes.html */
        Error_Handler();
    }
    else
    {
        APP_DBG_MSG("  Success: SHCI_C2_BLE_Init command\n\r");
    }

    /**
   * Initialization of HCI & GATT & GAP layer
   */
    Ble_Hci_Gap_Gatt_Init();

    /**
   * Initialization of the BLE Services
   */
    SVCCTL_Init();

    /**
   * Initialization of the BLE App Context
   */
    BleApplicationContext.Device_Connection_Status                      = APP_BLE_IDLE;
    BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0xFFFF;

    /**
   * From here, all initialization are BLE application specific
   */
    UTIL_SEQ_RegTask(1 << CFG_TASK_ADV_UPDATE_ID, UTIL_SEQ_RFU, Adv_Update);

    /**
   * Initialization of ADV - Ad Manufacturer Element - Support OTA Bit Mask
   */
#if (BLE_CFG_OTA_REBOOT_CHAR != 0)
    a_ManufData[sizeof(a_ManufData) - 8] = CFG_FEATURE_OTA_REBOOT;
#endif /* BLE_CFG_OTA_REBOOT_CHAR != 0 */

    /**
   * Initialize DIS Application
   */
    DISAPP_Init();

    /**
   * Create timer to handle the connection state machine
   */
    // HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.Advertising_mgr_timer_Id), hw_ts_SingleShot, Adv_Mgr);

    /**
   * Make device discoverable
   */
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = AD_TYPE_16_BIT_SERV_UUID;
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 1;
    Add_Advertisment_Service_UUID(HEART_RATE_SERVICE_UUID);

    /* Initialize intervals for reconnexion without intervals update */
    AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
    AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;

    /**
   * Start to Advertise to be connected by Collector
   */
    Adv_Request(APP_BLE_FAST_ADV);

    /* USER CODE BEGIN APP_BLE_Init_2 */

    /* Displays the board information: MAC Address, Stack version, FUS version*/
    if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
    {
        // Error
    }
    else
    {
        bdaddr = BleGetBdAddress();
        sprintf(BdAddress, "BD_ad=%02x%02x%02x%02x%02x%02x", bdaddr[5], bdaddr[4], bdaddr[3], bdaddr[2], bdaddr[1], bdaddr[0]);
        sprintf(
            StackVersion, "BLE Stack=v%d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);
        sprintf(StackBranch, "Branch=%d Type=%d", p_wireless_info->VersionBranch, p_wireless_info->VersionReleaseType);
        sprintf(FusVersion,
                "FUS v%d.%d.%d",
                p_wireless_info->FusVersionMajor,
                p_wireless_info->FusVersionMinor,
                p_wireless_info->FusVersionSub);
    }

    s_app_ble_is_ready = true;

    return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void* p_Pckt)
{
    hci_event_pckt*                       p_event_pckt;
    evt_le_meta_event*                    p_meta_evt;
    evt_blecore_aci*                      p_blecore_evt;
    uint8_t                               Tx_phy, Rx_phy;
    tBleStatus                            ret = BLE_STATUS_INVALID_PARAMS;
    hci_le_connection_complete_event_rp0* p_connection_complete_event;
    hci_disconnection_complete_event_rp0* p_disconnection_complete_event;
    hci_le_phy_update_complete_event_rp0* p_evt_le_phy_update_complete;
#if (CFG_DEBUG_APP_TRACE != 0)
    hci_le_connection_update_complete_event_rp0* p_connection_update_complete_event;
#endif /* CFG_DEBUG_APP_TRACE != 0 */

    /* USER CODE BEGIN SVCCTL_App_Notification */
    char           BdAddress[20];
    const uint8_t* bdaddr;
    /* USER CODE END SVCCTL_App_Notification */

    p_event_pckt = (hci_event_pckt*)((hci_uart_pckt*)p_Pckt)->data;

    switch (p_event_pckt->evt)
    {
        case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
        {
            p_disconnection_complete_event = (hci_disconnection_complete_event_rp0*)p_event_pckt->data;

            if (p_disconnection_complete_event->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
            {
                BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
                BleApplicationContext.Device_Connection_Status                      = APP_BLE_IDLE;
                APP_DBG_MSG(">>== HCI_DISCONNECTION_COMPLETE_EVT_CODE\n");
                APP_DBG_MSG("     - Connection Handle:   0x%x\n     - Reason:    0x%x\n\r",
                            p_disconnection_complete_event->Connection_Handle,
                            p_disconnection_complete_event->Reason);

                /* USER CODE BEGIN EVT_DISCONN_COMPLETE_2 */

                /* USER CODE END EVT_DISCONN_COMPLETE_2 */
            }

            /* USER CODE BEGIN EVT_DISCONN_COMPLETE_1 */
            bdaddr = BleGetBdAddress();
            sprintf(BdAddress, "BD_ad=%02x%02x%02x%02x%02x%02x", bdaddr[5], bdaddr[4], bdaddr[3], bdaddr[2], bdaddr[1], bdaddr[0]);
            /* USER CODE END EVT_DISCONN_COMPLETE_1 */

            /* restart advertising */
            Adv_Request(APP_BLE_FAST_ADV);

            /* USER CODE BEGIN EVT_DISCONN_COMPLETE */

            /* USER CODE END EVT_DISCONN_COMPLETE */
            break; /* HCI_DISCONNECTION_COMPLETE_EVT_CODE */
        }

        case HCI_LE_META_EVT_CODE:
        {
            p_meta_evt = (evt_le_meta_event*)p_event_pckt->data;
            /* USER CODE BEGIN EVT_LE_META_EVENT */

            /* USER CODE END EVT_LE_META_EVENT */
            switch (p_meta_evt->subevent)
            {
                case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
#if (CFG_DEBUG_APP_TRACE != 0)
                    p_connection_update_complete_event = (hci_le_connection_update_complete_event_rp0*)p_meta_evt->data;
                    APP_DBG_MSG(">>== HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE\n");
                    APP_DBG_MSG(
                        "     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                        p_connection_update_complete_event->Conn_Interval * 1.25,
                        p_connection_update_complete_event->Conn_Latency,
                        p_connection_update_complete_event->Supervision_Timeout * 10);
#endif /* CFG_DEBUG_APP_TRACE != 0 */

                    /* USER CODE BEGIN EVT_LE_CONN_UPDATE_COMPLETE */

                    /* USER CODE END EVT_LE_CONN_UPDATE_COMPLETE */
                    break;

                case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
                    p_evt_le_phy_update_complete = (hci_le_phy_update_complete_event_rp0*)p_meta_evt->data;
                    APP_DBG_MSG("==>> HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE - ");
                    if (p_evt_le_phy_update_complete->Status == 0)
                    {
                        APP_DBG_MSG("status ok \n");
                    }
                    else
                    {
                        APP_DBG_MSG("status nok \n");
                    }

                    ret = hci_le_read_phy(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, &Tx_phy, &Rx_phy);
                    if (ret != BLE_STATUS_SUCCESS)
                    {
                        APP_DBG_MSG("==>> hci_le_read_phy : fail\n\r");
                    }
                    else
                    {
                        APP_DBG_MSG("==>> hci_le_read_phy - Success \n");

                        if ((Tx_phy == TX_2M) && (Rx_phy == RX_2M))
                        {
                            APP_DBG_MSG("==>> PHY Param  TX= %d, RX= %d \n\r", Tx_phy, Rx_phy);
                        }
                        else
                        {
                            APP_DBG_MSG("==>> PHY Param  TX= %d, RX= %d \n\r", Tx_phy, Rx_phy);
                        }
                    }
                    /* USER CODE BEGIN EVT_LE_PHY_UPDATE_COMPLETE */

                    /* USER CODE END EVT_LE_PHY_UPDATE_COMPLETE */
                    break;

                case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                {
                    p_connection_complete_event = (hci_le_connection_complete_event_rp0*)p_meta_evt->data;
                    /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */

                    //HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);
                    advertising_timer.stop();

                    APP_DBG_MSG(">>== HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE - Connection handle: 0x%x\n",
                                p_connection_complete_event->Connection_Handle);
                    APP_DBG_MSG("     - Connection established with Central: @:%02x:%02x:%02x:%02x:%02x:%02x\n",
                                p_connection_complete_event->Peer_Address[5],
                                p_connection_complete_event->Peer_Address[4],
                                p_connection_complete_event->Peer_Address[3],
                                p_connection_complete_event->Peer_Address[2],
                                p_connection_complete_event->Peer_Address[1],
                                p_connection_complete_event->Peer_Address[0]);
                    APP_DBG_MSG(
                        "     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                        p_connection_complete_event->Conn_Interval * 1.25,
                        p_connection_complete_event->Conn_Latency,
                        p_connection_complete_event->Supervision_Timeout * 10);

                    if (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_CONNECTING)
                    {
                        /* Connection as client */
                        BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;
                    }
                    else
                    {
                        /* Connection as server */
                        BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_SERVER;
                    }
                    BleApplicationContext.BleApplicationContext_legacy.connectionHandle = p_connection_complete_event->Connection_Handle;
                    /* USER CODE BEGIN HCI_EVT_LE_CONN_COMPLETE */
                    bdaddr = BleGetBdAddress();
                    sprintf(BdAddress, "BD_ad=%02x%02x%02x%02x%02x%02x", bdaddr[5], bdaddr[4], bdaddr[3], bdaddr[2], bdaddr[1], bdaddr[0]);
                    /* USER CODE END HCI_EVT_LE_CONN_COMPLETE */
                    break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
                }

                default:
                    /* USER CODE BEGIN SUBEVENT_DEFAULT */

                    /* USER CODE END SUBEVENT_DEFAULT */
                    break;
            }

            /* USER CODE BEGIN META_EVT */

            /* USER CODE END META_EVT */
            break; /* HCI_LE_META_EVT_CODE */
        }

        case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
            p_blecore_evt = (evt_blecore_aci*)p_event_pckt->data;
            /* USER CODE BEGIN EVT_VENDOR */

            /* USER CODE END EVT_VENDOR */
            switch (p_blecore_evt->ecode)
            {
                /* USER CODE BEGIN ecode */
                aci_gap_pairing_complete_event_rp0* pairing_complete;

                case ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE:
                    APP_DBG_MSG(">>== ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE \n");
                    break; /* ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE */

                case ACI_GAP_PASS_KEY_REQ_VSEVT_CODE:
                    APP_DBG_MSG(">>== ACI_GAP_PASS_KEY_REQ_VSEVT_CODE \n");

                    ret = aci_gap_pass_key_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, 123456);
                    if (ret != BLE_STATUS_SUCCESS)
                    {
                        APP_DBG_MSG("==>> aci_gap_pass_key_resp : Fail, reason: 0x%x\n", ret);
                    }
                    else
                    {
                        APP_DBG_MSG("==>> aci_gap_pass_key_resp : Success \n");
                    }
                    break; /* ACI_GAP_PASS_KEY_REQ_VSEVT_CODE */

                case ACI_GAP_AUTHORIZATION_REQ_VSEVT_CODE:
                    APP_DBG_MSG(">>== ACI_GAP_AUTHORIZATION_REQ_VSEVT_CODE\n");
                    break; /* ACI_GAP_AUTHORIZATION_REQ_VSEVT_CODE */

                case ACI_GAP_PERIPHERAL_SECURITY_INITIATED_VSEVT_CODE:
                    APP_DBG_MSG("==>> ACI_GAP_PERIPHERAL_SECURITY_INITIATED_VSEVT_CODE \n");
                    break; /* ACI_GAP_PERIPHERAL_SECURITY_INITIATED_VSEVT_CODE */

                case ACI_GAP_BOND_LOST_VSEVT_CODE:
                    APP_DBG_MSG("==>> ACI_GAP_BOND_LOST_VSEVT_CODE \n");
                    ret = aci_gap_allow_rebond(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
                    if (ret != BLE_STATUS_SUCCESS)
                    {
                        APP_DBG_MSG("==>> aci_gap_allow_rebond : Fail, reason: 0x%x\n", ret);
                    }
                    else
                    {
                        APP_DBG_MSG("==>> aci_gap_allow_rebond : Success \n");
                    }
                    break; /* ACI_GAP_BOND_LOST_VSEVT_CODE */

                case ACI_GAP_ADDR_NOT_RESOLVED_VSEVT_CODE:
                    APP_DBG_MSG(">>== ACI_GAP_ADDR_NOT_RESOLVED_VSEVT_CODE \n");
                    break; /* ACI_GAP_ADDR_NOT_RESOLVED_VSEVT_CODE */

                case (ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE):
                    APP_DBG_MSG(">>== ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE\n");
                    break; /* ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE */

                case (ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE):
                    APP_DBG_MSG(">>== ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE\n");
                    APP_DBG_MSG("     - numeric_value = %ld\n",
                                ((aci_gap_numeric_comparison_value_event_rp0*)(p_blecore_evt->data))->Numeric_Value);
                    APP_DBG_MSG("     - Hex_value = %lx\n",
                                ((aci_gap_numeric_comparison_value_event_rp0*)(p_blecore_evt->data))->Numeric_Value);
                    ret = aci_gap_numeric_comparison_value_confirm_yesno(
                        BleApplicationContext.BleApplicationContext_legacy.connectionHandle, YES); /* CONFIRM_YES = 1 */
                    if (ret != BLE_STATUS_SUCCESS)
                    {
                        APP_DBG_MSG("==>> aci_gap_numeric_comparison_value_confirm_yesno-->YES : Fail, reason: 0x%x\n", ret);
                    }
                    else
                    {
                        APP_DBG_MSG("==>> aci_gap_numeric_comparison_value_confirm_yesno-->YES : Success \n");
                    }
                    break;

                case (ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE):
                {
                    pairing_complete = (aci_gap_pairing_complete_event_rp0*)p_blecore_evt->data;

                    APP_DBG_MSG(">>== ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE\n");
                    if (pairing_complete->Status == 0)
                    {
                        APP_DBG_MSG("     - Pairing Success\n");
                    }
                    else
                    {
                        APP_DBG_MSG("     - Pairing KO \n     - Status: 0x%x\n     - Reason: 0x%x\n",
                                    pairing_complete->Status,
                                    pairing_complete->Reason);
                    }
                    APP_DBG_MSG("\n");
                }
                break;
                    /* USER CODE END ecode */

                case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
                    APP_DBG_MSG(">>== ACI_GAP_PROC_COMPLETE_VSEVT_CODE \r");
                    /* USER CODE BEGIN EVT_BLUE_GAP_PROCEDURE_COMPLETE */

                    /* USER CODE END EVT_BLUE_GAP_PROCEDURE_COMPLETE */
                    break; /* ACI_GAP_PROC_COMPLETE_VSEVT_CODE */

                    /* USER CODE BEGIN BLUE_EVT */

                    /* USER CODE END BLUE_EVT */
            }
            break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

            /* USER CODE BEGIN EVENT_PCKT */

            /* USER CODE END EVENT_PCKT */

        default:
            /* USER CODE BEGIN ECODE_DEFAULT*/

            /* USER CODE END ECODE_DEFAULT*/
            break;
    }

    return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_Get_Server_Connection_Status(void)
{
    return BleApplicationContext.Device_Connection_Status;
}

/* USER CODE BEGIN FD*/
void APP_BLE_Key_Button1_Action(void)
{
    tBleStatus ret = BLE_STATUS_INVALID_PARAMS;

    ret = aci_gap_clear_security_db();
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("==>> aci_gap_clear_security_db - Fail, result: %d \n", ret);
    }
    else
    {
        APP_DBG_MSG("==>> aci_gap_clear_security_db - Success\n");
    }
}

void APP_BLE_Key_Button2_Action(void)
{
    tBleStatus ret = BLE_STATUS_INVALID_PARAMS;

    ret = aci_gap_slave_security_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);

    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("==>> aci_gap_slave_security_req() Fail , result: %d \n", ret);
    }
    else
    {
        APP_DBG_MSG("===>> aci_gap_slave_security_req - Success\n");
    }
}

/* USER CODE END FD*/

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Ble_Tl_Init(void)
{
    HCI_TL_HciInitConf_t Hci_Tl_Init_Conf;

    Hci_Tl_Init_Conf.p_cmdbuffer       = (uint8_t*)&BleCmdBuffer;
    Hci_Tl_Init_Conf.StatusNotCallBack = BLE_StatusNot;
    hci_init(BLE_UserEvtRx, (void*)&Hci_Tl_Init_Conf);

    return;
}

static void Ble_Hci_Gap_Gatt_Init(void)
{
    uint8_t        role;
    uint16_t       gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
    const uint8_t* p_bd_addr;
    uint16_t       a_appearance[1] = {BLE_CFG_GAP_APPEARANCE};
    tBleStatus     ret             = BLE_STATUS_INVALID_PARAMS;
    /* USER CODE BEGIN Ble_Hci_Gap_Gatt_Init*/

    /* USER CODE END Ble_Hci_Gap_Gatt_Init*/

    APP_DBG_MSG("==>> Start Ble_Hci_Gap_Gatt_Init function\n");

    /**
   * Initialize HCI layer
   */
    /*HCI Reset to synchronise BLE Stack*/
    ret = hci_reset();
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : hci_reset command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: hci_reset command\n");
    }

    /**
   * Write the BD Address
   */
    p_bd_addr = BleGetBdAddress();
    ret       = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, (uint8_t*)p_bd_addr);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_PUBADDR_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_PUBADDR_OFFSET\n");
        APP_DBG_MSG("  Public Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                    p_bd_addr[5],
                    p_bd_addr[4],
                    p_bd_addr[3],
                    p_bd_addr[2],
                    p_bd_addr[1],
                    p_bd_addr[0]);
    }

#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR)
    /* BLE MAC in ADV Packet */
    a_ManufData[sizeof(a_ManufData) - 6] = p_bd_addr[5];
    a_ManufData[sizeof(a_ManufData) - 5] = p_bd_addr[4];
    a_ManufData[sizeof(a_ManufData) - 4] = p_bd_addr[3];
    a_ManufData[sizeof(a_ManufData) - 3] = p_bd_addr[2];
    a_ManufData[sizeof(a_ManufData) - 2] = p_bd_addr[1];
    a_ManufData[sizeof(a_ManufData) - 1] = p_bd_addr[0];
#endif /* CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR */

    /**
   * Static random Address
   * The two upper bits shall be set to 1
   * The lowest 32bits is read from the UDN to differentiate between devices
   * The RNG may be used to provide a random number on each power on
   */
#if (CFG_IDENTITY_ADDRESS == GAP_STATIC_RANDOM_ADDR)
#if defined(CFG_STATIC_RANDOM_ADDRESS)
    a_srd_bd_addr[0] = CFG_STATIC_RANDOM_ADDRESS & 0xFFFFFFFF;
    a_srd_bd_addr[1] = (uint32_t)((uint64_t)CFG_STATIC_RANDOM_ADDRESS >> 32);
    a_srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */
#else
    /* Get RNG semaphore */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID))
        ;

    /* Enable RNG */
    __HAL_RNG_ENABLE(&hrng);

    /* Enable HSI48 oscillator */
    LL_RCC_HSI48_Enable();
    /* Wait until HSI48 is ready */
    while (!LL_RCC_HSI48_IsReady())
        ;

    if (HAL_RNG_GenerateRandomNumber(&hrng, &a_srd_bd_addr[1]) != HAL_OK)
    {
        /* Random number generation error */
        Error_Handler();
    }
    if (HAL_RNG_GenerateRandomNumber(&hrng, &a_srd_bd_addr[0]) != HAL_OK)
    {
        /* Random number generation error */
        Error_Handler();
    }
    a_srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */

    /* Disable HSI48 oscillator */
    LL_RCC_HSI48_Disable();

    /* Disable RNG */
    __HAL_RNG_DISABLE(&hrng);

    /* Release RNG semaphore */
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
#endif /* CFG_STATIC_RANDOM_ADDRESS */
#endif

#if (CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR)
    /* BLE MAC in ADV Packet */
    a_ManufData[sizeof(a_ManufData) - 6] = a_srd_bd_addr[1] >> 8;
    a_ManufData[sizeof(a_ManufData) - 5] = a_srd_bd_addr[1];
    a_ManufData[sizeof(a_ManufData) - 4] = a_srd_bd_addr[0] >> 24;
    a_ManufData[sizeof(a_ManufData) - 3] = a_srd_bd_addr[0] >> 16;
    a_ManufData[sizeof(a_ManufData) - 2] = a_srd_bd_addr[0] >> 8;
    a_ManufData[sizeof(a_ManufData) - 1] = a_srd_bd_addr[0];

    ret = aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)a_srd_bd_addr);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET\n");
        APP_DBG_MSG("  Random Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                    (uint8_t)(a_srd_bd_addr[1] >> 8),
                    (uint8_t)(a_srd_bd_addr[1]),
                    (uint8_t)(a_srd_bd_addr[0] >> 24),
                    (uint8_t)(a_srd_bd_addr[0] >> 16),
                    (uint8_t)(a_srd_bd_addr[0] >> 8),
                    (uint8_t)(a_srd_bd_addr[0]));
    }
#endif /* CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR */

    /**
   * Write Identity root key used to derive IRK and DHK(Legacy)
   */
    ret = aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)a_BLE_CfgIrValue);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_IR_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_IR_OFFSET\n");
    }

    /**
   * Write Encryption root key used to derive LTK and CSRK
   */
    ret = aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)a_BLE_CfgErValue);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_ER_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_ER_OFFSET\n");
    }

    /**
   * Set TX Power.
   */
    ret = aci_hal_set_tx_power_level(1, CFG_TX_POWER);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_hal_set_tx_power_level command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_hal_set_tx_power_level command\n");
    }

    /**
   * Initialize GATT interface
   */
    ret = aci_gatt_init();
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_gatt_init command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_gatt_init command\n");
    }

    /**
   * Initialize GAP interface
   */
    role = 0;

#if (BLE_CFG_PERIPHERAL == 1)
    role |= GAP_PERIPHERAL_ROLE;
#endif /* BLE_CFG_PERIPHERAL == 1 */

#if (BLE_CFG_CENTRAL == 1)
    role |= GAP_CENTRAL_ROLE;
#endif /* BLE_CFG_CENTRAL == 1 */

    /* USER CODE BEGIN Role_Mngt*/

    /* USER CODE END Role_Mngt */

    if (role > 0)
    {
        auto&       ov_config = ov::config::get();
        const char* name      = ov_config.device_name;
        ret                   = aci_gap_init(
            role, CFG_PRIVACY, APPBLE_GAP_DEVICE_NAME_LENGTH, &gap_service_handle, &gap_dev_name_char_handle, &gap_appearance_char_handle);

        if (ret != BLE_STATUS_SUCCESS)
        {
            APP_DBG_MSG("  Fail   : aci_gap_init command, result: 0x%x \n", ret);
        }
        else
        {
            APP_DBG_MSG("  Success: aci_gap_init command\n");
        }

        ret = aci_gatt_update_char_value(gap_service_handle, gap_dev_name_char_handle, 0, strlen(name), (uint8_t*)name);
        if (ret != BLE_STATUS_SUCCESS)
        {
            BLE_DBG_SVCCTL_MSG("  Fail   : aci_gatt_update_char_value - Device Name\n");
        }
        else
        {
            BLE_DBG_SVCCTL_MSG("  Success: aci_gatt_update_char_value - Device Name\n");
        }
    }

    ret = aci_gatt_update_char_value(gap_service_handle, gap_appearance_char_handle, 0, 2, (uint8_t*)&a_appearance);
    if (ret != BLE_STATUS_SUCCESS)
    {
        BLE_DBG_SVCCTL_MSG("  Fail   : aci_gatt_update_char_value - Appearance\n");
    }
    else
    {
        BLE_DBG_SVCCTL_MSG("  Success: aci_gatt_update_char_value - Appearance\n");
    }

    /**
   * Initialize Default PHY
   */
    ret = hci_le_set_default_phy(ALL_PHYS_PREFERENCE, TX_2M_PREFERRED, RX_2M_PREFERRED);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : hci_le_set_default_phy command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: hci_le_set_default_phy command\n");
    }

    /**
   * Initialize IO capability
   */
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = CFG_IO_CAPABILITY;
    ret = aci_gap_set_io_capability(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_gap_set_io_capability command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_gap_set_io_capability command\n");
    }

    /**
   * Initialize authentication
   */
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode            = CFG_MITM_PROTECTION;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = CFG_ENCRYPTION_KEY_SIZE_MIN;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = CFG_ENCRYPTION_KEY_SIZE_MAX;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin        = CFG_USED_FIXED_PIN;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin            = CFG_FIXED_PIN;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode         = CFG_BONDING_MODE;
    /* USER CODE BEGIN Ble_Hci_Gap_Gatt_Init_1*/

    /* USER CODE END Ble_Hci_Gap_Gatt_Init_1*/

    ret = aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                                 CFG_SC_SUPPORT,
                                                 CFG_KEYPRESS_NOTIFICATION_SUPPORT,
                                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                                 CFG_IDENTITY_ADDRESS);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("  Fail   : aci_gap_set_authentication_requirement command, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("  Success: aci_gap_set_authentication_requirement command\n");
    }

    /**
   * Initialize whitelist
   */
    if (BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
    {
        ret = aci_gap_configure_whitelist();
        if (ret != BLE_STATUS_SUCCESS)
        {
            APP_DBG_MSG("  Fail   : aci_gap_configure_whitelist command, result: 0x%x \n", ret);
        }
        else
        {
            APP_DBG_MSG("  Success: aci_gap_configure_whitelist command\n");
        }
    }
    APP_DBG_MSG("==>> End Ble_Hci_Gap_Gatt_Init function\n\r");
}

static void Adv_Request(APP_BLE_ConnStatus_t NewStatus)
{
    tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
    uint16_t   Min_Inter, Max_Inter;

    if (NewStatus == APP_BLE_FAST_ADV)
    {
        Min_Inter = AdvIntervalMin;
        Max_Inter = AdvIntervalMax;
    }
    else
    {
        Min_Inter = CFG_LP_CONN_ADV_INTERVAL_MIN;
        Max_Inter = CFG_LP_CONN_ADV_INTERVAL_MAX;
    }

    /**
   * Stop the timer, it will be restarted for a new shot
   * It does not hurt if the timer was not running
   */
    //HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);
    advertising_timer.stop();

    if ((NewStatus == APP_BLE_LP_ADV) && ((BleApplicationContext.Device_Connection_Status == APP_BLE_FAST_ADV) ||
                                          (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_ADV)))
    {
        /* Connection in ADVERTISE mode have to stop the current advertising */
        ret = aci_gap_set_non_discoverable();
        if (ret != BLE_STATUS_SUCCESS)
        {
            APP_DBG_MSG("==>> aci_gap_set_non_discoverable - Stop Advertising Failed , result: %d \n", ret);
        }
        else
        {
            APP_DBG_MSG("==>> aci_gap_set_non_discoverable - Successfully Stopped Advertising \n");
        }
    }

    BleApplicationContext.Device_Connection_Status = NewStatus;
    /* Start Fast or Low Power Advertising */
    ret = aci_gap_set_discoverable(ADV_IND,
                                   Min_Inter,
                                   Max_Inter,
                                   CFG_BLE_ADDRESS_TYPE,
                                   NO_WHITE_LIST_USE, /* use white list */
                                   a_LocalName_size,
                                   (uint8_t*)&a_LocalName,
                                   BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
                                   BleApplicationContext.BleApplicationContext_legacy.advtServUUID,
                                   0,
                                   0);
    if (ret != BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("==>> aci_gap_set_discoverable - fail, result: 0x%x \n", ret);
    }
    else
    {
        APP_DBG_MSG("==>> aci_gap_set_discoverable - Success\n");
    }

    /* Update Advertising data */
    ret = aci_gap_update_adv_data(sizeof(a_ManufData), (uint8_t*)a_ManufData);
    if (ret != BLE_STATUS_SUCCESS)
    {
        if (NewStatus == APP_BLE_FAST_ADV)
        {
            APP_DBG_MSG("==>> Start Fast Advertising Failed , result: %d \n\r", ret);
        }
        else
        {
            APP_DBG_MSG("==>> Start Low Power Advertising Failed , result: %d \n\r", ret);
        }
    }
    else
    {
        if (NewStatus == APP_BLE_FAST_ADV)
        {
            APP_DBG_MSG("==>> Success: Start Fast Advertising \n\r");
            /* Start Timer to STOP ADV - TIMEOUT - and next Restart Low Power Advertising */
            //HW_TS_Start(BleApplicationContext.Advertising_mgr_timer_Id, INITIAL_ADV_TIMEOUT);
            advertising_timer.start(INITIAL_ADV_TIMEOUT, true);
        }
        else
        {
            APP_DBG_MSG("==>> Success: Start Low Power Advertising \n\r");
        }
    }

    return;
}

const uint8_t* BleGetBdAddress(void)
{
    uint8_t*       p_otp_addr;
    const uint8_t* p_bd_addr;
    uint32_t       udn;
    uint32_t       company_id;
    uint32_t       device_id;

    udn = LL_FLASH_GetUDN();

    if (udn != 0xFFFFFFFF)
    {
        company_id = LL_FLASH_GetSTCompanyID();
        device_id  = LL_FLASH_GetDeviceID();

        /**
     * Public Address with the ST company ID
     * bit[47:24] : 24bits (OUI) equal to the company ID
     * bit[23:16] : Device ID.
     * bit[15:0] : The last 16bits from the UDN
     * Note: In order to use the Public Address in a final product, a dedicated
     * 24bits company ID (OUI) shall be bought.
     */
        a_BdAddrUdn[0] = (uint8_t)(udn & 0x000000FF);
        a_BdAddrUdn[1] = (uint8_t)((udn & 0x0000FF00) >> 8);
        a_BdAddrUdn[2] = (uint8_t)device_id;
        a_BdAddrUdn[3] = (uint8_t)(company_id & 0x000000FF);
        a_BdAddrUdn[4] = (uint8_t)((company_id & 0x0000FF00) >> 8);
        a_BdAddrUdn[5] = (uint8_t)((company_id & 0x00FF0000) >> 16);

        p_bd_addr = (const uint8_t*)a_BdAddrUdn;
    }
    else
    {
        p_otp_addr = OTP_Read(0);
        if (p_otp_addr)
        {
            p_bd_addr = ((OTP_ID0_t*)p_otp_addr)->bd_address;
        }
        else
        {
            p_bd_addr = a_MBdAddr;
        }
    }

    return p_bd_addr;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTION */

/* USER CODE END FD_LOCAL_FUNCTION */

/*************************************************************
 *
 *SPECIFIC FUNCTIONS
 *
 *************************************************************/
static void Add_Advertisment_Service_UUID(uint16_t servUUID)
{
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
        (uint8_t)(servUUID & 0xFF);
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
        (uint8_t)(servUUID >> 8) & 0xFF;
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;

    return;
}

static void Adv_Mgr(void)
{
    /**
   * The code shall be executed in the background as an aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
    UTIL_SEQ_SetTask(1 << CFG_TASK_ADV_UPDATE_ID, CFG_SCH_PRIO_0);

    return;
}

static void Adv_Update(void)
{
    Adv_Request(APP_BLE_LP_ADV);

    return;
}

/* USER CODE BEGIN FD_SPECIFIC_FUNCTIONS */

/* USER CODE END FD_SPECIFIC_FUNCTIONS */
/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void hci_notify_asynch_evt(void* p_Data)
{
    UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);

    return;
}

void hci_cmd_resp_release(uint32_t Flag)
{
    (void)Flag;
    bool higher_priority_task_woken = false;
    hci_cmd_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
    //UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);

    return;
}

void hci_cmd_resp_wait(uint32_t Timeout)
{
    hci_cmd_sem.take(Timeout);
    //UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);

    return;
}

static void BLE_UserEvtRx(void* p_Payload)
{
    SVCCTL_UserEvtFlowStatus_t svctl_return_status;
    tHCI_UserEvtRxParam*       p_param;

    p_param = (tHCI_UserEvtRxParam*)p_Payload;

    svctl_return_status = SVCCTL_UserEvtRx((void*)&(p_param->pckt->evtserial));
    if (svctl_return_status != SVCCTL_UserEvtFlowDisable)
    {
        p_param->status = HCI_TL_UserEventFlow_Enable;
    }
    else
    {
        p_param->status = HCI_TL_UserEventFlow_Disable;
    }

    return;
}

static void BLE_StatusNot(HCI_TL_CmdStatus_t Status)
{
    uint32_t task_id_list;
    switch (Status)
    {
        case HCI_TL_CmdBusy:
            /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
            task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
            UTIL_SEQ_PauseTask(task_id_list);
            /* USER CODE BEGIN HCI_TL_CmdBusy */

            /* USER CODE END HCI_TL_CmdBusy */
            break;

        case HCI_TL_CmdAvailable:
            /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
            task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
            UTIL_SEQ_ResumeTask(task_id_list);
            /* USER CODE BEGIN HCI_TL_CmdAvailable */

            /* USER CODE END HCI_TL_CmdAvailable */
            break;

        default:
            /* USER CODE BEGIN Status */

            /* USER CODE END Status */
            break;
    }

    return;
}

void SVCCTL_ResumeUserEventFlow(void)
{
    hci_resume_flow();

    return;
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

bool APP_BLE_Is_Ready()
{
    return s_app_ble_is_ready;
}

void APPE_Tl_Init(void)
{
    TL_MM_Config_t        tl_mm_config;
    SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
    /**< Reference table initialization */
    TL_Init();

    /**< System channel initialization */
    UTIL_SEQ_RegTask(1 << CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, shci_user_evt_proc);
    SHci_Tl_Init_Conf.p_cmdbuffer       = (uint8_t*)&SystemCmdBuffer;
    SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
    shci_init(APPE_SysUserEvtRx, (void*)&SHci_Tl_Init_Conf);

    /**< Memory Manager channel initialization */
    tl_mm_config.p_BleSpareEvtBuffer    = BleSpareEvtBuffer;
    tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
    tl_mm_config.p_AsynchEvtPool        = EvtPool;
    tl_mm_config.AsynchEvtPoolSize      = POOL_SIZE;
    TL_MM_Init(&tl_mm_config);

    TL_Enable();

    return;
}

static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status)
{
    (void)status;
    return;
}

/**
 * The type of the payload for a system user event is tSHCI_UserEvtRxParam
 * When the system event is both :
 *    - a ready event (subevtcode = SHCI_SUB_EVT_CODE_READY)
 *    - reported by the FUS (sysevt_ready_rsp == FUS_FW_RUNNING)
 * The buffer shall not be released
 * (eg ((tSHCI_UserEvtRxParam*)pPayload)->status shall be set to SHCI_TL_UserEventFlow_Disable)
 * When the status is not filled, the buffer is released by default
 */
static void APPE_SysUserEvtRx(void* pPayload)
{
    TL_AsynchEvt_t*  p_sys_event;
    WirelessFwInfo_t WirelessInfo;

    p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);

    switch (p_sys_event->subevtcode)
    {
        case SHCI_SUB_EVT_CODE_READY:
            /* Read the firmware version of both the wireless firmware and the FUS */
            SHCI_GetWirelessFwInfo(&WirelessInfo);
            APP_DBG_MSG(
                "Wireless Firmware version %d.%d.%d\n", WirelessInfo.VersionMajor, WirelessInfo.VersionMinor, WirelessInfo.VersionSub);
            APP_DBG_MSG("Wireless Firmware build %d\n", WirelessInfo.VersionReleaseType);
            APP_DBG_MSG("FUS version %d.%d.%d\n", WirelessInfo.FusVersionMajor, WirelessInfo.FusVersionMinor, WirelessInfo.FusVersionSub);

            APP_DBG_MSG(">>== SHCI_SUB_EVT_CODE_READY\n\r");
            APPE_SysEvtReadyProcessing(pPayload);
            break;

        case SHCI_SUB_EVT_ERROR_NOTIF:
            APP_DBG_MSG(">>== SHCI_SUB_EVT_ERROR_NOTIF \n\r");
            APPE_SysEvtError(pPayload);
            break;

        case SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE:
            APP_DBG_MSG(">>== SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE -- BLE NVM RAM HAS BEEN UPDATED BY CPU2 \n");
            APP_DBG_MSG("     - StartAddress = %lx , Size = %ld\n",
                        ((SHCI_C2_BleNvmRamUpdate_Evt_t*)p_sys_event->payload)->StartAddress,
                        ((SHCI_C2_BleNvmRamUpdate_Evt_t*)p_sys_event->payload)->Size);
            break;

        case SHCI_SUB_EVT_NVM_START_WRITE:
            APP_DBG_MSG("==>> SHCI_SUB_EVT_NVM_START_WRITE : NumberOfWords = %ld\n",
                        ((SHCI_C2_NvmStartWrite_Evt_t*)p_sys_event->payload)->NumberOfWords);
            break;

        case SHCI_SUB_EVT_NVM_END_WRITE:
            APP_DBG_MSG(">>== SHCI_SUB_EVT_NVM_END_WRITE\n\r");
            break;

        case SHCI_SUB_EVT_NVM_START_ERASE:
            APP_DBG_MSG("==>>SHCI_SUB_EVT_NVM_START_ERASE : NumberOfSectors = %ld\n",
                        ((SHCI_C2_NvmStartErase_Evt_t*)p_sys_event->payload)->NumberOfSectors);
            break;

        case SHCI_SUB_EVT_NVM_END_ERASE:
            APP_DBG_MSG(">>== SHCI_SUB_EVT_NVM_END_ERASE\n\r");
            break;

        default:
            break;
    }

    return;
}

/**
 * @brief Notify a system error coming from the M0 firmware
 * @param  ErrorCode  : errorCode detected by the M0 firmware
 *
 * @retval None
 */
static void APPE_SysEvtError(void* pPayload)
{
    TL_AsynchEvt_t*       p_sys_event;
    SCHI_SystemErrCode_t* p_sys_error_code;

    p_sys_event      = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
    p_sys_error_code = (SCHI_SystemErrCode_t*)p_sys_event->payload;

    APP_DBG_MSG(">>== SHCI_SUB_EVT_ERROR_NOTIF WITH REASON %x \n\r", (*p_sys_error_code));

    if ((*p_sys_error_code) == ERR_BLE_INIT)
    {
        /* Error during BLE stack initialization */
        APP_DBG_MSG(">>== SHCI_SUB_EVT_ERROR_NOTIF WITH REASON - ERR_BLE_INIT \n");
    }
    else
    {
        APP_DBG_MSG(">>== SHCI_SUB_EVT_ERROR_NOTIF WITH REASON - BLE ERROR \n");
    }
    return;
}

static void APPE_SysEvtReadyProcessing(void* pPayload)
{
    TL_AsynchEvt_t*      p_sys_event;
    SHCI_C2_Ready_Evt_t* p_sys_ready_event;

    SHCI_C2_CONFIG_Cmd_Param_t config_param = {};
    uint32_t                   RevisionID   = 0;
    uint32_t                   DeviceID     = 0;

    p_sys_event       = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
    p_sys_ready_event = (SHCI_C2_Ready_Evt_t*)p_sys_event->payload;

    if (p_sys_ready_event->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
    {
        /**
    * The wireless firmware is running on the CPU2
    */
        APP_DBG_MSG(">>== WIRELESS_FW_RUNNING \n");

        /* Traces channel initialization */
        //APPD_EnableCPU2();

        /* Enable all events Notification */
        config_param.PayloadCmdSize = SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE;
        config_param.EvtMask1 = SHCI_C2_CONFIG_EVTMASK1_BIT0_ERROR_NOTIF_ENABLE + SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE +
                                SHCI_C2_CONFIG_EVTMASK1_BIT2_THREAD_NVM_RAM_UPDATE_ENABLE +
                                SHCI_C2_CONFIG_EVTMASK1_BIT3_NVM_START_WRITE_ENABLE + SHCI_C2_CONFIG_EVTMASK1_BIT4_NVM_END_WRITE_ENABLE +
                                SHCI_C2_CONFIG_EVTMASK1_BIT5_NVM_START_ERASE_ENABLE + SHCI_C2_CONFIG_EVTMASK1_BIT6_NVM_END_ERASE_ENABLE;

        /* Read revision identifier */
        /**
    * @brief  Return the device revision identifier
    * @note   This field indicates the revision of the device.
    * @rmtoll DBGMCU_IDCODE REV_ID        LL_DBGMCU_GetRevisionID
    * @retval Values between Min_Data=0x00 and Max_Data=0xFFFF
    */
        RevisionID = LL_DBGMCU_GetRevisionID();

        APP_DBG_MSG(">>== DBGMCU_GetRevisionID= %lx \n\r", RevisionID);

        config_param.RevisionID = (uint16_t)RevisionID;

        DeviceID = LL_DBGMCU_GetDeviceID();
        APP_DBG_MSG(">>== DBGMCU_GetDeviceID= %lx \n\r", DeviceID);
        config_param.DeviceID = (uint16_t)DeviceID;
        (void)SHCI_C2_Config(&config_param);

        APP_BLE_Init();
        //UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
    }
    else if (p_sys_ready_event->sysevt_ready_rsp == FUS_FW_RUNNING)
    {
        /**
    * The FUS firmware is running on the CPU2
    * In the scope of this application, there should be no case when we get here
    */
        APP_DBG_MSG(">>== SHCI_SUB_EVT_CODE_READY - FUS_FW_RUNNING \n\r");

        /* The packet shall not be released as this is not supported by the FUS */
        ((tSHCI_UserEvtRxParam*)pPayload)->status = SHCI_TL_UserEventFlow_Disable;
    }
    else
    {
        APP_DBG_MSG(">>== SHCI_SUB_EVT_CODE_READY - UNEXPECTED CASE \n\r");
    }

    return;
}

void shci_notify_asynch_evt(void* pdata)
{
    UTIL_SEQ_SetTask(1 << CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
    return;
}

void shci_cmd_resp_release(uint32_t flag)
{
    (void)flag;
    bool higher_priority_task_woken = false;
    shci_cmd_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
    // UTIL_SEQ_SetEvt(1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
    return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
    shci_cmd_sem.take(timeout);
    // UTIL_SEQ_WaitEvt(1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
    return;
}

/* USER CODE END FD_WRAP_FUNCTIONS */
