
#ifndef OV_XCTRACK_LINK_H
#define OV_XCTRACK_LINK_H

#include "i_xctrack_link.h"
#include "thread.h"

namespace ov
{

/** @brief Handle the link with the XCTrack application */
class xctrack_link : public i_xctrack_link
{
  public:
    /** @brief Constructor */
    xctrack_link(i_usb_cdc& usb);

    /** @brief Initialize the link */
    bool init();

    /** @brief Get the USB CDC link */
    i_usb_cdc& get_usb() override { return m_usb; }

    /** @brief Indicate if the link is active */
    bool is_active() const override { return m_is_active; }

    /** @brief Change the active status of the link */
    void set_active(bool is_active) override { m_is_active = is_active; }

  protected:
    /** @brief USB CDC link */
    i_usb_cdc& m_usb;
    /** @brief Indicate if the link is active */
    bool m_is_active;
    /** @brief XCTrack thread */
    thread<2048u> m_thread;

    /** @brief XCTrack thread */
    void thread_func(void*);
    /** @brief Send barometric data */
    void send_barometric_data();
};

} // namespace ov

#endif // OV_XCTRACK_LINK_H
