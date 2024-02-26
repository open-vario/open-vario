/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_BLE_H
#define APP_BLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"

    /* Private includes ----------------------------------------------------------*/

    /* Exported types ------------------------------------------------------------*/

    typedef enum
    {
        APP_BLE_IDLE,
        APP_BLE_FAST_ADV,
        APP_BLE_LP_ADV,
        APP_BLE_SCAN,
        APP_BLE_LP_CONNECTING,
        APP_BLE_CONNECTED_SERVER,
        APP_BLE_CONNECTED_CLIENT
    } APP_BLE_ConnStatus_t;

    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* External variables --------------------------------------------------------*/
    /* USER CODE BEGIN EV */

    /* USER CODE END EV */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions ---------------------------------------------*/
    void                 APPE_Tl_Init(void);
    APP_BLE_ConnStatus_t APP_BLE_Get_Server_Connection_Status(void);
    bool                 APP_BLE_Is_Ready();

    /* USER CODE BEGIN EF */

    /* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*APP_BLE_H */
