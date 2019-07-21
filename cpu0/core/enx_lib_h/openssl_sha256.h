


////////////////////////// openssl ////////////////////////////////////// [[
#if 0

#ifndef _NGX_OPENSSL_SHA256_H_INCLUDED_
#define _NGX_OPENSSL_SHA256_H_INCLUDED_

#define SHA_LONG unsigned long
#define SHA_LBLOCK	16
typedef struct SHA256state_st
	{
	SHA_LONG h[8];
	SHA_LONG Nl,Nh;
	SHA_LONG data[SHA_LBLOCK];
	unsigned int num,md_len;
	} SHA256_CTX;

typedef struct SHAstate_st
	{
	SHA_LONG h0,h1,h2,h3,h4;
	SHA_LONG Nl,Nh;
	SHA_LONG data[SHA_LBLOCK];
	unsigned int num;
	} SHA_CTX;

#define SHA_CBLOCK	(SHA_LBLOCK*4)	/* SHA treats input data as a
					 * contiguous array of 32 bit
					 * wide big-endian values. */

#define	HASH_CTX		SHA256_CTX
#define	HASH_LONG		SHA_LONG
#define	HASH_CBLOCK		SHA_CBLOCK
#define MD32_REG_T long

#if 1//DATA_ORDER_IS_LITTLE_ENDIAN
#define HOST_c2l(c,l)	(l =(((unsigned long)(*((c)++)))<<24),		\
			 l|=(((unsigned long)(*((c)++)))<<16),		\
			 l|=(((unsigned long)(*((c)++)))<< 8),		\
			 l|=(((unsigned long)(*((c)++)))    ),		\
			 l)
			 
#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l)>>24)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>>16)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
			 *((c)++)=(unsigned char)(((l)    )&0xff),	\
			 l)
#else //BIG ENDIAN
#define HOST_c2l(c,l)	(l =(((unsigned long)(*((c)++)))    ),		\
				 l|=(((unsigned long)(*((c)++)))<< 8),		\
				 l|=(((unsigned long)(*((c)++)))<<16),		\
				 l|=(((unsigned long)(*((c)++)))<<24),		\
				 l)
#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l)    )&0xff),	\
				 *((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
				 *((c)++)=(unsigned char)(((l)>>16)&0xff),	\
				 *((c)++)=(unsigned char)(((l)>>24)&0xff),	\
				 l)
#endif
#define K_00_19	0x5a827999UL
#define K_20_39 0x6ed9eba1UL
#define K_40_59 0x8f1bbcdcUL
#define K_60_79 0xca62c1d6UL

#define	F_00_19(b,c,d)	((((c) ^ (d)) & (b)) ^ (d)) 
#define	F_20_39(b,c,d)	((b) ^ (c) ^ (d))
#define F_40_59(b,c,d)	(((b) & (c)) | (((b)|(c)) & (d))) 
#define	F_60_79(b,c,d)	F_20_39(b,c,d)

#if 1
#define	ROTATE(a,n)	(((a)>>(n))+((a)<<(32-(n))))
#else
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
#endif

#if 0
# define Xupdate(a,ix,ia,ib,ic,id)	(ix=(a)=(ia^ib^ic^id))
#else
#  define Xupdate(a,ix,ia,ib,ic,id)	( (a)=(ia^ib^ic^id),	\
					  ix=(a)=ROTATE((a),1)	\
					)
#endif

#ifndef MD32_XARRAY
  /*
   * Originally X was an array. As it's automatic it's natural
   * to expect RISC compiler to accomodate at least part of it in
   * the register bank, isn't it? Unfortunately not all compilers
   * "find" this expectation reasonable:-( On order to make such
   * compilers generate better code I replace X[] with a bunch of
   * X0, X1, etc. See the function body below...
   *					<appro@fy.chalmers.se>
   */
# define X(i)	XX##i
#else
  /*
   * However! Some compilers (most notably HP C) get overwhelmed by
   * that many local variables so that we have to have the way to
   * fall down to the original behavior.
   */
# define X(i)	XX[i]
#endif
#define OPENSSL_SMALL_FOOTPRINT//TEST
#ifndef OPENSSL_SMALL_FOOTPRINT
	
#define BODY_00_15(i,a,b,c,d,e,f,xi) \
		(f)=xi+(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
		(b)=ROTATE((b),30);

#define BODY_16_19(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	Xupdate(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_20_31(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	Xupdate(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_32_39(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_40_59(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+K_40_59+ROTATE((a),5)+F_40_59((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_60_79(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)=xa+(e)+K_60_79+ROTATE((a),5)+F_60_79((b),(c),(d)); \
	(b)=ROTATE((b),30);

#else
#define BODY_00_15(xi)		 do {	\
		T=E+K_00_19+F_00_19(B,C,D); \
		E=D, D=C, C=ROTATE(B,30), B=A;	\
		A=ROTATE(A,5)+T+xi; 	} while(0)
	
#define BODY_16_19(xa,xb,xc,xd)	 do {	\
			Xupdate(T,xa,xa,xb,xc,xd);	\
			T+=E+K_00_19+F_00_19(B,C,D);	\
			E=D, D=C, C=ROTATE(B,30), B=A;	\
			A=ROTATE(A,5)+T;		} while(0)
		
#define BODY_20_39(xa,xb,xc,xd)	 do {	\
			Xupdate(T,xa,xa,xb,xc,xd);	\
			T+=E+K_20_39+F_20_39(B,C,D);	\
			E=D, D=C, C=ROTATE(B,30), B=A;	\
			A=ROTATE(A,5)+T;		} while(0)
		
#define BODY_40_59(xa,xb,xc,xd)	 do {	\
			Xupdate(T,xa,xa,xb,xc,xd);	\
			T+=E+K_40_59+F_40_59(B,C,D);	\
			E=D, D=C, C=ROTATE(B,30), B=A;	\
			A=ROTATE(A,5)+T;		} while(0)
		
#define BODY_60_79(xa,xb,xc,xd)	 do {	\
			Xupdate(T,xa,xa,xb,xc,xd);	\
			T=E+K_60_79+F_60_79(B,C,D); \
			E=D, D=C, C=ROTATE(B,30), B=A;	\
			A=ROTATE(A,5)+T+xa; 	} while(0)
#endif

#define EVP_MAX_MD_SIZE			64	/* longest known is SHA512 */
#define EVP_MAX_KEY_LENGTH		32
#define EVP_MAX_IV_LENGTH		16
#define EVP_MAX_BLOCK_LENGTH		32

#define EVP_MD_CTX_FLAG_ONESHOT		0x0001 /* digest update will be called
						* once only */
#define EVP_MD_CTX_FLAG_CLEANED		0x0002 /* context has already been
						* cleaned */
#define EVP_MD_CTX_FLAG_REUSE		0x0004 /* Don't free up ctx->md_data
						* in EVP_MD_CTX_cleanup */
//#define	HASH_UPDATE		SHA256_Update
//#define	HASH_FINAL		SHA256_Final

/*
 * When changing the CRYPTO_LOCK_* list, be sure to maintin the text lock
 * names in cryptlib.c
 */

# define CRYPTO_LOCK_ERR                 1
# define CRYPTO_LOCK_EX_DATA             2
# define CRYPTO_LOCK_X509                3
# define CRYPTO_LOCK_X509_INFO           4
# define CRYPTO_LOCK_X509_PKEY           5
# define CRYPTO_LOCK_X509_CRL            6
# define CRYPTO_LOCK_X509_REQ            7
# define CRYPTO_LOCK_DSA                 8
# define CRYPTO_LOCK_RSA                 9
# define CRYPTO_LOCK_EVP_PKEY            10
# define CRYPTO_LOCK_X509_STORE          11
# define CRYPTO_LOCK_SSL_CTX             12
# define CRYPTO_LOCK_SSL_CERT            13
# define CRYPTO_LOCK_SSL_SESSION         14
# define CRYPTO_LOCK_SSL_SESS_CERT       15
# define CRYPTO_LOCK_SSL                 16
# define CRYPTO_LOCK_SSL_METHOD          17
# define CRYPTO_LOCK_RAND                18
# define CRYPTO_LOCK_RAND2               19
# define CRYPTO_LOCK_MALLOC              20
# define CRYPTO_LOCK_BIO                 21
# define CRYPTO_LOCK_GETHOSTBYNAME       22
# define CRYPTO_LOCK_GETSERVBYNAME       23
# define CRYPTO_LOCK_READDIR             24
# define CRYPTO_LOCK_RSA_BLINDING        25
# define CRYPTO_LOCK_DH                  26
# define CRYPTO_LOCK_MALLOC2             27
# define CRYPTO_LOCK_DSO                 28
# define CRYPTO_LOCK_DYNLOCK             29
# define CRYPTO_LOCK_ENGINE              30
# define CRYPTO_LOCK_UI                  31
# define CRYPTO_LOCK_ECDSA               32
# define CRYPTO_LOCK_EC                  33
# define CRYPTO_LOCK_ECDH                34
# define CRYPTO_LOCK_BN                  35
# define CRYPTO_LOCK_EC_PRE_COMP         36
# define CRYPTO_LOCK_STORE               37
# define CRYPTO_LOCK_COMP                38
# define CRYPTO_LOCK_FIPS                39
# define CRYPTO_LOCK_FIPS2               40
# define CRYPTO_NUM_LOCKS                41

# define CRYPTO_LOCK             1
# define CRYPTO_UNLOCK           2
# define CRYPTO_READ             4
# define CRYPTO_WRITE            8

#define OPENSSL_NO_LOCKING
# ifndef OPENSSL_NO_LOCKING
#  ifndef CRYPTO_w_lock
#   define CRYPTO_w_lock(type)     \
        CRYPTO_lock(CRYPTO_LOCK|CRYPTO_WRITE,type,__FILE__,__LINE__)
#   define CRYPTO_w_unlock(type)   \
        CRYPTO_lock(CRYPTO_UNLOCK|CRYPTO_WRITE,type,__FILE__,__LINE__)
#   define CRYPTO_r_lock(type)     \
        CRYPTO_lock(CRYPTO_LOCK|CRYPTO_READ,type,__FILE__,__LINE__)
#   define CRYPTO_r_unlock(type)   \
        CRYPTO_lock(CRYPTO_UNLOCK|CRYPTO_READ,type,__FILE__,__LINE__)
#   define CRYPTO_add(addr,amount,type)    \
        CRYPTO_add_lock(addr,amount,type,__FILE__,__LINE__)
#  endif
# else
#  define CRYPTO_w_lock(a)
#  define CRYPTO_w_unlock(a)
#  define CRYPTO_r_lock(a)
#  define CRYPTO_r_unlock(a)
#  define CRYPTO_add(a,b,c)       ((*(a))+=(b))
# endif
# define EVP_MD_CTX_FLAG_PAD_MASK        0xF0/* RSA mode to use */
# define EVP_MD_CTX_FLAG_PAD_PKCS1       0x00/* PKCS#1 v1.5 mode */
# define EVP_MD_CTX_FLAG_PAD_X931        0x10/* X9.31 mode */
# define EVP_MD_CTX_FLAG_PAD_PSS         0x20/* PSS mode */

# define EVP_MD_CTX_FLAG_NO_INIT         0x0100/* Don't initialize md_data */
# define EVP_PKEY_OP_UNDEFINED           0
# define EVP_PKEY_OP_PARAMGEN            (1<<1)
# define EVP_PKEY_OP_KEYGEN              (1<<2)
# define EVP_PKEY_OP_SIGN                (1<<3)
# define EVP_PKEY_OP_VERIFY              (1<<4)
# define EVP_PKEY_OP_VERIFYRECOVER       (1<<5)
# define EVP_PKEY_OP_SIGNCTX             (1<<6)
# define EVP_PKEY_OP_VERIFYCTX           (1<<7)
# define EVP_PKEY_OP_ENCRYPT             (1<<8)
# define EVP_PKEY_OP_DECRYPT             (1<<9)
# define EVP_PKEY_OP_DERIVE              (1<<10)

# define EVP_PKEY_OP_TYPE_SIG    \
        (EVP_PKEY_OP_SIGN | EVP_PKEY_OP_VERIFY | EVP_PKEY_OP_VERIFYRECOVER \
                | EVP_PKEY_OP_SIGNCTX | EVP_PKEY_OP_VERIFYCTX)

# define EVP_PKEY_OP_TYPE_CRYPT \
        (EVP_PKEY_OP_ENCRYPT | EVP_PKEY_OP_DECRYPT)

# define EVP_PKEY_OP_TYPE_NOGEN \
        (EVP_PKEY_OP_SIG | EVP_PKEY_OP_CRYPT | EVP_PKEY_OP_DERIVE)

# define EVP_PKEY_OP_TYPE_GEN \
                (EVP_PKEY_OP_PARAMGEN | EVP_PKEY_OP_KEYGEN)

# define  EVP_PKEY_CTX_set_signature_md(ctx, md) \
                EVP_PKEY_CTX_ctrl(ctx, -1, EVP_PKEY_OP_TYPE_SIG,  \
                                        EVP_PKEY_CTRL_MD, 0, (void *)md)
# define EVP_PKEY_CTRL_MD                1
# define EVP_PKEY_CTRL_PEER_KEY          2
	
# define EVP_PKEY_CTRL_PKCS7_ENCRYPT     3
# define EVP_PKEY_CTRL_PKCS7_DECRYPT     4
	
# define EVP_PKEY_CTRL_PKCS7_SIGN        5
	
# define EVP_PKEY_CTRL_SET_MAC_KEY       6
	
# define EVP_PKEY_CTRL_DIGESTINIT        7

typedef struct env_md_st EVP_MD;
typedef struct env_md_ctx_st EVP_MD_CTX;
typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;
typedef struct evp_pkey_method_st EVP_PKEY_METHOD;
typedef struct evp_pkey_st EVP_PKEY;
typedef int EVP_PKEY_gen_cb (EVP_PKEY_CTX *ctx);
typedef struct evp_pkey_asn1_method_st EVP_PKEY_ASN1_METHOD;

#define HMAC_MAX_MD_CBLOCK	128	/* largest known is SHA512 */

struct env_md_ctx_st {
    const EVP_MD *digest;
    void/*ENGINE*/ *engine;             /* functional reference if 'digest' is
                                 * ENGINE-provided */
    unsigned long flags;
    void *md_data;
    /* Public key context for sign/verify */
    EVP_PKEY_CTX *pctx;
    /* Update function: usually copied from EVP_MD */
    int (*update) (EVP_MD_CTX *ctx, const void *data, size_t count);
} /* EVP_MD_CTX */ ;

typedef struct hmac_ctx_st
	{
	const EVP_MD *md;
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX i_ctx;
	EVP_MD_CTX o_ctx;
	unsigned int key_length;
	unsigned char key[HMAC_MAX_MD_CBLOCK];
	} HMAC_CTX;
struct evp_pkey_ctx_st {
    /* Method associated with this operation */
    const EVP_PKEY_METHOD *pmeth;
    /* Engine that implements this method or NULL if builtin */
    void/*ENGINE*/ *engine;
    /* Key: may be NULL */
    EVP_PKEY *pkey;
    /* Peer key for key agreement, may be NULL */
    EVP_PKEY *peerkey;
    /* Actual operation */
    int operation;
    /* Algorithm specific data */
    void *data;
    /* Application specific data */
    void *app_data;
    /* Keygen callback */
    EVP_PKEY_gen_cb *pkey_gencb;
    /* implementation specific keygen data */
    int *keygen_info;
    int keygen_info_count;
} /* EVP_PKEY_CTX */ ;

struct evp_pkey_method_st {
    int pkey_id;
    int flags;
    int (*init) (EVP_PKEY_CTX *ctx);
    int (*copy) (EVP_PKEY_CTX *dst, EVP_PKEY_CTX *src);
    void (*cleanup) (EVP_PKEY_CTX *ctx);
    int (*paramgen_init) (EVP_PKEY_CTX *ctx);
    int (*paramgen) (EVP_PKEY_CTX *ctx, EVP_PKEY *pkey);
    int (*keygen_init) (EVP_PKEY_CTX *ctx);
    int (*keygen) (EVP_PKEY_CTX *ctx, EVP_PKEY *pkey);
    int (*sign_init) (EVP_PKEY_CTX *ctx);
    int (*sign) (EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen,
                 const unsigned char *tbs, size_t tbslen);
    int (*verify_init) (EVP_PKEY_CTX *ctx);
    int (*verify) (EVP_PKEY_CTX *ctx,
                   const unsigned char *sig, size_t siglen,
                   const unsigned char *tbs, size_t tbslen);
    int (*verify_recover_init) (EVP_PKEY_CTX *ctx);
    int (*verify_recover) (EVP_PKEY_CTX *ctx,
                           unsigned char *rout, size_t *routlen,
                           const unsigned char *sig, size_t siglen);
    int (*signctx_init) (EVP_PKEY_CTX *ctx, EVP_MD_CTX *mctx);
    int (*signctx) (EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen,
                    EVP_MD_CTX *mctx);
    int (*verifyctx_init) (EVP_PKEY_CTX *ctx, EVP_MD_CTX *mctx);
    int (*verifyctx) (EVP_PKEY_CTX *ctx, const unsigned char *sig, int siglen,
                      EVP_MD_CTX *mctx);
    int (*encrypt_init) (EVP_PKEY_CTX *ctx);
    int (*encrypt) (EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
                    const unsigned char *in, size_t inlen);
    int (*decrypt_init) (EVP_PKEY_CTX *ctx);
    int (*decrypt) (EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
                    const unsigned char *in, size_t inlen);
    int (*derive_init) (EVP_PKEY_CTX *ctx);
    int (*derive) (EVP_PKEY_CTX *ctx, unsigned char *key, size_t *keylen);
    int (*ctrl) (EVP_PKEY_CTX *ctx, int type, int p1, void *p2);
    int (*ctrl_str) (EVP_PKEY_CTX *ctx, const char *type, const char *value);
} /* EVP_PKEY_METHOD */ ;
/*
 * Type needs to be a bit field Sub-type needs to be for variations on the
 * method, as in, can it do arbitrary encryption....
 */
struct evp_pkey_st {
    int type;
    int save_type;
    int references;
    const EVP_PKEY_ASN1_METHOD *ameth;
    void/*ENGINE*/ *engine;
    union {
        char *ptr;
# ifndef OPENSSL_NO_RSA
        struct rsa_st *rsa;     /* RSA */
# endif
# ifndef OPENSSL_NO_DSA
        struct dsa_st *dsa;     /* DSA */
# endif
# ifndef OPENSSL_NO_DH
        struct dh_st *dh;       /* DH */
# endif
# ifndef OPENSSL_NO_EC
        struct ec_key_st *ec;   /* ECC */
# endif
    } pkey;
    int save_parameters;
   // STACK_OF(X509_ATTRIBUTE) *attributes; /* [ 0 ] */
} /* EVP_PKEY */ ;

struct evp_pkey_asn1_method_st {
    int pkey_id;
    int pkey_base_id;
    unsigned long pkey_flags;
    char *pem_str;
    char *info;
#if 0	
    int (*pub_decode) (EVP_PKEY *pk, X509_PUBKEY *pub);
    int (*pub_encode) (X509_PUBKEY *pub, const EVP_PKEY *pk);
    int (*pub_cmp) (const EVP_PKEY *a, const EVP_PKEY *b);
    int (*pub_print) (BIO *out, const EVP_PKEY *pkey, int indent,
                      ASN1_PCTX *pctx);
    int (*priv_decode) (EVP_PKEY *pk, PKCS8_PRIV_KEY_INFO *p8inf);
    int (*priv_encode) (PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pk);
    int (*priv_print) (BIO *out, const EVP_PKEY *pkey, int indent,
                       ASN1_PCTX *pctx);
    int (*pkey_size) (const EVP_PKEY *pk);
    int (*pkey_bits) (const EVP_PKEY *pk);
    int (*param_decode) (EVP_PKEY *pkey,
                         const unsigned char **pder, int derlen);
    int (*param_encode) (const EVP_PKEY *pkey, unsigned char **pder);
    int (*param_missing) (const EVP_PKEY *pk);
    int (*param_copy) (EVP_PKEY *to, const EVP_PKEY *from);
    int (*param_cmp) (const EVP_PKEY *a, const EVP_PKEY *b);
    int (*param_print) (BIO *out, const EVP_PKEY *pkey, int indent,
                        ASN1_PCTX *pctx);
    int (*sig_print) (BIO *out,
                      const X509_ALGOR *sigalg, const ASN1_STRING *sig,
                      int indent, ASN1_PCTX *pctx);
    void (*pkey_free) (EVP_PKEY *pkey);
    int (*pkey_ctrl) (EVP_PKEY *pkey, int op, long arg1, void *arg2);
    /* Legacy functions for old PEM */
    int (*old_priv_decode) (EVP_PKEY *pkey,
                            const unsigned char **pder, int derlen);
    int (*old_priv_encode) (const EVP_PKEY *pkey, unsigned char **pder);
    /* Custom ASN1 signature verification */
    int (*item_verify) (EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
                        X509_ALGOR *a, ASN1_BIT_STRING *sig, EVP_PKEY *pkey);
    int (*item_sign) (EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
                      X509_ALGOR *alg1, X509_ALGOR *alg2,
                      ASN1_BIT_STRING *sig);
#endif	
} /* EVP_PKEY_ASN1_METHOD */ ;

struct rsa_st {
    /*
     * The first parameter is used to pickup errors where this is passed
     * instead of aEVP_PKEY, it is set to 0
     */
    int pad;
    long version;
#if 0	
    const RSA_METHOD *meth;
    /* functional reference if 'meth' is ENGINE-provided */
    ENGINE *engine;
    BIGNUM *n;
    BIGNUM *e;
    BIGNUM *d;
    BIGNUM *p;
    BIGNUM *q;
    BIGNUM *dmp1;
    BIGNUM *dmq1;
    BIGNUM *iqmp;
    /* be careful using this if the RSA structure is shared */
    CRYPTO_EX_DATA ex_data;
    int references;
    int flags;
    /* Used to cache montgomery values */
    BN_MONT_CTX *_method_mod_n;
    BN_MONT_CTX *_method_mod_p;
    BN_MONT_CTX *_method_mod_q;
    /*
     * all BIGNUM values are actually in the following data, if it is not
     * NULL
     */
    char *bignum_data;
    BN_BLINDING *blinding;
    BN_BLINDING *mt_blinding;
#endif	
};

struct dsa_st {
    /*
     * This first variable is used to pick up errors where a DSA is passed
     * instead of of a EVP_PKEY
     */
    int pad;
    long version;
    int write_params;
#if 0	
    BIGNUM *p;
    BIGNUM *q;                  /* == 20 */
    BIGNUM *g;
    BIGNUM *pub_key;            /* y public key */
    BIGNUM *priv_key;           /* x private key */
    BIGNUM *kinv;               /* Signing pre-calc */
    BIGNUM *r;                  /* Signing pre-calc */
    int flags;
    /* Normally used to cache montgomery values */
    BN_MONT_CTX *method_mont_p;
    int references;
    CRYPTO_EX_DATA ex_data;
    const DSA_METHOD *meth;
    /* functional reference if 'meth' is ENGINE-provided */
    ENGINE *engine;
#endif	
};

struct dh_st {
    /*
     * This first argument is used to pick up errors when a DH is passed
     * instead of a EVP_PKEY
     */
    int pad;
    int version;
#if 0	
    BIGNUM *p;
    BIGNUM *g;
    long length;                /* optional */
    BIGNUM *pub_key;            /* g^x */
    BIGNUM *priv_key;           /* x */
    int flags;
    BN_MONT_CTX *method_mont_p;
    /* Place holders if we want to do X9.42 DH */
    BIGNUM *q;
    BIGNUM *j;
    unsigned char *seed;
    int seedlen;
    BIGNUM *counter;
    int references;
    CRYPTO_EX_DATA ex_data;
    const DH_METHOD *meth;
    ENGINE *engine;
#endif	
};

struct ec_key_st {
    int version;
#if	0
    EC_GROUP *group;
    EC_POINT *pub_key;
    BIGNUM *priv_key;
    unsigned int enc_flag;
    point_conversion_form_t conv_form;
    int references;
    int flags;
    EC_EXTRA_DATA *method_data;
#endif	
} /* EC_KEY */ ;

#define NID_sha256		672
#define NID_sha256WithRSAEncryption		668

#define EVP_PKEY_NULL_method	NULL,NULL,{0,0,0,0}
#define EVP_PKEY_RSA_method	EVP_PKEY_NULL_method

#define SHA256_CBLOCK	(SHA_LBLOCK*4)	/* SHA-256 treats input data as a
					 * contiguous array of 32 bit
					 * wide big-endian values. */
struct env_md_st
{
	int type;
	int pkey_type;
	int md_size;
	unsigned long flags;
	int (*init)(EVP_MD_CTX *ctx);
	int (*update)(EVP_MD_CTX *ctx,const void *data,size_t count);
	int (*final)(EVP_MD_CTX *ctx,unsigned char *md);
	int (*copy)(EVP_MD_CTX *to,const EVP_MD_CTX *from);
	int (*cleanup)(EVP_MD_CTX *ctx);

	/* FIXME: prototype these some day */
	int (*sign)(int type, const unsigned char *m, unsigned int m_length,
			unsigned char *sigret, unsigned int *siglen, void *key);
	int (*verify)(int type, const unsigned char *m, unsigned int m_length,
			  const unsigned char *sigbuf, unsigned int siglen,
			  void *key);
	int required_pkey_type[5]; /*EVP_PKEY_xxx */
	int block_size;
	int ctx_size; /* how big does the ctx->md_data need to be */
	
  //  int (*md_ctrl) (EVP_MD_CTX *ctx, int cmd, int p1, void *p2);
} /* EVP_MD */;
#endif
#endif
///////////////////////////open ssl /////////////////////////////////////// ]]

