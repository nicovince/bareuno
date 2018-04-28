#ifndef __SLIP_H__
#define __SLIP_H__
#include <stdint.h>
#include <stddef.h>
typedef int16_t ssize_t;
typedef enum slip_state_e {
    SLIP_WAIT_END,
    SLIP_ESCAPING,
    SLIP_STORE_INCOMING
} slip_state_t;

typedef struct {
    slip_state_t state;
    uint8_t idx;
    uint8_t buf_size;
    uint8_t *buf;
} slip_decoder_t;

/**
 * @brief Decode byte into slip frame
 *
 * @param slip_handle Handle to slip decoder
 * @param b Byte to decode
 *
 * @return Number of byte available in slip payload once finished decoding,
 * -1 when the frame is not finished
 */
ssize_t slip_decode(slip_decoder_t *slip_handle, uint8_t b);
/**
 * @brief Initialize slip decoder
 *
 * @param slip_handle Handler to slip decoder
 * @param buf Buffer where slip payload is stored
 * @param buf_size Size of buffer
 */
void init_slip_decoder(slip_decoder_t *slip_handle, uint8_t *buf, uint8_t buf_size);

/**
 * @brief Reinitiliaze slip decoder, this function must be called after decode
 * has successfully decoded a slip frame
 *
 * @param slip_handle Handle to slip decoder
 */
void reset_slip_decoder(slip_decoder_t *slip_handle);
#endif // __SLIP_H__
