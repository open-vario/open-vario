
#ifndef OV_FLIGHT_FILE_H
#define OV_FLIGHT_FILE_H

#include "date_time.h"
#include "file.h"
#include "i_barometric_altimeter.h"
#include "i_gnss.h"

namespace ov
{

/** @brief Flight file */
class flight_file
{
  public:
    /** @brief Header */
    struct header
    {
        /** @brief Timestamp */
        date_time timestamp;
        /** @brief Glider */
        char glider[32u];
        /** @brief Entry period in milliseconds */
        uint16_t period;

        /** @brief Magic number value */
        static constexpr uint32_t MAGIC_NUMBER = 0xBEEFF00Du;
    };

    /** @brief Flight file entry */
    struct entry
    {
        /** @brief GNSS data */
        i_gnss::data gnss;
        /** @brief Barometric altimeter data */
        i_barometric_altimeter::data altimeter;
    };

    /** @brief Constructor to open the file for read operations */
    flight_file(const char* path);

    /** @brief Constructor to open the file for write operations */
    flight_file(const char* path, header& header);

    /** @brief Close the file */
    bool close();

    /** @brief Indicate if the file is valid */
    bool is_open() const { return m_file.is_open(); }

    /** @brief Get the file header */
    const header& get_header() const { return m_header; }

    /** @brief Write a flight entry to the file */
    bool write(const entry& e);

    /** @brief Read a flight entry from the file */
    bool read(entry& e);

  protected:
    /** @brief Header */
    header m_header;
    /** @brief File handle */
    file m_file;
};

} // namespace ov

#endif // OV_FLIGHT_FILE_H
