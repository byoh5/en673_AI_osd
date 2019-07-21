#include "board.h"
#include "ngx_rtmp_config.h"


#ifndef SHA256_H
# define SHA256_H 1

# include <stdio.h>
# include <stdint.h>

# if HAVE_OPENSSL_SHA256
#  include <openssl/sha.h>
# endif

# ifdef __cplusplus
extern "C" {
# endif

enum { SHA224_DIGEST_SIZE = 224 / 8 };
enum { SHA256_DIGEST_SIZE = 256 / 8 };

# if HAVE_OPENSSL_SHA256
#  define GL_OPENSSL_NAME 224
#  include "gl_openssl.h"
#  define GL_OPENSSL_NAME 256
#  include "gl_openssl.h"
# else
/* Structure to save state of computation between the single steps.  */
struct sha256_ctx
{
  uint32 state[8];

  uint32 total[2];
  size_t buflen;
  uint32 buffer[32];
};

/** hash context */
typedef struct AVSHA {
    uint8  digest_len;  ///< digest length in 32-bit words
    uint64 count;       ///< number of bytes in buffer
    uint8  buffer[64];  ///< 512-bit buffer of input values used in hash updating
    uint32 state[8];    ///< current hash value
    /** function used to update hash for 512-bit input block */
    void     (*transform)(uint32 *state, const uint8 buffer[64]);
} AVSHA;


#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define blk0(i) (block[i] = (((const uint32*)buffer)[i]))
#define blk(i)  (block[i] = rol(block[i-3] ^ block[i-8] ^ block[i-14] ^ block[i-16], 1))

#define R0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + blk0(i) + 0x5A827999 + rol(v, 5); w = rol(w, 30);
#define R1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + blk (i) + 0x5A827999 + rol(v, 5); w = rol(w, 30);
#define R2(v,w,x,y,z,i) z += ( w^x     ^y)     + blk (i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + blk (i) + 0x8F1BBCDC + rol(v, 5); w = rol(w, 30);
#define R4(v,w,x,y,z,i) z += ( w^x     ^y)     + blk (i) + 0xCA62C1D6 + rol(v, 5); w = rol(w, 30);

#define IPAD 0x36
#define OPAD 0x5c

/* Hash a single 512-bit block. This is the core of the algorithm. */

/* Initialize structure containing state of computation. */
extern void sha256_init_ctx (struct sha256_ctx *ctx);
extern void sha224_init_ctx (struct sha256_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is necessary that LEN is a multiple of 64!!! */
extern void sha256_process_block (const void *buffer, size_t len,
                                  struct sha256_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is NOT required that LEN is a multiple of 64.  */
extern void sha256_process_bytes (const void *buffer, size_t len,
                                  struct sha256_ctx *ctx);

/* Process the remaining bytes in the buffer and put result from CTX
   in first 32 (28) bytes following RESBUF.  The result is always in little
   endian byte order, so that a byte-wise output yields to the wanted
   ASCII representation of the message digest.  */
extern void *sha256_finish_ctx (struct sha256_ctx *ctx, void *resbuf);
extern void *sha224_finish_ctx (struct sha256_ctx *ctx, void *resbuf);


/* Put result from CTX in first 32 (28) bytes following RESBUF.  The result is
   always in little endian byte order, so that a byte-wise output yields
   to the wanted ASCII representation of the message digest.  */
extern void *sha256_read_ctx (const struct sha256_ctx *ctx, void *resbuf);
extern void *sha224_read_ctx (const struct sha256_ctx *ctx, void *resbuf);


/* Compute SHA256 (SHA224) message digest for LEN bytes beginning at BUFFER.  The
   result is always in little endian byte order, so that a byte-wise
   output yields to the wanted ASCII representation of the message
   digest.  */
extern void *sha256_buffer (const char *buffer, size_t len, void *resblock);
extern void *sha224_buffer (const char *buffer, size_t len, void *resblock);

# endif
/* Compute SHA256 (SHA224) message digest for bytes read from STREAM.  The
   resulting message digest number will be written into the 32 (28) bytes
   beginning at RESBLOCK.  */
//extern int sha256_stream (FILE *stream, void *resblock);
//extern int sha224_stream (FILE *stream, void *resblock);


# ifdef __cplusplus
}
# endif

#endif


