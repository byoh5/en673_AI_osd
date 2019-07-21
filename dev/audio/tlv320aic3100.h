#ifndef __TLV320AIC3100_H__
#define __TLV320AIC3100_H__

#ifdef __AUD_TLV320AIC3100__

/* I2C bus slave registers. */
#define TLV320AIC3100_I2C_READ          0x31
#define TLV320AIC3100_I2C_WRITE         0x30

extern void tlv320aic3100_volume_updown(BOOL updown);
extern void tlv320aic3100_control(UINT mode, BYTE reg, BYTE *var);
extern void tlv320aic3100_init(void);
extern void tlv320aic3100_dac_init(void);
extern void tlv320aic3100_agc(int type);

#endif
#endif
