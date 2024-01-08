
#include "sensors_console.h"
#include "ov_data.h"

#include <cstdio>

namespace ov
{

/** @brief Constructor */
sensors_console::sensors_console(i_debug_console& console)
    : m_console(console),
      m_gnss_handler{"gnss",
                     "Display the GNSS data",
                     ov::handler_func::create<sensors_console, &sensors_console::gnss_handler>(*this),
                     nullptr,
                     false},
      m_alti_handler{"alti",
                     "Display the barometric altimeter data",
                     ov::handler_func::create<sensors_console, &sensors_console::alti_handler>(*this),
                     nullptr,
                     false}
{
}

/** @brief Register command handlers */
void sensors_console::register_handlers()
{
    m_console.register_handler(m_gnss_handler);
    m_console.register_handler(m_alti_handler);
}

/** @brief Handler for the 'gnss' command */
void sensors_console::gnss_handler(const char*)
{
    m_console.write_line("Periodic display of GNSS data");

    auto handler = ov::periodic_handler_func::create<sensors_console, &sensors_console::display_gnss_data>(*this);
    m_console.start_periodic(handler, 1000u);
}

/** @brief Handler for the 'alti' command */
void sensors_console::alti_handler(const char*)
{
    m_console.write_line("Periodic display of barometric altimeter data");

    auto handler = ov::periodic_handler_func::create<sensors_console, &sensors_console::display_alti_data>(*this);
    m_console.start_periodic(handler, 250u);
}

/** @brief Display gnss data */
void sensors_console::display_gnss_data()
{
    char     tmp[32u] = {};
    uint32_t deg      = 0;
    uint32_t min      = 0;
    uint32_t sec      = 0;
    char     ref      = 0;

    auto gnss = ov::data::get_gnss();

    m_console.write_line("------------------------------");
    if (gnss.is_valid)
    {
        m_console.write("Sattelites : ");
        snprintf(tmp, sizeof(tmp), "%d", gnss.satellite_count);
        m_console.write_line(tmp);

        m_console.write("Date/time : ");
        snprintf(tmp,
                 sizeof(tmp),
                 "%04d-%02d-%02dT%02d:%02d:%02dZ",
                 static_cast<int>(gnss.date.year) + 2000,
                 static_cast<int>(gnss.date.month),
                 static_cast<int>(gnss.date.day),
                 static_cast<int>(gnss.date.hour),
                 static_cast<int>(gnss.date.minute),
                 static_cast<int>(gnss.date.second));
        m_console.write_line(tmp);

        m_console.write("Latitude : ");
        if (gnss.latitude >= 0)
        {
            ref = 'N';
        }
        else
        {
            ref = 'S';
            gnss.latitude *= -1.;
        }
        i_gnss::to_dms(gnss.latitude, deg, min, sec);
        snprintf(tmp, sizeof(tmp), "%02ld.%02ld'%02ld''%c", deg, min, sec, ref);
        m_console.write_line(tmp);

        m_console.write("Longitude : ");
        if (gnss.longitude >= 0)
        {
            ref = 'E';
        }
        else
        {
            ref = 'W';
            gnss.longitude *= -1.;
        }
        i_gnss::to_dms(gnss.longitude, deg, min, sec);
        snprintf(tmp, sizeof(tmp), "%02ld.%02ld'%02ld''%c", deg, min, sec, ref);
        m_console.write_line(tmp);

        m_console.write("Speed : ");
        snprintf(tmp, sizeof(tmp), "%ld", gnss.speed);
        m_console.write_line(tmp);

        m_console.write("Altitude : ");
        snprintf(tmp, sizeof(tmp), "%ld", gnss.altitude);
        m_console.write_line(tmp);

        m_console.write("Track angle : ");
        snprintf(tmp, sizeof(tmp), "%d", gnss.track_angle);
        m_console.write_line(tmp);
    }
    else
    {
        m_console.write_line("Invalid data");
    }
}

/** @brief Display alti data */
void sensors_console::display_alti_data()
{
    char tmp[32u] = {};

    auto altimeter = ov::data::get_altimeter();

    m_console.write_line("------------------------------");
    if (altimeter.is_valid)
    {
        m_console.write("Pressure : ");
        snprintf(tmp, sizeof(tmp), "%ld", altimeter.pressure);
        m_console.write_line(tmp);

        m_console.write("Altitude : ");
        snprintf(tmp, sizeof(tmp), "%ld", altimeter.altitude);
        m_console.write_line(tmp);

        m_console.write("Temperature : ");
        snprintf(tmp, sizeof(tmp), "%d", altimeter.temperature);
        m_console.write_line(tmp);
    }
    else
    {
        m_console.write_line("Invalid data");
    }
}

} // namespace ov
