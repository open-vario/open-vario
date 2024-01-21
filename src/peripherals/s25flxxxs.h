
#ifndef OV_S25FLXXXS_H
#define OV_S25FLXXXS_H

#include "i_qspi.h"
#include "i_storage_memory.h"

namespace ov
{

/** @brief S25FLXXXS Quad SPI memory */
class s25flxxxs : public i_storage_memory
{
  public:
    /** @brief Memory reference */
    enum class ref : int
    {
        /** @brief S25FL128S */
        s25fl128s,
        /** @brief S25FL256S */
        s25fl256s
    };

    /** @brief Constructor */
    s25flxxxs(ref reference, i_qspi& qspi);
    /** @brief Destructor */
    virtual ~s25flxxxs() = default;

    /** @brief Get the memory size in bytes */
    size_t get_size() override;

    /** @brief Get the erase block size in bytes */
    size_t get_block_size() override;

    /** @brief Reset the memory */
    bool reset() override;

    /** @brief Read data from the memory */
    bool read(size_t address, void* buffer, size_t size) override;

    /** @brief Write data to the memory */
    bool write(size_t address, const void* buffer, size_t size) override;

    /** @brief Erase a block */
    bool erase(size_t block) override;

  private:
    /** @brief Reference */
    const ref m_reference;
    /** @brief QSPI driver */
    i_qspi& m_qspi;

    /** @brief Wait for the memory to be ready for another command */
    bool wait_ready();

    /** @brief Put the memory in write mode */
    bool enable_write();

    /** @brief Skip the first 128kBytes which are only 4kBytes sectors */
    static constexpr size_t MEMSKIP_OFFSET = 0x20000u;
};

} // namespace ov

#endif // OV_S25FLXXXS_H
