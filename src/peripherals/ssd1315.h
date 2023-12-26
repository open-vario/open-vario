
#ifndef OV_SSD1315_H
#define OV_SSD1315_H

#include "i_display.h"
#include "i_output_pin.h"
#include "i_spi.h"

namespace ov
{

/** @brief SSD1315 OLED display */
class ssd1315 : public i_display
{
  public:
    /** @brief Constructor */
    ssd1315(i_spi& spi_drv, uint8_t spi_cs_line, i_output_pin& reset_pin, i_output_pin& data_pin);

    /** @brief Get the display width in pixels */
    size_t get_width() override { return 128u; }

    /** @brief Get the display heigth in pixels */
    size_t get_heigth() override { return 64u; }

    /** @brief Get the frame buffer of the display */
    uint8_t* get_frame_buffer() { return m_frame_buffer; }

    /** @brief Initialize the display */
    bool init();

    /** @brief Reset the display */
    bool reset() override;

    /** @brief Turn ON the display */
    bool turn_on() override;

    /** @brief Turn OFF the display */
    bool turn_off() override;

    /** @brief Refresh the display contents */
    bool refresh() override;

  private:
    /** @brief SPI driver */
    i_spi& m_spi_drv;
    /** @brief SPI chip select line */
    const uint8_t m_spi_cs_line;
    /** @brief Reset pin */
    i_output_pin& m_reset_pin;
    /** @brief Data/control pin */
    i_output_pin& m_data_pin;

    /** @brief Frame buffer */
    uint8_t m_frame_buffer[128u * 8u];

    /** @brief Send a command to the display */
    bool send_command(uint8_t cmd);

    /** @brief Write the frame buffer contents to the display */
    bool write_frame_buffer();
};

} // namespace ov

#endif // OV_SSD1315_H
