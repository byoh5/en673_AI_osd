#ifndef __BMA250_H__
#define __BMA250_H__

#ifdef __GSS_BMA250__

typedef struct {
	int rev  : 22;
	int data : 10;
}AccDataPoint;

typedef struct {
	UINT nSet;
	UINT nTemp;
	AccDataPoint AccX;
	AccDataPoint AccY;
	AccDataPoint AccZ;
	float32 AccNorX;
	float32 AccNorY;
	float32 AccNorZ;
}AccData;

/* I2C bus slave registers.
 * The default I2C address of the device is 0011000xb. Itis used if the SDO pin is pulled to 'GND'.
 * The alternative address 0011001xb s selected by pulling the SDO pin to 'V_DDIO'.
 */
#define BMA250_I2C_READ             0x31	// 0x33
#define BMA250_I2C_WRITE            0x30	// 0x32

/* Register */
#define BMA250_ACC_X_LSB            0x02
#define BMA250_ACC_X_MSB            0x03
#define BMA250_ACC_Y_LSB            0x04
#define BMA250_ACC_Y_MSB            0x05
#define BMA250_ACC_Z_LSB            0x06
#define BMA250_ACC_Z_MSB            0x07
#define BMA250_TEMP                 0x08

#define BMA250_G_RANGE              0x0F
#define BMA250_BANDWIDTH            0x10

#define BMA250_LOW_TH               0x23
#define BMA250_HIGH_TH              0x26

#define BMA250_OFFSET_FILT_X        0x38
#define BMA250_OFFSET_FILT_Y        0x39
#define BMA250_OFFSET_FILT_Z        0x3A

/* Temp */
#define BMA250_TEMP_DEFAULT			24	// REG(0x08) => value:0 => 24กษ
#define BMA250_TEMP_INCREMENT		0.5	// REG(0x08) => 1LSB increment of temperature sensor is 0.5กษ

/* etc. */
#define BMA250_DEF_ACC_RANGE		4	// +-4G
#define BMA250_DEF_ACC_BANDWIDTH	4	// 125Hz == 4ms

/* Function */
extern BYTE bma250_get(AccData *adOut);
extern void bma250_init(void);

#endif
#endif
