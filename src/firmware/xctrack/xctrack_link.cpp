
#include "xctrack_link.h"
#include "os.h"
#include "ov_data.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
xctrack_link::xctrack_link(i_usb_cdc& usb) : m_usb(usb), m_is_active(true), m_thread() { }

/** @brief Initialize the maintenance */
bool xctrack_link::init()
{
    // Start XCTrack thread
    auto thread_func = ov::thread_func::create<xctrack_link, &xctrack_link::thread_func>(*this);
    bool ret         = m_thread.start(thread_func, "XCTrack", 6u, nullptr);

    return ret;
}

/** @brief Maintenance thread */
void xctrack_link::thread_func(void*)
{
    // Thread loop
    while (true)
    {
        // Check active state
        if (m_is_active)
        {
            // Check USB link status
            if (m_usb.is_link_up())
            {
                // Send data to XCTrack
                send_barometric_data();
            }
        }

        // Periodicity
        ov::this_thread::sleep_for(100u);
    }
}

/** @brief Send barometric data */
void xctrack_link::send_barometric_data()
{
    // Use LK8000 protocol => https://github.com/LK8000/LK8000/blob/master/Docs/LK8EX1.txt
    // $LK8EX1,pressure,altitude,vario,temperature,battery,*checksum
    // $LK8EX1,pressure,99999,9999,temp,999,*checksum
    static const char* lk8000_frame_format = "$LK8EX1,%d,99999,9999,%d,999,";

    // Get barometric data
    auto baro_data = ov::data::get_altimeter();
    if (!baro_data.is_valid)
    {
        baro_data.pressure    = 999999;
        baro_data.temperature = 999;
    }

    // Prepare frame
    char lk8000_frame[40u];
    int  lk8000_frame_size =
        snprintf(lk8000_frame, sizeof(lk8000_frame), lk8000_frame_format, baro_data.pressure, (baro_data.temperature / 10));

    // Compute checksum
    char checksum = 0u;
    for (int i = 1; i < lk8000_frame_size; i++)
    {
        checksum ^= lk8000_frame[i];
    }
    char checksum_str[6u];
    snprintf(checksum_str, sizeof(checksum_str), "*%X\r\n", static_cast<int>(checksum));
    strcat(lk8000_frame, checksum_str);

    // Send frame
    m_usb.write(lk8000_frame);
}

} // namespace ov
