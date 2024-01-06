
#ifndef OV_APP_H
#define OV_APP_H

#include "ble_manager.h"
#include "config_console.h"
#include "debug_console.h"
#include "fs_console.h"
#include "hmi_manager.h"
#include "ov_board.h"
#include "sensors_console.h"
#include "thread.h"

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
    /** @brief HMI manager */
    hmi_manager m_hmi;
    /** @brief BLE */
    ble_manager m_ble;
    /** @brief Main thread */
    thread<2048u> m_thread;

    /** @brief Main thread */
    void thread_func(void*);

    /** @brief Startup process */
    void startup();
};

} // namespace ov

#endif // OV_APP_H
