// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
#include <fail-simulator-on-error.h>
#include <string_view>
#include <thread.h>

__BEGIN_DECLS
#include <mlkem_native.h>
constexpr size_t MLKEMPublicKeyBytes    = CRYPTO_PUBLICKEYBYTES;
constexpr size_t MLKEMSecretKeyBytes    = CRYPTO_SECRETKEYBYTES;
constexpr size_t MLKEMSharedSecretBytes = CRYPTO_BYTES;
#undef CRYPTO_PUBLICKEYBYTES
#undef CRYPTO_SECRETKEYBYTES
#undef CRYPTO_BYTES
#define MLK_CONFIG_API_NO_SUPERCOP
#include <mldsa_native.h>
constexpr size_t MLDSAPublicKeyBytes = MLDSA_PUBLICKEYBYTES(44);
constexpr size_t MLDSASecretKeyBytes = MLDSA_SECRETKEYBYTES(44);
constexpr size_t MLDSASignatureBytes = MLDSA_BYTES(44);
__END_DECLS

/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "PQC benchmark">;

namespace
{
	// ML-KEM things
	uint8_t mlkem_public_key[MLKEMPublicKeyBytes];
	uint8_t mlkem_secret_key[MLKEMSecretKeyBytes];
	uint8_t mlkem_ciphertext[CRYPTO_CIPHERTEXTBYTES];
	uint8_t mlkem_shared_secret_original[MLKEMSharedSecretBytes];
	uint8_t mlkem_shared_secret_decrypted[MLKEMSharedSecretBytes];
	uint8_t mlkem_result[32];

	// ML-DSA things
	uint8_t                    mldsa_public_key[MLDSAPublicKeyBytes];
	uint8_t                    mldsa_secret_key[MLDSASecretKeyBytes];
	uint8_t                    mldsa_signature[MLDSASignatureBytes];
	constexpr std::string_view TestMessage      = "Hello world!";
	constexpr size_t           TestMessageBytes = TestMessage.size();
	uint8_t mldsa_signed_message[TestMessageBytes + MLDSASignatureBytes];
	uint8_t mldsa_recovered_message[TestMessageBytes + MLDSASignatureBytes];

} // namespace

/// Thread entry point.
int __cheri_compartment("pqc_benchmark") benchmark()
{
	ptraddr_t stackTop = CHERI::Capability{__builtin_cheri_stack_get()}.top();
	int       rc       = 0;
	auto      lastTime = rdcycle64();
	auto      reportStack = [&](const char *msg) {
        auto      time   = rdcycle64();
        ptraddr_t lowest = stack_lowest_used_address();
        ssize_t   used   = stackTop - lowest;
        Debug::Invariant(rc == 0, "ERROR: {} failed! {}", msg, rc);
        Debug::log("{}: stack used: {} bytes, cycles elapsed: {}",
                   msg,
                   used,
                   static_cast<int64_t>(time - lastTime));
        // Reset the stack depth
        Timeout t{0};
        thread_sleep(&t);
        // Record the cycle counter for the next operation
        lastTime = rdcycle64();
	};
	reportStack("Starting");
	rc = mlkem768_keypair(mlkem_public_key, mlkem_secret_key);
	reportStack("Generated ML-KEM key pair");
	rc = mlkem768_enc(
	  mlkem_ciphertext, mlkem_shared_secret_original, mlkem_public_key);
	reportStack("Encrypted secret pair with ML-KEM");
	rc = mlkem768_dec(
	  mlkem_shared_secret_decrypted, mlkem_ciphertext, mlkem_secret_key);
	reportStack("Decrypted secret pair with ML-KEM");
	rc = memcmp(mlkem_shared_secret_original,
	            mlkem_shared_secret_decrypted,
	            MLKEMSharedSecretBytes);
	reportStack("Compared results successfully for ML-KEM");

	rc = mldsa44_keypair(mldsa_public_key, mldsa_secret_key);
	reportStack("Generated ML-DSA key pair");

	size_t                            signatureLength;
	static constexpr std::string_view Context = "SomeContext";

	rc =
	  mldsa44_signature(mldsa_signature,
	                    &signatureLength,
	                    reinterpret_cast<const uint8_t *>(TestMessage.data()),
	                    TestMessage.size(),
	                    reinterpret_cast<const uint8_t *>(Context.data()),
	                    Context.size(),
	                    mldsa_secret_key);
	Debug::Invariant(signatureLength == MLDSASignatureBytes, "{} != {}", signatureLength, MLDSASignatureBytes);
	reportStack("Signed message with ML-DSA");

	rc = mldsa44_verify(mldsa_signature,
	                    signatureLength,
	                    reinterpret_cast<const uint8_t *>(TestMessage.data()),
	                    TestMessage.size(),
	                    reinterpret_cast<const uint8_t *>(Context.data()),
	                    Context.size(),
	                    mldsa_public_key);
	reportStack("Verified message signature with ML-DSA");

	mldsa_signature[4]++;
	rc = mldsa44_verify(mldsa_signature,
	                    signatureLength,
	                    reinterpret_cast<const uint8_t *>(TestMessage.data()),
	                    TestMessage.size(),
	                    reinterpret_cast<const uint8_t *>(Context.data()),
	                    Context.size(),
	                    mldsa_public_key);
	rc = (rc == 0);
	reportStack("Correctly failed to verify message signature with ML-DSA "
	            "after tampering");

	return 0;
}
