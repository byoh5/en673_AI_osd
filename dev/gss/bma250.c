#include "dev.h"

#ifdef __GSS_BMA250__
#include "bma250.h"

//-----------------------------------------------------------------------------
// I2C control function
//-----------------------------------------------------------------------------
#define I2C_CHECK	I2c2_Check
#define I2C_WRITE	I2c2_Write
#define I2C_READ	I2c2_Read

UINT bma250_status = DEF_OFF;

static UINT bma250_check(void)
{
	return I2C_CHECK(BMA250_I2C_WRITE);
}

static BYTE bma250_write_reg(BYTE reg, BYTE data)
{
	while(I2C_WRITE(BMA250_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(data,1, 0))	return DEF_FAIL;
	return DEF_OK;
}

static BYTE bma250_read_reg(BYTE reg, BYTE *data)
{
	while(I2C_WRITE(BMA250_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 1, 1))	return DEF_FAIL;
	while(I2C_WRITE(BMA250_I2C_READ, 0, 0));
	*data = I2C_READ(1, 1);
	return DEF_OK;
}

static BYTE bma250_burstread_reg(BYTE start_reg, BYTE *data, UINT len)
{
	UINT i;
	while(I2C_WRITE(BMA250_I2C_WRITE, 0, 0));
	if(I2C_WRITE(start_reg, 1, 1))	return DEF_FAIL;
	while(I2C_WRITE(BMA250_I2C_READ, 0, 0));
	for(i=0;i<len-1;i++)
	{
		data[i] = I2C_READ(0, 0);
	}
	data[i] = I2C_READ(1, 1);
	return DEF_OK;
}

//-----------------------------------------------------------------------------
// BMA250 Register
//-----------------------------------------------------------------------------

// measurement range selection
BYTE bma250_set_g_range(BYTE g)
{
	BYTE bRes = DEF_FAIL, bG;
	switch(g)
	{
		case 2:		bG = 0x03;	break;	// +-2, 3.91mg/LSB
		case 4:		bG = 0x05;	break;	// +-4, 7.81mg/LSB
		case 8:		bG = 0x08;	break;	// +-8, 15.62mg/LSB
		case 16:	bG = 0x0C;	break;	// +-16, 31.25mg/LSB
		default:	goto done;
	}
	bRes = bma250_write_reg(BMA250_G_RANGE, bG);
done:
	return bRes;
}

BYTE bma250_get_g_range(BYTE *g)
{
	BYTE bRes = DEF_FAIL, bG;
	bRes = bma250_read_reg(BMA250_G_RANGE, &bG);
	switch(bG)
	{
		case 0x03:	*g = 2;				break;
		case 0x05:	*g = 4;				break;
		case 0x08:	*g = 8;				break;
		case 0x0C:	*g = 16;			break;
		default:	bRes = DEF_FAIL;	break;
	}
	return bRes;
}

// bandwidth configuration
BYTE bma250_set_bandwidth(BYTE ms)
{
	BYTE bRes = DEF_FAIL, bMs;
	switch(ms)
	{
		case 64:	bMs = 0x08;	break;	// 64ms, 7.81Hz
		case 32:	bMs = 0x09;	break;	// 32ms, 15.63Hz
		case 16:	bMs = 0x0A;	break;	// 16ms, 31.25Hz
		case 8:		bMs = 0x0B;	break;	// 8ms, 62.5Hz
		case 4:		bMs = 0x0C;	break;	// 4ms, 125Hz
		case 2:		bMs = 0x0D;	break;	// 2ms, 250Hz
		case 1:		bMs = 0x0E;	break;	// 1ms, 500Hz
		case 0:		bMs = 0x0F;	break;	// 0.5ms, 1000Hz
		default:	goto done;
	}
	bRes = bma250_write_reg(BMA250_BANDWIDTH, bMs);
done:
	return bRes;
}

BYTE bma250_get_bandwidth(BYTE *ms)
{
	BYTE bRes = DEF_FAIL, bMs = 0;
	bRes = bma250_read_reg(BMA250_G_RANGE, &bMs);
	switch(bMs)
	{
		case 0x08:	*ms = 64;			break;
		case 0x09:	*ms = 32;			break;
		case 0x0A:	*ms = 16;			break;
		case 0x0B:	*ms = 8;			break;
		case 0x0C:	*ms = 4;			break;
		case 0x0D:	*ms = 2;			break;
		case 0x0E:	*ms = 1;			break;
		case 0x0F:	*ms = 0;			break;
		default:	bRes = DEF_FAIL;	break;
	}
	return bRes;
}

// contains the compensation value for filtered data for the x-axis, y-axis, z-axis
BYTE bma250_set_offset_filt(BYTE x, BYTE y, BYTE z)
{
	BYTE bRes[3] = {DEF_FAIL, DEF_FAIL, DEF_FAIL};
	bRes[0] = bma250_write_reg(BMA250_OFFSET_FILT_X, x);	// offset_filt_x
	bRes[1] = bma250_write_reg(BMA250_OFFSET_FILT_Y, y);	// offset_filt_y
	bRes[2] = bma250_write_reg(BMA250_OFFSET_FILT_Z, z);	// offset_filt_z
	return bRes[0] & bRes[1] & bRes[2];
}

BYTE bma250_get_offset_filt(BYTE *x, BYTE *y, BYTE *z)
{
	BYTE bRes[3] = {DEF_FAIL, DEF_FAIL, DEF_FAIL};
	bRes[0] = bma250_read_reg(BMA250_OFFSET_FILT_X, x);		// offset_filt_x
	bRes[1] = bma250_read_reg(BMA250_OFFSET_FILT_Y, y);		// offset_filt_y
	bRes[2] = bma250_read_reg(BMA250_OFFSET_FILT_Z, z);		// offset_filt_z
	return bRes[0] & bRes[1] & bRes[2];
}

// contains the threshold definition for the high-g interrupt. Default value of HIGH_TH is 0xC0.
BYTE bma250_set_high_th(BYTE high_th)
{
	return bma250_write_reg(BMA250_HIGH_TH, high_th);		// high_th
}

BYTE bma250_get_high_th(BYTE *high_th)
{
	return bma250_read_reg(BMA250_HIGH_TH, high_th);		// high_th
}

// contains the threshold definition for the low-g interrupt. Default value of LOW_TH is 0x30.
BYTE bma250_set_low_th(BYTE low_th)
{
	return bma250_write_reg(BMA250_LOW_TH, low_th);			// low_th
}

BYTE bma250_get_low_th(BYTE *low_th)
{
	return bma250_read_reg(BMA250_LOW_TH, low_th);			// low_th
}

BYTE bma250_get(AccData *adOut)
{
	BYTE bpTBL[7] = {0}; // BMA250_ACC_X_LSB, BMA250_ACC_X_MSB, BMA250_ACC_Y_LSB, BMA250_ACC_Y_MSB, BMA250_ACC_Z_LSB, BMA250_ACC_Z_MSB, BMA250_TEMP
	BYTE bRes = DEF_FAIL;
	if(bma250_status == DEF_ON)
		bRes = bma250_burstread_reg(BMA250_ACC_X_LSB, bpTBL, sizeof(bpTBL));
	if(bRes == DEF_OK)
	{
		adOut->nTemp = (bpTBL[6] * BMA250_TEMP_INCREMENT) + BMA250_TEMP_DEFAULT;
		adOut->AccX.data = (((UINT)(bpTBL[1] & 0xFF)) << 2) | (((UINT)(bpTBL[0] >> 6)) & 0x00000003);
		adOut->AccY.data = (((UINT)(bpTBL[3] & 0xFF)) << 2) | (((UINT)(bpTBL[2] >> 6)) & 0x00000003);
		adOut->AccZ.data = (((UINT)(bpTBL[5] & 0xFF)) << 2) | (((UINT)(bpTBL[4] >> 6)) & 0x00000003);

		// normalization
#if 1	// EX-RISC float
		float32 var1 = _itof(BMA250_DEF_ACC_RANGE * 2);
		float32 var2 = _itof(1023);
		float32 unit = _fdiv(var1, var2);
		float32 data;
		data = _itof(adOut->AccX.data);
		adOut->AccNorX = _fmul(unit, data);
		data = _itof(adOut->AccY.data);
		adOut->AccNorY = _fmul(unit, data);
		data = _itof(adOut->AccZ.data);
		adOut->AccNorZ = _fmul(unit, data);
#else	// S/W float
		float unit = (BMA250_DEF_ACC_RANGE * 2) / 1023.0;
		adOut->AccNorX = unit * adOut->AccX.data;
		adOut->AccNorY = unit * adOut->AccY.data;
		adOut->AccNorZ = unit * adOut->AccZ.data;
#endif
		adOut->nSet = DEF_OK;
	}
	else
	{
		adOut->nTemp = 0;
		adOut->AccX.data = 0;
		adOut->AccY.data = 0;
		adOut->AccZ.data = 0;
		adOut->AccNorX = _itof(0);
		adOut->AccNorY = _itof(0);
		adOut->AccNorZ = _itof(0);
		adOut->nSet = DEF_FAIL;
	}

	return bRes;
}

void bma250_init(void)
{
	bma250_status = DEF_OFF;
	if(bma250_check() == DEF_FAIL) 
	{
		printf("  >>BMA250 Not Connected...\r\n");
	}
	else
	{
		bma250_status = DEF_ON;
		printf(" >>BMA250 Connected...\r\n");

		// sensor setup
		// base data sheet : BMA250_DS002-05
		bma250_set_g_range(BMA250_DEF_ACC_RANGE);			// +-4G
		bma250_set_bandwidth(BMA250_DEF_ACC_BANDWIDTH);	// 125Hz == 4ms
#if 1
		bma250_set_offset_filt(0, 0x80, 0);
#else
		IicW8(Gsensor_addr, 0x36, 128, &bErr);	// offset reset
#endif

//		IicW8(Gsensor_addr, 0x16, 0x40, &bErr);				// interrupt enable(orientation)
//		IicW8(Gsensor_addr, 0x0C, 0x04|0x02|0x01, &bErr);	// 
//		IicW8(Gsensor_addr, 0x17, 0x04|0x02|0x01, &bErr);	// interrupt enable(high-g z,y,x)
//		IicW8(Gsensor_addr, 0x19, 0x02, &bErr);				// high-g interrupt enable(INT1)
//		IicW8(Gsensor_addr, 0x1B, 0x40, &bErr);				// orientation interrupt enable(INT2)
//		IicW8(Gsensor_addr, 0x17, 0x08, &bErr);				// interrupt enable(low-g)
//		IicW8(Gsensor_addr, 0x19, 0x01, &bErr);				// low-g interrupt enable(INT1)
	}
}
#endif
