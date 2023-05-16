// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_KYBER_H
#define OQS_KEM_KYBER_H

//#include <oqs/oqs.h>
#include "kem.h"

// For now, only enable OQS KEM 512
#define OQS_KEM_kyber_512_length_public_key 800
#define OQS_KEM_kyber_512_length_secret_key 1632
#define OQS_KEM_kyber_512_length_ciphertext 768
#define OQS_KEM_kyber_512_length_shared_secret 32
OQS_KEM *OQS_KEM_kyber_512_new(void);
OQS_API OQS_STATUS OQS_KEM_kyber_512_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_kyber_512_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_kyber_512_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

