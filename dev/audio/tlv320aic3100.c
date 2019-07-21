#include "dev.h"

#ifdef __AUDIO__
#ifdef __AUD_TLV320AIC3100__
#include "dev.h"
#include "dev_inline.h"
#include "tlv320aic3100.h"

#ifdef DEF_CPU1	// vTaskDelay
#include "FreeRTOS.h"
#include "task.h"
#endif

//-----------------------------------------------------------------------------
// I2C control function
//-----------------------------------------------------------------------------
#define I2C_CHECK	I2c2_Check
#define I2C_WRITE	I2c2_Write
#define I2C_READ	I2c2_Read

typedef struct {
	UINT status;
	char volume;	// -127~48, not use(-128, 49~127)
}tlv320aic3100_state;

static tlv320aic3100_state tlv320aic3100_info = {
	.status = DEF_OFF,
	.volume = 0x00,
};

static UINT tlv320aic3100_check(void)
{
	return I2C_CHECK(TLV320AIC3100_I2C_WRITE);
}

static BYTE tlv320aic3100_write_reg(BYTE reg, BYTE data)
{
	while(I2C_WRITE(TLV320AIC3100_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(data,1, 0))	return DEF_FAIL;
	return DEF_OK;
}

static BYTE tlv320aic3100_read_reg(BYTE reg, BYTE *data)
{
	while(I2C_WRITE(TLV320AIC3100_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 1, 1))	return DEF_FAIL;
	while(I2C_WRITE(TLV320AIC3100_I2C_READ, 0, 0));
	*data = I2C_READ(1, 1);
	return DEF_OK;
}

void tlv320aic3100_volume_updown(BOOL updown)
{	// not use (-128, 49~127)
	if(tlv320aic3100_info.status == DEF_ON)
	{
		if(tlv320aic3100_info.volume > -127 && tlv320aic3100_info.volume < 48)
		{
			if(updown)
			{
				tlv320aic3100_info.volume++;
			}
			else
			{
				tlv320aic3100_info.volume--;
			}
			tlv320aic3100_write_reg( 0, 0x00);				// Page 0
			tlv320aic3100_write_reg(65, tlv320aic3100_info.volume);	// Left Analog Vol to HPL
			tlv320aic3100_write_reg(66, tlv320aic3100_info.volume);	// Right Analog Vol to HPR
		}
	}
}

void tlv320aic3100_control(UINT mode, BYTE reg, BYTE *var)
{
	if(tlv320aic3100_info.status == DEF_ON)
	{
		if(mode == 0)	// write
		{
			tlv320aic3100_write_reg(reg, *var);
		}
		if(mode == 1)	// read
		{
			tlv320aic3100_read_reg(reg, var);
		}
	}
}

void tlv320aic3100_delay(UINT ms)
{
#ifdef DEF_CPU0
	WaitXms(ms);
#endif
#ifdef DEF_CPU1
	vTaskDelay(ms);	// * 10 ms
#endif
}

const BYTE tlv320aic3100_tbl[][2] = {
//////////////////////////////////////////////////////////////////////

	// Codec Reset
	{0x00, 0x00},	// Page 0
	{0x01, 0x01},	// 01 Soft Reset
	{0xFE, 0x01},	// Delay

//////////////////////////////////////////////////////////////////////

#if 1
	// Codec PLL Setting(MCLK27MHz, Fs8KHz)
	{0x04, 0x03},	// 04 Clock-Gen Muxing : PLL_CLKIN = MCLK, CODEC_CLKIN = PLL_CLK
	{0x05, 0xA1},	// 05 PLL P & R : PLL divider P = 02h, multiplier R = 01h
	{0x06, 0x06},	// 06 PLL J : PLL multiplier J = 06h
	{0x07, 0x05},	// 07 PLL D MSB : PLL multiplier D MSB = 05h
	{0x08, 0xA0},	// 08 PLL D LSB : PLL multiplier D LSB = A0h (1440d)
	{0xFE, 0x01},	// Delay

	// DAC setting
	{0x0B, 0x83},	// 11 DAC NDAC_VAL : NDAC divider is powered up, NDAC divider = 03h(03d)
	{0x0C, 0xB6},	// 12 DAC MDAC_VAL : MDAC divider is powered up, MDAC divider = 36h(54d)
	{0x0D, 0x00},	// 13 DAC DOSR_VAL MSB : Oversampling Value MSB = 00h
	{0x0E, 0x40},	// 14 DAC DOSR_VAL LSB : Oversampling Value LSB = 40h (64d)
	{0xFE, 0x01},	// Delay

	// ADC setting
	{0x12, 0x83},	// 18 ADC NADC_VAL : NADC divider is powered up, NADC divider = 03h(03d)
	{0x13, 0xB6},	// 19 ADC MADC_VAL : MADC divider is powered up, MADC divider = 36h(54d)
	{0x14, 0x40},	// 20 ADC AOSR_VAL : Oversampling Value = 40h (64d)
	{0xFE, 0x01},	// Delay

	// Codec Interface - Slave Mode
	{0x1B, 0x0C},	// 27 Codec Interface Control : EN673 Slave I2S + 16bit + BCLK out + WCLK out
	{0x1D, 0x05},	// 29 Codec Interface Control2 : BDIV_CLKIN = DAC_MOD_CLK
	{0x1E, 0x82},	// 30 BCLK_N_VAL : BCLK N-divider is powered up, BCLK divider N = 02h
	{0xFE, 0x01},	// Delay
#else
	// Codec PLL Setting(MCLK27MHz, Fs44.1KHz)
	{0x04, 0x03},	// 04 Clock-Gen Muxing : PLL_CLKIN = MCLK, CODEC_CLKIN = PLL_CLK
	{0x05, 0xA1},	// 05 PLL P & R : PLL divider P = 02h, multiplier R = 01h
	{0x06, 0x06},	// 06 PLL J : PLL multiplier J = 06h
	{0x07, 0x0A},	// 07 PLL D MSB : PLL multiplier D MSB = 0Ah
	{0x08, 0xA0},	// 08 PLL D LSB : PLL multiplier D LSB = A0h (2720d)
	{0xFE, 0x01},	// Delay

	// DAC setting
	{0x0B, 0x83},	// 11 DAC NDAC_VAL : NDAC divider is powered up, NDAC divider = 03h(03d)
	{0x0C, 0x85},	// 12 DAC MDAC_VAL : MDAC divider is powered up, MDAC divider = 05h(05d)
	{0x0D, 0x00},	// 13 DAC DOSR_VAL MSB : Oversampling Value MSB = 00h
	{0x0E, 0x80},	// 14 DAC DOSR_VAL LSB : Oversampling Value LSB = 80h (128d)
	{0xFE, 0x01},	// Delay

	// ADC setting
	{0x12, 0x83},	// 18 ADC NADC_VAL : NADC divider is powered up, NADC divider = 03h(03d)
	{0x13, 0x85},	// 19 ADC MADC_VAL : MADC divider is powered up, MADC divider = 36h(54d)
	{0x14, 0x80},	// 20 ADC AOSR_VAL : Oversampling Value = 80h (128d)
	{0xFE, 0x01},	// Delay

	// Codec Interface - Slave Mode
	{0x1B, 0x0C},	// 27 Codec Interface Control : EN673 Slave I2S + 16bit + BCLK out + WCLK out
	{0x1D, 0x05},	// 29 Codec Interface Control2 : BDIV_CLKIN = DAC_MOD_CLK
	{0x1E, 0x84},	// 30 BCLK_N_VAL : BCLK N-divider is powered up, BCLK divider N = 04h
	{0xFE, 0x01},	// Delay
#endif

//////////////////////////////////////////////////////////////////////

	// Mic Setting
	{0x00, 0x01},	// Page 1
	{0x2E, 0x03},	// 46 MICBIAS : MICBIAS output is powered to AVDD.
	{0x2F, 0x8F},	// 47 MIC PGA : MIC PAG is at 0 dB. (PAG = 7.5dB)
	{0x30, 0xF0},	// 48 Delta-Sigma Mono ADC Channel Fine Gain Input Selection for P-Terminal : 
	{0x31, 0x80},	// 49 ADC Input Selection for M-Terminal : 
	{0x32, 0xC0},	// 50 Input CM Settings : 

#if 1
	{0x00, 0x04},	// Page 4	
	{0x0E, 0x7F},	// 14 FIR
	{0x0F, 0xFF},	// 15 FIR
	{0x10, 0x00},	// 16 FIR
	{0x11, 0x00},	// 17 FIR
	{0x12, 0x00},	// 18 FIR
	{0x13, 0x00},	// 19 FIR
	{0x14, 0x00},	// 20 FIR
	{0x15, 0x00},	// 21 FIR
	{0x16, 0x00},	// 22 FIR
	{0x17, 0x00},	// 23 FIR
	{0x18, 0x00},	// 24 FIR
	{0x19, 0x00},	// 25 FIR
	{0x1A, 0x00},	// 26 FIR
	{0x1B, 0x00},	// 27 FIR
	{0x1C, 0x00},	// 28 FIR
	{0x1D, 0x00},	// 29 FIR
	{0x1E, 0x00},	// 30 FIR
	{0x1F, 0x00},	// 31 FIR
	{0x20, 0x00},	// 32 FIR
	{0x21, 0x00},	// 33 FIR
	{0x22, 0x00},	// 34 FIR
	{0x23, 0x00},	// 35 FIR
	{0x24, 0x00},	// 36 FIR
	{0x25, 0x00},	// 37 FIR
	{0x26, 0x00},	// 38 FIR
	{0x27, 0x00},	// 39 FIR
	{0x28, 0x00},	// 40 FIR
	{0x29, 0x00},	// 41 FIR
	{0x2A, 0x00},	// 42 FIR
	{0x2B, 0x00},	// 43 FIR
	{0x2C, 0x00},	// 44 FIR
	{0x2D, 0x00},	// 45 FIR
	{0x2E, 0x00},	// 46 FIR
	{0x2F, 0x00},	// 47 FIR
	{0x30, 0x00},	// 48 FIR
	{0x31, 0x00},	// 49 FIR
	{0x32, 0x00},	// 50 FIR
	{0x33, 0x00},	// 51 FIR
	{0x34, 0x00},	// 52 FIR
	{0x35, 0x00},	// 53 FIR
	{0x36, 0x00},	// 54 FIR
	{0x37, 0x00},	// 55 FIR
	{0x38, 0x00},	// 56 FIR
	{0x39, 0x00},	// 57 FIR
	{0x3A, 0x00},	// 58 FIR
	{0x3B, 0x00},	// 59 FIR
	{0x3C, 0x00},	// 60 FIR
	{0x3D, 0x00},	// 61 FIR
	{0x3E, 0x00},	// 62 FIR
	{0x3F, 0x00},	// 63 FIR
#endif

	{0x00, 0x00},	// Page 0
	{0x3D, 0x06},	// 61 ADC Instruction Set  PRB_R6(Decimation filter:A, FIR:25tap, AOSR:128,64, Resource:4)
//	{0x3D, 0x0C},	// 61 ADC Instruction Set PRB_R12(Decimation filter:B, FIR:20tap, AOSR:64,     Resource:2)
//	{0x3D, 0x12},	// 61 ADC Instruction Set PRB_R18(Decimation filter:C, FIR:25tap, AOSR:32,     Resource:4)

//////////////////////////////////////////////////////////////////////

	// ADC Setting
	{0x00, 0x00},	// Page 0
	{0x51, 0xA0},	// 81 ADC Digital Mic : ADC channel is powered up, Digital microphone input is obtained from DIN pin
	{0x52, 0x00},	// 82 ADC Digital Volume Control Fine Adjust : ADC channel not muted, Delta-Sigma Mono ADC Channel Volume Control Fine Gain 0dB

//////////////////////////////////////////////////////////////////////

	// Etc.
//	{0x00, 0x00},	// Page 0
//	{0x74, 0x80},	// 116 Volume Control Pin Enable(speaker)

//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

const BYTE tlv320aic3100_dac_tbl[][2] = {
	{0x00, 0x01},	// Page 1
	{0x23, 0x40},	// 35 DAC_L and DAC_R Output Mixer Routing(0x40:SPK+HP(L), 0x44:SPK+HP(L+R), 0x88:HP(L+R))

	// Headphone
	{0x1F, 0xC4},	// 31 HeadPhone Drivers: HPL and HPR powered up
	{0x21, 0x4E},	// 33 HP Output Drivers POP Removal Settings(Power on(1.22s), Step time(3.9ms))
	{0x24, 0x36},	// 36 Left Analog Vol to HPL
	{0x25, 0x36},	// 37 Right Analog Vol to HPR
//	{0x24, 0xA0},	// 36 Left Analog Vol to HPL
//	{0x25, 0xA0},	// 37 Right Analog Vol to HPR
	{0x28, 0x0E},	// 40 HPL Driver: HPL unmute and gain 1db
	{0x29, 0x0E},	// 41 HPR Driver: HPL unmute and gain 1db

	// Class-D Speaker Amplifier
	{0x20, 0x86},	// 32 Class-D Speaker Amplifier
	{0x26, 0xA0},	// 38 Left Analog Vol to SPK
	{0x2A, 0x1C},	// 42 SPK Driver: Mono class-D driver output stage gain(24dB), not muted.

//////////////////////////////////////////////////////////////////////
	{0x00, 0x00},	// Page 0
	{0x3C, 0x10},	// 60 DAC Instruction Set PRB_P16
	{0x00, 0x08},	// Page 8
	{0x01, 0x04},	// 01 DAC Coefficient Buffer Control
	{0x00, 0x00},	// Page 0
	{0x3F, 0xD6},	// 63 DAC Data-Path Setup
	{0x40, 0x00},	// 64 DAC Volume Control

	// Setup for DRC
//	{0x41, 0x18},	// 65 DAC 12db gain left
//	{0x42, 0x18},	// 66 DAC 12db gain right
	{0x44, 0x7F},	// 68 DAC DRC enabled for both channels, Threshold = -24dB, Hysteresis = 3dB
	{0x45, 0x00},	// 69 DRC Hold Disabled
	{0x46, 0xB6},	// 70 Attack Rate = 1.9531e-4dB/Frame, DRC Decay Rate = 2.4414e-5dB/Frame

	// DRC HPF
	{0x00, 0x09},	// Page 9
	{0x0E, 0x7F},	// 
	{0x0F, 0xAB},	// 
	{0x10, 0x80},	// 
	{0x11, 0x55},	// 
	{0x12, 0x7F},	// 
	{0x13, 0x56},	// 

	// DRC LPF
	{0x14, 0x00},	// Page 9
	{0x15, 0x11},	// 
	{0x16, 0x00},	// 
	{0x17, 0x11},	// 
	{0x18, 0x00},	// 
	{0x19, 0x7F},	// 
	{0x1A, 0xDE},	// 

//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

const BYTE tlv320aic3100_agcon1_tbl[][2] = {
	{0x00, 0x00},	// Page 0
	// AGC Setting
	{0x56, 0x80},	// 86 AGC enabled, target level = -5.5 dB
	{0x57, 0xFE},	// 87 set hysteresis = disable, Noise threshold = -90 dB
	{0x58, 0x64},	// 88 set Maximum gain = 50 dB
	{0x59, 0x02},	// 89 set attack time = 20 ms
	{0x5A, 0x30},	// 90 Decay time = 576 ms
	{0x5B, 0x00},	// 91 Noise debounce time = 0 ms
	{0x5C, 0x00},	// 92 signal debounce time = 0 ms
//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

const BYTE tlv320aic3100_agcon2_tbl[][2] = {
	{0x00, 0x00},	// Page 0
	// AGC Setting
	{0x56, 0xA0},	// 86 AGC enabled, target level = -10 dB
	{0x57, 0x7E},	// 87 set hysteresis = 2 dB, Noise threshold = -90 dB
	{0x58, 0x50},	// 88 set Maximum gain = 40 dB
	{0x59, 0x02},	// 89 set attack time = 20 ms
	{0x5A, 0x30},	// 90 Decay time = 576 ms
	{0x5B, 0x00},	// 91 Noise debounce time = 0 ms
	{0x5C, 0x03},	// 92 signal debounce time = 2 ms
//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

const BYTE tlv320aic3100_agcon3_tbl[][2] = {
	{0x00, 0x00},	// Page 0
	// AGC Setting
	{0x56, 0x80},	// 86 
	{0x57, 0x00},	// 87 
	{0x58, 0x30},	// 88 
//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

const BYTE tlv320aic3100_agcoff_tbl[][2] = {
	{0x00, 0x00},	// Page 0
	{0x56, 0x00},	// 86

//////////////////////////////////////////////////////////////////////
	{0x00, 0x00}	// Page 0
};

void tlv320aic3100_agc(int type)
{
//	printf("Start %s(type:%d)\r\n", __func__, type);

	if(tlv320aic3100_info.status == DEF_ON)
	{
		const BYTE (*ppbAgcTable)[2] = NULL;
		int i, size = 0;
		switch(type)
		{
			case 0:
				size = sizeof(tlv320aic3100_agcoff_tbl) / sizeof(tlv320aic3100_agcoff_tbl[0]);
				ppbAgcTable = tlv320aic3100_agcoff_tbl;
				break;
			case 1:
				size = sizeof(tlv320aic3100_agcon1_tbl) / sizeof(tlv320aic3100_agcon1_tbl[0]);
				ppbAgcTable = tlv320aic3100_agcon1_tbl;
				break;
			case 2:
				size = sizeof(tlv320aic3100_agcon2_tbl) / sizeof(tlv320aic3100_agcon2_tbl[0]);
				ppbAgcTable = tlv320aic3100_agcon2_tbl;
				break;
			case 3:
				size = sizeof(tlv320aic3100_agcon3_tbl) / sizeof(tlv320aic3100_agcon3_tbl[0]);
				ppbAgcTable = tlv320aic3100_agcon3_tbl;
				break;
			default:
				break;
		}

		for(i=0;i<size;i++)
		{
			if(ppbAgcTable[i][0] == 0xFE)	tlv320aic3100_delay((UINT)ppbAgcTable[i][1]);
			else							tlv320aic3100_write_reg(ppbAgcTable[i][0], ppbAgcTable[i][1]);
		}
	}

//	printf("End %s(size:%d)\r\n", __func__, size);
}

void tlv320aic3100_dac_init(void)
{
//	printf("Start %s\r\n", __func__);

	if(tlv320aic3100_info.status == DEF_ON)
	{
		int i, size = sizeof(tlv320aic3100_dac_tbl) / sizeof(tlv320aic3100_dac_tbl[0]);
		for(i=0;i<size;i++)
		{
			if(tlv320aic3100_dac_tbl[i][0] == 0xFE)	tlv320aic3100_delay((UINT)tlv320aic3100_dac_tbl[i][1]);
			else									tlv320aic3100_write_reg(tlv320aic3100_dac_tbl[i][0], tlv320aic3100_dac_tbl[i][1]);
		}
	}

//	printf("End %s(size:%d)\r\n", __func__, size);
}

void tlv320aic3100_init(void)
{
//	printf("Start %s\r\n", __func__);

	tlv320aic3100_info.status = DEF_OFF;
	if(tlv320aic3100_check() == DEF_FAIL) 
	{
		printf("  >>TLV320AIC3100 Not Connected...\r\n");
	}
	else
	{
		tlv320aic3100_info.status = DEF_ON;
		printf("  >>TLV320AIC3100 Connected...\r\n");
		
		int i, size = sizeof(tlv320aic3100_tbl) / sizeof(tlv320aic3100_tbl[0]);
		for(i=0;i<size;i++)
		{
			if(tlv320aic3100_tbl[i][0] == 0xFE)	tlv320aic3100_delay((UINT)tlv320aic3100_tbl[i][1]);
			else								tlv320aic3100_write_reg(tlv320aic3100_tbl[i][0], tlv320aic3100_tbl[i][1]);
		}
	}
//	printf("End %s(size:%d)\r\n", __func__, size);
}
#endif
#endif
