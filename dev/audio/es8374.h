#ifndef __ES8374_H__
#define __ES8374_H__

#ifdef __AUD_ES8374__

/* I2C bus slave registers. */
#define ES8374_I2C_READ            0x21
#define ES8374_I2C_WRITE           0x20

#define ES8374_RESET_REG00                 0x00
#define ES8374_CLOCKMANAGER0_REG01         0x01
#define ES8374_CLOCKMANAGER1_REG02         0x02
#define ES8374_CLOCKMANAGER2_REG03         0x03
#define ES8374_CLOCKMANAGER3_REG04         0x04
#define ES8374_CLOCKMANAGER4_REG05         0x05
#define ES8374_CLOCKMANAGER5_REG06         0x06
#define ES8374_CLOCKMANAGER6_REG07         0x07
#define ES8374_CLOCKMANAGER7_REG08         0x08
#define ES8374_CLOCKMANAGER8_REG09         0x09
#define ES8374_CLOCKMANAGER9_REG0A         0x0a
#define ES8374_CLOCKMANAGER10_REG0B        0x0b
#define ES8374_CLOCKMANAGER11_REG0C        0x0c
#define ES8374_CLOCKMANAGER12_REG0D        0x0d
#define ES8374_CLOCKMANAGER13_REG0E        0x0e
#define ES8374_SDP0_REG0F                  0x0f
#define ES8374_SDP1_REG10                  0x10
#define ES8374_SDP2_REG11                  0x11
#define ES8374_SYSTEM0_REG12               0x12
#define ES8374_SYSTEM1_REG13               0x13
#define ES8374_ANALOG_REFERENCE_REG14      0x14
#define ES8374_ANALOG_POWER_DOWN_REG15     0x15
#define ES8374_ANALOG_LOWPOWER_MODE_REG16  0x16
#define ES8374_REFERENCE_POWERMODE_REG17   0x17
#define ES8374_BIAS_SELECTION_REG18        0x18
#define ES8374_MONO_OUT_SELECTION_REG1A    0x1a
#define ES8374_MONO_OUT_GAIN_REG1B         0x1b
#define ES8374_SPK_MIXER_REG1C             0x1c
#define ES8374_SPK_MIXER_GAIN_REG1D        0x1d
#define ES8374_SPEAKER_EN_REG1E            0x1e
#define ES8374_SPK_PROTECTION_REG1F        0x1f
#define ES8374_SPK_PROTECTION_REG20        0x20
#define ES8374_ADC_PGA_REG21               0x21
#define ES8374_ADC_PGA_GAIN_REG22          0x22
#define ES8374_ADC_CONTROL_REG24           0x24
#define ES8374_ADC_DIG_VOL_REG25           0x25
#define ES8374_ADC_ALC_CONTROL_REG26       0x26
#define ES8374_ADC_ALC_CONTROL_REG27       0x27
#define ES8374_ADC_ALC_CONTROL_REG28       0x28
#define ES8374_ADC_ALC_CONTROL_REG29       0x29
#define ES8374_ADC_ALC_CONTROL_REG2A       0x2a
#define ES8374_ADC_ALC_CONTROL_REG2B       0x2b
#define ES8374_ADC_HPFSET_REG2C            0x2c
#define ES8374_ADC_HPFSET_REG2D            0x2d
#define ES8374_ADC_CONTROL_REG2E           0x2e
#define ES8374_ADC_CONTROL_REG2F           0x2f
#define ES8374_ADC_CONTROL_REG30           0x30
#define ES8374_ADC_CONTROL_REG31           0x31
#define ES8374_ADC_CONTROL_REG32           0x32
#define ES8374_ADC_CONTROL_REG33           0x33
#define ES8374_ADC_CONTROL_REG34           0x34
#define ES8374_ADC_CONTROL_REG35           0x35
#define ES8374_DAC_CONTROL_REG36           0x36
#define ES8374_DAC_CONTROL_REG37           0x37
#define ES8374_DAC_DIG_VOL_REG38           0x38
#define ES8374_DAC_OFFSET_REG39            0x39
#define ES8374_INTERRUPT_CONTROL_REG6D     0x6D
#define ES8374_FLAGS_REG6E                 0x6E

extern void es8374_w_control(BYTE reg, BYTE var);
extern BYTE es8374_r_control(BYTE reg);
extern void es8374_control(UINT mode, BYTE reg, BYTE *var);
extern void es8374_volume_updown(BOOL updown);
extern void es8374_init(void);

#endif
#endif
