#pragma once

#include <stdint.h>
#include <stdlib.h>

// based on https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/

#ifdef __cplusplus
extern "C" {
#endif

size_t b64_encoded_size(size_t inlen);
char *b64_encode(const uint8_t *in, size_t len);

size_t b64_decoded_size(const char *in);
int b64_decode(const char *in, uint8_t *out, size_t outlen);

#ifdef __cplusplus
}
#endif
