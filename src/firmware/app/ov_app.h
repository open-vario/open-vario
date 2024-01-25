
#ifndef OV_APP_H
#define OV_APP_H

#include "ble_manager.h"
#include "config_console.h"
#include "debug_console.h"
#include "flight_recorder.h"
#include "fs_console.h"
#include "hmi_manager.h"
#include "maintenance_manager.h"
#include "ov_board.h"
#include "recorder_console.h"
#include "sensors_console.h"
#include "thread.h"
#include "xctrack_link.h"

namespace ov
{

/** @brief Open Vario application */
class ov_app
{
  public:
    /** @brief Constructor */
    ov_app();

    /** @brief Initialize application */
    bool init();

    /** @brief Start the application (shall not return in case of sucess) */
    void start();

  private:
    /** @brief Board */
    ov_board m_board;
    /** @brief Debug console */
    debug_console m_console;
    /** @brief Filesystem console commands */
    fs_console m_fs_console;
    /** @brief Configuration console commands */
    config_console m_config_console;
    /** @brief Sensors console commands */
    sensors_console m_sensors_console;
    /** @brief Flight recorder console */
    recorder_console m_recorder_console;
    /** @brief HMI manager */
    hmi_manager m_hmi;
    /** @brief BLE */
    ble_manager m_ble;
    /** @brief Flight recorder */
    flight_recorder m_recorder;
    /** @brief XCTrack link */
    xctrack_link m_xctrack;
    /** @brief Maintenance manager */
    maintenance_manager m_maintenance;
    /** @brief Main thread */
    thread<2048u> m_thread;

    /** @brief Main thread */
    void thread_func(void*);

    /** @brief Startup process */
    void startup();
};

} // namespace ov

#endif // OV_APP_H
