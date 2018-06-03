#ifndef __SLIP_H__
#define __SLIP_H__
#include <stdint.h>
#include <stddef.h>

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

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
int16_t slip_decode(slip_decoder_t *slip_handle, uint8_t b);
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

/**
 * @brief Encode buffer for slip protocol
 *
 * @param src Payload to encode
 * @param dst Buffer where encoded data is stored (must be roughly twice src size)
 * @param len Length of payload to encode
 *
 * @return Size of encoded data
 */
int16_t slip_encode(const uint8_t *src, uint8_t *dst, int16_t len);

/**
 * @brief Encode a single byte for slip protocol
 *
 * @param in Byte to encode
 * @param dst[2] Buffer where encoded byte is stored
 *
 * @return Size of encoded data (1 or 2)
 */
uint8_t slip_encode_byte(const uint8_t in, uint8_t dst[2]);
#endif // __SLIP_H__
