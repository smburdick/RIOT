/**
 * \file kem.h
 * \brief Key encapsulation mechanisms
 *
 * The file `tests/example_kem.c` contains two examples on using the OQS_KEM API.
 *
 * The first example uses the individual scheme's algorithms directly and uses
 * no dynamic memory allocation -- all buffers are allocated on the stack, with
 * sizes indicated using preprocessor macros.  Since algorithms can be disabled at
 * compile-time, the programmer should wrap the code in \#ifdefs.
 *
 * The second example uses an OQS_KEM object to use an algorithm specified at
 * runtime.  Therefore it uses dynamic memory allocation -- all buffers must be
 * malloc'ed by the programmer, with sizes indicated using the corresponding length
 * member of the OQS_KEM object in question.  Since algorithms can be disabled at
 * compile-time, the programmer should check that the OQS_KEM object is not `NULL`.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_KEM_H
#define OQS_KEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "oqs.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Algorithm identifier for Kyber512 KEM. */
#define OQS_KEM_alg_kyber_512 "Kyber512"

// EDIT-WHEN-ADDING-KEM
///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALGS_LENGTH_START
/** Number of algorithm identifiers above. */
#define OQS_KEM_algs_length 1
///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALGS_LENGTH_END

/**
 * Returns identifiers for available key encapsulation mechanisms in liboqs.  Used with OQS_KEM_new.
 *
 * Note that algorithm identifiers are present in this list even when the algorithm is disabled
 * at compile time.
 *
 * @param[in] i Index of the algorithm identifier to return, 0 <= i < OQS_KEM_algs_length
 * @return Algorithm identifier as a string, or NULL.
 */
OQS_API const char *OQS_KEM_alg_identifier(size_t i);

/**
 * Returns the number of key encapsulation mechanisms in liboqs.  They can be enumerated with
 * OQS_KEM_alg_identifier.
 *
 * Note that some mechanisms may be disabled at compile time.
 *
 * @return The number of key encapsulation mechanisms.
 */
OQS_API int OQS_KEM_alg_count(void);

/**
 * Indicates whether the specified algorithm was enabled at compile-time or not.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_KEM_algs`.
 * @return 1 if enabled, 0 if disabled or not found
 */
OQS_API int OQS_KEM_alg_is_enabled(const char *method_name);

/**
 * Key encapsulation mechanism object
 */
typedef struct OQS_KEM {

	/** Printable string representing the name of the key encapsulation mechanism. */
	const char *method_name;

	/**
	 * Printable string representing the version of the cryptographic algorithm.
	 *
	 * Implementations with the same method_name and same alg_version will be interoperable.
	 * See README.md for information about algorithm compatibility.
	 */
	const char *alg_version;

	/** The NIST security level (1, 2, 3, 4, 5) claimed in this algorithm's original NIST submission. */
	uint8_t claimed_nist_level;

	/** Whether the KEM offers IND-CCA security (TRUE) or IND-CPA security (FALSE). */
	bool ind_cca;

	/** The (maximum) length, in bytes, of public keys for this KEM. */
	size_t length_public_key;
	/** The (maximum) length, in bytes, of secret keys for this KEM. */
	size_t length_secret_key;
	/** The (maximum) length, in bytes, of ciphertexts for this KEM. */
	size_t length_ciphertext;
	/** The (maximum) length, in bytes, of shared secrets for this KEM. */
	size_t length_shared_secret;

	/**
	 * Keypair generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `public_key` and
	 * `secret_key`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] public_key The public key represented as a byte string.
	 * @param[out] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

	/**
	 * Encapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `ciphertext` and
	 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] public_key The public key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*encaps)(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

	/**
	 * Decapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
	 * on the `length_*` members in this object or the per-scheme compile-time macros
	 * `OQS_KEM_*_length_*`.
	 *
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*decaps)(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

} OQS_KEM;

/**
 * Constructs an OQS_KEM object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm name was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_KEM_algs`.
 * @return An OQS_KEM for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_API OQS_KEM *OQS_KEM_new(const char *method_name);

/**
 * Keypair generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `public_key` and
 * `secret_key`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] public_key The public key represented as a byte string.
 * @param[out] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key);

/**
 * Encapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `ciphertext` and
 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

/**
 * Decapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
 * on the `length_*` members in this object or the per-scheme compile-time macros
 * `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

/**
 * Frees an OQS_KEM object that was constructed by OQS_KEM_new.
 *
 * @param[in] kem The OQS_KEM object to free.
 */
OQS_API void OQS_KEM_free(OQS_KEM *kem);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_KEM_H
