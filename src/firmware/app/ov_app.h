
#ifndef OV_APP_H
#define OV_APP_H

#include "debug_console.h"
#include "fs_console.h"
#include "hmi_manager.h"
#include "ov_board.h"
#include "thread.h"

namespace ov
{

/** @brief Open Vario application */
class ov_app
{
  public:
    /** @brief Constructor */
    ov_app();

    /** @brief Start the application (shall not return in case of sucess) */
    void start();

  private:
    /** @brief Board */
    ov_board m_board;
    /** @brief Debug console */
    debug_console m_console;
    /** @brief Filesystem console commands */
    fs_console m_fs_console;
    /** @brief HMI manager */
    hmi_manager m_hmi;
    /** @brief Main thread */
    thread<2048u> m_thread;

    /** @brief Main thread */
    void thread_func(void*);

    /** @brief Init process */
    void init();
};

} // namespace ov

#endif // OV_APP_H
