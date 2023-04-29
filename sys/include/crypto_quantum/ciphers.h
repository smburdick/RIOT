/**
 * @ingroup     sys_crypto_quantum
 * @{
 *
 * @file
 * @brief       Headers for the packet encryption class. They are used to encrypt single packets.
 *
 */

#ifndef CRYPTO_CIPHERS_H
#define CRYPTO_CIPHERS_H

#include <stdint.h>
#include "modules.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Shared header file for all quantum resilitent cipher algorithms */

    /* return codes */

#define CIPHER_ERR_INVALID_LENGTH -4
#define CIPHER_ERR_ENC_FAILED -5
#define CIPHER_ERR_DEC_FAILED -6
/** Is returned by the cipher_init functions, if the corresponding algorithm
 * has not been included in the build */
#define CIPHER_ERR_BAD_CONTEXT_SIZE 0
#define CIPHER_INIT_SUCCESS 1

    /**
     * @brief   the context for qrcipher-operations
     */
    typedef struct
    {
        uint8_t key_size;                         /**< key size used */
    } quantum_cipher_context_t;

    /**
     * @brief   Interface for the QRCipher-Algorithms
     */
    typedef struct quantum_cipher_interface_st
    {

        /**
         * @brief the init function.
         *
         * This function is responsible for checking that the given key_size is
         * valid for the chosen cipher.
         */
        int (*init)(quantum_cipher_context_t *ctx, const uint8_t *key, uint8_t key_size);

        /** @brief the encrypt function */
        int (*encrypt)(const quantum_cipher_context_t *ctx, const uint8_t *plain_block,
                       uint8_t *cipher_block);

        /** @brief the decrypt function */
        int (*decrypt)(const quantum_cipher_context_t *ctx, const uint8_t *cipher_block,
                       uint8_t *plain_block);
    } quantum_cipher_interface_t;

    /** Pointer type to QRCipher-Interface for the QRCipher-Algorithms */
    typedef const quantum_cipher_interface_t *quantum_cipher_id_t;

    /**
     * @brief basic struct for using quantum resilient ciphers
     *        contains the cipher interface and the context
     */
    typedef struct
    {
        const quantum_cipher_interface_t *interface;
        quantum_cipher_context_t context;            /**< The encryption context (buffer)
                                                  for the algorithm */
    } quantum_cipher_t;

    /**
     * @brief Initialize new cipher state
     *
     * @param cipher     cipher struct to init (already allocated memory)
     * @param cipher_id  cipher algorithm id
     * @param key        encryption key to use
     * @param key_size   length of the encryption key
     *
     * @return  CIPHER_INIT_SUCCESS if the initialization was successful.
     * @return  CIPHER_ERR_BAD_CONTEXT_SIZE if CIPHER_MAX_CONTEXT_SIZE has not
     *          been defined (which means that the cipher has not been included
     *          in the build)
     * @return  The command may return CIPHER_ERR_INVALID_KEY_SIZE if the
     *          key size is not valid.
     */
    int quantum_cipher_init(quantum_cipher_t *cipher, quantum_cipher_id_t cipher_id, const uint8_t *key,
                    uint8_t key_size);

    /**
     * @brief Encrypt data
     * *
     *
     * @param cipher     Already initialized cipher struct
     * @param input      pointer to input data to encrypt
     * @param output     pointer to allocated memory for encrypted data. It has to
     *                   be of size BLOCK_SIZE
     *
     * @return           The result of the encrypt operation of the underlying
     *                   cipher, which is always 1 in case of success
     * @return           A negative value for an error
     */
    int quantum_cipher_encrypt(const quantum_cipher_t *cipher, const uint8_t *input,
                       uint8_t *output);

    /**
     * @brief Decrypt data
     * *
     *
     * @param cipher     Already initialized cipher struct
     * @param input      pointer to input data (of size BLOCKS_SIZE) to decrypt
     * @param output     pointer to allocated memory for decrypted data. It has to
     *                   be of size BLOCK_SIZE
     *
     * @return           The result of the decrypt operation of the underlying
     *                   cipher, which is always 1 in case of success
     * @return           A negative value for an error
     */
    int quantum_cipher_decrypt(const quantum_cipher_t *cipher, const uint8_t *input,
                       uint8_t *output);


#ifdef __cplusplus
}
#endif

/** @} */
#endif /* CRYPTO_QUANTUM_CIPHERS_H */
