#include "board.h"
#include "dev.h"
#include "ffmpeg_sha256.h"

const uint32 K256[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define Ch(x,y,z)   (((x) & ((y) ^ (z))) ^ (z))
#define Maj(x,y,z)  ((((x) | (y)) & (z)) | ((x) & (y)))

#define Sigma0_256(x)   (rol((x), 30) ^ rol((x), 19) ^ rol((x), 10))
#define Sigma1_256(x)   (rol((x), 26) ^ rol((x), 21) ^ rol((x),  7))
#define sigma0_256(x)   (rol((x), 25) ^ rol((x), 14) ^ ((x) >> 3))
#define sigma1_256(x)   (rol((x), 15) ^ rol((x), 13) ^ ((x) >> 10))

#undef blk
#define blk(i)  (block[i] = block[i - 16] + sigma0_256(block[i - 15]) + \
                            sigma1_256(block[i - 2]) + block[i - 7])

#define ROUND256(a,b,c,d,e,f,g,h)   \
    T1 += (h) + Sigma1_256(e) + Ch((e), (f), (g)) + K256[i]; \
    (d) += T1; \
    (h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
    i++

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)   \
    T1 = blk0(i); \
    ROUND256(a,b,c,d,e,f,g,h)

#define ROUND256_16_TO_63(a,b,c,d,e,f,g,h)   \
    T1 = blk(i); \
    ROUND256(a,b,c,d,e,f,g,h)

static void sha256_transform(uint32 *state, const uint8 buffer[64])
{//printf("%s a\n",__func__);
    unsigned int i, a, b, c, d, e, f, g, h;
    uint32 block[64];
    uint32 T1;//, av_unused(T2);

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];
#if CONFIG_SMALL
    for (i = 0; i < 64; i++) {
        if (i < 16)
            T1 = blk0(i);
        else
            T1 = blk(i);
        T1 += h + Sigma1_256(e) + Ch(e, f, g) + K256[i];
        T2 = Sigma0_256(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
#else
	//printf("%s b\n",__func__);

    for (i = 0; i < 16;) {//printf("%s BB i %d\n",__func__,i);
        ROUND256_0_TO_15(a, b, c, d, e, f, g, h);
        ROUND256_0_TO_15(h, a, b, c, d, e, f, g);
        ROUND256_0_TO_15(g, h, a, b, c, d, e, f);
        ROUND256_0_TO_15(f, g, h, a, b, c, d, e);
        ROUND256_0_TO_15(e, f, g, h, a, b, c, d);
        ROUND256_0_TO_15(d, e, f, g, h, a, b, c);
        ROUND256_0_TO_15(c, d, e, f, g, h, a, b);
        ROUND256_0_TO_15(b, c, d, e, f, g, h, a);
    }
	//printf("%s c\n",__func__);

    for (; i < 64;) {
        ROUND256_16_TO_63(a, b, c, d, e, f, g, h);
        ROUND256_16_TO_63(h, a, b, c, d, e, f, g);
        ROUND256_16_TO_63(g, h, a, b, c, d, e, f);
        ROUND256_16_TO_63(f, g, h, a, b, c, d, e);
        ROUND256_16_TO_63(e, f, g, h, a, b, c, d);
        ROUND256_16_TO_63(d, e, f, g, h, a, b, c);
        ROUND256_16_TO_63(c, d, e, f, g, h, a, b);
        ROUND256_16_TO_63(b, c, d, e, f, g, h, a);
    }
	//printf("%s d\n",__func__);
#endif
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
	//printf("%s e\n",__func__);
}


int av_sha_init(AVSHA* ctx, int bits)
{
    ctx->digest_len = bits >> 5;
    switch (bits) {
 /*   case 160: // SHA-1
        ctx->state[0] = 0x67452301;
        ctx->state[1] = 0xEFCDAB89;
        ctx->state[2] = 0x98BADCFE;
        ctx->state[3] = 0x10325476;
        ctx->state[4] = 0xC3D2E1F0;
        ctx->transform = sha1_transform;
        break;
    case 224: // SHA-224
        ctx->state[0] = 0xC1059ED8;
        ctx->state[1] = 0x367CD507;
        ctx->state[2] = 0x3070DD17;
        ctx->state[3] = 0xF70E5939;
        ctx->state[4] = 0xFFC00B31;
        ctx->state[5] = 0x68581511;
        ctx->state[6] = 0x64F98FA7;
        ctx->state[7] = 0xBEFA4FA4;
        ctx->transform = sha256_transform;
        break;*/
    case 256: // SHA-256
        ctx->state[0] = 0x6A09E667;
        ctx->state[1] = 0xBB67AE85;
        ctx->state[2] = 0x3C6EF372;
        ctx->state[3] = 0xA54FF53A;
        ctx->state[4] = 0x510E527F;
        ctx->state[5] = 0x9B05688C;
        ctx->state[6] = 0x1F83D9AB;
        ctx->state[7] = 0x5BE0CD19;
        ctx->transform = sha256_transform;
        break;
    default:
        return -1;
    }
    ctx->count = 0;
    return 0;
}

void av_sha_update(AVSHA* ctx, const uint8* data, unsigned int len)
{
    unsigned int i, j;
	//printf("%s /1\n",__func__);

    j = ctx->count & 63;
    ctx->count += len;
#if CONFIG_SMALL
    for (i = 0; i < len; i++) {
        ctx->buffer[j++] = data[i];
        if (64 == j) {
            ctx->transform(ctx->state, ctx->buffer);
            j = 0;
        }
    }
#else
//printf("%s /2\n",__func__);
    if ((j + len) > 63) {
		//printf("%s /3 i %d j %d\n",__func__,i,j);
		//printf("%s data  %x (i = 64 - j) %d\n",__func__,data,(i = 64 - j));
        memcpy(&ctx->buffer[j], data, (i = 64 - j));
		//printf("%s /3\n",__func__);
        ctx->transform(ctx->state, ctx->buffer);
        for (; i + 63 < len; i += 64)
            ctx->transform(ctx->state, &data[i]);
        j = 0;
    } else
        i = 0;
	//printf("%s /4\n",__func__);
    memcpy(&ctx->buffer[j], &data[i], len - i);
	//printf("%s /5\n",__func__);
#endif
}

void av_sha_final(AVSHA* ctx, uint8 *digest)
{
    int i;
    uint64_t finalcount = (ctx->count << 3);

    av_sha_update(ctx, (const uint8*)"\200", 1);
    while ((ctx->count & 63) != 56)
        av_sha_update(ctx, (const uint8*)"", 1);
    av_sha_update(ctx, (uint8 *)&finalcount, 8); /* Should cause a transform() */
    for (i = 0; i < ctx->digest_len; i++)
        ((uint32*)digest)[i] = (ctx->state[i]);
}

#if LIBAVUTIL_VERSION_MAJOR < 51
struct AVSHA1 {
    AVSHA sha;
};

const int av_sha1_size = sizeof(struct AVSHA1);

//void av_sha1_init(struct AVSHA1* context)
//{
//    av_sha_init(&context->sha, 160);
//}

void av_sha1_update(struct AVSHA1* context, const uint8* data, unsigned int len)
{
    av_sha_update(&context->sha, data, len);
}

void av_sha1_final(struct AVSHA1* context, uint8 digest[20])
{
    av_sha_final(&context->sha, digest);
}
#endif

