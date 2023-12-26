
#include "ssd1315.h"
#include "thread.h"

#include <cstring>

/** @brief  SSD1315 Scrolling */
#define SSD1315_SCROLL_RIGHT 0x26
#define SSD1315_SCROLL_LEFT 0x27
#define SSD1315_ACTIVATE_SCROLL 0x2F
#define SSD1315_DESACTIVATE_SCROLL 0x2E

#define SSD1315_SCROLL_FREQ_2FRAMES 0x07
#define SSD1315_SCROLL_FREQ_3FRAMES 0x04
#define SSD1315_SCROLL_FREQ_4FRAMES 0x05
#define SSD1315_SCROLL_FREQ_5FRAMES 0x00
#define SSD1315_SCROLL_FREQ_25FRAMES 0x06
#define SSD1315_SCROLL_FREQ_64FRAMES 0x01
#define SSD1315_SCROLL_FREQ_128FRAMES 0x02
#define SSD1315_SCROLL_FREQ_256FRAMES 0x03

/** @brief  SSD1315 Commands */
#define SSD1315_CHARGE_PUMP_SETTING 0x8D
#define SSD1315_READWRITE_CMD 0x80
#define SSD1315_HIGHER_COLUMN_START_ADRESS_1 0x10
#define SSD1315_HIGHER_COLUMN_START_ADRESS_2 0x11
#define SSD1315_HIGHER_COLUMN_START_ADRESS_3 0x12
#define SSD1315_HIGHER_COLUMN_START_ADRESS_4 0x13
#define SSD1315_HIGHER_COLUMN_START_ADRESS_5 0x14
#define SSD1315_HIGHER_COLUMN_START_ADRESS_6 0x15
#define SSD1315_HIGHER_COLUMN_START_ADRESS_7 0x16
#define SSD1315_HIGHER_COLUMN_START_ADRESS_8 0x17
#define SSD1315_LOWER_COLUMN_START_ADRESS 0x00
#define SSD1315_LOWER_COLUMN_START_ADRESS_15 0x0F
#define SSD1315_MEMORY_ADRESS_MODE 0x20
#define SSD1315_SET_COLUMN_ADRESS 0x21
#define SSD1315_SET_PAGE_ADRESS 0x22
#define SSD1315_DISPLAY_START_LINE_1 0x40
#define SSD1315_DISPLAY_START_LINE_32 0x1F
#define SSD1315_DISPLAY_START_LINE_64 0x7F
#define SSD1315_REMAPPED_MODE 0xC8
#define SSD1315_CONTRAST_CONTROL 0xA1
#define SSD1315_CONTRAST_CONTROL_2 0xFF
#define SSD1315_DISPLAY_ON 0xAF
#define SSD1315_DISPLAY_OFF 0xAE
#define SSD1315_SET_PAGE_START_ADRESS 0xB0

namespace ov
{

/** @brief Constructor */
ssd1315::ssd1315(i_spi& spi_drv, uint8_t spi_cs_line, i_output_pin& reset_pin, i_output_pin& data_pin)
    : m_spi_drv(spi_drv), m_spi_cs_line(spi_cs_line), m_reset_pin(reset_pin), m_data_pin(data_pin)
{
}

/** @brief Initialize the display */
bool ssd1315::init()
{
    // Clear frame buffer to black
    memset(m_frame_buffer, 0xFF, sizeof(m_frame_buffer));

    // Release data/control line
    m_data_pin.set_low();

    // Reset device
    bool ret = reset();

    // Initialize display
    ret = send_command(SSD1315_READWRITE_CMD) && ret;
    ret = send_command(SSD1315_CHARGE_PUMP_SETTING) && ret;
    ret = send_command(SSD1315_HIGHER_COLUMN_START_ADRESS_5) && ret;
    ret = send_command(SSD1315_MEMORY_ADRESS_MODE) && ret;
    ret = send_command(SSD1315_LOWER_COLUMN_START_ADRESS) && ret;
    ret = send_command(SSD1315_DISPLAY_START_LINE_1) && ret;
    ret = send_command(SSD1315_REMAPPED_MODE) && ret;
    ret = send_command(SSD1315_CONTRAST_CONTROL) && ret;
    ret = send_command(SSD1315_DISPLAY_ON) && ret;

    // Refresh contents
    ret = refresh() && ret;

    return ret;
}

/** @brief Reset the display */
bool ssd1315::reset()
{
    m_reset_pin.set_low();
    ov::this_thread::sleep_for(10u);
    m_reset_pin.set_high();
    ov::this_thread::sleep_for(100u);

    return true;
}

/** @brief Turn ON the display */
bool ssd1315::turn_on()
{
    bool ret;

    ret = send_command(SSD1315_CHARGE_PUMP_SETTING);
    ret = send_command(SSD1315_HIGHER_COLUMN_START_ADRESS_5) && ret;
    ret = send_command(SSD1315_DISPLAY_ON) && ret;

    return ret;
}

/** @brief Turn OFF the display */
bool ssd1315::turn_off()
{
    bool ret;

    ret = send_command(SSD1315_CHARGE_PUMP_SETTING);
    ret = send_command(SSD1315_HIGHER_COLUMN_START_ADRESS_1) && ret;
    ret = send_command(SSD1315_DISPLAY_OFF) && ret;

    return ret;
}

/** @brief Refresh the display contents */
bool ssd1315::refresh()
{
    bool ret = false;

    ret = send_command(SSD1315_DISPLAY_START_LINE_1);
    ret = send_command(SSD1315_SET_COLUMN_ADRESS) && ret;
    ret = send_command(SSD1315_LOWER_COLUMN_START_ADRESS) && ret;
    ret = send_command(SSD1315_DISPLAY_START_LINE_64) && ret;
    ret = send_command(SSD1315_SET_PAGE_ADRESS) && ret;
    ret = send_command(SSD1315_LOWER_COLUMN_START_ADRESS) && ret;
    ret = send_command(SSD1315_LOWER_COLUMN_START_ADRESS_15) && ret;
    ret = write_frame_buffer() && ret;

    return ret;
}

/** @brief Send a command to the display */
bool ssd1315::send_command(uint8_t cmd)
{
    i_spi::xfer_desc xfer;
    xfer.cs         = m_spi_cs_line;
    xfer.write_data = &cmd;
    xfer.size       = 1u;

    bool ret = m_spi_drv.xfer(xfer);
    return ret;
}

/** @brief Write the frame buffer contents to the display */
bool ssd1315::write_frame_buffer()
{
    m_data_pin.set_high();

    i_spi::xfer_desc xfer;
    xfer.cs         = m_spi_cs_line;
    xfer.write_data = m_frame_buffer;
    xfer.size       = sizeof(m_frame_buffer);

    bool ret = m_spi_drv.xfer(xfer);

    m_data_pin.set_low();

    return ret;
}

} // namespace ov
