#ifndef _DEV_REGS_ISP_H_
#define	_DEV_REGS_ISP_H_

_regs_	UINT _res0				: 2;
		UINT FORCE_ABT_SOFF		: 1;
		UINT FORCE_ABT_SON		: 1;
		UINT _res1				:28;	_rege_	_ISP_0002;

#define ISP_FORCE_ABT_SOFF		_bm(_ISP_0002, ISP_REG_BASE, (0x0002<<2), FORCE_ABT_SOFF	)	//	
#define ISP_FORCE_ABT_SON		_bm(_ISP_0002, ISP_REG_BASE, (0x0002<<2), FORCE_ABT_SON		)	//	
#define ISP_0002_T				_am(_ISP_0002, ISP_REG_BASE, (0x0002<<2))						//	

_regs_	UINT SYNC_UP			: 1;
		UINT _res0				:31;	_rege_	_ISP_0003;

#define ISP_SYNC_UP				_bm(_ISP_0003, ISP_REG_BASE, (0x0003<<2), SYNC_UP			)	//	
#define ISP_0003_T				_am(_ISP_0003, ISP_REG_BASE, (0x0003<<2))						//	

_regs_	UINT PCLKI1_PD			: 1;
		UINT FPCK_PD			: 1;
		UINT PPCK_PD			: 1;
		UINT PSCK_PD			: 1;
		UINT PR_CK_PD			: 1;
		UINT FN_CK_PD			: 1;
		UINT _res0				:26;	_rege_	_ISP_0005;

#define ISP_PCLKI1_PD			_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), PCLKI1_PD			)	//	
#define ISP_FPCK_PD				_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), FPCK_PD			)	//	
#define ISP_PPCK_PD				_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), PPCK_PD			)	//	
#define ISP_PSCK_PD				_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), PSCK_PD			)	//	
#define ISP_PR_CK_PD			_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), PR_CK_PD			)	//	
#define ISP_FN_CK_PD			_bm(_ISP_0005, ISP_REG_BASE, (0x0005<<2), FN_CK_PD			)	//	
#define ISP_0005_T				_am(_ISP_0005, ISP_REG_BASE, (0x0005<<2))						//	

_regs_	UINT _res0				: 7;
		UINT YCW_CK0_PD			: 1;
		UINT YCW_CK1_PD			: 1;
		UINT YCW_CK2_PD			: 1;
		UINT IMD_CK_PD			: 1;
		UINT JPGCK_PD			: 1;
		UINT CDS0_PCK_PD		: 1;
		UINT CDS1_PCK_PD		: 1;
		UINT PIP_PPCK_PD		: 1;
		UINT IBT_PCK_PD			: 1;
		UINT IBT0_PCK_PD		: 1;
		UINT BT_PCK_PD			: 1;
		UINT BT0_PCK_PD			: 1;
		UINT YCR_CK0_PD			: 1;
		UINT IPUIRQ0_PD			: 1;
		UINT IPUIRQ1_PD			: 1;
		UINT PCLK_ON			: 1;
		UINT PCKO0_ON			: 1;
		UINT PCKO1_ON			: 1;
		UINT _res1				: 1;
		UINT PCKO_OEN			: 1;
		UINT BT1120_PCK_PD		: 1;
		UINT BT1120_PCK0_PD		: 1;
		UINT _res2				: 3;	_rege_	_ISP_0006;

#define ISP_YCW_CK0_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), YCW_CK0_PD		)	//	
#define ISP_YCW_CK1_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), YCW_CK1_PD		)	//	
#define ISP_YCW_CK2_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), YCW_CK2_PD		)	//	
#define ISP_IMD_CK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), IMD_CK_PD			)	//	
#define ISP_JPGCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), JPGCK_PD			)	//	
#define ISP_CDS0_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), CDS0_PCK_PD		)	//	
#define ISP_CDS1_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), CDS1_PCK_PD		)	//	
#define ISP_PIP_PPCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), PIP_PPCK_PD		)	//	
#define ISP_IBT_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), IBT_PCK_PD		)	//	
#define ISP_IBT0_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), IBT0_PCK_PD		)	//	
#define ISP_BT_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), BT_PCK_PD			)	//	
#define ISP_BT0_PCK_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), BT0_PCK_PD		)	//	
#define ISP_YCR_CK0_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), YCR_CK0_PD		)	//	
#define ISP_IPUIRQ0_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), IPUIRQ0_PD		)	//	
#define ISP_IPUIRQ1_PD			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), IPUIRQ1_PD		)	//	
#define ISP_PCLK_ON				_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), PCLK_ON			)	//	
#define ISP_PCKO0_ON			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), PCKO0_ON			)	//	
#define ISP_PCKO1_ON			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), PCKO1_ON			)	//	
#define ISP_PCKO_OEN			_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), PCKO_OEN			)	//	
#define ISP_BT1120_PCK_PD		_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), BT1120_PCK_PD		)	//	
#define ISP_BT1120_PCK0_PD		_bm(_ISP_0006, ISP_REG_BASE, (0x0006<<2), BT1120_PCK0_PD	)	//	
#define ISP_0006_T				_am(_ISP_0006, ISP_REG_BASE, (0x0006<<2))						//	

_regs_	UINT _res0				:16;
		UINT PCLK1_DLY			: 4;
		UINT PCLK_DLY			: 4;
		UINT _res1				: 8;	_rege_	_ISP_0007;

#define ISP_PCLK1_DLY			_bm(_ISP_0007, ISP_REG_BASE, (0x0007<<2), PCLK1_DLY			)	//	
#define ISP_PCLK_DLY			_bm(_ISP_0007, ISP_REG_BASE, (0x0007<<2), PCLK_DLY			)	//	
#define ISP_0007_T				_am(_ISP_0007, ISP_REG_BASE, (0x0007<<2))						//	

_regs_	UINT VIRQI_EN			: 1;
		UINT VIRQW_EN			: 1;
		UINT VIRQO_EN			: 1;
		UINT UIRQ0_EN			: 1;
		UINT UIRQ1_EN			: 1;
		UINT VIRQW2_EN			: 1;
		UINT _res0				:18;
		UINT CLI_VLOCKI			: 1;
		UINT CLI_VLOCKW			: 1;
		UINT CLI_VLOCKO			: 1;
		UINT CLI_UIRQ0			: 1;
		UINT CLI_UIRQ1			: 1;
		UINT CLI_VLOCKW2		: 1;
		UINT _res1				: 2;	_rege_	_ISP_0008;

#define ISP_VIRQI_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), VIRQI_EN			)	//	
#define ISP_VIRQW_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), VIRQW_EN			)	//	
#define ISP_VIRQO_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), VIRQO_EN			)	//	
#define ISP_UIRQ0_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), UIRQ0_EN			)	//	
#define ISP_UIRQ1_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), UIRQ1_EN			)	//	
#define ISP_VIRQW2_EN			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), VIRQW2_EN			)	//	
#define ISP_CLI_VLOCKI			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_VLOCKI		)	//	
#define ISP_CLI_VLOCKW			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_VLOCKW		)	//	
#define ISP_CLI_VLOCKO			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_VLOCKO		)	//	
#define ISP_CLI_UIRQ0			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_UIRQ0			)	//	
#define ISP_CLI_UIRQ1			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_UIRQ1			)	//	
#define ISP_CLI_VLOCKW2			_bm(_ISP_0008, ISP_REG_BASE, (0x0008<<2), CLI_VLOCKW2		)	//	
#define ISP_0008_T				_am(_ISP_0008, ISP_REG_BASE, (0x0008<<2))						//	

_regs_	UINT _res0				: 2;
		UINT BT_TRG_DIN0		: 1;
		UINT BT_TRG_HSI0		: 1;
		UINT BT_TRG_VSI0		: 1;
		UINT BT_POL_HSI0		: 1;
		UINT BT_YC_HSPI0		:13;
		UINT BT_YC_HW0			:13;	_rege_	_ISP_0009;

#define ISP_BT_TRG_DIN0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_TRG_DIN0		)	//	
#define ISP_BT_TRG_HSI0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_TRG_HSI0		)	//	
#define ISP_BT_TRG_VSI0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_TRG_VSI0		)	//	
#define ISP_BT_POL_HSI0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_POL_HSI0		)	//	
#define ISP_BT_YC_HSPI0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_YC_HSPI0		)	//	
#define ISP_BT_YC_HW0			_bm(_ISP_0009, ISP_REG_BASE, (0x0009<<2), BT_YC_HW0			)	//	
#define ISP_0009_T				_am(_ISP_0009, ISP_REG_BASE, (0x0009<<2))						//	

_regs_	UINT TRG_DIN0			: 1;
		UINT TRG_HSI0			: 1;
		UINT TRG_VSI0			: 1;
		UINT POL_HSI0			: 1;
		UINT POL_VSI0			: 1;
		UINT ASYNC_ON0			: 1;
		UINT VSYN_NAON0			: 1;
		UINT BT_MODESD0			: 1;
		UINT BT1120_MOD0		: 1;
		UINT BTBLK_RON0			: 1;
		UINT YC_MOD0			: 1;
		UINT YCBCR_MOD0			: 1;
		UINT YC_FLG_INV0		: 1;
		UINT CBR_FLG_INV0		: 1;
		UINT TRS_REON0			: 1;
		UINT DSENS_ON			: 1;
		UINT ASYNC_VPOS0		:13;
		UINT INSEL0				: 3;	_rege_	_ISP_000A;

#define ISP_TRG_DIN0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), TRG_DIN0			)	//	
#define ISP_TRG_HSI0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), TRG_HSI0			)	//	
#define ISP_TRG_VSI0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), TRG_VSI0			)	//	
#define ISP_POL_HSI0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), POL_HSI0			)	//	
#define ISP_POL_VSI0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), POL_VSI0			)	//	
#define ISP_ASYNC_ON0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), ASYNC_ON0			)	//	
#define ISP_VSYN_NAON0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), VSYN_NAON0		)	//	
#define ISP_BT_MODESD0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), BT_MODESD0		)	//	
#define ISP_BT1120_MOD0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), BT1120_MOD0		)	//	
#define ISP_BTBLK_RON0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), BTBLK_RON0		)	//	
#define ISP_YC_MOD0				_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), YC_MOD0			)	//	
#define ISP_YCBCR_MOD0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), YCBCR_MOD0		)	//	
#define ISP_YC_FLG_INV0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), YC_FLG_INV0		)	//	
#define ISP_CBR_FLG_INV0		_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), CBR_FLG_INV0		)	//	
#define ISP_TRS_REON0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), TRS_REON0			)	//	
#define ISP_DSENS_ON			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), DSENS_ON			)	//	
#define ISP_ASYNC_VPOS0			_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), ASYNC_VPOS0		)	//	
#define ISP_INSEL0				_bm(_ISP_000A, ISP_REG_BASE, (0x000A<<2), INSEL0			)	//	
#define ISP_000A_T				_am(_ISP_000A, ISP_REG_BASE, (0x000A<<2))						//	

_regs_	UINT DI0_BYP			: 1;
		UINT BT0_656_SEL		: 1;
		UINT BT_OBUF_RST0		: 1;
		UINT _res0				: 1;
		UINT VLOCKI_POS0		:12;
		UINT BT0_656_DSEL		: 1;
		UINT _res1				: 3;
		UINT HLOCKI_POS0		:12;	_rege_	_ISP_000B;

#define ISP_DI0_BYP				_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), DI0_BYP			)	//	
#define ISP_BT0_656_SEL			_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), BT0_656_SEL		)	//	
#define ISP_BT_OBUF_RST0		_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), BT_OBUF_RST0		)	//	
#define ISP_VLOCKI_POS0			_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), VLOCKI_POS0		)	//	
#define ISP_BT0_656_DSEL		_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), BT0_656_DSEL		)	//	
#define ISP_HLOCKI_POS0			_bm(_ISP_000B, ISP_REG_BASE, (0x000B<<2), HLOCKI_POS0		)	//	
#define ISP_000B_T				_am(_ISP_000B, ISP_REG_BASE, (0x000B<<2))						//	

_regs_	UINT BT_SM				: 1;
		UINT BT_SM0				: 1;
		UINT BT0_MD_OSD_ON		: 1;
		UINT _res0				: 1;
		UINT HSPI0				:12;
		UINT BIT_EXT_ON			: 1;
		UINT BIT_EXT_ON0		: 1;
		UINT _res1				: 2;
		UINT VSPI0				:12;	_rege_	_ISP_000C;

#define ISP_BT_SM				_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), BT_SM				)	//	
#define ISP_BT_SM0				_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), BT_SM0			)	//	
#define ISP_BT0_MD_OSD_ON		_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), BT0_MD_OSD_ON		)	//	
#define ISP_HSPI0				_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), HSPI0				)	//	
#define ISP_BIT_EXT_ON			_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), BIT_EXT_ON		)	//	
#define ISP_BIT_EXT_ON0			_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), BIT_EXT_ON0		)	//	
#define ISP_VSPI0				_bm(_ISP_000C, ISP_REG_BASE, (0x000C<<2), VSPI0				)	//	
#define ISP_000C_T				_am(_ISP_000C, ISP_REG_BASE, (0x000C<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HWI0				:12;
		UINT _res1				: 4;
		UINT VWI0				:12;	_rege_	_ISP_000D;

#define ISP_HWI0				_bm(_ISP_000D, ISP_REG_BASE, (0x000D<<2), HWI0				)	//	
#define ISP_VWI0				_bm(_ISP_000D, ISP_REG_BASE, (0x000D<<2), VWI0				)	//	
#define ISP_000D_T				_am(_ISP_000D, ISP_REG_BASE, (0x000D<<2))						//	

_regs_	UINT BT_OBUF_RST		: 1;
		UINT _res0				: 2;
		UINT ASYNC_VST0			:13;
		UINT ASYNC_HW0			: 9;
		UINT BT1120_MOD			: 1;
		UINT BTBLK_RON			: 1;
		UINT YC_MOD				: 1;
		UINT BT_MODESD			: 1;
		UINT BT_656_SEL			: 1;
		UINT BT_656_DSEL		: 1;
		UINT BT_POL_HSI			: 1;	_rege_	_ISP_000E;

#define ISP_BT_OBUF_RST			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT_OBUF_RST		)	//	
#define ISP_ASYNC_VST0			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), ASYNC_VST0		)	//	
#define ISP_ASYNC_HW0			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), ASYNC_HW0			)	//	
#define ISP_BT1120_MOD			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT1120_MOD		)	//	
#define ISP_BTBLK_RON			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BTBLK_RON			)	//	
#define ISP_YC_MOD				_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), YC_MOD			)	//	
#define ISP_BT_MODESD			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT_MODESD			)	//	
#define ISP_BT_656_SEL			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT_656_SEL		)	//	
#define ISP_BT_656_DSEL			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT_656_DSEL		)	//	
#define ISP_BT_POL_HSI			_bm(_ISP_000E, ISP_REG_BASE, (0x000E<<2), BT_POL_HSI		)	//	
#define ISP_000E_T				_am(_ISP_000E, ISP_REG_BASE, (0x000E<<2))						//	

_regs_	UINT BT_MD_OSD_ON		: 1;
		UINT YCBCR_MOD			: 1;
		UINT YC_FLG_INV			: 1;
		UINT CBR_FLG_INV		: 1;
		UINT BT_DIN_SEL			: 1;
		UINT TRS_REON			: 1;
		UINT BT_YC_HSPI			:13;
		UINT BT_YC_HW			:13;	_rege_	_ISP_000F;

#define ISP_BT_MD_OSD_ON		_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), BT_MD_OSD_ON		)	//	
#define ISP_YCBCR_MOD			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), YCBCR_MOD			)	//	
#define ISP_YC_FLG_INV			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), YC_FLG_INV		)	//	
#define ISP_CBR_FLG_INV			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), CBR_FLG_INV		)	//	
#define ISP_BT_DIN_SEL			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), BT_DIN_SEL		)	//	
#define ISP_TRS_REON			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), TRS_REON			)	//	
#define ISP_BT_YC_HSPI			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), BT_YC_HSPI		)	//	
#define ISP_BT_YC_HW			_bm(_ISP_000F, ISP_REG_BASE, (0x000F<<2), BT_YC_HW			)	//	
#define ISP_000F_T				_am(_ISP_000F, ISP_REG_BASE, (0x000F<<2))						//	

_regs_	UINT IBT_PCK_SEL		: 1;
		UINT _res0				: 1;
		UINT FPCK_SEL			: 2;
		UINT BT_PCK_SEL			: 2;
		UINT PPCK_SEL			: 2;
		UINT _res1				: 1;
		UINT PSCK_SEL			: 3;
		UINT _res2				: 1;
		UINT PCKO0_SEL			: 3;
		UINT _res3				: 1;
		UINT FN_CK_SEL			: 3;
		UINT _res4				: 1;
		UINT PR_CK_SEL			: 3;
		UINT _res5				: 1;
		UINT IMD_CK_SEL			: 3;
		UINT _res6				: 1;
		UINT PIP_PPCK_SEL		: 3;	_rege_	_ISP_0010;

#define ISP_IBT_PCK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), IBT_PCK_SEL		)	//	
#define ISP_FPCK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), FPCK_SEL			)	//	
#define ISP_BT_PCK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), BT_PCK_SEL		)	//	
#define ISP_PPCK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), PPCK_SEL			)	//	
#define ISP_PSCK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), PSCK_SEL			)	//	
#define ISP_PCKO0_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), PCKO0_SEL			)	//	
#define ISP_FN_CK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), FN_CK_SEL			)	//	
#define ISP_PR_CK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), PR_CK_SEL			)	//	
#define ISP_IMD_CK_SEL			_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), IMD_CK_SEL		)	//	
#define ISP_PIP_PPCK_SEL		_bm(_ISP_0010, ISP_REG_BASE, (0x0010<<2), PIP_PPCK_SEL		)	//	
#define ISP_0010_T				_am(_ISP_0010, ISP_REG_BASE, (0x0010<<2))						//	

_regs_	UINT IBT0_PCK_SEL		: 1;
		UINT _res0				: 2;
		UINT BT0_PCK_SEL		: 1;
		UINT _res1				: 1;
		UINT PCKO1_SEL			: 3;
		UINT _res2				: 1;
		UINT YCW_CK0_SEL		: 3;
		UINT _res3				: 1;
		UINT YCW_CK1_SEL		: 3;
		UINT _res4				: 1;
		UINT YCW_CK2_SEL		: 3;
		UINT YCRCK0_SEL			: 3;
		UINT CDS0_PCK_SEL		: 3;
		UINT CDS1_PCK_SEL		: 3;
		UINT JPGCK_SEL			: 3;	_rege_	_ISP_0011;

#define ISP_IBT0_PCK_SEL		_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), IBT0_PCK_SEL		)	//	
#define ISP_BT0_PCK_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), BT0_PCK_SEL		)	//	
#define ISP_PCKO1_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), PCKO1_SEL			)	//	
#define ISP_YCW_CK0_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), YCW_CK0_SEL		)	//	
#define ISP_YCW_CK1_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), YCW_CK1_SEL		)	//	
#define ISP_YCW_CK2_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), YCW_CK2_SEL		)	//	
#define ISP_YCRCK0_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), YCRCK0_SEL		)	//	
#define ISP_CDS0_PCK_SEL		_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), CDS0_PCK_SEL		)	//	
#define ISP_CDS1_PCK_SEL		_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), CDS1_PCK_SEL		)	//	
#define ISP_JPGCK_SEL			_bm(_ISP_0011, ISP_REG_BASE, (0x0011<<2), JPGCK_SEL			)	//	
#define ISP_0011_T				_am(_ISP_0011, ISP_REG_BASE, (0x0011<<2))						//	

_regs_	UINT SLV				: 2;
		UINT CH_DSEL			: 1;
		UINT CH_HSSEL			: 1;
		UINT CH_VSSEL			: 1;
		UINT TRG_DIN			: 1;
		UINT TRG_HSI			: 1;
		UINT TRG_VSI			: 1;
		UINT POL_HSI			: 1;
		UINT POL_VSI			: 1;
		UINT SHSO_INV			: 1;
		UINT SVSO_INV			: 1;
		UINT SHSO_TRG			: 1;
		UINT SVSO_TRG			: 1;
		UINT IDDR_ON			: 1;
		UINT ISDR_OP			: 1;
		UINT DDRFF_RS			: 1;
		UINT SORD				: 1;
		UINT CATP				: 1;
		UINT ASYNC_ON			: 1;
		UINT VSYN_NAON			: 1;
		UINT PDI_MOD			: 2;
		UINT SYNC_BYS			: 1;
		UINT _res0				: 2;
		UINT ICSEL				: 2;
		UINT INSEL_MOD			: 1;
		UINT INSEL				: 3;	_rege_	_ISP_0012;

#define ISP_SLV					_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SLV				)	//	
#define ISP_CH_DSEL				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), CH_DSEL			)	//	
#define ISP_CH_HSSEL			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), CH_HSSEL			)	//	
#define ISP_CH_VSSEL			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), CH_VSSEL			)	//	
#define ISP_TRG_DIN				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), TRG_DIN			)	//	
#define ISP_TRG_HSI				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), TRG_HSI			)	//	
#define ISP_TRG_VSI				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), TRG_VSI			)	//	
#define ISP_POL_HSI				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), POL_HSI			)	//	
#define ISP_POL_VSI				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), POL_VSI			)	//	
#define ISP_SHSO_INV			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SHSO_INV			)	//	
#define ISP_SVSO_INV			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SVSO_INV			)	//	
#define ISP_SHSO_TRG			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SHSO_TRG			)	//	
#define ISP_SVSO_TRG			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SVSO_TRG			)	//	
#define ISP_IDDR_ON				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), IDDR_ON			)	//	
#define ISP_ISDR_OP				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), ISDR_OP			)	//	
#define ISP_DDRFF_RS			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), DDRFF_RS			)	//	
#define ISP_SORD				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SORD				)	//	
#define ISP_CATP				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), CATP				)	//	
#define ISP_ASYNC_ON			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), ASYNC_ON			)	//	
#define ISP_VSYN_NAON			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), VSYN_NAON			)	//	
#define ISP_PDI_MOD				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), PDI_MOD			)	//	
#define ISP_SYNC_BYS			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), SYNC_BYS			)	//	
#define ISP_ICSEL				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), ICSEL				)	//	
#define ISP_INSEL_MOD			_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), INSEL_MOD			)	//	
#define ISP_INSEL				_bm(_ISP_0012, ISP_REG_BASE, (0x0012<<2), INSEL				)	//	
#define ISP_0012_T				_am(_ISP_0012, ISP_REG_BASE, (0x0012<<2))						//	

_regs_	UINT HTWI				:16;
		UINT VTWI				:16;	_rege_	_ISP_0013;

#define ISP_HTWI				_bm(_ISP_0013, ISP_REG_BASE, (0x0013<<2), HTWI				)	//	
#define ISP_VTWI				_bm(_ISP_0013, ISP_REG_BASE, (0x0013<<2), VTWI				)	//	
#define ISP_0013_T				_am(_ISP_0013, ISP_REG_BASE, (0x0013<<2))						//	

_regs_	UINT PRS_HZ				: 1;
		UINT POS_HZ				: 1;
		UINT _res0				: 2;
		UINT VLOCKI_POS			:12;
		UINT _res1				: 3;
		UINT HLOCKI_POS			:13;	_rege_	_ISP_0014;

#define ISP_PRS_HZ				_bm(_ISP_0014, ISP_REG_BASE, (0x0014<<2), PRS_HZ			)	//	
#define ISP_POS_HZ				_bm(_ISP_0014, ISP_REG_BASE, (0x0014<<2), POS_HZ			)	//	
#define ISP_VLOCKI_POS			_bm(_ISP_0014, ISP_REG_BASE, (0x0014<<2), VLOCKI_POS		)	//	
#define ISP_HLOCKI_POS			_bm(_ISP_0014, ISP_REG_BASE, (0x0014<<2), HLOCKI_POS		)	//	
#define ISP_0014_T				_am(_ISP_0014, ISP_REG_BASE, (0x0014<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HSPI				:12;
		UINT _res1				: 4;
		UINT VSPI				:12;	_rege_	_ISP_0015;

#define ISP_HSPI				_bm(_ISP_0015, ISP_REG_BASE, (0x0015<<2), HSPI				)	//	
#define ISP_VSPI				_bm(_ISP_0015, ISP_REG_BASE, (0x0015<<2), VSPI				)	//	
#define ISP_0015_T				_am(_ISP_0015, ISP_REG_BASE, (0x0015<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HWI				:12;
		UINT _res1				: 4;
		UINT VWI				:12;	_rege_	_ISP_0016;

#define ISP_HWI					_bm(_ISP_0016, ISP_REG_BASE, (0x0016<<2), HWI				)	//	
#define ISP_VWI					_bm(_ISP_0016, ISP_REG_BASE, (0x0016<<2), VWI				)	//	
#define ISP_0016_T				_am(_ISP_0016, ISP_REG_BASE, (0x0016<<2))						//	

_regs_	UINT HTWO				:16;
		UINT VTWO				:16;	_rege_	_ISP_0017;

#define ISP_HTWO				_bm(_ISP_0017, ISP_REG_BASE, (0x0017<<2), HTWO				)	//	
#define ISP_VTWO				_bm(_ISP_0017, ISP_REG_BASE, (0x0017<<2), VTWO				)	//	
#define ISP_0017_T				_am(_ISP_0017, ISP_REG_BASE, (0x0017<<2))						//	

_regs_	UINT OSYNC_MOD			: 1;
		UINT _res0				: 3;
		UINT VLOCKI2_POS		:12;
		UINT _res1				: 4;
		UINT HLOCKI2_POS		:12;	_rege_	_ISP_0018;

#define ISP_OSYNC_MOD			_bm(_ISP_0018, ISP_REG_BASE, (0x0018<<2), OSYNC_MOD			)	//	
#define ISP_VLOCKI2_POS			_bm(_ISP_0018, ISP_REG_BASE, (0x0018<<2), VLOCKI2_POS		)	//	
#define ISP_HLOCKI2_POS			_bm(_ISP_0018, ISP_REG_BASE, (0x0018<<2), HLOCKI2_POS		)	//	
#define ISP_0018_T				_am(_ISP_0018, ISP_REG_BASE, (0x0018<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HSPO				:12;
		UINT _res1				: 4;
		UINT VSPO				:12;	_rege_	_ISP_0019;

#define ISP_HSPO				_bm(_ISP_0019, ISP_REG_BASE, (0x0019<<2), HSPO				)	//	
#define ISP_VSPO				_bm(_ISP_0019, ISP_REG_BASE, (0x0019<<2), VSPO				)	//	
#define ISP_0019_T				_am(_ISP_0019, ISP_REG_BASE, (0x0019<<2))						//	

_regs_	UINT HVWIO_SYN			: 1;
		UINT _res0				: 3;
		UINT HWO				:12;
		UINT _res1				: 4;
		UINT VWO				:12;	_rege_	_ISP_001A;

#define ISP_HVWIO_SYN			_bm(_ISP_001A, ISP_REG_BASE, (0x001A<<2), HVWIO_SYN			)	//	
#define ISP_HWO					_bm(_ISP_001A, ISP_REG_BASE, (0x001A<<2), HWO				)	//	
#define ISP_VWO					_bm(_ISP_001A, ISP_REG_BASE, (0x001A<<2), VWO				)	//	
#define ISP_001A_T				_am(_ISP_001A, ISP_REG_BASE, (0x001A<<2))						//	

_regs_	UINT HSYNC_OFS_ON		: 1;
		UINT HSYNC_OFS_VFLG_SEL	: 1;
		UINT HSYNC_OFS_INV		: 1;
		UINT _res0				: 2;
		UINT HSYNC_OFS			: 3;
		UINT _res1				:24;	_rege_	_ISP_001C;

#define ISP_HSYNC_OFS_ON		_bm(_ISP_001C, ISP_REG_BASE, (0x001C<<2), HSYNC_OFS_ON		)	//	
#define ISP_HSYNC_OFS_VFLG_SEL	_bm(_ISP_001C, ISP_REG_BASE, (0x001C<<2), HSYNC_OFS_VFLG_SEL	)	//	
#define ISP_HSYNC_OFS_INV		_bm(_ISP_001C, ISP_REG_BASE, (0x001C<<2), HSYNC_OFS_INV		)	//	
#define ISP_HSYNC_OFS			_bm(_ISP_001C, ISP_REG_BASE, (0x001C<<2), HSYNC_OFS			)	//	
#define ISP_001C_T				_am(_ISP_001C, ISP_REG_BASE, (0x001C<<2))						//	

_regs_	UINT OCSEL				: 2;
		UINT _res0				: 2;
		UINT YDLY				: 4;
		UINT _res1				:24;	_rege_	_ISP_001E;

#define ISP_OCSEL				_bm(_ISP_001E, ISP_REG_BASE, (0x001E<<2), OCSEL				)	//	
#define ISP_YDLY				_bm(_ISP_001E, ISP_REG_BASE, (0x001E<<2), YDLY				)	//	
#define ISP_001E_T				_am(_ISP_001E, ISP_REG_BASE, (0x001E<<2))						//	

_regs_	UINT REG_UPMODE			: 1;
		UINT _res0				:31;	_rege_	_ISP_001F;

#define ISP_REG_UPMODE			_bm(_ISP_001F, ISP_REG_BASE, (0x001F<<2), REG_UPMODE		)	//	
#define ISP_001F_T				_am(_ISP_001F, ISP_REG_BASE, (0x001F<<2))						//	

_regs_	UINT SHSO_FP			:16;
		UINT SHSO_RP			:16;	_rege_	_ISP_0020;

#define ISP_SHSO_FP				_bm(_ISP_0020, ISP_REG_BASE, (0x0020<<2), SHSO_FP			)	//	
#define ISP_SHSO_RP				_bm(_ISP_0020, ISP_REG_BASE, (0x0020<<2), SHSO_RP			)	//	
#define ISP_0020_T				_am(_ISP_0020, ISP_REG_BASE, (0x0020<<2))						//	

_regs_	UINT SVSO_FP			:16;
		UINT SVSO_RP			:16;	_rege_	_ISP_0021;

#define ISP_SVSO_FP				_bm(_ISP_0021, ISP_REG_BASE, (0x0021<<2), SVSO_FP			)	//	
#define ISP_SVSO_RP				_bm(_ISP_0021, ISP_REG_BASE, (0x0021<<2), SVSO_RP			)	//	
#define ISP_0021_T				_am(_ISP_0021, ISP_REG_BASE, (0x0021<<2))						//	

_regs_	UINT _res0				: 3;
		UINT ASYNC_VPOS			:13;
		UINT _res1				: 3;
		UINT ASYNC_VST			:13;	_rege_	_ISP_0022;

#define ISP_ASYNC_VPOS			_bm(_ISP_0022, ISP_REG_BASE, (0x0022<<2), ASYNC_VPOS		)	//	
#define ISP_ASYNC_VST			_bm(_ISP_0022, ISP_REG_BASE, (0x0022<<2), ASYNC_VST			)	//	
#define ISP_0022_T				_am(_ISP_0022, ISP_REG_BASE, (0x0022<<2))						//	

_regs_	UINT _res0				:23;
		UINT ASYNC_HW			: 9;	_rege_	_ISP_0023;

#define ISP_ASYNC_HW			_bm(_ISP_0023, ISP_REG_BASE, (0x0023<<2), ASYNC_HW			)	//	
#define ISP_0023_T				_am(_ISP_0023, ISP_REG_BASE, (0x0023<<2))						//	

_regs_	UINT POST_MASK			: 1;
		UINT _res0				: 1;
		UINT ONSEL				: 2;
		UINT PCBR_CINV			: 1;
		UINT _res1				: 1;
		UINT YCBR2YC_SEL		: 2;
		UINT _res2				: 7;
		UINT YC_OSEL			: 5;
		UINT POST_OSEL			: 4;
		UINT _res3				: 3;
		UINT PRE_OSEL			: 5;	_rege_	_ISP_0024;

#define ISP_POST_MASK			_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), POST_MASK			)	//	
#define ISP_ONSEL				_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), ONSEL				)	//	
#define ISP_PCBR_CINV			_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), PCBR_CINV			)	//	
#define ISP_YCBR2YC_SEL			_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), YCBR2YC_SEL		)	//	
#define ISP_YC_OSEL				_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), YC_OSEL			)	//	
#define ISP_POST_OSEL			_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), POST_OSEL			)	//	
#define ISP_PRE_OSEL			_bm(_ISP_0024, ISP_REG_BASE, (0x0024<<2), PRE_OSEL			)	//	
#define ISP_0024_T				_am(_ISP_0024, ISP_REG_BASE, (0x0024<<2))						//	

_regs_	UINT _res0				: 3;
		UINT UIRQ_FHPOS0		:13;
		UINT _res1				: 4;
		UINT UIRQ_FVPOS0		:12;	_rege_	_ISP_0025;

#define ISP_UIRQ_FHPOS0			_bm(_ISP_0025, ISP_REG_BASE, (0x0025<<2), UIRQ_FHPOS0		)	//	
#define ISP_UIRQ_FVPOS0			_bm(_ISP_0025, ISP_REG_BASE, (0x0025<<2), UIRQ_FVPOS0		)	//	
#define ISP_0025_T				_am(_ISP_0025, ISP_REG_BASE, (0x0025<<2))						//	

_regs_	UINT _res0				: 4;
		UINT UIRQ_FHPOS1		:12;
		UINT _res1				: 4;
		UINT UIRQ_FVPOS1		:12;	_rege_	_ISP_0026;

#define ISP_UIRQ_FHPOS1			_bm(_ISP_0026, ISP_REG_BASE, (0x0026<<2), UIRQ_FHPOS1		)	//	
#define ISP_UIRQ_FVPOS1			_bm(_ISP_0026, ISP_REG_BASE, (0x0026<<2), UIRQ_FVPOS1		)	//	
#define ISP_0026_T				_am(_ISP_0026, ISP_REG_BASE, (0x0026<<2))						//	

_regs_	UINT _res0				:15;
		UINT IRIS_MUX			: 1;
		UINT _res1				: 3;
		UINT SDES_PC			: 5;
		UINT _res2				: 3;
		UINT SDES_PD			: 5;	_rege_	_ISP_0027;

#define ISP_IRIS_MUX			_bm(_ISP_0027, ISP_REG_BASE, (0x0027<<2), IRIS_MUX			)	//	
#define ISP_SDES_PC				_bm(_ISP_0027, ISP_REG_BASE, (0x0027<<2), SDES_PC			)	//	
#define ISP_SDES_PD				_bm(_ISP_0027, ISP_REG_BASE, (0x0027<<2), SDES_PD			)	//	
#define ISP_0027_T				_am(_ISP_0027, ISP_REG_BASE, (0x0027<<2))						//	

_regs_	UINT LVDS_RXON			: 1;
		UINT LVDS_TCHK			: 1;
		UINT LVDS_FON			: 1;
		UINT LMOD_BIT			: 2;
		UINT LDI_POL			: 1;
		UINT LDI_PNSEL			: 1;
		UINT LVDS_LSB			: 1;
		UINT LDO3_SEL			: 2;
		UINT LDO2_SEL			: 2;
		UINT LDO1_SEL			: 2;
		UINT LDO0_SEL			: 2;
		UINT CH2_RFLG_INV		: 1;
		UINT LDIDLY3			: 3;
		UINT LDIDLY2			: 3;
		UINT LDIDLY1			: 3;
		UINT LDIDLY0			: 3;
		UINT LCK_DLY			: 3;	_rege_	_ISP_0028;

#define ISP_LVDS_RXON			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LVDS_RXON			)	//	
#define ISP_LVDS_TCHK			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LVDS_TCHK			)	//	
#define ISP_LVDS_FON			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LVDS_FON			)	//	
#define ISP_LMOD_BIT			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LMOD_BIT			)	//	
#define ISP_LDI_POL				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDI_POL			)	//	
#define ISP_LDI_PNSEL			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDI_PNSEL			)	//	
#define ISP_LVDS_LSB			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LVDS_LSB			)	//	
#define ISP_LDO3_SEL			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDO3_SEL			)	//	
#define ISP_LDO2_SEL			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDO2_SEL			)	//	
#define ISP_LDO1_SEL			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDO1_SEL			)	//	
#define ISP_LDO0_SEL			_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDO0_SEL			)	//	
#define ISP_CH2_RFLG_INV		_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), CH2_RFLG_INV		)	//	
#define ISP_LDIDLY3				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDIDLY3			)	//	
#define ISP_LDIDLY2				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDIDLY2			)	//	
#define ISP_LDIDLY1				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDIDLY1			)	//	
#define ISP_LDIDLY0				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LDIDLY0			)	//	
#define ISP_LCK_DLY				_bm(_ISP_0028, ISP_REG_BASE, (0x0028<<2), LCK_DLY			)	//	
#define ISP_0028_T				_am(_ISP_0028, ISP_REG_BASE, (0x0028<<2))						//	

_regs_	UINT SYNC_COD0			:16;
		UINT SYNC_COD1			:16;	_rege_	_ISP_0029;

#define ISP_SYNC_COD0			_bm(_ISP_0029, ISP_REG_BASE, (0x0029<<2), SYNC_COD0			)	//	
#define ISP_SYNC_COD1			_bm(_ISP_0029, ISP_REG_BASE, (0x0029<<2), SYNC_COD1			)	//	
#define ISP_0029_T				_am(_ISP_0029, ISP_REG_BASE, (0x0029<<2))						//	

_regs_	UINT SYNC_COD2			:16;
		UINT SYNC_COD3_0		:16;	_rege_	_ISP_002A;

#define ISP_SYNC_COD2			_bm(_ISP_002A, ISP_REG_BASE, (0x002A<<2), SYNC_COD2			)	//	
#define ISP_SYNC_COD3_0			_bm(_ISP_002A, ISP_REG_BASE, (0x002A<<2), SYNC_COD3_0		)	//	
#define ISP_002A_T				_am(_ISP_002A, ISP_REG_BASE, (0x002A<<2))						//	

_regs_	UINT SYNC_COD3_1		:16;
		UINT SYNC_COD3_2		:16;	_rege_	_ISP_002B;

#define ISP_SYNC_COD3_1			_bm(_ISP_002B, ISP_REG_BASE, (0x002B<<2), SYNC_COD3_1		)	//	
#define ISP_SYNC_COD3_2			_bm(_ISP_002B, ISP_REG_BASE, (0x002B<<2), SYNC_COD3_2		)	//	
#define ISP_002B_T				_am(_ISP_002B, ISP_REG_BASE, (0x002B<<2))						//	

_regs_	UINT LBUF_ON			: 1;
		UINT LBUF_DON			: 1;
		UINT LVDS_FIFO_RST		: 1;
		UINT LVDS_2CH_ON		: 1;
		UINT LV_RPOS			: 2;
		UINT LDCAP_POS			: 2;
		UINT BLANK_SOL_ON		: 1;
		UINT SONY_V_ON			: 1;
		UINT LBUF_WEN			: 1;
		UINT _res0				:21;	_rege_	_ISP_002C;

#define ISP_LBUF_ON				_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LBUF_ON			)	//	
#define ISP_LBUF_DON			_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LBUF_DON			)	//	
#define ISP_LVDS_FIFO_RST		_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LVDS_FIFO_RST		)	//	
#define ISP_LVDS_2CH_ON			_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LVDS_2CH_ON		)	//	
#define ISP_LV_RPOS				_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LV_RPOS			)	//	
#define ISP_LDCAP_POS			_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LDCAP_POS			)	//	
#define ISP_BLANK_SOL_ON		_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), BLANK_SOL_ON		)	//	
#define ISP_SONY_V_ON			_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), SONY_V_ON			)	//	
#define ISP_LBUF_WEN			_bm(_ISP_002C, ISP_REG_BASE, (0x002C<<2), LBUF_WEN			)	//	
#define ISP_002C_T				_am(_ISP_002C, ISP_REG_BASE, (0x002C<<2))						//	

_regs_	UINT _res0				:20;
		UINT MERGE_HW			:12;	_rege_	_ISP_002D;

#define ISP_MERGE_HW			_bm(_ISP_002D, ISP_REG_BASE, (0x002D<<2), MERGE_HW			)	//	
#define ISP_002D_T				_am(_ISP_002D, ISP_REG_BASE, (0x002D<<2))						//	

_regs_	UINT SHD_MODE			: 2;
		UINT SHD_TEST			: 2;
		UINT SHD_DETOSD			: 1;
		UINT SHD_ECHK			: 1;
		UINT SHD_KH				:10;
		UINT _res0				: 6;
		UINT SHD_KV				:10;	_rege_	_ISP_0030;

#define ISP_SHD_MODE			_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_MODE			)	//	
#define ISP_SHD_TEST			_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_TEST			)	//	
#define ISP_SHD_DETOSD			_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_DETOSD		)	//	
#define ISP_SHD_ECHK			_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_ECHK			)	//	
#define ISP_SHD_KH				_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_KH			)	//	
#define ISP_SHD_KV				_bm(_ISP_0030, ISP_REG_BASE, (0x0030<<2), SHD_KV			)	//	
#define ISP_0030_T				_am(_ISP_0030, ISP_REG_BASE, (0x0030<<2))						//	

_regs_	UINT _res0				: 3;
		UINT SHD_DHOFF			:13;
		UINT _res1				: 3;
		UINT SHD_DVOFF			:13;	_rege_	_ISP_0031;

#define ISP_SHD_DHOFF			_bm(_ISP_0031, ISP_REG_BASE, (0x0031<<2), SHD_DHOFF			)	//	
#define ISP_SHD_DVOFF			_bm(_ISP_0031, ISP_REG_BASE, (0x0031<<2), SHD_DVOFF			)	//	
#define ISP_0031_T				_am(_ISP_0031, ISP_REG_BASE, (0x0031<<2))						//	

_regs_	UINT _res0				: 4;
		UINT SHD_CHOFF			:12;
		UINT _res1				: 4;
		UINT SHD_CVOFF			:12;	_rege_	_ISP_0032;

#define ISP_SHD_CHOFF			_bm(_ISP_0032, ISP_REG_BASE, (0x0032<<2), SHD_CHOFF			)	//	
#define ISP_SHD_CVOFF			_bm(_ISP_0032, ISP_REG_BASE, (0x0032<<2), SHD_CVOFF			)	//	
#define ISP_0032_T				_am(_ISP_0032, ISP_REG_BASE, (0x0032<<2))						//	

_regs_	UINT _res0				:24;
		UINT SHD_GAIN			: 8;	_rege_	_ISP_0033;

#define ISP_SHD_GAIN			_bm(_ISP_0033, ISP_REG_BASE, (0x0033<<2), SHD_GAIN			)	//	
#define ISP_0033_T				_am(_ISP_0033, ISP_REG_BASE, (0x0033<<2))						//	

_regs_	UINT _res0				: 3;
		UINT SHD_DHW			:13;
		UINT _res1				: 3;
		UINT SHD_DVW			:13;	_rege_	_ISP_0034;

#define ISP_SHD_DHW				_bm(_ISP_0034, ISP_REG_BASE, (0x0034<<2), SHD_DHW			)	//	
#define ISP_SHD_DVW				_bm(_ISP_0034, ISP_REG_BASE, (0x0034<<2), SHD_DVW			)	//	
#define ISP_0034_T				_am(_ISP_0034, ISP_REG_BASE, (0x0034<<2))						//	

_regs_	UINT GV_RSEL			: 2;
		UINT GV_656				: 1;
		UINT GV_RSTN			: 1;
		UINT FPGA_EXSYNC_PARALLEL	: 1;
		UINT _res0				:11;
		UINT SENSOR_WID			: 8;
		UINT PARALLEL_SENSOR	: 1;
		UINT _res1				: 1;
		UINT ISSPILOGIC			: 1;
		UINT _res2				: 1;
		UINT LSPI_RSTN			: 1;
		UINT _res3				: 2;
		UINT SSTYPE_SEL			: 1;	_rege_	_ISP_0035;

#define ISP_GV_RSEL				_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), GV_RSEL			)	//	
#define ISP_GV_656				_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), GV_656			)	//	
#define ISP_GV_RSTN				_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), GV_RSTN			)	//	
#define ISP_FPGA_EXSYNC_PARALLEL	_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), FPGA_EXSYNC_PARALLEL	)	//	
#define ISP_SENSOR_WID			_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), SENSOR_WID		)	//	
#define ISP_PARALLEL_SENSOR		_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), PARALLEL_SENSOR	)	//	
#define ISP_ISSPILOGIC			_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), ISSPILOGIC		)	//	
#define ISP_LSPI_RSTN			_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), LSPI_RSTN			)	//	
#define ISP_SSTYPE_SEL			_bm(_ISP_0035, ISP_REG_BASE, (0x0035<<2), SSTYPE_SEL		)	//	
#define ISP_0035_T				_am(_ISP_0035, ISP_REG_BASE, (0x0035<<2))						//	

_regs_	UINT CMP_ON				: 1;
		UINT CMP_OSD_ON			: 1;
		UINT CMP_YGINV			: 1;
		UINT CMP_RBINV			: 1;
		UINT _res0				: 1;
		UINT CMP_HW				: 3;
		UINT _res1				: 1;
		UINT CMP_VW				: 3;
		UINT _res2				: 3;
		UINT CMP_MOD			: 1;
		UINT _res3				: 4;
		UINT CMP_VSP			:12;	_rege_	_ISP_0036;

#define ISP_CMP_ON				_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_ON			)	//	
#define ISP_CMP_OSD_ON			_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_OSD_ON		)	//	
#define ISP_CMP_YGINV			_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_YGINV			)	//	
#define ISP_CMP_RBINV			_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_RBINV			)	//	
#define ISP_CMP_HW				_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_HW			)	//	
#define ISP_CMP_VW				_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_VW			)	//	
#define ISP_CMP_MOD				_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_MOD			)	//	
#define ISP_CMP_VSP				_bm(_ISP_0036, ISP_REG_BASE, (0x0036<<2), CMP_VSP			)	//	
#define ISP_0036_T				_am(_ISP_0036, ISP_REG_BASE, (0x0036<<2))						//	

_regs_	UINT _res0				: 4;
		UINT CMP_HSP			:12;
		UINT _res1				: 4;
		UINT CMP_DTH			:12;	_rege_	_ISP_0037;

#define ISP_CMP_HSP				_bm(_ISP_0037, ISP_REG_BASE, (0x0037<<2), CMP_HSP			)	//	
#define ISP_CMP_DTH				_bm(_ISP_0037, ISP_REG_BASE, (0x0037<<2), CMP_DTH			)	//	
#define ISP_0037_T				_am(_ISP_0037, ISP_REG_BASE, (0x0037<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CMP_GROFS			:10;
		UINT _res1				: 6;
		UINT CMP_GBOFS			:10;	_rege_	_ISP_0038;

#define ISP_CMP_GROFS			_bm(_ISP_0038, ISP_REG_BASE, (0x0038<<2), CMP_GROFS			)	//	
#define ISP_CMP_GBOFS			_bm(_ISP_0038, ISP_REG_BASE, (0x0038<<2), CMP_GBOFS			)	//	
#define ISP_0038_T				_am(_ISP_0038, ISP_REG_BASE, (0x0038<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CMP_ROFS			:10;
		UINT _res1				: 6;
		UINT CMP_BOFS			:10;	_rege_	_ISP_0039;

#define ISP_CMP_ROFS			_bm(_ISP_0039, ISP_REG_BASE, (0x0039<<2), CMP_ROFS			)	//	
#define ISP_CMP_BOFS			_bm(_ISP_0039, ISP_REG_BASE, (0x0039<<2), CMP_BOFS			)	//	
#define ISP_0039_T				_am(_ISP_0039, ISP_REG_BASE, (0x0039<<2))						//	

_regs_	UINT _res0				:20;
		UINT PREGAIN			:12;	_rege_	_ISP_003A;

#define ISP_PREGAIN				_bm(_ISP_003A, ISP_REG_BASE, (0x003A<<2), PREGAIN			)	//	
#define ISP_003A_T				_am(_ISP_003A, ISP_REG_BASE, (0x003A<<2))						//	

_regs_	UINT DNR3D_ON			: 1;
		UINT _res0				: 1;
		UINT DNR3D_GA			: 6;
		UINT DNR3D_FK			: 8;
		UINT DNR3D_TH			: 8;
		UINT _res1				: 6;
		UINT DNROUT_SEL			: 2;	_rege_	_ISP_003C;

#define ISP_DNR3D_ON			_bm(_ISP_003C, ISP_REG_BASE, (0x003C<<2), DNR3D_ON			)	//	
#define ISP_DNR3D_GA			_bm(_ISP_003C, ISP_REG_BASE, (0x003C<<2), DNR3D_GA			)	//	
#define ISP_DNR3D_FK			_bm(_ISP_003C, ISP_REG_BASE, (0x003C<<2), DNR3D_FK			)	//	
#define ISP_DNR3D_TH			_bm(_ISP_003C, ISP_REG_BASE, (0x003C<<2), DNR3D_TH			)	//	
#define ISP_DNROUT_SEL			_bm(_ISP_003C, ISP_REG_BASE, (0x003C<<2), DNROUT_SEL		)	//	
#define ISP_003C_T				_am(_ISP_003C, ISP_REG_BASE, (0x003C<<2))						//	

_regs_	UINT DF_DIFGA			: 8;
		UINT DF_DIFTH			: 8;
		UINT _res0				: 2;
		UINT DF_ALPHA			: 6;
		UINT DF_BETA			: 8;	_rege_	_ISP_0040;

#define ISP_DF_DIFGA			_bm(_ISP_0040, ISP_REG_BASE, (0x0040<<2), DF_DIFGA			)	//	
#define ISP_DF_DIFTH			_bm(_ISP_0040, ISP_REG_BASE, (0x0040<<2), DF_DIFTH			)	//	
#define ISP_DF_ALPHA			_bm(_ISP_0040, ISP_REG_BASE, (0x0040<<2), DF_ALPHA			)	//	
#define ISP_DF_BETA				_bm(_ISP_0040, ISP_REG_BASE, (0x0040<<2), DF_BETA			)	//	
#define ISP_0040_T				_am(_ISP_0040, ISP_REG_BASE, (0x0040<<2))						//	

_regs_	UINT DF_CWEGHT			: 2;
		UINT _res0				: 2;
		UINT DF_DETHSP			:12;
		UINT _res1				: 4;
		UINT DF_DETHEP			:12;	_rege_	_ISP_0041;

#define ISP_DF_CWEGHT			_bm(_ISP_0041, ISP_REG_BASE, (0x0041<<2), DF_CWEGHT			)	//	
#define ISP_DF_DETHSP			_bm(_ISP_0041, ISP_REG_BASE, (0x0041<<2), DF_DETHSP			)	//	
#define ISP_DF_DETHEP			_bm(_ISP_0041, ISP_REG_BASE, (0x0041<<2), DF_DETHEP			)	//	
#define ISP_0041_T				_am(_ISP_0041, ISP_REG_BASE, (0x0041<<2))						//	

_regs_	UINT DF_TSEL			: 3;
		UINT _res0				: 1;
		UINT DF_DETVSP			:12;
		UINT _res1				: 4;
		UINT DF_DETVEP			:12;	_rege_	_ISP_0042;

#define ISP_DF_TSEL				_bm(_ISP_0042, ISP_REG_BASE, (0x0042<<2), DF_TSEL			)	//	
#define ISP_DF_DETVSP			_bm(_ISP_0042, ISP_REG_BASE, (0x0042<<2), DF_DETVSP			)	//	
#define ISP_DF_DETVEP			_bm(_ISP_0042, ISP_REG_BASE, (0x0042<<2), DF_DETVEP			)	//	
#define ISP_0042_T				_am(_ISP_0042, ISP_REG_BASE, (0x0042<<2))						//	

_regs_	UINT DF_OSDON			: 1;
		UINT DF_CMD				: 3;
		UINT _res0				: 4;
		UINT DF_DTHRES			: 8;
		UINT DFS_RBG_SEL		: 1;
		UINT DFS_DIFGA			: 7;
		UINT DFS_DIFTH			: 8;	_rege_	_ISP_0043;

#define ISP_DF_OSDON			_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DF_OSDON			)	//	
#define ISP_DF_CMD				_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DF_CMD			)	//	
#define ISP_DF_DTHRES			_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DF_DTHRES			)	//	
#define ISP_DFS_RBG_SEL			_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DFS_RBG_SEL		)	//	
#define ISP_DFS_DIFGA			_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DFS_DIFGA			)	//	
#define ISP_DFS_DIFTH			_bm(_ISP_0043, ISP_REG_BASE, (0x0043<<2), DFS_DIFTH			)	//	
#define ISP_0043_T				_am(_ISP_0043, ISP_REG_BASE, (0x0043<<2))						//	

_regs_	UINT DF_TESTON			: 1;
		UINT _res0				: 3;
		UINT DF_TESTPH1			:12;
		UINT _res1				: 5;
		UINT DF_TESTPV1			:11;	_rege_	_ISP_0044;

#define ISP_DF_TESTON			_bm(_ISP_0044, ISP_REG_BASE, (0x0044<<2), DF_TESTON			)	//	
#define ISP_DF_TESTPH1			_bm(_ISP_0044, ISP_REG_BASE, (0x0044<<2), DF_TESTPH1		)	//	
#define ISP_DF_TESTPV1			_bm(_ISP_0044, ISP_REG_BASE, (0x0044<<2), DF_TESTPV1		)	//	
#define ISP_0044_T				_am(_ISP_0044, ISP_REG_BASE, (0x0044<<2))						//	

_regs_	UINT _res0				: 4;
		UINT DF_TESTPH2			:12;
		UINT _res1				: 5;
		UINT DF_TESTPV2			:11;	_rege_	_ISP_0045;

#define ISP_DF_TESTPH2			_bm(_ISP_0045, ISP_REG_BASE, (0x0045<<2), DF_TESTPH2		)	//	
#define ISP_DF_TESTPV2			_bm(_ISP_0045, ISP_REG_BASE, (0x0045<<2), DF_TESTPV2		)	//	
#define ISP_0045_T				_am(_ISP_0045, ISP_REG_BASE, (0x0045<<2))						//	

_regs_	UINT _res0				: 4;
		UINT DF_TESTPH3			:12;
		UINT _res1				: 5;
		UINT DF_TESTPV3			:11;	_rege_	_ISP_0046;

#define ISP_DF_TESTPH3			_bm(_ISP_0046, ISP_REG_BASE, (0x0046<<2), DF_TESTPH3		)	//	
#define ISP_DF_TESTPV3			_bm(_ISP_0046, ISP_REG_BASE, (0x0046<<2), DF_TESTPV3		)	//	
#define ISP_0046_T				_am(_ISP_0046, ISP_REG_BASE, (0x0046<<2))						//	

_regs_	UINT _res0				: 4;
		UINT DF_TESTPH4			:12;
		UINT _res1				: 5;
		UINT DF_TESTPV4			:11;	_rege_	_ISP_0047;

#define ISP_DF_TESTPH4			_bm(_ISP_0047, ISP_REG_BASE, (0x0047<<2), DF_TESTPH4		)	//	
#define ISP_DF_TESTPV4			_bm(_ISP_0047, ISP_REG_BASE, (0x0047<<2), DF_TESTPV4		)	//	
#define ISP_0047_T				_am(_ISP_0047, ISP_REG_BASE, (0x0047<<2))						//	

_regs_	UINT _res0				: 4;
		UINT DF_TESTPLV			:12;
		UINT _res1				: 8;
		UINT RGB_DIR_THR		: 8;	_rege_	_ISP_0048;

#define ISP_DF_TESTPLV			_bm(_ISP_0048, ISP_REG_BASE, (0x0048<<2), DF_TESTPLV		)	//	
#define ISP_RGB_DIR_THR			_bm(_ISP_0048, ISP_REG_BASE, (0x0048<<2), RGB_DIR_THR		)	//	
#define ISP_0048_T				_am(_ISP_0048, ISP_REG_BASE, (0x0048<<2))						//	

_regs_	UINT WDR_ON				: 1;
		UINT WDR_SCHM_ON		: 1;
		UINT WDR_SGAMMA_ON		: 1;
		UINT WDR_SWGT_LMT_ON	: 1;
		UINT LS_FLG_INV			: 1;
		UINT WDR_LS_SWCH_ON		: 1;
		UINT WDR_G_SWCH_ON		: 1;
		UINT WDR_MOTION_ON		: 1;
		UINT LS_FLG_POST_H		: 1;
		UINT WDR_LD_MSTEP		: 7;
		UINT _res0				: 2;
		UINT WDR_LOW_TH			: 6;
		UINT _res1				: 3;
		UINT WDR_TSEL			: 5;	_rege_	_ISP_0049;

#define ISP_WDR_ON				_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_ON			)	//	
#define ISP_WDR_SCHM_ON			_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_SCHM_ON		)	//	
#define ISP_WDR_SGAMMA_ON		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_SGAMMA_ON		)	//	
#define ISP_WDR_SWGT_LMT_ON		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_SWGT_LMT_ON	)	//	
#define ISP_LS_FLG_INV			_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), LS_FLG_INV		)	//	
#define ISP_WDR_LS_SWCH_ON		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_LS_SWCH_ON	)	//	
#define ISP_WDR_G_SWCH_ON		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_G_SWCH_ON		)	//	
#define ISP_WDR_MOTION_ON		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_MOTION_ON		)	//	
#define ISP_LS_FLG_POST_H		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), LS_FLG_POST_H		)	//	
#define ISP_WDR_LD_MSTEP		_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_LD_MSTEP		)	//	
#define ISP_WDR_LOW_TH			_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_LOW_TH		)	//	
#define ISP_WDR_TSEL			_bm(_ISP_0049, ISP_REG_BASE, (0x0049<<2), WDR_TSEL			)	//	
#define ISP_0049_T				_am(_ISP_0049, ISP_REG_BASE, (0x0049<<2))						//	

_regs_	UINT _res0				: 6;
		UINT WDR_SATV			:10;
		UINT _res1				: 6;
		UINT WDR_SWGT			:10;	_rege_	_ISP_004A;

#define ISP_WDR_SATV			_bm(_ISP_004A, ISP_REG_BASE, (0x004A<<2), WDR_SATV			)	//	
#define ISP_WDR_SWGT			_bm(_ISP_004A, ISP_REG_BASE, (0x004A<<2), WDR_SWGT			)	//	
#define ISP_004A_T				_am(_ISP_004A, ISP_REG_BASE, (0x004A<<2))						//	

_regs_	UINT _res0				: 4;
		UINT WDR_LSHUT			:12;
		UINT _res1				: 4;
		UINT WDR_SSHUT			:12;	_rege_	_ISP_004B;

#define ISP_WDR_LSHUT			_bm(_ISP_004B, ISP_REG_BASE, (0x004B<<2), WDR_LSHUT			)	//	
#define ISP_WDR_SSHUT			_bm(_ISP_004B, ISP_REG_BASE, (0x004B<<2), WDR_SSHUT			)	//	
#define ISP_004B_T				_am(_ISP_004B, ISP_REG_BASE, (0x004B<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_SGM_RANGE		: 8;
		UINT _res1				: 6;
		UINT WDR_SCH_MAX		:10;	_rege_	_ISP_004C;

#define ISP_WDR_SGM_RANGE		_bm(_ISP_004C, ISP_REG_BASE, (0x004C<<2), WDR_SGM_RANGE		)	//	
#define ISP_WDR_SCH_MAX			_bm(_ISP_004C, ISP_REG_BASE, (0x004C<<2), WDR_SCH_MAX		)	//	
#define ISP_004C_T				_am(_ISP_004C, ISP_REG_BASE, (0x004C<<2))						//	

_regs_	UINT _res0				: 7;
		UINT WDR_LS_MSTEP		: 9;
		UINT _res1				: 6;
		UINT WDR_BIT_MAX		:10;	_rege_	_ISP_004D;

#define ISP_WDR_LS_MSTEP		_bm(_ISP_004D, ISP_REG_BASE, (0x004D<<2), WDR_LS_MSTEP		)	//	
#define ISP_WDR_BIT_MAX			_bm(_ISP_004D, ISP_REG_BASE, (0x004D<<2), WDR_BIT_MAX		)	//	
#define ISP_004D_T				_am(_ISP_004D, ISP_REG_BASE, (0x004D<<2))						//	

_regs_	UINT _res0				:11;
		UINT WDR_HIGH_CNT_TH	:21;	_rege_	_ISP_004E;

#define ISP_WDR_HIGH_CNT_TH		_bm(_ISP_004E, ISP_REG_BASE, (0x004E<<2), WDR_HIGH_CNT_TH	)	//	
#define ISP_004E_T				_am(_ISP_004E, ISP_REG_BASE, (0x004E<<2))						//	

_regs_	UINT WDR_SL_R_IIR		: 8;
		UINT _res0				: 3;
		UINT WDR_LOW_CNT_TH		:21;	_rege_	_ISP_004F;

#define ISP_WDR_SL_R_IIR		_bm(_ISP_004F, ISP_REG_BASE, (0x004F<<2), WDR_SL_R_IIR		)	//	
#define ISP_WDR_LOW_CNT_TH		_bm(_ISP_004F, ISP_REG_BASE, (0x004F<<2), WDR_LOW_CNT_TH	)	//	
#define ISP_004F_T				_am(_ISP_004F, ISP_REG_BASE, (0x004F<<2))						//	

_regs_	UINT _res0				: 5;
		UINT WDR_SR_GAIN		:11;
		UINT _res1				: 5;
		UINT WDR_SB_GAIN		:11;	_rege_	_ISP_0050;

#define ISP_WDR_SR_GAIN			_bm(_ISP_0050, ISP_REG_BASE, (0x0050<<2), WDR_SR_GAIN		)	//	
#define ISP_WDR_SB_GAIN			_bm(_ISP_0050, ISP_REG_BASE, (0x0050<<2), WDR_SB_GAIN		)	//	
#define ISP_0050_T				_am(_ISP_0050, ISP_REG_BASE, (0x0050<<2))						//	

_regs_	UINT WDR_BACKGND_SEL	: 2;
		UINT WDR_WINDOW_SEL		: 2;
		UINT WDR_WND_ON			: 4;
		UINT WDR_WND0_HSP		:12;
		UINT WDR_WND0_VSP		:12;	_rege_	_ISP_0051;

#define ISP_WDR_BACKGND_SEL		_bm(_ISP_0051, ISP_REG_BASE, (0x0051<<2), WDR_BACKGND_SEL	)	//	
#define ISP_WDR_WINDOW_SEL		_bm(_ISP_0051, ISP_REG_BASE, (0x0051<<2), WDR_WINDOW_SEL	)	//	
#define ISP_WDR_WND_ON			_bm(_ISP_0051, ISP_REG_BASE, (0x0051<<2), WDR_WND_ON		)	//	
#define ISP_WDR_WND0_HSP		_bm(_ISP_0051, ISP_REG_BASE, (0x0051<<2), WDR_WND0_HSP		)	//	
#define ISP_WDR_WND0_VSP		_bm(_ISP_0051, ISP_REG_BASE, (0x0051<<2), WDR_WND0_VSP		)	//	
#define ISP_0051_T				_am(_ISP_0051, ISP_REG_BASE, (0x0051<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND0_HEP		:12;
		UINT WDR_WND0_VEP		:12;	_rege_	_ISP_0052;

#define ISP_WDR_WND0_HEP		_bm(_ISP_0052, ISP_REG_BASE, (0x0052<<2), WDR_WND0_HEP		)	//	
#define ISP_WDR_WND0_VEP		_bm(_ISP_0052, ISP_REG_BASE, (0x0052<<2), WDR_WND0_VEP		)	//	
#define ISP_0052_T				_am(_ISP_0052, ISP_REG_BASE, (0x0052<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND1_HSP		:12;
		UINT WDR_WND1_VSP		:12;	_rege_	_ISP_0053;

#define ISP_WDR_WND1_HSP		_bm(_ISP_0053, ISP_REG_BASE, (0x0053<<2), WDR_WND1_HSP		)	//	
#define ISP_WDR_WND1_VSP		_bm(_ISP_0053, ISP_REG_BASE, (0x0053<<2), WDR_WND1_VSP		)	//	
#define ISP_0053_T				_am(_ISP_0053, ISP_REG_BASE, (0x0053<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND1_HEP		:12;
		UINT WDR_WND1_VEP		:12;	_rege_	_ISP_0054;

#define ISP_WDR_WND1_HEP		_bm(_ISP_0054, ISP_REG_BASE, (0x0054<<2), WDR_WND1_HEP		)	//	
#define ISP_WDR_WND1_VEP		_bm(_ISP_0054, ISP_REG_BASE, (0x0054<<2), WDR_WND1_VEP		)	//	
#define ISP_0054_T				_am(_ISP_0054, ISP_REG_BASE, (0x0054<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND2_HSP		:12;
		UINT WDR_WND2_VSP		:12;	_rege_	_ISP_0055;

#define ISP_WDR_WND2_HSP		_bm(_ISP_0055, ISP_REG_BASE, (0x0055<<2), WDR_WND2_HSP		)	//	
#define ISP_WDR_WND2_VSP		_bm(_ISP_0055, ISP_REG_BASE, (0x0055<<2), WDR_WND2_VSP		)	//	
#define ISP_0055_T				_am(_ISP_0055, ISP_REG_BASE, (0x0055<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND2_HEP		:12;
		UINT WDR_WND2_VEP		:12;	_rege_	_ISP_0056;

#define ISP_WDR_WND2_HEP		_bm(_ISP_0056, ISP_REG_BASE, (0x0056<<2), WDR_WND2_HEP		)	//	
#define ISP_WDR_WND2_VEP		_bm(_ISP_0056, ISP_REG_BASE, (0x0056<<2), WDR_WND2_VEP		)	//	
#define ISP_0056_T				_am(_ISP_0056, ISP_REG_BASE, (0x0056<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND3_HSP		:12;
		UINT WDR_WND3_VSP		:12;	_rege_	_ISP_0057;

#define ISP_WDR_WND3_HSP		_bm(_ISP_0057, ISP_REG_BASE, (0x0057<<2), WDR_WND3_HSP		)	//	
#define ISP_WDR_WND3_VSP		_bm(_ISP_0057, ISP_REG_BASE, (0x0057<<2), WDR_WND3_VSP		)	//	
#define ISP_0057_T				_am(_ISP_0057, ISP_REG_BASE, (0x0057<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_WND3_HEP		:12;
		UINT WDR_WND3_VEP		:12;	_rege_	_ISP_0058;

#define ISP_WDR_WND3_HEP		_bm(_ISP_0058, ISP_REG_BASE, (0x0058<<2), WDR_WND3_HEP		)	//	
#define ISP_WDR_WND3_VEP		_bm(_ISP_0058, ISP_REG_BASE, (0x0058<<2), WDR_WND3_VEP		)	//	
#define ISP_0058_T				_am(_ISP_0058, ISP_REG_BASE, (0x0058<<2))						//	

#if 0
_regs_	UINT IMD_ON				: 1;
		UINT IMD_FLT			: 1;
		UINT IMD_TEST			: 2;
		UINT IMD_FTTYPE			: 1;
		UINT IMD_COLOR			: 3;
		UINT IMD_TH				: 8;
		UINT _res0				: 1;
		UINT IMD_HW				: 7;
		UINT IMD_ISEL			: 2;
		UINT IMD_VW				: 6;	_rege_	_ISP_0060;

#define ISP_IMD_ON				_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_ON			)	//	
#define ISP_IMD_FLT				_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_FLT			)	//	
#define ISP_IMD_TEST			_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_TEST			)	//	
#define ISP_IMD_FTTYPE			_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_FTTYPE		)	//	
#define ISP_IMD_COLOR			_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_COLOR			)	//	
#define ISP_IMD_TH				_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_TH			)	//	
#define ISP_IMD_HW				_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_HW			)	//	
#define ISP_IMD_ISEL			_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_ISEL			)	//	
#define ISP_IMD_VW				_bm(_ISP_0060, ISP_REG_BASE, (0x0060<<2), IMD_VW			)	//	
#define ISP_0060_T				_am(_ISP_0060, ISP_REG_BASE, (0x0060<<2))						//	
#endif

_regs_	UINT _res0				: 4;
		UINT IMD_HSP			:12;
		UINT _res1				: 5;
		UINT IMD_VSP			:11;	_rege_	_ISP_0061;

#define ISP_IMD_HSP				_bm(_ISP_0061, ISP_REG_BASE, (0x0061<<2), IMD_HSP			)	//	
#define ISP_IMD_VSP				_bm(_ISP_0061, ISP_REG_BASE, (0x0061<<2), IMD_VSP			)	//	
#define ISP_0061_T				_am(_ISP_0061, ISP_REG_BASE, (0x0061<<2))						//	

_regs_	UINT IMD_KMAX			: 8;
		UINT IMD_KMIN			: 8;
		UINT IMD_KGA			: 8;
		UINT IMD_KTH			: 8;	_rege_	_ISP_0062;

#define ISP_IMD_KMAX			_bm(_ISP_0062, ISP_REG_BASE, (0x0062<<2), IMD_KMAX			)	//	
#define ISP_IMD_KMIN			_bm(_ISP_0062, ISP_REG_BASE, (0x0062<<2), IMD_KMIN			)	//	
#define ISP_IMD_KGA				_bm(_ISP_0062, ISP_REG_BASE, (0x0062<<2), IMD_KGA			)	//	
#define ISP_IMD_KTH				_bm(_ISP_0062, ISP_REG_BASE, (0x0062<<2), IMD_KTH			)	//	
#define ISP_0062_T				_am(_ISP_0062, ISP_REG_BASE, (0x0062<<2))						//	

_regs_	UINT IMD_BUPC			: 8;
		UINT IMD_AUPC			: 8;
		UINT _res0				:16;	_rege_	_ISP_0063;

#define ISP_IMD_BUPC			_bm(_ISP_0063, ISP_REG_BASE, (0x0063<<2), IMD_BUPC			)	//	
#define ISP_IMD_AUPC			_bm(_ISP_0063, ISP_REG_BASE, (0x0063<<2), IMD_AUPC			)	//	
#define ISP_0063_T				_am(_ISP_0063, ISP_REG_BASE, (0x0063<<2))						//	

_regs_	UINT IMD_BDW			: 1;
		UINT IMD_GMA			: 1;
		UINT IMD_OSDON			: 6;
		UINT IMD_OSDLV			:10;
		UINT _res0				:14;	_rege_	_ISP_0064;

#define ISP_IMD_BDW				_bm(_ISP_0064, ISP_REG_BASE, (0x0064<<2), IMD_BDW			)	//	
#define ISP_IMD_GMA				_bm(_ISP_0064, ISP_REG_BASE, (0x0064<<2), IMD_GMA			)	//	
#define ISP_IMD_OSDON			_bm(_ISP_0064, ISP_REG_BASE, (0x0064<<2), IMD_OSDON			)	//	
#define ISP_IMD_OSDLV			_bm(_ISP_0064, ISP_REG_BASE, (0x0064<<2), IMD_OSDLV			)	//	
#define ISP_0064_T				_am(_ISP_0064, ISP_REG_BASE, (0x0064<<2))						//	

_regs_	UINT IMD_IIRON			: 1;
		UINT IMD_IIRAUTO		: 1;
		UINT IMD_ACCON			: 1;
		UINT _res0				: 6;
		UINT IMD_ACCTH			: 7;
		UINT _res1				: 1;
		UINT IMD_ACCMAX			: 7;
		UINT _res2				: 1;
		UINT IMD_ACCMIN			: 7;	_rege_	_ISP_0065;

#define ISP_IMD_IIRON			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_IIRON			)	//	
#define ISP_IMD_IIRAUTO			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_IIRAUTO		)	//	
#define ISP_IMD_ACCON			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_ACCON			)	//	
#define ISP_IMD_ACCTH			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_ACCTH			)	//	
#define ISP_IMD_ACCMAX			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_ACCMAX		)	//	
#define ISP_IMD_ACCMIN			_bm(_ISP_0065, ISP_REG_BASE, (0x0065<<2), IMD_ACCMIN		)	//	
#define ISP_0065_T				_am(_ISP_0065, ISP_REG_BASE, (0x0065<<2))						//	

_regs_	UINT _res0				: 1;
		UINT IMD_ACCINC			: 7;
		UINT _res1				: 1;
		UINT IMD_ACCDEC			: 7;
		UINT IMD_IIRK			: 8;
		UINT _res2				: 8;	_rege_	_ISP_0066;

#define ISP_IMD_ACCINC			_bm(_ISP_0066, ISP_REG_BASE, (0x0066<<2), IMD_ACCINC		)	//	
#define ISP_IMD_ACCDEC			_bm(_ISP_0066, ISP_REG_BASE, (0x0066<<2), IMD_ACCDEC		)	//	
#define ISP_IMD_IIRK			_bm(_ISP_0066, ISP_REG_BASE, (0x0066<<2), IMD_IIRK			)	//	
#define ISP_0066_T				_am(_ISP_0066, ISP_REG_BASE, (0x0066<<2))						//	

_regs_	UINT IMD_NUMTH			: 8;
		UINT _res0				: 2;
		UINT IMD_HTH			: 6;
		UINT _res1				: 2;
		UINT IMD_VTH			: 6;
		UINT _res2				: 8;	_rege_	_ISP_0067;

#define ISP_IMD_NUMTH			_bm(_ISP_0067, ISP_REG_BASE, (0x0067<<2), IMD_NUMTH			)	//	
#define ISP_IMD_HTH				_bm(_ISP_0067, ISP_REG_BASE, (0x0067<<2), IMD_HTH			)	//	
#define ISP_IMD_VTH				_bm(_ISP_0067, ISP_REG_BASE, (0x0067<<2), IMD_VTH			)	//	
#define ISP_0067_T				_am(_ISP_0067, ISP_REG_BASE, (0x0067<<2))						//	

_regs_	UINT IMD_WON1			: 1;
		UINT IMD_WHSP1			: 7;
		UINT _res0				: 1;
		UINT IMD_WHEP1			: 7;
		UINT _res1				: 2;
		UINT IMD_WVSP1			: 6;
		UINT _res2				: 2;
		UINT IMD_WVEP1			: 6;	_rege_	_ISP_0068;

#define ISP_IMD_WON1			_bm(_ISP_0068, ISP_REG_BASE, (0x0068<<2), IMD_WON1			)	//	
#define ISP_IMD_WHSP1			_bm(_ISP_0068, ISP_REG_BASE, (0x0068<<2), IMD_WHSP1			)	//	
#define ISP_IMD_WHEP1			_bm(_ISP_0068, ISP_REG_BASE, (0x0068<<2), IMD_WHEP1			)	//	
#define ISP_IMD_WVSP1			_bm(_ISP_0068, ISP_REG_BASE, (0x0068<<2), IMD_WVSP1			)	//	
#define ISP_IMD_WVEP1			_bm(_ISP_0068, ISP_REG_BASE, (0x0068<<2), IMD_WVEP1			)	//	
#define ISP_0068_T				_am(_ISP_0068, ISP_REG_BASE, (0x0068<<2))						//	

_regs_	UINT IMD_WON2			: 1;
		UINT IMD_WHSP2			: 7;
		UINT _res0				: 1;
		UINT IMD_WHEP2			: 7;
		UINT _res1				: 2;
		UINT IMD_WVSP2			: 6;
		UINT _res2				: 2;
		UINT IMD_WVEP2			: 6;	_rege_	_ISP_0069;

#define ISP_IMD_WON2			_bm(_ISP_0069, ISP_REG_BASE, (0x0069<<2), IMD_WON2			)	//	
#define ISP_IMD_WHSP2			_bm(_ISP_0069, ISP_REG_BASE, (0x0069<<2), IMD_WHSP2			)	//	
#define ISP_IMD_WHEP2			_bm(_ISP_0069, ISP_REG_BASE, (0x0069<<2), IMD_WHEP2			)	//	
#define ISP_IMD_WVSP2			_bm(_ISP_0069, ISP_REG_BASE, (0x0069<<2), IMD_WVSP2			)	//	
#define ISP_IMD_WVEP2			_bm(_ISP_0069, ISP_REG_BASE, (0x0069<<2), IMD_WVEP2			)	//	
#define ISP_0069_T				_am(_ISP_0069, ISP_REG_BASE, (0x0069<<2))						//	

_regs_	UINT IMD_WON3			: 1;
		UINT IMD_WHSP3			: 7;
		UINT _res0				: 1;
		UINT IMD_WHEP3			: 7;
		UINT _res1				: 2;
		UINT IMD_WVSP3			: 6;
		UINT _res2				: 2;
		UINT IMD_WVEP3			: 6;	_rege_	_ISP_006A;

#define ISP_IMD_WON3			_bm(_ISP_006A, ISP_REG_BASE, (0x006A<<2), IMD_WON3			)	//	
#define ISP_IMD_WHSP3			_bm(_ISP_006A, ISP_REG_BASE, (0x006A<<2), IMD_WHSP3			)	//	
#define ISP_IMD_WHEP3			_bm(_ISP_006A, ISP_REG_BASE, (0x006A<<2), IMD_WHEP3			)	//	
#define ISP_IMD_WVSP3			_bm(_ISP_006A, ISP_REG_BASE, (0x006A<<2), IMD_WVSP3			)	//	
#define ISP_IMD_WVEP3			_bm(_ISP_006A, ISP_REG_BASE, (0x006A<<2), IMD_WVEP3			)	//	
#define ISP_006A_T				_am(_ISP_006A, ISP_REG_BASE, (0x006A<<2))						//	

_regs_	UINT IMD_WON4			: 1;
		UINT IMD_WHSP4			: 7;
		UINT _res0				: 1;
		UINT IMD_WHEP4			: 7;
		UINT _res1				: 2;
		UINT IMD_WVSP4			: 6;
		UINT _res2				: 2;
		UINT IMD_WVEP4			: 6;	_rege_	_ISP_006B;

#define ISP_IMD_WON4			_bm(_ISP_006B, ISP_REG_BASE, (0x006B<<2), IMD_WON4			)	//	
#define ISP_IMD_WHSP4			_bm(_ISP_006B, ISP_REG_BASE, (0x006B<<2), IMD_WHSP4			)	//	
#define ISP_IMD_WHEP4			_bm(_ISP_006B, ISP_REG_BASE, (0x006B<<2), IMD_WHEP4			)	//	
#define ISP_IMD_WVSP4			_bm(_ISP_006B, ISP_REG_BASE, (0x006B<<2), IMD_WVSP4			)	//	
#define ISP_IMD_WVEP4			_bm(_ISP_006B, ISP_REG_BASE, (0x006B<<2), IMD_WVEP4			)	//	
#define ISP_006B_T				_am(_ISP_006B, ISP_REG_BASE, (0x006B<<2))						//	

_regs_	UINT _res0				: 1;
		UINT OZONE_SEL			: 3;
		UINT _res1				:28;	_rege_	_ISP_006D;

#define ISP_OZONE_SEL			_bm(_ISP_006D, ISP_REG_BASE, (0x006D<<2), OZONE_SEL			)	//	
#define ISP_006D_T				_am(_ISP_006D, ISP_REG_BASE, (0x006D<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE_LOCK_SEL		: 1;
		UINT AF_YSEL			: 2;
		UINT AF_YLPF			: 1;
		UINT AE_WIN_SEL			: 4;
		UINT _res1				:20;	_rege_	_ISP_006E;

#define ISP_AE_LOCK_SEL			_bm(_ISP_006E, ISP_REG_BASE, (0x006E<<2), AE_LOCK_SEL		)	//	
#define ISP_AF_YSEL				_bm(_ISP_006E, ISP_REG_BASE, (0x006E<<2), AF_YSEL			)	//	
#define ISP_AF_YLPF				_bm(_ISP_006E, ISP_REG_BASE, (0x006E<<2), AF_YLPF			)	//	
#define ISP_AE_WIN_SEL			_bm(_ISP_006E, ISP_REG_BASE, (0x006E<<2), AE_WIN_SEL		)	//	
#define ISP_006E_T				_am(_ISP_006E, ISP_REG_BASE, (0x006E<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE1_HSP			:12;
		UINT _res1				: 4;
		UINT AE1_HEP			:12;	_rege_	_ISP_0070;

#define ISP_AE1_HSP				_bm(_ISP_0070, ISP_REG_BASE, (0x0070<<2), AE1_HSP			)	//	
#define ISP_AE1_HEP				_bm(_ISP_0070, ISP_REG_BASE, (0x0070<<2), AE1_HEP			)	//	
#define ISP_0070_T				_am(_ISP_0070, ISP_REG_BASE, (0x0070<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE1_VSP			:12;
		UINT _res1				: 4;
		UINT AE1_VEP			:12;	_rege_	_ISP_0071;

#define ISP_AE1_VSP				_bm(_ISP_0071, ISP_REG_BASE, (0x0071<<2), AE1_VSP			)	//	
#define ISP_AE1_VEP				_bm(_ISP_0071, ISP_REG_BASE, (0x0071<<2), AE1_VEP			)	//	
#define ISP_0071_T				_am(_ISP_0071, ISP_REG_BASE, (0x0071<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE2_HSP			:12;
		UINT _res1				: 4;
		UINT AE2_HEP			:12;	_rege_	_ISP_0072;

#define ISP_AE2_HSP				_bm(_ISP_0072, ISP_REG_BASE, (0x0072<<2), AE2_HSP			)	//	
#define ISP_AE2_HEP				_bm(_ISP_0072, ISP_REG_BASE, (0x0072<<2), AE2_HEP			)	//	
#define ISP_0072_T				_am(_ISP_0072, ISP_REG_BASE, (0x0072<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE2_VSP			:12;
		UINT _res1				: 4;
		UINT AE2_VEP			:12;	_rege_	_ISP_0073;

#define ISP_AE2_VSP				_bm(_ISP_0073, ISP_REG_BASE, (0x0073<<2), AE2_VSP			)	//	
#define ISP_AE2_VEP				_bm(_ISP_0073, ISP_REG_BASE, (0x0073<<2), AE2_VEP			)	//	
#define ISP_0073_T				_am(_ISP_0073, ISP_REG_BASE, (0x0073<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE3_HSP			:12;
		UINT _res1				: 4;
		UINT AE3_HEP			:12;	_rege_	_ISP_0074;

#define ISP_AE3_HSP				_bm(_ISP_0074, ISP_REG_BASE, (0x0074<<2), AE3_HSP			)	//	
#define ISP_AE3_HEP				_bm(_ISP_0074, ISP_REG_BASE, (0x0074<<2), AE3_HEP			)	//	
#define ISP_0074_T				_am(_ISP_0074, ISP_REG_BASE, (0x0074<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE3_VSP			:12;
		UINT _res1				: 4;
		UINT AE3_VEP			:12;	_rege_	_ISP_0075;

#define ISP_AE3_VSP				_bm(_ISP_0075, ISP_REG_BASE, (0x0075<<2), AE3_VSP			)	//	
#define ISP_AE3_VEP				_bm(_ISP_0075, ISP_REG_BASE, (0x0075<<2), AE3_VEP			)	//	
#define ISP_0075_T				_am(_ISP_0075, ISP_REG_BASE, (0x0075<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE4_HSP			:12;
		UINT _res1				: 4;
		UINT AE4_HEP			:12;	_rege_	_ISP_0076;

#define ISP_AE4_HSP				_bm(_ISP_0076, ISP_REG_BASE, (0x0076<<2), AE4_HSP			)	//	
#define ISP_AE4_HEP				_bm(_ISP_0076, ISP_REG_BASE, (0x0076<<2), AE4_HEP			)	//	
#define ISP_0076_T				_am(_ISP_0076, ISP_REG_BASE, (0x0076<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AE4_VSP			:12;
		UINT _res1				: 4;
		UINT AE4_VEP			:12;	_rege_	_ISP_0077;

#define ISP_AE4_VSP				_bm(_ISP_0077, ISP_REG_BASE, (0x0077<<2), AE4_VSP			)	//	
#define ISP_AE4_VEP				_bm(_ISP_0077, ISP_REG_BASE, (0x0077<<2), AE4_VEP			)	//	
#define ISP_0077_T				_am(_ISP_0077, ISP_REG_BASE, (0x0077<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AE1_SLICE			:10;
		UINT _res1				: 6;
		UINT AE1_CLIP			:10;	_rege_	_ISP_0078;

#define ISP_AE1_SLICE			_bm(_ISP_0078, ISP_REG_BASE, (0x0078<<2), AE1_SLICE			)	//	
#define ISP_AE1_CLIP			_bm(_ISP_0078, ISP_REG_BASE, (0x0078<<2), AE1_CLIP			)	//	
#define ISP_0078_T				_am(_ISP_0078, ISP_REG_BASE, (0x0078<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AE2_SLICE			:10;
		UINT _res1				: 6;
		UINT AE2_CLIP			:10;	_rege_	_ISP_0079;

#define ISP_AE2_SLICE			_bm(_ISP_0079, ISP_REG_BASE, (0x0079<<2), AE2_SLICE			)	//	
#define ISP_AE2_CLIP			_bm(_ISP_0079, ISP_REG_BASE, (0x0079<<2), AE2_CLIP			)	//	
#define ISP_0079_T				_am(_ISP_0079, ISP_REG_BASE, (0x0079<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AE3_SLICE			:10;
		UINT _res1				: 6;
		UINT AE3_CLIP			:10;	_rege_	_ISP_007A;

#define ISP_AE3_SLICE			_bm(_ISP_007A, ISP_REG_BASE, (0x007A<<2), AE3_SLICE			)	//	
#define ISP_AE3_CLIP			_bm(_ISP_007A, ISP_REG_BASE, (0x007A<<2), AE3_CLIP			)	//	
#define ISP_007A_T				_am(_ISP_007A, ISP_REG_BASE, (0x007A<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AE4_SLICE			:10;
		UINT _res1				: 6;
		UINT AE4_CLIP			:10;	_rege_	_ISP_007B;

#define ISP_AE4_SLICE			_bm(_ISP_007B, ISP_REG_BASE, (0x007B<<2), AE4_SLICE			)	//	
#define ISP_AE4_CLIP			_bm(_ISP_007B, ISP_REG_BASE, (0x007B<<2), AE4_CLIP			)	//	
#define ISP_007B_T				_am(_ISP_007B, ISP_REG_BASE, (0x007B<<2))						//	

_regs_	UINT _res0				: 1;
		UINT HISTO0_MAX			: 7;
		UINT _res1				: 1;
		UINT HISTO1_MAX			: 7;
		UINT _res2				: 1;
		UINT HISTO2_MAX			: 7;
		UINT _res3				: 1;
		UINT HISTO3_MAX			: 7;	_rege_	_ISP_007C;

#define ISP_HISTO0_MAX			_bm(_ISP_007C, ISP_REG_BASE, (0x007C<<2), HISTO0_MAX		)	//	
#define ISP_HISTO1_MAX			_bm(_ISP_007C, ISP_REG_BASE, (0x007C<<2), HISTO1_MAX		)	//	
#define ISP_HISTO2_MAX			_bm(_ISP_007C, ISP_REG_BASE, (0x007C<<2), HISTO2_MAX		)	//	
#define ISP_HISTO3_MAX			_bm(_ISP_007C, ISP_REG_BASE, (0x007C<<2), HISTO3_MAX		)	//	
#define ISP_007C_T				_am(_ISP_007C, ISP_REG_BASE, (0x007C<<2))						//	

_regs_	UINT _res0				: 1;
		UINT HISTO4_MAX			: 7;
		UINT _res1				: 1;
		UINT HISTO5_MAX			: 7;
		UINT _res2				: 1;
		UINT HISTO6_MAX			: 7;
		UINT _res3				: 1;
		UINT HISTO7_MAX			: 7;	_rege_	_ISP_007D;

#define ISP_HISTO4_MAX			_bm(_ISP_007D, ISP_REG_BASE, (0x007D<<2), HISTO4_MAX		)	//	
#define ISP_HISTO5_MAX			_bm(_ISP_007D, ISP_REG_BASE, (0x007D<<2), HISTO5_MAX		)	//	
#define ISP_HISTO6_MAX			_bm(_ISP_007D, ISP_REG_BASE, (0x007D<<2), HISTO6_MAX		)	//	
#define ISP_HISTO7_MAX			_bm(_ISP_007D, ISP_REG_BASE, (0x007D<<2), HISTO7_MAX		)	//	
#define ISP_007D_T				_am(_ISP_007D, ISP_REG_BASE, (0x007D<<2))						//	

_regs_	UINT _res0				: 1;
		UINT HISTO8_MAX			: 7;
		UINT _res1				: 1;
		UINT HISTO9_MAX			: 7;
		UINT _res2				: 1;
		UINT HISTO10_MAX		: 7;
		UINT _res3				: 1;
		UINT HISTO11_MAX		: 7;	_rege_	_ISP_007E;

#define ISP_HISTO8_MAX			_bm(_ISP_007E, ISP_REG_BASE, (0x007E<<2), HISTO8_MAX		)	//	
#define ISP_HISTO9_MAX			_bm(_ISP_007E, ISP_REG_BASE, (0x007E<<2), HISTO9_MAX		)	//	
#define ISP_HISTO10_MAX			_bm(_ISP_007E, ISP_REG_BASE, (0x007E<<2), HISTO10_MAX		)	//	
#define ISP_HISTO11_MAX			_bm(_ISP_007E, ISP_REG_BASE, (0x007E<<2), HISTO11_MAX		)	//	
#define ISP_007E_T				_am(_ISP_007E, ISP_REG_BASE, (0x007E<<2))						//	

_regs_	UINT _res0				: 1;
		UINT HISTO12_MAX		: 7;
		UINT _res1				: 1;
		UINT HISTO13_MAX		: 7;
		UINT _res2				: 1;
		UINT HISTO14_MAX		: 7;
		UINT _res3				: 8;	_rege_	_ISP_007F;

#define ISP_HISTO12_MAX			_bm(_ISP_007F, ISP_REG_BASE, (0x007F<<2), HISTO12_MAX		)	//	
#define ISP_HISTO13_MAX			_bm(_ISP_007F, ISP_REG_BASE, (0x007F<<2), HISTO13_MAX		)	//	
#define ISP_HISTO14_MAX			_bm(_ISP_007F, ISP_REG_BASE, (0x007F<<2), HISTO14_MAX		)	//	
#define ISP_007F_T				_am(_ISP_007F, ISP_REG_BASE, (0x007F<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AWB_WIN1_HSP		:12;
		UINT _res1				: 4;
		UINT AWB_WIN1_HW		:12;	_rege_	_ISP_0080;

#define ISP_AWB_WIN1_HSP		_bm(_ISP_0080, ISP_REG_BASE, (0x0080<<2), AWB_WIN1_HSP		)	//	
#define ISP_AWB_WIN1_HW			_bm(_ISP_0080, ISP_REG_BASE, (0x0080<<2), AWB_WIN1_HW		)	//	
#define ISP_0080_T				_am(_ISP_0080, ISP_REG_BASE, (0x0080<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AWB_WIN1_VSP		:12;
		UINT _res1				: 4;
		UINT AWB_WIN1_VW		:12;	_rege_	_ISP_0081;

#define ISP_AWB_WIN1_VSP		_bm(_ISP_0081, ISP_REG_BASE, (0x0081<<2), AWB_WIN1_VSP		)	//	
#define ISP_AWB_WIN1_VW			_bm(_ISP_0081, ISP_REG_BASE, (0x0081<<2), AWB_WIN1_VW		)	//	
#define ISP_0081_T				_am(_ISP_0081, ISP_REG_BASE, (0x0081<<2))						//	

_regs_	UINT _res0				:16;
		UINT AWBL_DRGAIN0		: 8;
		UINT AWBL_DBGAIN0		: 8;	_rege_	_ISP_0082;

#define ISP_AWBL_DRGAIN0		_bm(_ISP_0082, ISP_REG_BASE, (0x0082<<2), AWBL_DRGAIN0		)	//	
#define ISP_AWBL_DBGAIN0		_bm(_ISP_0082, ISP_REG_BASE, (0x0082<<2), AWBL_DBGAIN0		)	//	
#define ISP_0082_T				_am(_ISP_0082, ISP_REG_BASE, (0x0082<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AWBL0_DBOFFSET		:10;
		UINT _res1				: 6;
		UINT AWBL0_DROFFSET		:10;	_rege_	_ISP_0083;

#define ISP_AWBL0_DBOFFSET		_bm(_ISP_0083, ISP_REG_BASE, (0x0083<<2), AWBL0_DBOFFSET	)	//	
#define ISP_AWBL0_DROFFSET		_bm(_ISP_0083, ISP_REG_BASE, (0x0083<<2), AWBL0_DROFFSET	)	//	
#define ISP_0083_T				_am(_ISP_0083, ISP_REG_BASE, (0x0083<<2))						//	

_regs_	UINT _res0				: 1;
		UINT AWBLCBCRO_SEL		: 1;
		UINT AWBLNOR_SEL		: 1;
		UINT AWB_LOCK_SEL		: 1;
		UINT _res1				: 2;
		UINT AWBL_HTAB_SEL		: 2;
		UINT _res2				:14;
		UINT AWBLYEDGE_TH		:10;	_rege_	_ISP_0084;

#define ISP_AWBLCBCRO_SEL		_bm(_ISP_0084, ISP_REG_BASE, (0x0084<<2), AWBLCBCRO_SEL		)	//	
#define ISP_AWBLNOR_SEL			_bm(_ISP_0084, ISP_REG_BASE, (0x0084<<2), AWBLNOR_SEL		)	//	
#define ISP_AWB_LOCK_SEL		_bm(_ISP_0084, ISP_REG_BASE, (0x0084<<2), AWB_LOCK_SEL		)	//	
#define ISP_AWBL_HTAB_SEL		_bm(_ISP_0084, ISP_REG_BASE, (0x0084<<2), AWBL_HTAB_SEL		)	//	
#define ISP_AWBLYEDGE_TH		_bm(_ISP_0084, ISP_REG_BASE, (0x0084<<2), AWBLYEDGE_TH		)	//	
#define ISP_0084_T				_am(_ISP_0084, ISP_REG_BASE, (0x0084<<2))						//	

_regs_	UINT _res0				:23;
		UINT AWBLOSD_ON			: 1;
		UINT AWBL_OSD_SEL		: 8;	_rege_	_ISP_0085;

#define ISP_AWBLOSD_ON			_bm(_ISP_0085, ISP_REG_BASE, (0x0085<<2), AWBLOSD_ON		)	//	
#define ISP_AWBL_OSD_SEL		_bm(_ISP_0085, ISP_REG_BASE, (0x0085<<2), AWBL_OSD_SEL		)	//	
#define ISP_0085_T				_am(_ISP_0085, ISP_REG_BASE, (0x0085<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AWBL_CYCLIP		:10;
		UINT _res1				: 6;
		UINT AWBL_CYSLICE		:10;	_rege_	_ISP_0086;

#define ISP_AWBL_CYCLIP			_bm(_ISP_0086, ISP_REG_BASE, (0x0086<<2), AWBL_CYCLIP		)	//	
#define ISP_AWBL_CYSLICE		_bm(_ISP_0086, ISP_REG_BASE, (0x0086<<2), AWBL_CYSLICE		)	//	
#define ISP_0086_T				_am(_ISP_0086, ISP_REG_BASE, (0x0086<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AWBL_CM_HSP		:12;
		UINT _res1				: 4;
		UINT AWBL_CM_VSP		:12;	_rege_	_ISP_0087;

#define ISP_AWBL_CM_HSP			_bm(_ISP_0087, ISP_REG_BASE, (0x0087<<2), AWBL_CM_HSP		)	//	
#define ISP_AWBL_CM_VSP			_bm(_ISP_0087, ISP_REG_BASE, (0x0087<<2), AWBL_CM_VSP		)	//	
#define ISP_0087_T				_am(_ISP_0087, ISP_REG_BASE, (0x0087<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM00_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM00_HEP		: 6;	_rege_	_ISP_0088;

#define ISP_AWBL_CM00_HSP		_bm(_ISP_0088, ISP_REG_BASE, (0x0088<<2), AWBL_CM00_HSP		)	//	
#define ISP_AWBL_CM00_HEP		_bm(_ISP_0088, ISP_REG_BASE, (0x0088<<2), AWBL_CM00_HEP		)	//	
#define ISP_0088_T				_am(_ISP_0088, ISP_REG_BASE, (0x0088<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM01_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM01_HEP		: 6;	_rege_	_ISP_0089;

#define ISP_AWBL_CM01_HSP		_bm(_ISP_0089, ISP_REG_BASE, (0x0089<<2), AWBL_CM01_HSP		)	//	
#define ISP_AWBL_CM01_HEP		_bm(_ISP_0089, ISP_REG_BASE, (0x0089<<2), AWBL_CM01_HEP		)	//	
#define ISP_0089_T				_am(_ISP_0089, ISP_REG_BASE, (0x0089<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM02_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM02_HEP		: 6;	_rege_	_ISP_008A;

#define ISP_AWBL_CM02_HSP		_bm(_ISP_008A, ISP_REG_BASE, (0x008A<<2), AWBL_CM02_HSP		)	//	
#define ISP_AWBL_CM02_HEP		_bm(_ISP_008A, ISP_REG_BASE, (0x008A<<2), AWBL_CM02_HEP		)	//	
#define ISP_008A_T				_am(_ISP_008A, ISP_REG_BASE, (0x008A<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM03_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM03_HEP		: 6;	_rege_	_ISP_008B;

#define ISP_AWBL_CM03_HSP		_bm(_ISP_008B, ISP_REG_BASE, (0x008B<<2), AWBL_CM03_HSP		)	//	
#define ISP_AWBL_CM03_HEP		_bm(_ISP_008B, ISP_REG_BASE, (0x008B<<2), AWBL_CM03_HEP		)	//	
#define ISP_008B_T				_am(_ISP_008B, ISP_REG_BASE, (0x008B<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM04_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM04_HEP		: 6;	_rege_	_ISP_008C;

#define ISP_AWBL_CM04_HSP		_bm(_ISP_008C, ISP_REG_BASE, (0x008C<<2), AWBL_CM04_HSP		)	//	
#define ISP_AWBL_CM04_HEP		_bm(_ISP_008C, ISP_REG_BASE, (0x008C<<2), AWBL_CM04_HEP		)	//	
#define ISP_008C_T				_am(_ISP_008C, ISP_REG_BASE, (0x008C<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM05_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM05_HEP		: 6;	_rege_	_ISP_008D;

#define ISP_AWBL_CM05_HSP		_bm(_ISP_008D, ISP_REG_BASE, (0x008D<<2), AWBL_CM05_HSP		)	//	
#define ISP_AWBL_CM05_HEP		_bm(_ISP_008D, ISP_REG_BASE, (0x008D<<2), AWBL_CM05_HEP		)	//	
#define ISP_008D_T				_am(_ISP_008D, ISP_REG_BASE, (0x008D<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM06_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM06_HEP		: 6;	_rege_	_ISP_008E;

#define ISP_AWBL_CM06_HSP		_bm(_ISP_008E, ISP_REG_BASE, (0x008E<<2), AWBL_CM06_HSP		)	//	
#define ISP_AWBL_CM06_HEP		_bm(_ISP_008E, ISP_REG_BASE, (0x008E<<2), AWBL_CM06_HEP		)	//	
#define ISP_008E_T				_am(_ISP_008E, ISP_REG_BASE, (0x008E<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM07_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM07_HEP		: 6;	_rege_	_ISP_008F;

#define ISP_AWBL_CM07_HSP		_bm(_ISP_008F, ISP_REG_BASE, (0x008F<<2), AWBL_CM07_HSP		)	//	
#define ISP_AWBL_CM07_HEP		_bm(_ISP_008F, ISP_REG_BASE, (0x008F<<2), AWBL_CM07_HEP		)	//	
#define ISP_008F_T				_am(_ISP_008F, ISP_REG_BASE, (0x008F<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM08_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM08_HEP		: 6;	_rege_	_ISP_0090;

#define ISP_AWBL_CM08_HSP		_bm(_ISP_0090, ISP_REG_BASE, (0x0090<<2), AWBL_CM08_HSP		)	//	
#define ISP_AWBL_CM08_HEP		_bm(_ISP_0090, ISP_REG_BASE, (0x0090<<2), AWBL_CM08_HEP		)	//	
#define ISP_0090_T				_am(_ISP_0090, ISP_REG_BASE, (0x0090<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM09_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM09_HEP		: 6;	_rege_	_ISP_0091;

#define ISP_AWBL_CM09_HSP		_bm(_ISP_0091, ISP_REG_BASE, (0x0091<<2), AWBL_CM09_HSP		)	//	
#define ISP_AWBL_CM09_HEP		_bm(_ISP_0091, ISP_REG_BASE, (0x0091<<2), AWBL_CM09_HEP		)	//	
#define ISP_0091_T				_am(_ISP_0091, ISP_REG_BASE, (0x0091<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM10_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM10_HEP		: 6;	_rege_	_ISP_0092;

#define ISP_AWBL_CM10_HSP		_bm(_ISP_0092, ISP_REG_BASE, (0x0092<<2), AWBL_CM10_HSP		)	//	
#define ISP_AWBL_CM10_HEP		_bm(_ISP_0092, ISP_REG_BASE, (0x0092<<2), AWBL_CM10_HEP		)	//	
#define ISP_0092_T				_am(_ISP_0092, ISP_REG_BASE, (0x0092<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM11_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM11_HEP		: 6;	_rege_	_ISP_0093;

#define ISP_AWBL_CM11_HSP		_bm(_ISP_0093, ISP_REG_BASE, (0x0093<<2), AWBL_CM11_HSP		)	//	
#define ISP_AWBL_CM11_HEP		_bm(_ISP_0093, ISP_REG_BASE, (0x0093<<2), AWBL_CM11_HEP		)	//	
#define ISP_0093_T				_am(_ISP_0093, ISP_REG_BASE, (0x0093<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM12_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM12_HEP		: 6;	_rege_	_ISP_0094;

#define ISP_AWBL_CM12_HSP		_bm(_ISP_0094, ISP_REG_BASE, (0x0094<<2), AWBL_CM12_HSP		)	//	
#define ISP_AWBL_CM12_HEP		_bm(_ISP_0094, ISP_REG_BASE, (0x0094<<2), AWBL_CM12_HEP		)	//	
#define ISP_0094_T				_am(_ISP_0094, ISP_REG_BASE, (0x0094<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM13_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM13_HEP		: 6;	_rege_	_ISP_0095;

#define ISP_AWBL_CM13_HSP		_bm(_ISP_0095, ISP_REG_BASE, (0x0095<<2), AWBL_CM13_HSP		)	//	
#define ISP_AWBL_CM13_HEP		_bm(_ISP_0095, ISP_REG_BASE, (0x0095<<2), AWBL_CM13_HEP		)	//	
#define ISP_0095_T				_am(_ISP_0095, ISP_REG_BASE, (0x0095<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM14_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM14_HEP		: 6;	_rege_	_ISP_0096;

#define ISP_AWBL_CM14_HSP		_bm(_ISP_0096, ISP_REG_BASE, (0x0096<<2), AWBL_CM14_HSP		)	//	
#define ISP_AWBL_CM14_HEP		_bm(_ISP_0096, ISP_REG_BASE, (0x0096<<2), AWBL_CM14_HEP		)	//	
#define ISP_0096_T				_am(_ISP_0096, ISP_REG_BASE, (0x0096<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM15_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM15_HEP		: 6;	_rege_	_ISP_0097;

#define ISP_AWBL_CM15_HSP		_bm(_ISP_0097, ISP_REG_BASE, (0x0097<<2), AWBL_CM15_HSP		)	//	
#define ISP_AWBL_CM15_HEP		_bm(_ISP_0097, ISP_REG_BASE, (0x0097<<2), AWBL_CM15_HEP		)	//	
#define ISP_0097_T				_am(_ISP_0097, ISP_REG_BASE, (0x0097<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM16_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM16_HEP		: 6;	_rege_	_ISP_0098;

#define ISP_AWBL_CM16_HSP		_bm(_ISP_0098, ISP_REG_BASE, (0x0098<<2), AWBL_CM16_HSP		)	//	
#define ISP_AWBL_CM16_HEP		_bm(_ISP_0098, ISP_REG_BASE, (0x0098<<2), AWBL_CM16_HEP		)	//	
#define ISP_0098_T				_am(_ISP_0098, ISP_REG_BASE, (0x0098<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM17_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM17_HEP		: 6;	_rege_	_ISP_0099;

#define ISP_AWBL_CM17_HSP		_bm(_ISP_0099, ISP_REG_BASE, (0x0099<<2), AWBL_CM17_HSP		)	//	
#define ISP_AWBL_CM17_HEP		_bm(_ISP_0099, ISP_REG_BASE, (0x0099<<2), AWBL_CM17_HEP		)	//	
#define ISP_0099_T				_am(_ISP_0099, ISP_REG_BASE, (0x0099<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM18_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM18_HEP		: 6;	_rege_	_ISP_009A;

#define ISP_AWBL_CM18_HSP		_bm(_ISP_009A, ISP_REG_BASE, (0x009A<<2), AWBL_CM18_HSP		)	//	
#define ISP_AWBL_CM18_HEP		_bm(_ISP_009A, ISP_REG_BASE, (0x009A<<2), AWBL_CM18_HEP		)	//	
#define ISP_009A_T				_am(_ISP_009A, ISP_REG_BASE, (0x009A<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM19_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM19_HEP		: 6;	_rege_	_ISP_009B;

#define ISP_AWBL_CM19_HSP		_bm(_ISP_009B, ISP_REG_BASE, (0x009B<<2), AWBL_CM19_HSP		)	//	
#define ISP_AWBL_CM19_HEP		_bm(_ISP_009B, ISP_REG_BASE, (0x009B<<2), AWBL_CM19_HEP		)	//	
#define ISP_009B_T				_am(_ISP_009B, ISP_REG_BASE, (0x009B<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM20_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM20_HEP		: 6;	_rege_	_ISP_009C;

#define ISP_AWBL_CM20_HSP		_bm(_ISP_009C, ISP_REG_BASE, (0x009C<<2), AWBL_CM20_HSP		)	//	
#define ISP_AWBL_CM20_HEP		_bm(_ISP_009C, ISP_REG_BASE, (0x009C<<2), AWBL_CM20_HEP		)	//	
#define ISP_009C_T				_am(_ISP_009C, ISP_REG_BASE, (0x009C<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM21_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM21_HEP		: 6;	_rege_	_ISP_009D;

#define ISP_AWBL_CM21_HSP		_bm(_ISP_009D, ISP_REG_BASE, (0x009D<<2), AWBL_CM21_HSP		)	//	
#define ISP_AWBL_CM21_HEP		_bm(_ISP_009D, ISP_REG_BASE, (0x009D<<2), AWBL_CM21_HEP		)	//	
#define ISP_009D_T				_am(_ISP_009D, ISP_REG_BASE, (0x009D<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM22_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM22_HEP		: 6;	_rege_	_ISP_009E;

#define ISP_AWBL_CM22_HSP		_bm(_ISP_009E, ISP_REG_BASE, (0x009E<<2), AWBL_CM22_HSP		)	//	
#define ISP_AWBL_CM22_HEP		_bm(_ISP_009E, ISP_REG_BASE, (0x009E<<2), AWBL_CM22_HEP		)	//	
#define ISP_009E_T				_am(_ISP_009E, ISP_REG_BASE, (0x009E<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM23_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM23_HEP		: 6;	_rege_	_ISP_009F;

#define ISP_AWBL_CM23_HSP		_bm(_ISP_009F, ISP_REG_BASE, (0x009F<<2), AWBL_CM23_HSP		)	//	
#define ISP_AWBL_CM23_HEP		_bm(_ISP_009F, ISP_REG_BASE, (0x009F<<2), AWBL_CM23_HEP		)	//	
#define ISP_009F_T				_am(_ISP_009F, ISP_REG_BASE, (0x009F<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM24_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM24_HEP		: 6;	_rege_	_ISP_00A0;

#define ISP_AWBL_CM24_HSP		_bm(_ISP_00A0, ISP_REG_BASE, (0x00A0<<2), AWBL_CM24_HSP		)	//	
#define ISP_AWBL_CM24_HEP		_bm(_ISP_00A0, ISP_REG_BASE, (0x00A0<<2), AWBL_CM24_HEP		)	//	
#define ISP_00A0_T				_am(_ISP_00A0, ISP_REG_BASE, (0x00A0<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM25_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM25_HEP		: 6;	_rege_	_ISP_00A1;

#define ISP_AWBL_CM25_HSP		_bm(_ISP_00A1, ISP_REG_BASE, (0x00A1<<2), AWBL_CM25_HSP		)	//	
#define ISP_AWBL_CM25_HEP		_bm(_ISP_00A1, ISP_REG_BASE, (0x00A1<<2), AWBL_CM25_HEP		)	//	
#define ISP_00A1_T				_am(_ISP_00A1, ISP_REG_BASE, (0x00A1<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM26_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM26_HEP		: 6;	_rege_	_ISP_00A2;

#define ISP_AWBL_CM26_HSP		_bm(_ISP_00A2, ISP_REG_BASE, (0x00A2<<2), AWBL_CM26_HSP		)	//	
#define ISP_AWBL_CM26_HEP		_bm(_ISP_00A2, ISP_REG_BASE, (0x00A2<<2), AWBL_CM26_HEP		)	//	
#define ISP_00A2_T				_am(_ISP_00A2, ISP_REG_BASE, (0x00A2<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM27_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM27_HEP		: 6;	_rege_	_ISP_00A3;

#define ISP_AWBL_CM27_HSP		_bm(_ISP_00A3, ISP_REG_BASE, (0x00A3<<2), AWBL_CM27_HSP		)	//	
#define ISP_AWBL_CM27_HEP		_bm(_ISP_00A3, ISP_REG_BASE, (0x00A3<<2), AWBL_CM27_HEP		)	//	
#define ISP_00A3_T				_am(_ISP_00A3, ISP_REG_BASE, (0x00A3<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM28_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM28_HEP		: 6;	_rege_	_ISP_00A4;

#define ISP_AWBL_CM28_HSP		_bm(_ISP_00A4, ISP_REG_BASE, (0x00A4<<2), AWBL_CM28_HSP		)	//	
#define ISP_AWBL_CM28_HEP		_bm(_ISP_00A4, ISP_REG_BASE, (0x00A4<<2), AWBL_CM28_HEP		)	//	
#define ISP_00A4_T				_am(_ISP_00A4, ISP_REG_BASE, (0x00A4<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM29_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM29_HEP		: 6;	_rege_	_ISP_00A5;

#define ISP_AWBL_CM29_HSP		_bm(_ISP_00A5, ISP_REG_BASE, (0x00A5<<2), AWBL_CM29_HSP		)	//	
#define ISP_AWBL_CM29_HEP		_bm(_ISP_00A5, ISP_REG_BASE, (0x00A5<<2), AWBL_CM29_HEP		)	//	
#define ISP_00A5_T				_am(_ISP_00A5, ISP_REG_BASE, (0x00A5<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM30_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM30_HEP		: 6;	_rege_	_ISP_00A6;

#define ISP_AWBL_CM30_HSP		_bm(_ISP_00A6, ISP_REG_BASE, (0x00A6<<2), AWBL_CM30_HSP		)	//	
#define ISP_AWBL_CM30_HEP		_bm(_ISP_00A6, ISP_REG_BASE, (0x00A6<<2), AWBL_CM30_HEP		)	//	
#define ISP_00A6_T				_am(_ISP_00A6, ISP_REG_BASE, (0x00A6<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM31_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM31_HEP		: 6;	_rege_	_ISP_00A7;

#define ISP_AWBL_CM31_HSP		_bm(_ISP_00A7, ISP_REG_BASE, (0x00A7<<2), AWBL_CM31_HSP		)	//	
#define ISP_AWBL_CM31_HEP		_bm(_ISP_00A7, ISP_REG_BASE, (0x00A7<<2), AWBL_CM31_HEP		)	//	
#define ISP_00A7_T				_am(_ISP_00A7, ISP_REG_BASE, (0x00A7<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM32_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM32_HEP		: 6;	_rege_	_ISP_00A8;

#define ISP_AWBL_CM32_HSP		_bm(_ISP_00A8, ISP_REG_BASE, (0x00A8<<2), AWBL_CM32_HSP		)	//	
#define ISP_AWBL_CM32_HEP		_bm(_ISP_00A8, ISP_REG_BASE, (0x00A8<<2), AWBL_CM32_HEP		)	//	
#define ISP_00A8_T				_am(_ISP_00A8, ISP_REG_BASE, (0x00A8<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM33_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM33_HEP		: 6;	_rege_	_ISP_00A9;

#define ISP_AWBL_CM33_HSP		_bm(_ISP_00A9, ISP_REG_BASE, (0x00A9<<2), AWBL_CM33_HSP		)	//	
#define ISP_AWBL_CM33_HEP		_bm(_ISP_00A9, ISP_REG_BASE, (0x00A9<<2), AWBL_CM33_HEP		)	//	
#define ISP_00A9_T				_am(_ISP_00A9, ISP_REG_BASE, (0x00A9<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM34_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM34_HEP		: 6;	_rege_	_ISP_00AA;

#define ISP_AWBL_CM34_HSP		_bm(_ISP_00AA, ISP_REG_BASE, (0x00AA<<2), AWBL_CM34_HSP		)	//	
#define ISP_AWBL_CM34_HEP		_bm(_ISP_00AA, ISP_REG_BASE, (0x00AA<<2), AWBL_CM34_HEP		)	//	
#define ISP_00AA_T				_am(_ISP_00AA, ISP_REG_BASE, (0x00AA<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM35_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM35_HEP		: 6;	_rege_	_ISP_00AB;

#define ISP_AWBL_CM35_HSP		_bm(_ISP_00AB, ISP_REG_BASE, (0x00AB<<2), AWBL_CM35_HSP		)	//	
#define ISP_AWBL_CM35_HEP		_bm(_ISP_00AB, ISP_REG_BASE, (0x00AB<<2), AWBL_CM35_HEP		)	//	
#define ISP_00AB_T				_am(_ISP_00AB, ISP_REG_BASE, (0x00AB<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM36_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM36_HEP		: 6;	_rege_	_ISP_00AC;

#define ISP_AWBL_CM36_HSP		_bm(_ISP_00AC, ISP_REG_BASE, (0x00AC<<2), AWBL_CM36_HSP		)	//	
#define ISP_AWBL_CM36_HEP		_bm(_ISP_00AC, ISP_REG_BASE, (0x00AC<<2), AWBL_CM36_HEP		)	//	
#define ISP_00AC_T				_am(_ISP_00AC, ISP_REG_BASE, (0x00AC<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM37_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM37_HEP		: 6;	_rege_	_ISP_00AD;

#define ISP_AWBL_CM37_HSP		_bm(_ISP_00AD, ISP_REG_BASE, (0x00AD<<2), AWBL_CM37_HSP		)	//	
#define ISP_AWBL_CM37_HEP		_bm(_ISP_00AD, ISP_REG_BASE, (0x00AD<<2), AWBL_CM37_HEP		)	//	
#define ISP_00AD_T				_am(_ISP_00AD, ISP_REG_BASE, (0x00AD<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM38_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM38_HEP		: 6;	_rege_	_ISP_00AE;

#define ISP_AWBL_CM38_HSP		_bm(_ISP_00AE, ISP_REG_BASE, (0x00AE<<2), AWBL_CM38_HSP		)	//	
#define ISP_AWBL_CM38_HEP		_bm(_ISP_00AE, ISP_REG_BASE, (0x00AE<<2), AWBL_CM38_HEP		)	//	
#define ISP_00AE_T				_am(_ISP_00AE, ISP_REG_BASE, (0x00AE<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM39_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM39_HEP		: 6;	_rege_	_ISP_00AF;

#define ISP_AWBL_CM39_HSP		_bm(_ISP_00AF, ISP_REG_BASE, (0x00AF<<2), AWBL_CM39_HSP		)	//	
#define ISP_AWBL_CM39_HEP		_bm(_ISP_00AF, ISP_REG_BASE, (0x00AF<<2), AWBL_CM39_HEP		)	//	
#define ISP_00AF_T				_am(_ISP_00AF, ISP_REG_BASE, (0x00AF<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM40_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM40_HEP		: 6;	_rege_	_ISP_00B0;

#define ISP_AWBL_CM40_HSP		_bm(_ISP_00B0, ISP_REG_BASE, (0x00B0<<2), AWBL_CM40_HSP		)	//	
#define ISP_AWBL_CM40_HEP		_bm(_ISP_00B0, ISP_REG_BASE, (0x00B0<<2), AWBL_CM40_HEP		)	//	
#define ISP_00B0_T				_am(_ISP_00B0, ISP_REG_BASE, (0x00B0<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM41_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM41_HEP		: 6;	_rege_	_ISP_00B1;

#define ISP_AWBL_CM41_HSP		_bm(_ISP_00B1, ISP_REG_BASE, (0x00B1<<2), AWBL_CM41_HSP		)	//	
#define ISP_AWBL_CM41_HEP		_bm(_ISP_00B1, ISP_REG_BASE, (0x00B1<<2), AWBL_CM41_HEP		)	//	
#define ISP_00B1_T				_am(_ISP_00B1, ISP_REG_BASE, (0x00B1<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM42_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM42_HEP		: 6;	_rege_	_ISP_00B2;

#define ISP_AWBL_CM42_HSP		_bm(_ISP_00B2, ISP_REG_BASE, (0x00B2<<2), AWBL_CM42_HSP		)	//	
#define ISP_AWBL_CM42_HEP		_bm(_ISP_00B2, ISP_REG_BASE, (0x00B2<<2), AWBL_CM42_HEP		)	//	
#define ISP_00B2_T				_am(_ISP_00B2, ISP_REG_BASE, (0x00B2<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM43_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM43_HEP		: 6;	_rege_	_ISP_00B3;

#define ISP_AWBL_CM43_HSP		_bm(_ISP_00B3, ISP_REG_BASE, (0x00B3<<2), AWBL_CM43_HSP		)	//	
#define ISP_AWBL_CM43_HEP		_bm(_ISP_00B3, ISP_REG_BASE, (0x00B3<<2), AWBL_CM43_HEP		)	//	
#define ISP_00B3_T				_am(_ISP_00B3, ISP_REG_BASE, (0x00B3<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM44_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM44_HEP		: 6;	_rege_	_ISP_00B4;

#define ISP_AWBL_CM44_HSP		_bm(_ISP_00B4, ISP_REG_BASE, (0x00B4<<2), AWBL_CM44_HSP		)	//	
#define ISP_AWBL_CM44_HEP		_bm(_ISP_00B4, ISP_REG_BASE, (0x00B4<<2), AWBL_CM44_HEP		)	//	
#define ISP_00B4_T				_am(_ISP_00B4, ISP_REG_BASE, (0x00B4<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM45_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM45_HEP		: 6;	_rege_	_ISP_00B5;

#define ISP_AWBL_CM45_HSP		_bm(_ISP_00B5, ISP_REG_BASE, (0x00B5<<2), AWBL_CM45_HSP		)	//	
#define ISP_AWBL_CM45_HEP		_bm(_ISP_00B5, ISP_REG_BASE, (0x00B5<<2), AWBL_CM45_HEP		)	//	
#define ISP_00B5_T				_am(_ISP_00B5, ISP_REG_BASE, (0x00B5<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM46_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM46_HEP		: 6;	_rege_	_ISP_00B6;

#define ISP_AWBL_CM46_HSP		_bm(_ISP_00B6, ISP_REG_BASE, (0x00B6<<2), AWBL_CM46_HSP		)	//	
#define ISP_AWBL_CM46_HEP		_bm(_ISP_00B6, ISP_REG_BASE, (0x00B6<<2), AWBL_CM46_HEP		)	//	
#define ISP_00B6_T				_am(_ISP_00B6, ISP_REG_BASE, (0x00B6<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM47_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM47_HEP		: 6;	_rege_	_ISP_00B7;

#define ISP_AWBL_CM47_HSP		_bm(_ISP_00B7, ISP_REG_BASE, (0x00B7<<2), AWBL_CM47_HSP		)	//	
#define ISP_AWBL_CM47_HEP		_bm(_ISP_00B7, ISP_REG_BASE, (0x00B7<<2), AWBL_CM47_HEP		)	//	
#define ISP_00B7_T				_am(_ISP_00B7, ISP_REG_BASE, (0x00B7<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM48_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM48_HEP		: 6;	_rege_	_ISP_00B8;

#define ISP_AWBL_CM48_HSP		_bm(_ISP_00B8, ISP_REG_BASE, (0x00B8<<2), AWBL_CM48_HSP		)	//	
#define ISP_AWBL_CM48_HEP		_bm(_ISP_00B8, ISP_REG_BASE, (0x00B8<<2), AWBL_CM48_HEP		)	//	
#define ISP_00B8_T				_am(_ISP_00B8, ISP_REG_BASE, (0x00B8<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM49_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM49_HEP		: 6;	_rege_	_ISP_00B9;

#define ISP_AWBL_CM49_HSP		_bm(_ISP_00B9, ISP_REG_BASE, (0x00B9<<2), AWBL_CM49_HSP		)	//	
#define ISP_AWBL_CM49_HEP		_bm(_ISP_00B9, ISP_REG_BASE, (0x00B9<<2), AWBL_CM49_HEP		)	//	
#define ISP_00B9_T				_am(_ISP_00B9, ISP_REG_BASE, (0x00B9<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM50_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM50_HEP		: 6;	_rege_	_ISP_00BA;

#define ISP_AWBL_CM50_HSP		_bm(_ISP_00BA, ISP_REG_BASE, (0x00BA<<2), AWBL_CM50_HSP		)	//	
#define ISP_AWBL_CM50_HEP		_bm(_ISP_00BA, ISP_REG_BASE, (0x00BA<<2), AWBL_CM50_HEP		)	//	
#define ISP_00BA_T				_am(_ISP_00BA, ISP_REG_BASE, (0x00BA<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM51_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM51_HEP		: 6;	_rege_	_ISP_00BB;

#define ISP_AWBL_CM51_HSP		_bm(_ISP_00BB, ISP_REG_BASE, (0x00BB<<2), AWBL_CM51_HSP		)	//	
#define ISP_AWBL_CM51_HEP		_bm(_ISP_00BB, ISP_REG_BASE, (0x00BB<<2), AWBL_CM51_HEP		)	//	
#define ISP_00BB_T				_am(_ISP_00BB, ISP_REG_BASE, (0x00BB<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM52_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM52_HEP		: 6;	_rege_	_ISP_00BC;

#define ISP_AWBL_CM52_HSP		_bm(_ISP_00BC, ISP_REG_BASE, (0x00BC<<2), AWBL_CM52_HSP		)	//	
#define ISP_AWBL_CM52_HEP		_bm(_ISP_00BC, ISP_REG_BASE, (0x00BC<<2), AWBL_CM52_HEP		)	//	
#define ISP_00BC_T				_am(_ISP_00BC, ISP_REG_BASE, (0x00BC<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM53_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM53_HEP		: 6;	_rege_	_ISP_00BD;

#define ISP_AWBL_CM53_HSP		_bm(_ISP_00BD, ISP_REG_BASE, (0x00BD<<2), AWBL_CM53_HSP		)	//	
#define ISP_AWBL_CM53_HEP		_bm(_ISP_00BD, ISP_REG_BASE, (0x00BD<<2), AWBL_CM53_HEP		)	//	
#define ISP_00BD_T				_am(_ISP_00BD, ISP_REG_BASE, (0x00BD<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM54_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM54_HEP		: 6;	_rege_	_ISP_00BE;

#define ISP_AWBL_CM54_HSP		_bm(_ISP_00BE, ISP_REG_BASE, (0x00BE<<2), AWBL_CM54_HSP		)	//	
#define ISP_AWBL_CM54_HEP		_bm(_ISP_00BE, ISP_REG_BASE, (0x00BE<<2), AWBL_CM54_HEP		)	//	
#define ISP_00BE_T				_am(_ISP_00BE, ISP_REG_BASE, (0x00BE<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM55_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM55_HEP		: 6;	_rege_	_ISP_00BF;

#define ISP_AWBL_CM55_HSP		_bm(_ISP_00BF, ISP_REG_BASE, (0x00BF<<2), AWBL_CM55_HSP		)	//	
#define ISP_AWBL_CM55_HEP		_bm(_ISP_00BF, ISP_REG_BASE, (0x00BF<<2), AWBL_CM55_HEP		)	//	
#define ISP_00BF_T				_am(_ISP_00BF, ISP_REG_BASE, (0x00BF<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM56_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM56_HEP		: 6;	_rege_	_ISP_00C0;

#define ISP_AWBL_CM56_HSP		_bm(_ISP_00C0, ISP_REG_BASE, (0x00C0<<2), AWBL_CM56_HSP		)	//	
#define ISP_AWBL_CM56_HEP		_bm(_ISP_00C0, ISP_REG_BASE, (0x00C0<<2), AWBL_CM56_HEP		)	//	
#define ISP_00C0_T				_am(_ISP_00C0, ISP_REG_BASE, (0x00C0<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM57_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM57_HEP		: 6;	_rege_	_ISP_00C1;

#define ISP_AWBL_CM57_HSP		_bm(_ISP_00C1, ISP_REG_BASE, (0x00C1<<2), AWBL_CM57_HSP		)	//	
#define ISP_AWBL_CM57_HEP		_bm(_ISP_00C1, ISP_REG_BASE, (0x00C1<<2), AWBL_CM57_HEP		)	//	
#define ISP_00C1_T				_am(_ISP_00C1, ISP_REG_BASE, (0x00C1<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM58_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM58_HEP		: 6;	_rege_	_ISP_00C2;

#define ISP_AWBL_CM58_HSP		_bm(_ISP_00C2, ISP_REG_BASE, (0x00C2<<2), AWBL_CM58_HSP		)	//	
#define ISP_AWBL_CM58_HEP		_bm(_ISP_00C2, ISP_REG_BASE, (0x00C2<<2), AWBL_CM58_HEP		)	//	
#define ISP_00C2_T				_am(_ISP_00C2, ISP_REG_BASE, (0x00C2<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM59_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM59_HEP		: 6;	_rege_	_ISP_00C3;

#define ISP_AWBL_CM59_HSP		_bm(_ISP_00C3, ISP_REG_BASE, (0x00C3<<2), AWBL_CM59_HSP		)	//	
#define ISP_AWBL_CM59_HEP		_bm(_ISP_00C3, ISP_REG_BASE, (0x00C3<<2), AWBL_CM59_HEP		)	//	
#define ISP_00C3_T				_am(_ISP_00C3, ISP_REG_BASE, (0x00C3<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM60_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM60_HEP		: 6;	_rege_	_ISP_00C4;

#define ISP_AWBL_CM60_HSP		_bm(_ISP_00C4, ISP_REG_BASE, (0x00C4<<2), AWBL_CM60_HSP		)	//	
#define ISP_AWBL_CM60_HEP		_bm(_ISP_00C4, ISP_REG_BASE, (0x00C4<<2), AWBL_CM60_HEP		)	//	
#define ISP_00C4_T				_am(_ISP_00C4, ISP_REG_BASE, (0x00C4<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM61_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM61_HEP		: 6;	_rege_	_ISP_00C5;

#define ISP_AWBL_CM61_HSP		_bm(_ISP_00C5, ISP_REG_BASE, (0x00C5<<2), AWBL_CM61_HSP		)	//	
#define ISP_AWBL_CM61_HEP		_bm(_ISP_00C5, ISP_REG_BASE, (0x00C5<<2), AWBL_CM61_HEP		)	//	
#define ISP_00C5_T				_am(_ISP_00C5, ISP_REG_BASE, (0x00C5<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM62_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM62_HEP		: 6;	_rege_	_ISP_00C6;

#define ISP_AWBL_CM62_HSP		_bm(_ISP_00C6, ISP_REG_BASE, (0x00C6<<2), AWBL_CM62_HSP		)	//	
#define ISP_AWBL_CM62_HEP		_bm(_ISP_00C6, ISP_REG_BASE, (0x00C6<<2), AWBL_CM62_HEP		)	//	
#define ISP_00C6_T				_am(_ISP_00C6, ISP_REG_BASE, (0x00C6<<2))						//	

_regs_	UINT _res0				:18;
		UINT AWBL_CM63_HSP		: 6;
		UINT _res1				: 2;
		UINT AWBL_CM63_HEP		: 6;	_rege_	_ISP_00C7;

#define ISP_AWBL_CM63_HSP		_bm(_ISP_00C7, ISP_REG_BASE, (0x00C7<<2), AWBL_CM63_HSP		)	//	
#define ISP_AWBL_CM63_HEP		_bm(_ISP_00C7, ISP_REG_BASE, (0x00C7<<2), AWBL_CM63_HEP		)	//	
#define ISP_00C7_T				_am(_ISP_00C7, ISP_REG_BASE, (0x00C7<<2))						//	

_regs_	UINT _res0				: 5;
		UINT PRR_GAIN			:11;
		UINT _res1				: 4;
		UINT PRG_GAIN			:12;	_rege_	_ISP_00C8;

#define ISP_PRR_GAIN			_bm(_ISP_00C8, ISP_REG_BASE, (0x00C8<<2), PRR_GAIN			)	//	
#define ISP_PRG_GAIN			_bm(_ISP_00C8, ISP_REG_BASE, (0x00C8<<2), PRG_GAIN			)	//	
#define ISP_00C8_T				_am(_ISP_00C8, ISP_REG_BASE, (0x00C8<<2))						//	

_regs_	UINT _res0				: 4;
		UINT PRB_GAIN			:12;
		UINT _res1				: 4;
		UINT PGR_GAIN			:12;	_rege_	_ISP_00C9;

#define ISP_PRB_GAIN			_bm(_ISP_00C9, ISP_REG_BASE, (0x00C9<<2), PRB_GAIN			)	//	
#define ISP_PGR_GAIN			_bm(_ISP_00C9, ISP_REG_BASE, (0x00C9<<2), PGR_GAIN			)	//	
#define ISP_00C9_T				_am(_ISP_00C9, ISP_REG_BASE, (0x00C9<<2))						//	

_regs_	UINT _res0				: 5;
		UINT PGG_GAIN			:11;
		UINT _res1				: 4;
		UINT PGB_GAIN			:12;	_rege_	_ISP_00CA;

#define ISP_PGG_GAIN			_bm(_ISP_00CA, ISP_REG_BASE, (0x00CA<<2), PGG_GAIN			)	//	
#define ISP_PGB_GAIN			_bm(_ISP_00CA, ISP_REG_BASE, (0x00CA<<2), PGB_GAIN			)	//	
#define ISP_00CA_T				_am(_ISP_00CA, ISP_REG_BASE, (0x00CA<<2))						//	

_regs_	UINT _res0				: 4;
		UINT PBR_GAIN			:12;
		UINT _res1				: 4;
		UINT PBG_GAIN			:12;	_rege_	_ISP_00CB;

#define ISP_PBR_GAIN			_bm(_ISP_00CB, ISP_REG_BASE, (0x00CB<<2), PBR_GAIN			)	//	
#define ISP_PBG_GAIN			_bm(_ISP_00CB, ISP_REG_BASE, (0x00CB<<2), PBG_GAIN			)	//	
#define ISP_00CB_T				_am(_ISP_00CB, ISP_REG_BASE, (0x00CB<<2))						//	

_regs_	UINT _res0				:21;
		UINT PBB_GAIN			:11;	_rege_	_ISP_00CD;

#define ISP_PBB_GAIN			_bm(_ISP_00CD, ISP_REG_BASE, (0x00CD<<2), PBB_GAIN			)	//	
#define ISP_00CD_T				_am(_ISP_00CD, ISP_REG_BASE, (0x00CD<<2))						//	

_regs_	UINT _res0				: 8;
		UINT PYR_GAIN			: 8;
		UINT PYG_GAIN			: 8;
		UINT PYB_GAIN			: 8;	_rege_	_ISP_00CE;

#define ISP_PYR_GAIN			_bm(_ISP_00CE, ISP_REG_BASE, (0x00CE<<2), PYR_GAIN			)	//	
#define ISP_PYG_GAIN			_bm(_ISP_00CE, ISP_REG_BASE, (0x00CE<<2), PYG_GAIN			)	//	
#define ISP_PYB_GAIN			_bm(_ISP_00CE, ISP_REG_BASE, (0x00CE<<2), PYB_GAIN			)	//	
#define ISP_00CE_T				_am(_ISP_00CE, ISP_REG_BASE, (0x00CE<<2))						//	

_regs_	UINT _res0				: 6;
		UINT ODM_M1				:10;
		UINT _res1				: 6;
		UINT ODM_M2				:10;	_rege_	_ISP_00D0;

#define ISP_ODM_M1				_bm(_ISP_00D0, ISP_REG_BASE, (0x00D0<<2), ODM_M1			)	//	
#define ISP_ODM_M2				_bm(_ISP_00D0, ISP_REG_BASE, (0x00D0<<2), ODM_M2			)	//	
#define ISP_00D0_T				_am(_ISP_00D0, ISP_REG_BASE, (0x00D0<<2))						//	

_regs_	UINT _res0				: 6;
		UINT ODM_M3				:10;
		UINT _res1				: 6;
		UINT ODM_M4				:10;	_rege_	_ISP_00D1;

#define ISP_ODM_M3				_bm(_ISP_00D1, ISP_REG_BASE, (0x00D1<<2), ODM_M3			)	//	
#define ISP_ODM_M4				_bm(_ISP_00D1, ISP_REG_BASE, (0x00D1<<2), ODM_M4			)	//	
#define ISP_00D1_T				_am(_ISP_00D1, ISP_REG_BASE, (0x00D1<<2))						//	

_regs_	UINT _res0				: 6;
		UINT ODM_M5				:10;
		UINT _res1				: 6;
		UINT ODM_M6				:10;	_rege_	_ISP_00D2;

#define ISP_ODM_M5				_bm(_ISP_00D2, ISP_REG_BASE, (0x00D2<<2), ODM_M5			)	//	
#define ISP_ODM_M6				_bm(_ISP_00D2, ISP_REG_BASE, (0x00D2<<2), ODM_M6			)	//	
#define ISP_00D2_T				_am(_ISP_00D2, ISP_REG_BASE, (0x00D2<<2))						//	

_regs_	UINT _res0				: 6;
		UINT ODM_M7				:10;
		UINT _res1				: 6;
		UINT ODM_M8				:10;	_rege_	_ISP_00D3;

#define ISP_ODM_M7				_bm(_ISP_00D3, ISP_REG_BASE, (0x00D3<<2), ODM_M7			)	//	
#define ISP_ODM_M8				_bm(_ISP_00D3, ISP_REG_BASE, (0x00D3<<2), ODM_M8			)	//	
#define ISP_00D3_T				_am(_ISP_00D3, ISP_REG_BASE, (0x00D3<<2))						//	

_regs_	UINT _res0				:22;
		UINT ODM_M9				:10;	_rege_	_ISP_00D4;

#define ISP_ODM_M9				_bm(_ISP_00D4, ISP_REG_BASE, (0x00D4<<2), ODM_M9			)	//	
#define ISP_00D4_T				_am(_ISP_00D4, ISP_REG_BASE, (0x00D4<<2))						//	

_regs_	UINT IRIS_ON			: 1;
		UINT IRIS_INV			: 1;
		UINT IRIS_MOD			: 2;
		UINT IRIS_SW			: 1;
		UINT IRIS_WSEL			: 3;
		UINT IRIS_FBK			: 8;
		UINT _res0				: 6;
		UINT IRIS_TLV			:10;	_rege_	_ISP_00D5;

#define ISP_IRIS_ON				_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_ON			)	//	
#define ISP_IRIS_INV			_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_INV			)	//	
#define ISP_IRIS_MOD			_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_MOD			)	//	
#define ISP_IRIS_SW				_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_SW			)	//	
#define ISP_IRIS_WSEL			_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_WSEL			)	//	
#define ISP_IRIS_FBK			_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_FBK			)	//	
#define ISP_IRIS_TLV			_bm(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2), IRIS_TLV			)	//	
#define ISP_00D5_T				_am(_ISP_00D5, ISP_REG_BASE, (0x00D5<<2))						//	

_regs_	UINT _res0				: 6;
		UINT IRIS_TGA			:10;
		UINT IRIS_OFS			:16;	_rege_	_ISP_00D6;

#define ISP_IRIS_TGA			_bm(_ISP_00D6, ISP_REG_BASE, (0x00D6<<2), IRIS_TGA			)	//	
#define ISP_IRIS_OFS			_bm(_ISP_00D6, ISP_REG_BASE, (0x00D6<<2), IRIS_OFS			)	//	
#define ISP_00D6_T				_am(_ISP_00D6, ISP_REG_BASE, (0x00D6<<2))						//	

_regs_	UINT IRIS_CLPL			:16;
		UINT IRIS_CLPH			:16;	_rege_	_ISP_00D7;

#define ISP_IRIS_CLPL			_bm(_ISP_00D7, ISP_REG_BASE, (0x00D7<<2), IRIS_CLPL			)	//	
#define ISP_IRIS_CLPH			_bm(_ISP_00D7, ISP_REG_BASE, (0x00D7<<2), IRIS_CLPH			)	//	
#define ISP_00D7_T				_am(_ISP_00D7, ISP_REG_BASE, (0x00D7<<2))						//	

_regs_	UINT IRIS_VSP			:16;
		UINT IRIS_VEP			:16;	_rege_	_ISP_00D8;

#define ISP_IRIS_VSP			_bm(_ISP_00D8, ISP_REG_BASE, (0x00D8<<2), IRIS_VSP			)	//	
#define ISP_IRIS_VEP			_bm(_ISP_00D8, ISP_REG_BASE, (0x00D8<<2), IRIS_VEP			)	//	
#define ISP_00D8_T				_am(_ISP_00D8, ISP_REG_BASE, (0x00D8<<2))						//	

_regs_	UINT IRIS_HSP			:16;
		UINT IRIS_HEP			:16;	_rege_	_ISP_00D9;

#define ISP_IRIS_HSP			_bm(_ISP_00D9, ISP_REG_BASE, (0x00D9<<2), IRIS_HSP			)	//	
#define ISP_IRIS_HEP			_bm(_ISP_00D9, ISP_REG_BASE, (0x00D9<<2), IRIS_HEP			)	//	
#define ISP_00D9_T				_am(_ISP_00D9, ISP_REG_BASE, (0x00D9<<2))						//	

_regs_	UINT _res0				: 8;
		UINT INTP_DIR_TH		: 8;
		UINT _res1				: 2;
		UINT HSUB_SEL			: 2;
		UINT _res2				: 2;
		UINT VSUB_SEL			: 2;
		UINT _res3				: 2;
		UINT HVSUB_SEL			: 2;
		UINT _res4				: 2;
		UINT DSUB_SEL			: 2;	_rege_	_ISP_00DA;

#define ISP_INTP_DIR_TH			_bm(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2), INTP_DIR_TH		)	//	
#define ISP_HSUB_SEL			_bm(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2), HSUB_SEL			)	//	
#define ISP_VSUB_SEL			_bm(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2), VSUB_SEL			)	//	
#define ISP_HVSUB_SEL			_bm(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2), HVSUB_SEL			)	//	
#define ISP_DSUB_SEL			_bm(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2), DSUB_SEL			)	//	
#define ISP_00DA_T				_am(_ISP_00DA, ISP_REG_BASE, (0x00DA<<2))						//	

_regs_	UINT _res0				: 8;
		UINT POST_YBGAIN		: 8;
		UINT POST_YGGAIN		: 8;
		UINT POST_YRGAIN		: 8;	_rege_	_ISP_00DB;

#define ISP_POST_YBGAIN			_bm(_ISP_00DB, ISP_REG_BASE, (0x00DB<<2), POST_YBGAIN		)	//	
#define ISP_POST_YGGAIN			_bm(_ISP_00DB, ISP_REG_BASE, (0x00DB<<2), POST_YGGAIN		)	//	
#define ISP_POST_YRGAIN			_bm(_ISP_00DB, ISP_REG_BASE, (0x00DB<<2), POST_YRGAIN		)	//	
#define ISP_00DB_T				_am(_ISP_00DB, ISP_REG_BASE, (0x00DB<<2))						//	

_regs_	UINT _res0				: 8;
		UINT POST_CRGAIN		: 8;
		UINT POST_CGGAIN		: 8;
		UINT POST_CBGAIN		: 8;	_rege_	_ISP_00DC;

#define ISP_POST_CRGAIN			_bm(_ISP_00DC, ISP_REG_BASE, (0x00DC<<2), POST_CRGAIN		)	//	
#define ISP_POST_CGGAIN			_bm(_ISP_00DC, ISP_REG_BASE, (0x00DC<<2), POST_CGGAIN		)	//	
#define ISP_POST_CBGAIN			_bm(_ISP_00DC, ISP_REG_BASE, (0x00DC<<2), POST_CBGAIN		)	//	
#define ISP_00DC_T				_am(_ISP_00DC, ISP_REG_BASE, (0x00DC<<2))						//	

_regs_	UINT _res0				: 3;
		UINT WPRG_GAIN			:13;
		UINT _res1				: 3;
		UINT WPRR_GAIN			:13;	_rege_	_ISP_00E0;

#define ISP_WPRG_GAIN			_bm(_ISP_00E0, ISP_REG_BASE, (0x00E0<<2), WPRG_GAIN			)	//	
#define ISP_WPRR_GAIN			_bm(_ISP_00E0, ISP_REG_BASE, (0x00E0<<2), WPRR_GAIN			)	//	
#define ISP_00E0_T				_am(_ISP_00E0, ISP_REG_BASE, (0x00E0<<2))						//	

_regs_	UINT _res0				: 3;
		UINT WPGR_GAIN			:13;
		UINT _res1				: 3;
		UINT WPRB_GAIN			:13;	_rege_	_ISP_00E1;

#define ISP_WPGR_GAIN			_bm(_ISP_00E1, ISP_REG_BASE, (0x00E1<<2), WPGR_GAIN			)	//	
#define ISP_WPRB_GAIN			_bm(_ISP_00E1, ISP_REG_BASE, (0x00E1<<2), WPRB_GAIN			)	//	
#define ISP_00E1_T				_am(_ISP_00E1, ISP_REG_BASE, (0x00E1<<2))						//	

_regs_	UINT _res0				: 3;
		UINT WPGB_GAIN			:13;
		UINT _res1				: 3;
		UINT WPGG_GAIN			:13;	_rege_	_ISP_00E2;

#define ISP_WPGB_GAIN			_bm(_ISP_00E2, ISP_REG_BASE, (0x00E2<<2), WPGB_GAIN			)	//	
#define ISP_WPGG_GAIN			_bm(_ISP_00E2, ISP_REG_BASE, (0x00E2<<2), WPGG_GAIN			)	//	
#define ISP_00E2_T				_am(_ISP_00E2, ISP_REG_BASE, (0x00E2<<2))						//	

_regs_	UINT _res0				: 3;
		UINT WPBG_GAIN			:13;
		UINT _res1				: 3;
		UINT WPBR_GAIN			:13;	_rege_	_ISP_00E3;

#define ISP_WPBG_GAIN			_bm(_ISP_00E3, ISP_REG_BASE, (0x00E3<<2), WPBG_GAIN			)	//	
#define ISP_WPBR_GAIN			_bm(_ISP_00E3, ISP_REG_BASE, (0x00E3<<2), WPBR_GAIN			)	//	
#define ISP_00E3_T				_am(_ISP_00E3, ISP_REG_BASE, (0x00E3<<2))						//	

_regs_	UINT _res0				:19;
		UINT WPBB_GAIN			:13;	_rege_	_ISP_00E4;

#define ISP_WPBB_GAIN			_bm(_ISP_00E4, ISP_REG_BASE, (0x00E4<<2), WPBB_GAIN			)	//	
#define ISP_00E4_T				_am(_ISP_00E4, ISP_REG_BASE, (0x00E4<<2))						//	

_regs_	UINT _res0				: 6;
		UINT RMYG				:10;
		UINT _res1				: 6;
		UINT RMYR				:10;	_rege_	_ISP_00E5;

#define ISP_RMYG				_bm(_ISP_00E5, ISP_REG_BASE, (0x00E5<<2), RMYG				)	//	
#define ISP_RMYR				_bm(_ISP_00E5, ISP_REG_BASE, (0x00E5<<2), RMYR				)	//	
#define ISP_00E5_T				_am(_ISP_00E5, ISP_REG_BASE, (0x00E5<<2))						//	

_regs_	UINT _res0				: 6;
		UINT BMYR				:10;
		UINT _res1				: 6;
		UINT RMYB				:10;	_rege_	_ISP_00E6;

#define ISP_BMYR				_bm(_ISP_00E6, ISP_REG_BASE, (0x00E6<<2), BMYR				)	//	
#define ISP_RMYB				_bm(_ISP_00E6, ISP_REG_BASE, (0x00E6<<2), RMYB				)	//	
#define ISP_00E6_T				_am(_ISP_00E6, ISP_REG_BASE, (0x00E6<<2))						//	

_regs_	UINT _res0				: 6;
		UINT BMYB				:10;
		UINT _res1				: 6;
		UINT BMYG				:10;	_rege_	_ISP_00E7;

#define ISP_BMYB				_bm(_ISP_00E7, ISP_REG_BASE, (0x00E7<<2), BMYB				)	//	
#define ISP_BMYG				_bm(_ISP_00E7, ISP_REG_BASE, (0x00E7<<2), BMYG				)	//	
#define ISP_00E7_T				_am(_ISP_00E7, ISP_REG_BASE, (0x00E7<<2))						//	

_regs_	UINT BYGAINN			: 8;
		UINT BYGAINP			: 8;
		UINT RYGAINN			: 8;
		UINT RYGAINP			: 8;	_rege_	_ISP_00E8;

#define ISP_BYGAINN				_bm(_ISP_00E8, ISP_REG_BASE, (0x00E8<<2), BYGAINN			)	//	
#define ISP_BYGAINP				_bm(_ISP_00E8, ISP_REG_BASE, (0x00E8<<2), BYGAINP			)	//	
#define ISP_RYGAINN				_bm(_ISP_00E8, ISP_REG_BASE, (0x00E8<<2), RYGAINN			)	//	
#define ISP_RYGAINP				_bm(_ISP_00E8, ISP_REG_BASE, (0x00E8<<2), RYGAINP			)	//	
#define ISP_00E8_T				_am(_ISP_00E8, ISP_REG_BASE, (0x00E8<<2))						//	

_regs_	UINT BYHUEN				: 8;
		UINT BYHUEP				: 8;
		UINT RYHUEN				: 8;
		UINT RYHUEP				: 8;	_rege_	_ISP_00E9;

#define ISP_BYHUEN				_bm(_ISP_00E9, ISP_REG_BASE, (0x00E9<<2), BYHUEN			)	//	
#define ISP_BYHUEP				_bm(_ISP_00E9, ISP_REG_BASE, (0x00E9<<2), BYHUEP			)	//	
#define ISP_RYHUEN				_bm(_ISP_00E9, ISP_REG_BASE, (0x00E9<<2), RYHUEN			)	//	
#define ISP_RYHUEP				_bm(_ISP_00E9, ISP_REG_BASE, (0x00E9<<2), RYHUEP			)	//	
#define ISP_00E9_T				_am(_ISP_00E9, ISP_REG_BASE, (0x00E9<<2))						//	

_regs_	UINT _res0				: 5;
		UINT PR_OFS				:11;
		UINT _res1				: 5;
		UINT PG_OFS				:11;	_rege_	_ISP_00EA;

#define ISP_PR_OFS				_bm(_ISP_00EA, ISP_REG_BASE, (0x00EA<<2), PR_OFS			)	//	
#define ISP_PG_OFS				_bm(_ISP_00EA, ISP_REG_BASE, (0x00EA<<2), PG_OFS			)	//	
#define ISP_00EA_T				_am(_ISP_00EA, ISP_REG_BASE, (0x00EA<<2))						//	

_regs_	UINT _res0				:21;
		UINT PB_OFS				:11;	_rege_	_ISP_00EB;

#define ISP_PB_OFS				_bm(_ISP_00EB, ISP_REG_BASE, (0x00EB<<2), PB_OFS			)	//	
#define ISP_00EB_T				_am(_ISP_00EB, ISP_REG_BASE, (0x00EB<<2))						//	

_regs_	UINT NEGA_ON			: 1;
		UINT BW_ON				: 1;
		UINT _res0				:28;
		UINT RBFLG_INV2			: 1;
		UINT RBFLG_INV			: 1;	_rege_	_ISP_00EC;

#define ISP_NEGA_ON				_bm(_ISP_00EC, ISP_REG_BASE, (0x00EC<<2), NEGA_ON			)	//	
#define ISP_BW_ON				_bm(_ISP_00EC, ISP_REG_BASE, (0x00EC<<2), BW_ON				)	//	
#define ISP_RBFLG_INV2			_bm(_ISP_00EC, ISP_REG_BASE, (0x00EC<<2), RBFLG_INV2		)	//	
#define ISP_RBFLG_INV			_bm(_ISP_00EC, ISP_REG_BASE, (0x00EC<<2), RBFLG_INV			)	//	
#define ISP_00EC_T				_am(_ISP_00EC, ISP_REG_BASE, (0x00EC<<2))						//	

#if 0
_regs_	UINT HLMASK_ON			: 1;
		UINT CSUP_ON			: 1;
		UINT CSUP_GA			: 6;
		UINT CSUP_TH			: 8;
		UINT _res0				: 4;
		UINT CES_DLY			: 4;
		UINT CVLPF_SEL			: 1;
		UINT CVLPF_ON			: 1;
		UINT CHLPF_SEL			: 1;
		UINT CHLPF_ON			: 1;
		UINT _res1				: 1;
		UINT HSUP_GA			: 2;
		UINT HSUP_ON			: 1;	_rege_	_ISP_00ED;

#define ISP_HLMASK_ON			_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), HLMASK_ON			)	//	
#define ISP_CSUP_ON				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CSUP_ON			)	//	
#define ISP_CSUP_GA				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CSUP_GA			)	//	
#define ISP_CSUP_TH				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CSUP_TH			)	//	
#define ISP_CES_DLY				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CES_DLY			)	//	
#define ISP_CVLPF_SEL			_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CVLPF_SEL			)	//	
#define ISP_CVLPF_ON			_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CVLPF_ON			)	//	
#define ISP_CHLPF_SEL			_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CHLPF_SEL			)	//	
#define ISP_CHLPF_ON			_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), CHLPF_ON			)	//	
#define ISP_HSUP_GA				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), HSUP_GA			)	//	
#define ISP_HSUP_ON				_bm(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2), HSUP_ON			)	//	
#define ISP_00ED_T				_am(_ISP_00ED, ISP_REG_BASE, (0x00ED<<2))						//	
#endif

_regs_	UINT HLMASK_BLV			: 8;
		UINT HLMASK_RLV			: 8;
		UINT HLMASK_YLV			: 8;
		UINT HLMASK_TH			: 8;	_rege_	_ISP_00EE;

#define ISP_HLMASK_BLV			_bm(_ISP_00EE, ISP_REG_BASE, (0x00EE<<2), HLMASK_BLV		)	//	
#define ISP_HLMASK_RLV			_bm(_ISP_00EE, ISP_REG_BASE, (0x00EE<<2), HLMASK_RLV		)	//	
#define ISP_HLMASK_YLV			_bm(_ISP_00EE, ISP_REG_BASE, (0x00EE<<2), HLMASK_YLV		)	//	
#define ISP_HLMASK_TH			_bm(_ISP_00EE, ISP_REG_BASE, (0x00EE<<2), HLMASK_TH			)	//	
#define ISP_00EE_T				_am(_ISP_00EE, ISP_REG_BASE, (0x00EE<<2))						//	

_regs_	UINT LSUP_ON			: 1;
		UINT _res0				: 1;
		UINT LSUP_GA			: 6;
		UINT LSUP_TH			: 8;
		UINT CES_ON				: 1;
		UINT _res1				: 1;
		UINT CES_GAIN			: 6;
		UINT CES_LCLIP			: 8;	_rege_	_ISP_00EF;

#define ISP_LSUP_ON				_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), LSUP_ON			)	//	
#define ISP_LSUP_GA				_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), LSUP_GA			)	//	
#define ISP_LSUP_TH				_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), LSUP_TH			)	//	
#define ISP_CES_ON				_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), CES_ON			)	//	
#define ISP_CES_GAIN			_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), CES_GAIN			)	//	
#define ISP_CES_LCLIP			_bm(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2), CES_LCLIP			)	//	
#define ISP_00EF_T				_am(_ISP_00EF, ISP_REG_BASE, (0x00EF<<2))						//	

_regs_	UINT DCBAR_ON			: 1;
		UINT _res0				: 3;
		UINT DCBAR_ST			:12;
		UINT _res1				: 6;
		UINT DCBAR_HW			:10;	_rege_	_ISP_00F0;

#define ISP_DCBAR_ON			_bm(_ISP_00F0, ISP_REG_BASE, (0x00F0<<2), DCBAR_ON			)	//	
#define ISP_DCBAR_ST			_bm(_ISP_00F0, ISP_REG_BASE, (0x00F0<<2), DCBAR_ST			)	//	
#define ISP_DCBAR_HW			_bm(_ISP_00F0, ISP_REG_BASE, (0x00F0<<2), DCBAR_HW			)	//	
#define ISP_00F0_T				_am(_ISP_00F0, ISP_REG_BASE, (0x00F0<<2))						//	

_regs_	UINT ACE_BP				: 1;
		UINT ACE_ON				: 1;
		UINT ACE_TEST			: 1;
		UINT ACE_OSD			: 1;
		UINT ACE_HSP			:12;
		UINT ACE_DTH			: 4;
		UINT ACE_VSP			:12;	_rege_	_ISP_00F1;

#define ISP_ACE_BP				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_BP			)	//	
#define ISP_ACE_ON				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_ON			)	//	
#define ISP_ACE_TEST			_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_TEST			)	//	
#define ISP_ACE_OSD				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_OSD			)	//	
#define ISP_ACE_HSP				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_HSP			)	//	
#define ISP_ACE_DTH				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_DTH			)	//	
#define ISP_ACE_VSP				_bm(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2), ACE_VSP			)	//	
#define ISP_00F1_T				_am(_ISP_00F1, ISP_REG_BASE, (0x00F1<<2))						//	

_regs_	UINT ACE_LPF			: 1;
		UINT _res0				: 7;
		UINT ACE_TH1			: 8;
		UINT ACE_TH2			: 8;
		UINT ACE_HBS			: 2;
		UINT ACE_VBS			: 2;
		UINT _res1				: 4;	_rege_	_ISP_00F2;

#define ISP_ACE_LPF				_bm(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2), ACE_LPF			)	//	
#define ISP_ACE_TH1				_bm(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2), ACE_TH1			)	//	
#define ISP_ACE_TH2				_bm(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2), ACE_TH2			)	//	
#define ISP_ACE_HBS				_bm(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2), ACE_HBS			)	//	
#define ISP_ACE_VBS				_bm(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2), ACE_VBS			)	//	
#define ISP_00F2_T				_am(_ISP_00F2, ISP_REG_BASE, (0x00F2<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ACE_HB				: 4;
		UINT ACE_VB				: 4;
		UINT _res1				: 2;
		UINT ACE_IIR			: 6;
		UINT ACE_GMGN			: 8;	_rege_	_ISP_00F3;

#define ISP_ACE_HB				_bm(_ISP_00F3, ISP_REG_BASE, (0x00F3<<2), ACE_HB			)	//	
#define ISP_ACE_VB				_bm(_ISP_00F3, ISP_REG_BASE, (0x00F3<<2), ACE_VB			)	//	
#define ISP_ACE_IIR				_bm(_ISP_00F3, ISP_REG_BASE, (0x00F3<<2), ACE_IIR			)	//	
#define ISP_ACE_GMGN			_bm(_ISP_00F3, ISP_REG_BASE, (0x00F3<<2), ACE_GMGN			)	//	
#define ISP_00F3_T				_am(_ISP_00F3, ISP_REG_BASE, (0x00F3<<2))						//	

_regs_	UINT ACE_GM00			: 8;
		UINT ACE_GM01			: 8;
		UINT ACE_GM02			: 8;
		UINT ACE_GM03			: 8;	_rege_	_ISP_00F4;

#define ISP_ACE_GM00			_bm(_ISP_00F4, ISP_REG_BASE, (0x00F4<<2), ACE_GM00			)	//	
#define ISP_ACE_GM01			_bm(_ISP_00F4, ISP_REG_BASE, (0x00F4<<2), ACE_GM01			)	//	
#define ISP_ACE_GM02			_bm(_ISP_00F4, ISP_REG_BASE, (0x00F4<<2), ACE_GM02			)	//	
#define ISP_ACE_GM03			_bm(_ISP_00F4, ISP_REG_BASE, (0x00F4<<2), ACE_GM03			)	//	
#define ISP_00F4_T				_am(_ISP_00F4, ISP_REG_BASE, (0x00F4<<2))						//	

_regs_	UINT ACE_GM04			: 8;
		UINT ACE_GM05			: 8;
		UINT ACE_GM06			: 8;
		UINT ACE_GM07			: 8;	_rege_	_ISP_00F5;

#define ISP_ACE_GM04			_bm(_ISP_00F5, ISP_REG_BASE, (0x00F5<<2), ACE_GM04			)	//	
#define ISP_ACE_GM05			_bm(_ISP_00F5, ISP_REG_BASE, (0x00F5<<2), ACE_GM05			)	//	
#define ISP_ACE_GM06			_bm(_ISP_00F5, ISP_REG_BASE, (0x00F5<<2), ACE_GM06			)	//	
#define ISP_ACE_GM07			_bm(_ISP_00F5, ISP_REG_BASE, (0x00F5<<2), ACE_GM07			)	//	
#define ISP_00F5_T				_am(_ISP_00F5, ISP_REG_BASE, (0x00F5<<2))						//	

_regs_	UINT ACE_GM08			: 8;
		UINT ACE_GM09			: 8;
		UINT ACE_GM10			: 8;
		UINT ACE_GM11			: 8;	_rege_	_ISP_00F6;

#define ISP_ACE_GM08			_bm(_ISP_00F6, ISP_REG_BASE, (0x00F6<<2), ACE_GM08			)	//	
#define ISP_ACE_GM09			_bm(_ISP_00F6, ISP_REG_BASE, (0x00F6<<2), ACE_GM09			)	//	
#define ISP_ACE_GM10			_bm(_ISP_00F6, ISP_REG_BASE, (0x00F6<<2), ACE_GM10			)	//	
#define ISP_ACE_GM11			_bm(_ISP_00F6, ISP_REG_BASE, (0x00F6<<2), ACE_GM11			)	//	
#define ISP_00F6_T				_am(_ISP_00F6, ISP_REG_BASE, (0x00F6<<2))						//	

_regs_	UINT ACE_GM12			: 8;
		UINT ACE_GM13			: 8;
		UINT ACE_GM14			: 8;
		UINT ACE_GM15			: 8;	_rege_	_ISP_00F7;

#define ISP_ACE_GM12			_bm(_ISP_00F7, ISP_REG_BASE, (0x00F7<<2), ACE_GM12			)	//	
#define ISP_ACE_GM13			_bm(_ISP_00F7, ISP_REG_BASE, (0x00F7<<2), ACE_GM13			)	//	
#define ISP_ACE_GM14			_bm(_ISP_00F7, ISP_REG_BASE, (0x00F7<<2), ACE_GM14			)	//	
#define ISP_ACE_GM15			_bm(_ISP_00F7, ISP_REG_BASE, (0x00F7<<2), ACE_GM15			)	//	
#define ISP_00F7_T				_am(_ISP_00F7, ISP_REG_BASE, (0x00F7<<2))						//	

_regs_	UINT HEQ_ON				: 1;
		UINT HEQ_OSD			: 1;
		UINT _res0				: 5;
		UINT HEQ_WG				: 9;
		UINT _res1				: 1;
		UINT HEQ_TH				:15;	_rege_	_ISP_00F8;

#define ISP_HEQ_ON				_bm(_ISP_00F8, ISP_REG_BASE, (0x00F8<<2), HEQ_ON			)	//	
#define ISP_HEQ_OSD				_bm(_ISP_00F8, ISP_REG_BASE, (0x00F8<<2), HEQ_OSD			)	//	
#define ISP_HEQ_WG				_bm(_ISP_00F8, ISP_REG_BASE, (0x00F8<<2), HEQ_WG			)	//	
#define ISP_HEQ_TH				_bm(_ISP_00F8, ISP_REG_BASE, (0x00F8<<2), HEQ_TH			)	//	
#define ISP_00F8_T				_am(_ISP_00F8, ISP_REG_BASE, (0x00F8<<2))						//	

_regs_	UINT HEQ_HW				: 4;
		UINT HEQ_HSP			:12;
		UINT HEQ_VW				: 4;
		UINT HEQ_VSP			:12;	_rege_	_ISP_00F9;

#define ISP_HEQ_HW				_bm(_ISP_00F9, ISP_REG_BASE, (0x00F9<<2), HEQ_HW			)	//	
#define ISP_HEQ_HSP				_bm(_ISP_00F9, ISP_REG_BASE, (0x00F9<<2), HEQ_HSP			)	//	
#define ISP_HEQ_VW				_bm(_ISP_00F9, ISP_REG_BASE, (0x00F9<<2), HEQ_VW			)	//	
#define ISP_HEQ_VSP				_bm(_ISP_00F9, ISP_REG_BASE, (0x00F9<<2), HEQ_VSP			)	//	
#define ISP_00F9_T				_am(_ISP_00F9, ISP_REG_BASE, (0x00F9<<2))						//	

_regs_	UINT _res0				:24;
		UINT HEQ_IIRK			: 8;	_rege_	_ISP_00FA;

#define ISP_HEQ_IIRK			_bm(_ISP_00FA, ISP_REG_BASE, (0x00FA<<2), HEQ_IIRK			)	//	
#define ISP_00FA_T				_am(_ISP_00FA, ISP_REG_BASE, (0x00FA<<2))						//	

_regs_	UINT APT_ON				: 1;
		UINT APT_GAIN_ON		: 1;
		UINT APT_TEST			: 2;
		UINT _res0				:18;
		UINT APT_TH3			:10;	_rege_	_ISP_0100;

#define ISP_APT_ON				_bm(_ISP_0100, ISP_REG_BASE, (0x0100<<2), APT_ON			)	//	
#define ISP_APT_GAIN_ON			_bm(_ISP_0100, ISP_REG_BASE, (0x0100<<2), APT_GAIN_ON		)	//	
#define ISP_APT_TEST			_bm(_ISP_0100, ISP_REG_BASE, (0x0100<<2), APT_TEST			)	//	
#define ISP_APT_TH3				_bm(_ISP_0100, ISP_REG_BASE, (0x0100<<2), APT_TH3			)	//	
#define ISP_0100_T				_am(_ISP_0100, ISP_REG_BASE, (0x0100<<2))						//	

_regs_	UINT APT_TH1			:16;
		UINT APT_TH2			:16;	_rege_	_ISP_0101;

#define ISP_APT_TH1				_bm(_ISP_0101, ISP_REG_BASE, (0x0101<<2), APT_TH1			)	//	
#define ISP_APT_TH2				_bm(_ISP_0101, ISP_REG_BASE, (0x0101<<2), APT_TH2			)	//	
#define ISP_0101_T				_am(_ISP_0101, ISP_REG_BASE, (0x0101<<2))						//	

_regs_	UINT APT_GAIN2_NEG		: 8;
		UINT APT_GAIN1_NEG		: 8;
		UINT APT_GAIN2_POS		: 8;
		UINT APT_GAIN1_POS		: 8;	_rege_	_ISP_0102;

#define ISP_APT_GAIN2_NEG		_bm(_ISP_0102, ISP_REG_BASE, (0x0102<<2), APT_GAIN2_NEG		)	//	
#define ISP_APT_GAIN1_NEG		_bm(_ISP_0102, ISP_REG_BASE, (0x0102<<2), APT_GAIN1_NEG		)	//	
#define ISP_APT_GAIN2_POS		_bm(_ISP_0102, ISP_REG_BASE, (0x0102<<2), APT_GAIN2_POS		)	//	
#define ISP_APT_GAIN1_POS		_bm(_ISP_0102, ISP_REG_BASE, (0x0102<<2), APT_GAIN1_POS		)	//	
#define ISP_0102_T				_am(_ISP_0102, ISP_REG_BASE, (0x0102<<2))						//	

_regs_	UINT _res0				: 2;
		UINT APT_SLIC3			:14;
		UINT _res1				: 6;
		UINT APT_CLIP3			:10;	_rege_	_ISP_0103;

#define ISP_APT_SLIC3			_bm(_ISP_0103, ISP_REG_BASE, (0x0103<<2), APT_SLIC3			)	//	
#define ISP_APT_CLIP3			_bm(_ISP_0103, ISP_REG_BASE, (0x0103<<2), APT_CLIP3			)	//	
#define ISP_0103_T				_am(_ISP_0103, ISP_REG_BASE, (0x0103<<2))						//	

_regs_	UINT _res0				:24;
		UINT APT_GAIN3			: 8;	_rege_	_ISP_0104;

#define ISP_APT_GAIN3			_bm(_ISP_0104, ISP_REG_BASE, (0x0104<<2), APT_GAIN3			)	//	
#define ISP_0104_T				_am(_ISP_0104, ISP_REG_BASE, (0x0104<<2))						//	

_regs_	UINT APT_LPFK			: 8;
		UINT APT_LPFGA			: 8;
		UINT _res0				: 6;
		UINT APT_LPFTH			:10;	_rege_	_ISP_0105;

#define ISP_APT_LPFK			_bm(_ISP_0105, ISP_REG_BASE, (0x0105<<2), APT_LPFK			)	//	
#define ISP_APT_LPFGA			_bm(_ISP_0105, ISP_REG_BASE, (0x0105<<2), APT_LPFGA			)	//	
#define ISP_APT_LPFTH			_bm(_ISP_0105, ISP_REG_BASE, (0x0105<<2), APT_LPFTH			)	//	
#define ISP_0105_T				_am(_ISP_0105, ISP_REG_BASE, (0x0105<<2))						//	

_regs_	UINT YGMA_ON			: 1;
		UINT CGMA_ON			: 1;
		UINT _res0				: 4;
		UINT YGMY_00			:10;
		UINT _res1				: 6;
		UINT YGMY_01			:10;	_rege_	_ISP_0106;

#define ISP_YGMA_ON				_bm(_ISP_0106, ISP_REG_BASE, (0x0106<<2), YGMA_ON			)	//	
#define ISP_CGMA_ON				_bm(_ISP_0106, ISP_REG_BASE, (0x0106<<2), CGMA_ON			)	//	
#define ISP_YGMY_00				_bm(_ISP_0106, ISP_REG_BASE, (0x0106<<2), YGMY_00			)	//	
#define ISP_YGMY_01				_bm(_ISP_0106, ISP_REG_BASE, (0x0106<<2), YGMY_01			)	//	
#define ISP_0106_T				_am(_ISP_0106, ISP_REG_BASE, (0x0106<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_02			:10;
		UINT _res1				: 6;
		UINT YGMY_03			:10;	_rege_	_ISP_0107;

#define ISP_YGMY_02				_bm(_ISP_0107, ISP_REG_BASE, (0x0107<<2), YGMY_02			)	//	
#define ISP_YGMY_03				_bm(_ISP_0107, ISP_REG_BASE, (0x0107<<2), YGMY_03			)	//	
#define ISP_0107_T				_am(_ISP_0107, ISP_REG_BASE, (0x0107<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_04			:10;
		UINT _res1				: 6;
		UINT YGMY_05			:10;	_rege_	_ISP_0108;

#define ISP_YGMY_04				_bm(_ISP_0108, ISP_REG_BASE, (0x0108<<2), YGMY_04			)	//	
#define ISP_YGMY_05				_bm(_ISP_0108, ISP_REG_BASE, (0x0108<<2), YGMY_05			)	//	
#define ISP_0108_T				_am(_ISP_0108, ISP_REG_BASE, (0x0108<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_06			:10;
		UINT _res1				: 6;
		UINT YGMY_07			:10;	_rege_	_ISP_0109;

#define ISP_YGMY_06				_bm(_ISP_0109, ISP_REG_BASE, (0x0109<<2), YGMY_06			)	//	
#define ISP_YGMY_07				_bm(_ISP_0109, ISP_REG_BASE, (0x0109<<2), YGMY_07			)	//	
#define ISP_0109_T				_am(_ISP_0109, ISP_REG_BASE, (0x0109<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_08			:10;
		UINT _res1				: 6;
		UINT YGMY_09			:10;	_rege_	_ISP_010A;

#define ISP_YGMY_08				_bm(_ISP_010A, ISP_REG_BASE, (0x010A<<2), YGMY_08			)	//	
#define ISP_YGMY_09				_bm(_ISP_010A, ISP_REG_BASE, (0x010A<<2), YGMY_09			)	//	
#define ISP_010A_T				_am(_ISP_010A, ISP_REG_BASE, (0x010A<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_10			:10;
		UINT _res1				: 6;
		UINT YGMY_11			:10;	_rege_	_ISP_010B;

#define ISP_YGMY_10				_bm(_ISP_010B, ISP_REG_BASE, (0x010B<<2), YGMY_10			)	//	
#define ISP_YGMY_11				_bm(_ISP_010B, ISP_REG_BASE, (0x010B<<2), YGMY_11			)	//	
#define ISP_010B_T				_am(_ISP_010B, ISP_REG_BASE, (0x010B<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_12			:10;
		UINT _res1				: 6;
		UINT YGMY_13			:10;	_rege_	_ISP_010C;

#define ISP_YGMY_12				_bm(_ISP_010C, ISP_REG_BASE, (0x010C<<2), YGMY_12			)	//	
#define ISP_YGMY_13				_bm(_ISP_010C, ISP_REG_BASE, (0x010C<<2), YGMY_13			)	//	
#define ISP_010C_T				_am(_ISP_010C, ISP_REG_BASE, (0x010C<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_14			:10;
		UINT _res1				: 6;
		UINT YGMY_15			:10;	_rege_	_ISP_010D;

#define ISP_YGMY_14				_bm(_ISP_010D, ISP_REG_BASE, (0x010D<<2), YGMY_14			)	//	
#define ISP_YGMY_15				_bm(_ISP_010D, ISP_REG_BASE, (0x010D<<2), YGMY_15			)	//	
#define ISP_010D_T				_am(_ISP_010D, ISP_REG_BASE, (0x010D<<2))						//	

_regs_	UINT _res0				: 6;
		UINT YGMY_16			:10;
		UINT _res1				:16;	_rege_	_ISP_010E;

#define ISP_YGMY_16				_bm(_ISP_010E, ISP_REG_BASE, (0x010E<<2), YGMY_16			)	//	
#define ISP_010E_T				_am(_ISP_010E, ISP_REG_BASE, (0x010E<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_00			:10;
		UINT _res1				: 6;
		UINT CGMY_01			:10;	_rege_	_ISP_0110;

#define ISP_CGMY_00				_bm(_ISP_0110, ISP_REG_BASE, (0x0110<<2), CGMY_00			)	//	
#define ISP_CGMY_01				_bm(_ISP_0110, ISP_REG_BASE, (0x0110<<2), CGMY_01			)	//	
#define ISP_0110_T				_am(_ISP_0110, ISP_REG_BASE, (0x0110<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_02			:10;
		UINT _res1				: 6;
		UINT CGMY_03			:10;	_rege_	_ISP_0111;

#define ISP_CGMY_02				_bm(_ISP_0111, ISP_REG_BASE, (0x0111<<2), CGMY_02			)	//	
#define ISP_CGMY_03				_bm(_ISP_0111, ISP_REG_BASE, (0x0111<<2), CGMY_03			)	//	
#define ISP_0111_T				_am(_ISP_0111, ISP_REG_BASE, (0x0111<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_04			:10;
		UINT _res1				: 6;
		UINT CGMY_05			:10;	_rege_	_ISP_0112;

#define ISP_CGMY_04				_bm(_ISP_0112, ISP_REG_BASE, (0x0112<<2), CGMY_04			)	//	
#define ISP_CGMY_05				_bm(_ISP_0112, ISP_REG_BASE, (0x0112<<2), CGMY_05			)	//	
#define ISP_0112_T				_am(_ISP_0112, ISP_REG_BASE, (0x0112<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_06			:10;
		UINT _res1				: 6;
		UINT CGMY_07			:10;	_rege_	_ISP_0113;

#define ISP_CGMY_06				_bm(_ISP_0113, ISP_REG_BASE, (0x0113<<2), CGMY_06			)	//	
#define ISP_CGMY_07				_bm(_ISP_0113, ISP_REG_BASE, (0x0113<<2), CGMY_07			)	//	
#define ISP_0113_T				_am(_ISP_0113, ISP_REG_BASE, (0x0113<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_08			:10;
		UINT _res1				: 6;
		UINT CGMY_09			:10;	_rege_	_ISP_0114;

#define ISP_CGMY_08				_bm(_ISP_0114, ISP_REG_BASE, (0x0114<<2), CGMY_08			)	//	
#define ISP_CGMY_09				_bm(_ISP_0114, ISP_REG_BASE, (0x0114<<2), CGMY_09			)	//	
#define ISP_0114_T				_am(_ISP_0114, ISP_REG_BASE, (0x0114<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_10			:10;
		UINT _res1				: 6;
		UINT CGMY_11			:10;	_rege_	_ISP_0115;

#define ISP_CGMY_10				_bm(_ISP_0115, ISP_REG_BASE, (0x0115<<2), CGMY_10			)	//	
#define ISP_CGMY_11				_bm(_ISP_0115, ISP_REG_BASE, (0x0115<<2), CGMY_11			)	//	
#define ISP_0115_T				_am(_ISP_0115, ISP_REG_BASE, (0x0115<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_12			:10;
		UINT _res1				: 6;
		UINT CGMY_13			:10;	_rege_	_ISP_0116;

#define ISP_CGMY_12				_bm(_ISP_0116, ISP_REG_BASE, (0x0116<<2), CGMY_12			)	//	
#define ISP_CGMY_13				_bm(_ISP_0116, ISP_REG_BASE, (0x0116<<2), CGMY_13			)	//	
#define ISP_0116_T				_am(_ISP_0116, ISP_REG_BASE, (0x0116<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_14			:10;
		UINT _res1				: 6;
		UINT CGMY_15			:10;	_rege_	_ISP_0117;

#define ISP_CGMY_14				_bm(_ISP_0117, ISP_REG_BASE, (0x0117<<2), CGMY_14			)	//	
#define ISP_CGMY_15				_bm(_ISP_0117, ISP_REG_BASE, (0x0117<<2), CGMY_15			)	//	
#define ISP_0117_T				_am(_ISP_0117, ISP_REG_BASE, (0x0117<<2))						//	

_regs_	UINT _res0				: 6;
		UINT CGMY_16			:10;
		UINT _res1				:16;	_rege_	_ISP_0118;

#define ISP_CGMY_16				_bm(_ISP_0118, ISP_REG_BASE, (0x0118<<2), CGMY_16			)	//	
#define ISP_0118_T				_am(_ISP_0118, ISP_REG_BASE, (0x0118<<2))						//	

_regs_	UINT FONT_ON			: 1;
		UINT BOAD_ON			: 1;
		UINT BOAD_MODE			: 1;
		UINT FONT_MUTE			: 1;
		UINT FONT_OFY			:12;
		UINT _res0				: 4;
		UINT FONT_OFX			:12;	_rege_	_ISP_0119;

#define ISP_FONT_ON				_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), FONT_ON			)	//	
#define ISP_BOAD_ON				_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), BOAD_ON			)	//	
#define ISP_BOAD_MODE			_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), BOAD_MODE			)	//	
#define ISP_FONT_MUTE			_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), FONT_MUTE			)	//	
#define ISP_FONT_OFY			_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), FONT_OFY			)	//	
#define ISP_FONT_OFX			_bm(_ISP_0119, ISP_REG_BASE, (0x0119<<2), FONT_OFX			)	//	
#define ISP_0119_T				_am(_ISP_0119, ISP_REG_BASE, (0x0119<<2))						//	

_regs_	UINT _res0				: 8;
		UINT FONT_LV0			:24;	_rege_	_ISP_011A;

#define ISP_FONT_LV0			_bm(_ISP_011A, ISP_REG_BASE, (0x011A<<2), FONT_LV0			)	//	
#define ISP_011A_T				_am(_ISP_011A, ISP_REG_BASE, (0x011A<<2))						//	

_regs_	UINT _res0				: 8;
		UINT FONT_LV1			:24;	_rege_	_ISP_011B;

#define ISP_FONT_LV1			_bm(_ISP_011B, ISP_REG_BASE, (0x011B<<2), FONT_LV1			)	//	
#define ISP_011B_T				_am(_ISP_011B, ISP_REG_BASE, (0x011B<<2))						//	

_regs_	UINT _res0				: 8;
		UINT FONT_LV2			:24;	_rege_	_ISP_011C;

#define ISP_FONT_LV2			_bm(_ISP_011C, ISP_REG_BASE, (0x011C<<2), FONT_LV2			)	//	
#define ISP_011C_T				_am(_ISP_011C, ISP_REG_BASE, (0x011C<<2))						//	

_regs_	UINT _res0				: 8;
		UINT FONT_LV3			:24;	_rege_	_ISP_011D;

#define ISP_FONT_LV3			_bm(_ISP_011D, ISP_REG_BASE, (0x011D<<2), FONT_LV3			)	//	
#define ISP_011D_T				_am(_ISP_011D, ISP_REG_BASE, (0x011D<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOAD_LV0			:24;	_rege_	_ISP_011E;

#define ISP_BOAD_LV0			_bm(_ISP_011E, ISP_REG_BASE, (0x011E<<2), BOAD_LV0			)	//	
#define ISP_011E_T				_am(_ISP_011E, ISP_REG_BASE, (0x011E<<2))						//	

_regs_	UINT _res0				: 8;
		UINT FONT_VBLK			: 8;
		UINT FONT_HBLK			: 8;
		UINT FONT_VSIZ			: 4;
		UINT FONT_HSIZ			: 4;	_rege_	_ISP_011F;

#define ISP_FONT_VBLK			_bm(_ISP_011F, ISP_REG_BASE, (0x011F<<2), FONT_VBLK			)	//	
#define ISP_FONT_HBLK			_bm(_ISP_011F, ISP_REG_BASE, (0x011F<<2), FONT_HBLK			)	//	
#define ISP_FONT_VSIZ			_bm(_ISP_011F, ISP_REG_BASE, (0x011F<<2), FONT_VSIZ			)	//	
#define ISP_FONT_HSIZ			_bm(_ISP_011F, ISP_REG_BASE, (0x011F<<2), FONT_HSIZ			)	//	
#define ISP_011F_T				_am(_ISP_011F, ISP_REG_BASE, (0x011F<<2))						//	

_regs_	UINT _res0				: 2;
		UINT MASK_CHAD			: 9;
		UINT HALF_CHAD			: 9;
		UINT FONT_HW			:12;	_rege_	_ISP_0120;

#define ISP_MASK_CHAD			_bm(_ISP_0120, ISP_REG_BASE, (0x0120<<2), MASK_CHAD			)	//	
#define ISP_HALF_CHAD			_bm(_ISP_0120, ISP_REG_BASE, (0x0120<<2), HALF_CHAD			)	//	
#define ISP_FONT_HW				_bm(_ISP_0120, ISP_REG_BASE, (0x0120<<2), FONT_HW			)	//	
#define ISP_0120_T				_am(_ISP_0120, ISP_REG_BASE, (0x0120<<2))						//	

_regs_	UINT OSD_MUTE			: 1;
		UINT _res0				: 1;
		UINT OSD_ISEL			: 2;
		UINT BOX_HSP			:12;
		UINT _res1				: 2;
		UINT OSD_IVSEL			: 2;
		UINT BOX_VSP			:12;	_rege_	_ISP_0121;

#define ISP_OSD_MUTE			_bm(_ISP_0121, ISP_REG_BASE, (0x0121<<2), OSD_MUTE			)	//	
#define ISP_OSD_ISEL			_bm(_ISP_0121, ISP_REG_BASE, (0x0121<<2), OSD_ISEL			)	//	
#define ISP_BOX_HSP				_bm(_ISP_0121, ISP_REG_BASE, (0x0121<<2), BOX_HSP			)	//	
#define ISP_OSD_IVSEL			_bm(_ISP_0121, ISP_REG_BASE, (0x0121<<2), OSD_IVSEL			)	//	
#define ISP_BOX_VSP				_bm(_ISP_0121, ISP_REG_BASE, (0x0121<<2), BOX_VSP			)	//	
#define ISP_0121_T				_am(_ISP_0121, ISP_REG_BASE, (0x0121<<2))						//	

_regs_	UINT BOSD_ON			:32;	_rege_	_ISP_0122;

#define ISP_BOSD_ON				_bm(_ISP_0122, ISP_REG_BASE, (0x0122<<2), BOSD_ON			)	//	
#define ISP_0122_T				_am(_ISP_0122, ISP_REG_BASE, (0x0122<<2))						//	

_regs_	UINT BTONE31			: 2;
		UINT BTONE30			: 2;
		UINT BTONE29			: 2;
		UINT BTONE28			: 2;
		UINT BTONE27			: 2;
		UINT BTONE26			: 2;
		UINT BTONE25			: 2;
		UINT BTONE24			: 2;
		UINT BTONE23			: 2;
		UINT BTONE22			: 2;
		UINT BTONE21			: 2;
		UINT BTONE20			: 2;
		UINT BTONE19			: 2;
		UINT BTONE18			: 2;
		UINT BTONE17			: 2;
		UINT BTONE16			: 2;	_rege_	_ISP_0123;

#define ISP_BTONE31				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE31			)	//	
#define ISP_BTONE30				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE30			)	//	
#define ISP_BTONE29				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE29			)	//	
#define ISP_BTONE28				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE28			)	//	
#define ISP_BTONE27				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE27			)	//	
#define ISP_BTONE26				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE26			)	//	
#define ISP_BTONE25				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE25			)	//	
#define ISP_BTONE24				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE24			)	//	
#define ISP_BTONE23				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE23			)	//	
#define ISP_BTONE22				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE22			)	//	
#define ISP_BTONE21				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE21			)	//	
#define ISP_BTONE20				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE20			)	//	
#define ISP_BTONE19				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE19			)	//	
#define ISP_BTONE18				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE18			)	//	
#define ISP_BTONE17				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE17			)	//	
#define ISP_BTONE16				_bm(_ISP_0123, ISP_REG_BASE, (0x0123<<2), BTONE16			)	//	
#define ISP_0123_T				_am(_ISP_0123, ISP_REG_BASE, (0x0123<<2))						//	

_regs_	UINT BTONE15			: 2;
		UINT BTONE14			: 2;
		UINT BTONE13			: 2;
		UINT BTONE12			: 2;
		UINT BTONE11			: 2;
		UINT BTONE10			: 2;
		UINT BTONE9				: 2;
		UINT BTONE8				: 2;
		UINT BTONE7				: 2;
		UINT BTONE6				: 2;
		UINT BTONE5				: 2;
		UINT BTONE4				: 2;
		UINT BTONE3				: 2;
		UINT BTONE2				: 2;
		UINT BTONE1				: 2;
		UINT BTONE0				: 2;	_rege_	_ISP_0124;

#define ISP_BTONE15				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE15			)	//	
#define ISP_BTONE14				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE14			)	//	
#define ISP_BTONE13				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE13			)	//	
#define ISP_BTONE12				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE12			)	//	
#define ISP_BTONE11				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE11			)	//	
#define ISP_BTONE10				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE10			)	//	
#define ISP_BTONE9				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE9			)	//	
#define ISP_BTONE8				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE8			)	//	
#define ISP_BTONE7				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE7			)	//	
#define ISP_BTONE6				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE6			)	//	
#define ISP_BTONE5				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE5			)	//	
#define ISP_BTONE4				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE4			)	//	
#define ISP_BTONE3				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE3			)	//	
#define ISP_BTONE2				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE2			)	//	
#define ISP_BTONE1				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE1			)	//	
#define ISP_BTONE0				_bm(_ISP_0124, ISP_REG_BASE, (0x0124<<2), BTONE0			)	//	
#define ISP_0124_T				_am(_ISP_0124, ISP_REG_BASE, (0x0124<<2))						//	

_regs_	UINT BFL_ON31			: 1;
		UINT BFL_ON30			: 1;
		UINT BFL_ON29			: 1;
		UINT BFL_ON28			: 1;
		UINT BFL_ON27			: 1;
		UINT BFL_ON26			: 1;
		UINT BFL_ON25			: 1;
		UINT BFL_ON24			: 1;
		UINT BFL_ON23			: 1;
		UINT BFL_ON22			: 1;
		UINT BFL_ON21			: 1;
		UINT BFL_ON20			: 1;
		UINT BFL_ON19			: 1;
		UINT BFL_ON18			: 1;
		UINT BFL_ON17			: 1;
		UINT BFL_ON16			: 1;
		UINT BFL_ON15			: 1;
		UINT BFL_ON14			: 1;
		UINT BFL_ON13			: 1;
		UINT BFL_ON12			: 1;
		UINT BFL_ON11			: 1;
		UINT BFL_ON10			: 1;
		UINT BFL_ON9			: 1;
		UINT BFL_ON8			: 1;
		UINT BFL_ON7			: 1;
		UINT BFL_ON6			: 1;
		UINT BFL_ON5			: 1;
		UINT BFL_ON4			: 1;
		UINT BFL_ON3			: 1;
		UINT BFL_ON2			: 1;
		UINT BFL_ON1			: 1;
		UINT BFL_ON0			: 1;	_rege_	_ISP_0125;

#define ISP_BFL_ON31			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON31			)	//	
#define ISP_BFL_ON30			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON30			)	//	
#define ISP_BFL_ON29			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON29			)	//	
#define ISP_BFL_ON28			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON28			)	//	
#define ISP_BFL_ON27			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON27			)	//	
#define ISP_BFL_ON26			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON26			)	//	
#define ISP_BFL_ON25			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON25			)	//	
#define ISP_BFL_ON24			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON24			)	//	
#define ISP_BFL_ON23			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON23			)	//	
#define ISP_BFL_ON22			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON22			)	//	
#define ISP_BFL_ON21			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON21			)	//	
#define ISP_BFL_ON20			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON20			)	//	
#define ISP_BFL_ON19			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON19			)	//	
#define ISP_BFL_ON18			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON18			)	//	
#define ISP_BFL_ON17			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON17			)	//	
#define ISP_BFL_ON16			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON16			)	//	
#define ISP_BFL_ON15			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON15			)	//	
#define ISP_BFL_ON14			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON14			)	//	
#define ISP_BFL_ON13			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON13			)	//	
#define ISP_BFL_ON12			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON12			)	//	
#define ISP_BFL_ON11			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON11			)	//	
#define ISP_BFL_ON10			_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON10			)	//	
#define ISP_BFL_ON9				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON9			)	//	
#define ISP_BFL_ON8				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON8			)	//	
#define ISP_BFL_ON7				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON7			)	//	
#define ISP_BFL_ON6				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON6			)	//	
#define ISP_BFL_ON5				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON5			)	//	
#define ISP_BFL_ON4				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON4			)	//	
#define ISP_BFL_ON3				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON3			)	//	
#define ISP_BFL_ON2				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON2			)	//	
#define ISP_BFL_ON1				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON1			)	//	
#define ISP_BFL_ON0				_bm(_ISP_0125, ISP_REG_BASE, (0x0125<<2), BFL_ON0			)	//	
#define ISP_0125_T				_am(_ISP_0125, ISP_REG_BASE, (0x0125<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B0_VSP				:12;
		UINT _res1				: 4;
		UINT B0_VED				:12;	_rege_	_ISP_0126;

#define ISP_B0_VSP				_bm(_ISP_0126, ISP_REG_BASE, (0x0126<<2), B0_VSP			)	//	
#define ISP_B0_VED				_bm(_ISP_0126, ISP_REG_BASE, (0x0126<<2), B0_VED			)	//	
#define ISP_0126_T				_am(_ISP_0126, ISP_REG_BASE, (0x0126<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B0_HSP				:12;
		UINT _res1				: 4;
		UINT B0_HED				:12;	_rege_	_ISP_0127;

#define ISP_B0_HSP				_bm(_ISP_0127, ISP_REG_BASE, (0x0127<<2), B0_HSP			)	//	
#define ISP_B0_HED				_bm(_ISP_0127, ISP_REG_BASE, (0x0127<<2), B0_HED			)	//	
#define ISP_0127_T				_am(_ISP_0127, ISP_REG_BASE, (0x0127<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B1_VSP				:12;
		UINT _res1				: 4;
		UINT B1_VED				:12;	_rege_	_ISP_0128;

#define ISP_B1_VSP				_bm(_ISP_0128, ISP_REG_BASE, (0x0128<<2), B1_VSP			)	//	
#define ISP_B1_VED				_bm(_ISP_0128, ISP_REG_BASE, (0x0128<<2), B1_VED			)	//	
#define ISP_0128_T				_am(_ISP_0128, ISP_REG_BASE, (0x0128<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B1_HSP				:12;
		UINT _res1				: 4;
		UINT B1_HED				:12;	_rege_	_ISP_0129;

#define ISP_B1_HSP				_bm(_ISP_0129, ISP_REG_BASE, (0x0129<<2), B1_HSP			)	//	
#define ISP_B1_HED				_bm(_ISP_0129, ISP_REG_BASE, (0x0129<<2), B1_HED			)	//	
#define ISP_0129_T				_am(_ISP_0129, ISP_REG_BASE, (0x0129<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B2_VSP				:12;
		UINT _res1				: 4;
		UINT B2_VED				:12;	_rege_	_ISP_012A;

#define ISP_B2_VSP				_bm(_ISP_012A, ISP_REG_BASE, (0x012A<<2), B2_VSP			)	//	
#define ISP_B2_VED				_bm(_ISP_012A, ISP_REG_BASE, (0x012A<<2), B2_VED			)	//	
#define ISP_012A_T				_am(_ISP_012A, ISP_REG_BASE, (0x012A<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B2_HSP				:12;
		UINT _res1				: 4;
		UINT B2_HED				:12;	_rege_	_ISP_012B;

#define ISP_B2_HSP				_bm(_ISP_012B, ISP_REG_BASE, (0x012B<<2), B2_HSP			)	//	
#define ISP_B2_HED				_bm(_ISP_012B, ISP_REG_BASE, (0x012B<<2), B2_HED			)	//	
#define ISP_012B_T				_am(_ISP_012B, ISP_REG_BASE, (0x012B<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B3_VSP				:12;
		UINT _res1				: 4;
		UINT B3_VED				:12;	_rege_	_ISP_012C;

#define ISP_B3_VSP				_bm(_ISP_012C, ISP_REG_BASE, (0x012C<<2), B3_VSP			)	//	
#define ISP_B3_VED				_bm(_ISP_012C, ISP_REG_BASE, (0x012C<<2), B3_VED			)	//	
#define ISP_012C_T				_am(_ISP_012C, ISP_REG_BASE, (0x012C<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B3_HSP				:12;
		UINT _res1				: 4;
		UINT B3_HED				:12;	_rege_	_ISP_012D;

#define ISP_B3_HSP				_bm(_ISP_012D, ISP_REG_BASE, (0x012D<<2), B3_HSP			)	//	
#define ISP_B3_HED				_bm(_ISP_012D, ISP_REG_BASE, (0x012D<<2), B3_HED			)	//	
#define ISP_012D_T				_am(_ISP_012D, ISP_REG_BASE, (0x012D<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B4_VSP				:12;
		UINT _res1				: 4;
		UINT B4_VED				:12;	_rege_	_ISP_012E;

#define ISP_B4_VSP				_bm(_ISP_012E, ISP_REG_BASE, (0x012E<<2), B4_VSP			)	//	
#define ISP_B4_VED				_bm(_ISP_012E, ISP_REG_BASE, (0x012E<<2), B4_VED			)	//	
#define ISP_012E_T				_am(_ISP_012E, ISP_REG_BASE, (0x012E<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B4_HSP				:12;
		UINT _res1				: 4;
		UINT B4_HED				:12;	_rege_	_ISP_012F;

#define ISP_B4_HSP				_bm(_ISP_012F, ISP_REG_BASE, (0x012F<<2), B4_HSP			)	//	
#define ISP_B4_HED				_bm(_ISP_012F, ISP_REG_BASE, (0x012F<<2), B4_HED			)	//	
#define ISP_012F_T				_am(_ISP_012F, ISP_REG_BASE, (0x012F<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B5_VSP				:12;
		UINT _res1				: 4;
		UINT B5_VED				:12;	_rege_	_ISP_0130;

#define ISP_B5_VSP				_bm(_ISP_0130, ISP_REG_BASE, (0x0130<<2), B5_VSP			)	//	
#define ISP_B5_VED				_bm(_ISP_0130, ISP_REG_BASE, (0x0130<<2), B5_VED			)	//	
#define ISP_0130_T				_am(_ISP_0130, ISP_REG_BASE, (0x0130<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B5_HSP				:12;
		UINT _res1				: 4;
		UINT B5_HED				:12;	_rege_	_ISP_0131;

#define ISP_B5_HSP				_bm(_ISP_0131, ISP_REG_BASE, (0x0131<<2), B5_HSP			)	//	
#define ISP_B5_HED				_bm(_ISP_0131, ISP_REG_BASE, (0x0131<<2), B5_HED			)	//	
#define ISP_0131_T				_am(_ISP_0131, ISP_REG_BASE, (0x0131<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B6_VSP				:12;
		UINT _res1				: 4;
		UINT B6_VED				:12;	_rege_	_ISP_0132;

#define ISP_B6_VSP				_bm(_ISP_0132, ISP_REG_BASE, (0x0132<<2), B6_VSP			)	//	
#define ISP_B6_VED				_bm(_ISP_0132, ISP_REG_BASE, (0x0132<<2), B6_VED			)	//	
#define ISP_0132_T				_am(_ISP_0132, ISP_REG_BASE, (0x0132<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B6_HSP				:12;
		UINT _res1				: 4;
		UINT B6_HED				:12;	_rege_	_ISP_0133;

#define ISP_B6_HSP				_bm(_ISP_0133, ISP_REG_BASE, (0x0133<<2), B6_HSP			)	//	
#define ISP_B6_HED				_bm(_ISP_0133, ISP_REG_BASE, (0x0133<<2), B6_HED			)	//	
#define ISP_0133_T				_am(_ISP_0133, ISP_REG_BASE, (0x0133<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B7_VSP				:12;
		UINT _res1				: 4;
		UINT B7_VED				:12;	_rege_	_ISP_0134;

#define ISP_B7_VSP				_bm(_ISP_0134, ISP_REG_BASE, (0x0134<<2), B7_VSP			)	//	
#define ISP_B7_VED				_bm(_ISP_0134, ISP_REG_BASE, (0x0134<<2), B7_VED			)	//	
#define ISP_0134_T				_am(_ISP_0134, ISP_REG_BASE, (0x0134<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B7_HSP				:12;
		UINT _res1				: 4;
		UINT B7_HED				:12;	_rege_	_ISP_0135;

#define ISP_B7_HSP				_bm(_ISP_0135, ISP_REG_BASE, (0x0135<<2), B7_HSP			)	//	
#define ISP_B7_HED				_bm(_ISP_0135, ISP_REG_BASE, (0x0135<<2), B7_HED			)	//	
#define ISP_0135_T				_am(_ISP_0135, ISP_REG_BASE, (0x0135<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B8_VSP				:12;
		UINT _res1				: 4;
		UINT B8_VED				:12;	_rege_	_ISP_0136;

#define ISP_B8_VSP				_bm(_ISP_0136, ISP_REG_BASE, (0x0136<<2), B8_VSP			)	//	
#define ISP_B8_VED				_bm(_ISP_0136, ISP_REG_BASE, (0x0136<<2), B8_VED			)	//	
#define ISP_0136_T				_am(_ISP_0136, ISP_REG_BASE, (0x0136<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B8_HSP				:12;
		UINT _res1				: 4;
		UINT B8_HED				:12;	_rege_	_ISP_0137;

#define ISP_B8_HSP				_bm(_ISP_0137, ISP_REG_BASE, (0x0137<<2), B8_HSP			)	//	
#define ISP_B8_HED				_bm(_ISP_0137, ISP_REG_BASE, (0x0137<<2), B8_HED			)	//	
#define ISP_0137_T				_am(_ISP_0137, ISP_REG_BASE, (0x0137<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B9_VSP				:12;
		UINT _res1				: 4;
		UINT B9_VED				:12;	_rege_	_ISP_0138;

#define ISP_B9_VSP				_bm(_ISP_0138, ISP_REG_BASE, (0x0138<<2), B9_VSP			)	//	
#define ISP_B9_VED				_bm(_ISP_0138, ISP_REG_BASE, (0x0138<<2), B9_VED			)	//	
#define ISP_0138_T				_am(_ISP_0138, ISP_REG_BASE, (0x0138<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B9_HSP				:12;
		UINT _res1				: 4;
		UINT B9_HED				:12;	_rege_	_ISP_0139;

#define ISP_B9_HSP				_bm(_ISP_0139, ISP_REG_BASE, (0x0139<<2), B9_HSP			)	//	
#define ISP_B9_HED				_bm(_ISP_0139, ISP_REG_BASE, (0x0139<<2), B9_HED			)	//	
#define ISP_0139_T				_am(_ISP_0139, ISP_REG_BASE, (0x0139<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B10_VSP			:12;
		UINT _res1				: 4;
		UINT B10_VED			:12;	_rege_	_ISP_013A;

#define ISP_B10_VSP				_bm(_ISP_013A, ISP_REG_BASE, (0x013A<<2), B10_VSP			)	//	
#define ISP_B10_VED				_bm(_ISP_013A, ISP_REG_BASE, (0x013A<<2), B10_VED			)	//	
#define ISP_013A_T				_am(_ISP_013A, ISP_REG_BASE, (0x013A<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B10_HSP			:12;
		UINT _res1				: 4;
		UINT B10_HED			:12;	_rege_	_ISP_013B;

#define ISP_B10_HSP				_bm(_ISP_013B, ISP_REG_BASE, (0x013B<<2), B10_HSP			)	//	
#define ISP_B10_HED				_bm(_ISP_013B, ISP_REG_BASE, (0x013B<<2), B10_HED			)	//	
#define ISP_013B_T				_am(_ISP_013B, ISP_REG_BASE, (0x013B<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B11_VSP			:12;
		UINT _res1				: 4;
		UINT B11_VED			:12;	_rege_	_ISP_013C;

#define ISP_B11_VSP				_bm(_ISP_013C, ISP_REG_BASE, (0x013C<<2), B11_VSP			)	//	
#define ISP_B11_VED				_bm(_ISP_013C, ISP_REG_BASE, (0x013C<<2), B11_VED			)	//	
#define ISP_013C_T				_am(_ISP_013C, ISP_REG_BASE, (0x013C<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B11_HSP			:12;
		UINT _res1				: 4;
		UINT B11_HED			:12;	_rege_	_ISP_013D;

#define ISP_B11_HSP				_bm(_ISP_013D, ISP_REG_BASE, (0x013D<<2), B11_HSP			)	//	
#define ISP_B11_HED				_bm(_ISP_013D, ISP_REG_BASE, (0x013D<<2), B11_HED			)	//	
#define ISP_013D_T				_am(_ISP_013D, ISP_REG_BASE, (0x013D<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B12_VSP			:12;
		UINT _res1				: 4;
		UINT B12_VED			:12;	_rege_	_ISP_013E;

#define ISP_B12_VSP				_bm(_ISP_013E, ISP_REG_BASE, (0x013E<<2), B12_VSP			)	//	
#define ISP_B12_VED				_bm(_ISP_013E, ISP_REG_BASE, (0x013E<<2), B12_VED			)	//	
#define ISP_013E_T				_am(_ISP_013E, ISP_REG_BASE, (0x013E<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B12_HSP			:12;
		UINT _res1				: 4;
		UINT B12_HED			:12;	_rege_	_ISP_013F;

#define ISP_B12_HSP				_bm(_ISP_013F, ISP_REG_BASE, (0x013F<<2), B12_HSP			)	//	
#define ISP_B12_HED				_bm(_ISP_013F, ISP_REG_BASE, (0x013F<<2), B12_HED			)	//	
#define ISP_013F_T				_am(_ISP_013F, ISP_REG_BASE, (0x013F<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B13_VSP			:12;
		UINT _res1				: 4;
		UINT B13_VED			:12;	_rege_	_ISP_0140;

#define ISP_B13_VSP				_bm(_ISP_0140, ISP_REG_BASE, (0x0140<<2), B13_VSP			)	//	
#define ISP_B13_VED				_bm(_ISP_0140, ISP_REG_BASE, (0x0140<<2), B13_VED			)	//	
#define ISP_0140_T				_am(_ISP_0140, ISP_REG_BASE, (0x0140<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B13_HSP			:12;
		UINT _res1				: 4;
		UINT B13_HED			:12;	_rege_	_ISP_0141;

#define ISP_B13_HSP				_bm(_ISP_0141, ISP_REG_BASE, (0x0141<<2), B13_HSP			)	//	
#define ISP_B13_HED				_bm(_ISP_0141, ISP_REG_BASE, (0x0141<<2), B13_HED			)	//	
#define ISP_0141_T				_am(_ISP_0141, ISP_REG_BASE, (0x0141<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B14_VSP			:12;
		UINT _res1				: 4;
		UINT B14_VED			:12;	_rege_	_ISP_0142;

#define ISP_B14_VSP				_bm(_ISP_0142, ISP_REG_BASE, (0x0142<<2), B14_VSP			)	//	
#define ISP_B14_VED				_bm(_ISP_0142, ISP_REG_BASE, (0x0142<<2), B14_VED			)	//	
#define ISP_0142_T				_am(_ISP_0142, ISP_REG_BASE, (0x0142<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B14_HSP			:12;
		UINT _res1				: 4;
		UINT B14_HED			:12;	_rege_	_ISP_0143;

#define ISP_B14_HSP				_bm(_ISP_0143, ISP_REG_BASE, (0x0143<<2), B14_HSP			)	//	
#define ISP_B14_HED				_bm(_ISP_0143, ISP_REG_BASE, (0x0143<<2), B14_HED			)	//	
#define ISP_0143_T				_am(_ISP_0143, ISP_REG_BASE, (0x0143<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B15_VSP			:12;
		UINT _res1				: 4;
		UINT B15_VED			:12;	_rege_	_ISP_0144;

#define ISP_B15_VSP				_bm(_ISP_0144, ISP_REG_BASE, (0x0144<<2), B15_VSP			)	//	
#define ISP_B15_VED				_bm(_ISP_0144, ISP_REG_BASE, (0x0144<<2), B15_VED			)	//	
#define ISP_0144_T				_am(_ISP_0144, ISP_REG_BASE, (0x0144<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B15_HSP			:12;
		UINT _res1				: 4;
		UINT B15_HED			:12;	_rege_	_ISP_0145;

#define ISP_B15_HSP				_bm(_ISP_0145, ISP_REG_BASE, (0x0145<<2), B15_HSP			)	//	
#define ISP_B15_HED				_bm(_ISP_0145, ISP_REG_BASE, (0x0145<<2), B15_HED			)	//	
#define ISP_0145_T				_am(_ISP_0145, ISP_REG_BASE, (0x0145<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B16_VSP			:12;
		UINT _res1				: 4;
		UINT B16_VED			:12;	_rege_	_ISP_0146;

#define ISP_B16_VSP				_bm(_ISP_0146, ISP_REG_BASE, (0x0146<<2), B16_VSP			)	//	
#define ISP_B16_VED				_bm(_ISP_0146, ISP_REG_BASE, (0x0146<<2), B16_VED			)	//	
#define ISP_0146_T				_am(_ISP_0146, ISP_REG_BASE, (0x0146<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B16_HSP			:12;
		UINT _res1				: 4;
		UINT B16_HED			:12;	_rege_	_ISP_0147;

#define ISP_B16_HSP				_bm(_ISP_0147, ISP_REG_BASE, (0x0147<<2), B16_HSP			)	//	
#define ISP_B16_HED				_bm(_ISP_0147, ISP_REG_BASE, (0x0147<<2), B16_HED			)	//	
#define ISP_0147_T				_am(_ISP_0147, ISP_REG_BASE, (0x0147<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B17_VSP			:12;
		UINT _res1				: 4;
		UINT B17_VED			:12;	_rege_	_ISP_0148;

#define ISP_B17_VSP				_bm(_ISP_0148, ISP_REG_BASE, (0x0148<<2), B17_VSP			)	//	
#define ISP_B17_VED				_bm(_ISP_0148, ISP_REG_BASE, (0x0148<<2), B17_VED			)	//	
#define ISP_0148_T				_am(_ISP_0148, ISP_REG_BASE, (0x0148<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B17_HSP			:12;
		UINT _res1				: 4;
		UINT B17_HED			:12;	_rege_	_ISP_0149;

#define ISP_B17_HSP				_bm(_ISP_0149, ISP_REG_BASE, (0x0149<<2), B17_HSP			)	//	
#define ISP_B17_HED				_bm(_ISP_0149, ISP_REG_BASE, (0x0149<<2), B17_HED			)	//	
#define ISP_0149_T				_am(_ISP_0149, ISP_REG_BASE, (0x0149<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B18_VSP			:12;
		UINT _res1				: 4;
		UINT B18_VED			:12;	_rege_	_ISP_014A;

#define ISP_B18_VSP				_bm(_ISP_014A, ISP_REG_BASE, (0x014A<<2), B18_VSP			)	//	
#define ISP_B18_VED				_bm(_ISP_014A, ISP_REG_BASE, (0x014A<<2), B18_VED			)	//	
#define ISP_014A_T				_am(_ISP_014A, ISP_REG_BASE, (0x014A<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B18_HSP			:12;
		UINT _res1				: 4;
		UINT B18_HED			:12;	_rege_	_ISP_014B;

#define ISP_B18_HSP				_bm(_ISP_014B, ISP_REG_BASE, (0x014B<<2), B18_HSP			)	//	
#define ISP_B18_HED				_bm(_ISP_014B, ISP_REG_BASE, (0x014B<<2), B18_HED			)	//	
#define ISP_014B_T				_am(_ISP_014B, ISP_REG_BASE, (0x014B<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B19_VSP			:12;
		UINT _res1				: 4;
		UINT B19_VED			:12;	_rege_	_ISP_014C;

#define ISP_B19_VSP				_bm(_ISP_014C, ISP_REG_BASE, (0x014C<<2), B19_VSP			)	//	
#define ISP_B19_VED				_bm(_ISP_014C, ISP_REG_BASE, (0x014C<<2), B19_VED			)	//	
#define ISP_014C_T				_am(_ISP_014C, ISP_REG_BASE, (0x014C<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B19_HSP			:12;
		UINT _res1				: 4;
		UINT B19_HED			:12;	_rege_	_ISP_014D;

#define ISP_B19_HSP				_bm(_ISP_014D, ISP_REG_BASE, (0x014D<<2), B19_HSP			)	//	
#define ISP_B19_HED				_bm(_ISP_014D, ISP_REG_BASE, (0x014D<<2), B19_HED			)	//	
#define ISP_014D_T				_am(_ISP_014D, ISP_REG_BASE, (0x014D<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B20_VSP			:12;
		UINT _res1				: 4;
		UINT B20_VED			:12;	_rege_	_ISP_014E;

#define ISP_B20_VSP				_bm(_ISP_014E, ISP_REG_BASE, (0x014E<<2), B20_VSP			)	//	
#define ISP_B20_VED				_bm(_ISP_014E, ISP_REG_BASE, (0x014E<<2), B20_VED			)	//	
#define ISP_014E_T				_am(_ISP_014E, ISP_REG_BASE, (0x014E<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B20_HSP			:12;
		UINT _res1				: 4;
		UINT B20_HED			:12;	_rege_	_ISP_014F;

#define ISP_B20_HSP				_bm(_ISP_014F, ISP_REG_BASE, (0x014F<<2), B20_HSP			)	//	
#define ISP_B20_HED				_bm(_ISP_014F, ISP_REG_BASE, (0x014F<<2), B20_HED			)	//	
#define ISP_014F_T				_am(_ISP_014F, ISP_REG_BASE, (0x014F<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B21_VSP			:12;
		UINT _res1				: 4;
		UINT B21_VED			:12;	_rege_	_ISP_0150;

#define ISP_B21_VSP				_bm(_ISP_0150, ISP_REG_BASE, (0x0150<<2), B21_VSP			)	//	
#define ISP_B21_VED				_bm(_ISP_0150, ISP_REG_BASE, (0x0150<<2), B21_VED			)	//	
#define ISP_0150_T				_am(_ISP_0150, ISP_REG_BASE, (0x0150<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B21_HSP			:12;
		UINT _res1				: 4;
		UINT B21_HED			:12;	_rege_	_ISP_0151;

#define ISP_B21_HSP				_bm(_ISP_0151, ISP_REG_BASE, (0x0151<<2), B21_HSP			)	//	
#define ISP_B21_HED				_bm(_ISP_0151, ISP_REG_BASE, (0x0151<<2), B21_HED			)	//	
#define ISP_0151_T				_am(_ISP_0151, ISP_REG_BASE, (0x0151<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B22_VSP			:12;
		UINT _res1				: 4;
		UINT B22_VED			:12;	_rege_	_ISP_0152;

#define ISP_B22_VSP				_bm(_ISP_0152, ISP_REG_BASE, (0x0152<<2), B22_VSP			)	//	
#define ISP_B22_VED				_bm(_ISP_0152, ISP_REG_BASE, (0x0152<<2), B22_VED			)	//	
#define ISP_0152_T				_am(_ISP_0152, ISP_REG_BASE, (0x0152<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B22_HSP			:12;
		UINT _res1				: 4;
		UINT B22_HED			:12;	_rege_	_ISP_0153;

#define ISP_B22_HSP				_bm(_ISP_0153, ISP_REG_BASE, (0x0153<<2), B22_HSP			)	//	
#define ISP_B22_HED				_bm(_ISP_0153, ISP_REG_BASE, (0x0153<<2), B22_HED			)	//	
#define ISP_0153_T				_am(_ISP_0153, ISP_REG_BASE, (0x0153<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B23_VSP			:12;
		UINT _res1				: 4;
		UINT B23_VED			:12;	_rege_	_ISP_0154;

#define ISP_B23_VSP				_bm(_ISP_0154, ISP_REG_BASE, (0x0154<<2), B23_VSP			)	//	
#define ISP_B23_VED				_bm(_ISP_0154, ISP_REG_BASE, (0x0154<<2), B23_VED			)	//	
#define ISP_0154_T				_am(_ISP_0154, ISP_REG_BASE, (0x0154<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B23_HSP			:12;
		UINT _res1				: 4;
		UINT B23_HED			:12;	_rege_	_ISP_0155;

#define ISP_B23_HSP				_bm(_ISP_0155, ISP_REG_BASE, (0x0155<<2), B23_HSP			)	//	
#define ISP_B23_HED				_bm(_ISP_0155, ISP_REG_BASE, (0x0155<<2), B23_HED			)	//	
#define ISP_0155_T				_am(_ISP_0155, ISP_REG_BASE, (0x0155<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B24_VSP			:12;
		UINT _res1				: 4;
		UINT B24_VED			:12;	_rege_	_ISP_0156;

#define ISP_B24_VSP				_bm(_ISP_0156, ISP_REG_BASE, (0x0156<<2), B24_VSP			)	//	
#define ISP_B24_VED				_bm(_ISP_0156, ISP_REG_BASE, (0x0156<<2), B24_VED			)	//	
#define ISP_0156_T				_am(_ISP_0156, ISP_REG_BASE, (0x0156<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B24_HSP			:12;
		UINT _res1				: 4;
		UINT B24_HED			:12;	_rege_	_ISP_0157;

#define ISP_B24_HSP				_bm(_ISP_0157, ISP_REG_BASE, (0x0157<<2), B24_HSP			)	//	
#define ISP_B24_HED				_bm(_ISP_0157, ISP_REG_BASE, (0x0157<<2), B24_HED			)	//	
#define ISP_0157_T				_am(_ISP_0157, ISP_REG_BASE, (0x0157<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B25_VSP			:12;
		UINT _res1				: 4;
		UINT B25_VED			:12;	_rege_	_ISP_0158;

#define ISP_B25_VSP				_bm(_ISP_0158, ISP_REG_BASE, (0x0158<<2), B25_VSP			)	//	
#define ISP_B25_VED				_bm(_ISP_0158, ISP_REG_BASE, (0x0158<<2), B25_VED			)	//	
#define ISP_0158_T				_am(_ISP_0158, ISP_REG_BASE, (0x0158<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B25_HSP			:12;
		UINT _res1				: 4;
		UINT B25_HED			:12;	_rege_	_ISP_0159;

#define ISP_B25_HSP				_bm(_ISP_0159, ISP_REG_BASE, (0x0159<<2), B25_HSP			)	//	
#define ISP_B25_HED				_bm(_ISP_0159, ISP_REG_BASE, (0x0159<<2), B25_HED			)	//	
#define ISP_0159_T				_am(_ISP_0159, ISP_REG_BASE, (0x0159<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B26_VSP			:12;
		UINT _res1				: 4;
		UINT B26_VED			:12;	_rege_	_ISP_015A;

#define ISP_B26_VSP				_bm(_ISP_015A, ISP_REG_BASE, (0x015A<<2), B26_VSP			)	//	
#define ISP_B26_VED				_bm(_ISP_015A, ISP_REG_BASE, (0x015A<<2), B26_VED			)	//	
#define ISP_015A_T				_am(_ISP_015A, ISP_REG_BASE, (0x015A<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B26_HSP			:12;
		UINT _res1				: 4;
		UINT B26_HED			:12;	_rege_	_ISP_015B;

#define ISP_B26_HSP				_bm(_ISP_015B, ISP_REG_BASE, (0x015B<<2), B26_HSP			)	//	
#define ISP_B26_HED				_bm(_ISP_015B, ISP_REG_BASE, (0x015B<<2), B26_HED			)	//	
#define ISP_015B_T				_am(_ISP_015B, ISP_REG_BASE, (0x015B<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B27_VSP			:12;
		UINT _res1				: 4;
		UINT B27_VED			:12;	_rege_	_ISP_015C;

#define ISP_B27_VSP				_bm(_ISP_015C, ISP_REG_BASE, (0x015C<<2), B27_VSP			)	//	
#define ISP_B27_VED				_bm(_ISP_015C, ISP_REG_BASE, (0x015C<<2), B27_VED			)	//	
#define ISP_015C_T				_am(_ISP_015C, ISP_REG_BASE, (0x015C<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B27_HSP			:12;
		UINT _res1				: 4;
		UINT B27_HED			:12;	_rege_	_ISP_015D;

#define ISP_B27_HSP				_bm(_ISP_015D, ISP_REG_BASE, (0x015D<<2), B27_HSP			)	//	
#define ISP_B27_HED				_bm(_ISP_015D, ISP_REG_BASE, (0x015D<<2), B27_HED			)	//	
#define ISP_015D_T				_am(_ISP_015D, ISP_REG_BASE, (0x015D<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B28_VSP			:12;
		UINT _res1				: 4;
		UINT B28_VED			:12;	_rege_	_ISP_015E;

#define ISP_B28_VSP				_bm(_ISP_015E, ISP_REG_BASE, (0x015E<<2), B28_VSP			)	//	
#define ISP_B28_VED				_bm(_ISP_015E, ISP_REG_BASE, (0x015E<<2), B28_VED			)	//	
#define ISP_015E_T				_am(_ISP_015E, ISP_REG_BASE, (0x015E<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B28_HSP			:12;
		UINT _res1				: 4;
		UINT B28_HED			:12;	_rege_	_ISP_015F;

#define ISP_B28_HSP				_bm(_ISP_015F, ISP_REG_BASE, (0x015F<<2), B28_HSP			)	//	
#define ISP_B28_HED				_bm(_ISP_015F, ISP_REG_BASE, (0x015F<<2), B28_HED			)	//	
#define ISP_015F_T				_am(_ISP_015F, ISP_REG_BASE, (0x015F<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B29_VSP			:12;
		UINT _res1				: 4;
		UINT B29_VED			:12;	_rege_	_ISP_0160;

#define ISP_B29_VSP				_bm(_ISP_0160, ISP_REG_BASE, (0x0160<<2), B29_VSP			)	//	
#define ISP_B29_VED				_bm(_ISP_0160, ISP_REG_BASE, (0x0160<<2), B29_VED			)	//	
#define ISP_0160_T				_am(_ISP_0160, ISP_REG_BASE, (0x0160<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B29_HSP			:12;
		UINT _res1				: 4;
		UINT B29_HED			:12;	_rege_	_ISP_0161;

#define ISP_B29_HSP				_bm(_ISP_0161, ISP_REG_BASE, (0x0161<<2), B29_HSP			)	//	
#define ISP_B29_HED				_bm(_ISP_0161, ISP_REG_BASE, (0x0161<<2), B29_HED			)	//	
#define ISP_0161_T				_am(_ISP_0161, ISP_REG_BASE, (0x0161<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B30_VSP			:12;
		UINT _res1				: 4;
		UINT B30_VED			:12;	_rege_	_ISP_0162;

#define ISP_B30_VSP				_bm(_ISP_0162, ISP_REG_BASE, (0x0162<<2), B30_VSP			)	//	
#define ISP_B30_VED				_bm(_ISP_0162, ISP_REG_BASE, (0x0162<<2), B30_VED			)	//	
#define ISP_0162_T				_am(_ISP_0162, ISP_REG_BASE, (0x0162<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B30_HSP			:12;
		UINT _res1				: 4;
		UINT B30_HED			:12;	_rege_	_ISP_0163;

#define ISP_B30_HSP				_bm(_ISP_0163, ISP_REG_BASE, (0x0163<<2), B30_HSP			)	//	
#define ISP_B30_HED				_bm(_ISP_0163, ISP_REG_BASE, (0x0163<<2), B30_HED			)	//	
#define ISP_0163_T				_am(_ISP_0163, ISP_REG_BASE, (0x0163<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B31_VSP			:12;
		UINT _res1				: 4;
		UINT B31_VED			:12;	_rege_	_ISP_0164;

#define ISP_B31_VSP				_bm(_ISP_0164, ISP_REG_BASE, (0x0164<<2), B31_VSP			)	//	
#define ISP_B31_VED				_bm(_ISP_0164, ISP_REG_BASE, (0x0164<<2), B31_VED			)	//	
#define ISP_0164_T				_am(_ISP_0164, ISP_REG_BASE, (0x0164<<2))						//	

_regs_	UINT _res0				: 4;
		UINT B31_HSP			:12;
		UINT _res1				: 4;
		UINT B31_HED			:12;	_rege_	_ISP_0165;

#define ISP_B31_HSP				_bm(_ISP_0165, ISP_REG_BASE, (0x0165<<2), B31_HSP			)	//	
#define ISP_B31_HED				_bm(_ISP_0165, ISP_REG_BASE, (0x0165<<2), B31_HED			)	//	
#define ISP_0165_T				_am(_ISP_0165, ISP_REG_BASE, (0x0165<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX0_PLT			:24;	_rege_	_ISP_0167;

#define ISP_BOX0_PLT			_bm(_ISP_0167, ISP_REG_BASE, (0x0167<<2), BOX0_PLT			)	//	
#define ISP_0167_T				_am(_ISP_0167, ISP_REG_BASE, (0x0167<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX1_PLT			:24;	_rege_	_ISP_0168;

#define ISP_BOX1_PLT			_bm(_ISP_0168, ISP_REG_BASE, (0x0168<<2), BOX1_PLT			)	//	
#define ISP_0168_T				_am(_ISP_0168, ISP_REG_BASE, (0x0168<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX2_PLT			:24;	_rege_	_ISP_0169;

#define ISP_BOX2_PLT			_bm(_ISP_0169, ISP_REG_BASE, (0x0169<<2), BOX2_PLT			)	//	
#define ISP_0169_T				_am(_ISP_0169, ISP_REG_BASE, (0x0169<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX3_PLT			:24;	_rege_	_ISP_016A;

#define ISP_BOX3_PLT			_bm(_ISP_016A, ISP_REG_BASE, (0x016A<<2), BOX3_PLT			)	//	
#define ISP_016A_T				_am(_ISP_016A, ISP_REG_BASE, (0x016A<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX4_PLT			:24;	_rege_	_ISP_016B;

#define ISP_BOX4_PLT			_bm(_ISP_016B, ISP_REG_BASE, (0x016B<<2), BOX4_PLT			)	//	
#define ISP_016B_T				_am(_ISP_016B, ISP_REG_BASE, (0x016B<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX5_PLT			:24;	_rege_	_ISP_016C;

#define ISP_BOX5_PLT			_bm(_ISP_016C, ISP_REG_BASE, (0x016C<<2), BOX5_PLT			)	//	
#define ISP_016C_T				_am(_ISP_016C, ISP_REG_BASE, (0x016C<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX6_PLT			:24;	_rege_	_ISP_016D;

#define ISP_BOX6_PLT			_bm(_ISP_016D, ISP_REG_BASE, (0x016D<<2), BOX6_PLT			)	//	
#define ISP_016D_T				_am(_ISP_016D, ISP_REG_BASE, (0x016D<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX7_PLT			:24;	_rege_	_ISP_016E;

#define ISP_BOX7_PLT			_bm(_ISP_016E, ISP_REG_BASE, (0x016E<<2), BOX7_PLT			)	//	
#define ISP_016E_T				_am(_ISP_016E, ISP_REG_BASE, (0x016E<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX8_PLT			:24;	_rege_	_ISP_016F;

#define ISP_BOX8_PLT			_bm(_ISP_016F, ISP_REG_BASE, (0x016F<<2), BOX8_PLT			)	//	
#define ISP_016F_T				_am(_ISP_016F, ISP_REG_BASE, (0x016F<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX9_PLT			:24;	_rege_	_ISP_0170;

#define ISP_BOX9_PLT			_bm(_ISP_0170, ISP_REG_BASE, (0x0170<<2), BOX9_PLT			)	//	
#define ISP_0170_T				_am(_ISP_0170, ISP_REG_BASE, (0x0170<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX10_PLT			:24;	_rege_	_ISP_0171;

#define ISP_BOX10_PLT			_bm(_ISP_0171, ISP_REG_BASE, (0x0171<<2), BOX10_PLT			)	//	
#define ISP_0171_T				_am(_ISP_0171, ISP_REG_BASE, (0x0171<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX11_PLT			:24;	_rege_	_ISP_0172;

#define ISP_BOX11_PLT			_bm(_ISP_0172, ISP_REG_BASE, (0x0172<<2), BOX11_PLT			)	//	
#define ISP_0172_T				_am(_ISP_0172, ISP_REG_BASE, (0x0172<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX12_PLT			:24;	_rege_	_ISP_0173;

#define ISP_BOX12_PLT			_bm(_ISP_0173, ISP_REG_BASE, (0x0173<<2), BOX12_PLT			)	//	
#define ISP_0173_T				_am(_ISP_0173, ISP_REG_BASE, (0x0173<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX13_PLT			:24;	_rege_	_ISP_0174;

#define ISP_BOX13_PLT			_bm(_ISP_0174, ISP_REG_BASE, (0x0174<<2), BOX13_PLT			)	//	
#define ISP_0174_T				_am(_ISP_0174, ISP_REG_BASE, (0x0174<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX14_PLT			:24;	_rege_	_ISP_0175;

#define ISP_BOX14_PLT			_bm(_ISP_0175, ISP_REG_BASE, (0x0175<<2), BOX14_PLT			)	//	
#define ISP_0175_T				_am(_ISP_0175, ISP_REG_BASE, (0x0175<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX15_PLT			:24;	_rege_	_ISP_0176;

#define ISP_BOX15_PLT			_bm(_ISP_0176, ISP_REG_BASE, (0x0176<<2), BOX15_PLT			)	//	
#define ISP_0176_T				_am(_ISP_0176, ISP_REG_BASE, (0x0176<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX16_PLT			:24;	_rege_	_ISP_0177;

#define ISP_BOX16_PLT			_bm(_ISP_0177, ISP_REG_BASE, (0x0177<<2), BOX16_PLT			)	//	
#define ISP_0177_T				_am(_ISP_0177, ISP_REG_BASE, (0x0177<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX17_PLT			:24;	_rege_	_ISP_0178;

#define ISP_BOX17_PLT			_bm(_ISP_0178, ISP_REG_BASE, (0x0178<<2), BOX17_PLT			)	//	
#define ISP_0178_T				_am(_ISP_0178, ISP_REG_BASE, (0x0178<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX18_PLT			:24;	_rege_	_ISP_0179;

#define ISP_BOX18_PLT			_bm(_ISP_0179, ISP_REG_BASE, (0x0179<<2), BOX18_PLT			)	//	
#define ISP_0179_T				_am(_ISP_0179, ISP_REG_BASE, (0x0179<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX19_PLT			:24;	_rege_	_ISP_017A;

#define ISP_BOX19_PLT			_bm(_ISP_017A, ISP_REG_BASE, (0x017A<<2), BOX19_PLT			)	//	
#define ISP_017A_T				_am(_ISP_017A, ISP_REG_BASE, (0x017A<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX20_PLT			:24;	_rege_	_ISP_017B;

#define ISP_BOX20_PLT			_bm(_ISP_017B, ISP_REG_BASE, (0x017B<<2), BOX20_PLT			)	//	
#define ISP_017B_T				_am(_ISP_017B, ISP_REG_BASE, (0x017B<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX21_PLT			:24;	_rege_	_ISP_017C;

#define ISP_BOX21_PLT			_bm(_ISP_017C, ISP_REG_BASE, (0x017C<<2), BOX21_PLT			)	//	
#define ISP_017C_T				_am(_ISP_017C, ISP_REG_BASE, (0x017C<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX22_PLT			:24;	_rege_	_ISP_017D;

#define ISP_BOX22_PLT			_bm(_ISP_017D, ISP_REG_BASE, (0x017D<<2), BOX22_PLT			)	//	
#define ISP_017D_T				_am(_ISP_017D, ISP_REG_BASE, (0x017D<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX23_PLT			:24;	_rege_	_ISP_017E;

#define ISP_BOX23_PLT			_bm(_ISP_017E, ISP_REG_BASE, (0x017E<<2), BOX23_PLT			)	//	
#define ISP_017E_T				_am(_ISP_017E, ISP_REG_BASE, (0x017E<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX24_PLT			:24;	_rege_	_ISP_017F;

#define ISP_BOX24_PLT			_bm(_ISP_017F, ISP_REG_BASE, (0x017F<<2), BOX24_PLT			)	//	
#define ISP_017F_T				_am(_ISP_017F, ISP_REG_BASE, (0x017F<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX25_PLT			:24;	_rege_	_ISP_0180;

#define ISP_BOX25_PLT			_bm(_ISP_0180, ISP_REG_BASE, (0x0180<<2), BOX25_PLT			)	//	
#define ISP_0180_T				_am(_ISP_0180, ISP_REG_BASE, (0x0180<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX26_PLT			:24;	_rege_	_ISP_0181;

#define ISP_BOX26_PLT			_bm(_ISP_0181, ISP_REG_BASE, (0x0181<<2), BOX26_PLT			)	//	
#define ISP_0181_T				_am(_ISP_0181, ISP_REG_BASE, (0x0181<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX27_PLT			:24;	_rege_	_ISP_0182;

#define ISP_BOX27_PLT			_bm(_ISP_0182, ISP_REG_BASE, (0x0182<<2), BOX27_PLT			)	//	
#define ISP_0182_T				_am(_ISP_0182, ISP_REG_BASE, (0x0182<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX28_PLT			:24;	_rege_	_ISP_0183;

#define ISP_BOX28_PLT			_bm(_ISP_0183, ISP_REG_BASE, (0x0183<<2), BOX28_PLT			)	//	
#define ISP_0183_T				_am(_ISP_0183, ISP_REG_BASE, (0x0183<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX29_PLT			:24;	_rege_	_ISP_0184;

#define ISP_BOX29_PLT			_bm(_ISP_0184, ISP_REG_BASE, (0x0184<<2), BOX29_PLT			)	//	
#define ISP_0184_T				_am(_ISP_0184, ISP_REG_BASE, (0x0184<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX30_PLT			:24;	_rege_	_ISP_0185;

#define ISP_BOX30_PLT			_bm(_ISP_0185, ISP_REG_BASE, (0x0185<<2), BOX30_PLT			)	//	
#define ISP_0185_T				_am(_ISP_0185, ISP_REG_BASE, (0x0185<<2))						//	

_regs_	UINT _res0				: 8;
		UINT BOX31_PLT			:24;	_rege_	_ISP_0186;

#define ISP_BOX31_PLT			_bm(_ISP_0186, ISP_REG_BASE, (0x0186<<2), BOX31_PLT			)	//	
#define ISP_0186_T				_am(_ISP_0186, ISP_REG_BASE, (0x0186<<2))						//	

_regs_	UINT BITBOX				:32;	_rege_	_ISP_0187;

#define ISP_BITBOX				_bm(_ISP_0187, ISP_REG_BASE, (0x0187<<2), BITBOX			)	//	
#define ISP_0187_T				_am(_ISP_0187, ISP_REG_BASE, (0x0187<<2))						//	

_regs_	UINT _res0				: 2;
		UINT BIT_CB				:10;
		UINT BIT_CR				:10;
		UINT BITMAP_THRES		:10;	_rege_	_ISP_0188;

#define ISP_BIT_CB				_bm(_ISP_0188, ISP_REG_BASE, (0x0188<<2), BIT_CB			)	//	
#define ISP_BIT_CR				_bm(_ISP_0188, ISP_REG_BASE, (0x0188<<2), BIT_CR			)	//	
#define ISP_BITMAP_THRES		_bm(_ISP_0188, ISP_REG_BASE, (0x0188<<2), BITMAP_THRES		)	//	
#define ISP_0188_T				_am(_ISP_0188, ISP_REG_BASE, (0x0188<<2))						//	

_regs_	UINT _res0				: 8;
		UINT WDR_ADR			:24;	_rege_	_ISP_0190;

#define ISP_WDR_ADR				_bm(_ISP_0190, ISP_REG_BASE, (0x0190<<2), WDR_ADR			)	//	
#define ISP_0190_T				_am(_ISP_0190, ISP_REG_BASE, (0x0190<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_YADR_P0		:24;	_rege_	_ISP_0191;

#define ISP_ISP_YADR_P0			_bm(_ISP_0191, ISP_REG_BASE, (0x0191<<2), ISP_YADR_P0		)	//	
#define ISP_0191_T				_am(_ISP_0191, ISP_REG_BASE, (0x0191<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_CADR_P0		:24;	_rege_	_ISP_0192;

#define ISP_ISP_CADR_P0			_bm(_ISP_0192, ISP_REG_BASE, (0x0192<<2), ISP_CADR_P0		)	//	
#define ISP_0192_T				_am(_ISP_0192, ISP_REG_BASE, (0x0192<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_YADR_P1		:24;	_rege_	_ISP_0193;

#define ISP_ISP_YADR_P1			_bm(_ISP_0193, ISP_REG_BASE, (0x0193<<2), ISP_YADR_P1		)	//	
#define ISP_0193_T				_am(_ISP_0193, ISP_REG_BASE, (0x0193<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_CADR_P1		:24;	_rege_	_ISP_0194;

#define ISP_ISP_CADR_P1			_bm(_ISP_0194, ISP_REG_BASE, (0x0194<<2), ISP_CADR_P1		)	//	
#define ISP_0194_T				_am(_ISP_0194, ISP_REG_BASE, (0x0194<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_YADR_P2		:24;	_rege_	_ISP_0195;

#define ISP_ISP_YADR_P2			_bm(_ISP_0195, ISP_REG_BASE, (0x0195<<2), ISP_YADR_P2		)	//	
#define ISP_0195_T				_am(_ISP_0195, ISP_REG_BASE, (0x0195<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_CADR_P2		:24;	_rege_	_ISP_0196;

#define ISP_ISP_CADR_P2			_bm(_ISP_0196, ISP_REG_BASE, (0x0196<<2), ISP_CADR_P2		)	//	
#define ISP_0196_T				_am(_ISP_0196, ISP_REG_BASE, (0x0196<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_YADR0			:24;	_rege_	_ISP_0197;

#define ISP_ISP_YADR0			_bm(_ISP_0197, ISP_REG_BASE, (0x0197<<2), ISP_YADR0			)	//	
#define ISP_0197_T				_am(_ISP_0197, ISP_REG_BASE, (0x0197<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_CADR0			:24;	_rege_	_ISP_0198;

#define ISP_ISP_CADR0			_bm(_ISP_0198, ISP_REG_BASE, (0x0198<<2), ISP_CADR0			)	//	
#define ISP_0198_T				_am(_ISP_0198, ISP_REG_BASE, (0x0198<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_YADR1			:24;	_rege_	_ISP_0199;

#define ISP_ISP_YADR1			_bm(_ISP_0199, ISP_REG_BASE, (0x0199<<2), ISP_YADR1			)	//	
#define ISP_0199_T				_am(_ISP_0199, ISP_REG_BASE, (0x0199<<2))						//	

_regs_	UINT _res0				: 8;
		UINT ISP_CADR1			:24;	_rege_	_ISP_019A;

#define ISP_ISP_CADR1			_bm(_ISP_019A, ISP_REG_BASE, (0x019A<<2), ISP_CADR1			)	//	
#define ISP_019A_T				_am(_ISP_019A, ISP_REG_BASE, (0x019A<<2))						//	

_regs_	UINT DDR_RD_MOD			: 1;
		UINT _res0				:19;
		UINT DDR_RWDR_LTC		:12;	_rege_	_ISP_019B;

#define ISP_DDR_RD_MOD			_bm(_ISP_019B, ISP_REG_BASE, (0x019B<<2), DDR_RD_MOD		)	//	
#define ISP_DDR_RWDR_LTC		_bm(_ISP_019B, ISP_REG_BASE, (0x019B<<2), DDR_RWDR_LTC		)	//	
#define ISP_019B_T				_am(_ISP_019B, ISP_REG_BASE, (0x019B<<2))						//	

_regs_	UINT _res0				: 4;
		UINT DDR_RYC0_LTC		:12;
		UINT _res1				:16;	_rege_	_ISP_019C;

#define ISP_DDR_RYC0_LTC		_bm(_ISP_019C, ISP_REG_BASE, (0x019C<<2), DDR_RYC0_LTC		)	//	
#define ISP_019C_T				_am(_ISP_019C, ISP_REG_BASE, (0x019C<<2))						//	

_regs_	UINT _res0				: 2;
		UINT BT0_VISEL			: 2;
		UINT _res1				: 1;
		UINT BT0_ISEL			: 3;
		UINT _res2				: 1;
		UINT BT0_HWSEL			: 2;
		UINT _res3				: 1;
		UINT BT1_VISEL			: 2;
		UINT _res4				: 1;
		UINT BT1_ISEL			: 3;
		UINT _res5				: 1;
		UINT BT1_HWSEL			: 2;
		UINT _res6				:11;	_rege_	_ISP_01A0;

#define ISP_BT0_VISEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT0_VISEL			)	//	
#define ISP_BT0_ISEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT0_ISEL			)	//	
#define ISP_BT0_HWSEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT0_HWSEL			)	//	
#define ISP_BT1_VISEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT1_VISEL			)	//	
#define ISP_BT1_ISEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT1_ISEL			)	//	
#define ISP_BT1_HWSEL			_bm(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2), BT1_HWSEL			)	//	
#define ISP_01A0_T				_am(_ISP_01A0, ISP_REG_BASE, (0x01A0<<2))						//	

_regs_	UINT YC_FRC0_ON			: 1;
		UINT _res0				: 1;
		UINT ISP_ISEL			: 3;
		UINT _res1				: 1;
		UINT ISP_HWSEL			: 2;
		UINT _res2				: 2;
		UINT ISP_VISEL			: 2;
		UINT _res3				:13;
		UINT ISP_WCINV1			: 1;
		UINT ISP_WCLINE_INV1	: 1;
		UINT ISP_WCINV0			: 1;
		UINT ISP_WCLINE_INV0	: 1;
		UINT _res4				: 1;
		UINT ISP_WCINV			: 1;
		UINT ISP_WCLINE_INV		: 1;	_rege_	_ISP_01A1;

#define ISP_YC_FRC0_ON			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), YC_FRC0_ON		)	//	
#define ISP_ISP_ISEL			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_ISEL			)	//	
#define ISP_ISP_HWSEL			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_HWSEL			)	//	
#define ISP_ISP_VISEL			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_VISEL			)	//	
#define ISP_ISP_WCINV1			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCINV1		)	//	
#define ISP_ISP_WCLINE_INV1		_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCLINE_INV1	)	//	
#define ISP_ISP_WCINV0			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCINV0		)	//	
#define ISP_ISP_WCLINE_INV0		_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCLINE_INV0	)	//	
#define ISP_ISP_WCINV			_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCINV			)	//	
#define ISP_ISP_WCLINE_INV		_bm(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2), ISP_WCLINE_INV	)	//	
#define ISP_01A1_T				_am(_ISP_01A1, ISP_REG_BASE, (0x01A1<<2))						//	

_regs_	UINT _res0				:20;
		UINT BT_DS_HWI0			:12;	_rege_	_ISP_01A2;

#define ISP_BT_DS_HWI0			_bm(_ISP_01A2, ISP_REG_BASE, (0x01A2<<2), BT_DS_HWI0		)	//	
#define ISP_01A2_T				_am(_ISP_01A2, ISP_REG_BASE, (0x01A2<<2))						//	

_regs_	UINT _res0				:20;
		UINT BT_DS_HWI1			:12;	_rege_	_ISP_01A3;

#define ISP_BT_DS_HWI1			_bm(_ISP_01A3, ISP_REG_BASE, (0x01A3<<2), BT_DS_HWI1		)	//	
#define ISP_01A3_T				_am(_ISP_01A3, ISP_REG_BASE, (0x01A3<<2))						//	

_regs_	UINT _res0				:20;
		UINT BT_DS_HWI2			:12;	_rege_	_ISP_01A4;

#define ISP_BT_DS_HWI2			_bm(_ISP_01A4, ISP_REG_BASE, (0x01A4<<2), BT_DS_HWI2		)	//	
#define ISP_01A4_T				_am(_ISP_01A4, ISP_REG_BASE, (0x01A4<<2))						//	

_regs_	UINT ISP_RADR0_AON		: 1;
		UINT ISP_RON0			: 1;
		UINT ISP_CINV0			: 1;
		UINT _res0				: 5;
		UINT RYC0_FR_ON			: 1;
		UINT _res1				:23;	_rege_	_ISP_01A5;

#define ISP_ISP_RADR0_AON		_bm(_ISP_01A5, ISP_REG_BASE, (0x01A5<<2), ISP_RADR0_AON		)	//	
#define ISP_ISP_RON0			_bm(_ISP_01A5, ISP_REG_BASE, (0x01A5<<2), ISP_RON0			)	//	
#define ISP_ISP_CINV0			_bm(_ISP_01A5, ISP_REG_BASE, (0x01A5<<2), ISP_CINV0			)	//	
#define ISP_RYC0_FR_ON			_bm(_ISP_01A5, ISP_REG_BASE, (0x01A5<<2), RYC0_FR_ON		)	//	
#define ISP_01A5_T				_am(_ISP_01A5, ISP_REG_BASE, (0x01A5<<2))						//	

_regs_	UINT ISP_GO				: 1;
		UINT ISP_CGO			: 1;
		UINT ISP_GO0			: 1;
		UINT ISP_CGO0			: 1;
		UINT ISP_GO1			: 1;
		UINT ISP_CGO1			: 1;
		UINT _res0				: 2;
		UINT DDR_MEM_WADR_RST	: 1;
		UINT DDR_MEM_RADR_RST	: 1;
		UINT _res1				:22;	_rege_	_ISP_01A7;

#define ISP_ISP_GO				_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_GO			)	//	
#define ISP_ISP_CGO				_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_CGO			)	//	
#define ISP_ISP_GO0				_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_GO0			)	//	
#define ISP_ISP_CGO0			_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_CGO0			)	//	
#define ISP_ISP_GO1				_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_GO1			)	//	
#define ISP_ISP_CGO1			_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), ISP_CGO1			)	//	
#define ISP_DDR_MEM_WADR_RST	_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), DDR_MEM_WADR_RST	)	//	
#define ISP_DDR_MEM_RADR_RST	_bm(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2), DDR_MEM_RADR_RST	)	//	
#define ISP_01A7_T				_am(_ISP_01A7, ISP_REG_BASE, (0x01A7<<2))						//	

_regs_	UINT DS0_ON				: 1;
		UINT DS0_KXY_ON			: 1;
		UINT DS0_HLPF			: 2;
		UINT DS0_VLPF			: 2;
		UINT _res0				: 3;
		UINT CDS0_ISEL			: 3;
		UINT DS0_DKX			:10;
		UINT DS0_DKY			:10;	_rege_	_ISP_01A8;

#define ISP_DS0_ON				_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_ON			)	//	
#define ISP_DS0_KXY_ON			_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_KXY_ON		)	//	
#define ISP_DS0_HLPF			_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_HLPF			)	//	
#define ISP_DS0_VLPF			_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_VLPF			)	//	
#define ISP_CDS0_ISEL			_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), CDS0_ISEL			)	//	
#define ISP_DS0_DKX				_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_DKX			)	//	
#define ISP_DS0_DKY				_bm(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2), DS0_DKY			)	//	
#define ISP_01A8_T				_am(_ISP_01A8, ISP_REG_BASE, (0x01A8<<2))						//	

_regs_	UINT DS0_DKY_ST			: 6;
		UINT _res0				: 1;
		UINT DS0_HOFF			:13;
		UINT DS0_VOFF			:12;	_rege_	_ISP_01A9;

#define ISP_DS0_DKY_ST			_bm(_ISP_01A9, ISP_REG_BASE, (0x01A9<<2), DS0_DKY_ST		)	//	
#define ISP_DS0_HOFF			_bm(_ISP_01A9, ISP_REG_BASE, (0x01A9<<2), DS0_HOFF			)	//	
#define ISP_DS0_VOFF			_bm(_ISP_01A9, ISP_REG_BASE, (0x01A9<<2), DS0_VOFF			)	//	
#define ISP_01A9_T				_am(_ISP_01A9, ISP_REG_BASE, (0x01A9<<2))						//	

_regs_	UINT DS0_APTON			: 1;
		UINT _res0				: 7;
		UINT DS0_APTGA			: 8;
		UINT DS0_APTSL			: 8;
		UINT DS0_APTCL			: 8;	_rege_	_ISP_01AA;

#define ISP_DS0_APTON			_bm(_ISP_01AA, ISP_REG_BASE, (0x01AA<<2), DS0_APTON			)	//	
#define ISP_DS0_APTGA			_bm(_ISP_01AA, ISP_REG_BASE, (0x01AA<<2), DS0_APTGA			)	//	
#define ISP_DS0_APTSL			_bm(_ISP_01AA, ISP_REG_BASE, (0x01AA<<2), DS0_APTSL			)	//	
#define ISP_DS0_APTCL			_bm(_ISP_01AA, ISP_REG_BASE, (0x01AA<<2), DS0_APTCL			)	//	
#define ISP_01AA_T				_am(_ISP_01AA, ISP_REG_BASE, (0x01AA<<2))						//	

_regs_	UINT _res0				: 1;
		UINT CDS0_VISEL			: 3;
		UINT DS0_HW				:12;
		UINT _res1				: 4;
		UINT DS0_VW				:12;	_rege_	_ISP_01AB;

#define ISP_CDS0_VISEL			_bm(_ISP_01AB, ISP_REG_BASE, (0x01AB<<2), CDS0_VISEL		)	//	
#define ISP_DS0_HW				_bm(_ISP_01AB, ISP_REG_BASE, (0x01AB<<2), DS0_HW			)	//	
#define ISP_DS0_VW				_bm(_ISP_01AB, ISP_REG_BASE, (0x01AB<<2), DS0_VW			)	//	
#define ISP_01AB_T				_am(_ISP_01AB, ISP_REG_BASE, (0x01AB<<2))						//	

_regs_	UINT DS1_ON				: 1;
		UINT DS1_KXY_ON			: 1;
		UINT DS1_HLPF			: 2;
		UINT DS1_VLPF			: 2;
		UINT _res0				: 3;
		UINT CDS1_ISEL			: 3;
		UINT DS1_DKX			:10;
		UINT DS1_DKY			:10;	_rege_	_ISP_01B0;

#define ISP_DS1_ON				_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_ON			)	//	
#define ISP_DS1_KXY_ON			_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_KXY_ON		)	//	
#define ISP_DS1_HLPF			_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_HLPF			)	//	
#define ISP_DS1_VLPF			_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_VLPF			)	//	
#define ISP_CDS1_ISEL			_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), CDS1_ISEL			)	//	
#define ISP_DS1_DKX				_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_DKX			)	//	
#define ISP_DS1_DKY				_bm(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2), DS1_DKY			)	//	
#define ISP_01B0_T				_am(_ISP_01B0, ISP_REG_BASE, (0x01B0<<2))						//	

_regs_	UINT DS1_DKY_ST			: 6;
		UINT _res0				: 1;
		UINT DS1_HOFF			:13;
		UINT DS1_VOFF			:12;	_rege_	_ISP_01B1;

#define ISP_DS1_DKY_ST			_bm(_ISP_01B1, ISP_REG_BASE, (0x01B1<<2), DS1_DKY_ST		)	//	
#define ISP_DS1_HOFF			_bm(_ISP_01B1, ISP_REG_BASE, (0x01B1<<2), DS1_HOFF			)	//	
#define ISP_DS1_VOFF			_bm(_ISP_01B1, ISP_REG_BASE, (0x01B1<<2), DS1_VOFF			)	//	
#define ISP_01B1_T				_am(_ISP_01B1, ISP_REG_BASE, (0x01B1<<2))						//	

_regs_	UINT DS1_APTON			: 1;
		UINT _res0				: 7;
		UINT DS1_APTGA			: 8;
		UINT DS1_APTSL			: 8;
		UINT DS1_APTCL			: 8;	_rege_	_ISP_01B2;

#define ISP_DS1_APTON			_bm(_ISP_01B2, ISP_REG_BASE, (0x01B2<<2), DS1_APTON			)	//	
#define ISP_DS1_APTGA			_bm(_ISP_01B2, ISP_REG_BASE, (0x01B2<<2), DS1_APTGA			)	//	
#define ISP_DS1_APTSL			_bm(_ISP_01B2, ISP_REG_BASE, (0x01B2<<2), DS1_APTSL			)	//	
#define ISP_DS1_APTCL			_bm(_ISP_01B2, ISP_REG_BASE, (0x01B2<<2), DS1_APTCL			)	//	
#define ISP_01B2_T				_am(_ISP_01B2, ISP_REG_BASE, (0x01B2<<2))						//	

_regs_	UINT _res0				: 1;
		UINT CDS1_VISEL			: 3;
		UINT DS1_HW				:12;
		UINT _res1				: 4;
		UINT DS1_VW				:12;	_rege_	_ISP_01B3;

#define ISP_CDS1_VISEL			_bm(_ISP_01B3, ISP_REG_BASE, (0x01B3<<2), CDS1_VISEL		)	//	
#define ISP_DS1_HW				_bm(_ISP_01B3, ISP_REG_BASE, (0x01B3<<2), DS1_HW			)	//	
#define ISP_DS1_VW				_bm(_ISP_01B3, ISP_REG_BASE, (0x01B3<<2), DS1_VW			)	//	
#define ISP_01B3_T				_am(_ISP_01B3, ISP_REG_BASE, (0x01B3<<2))						//	

_regs_	UINT PIP0_RON			: 1;
		UINT _res0				: 1;
		UINT PIP0_ON			: 1;
		UINT PIP0_BOX_ON		: 1;
		UINT _res1				: 1;
		UINT PIP_CTOG_INV		: 1;
		UINT PIP_CSEL			: 2;
		UINT PIP0_BOX_Y			: 8;
		UINT PIP0_BOX_CB		: 8;
		UINT PIP0_BOX_CR		: 8;	_rege_	_ISP_01B5;

#define ISP_PIP0_RON			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_RON			)	//	
#define ISP_PIP0_ON				_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_ON			)	//	
#define ISP_PIP0_BOX_ON			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_BOX_ON		)	//	
#define ISP_PIP_CTOG_INV		_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP_CTOG_INV		)	//	
#define ISP_PIP_CSEL			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP_CSEL			)	//	
#define ISP_PIP0_BOX_Y			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_BOX_Y		)	//	
#define ISP_PIP0_BOX_CB			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_BOX_CB		)	//	
#define ISP_PIP0_BOX_CR			_bm(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2), PIP0_BOX_CR		)	//	
#define ISP_01B5_T				_am(_ISP_01B5, ISP_REG_BASE, (0x01B5<<2))						//	

_regs_	UINT PIP0_SUB_ISEL		: 1;
		UINT PIP_MAIN_ISEL		: 3;
		UINT _res0				: 2;
		UINT PIP0_HSP			:10;
		UINT _res1				: 6;
		UINT PIP0_VSP			:10;	_rege_	_ISP_01B6;

#define ISP_PIP0_SUB_ISEL		_bm(_ISP_01B6, ISP_REG_BASE, (0x01B6<<2), PIP0_SUB_ISEL		)	//	
#define ISP_PIP_MAIN_ISEL		_bm(_ISP_01B6, ISP_REG_BASE, (0x01B6<<2), PIP_MAIN_ISEL		)	//	
#define ISP_PIP0_HSP			_bm(_ISP_01B6, ISP_REG_BASE, (0x01B6<<2), PIP0_HSP			)	//	
#define ISP_PIP0_VSP			_bm(_ISP_01B6, ISP_REG_BASE, (0x01B6<<2), PIP0_VSP			)	//	
#define ISP_01B6_T				_am(_ISP_01B6, ISP_REG_BASE, (0x01B6<<2))						//	

_regs_	UINT _res0				: 6;
		UINT PIP0_HW			:10;
		UINT _res1				: 6;
		UINT PIP0_VW			:10;	_rege_	_ISP_01B7;

#define ISP_PIP0_HW				_bm(_ISP_01B7, ISP_REG_BASE, (0x01B7<<2), PIP0_HW			)	//	
#define ISP_PIP0_VW				_bm(_ISP_01B7, ISP_REG_BASE, (0x01B7<<2), PIP0_VW			)	//	
#define ISP_01B7_T				_am(_ISP_01B7, ISP_REG_BASE, (0x01B7<<2))						//	

_regs_	UINT _res0				:20;
		UINT PIP_DDR_LTC		:12;	_rege_	_ISP_01B8;

#define ISP_PIP_DDR_LTC			_bm(_ISP_01B8, ISP_REG_BASE, (0x01B8<<2), PIP_DDR_LTC		)	//	
#define ISP_01B8_T				_am(_ISP_01B8, ISP_REG_BASE, (0x01B8<<2))						//	

_regs_	UINT JPG_GO				: 1;
		UINT JPG_ZRL_CUT		: 1;
		UINT JPG_RST_IVL		: 2;
		UINT JPG_QP0			: 6;
		UINT JPG_QP1			: 2;
		UINT _res0				:20;	_rege_	_ISP_01BA;

#define ISP_JPG_GO				_bm(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2), JPG_GO			)	//	
#define ISP_JPG_ZRL_CUT			_bm(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2), JPG_ZRL_CUT		)	//	
#define ISP_JPG_RST_IVL			_bm(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2), JPG_RST_IVL		)	//	
#define ISP_JPG_QP0				_bm(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2), JPG_QP0			)	//	
#define ISP_JPG_QP1				_bm(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2), JPG_QP1			)	//	
#define ISP_01BA_T				_am(_ISP_01BA, ISP_REG_BASE, (0x01BA<<2))						//	

_regs_	UINT _res0				: 2;
		UINT JPG_IVSEL			: 2;
		UINT JPG_ISEL			: 4;
		UINT JPG_ADR			:24;	_rege_	_ISP_01BB;

#define ISP_JPG_IVSEL			_bm(_ISP_01BB, ISP_REG_BASE, (0x01BB<<2), JPG_IVSEL			)	//	
#define ISP_JPG_ISEL			_bm(_ISP_01BB, ISP_REG_BASE, (0x01BB<<2), JPG_ISEL			)	//	
#define ISP_JPG_ADR				_bm(_ISP_01BB, ISP_REG_BASE, (0x01BB<<2), JPG_ADR			)	//	
#define ISP_01BB_T				_am(_ISP_01BB, ISP_REG_BASE, (0x01BB<<2))						//	

_regs_	UINT JPG_ON				: 1;
		UINT _res0				: 4;
		UINT JPG_MAX			:27;	_rege_	_ISP_01BC;

#define ISP_JPG_ON				_bm(_ISP_01BC, ISP_REG_BASE, (0x01BC<<2), JPG_ON			)	//	
#define ISP_JPG_MAX				_bm(_ISP_01BC, ISP_REG_BASE, (0x01BC<<2), JPG_MAX			)	//	
#define ISP_01BC_T				_am(_ISP_01BC, ISP_REG_BASE, (0x01BC<<2))						//	

_regs_	UINT _res0				: 4;
		UINT JPG_HW				:12;
		UINT _res1				: 4;
		UINT JPG_VW				:12;	_rege_	_ISP_01BD;

#define ISP_JPG_HW				_bm(_ISP_01BD, ISP_REG_BASE, (0x01BD<<2), JPG_HW			)	//	
#define ISP_JPG_VW				_bm(_ISP_01BD, ISP_REG_BASE, (0x01BD<<2), JPG_VW			)	//	
#define ISP_01BD_T				_am(_ISP_01BD, ISP_REG_BASE, (0x01BD<<2))						//	

_regs_	UINT JPG_HBLK_SEL		: 1;
		UINT _res0				: 1;
		UINT JPG_HBLK_CYC		:14;
		UINT _res1				: 6;
		UINT JPG_VOFS			: 6;
		UINT _res2				: 4;	_rege_	_ISP_01BE;

#define ISP_JPG_HBLK_SEL		_bm(_ISP_01BE, ISP_REG_BASE, (0x01BE<<2), JPG_HBLK_SEL		)	//	
#define ISP_JPG_HBLK_CYC		_bm(_ISP_01BE, ISP_REG_BASE, (0x01BE<<2), JPG_HBLK_CYC		)	//	
#define ISP_JPG_VOFS			_bm(_ISP_01BE, ISP_REG_BASE, (0x01BE<<2), JPG_VOFS			)	//	
#define ISP_01BE_T				_am(_ISP_01BE, ISP_REG_BASE, (0x01BE<<2))						//	

_regs_	UINT _res0				:26;
		UINT JVLOCK_POS			: 6;	_rege_	_ISP_01BF;

#define ISP_JVLOCK_POS			_bm(_ISP_01BF, ISP_REG_BASE, (0x01BF<<2), JVLOCK_POS		)	//	
#define ISP_01BF_T				_am(_ISP_01BF, ISP_REG_BASE, (0x01BF<<2))						//	

_regs_	UINT _res0				:20;
		UINT MYCW_HWO			:12;	_rege_	_ISP_01C0;

#define ISP_MYCW_HWO			_bm(_ISP_01C0, ISP_REG_BASE, (0x01C0<<2), MYCW_HWO			)	//	
#define ISP_01C0_T				_am(_ISP_01C0, ISP_REG_BASE, (0x01C0<<2))						//	

_regs_	UINT VIRQI_EN1			: 1;
		UINT VIRQW_EN1			: 1;
		UINT VIRQO_EN1			: 1;
		UINT JPGIRQ_EN			: 1;
		UINT VIRQW2_EN1			: 1;
		UINT _res0				:22;
		UINT CLI_VLOCKW2_1		: 1;
		UINT CLI_JPG			: 1;
		UINT CLI_VLOCKI1		: 1;
		UINT CLI_VLOCKW1		: 1;
		UINT CLI_VLOCKO1		: 1;	_rege_	_ISP_01C1;

#define ISP_VIRQI_EN1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), VIRQI_EN1			)	//	
#define ISP_VIRQW_EN1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), VIRQW_EN1			)	//	
#define ISP_VIRQO_EN1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), VIRQO_EN1			)	//	
#define ISP_JPGIRQ_EN			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), JPGIRQ_EN			)	//	
#define ISP_VIRQW2_EN1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), VIRQW2_EN1		)	//	
#define ISP_CLI_VLOCKW2_1		_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), CLI_VLOCKW2_1		)	//	
#define ISP_CLI_JPG				_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), CLI_JPG			)	//	
#define ISP_CLI_VLOCKI1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), CLI_VLOCKI1		)	//	
#define ISP_CLI_VLOCKW1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), CLI_VLOCKW1		)	//	
#define ISP_CLI_VLOCKO1			_bm(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2), CLI_VLOCKO1		)	//	
#define ISP_01C1_T				_am(_ISP_01C1, ISP_REG_BASE, (0x01C1<<2))						//	

_regs_	UINT _res0				:17;
		UINT IP_UIRQ0_CK_SEL	: 3;
		UINT _res1				: 1;
		UINT IP_UIRQ1_CK_SEL	: 3;
		UINT _res2				: 8;	_rege_	_ISP_01C2;

#define ISP_IP_UIRQ0_CK_SEL		_bm(_ISP_01C2, ISP_REG_BASE, (0x01C2<<2), IP_UIRQ0_CK_SEL	)	//	
#define ISP_IP_UIRQ1_CK_SEL		_bm(_ISP_01C2, ISP_REG_BASE, (0x01C2<<2), IP_UIRQ1_CK_SEL	)	//	
#define ISP_01C2_T				_am(_ISP_01C2, ISP_REG_BASE, (0x01C2<<2))						//	

_regs_	UINT IP_UIRQ0_CNT_SEL	: 2;
		UINT IP_CLIUIRQ0		: 1;
		UINT IP_UIRQ0_EN		: 1;
		UINT IP_UIRQ_VPOS0		:12;
		UINT _res0				: 4;
		UINT IP_UIRQ_HPOS0		:12;	_rege_	_ISP_01C3;

#define ISP_IP_UIRQ0_CNT_SEL	_bm(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2), IP_UIRQ0_CNT_SEL	)	//	
#define ISP_IP_CLIUIRQ0			_bm(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2), IP_CLIUIRQ0		)	//	
#define ISP_IP_UIRQ0_EN			_bm(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2), IP_UIRQ0_EN		)	//	
#define ISP_IP_UIRQ_VPOS0		_bm(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2), IP_UIRQ_VPOS0		)	//	
#define ISP_IP_UIRQ_HPOS0		_bm(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2), IP_UIRQ_HPOS0		)	//	
#define ISP_01C3_T				_am(_ISP_01C3, ISP_REG_BASE, (0x01C3<<2))						//	

_regs_	UINT IP_UIRQ1_CNT_SEL	: 2;
		UINT IP_CLIUIRQ1		: 1;
		UINT IP_UIRQ1_EN		: 1;
		UINT IP_UIRQ_VPOS1		:12;
		UINT _res0				: 4;
		UINT IP_UIRQ_HPOS1		:12;	_rege_	_ISP_01C4;

#define ISP_IP_UIRQ1_CNT_SEL	_bm(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2), IP_UIRQ1_CNT_SEL	)	//	
#define ISP_IP_CLIUIRQ1			_bm(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2), IP_CLIUIRQ1		)	//	
#define ISP_IP_UIRQ1_EN			_bm(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2), IP_UIRQ1_EN		)	//	
#define ISP_IP_UIRQ_VPOS1		_bm(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2), IP_UIRQ_VPOS1		)	//	
#define ISP_IP_UIRQ_HPOS1		_bm(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2), IP_UIRQ_HPOS1		)	//	
#define ISP_01C4_T				_am(_ISP_01C4, ISP_REG_BASE, (0x01C4<<2))						//	

_regs_	UINT _res0				: 1;
		UINT ITU_SCON_EX0		: 1;
		UINT ITU_ORDR_EX0		: 1;
		UINT _res1				:12;
		UINT LNUM_EXT_ON		: 1;
		UINT _res2				: 1;
		UINT PLGL_TOG_ON		: 1;
		UINT CTOG_INV			: 1;
		UINT _res3				: 1;
		UINT ITU_LCON			: 1;
		UINT ITU_VID			: 1;
		UINT ITU_FID			: 1;
		UINT SDI_YCCH0			: 1;
		UINT _res4				: 6;
		UINT BT20_OSEL0			: 2;	_rege_	_ISP_01C6;

#define ISP_ITU_SCON_EX0		_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), ITU_SCON_EX0		)	//	
#define ISP_ITU_ORDR_EX0		_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), ITU_ORDR_EX0		)	//	
#define ISP_LNUM_EXT_ON			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), LNUM_EXT_ON		)	//	
#define ISP_PLGL_TOG_ON			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), PLGL_TOG_ON		)	//	
#define ISP_CTOG_INV			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), CTOG_INV			)	//	
#define ISP_ITU_LCON			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), ITU_LCON			)	//	
#define ISP_ITU_VID				_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), ITU_VID			)	//	
#define ISP_ITU_FID				_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), ITU_FID			)	//	
#define ISP_SDI_YCCH0			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), SDI_YCCH0			)	//	
#define ISP_BT20_OSEL0			_bm(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2), BT20_OSEL0		)	//	
#define ISP_01C6_T				_am(_ISP_01C6, ISP_REG_BASE, (0x01C6<<2))						//	

_regs_	UINT LAST_YCLIP			: 8;
		UINT LAST_YSLIC			: 8;
		UINT LAST_YOFFS			: 8;
		UINT _res0				: 8;	_rege_	_ISP_01C7;

#define ISP_LAST_YCLIP			_bm(_ISP_01C7, ISP_REG_BASE, (0x01C7<<2), LAST_YCLIP		)	//	
#define ISP_LAST_YSLIC			_bm(_ISP_01C7, ISP_REG_BASE, (0x01C7<<2), LAST_YSLIC		)	//	
#define ISP_LAST_YOFFS			_bm(_ISP_01C7, ISP_REG_BASE, (0x01C7<<2), LAST_YOFFS		)	//	
#define ISP_01C7_T				_am(_ISP_01C7, ISP_REG_BASE, (0x01C7<<2))						//	

_regs_	UINT LAST_CGAIN			: 8;
		UINT LAST_YGAIN			: 8;
		UINT LAST_CCLIP			: 8;
		UINT LAST_CSLIC			: 8;	_rege_	_ISP_01C8;

#define ISP_LAST_CGAIN			_bm(_ISP_01C8, ISP_REG_BASE, (0x01C8<<2), LAST_CGAIN		)	//	
#define ISP_LAST_YGAIN			_bm(_ISP_01C8, ISP_REG_BASE, (0x01C8<<2), LAST_YGAIN		)	//	
#define ISP_LAST_CCLIP			_bm(_ISP_01C8, ISP_REG_BASE, (0x01C8<<2), LAST_CCLIP		)	//	
#define ISP_LAST_CSLIC			_bm(_ISP_01C8, ISP_REG_BASE, (0x01C8<<2), LAST_CSLIC		)	//	
#define ISP_01C8_T				_am(_ISP_01C8, ISP_REG_BASE, (0x01C8<<2))						//	

_regs_	UINT PV_SEL				: 2;
		UINT _res0				: 1;
		UINT DCKO0_INV			: 1;
		UINT DCKO0_ON			: 1;
		UINT DCKO0_SEL			: 3;
		UINT _res1				:20;
		UINT DCKO0_DLY			: 4;	_rege_	_ISP_01C9;

#define ISP_PV_SEL				_bm(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2), PV_SEL			)	//	
#define ISP_DCKO0_INV			_bm(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2), DCKO0_INV			)	//	
#define ISP_DCKO0_ON			_bm(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2), DCKO0_ON			)	//	
#define ISP_DCKO0_SEL			_bm(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2), DCKO0_SEL			)	//	
#define ISP_DCKO0_DLY			_bm(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2), DCKO0_DLY			)	//	
#define ISP_01C9_T				_am(_ISP_01C9, ISP_REG_BASE, (0x01C9<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AF_HSP				:12;
		UINT _res1				: 4;
		UINT AF_HEP				:12;	_rege_	_ISP_01CA;

#define ISP_AF_HSP				_bm(_ISP_01CA, ISP_REG_BASE, (0x01CA<<2), AF_HSP			)	//	
#define ISP_AF_HEP				_bm(_ISP_01CA, ISP_REG_BASE, (0x01CA<<2), AF_HEP			)	//	
#define ISP_01CA_T				_am(_ISP_01CA, ISP_REG_BASE, (0x01CA<<2))						//	

_regs_	UINT _res0				: 4;
		UINT AF_VSP				:12;
		UINT _res1				: 4;
		UINT AF_VEP				:12;	_rege_	_ISP_01CB;

#define ISP_AF_VSP				_bm(_ISP_01CB, ISP_REG_BASE, (0x01CB<<2), AF_VSP			)	//	
#define ISP_AF_VEP				_bm(_ISP_01CB, ISP_REG_BASE, (0x01CB<<2), AF_VEP			)	//	
#define ISP_01CB_T				_am(_ISP_01CB, ISP_REG_BASE, (0x01CB<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AF_EDGE_TH1		:10;
		UINT _res1				: 6;
		UINT AF_EDGE_TH2		:10;	_rege_	_ISP_01CC;

#define ISP_AF_EDGE_TH1			_bm(_ISP_01CC, ISP_REG_BASE, (0x01CC<<2), AF_EDGE_TH1		)	//	
#define ISP_AF_EDGE_TH2			_bm(_ISP_01CC, ISP_REG_BASE, (0x01CC<<2), AF_EDGE_TH2		)	//	
#define ISP_01CC_T				_am(_ISP_01CC, ISP_REG_BASE, (0x01CC<<2))						//	

_regs_	UINT _res0				: 6;
		UINT AF_CLIP			:10;
		UINT _res1				: 6;
		UINT AF_SLICE			:10;	_rege_	_ISP_01CE;

#define ISP_AF_CLIP				_bm(_ISP_01CE, ISP_REG_BASE, (0x01CE<<2), AF_CLIP			)	//	
#define ISP_AF_SLICE			_bm(_ISP_01CE, ISP_REG_BASE, (0x01CE<<2), AF_SLICE			)	//	
#define ISP_01CE_T				_am(_ISP_01CE, ISP_REG_BASE, (0x01CE<<2))						//	

_regs_	UINT AF_PD				: 1;
		UINT _res0				: 2;
		UINT AF_CLIP_ON			: 1;
		UINT AF_WIN_SEL			: 2;
		UINT AF_CWD_HSEL		: 2;
		UINT AF_CWD_VSEL		: 2;
		UINT _res1				: 3;
		UINT AF_CWD_WGT			: 3;
		UINT _res2				:16;	_rege_	_ISP_01CF;

#define ISP_AF_PD				_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_PD				)	//	
#define ISP_AF_CLIP_ON			_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_CLIP_ON		)	//	
#define ISP_AF_WIN_SEL			_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_WIN_SEL		)	//	
#define ISP_AF_CWD_HSEL			_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_CWD_HSEL		)	//	
#define ISP_AF_CWD_VSEL			_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_CWD_VSEL		)	//	
#define ISP_AF_CWD_WGT			_bm(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2), AF_CWD_WGT		)	//	
#define ISP_01CF_T				_am(_ISP_01CF, ISP_REG_BASE, (0x01CF<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HLOCKO_POS			:12;
		UINT _res1				: 4;
		UINT VLOCKO_POS			:12;	_rege_	_ISP_01D0;

#define ISP_HLOCKO_POS			_bm(_ISP_01D0, ISP_REG_BASE, (0x01D0<<2), HLOCKO_POS		)	//	
#define ISP_VLOCKO_POS			_bm(_ISP_01D0, ISP_REG_BASE, (0x01D0<<2), VLOCKO_POS		)	//	
#define ISP_01D0_T				_am(_ISP_01D0, ISP_REG_BASE, (0x01D0<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HSPC				:12;
		UINT _res1				: 4;
		UINT VSPC				:12;	_rege_	_ISP_01D1;

#define ISP_HSPC				_bm(_ISP_01D1, ISP_REG_BASE, (0x01D1<<2), HSPC				)	//	
#define ISP_VSPC				_bm(_ISP_01D1, ISP_REG_BASE, (0x01D1<<2), VSPC				)	//	
#define ISP_01D1_T				_am(_ISP_01D1, ISP_REG_BASE, (0x01D1<<2))						//	

_regs_	UINT _res0				: 4;
		UINT HWC				:12;
		UINT _res1				: 4;
		UINT VWC				:12;	_rege_	_ISP_01D2;

#define ISP_HWC					_bm(_ISP_01D2, ISP_REG_BASE, (0x01D2<<2), HWC				)	//	
#define ISP_VWC					_bm(_ISP_01D2, ISP_REG_BASE, (0x01D2<<2), VWC				)	//	
#define ISP_01D2_T				_am(_ISP_01D2, ISP_REG_BASE, (0x01D2<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE1_SUM1_LOCK		:31;	_rege_	_ISP_0200;

#define ISP_rAE1_SUM1_LOCK		_bm(_ISP_0200, ISP_REG_BASE, (0x0200<<2), rAE1_SUM1_LOCK	)	//	
#define ISP_0200_T				_am(_ISP_0200, ISP_REG_BASE, (0x0200<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE1_SUM2_LOCK		:31;	_rege_	_ISP_0201;

#define ISP_rAE1_SUM2_LOCK		_bm(_ISP_0201, ISP_REG_BASE, (0x0201<<2), rAE1_SUM2_LOCK	)	//	
#define ISP_0201_T				_am(_ISP_0201, ISP_REG_BASE, (0x0201<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE2_SUM1_LOCK		:31;	_rege_	_ISP_0202;

#define ISP_rAE2_SUM1_LOCK		_bm(_ISP_0202, ISP_REG_BASE, (0x0202<<2), rAE2_SUM1_LOCK	)	//	
#define ISP_0202_T				_am(_ISP_0202, ISP_REG_BASE, (0x0202<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE2_SUM2_LOCK		:31;	_rege_	_ISP_0203;

#define ISP_rAE2_SUM2_LOCK		_bm(_ISP_0203, ISP_REG_BASE, (0x0203<<2), rAE2_SUM2_LOCK	)	//	
#define ISP_0203_T				_am(_ISP_0203, ISP_REG_BASE, (0x0203<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE3_SUM1_LOCK		:31;	_rege_	_ISP_0204;

#define ISP_rAE3_SUM1_LOCK		_bm(_ISP_0204, ISP_REG_BASE, (0x0204<<2), rAE3_SUM1_LOCK	)	//	
#define ISP_0204_T				_am(_ISP_0204, ISP_REG_BASE, (0x0204<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE3_SUM2_LOCK		:31;	_rege_	_ISP_0205;

#define ISP_rAE3_SUM2_LOCK		_bm(_ISP_0205, ISP_REG_BASE, (0x0205<<2), rAE3_SUM2_LOCK	)	//	
#define ISP_0205_T				_am(_ISP_0205, ISP_REG_BASE, (0x0205<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE4_SUM1_LOCK		:31;	_rege_	_ISP_0206;

#define ISP_rAE4_SUM1_LOCK		_bm(_ISP_0206, ISP_REG_BASE, (0x0206<<2), rAE4_SUM1_LOCK	)	//	
#define ISP_0206_T				_am(_ISP_0206, ISP_REG_BASE, (0x0206<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE4_SUM2_LOCK		:31;	_rege_	_ISP_0207;

#define ISP_rAE4_SUM2_LOCK		_bm(_ISP_0207, ISP_REG_BASE, (0x0207<<2), rAE4_SUM2_LOCK	)	//	
#define ISP_0207_T				_am(_ISP_0207, ISP_REG_BASE, (0x0207<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE1_CLCNT_LOCK	:21;	_rege_	_ISP_0208;

#define ISP_rAE1_CLCNT_LOCK		_bm(_ISP_0208, ISP_REG_BASE, (0x0208<<2), rAE1_CLCNT_LOCK	)	//	
#define ISP_0208_T				_am(_ISP_0208, ISP_REG_BASE, (0x0208<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE2_CLCNT_LOCK	:21;	_rege_	_ISP_0209;

#define ISP_rAE2_CLCNT_LOCK		_bm(_ISP_0209, ISP_REG_BASE, (0x0209<<2), rAE2_CLCNT_LOCK	)	//	
#define ISP_0209_T				_am(_ISP_0209, ISP_REG_BASE, (0x0209<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE3_CLCNT_LOCK	:21;	_rege_	_ISP_020A;

#define ISP_rAE3_CLCNT_LOCK		_bm(_ISP_020A, ISP_REG_BASE, (0x020A<<2), rAE3_CLCNT_LOCK	)	//	
#define ISP_020A_T				_am(_ISP_020A, ISP_REG_BASE, (0x020A<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE4_CLCNT_LOCK	:21;	_rege_	_ISP_020B;

#define ISP_rAE4_CLCNT_LOCK		_bm(_ISP_020B, ISP_REG_BASE, (0x020B<<2), rAE4_CLCNT_LOCK	)	//	
#define ISP_020B_T				_am(_ISP_020B, ISP_REG_BASE, (0x020B<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE1_SLCNT_LOCK	:21;	_rege_	_ISP_020C;

#define ISP_rAE1_SLCNT_LOCK		_bm(_ISP_020C, ISP_REG_BASE, (0x020C<<2), rAE1_SLCNT_LOCK	)	//	
#define ISP_020C_T				_am(_ISP_020C, ISP_REG_BASE, (0x020C<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE2_SLCNT_LOCK	:21;	_rege_	_ISP_020D;

#define ISP_rAE2_SLCNT_LOCK		_bm(_ISP_020D, ISP_REG_BASE, (0x020D<<2), rAE2_SLCNT_LOCK	)	//	
#define ISP_020D_T				_am(_ISP_020D, ISP_REG_BASE, (0x020D<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE3_SLCNT_LOCK	:21;	_rege_	_ISP_020E;

#define ISP_rAE3_SLCNT_LOCK		_bm(_ISP_020E, ISP_REG_BASE, (0x020E<<2), rAE3_SLCNT_LOCK	)	//	
#define ISP_020E_T				_am(_ISP_020E, ISP_REG_BASE, (0x020E<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAE4_SLCNT_LOCK	:21;	_rege_	_ISP_020F;

#define ISP_rAE4_SLCNT_LOCK		_bm(_ISP_020F, ISP_REG_BASE, (0x020F<<2), rAE4_SLCNT_LOCK	)	//	
#define ISP_020F_T				_am(_ISP_020F, ISP_REG_BASE, (0x020F<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO0_LOCK		:21;	_rege_	_ISP_0210;

#define ISP_rHISTO0_LOCK		_bm(_ISP_0210, ISP_REG_BASE, (0x0210<<2), rHISTO0_LOCK		)	//	
#define ISP_0210_T				_am(_ISP_0210, ISP_REG_BASE, (0x0210<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO1_LOCK		:21;	_rege_	_ISP_0211;

#define ISP_rHISTO1_LOCK		_bm(_ISP_0211, ISP_REG_BASE, (0x0211<<2), rHISTO1_LOCK		)	//	
#define ISP_0211_T				_am(_ISP_0211, ISP_REG_BASE, (0x0211<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO2_LOCK		:21;	_rege_	_ISP_0212;

#define ISP_rHISTO2_LOCK		_bm(_ISP_0212, ISP_REG_BASE, (0x0212<<2), rHISTO2_LOCK		)	//	
#define ISP_0212_T				_am(_ISP_0212, ISP_REG_BASE, (0x0212<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO3_LOCK		:21;	_rege_	_ISP_0213;

#define ISP_rHISTO3_LOCK		_bm(_ISP_0213, ISP_REG_BASE, (0x0213<<2), rHISTO3_LOCK		)	//	
#define ISP_0213_T				_am(_ISP_0213, ISP_REG_BASE, (0x0213<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO4_LOCK		:21;	_rege_	_ISP_0214;

#define ISP_rHISTO4_LOCK		_bm(_ISP_0214, ISP_REG_BASE, (0x0214<<2), rHISTO4_LOCK		)	//	
#define ISP_0214_T				_am(_ISP_0214, ISP_REG_BASE, (0x0214<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO5_LOCK		:21;	_rege_	_ISP_0215;

#define ISP_rHISTO5_LOCK		_bm(_ISP_0215, ISP_REG_BASE, (0x0215<<2), rHISTO5_LOCK		)	//	
#define ISP_0215_T				_am(_ISP_0215, ISP_REG_BASE, (0x0215<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO6_LOCK		:21;	_rege_	_ISP_0216;

#define ISP_rHISTO6_LOCK		_bm(_ISP_0216, ISP_REG_BASE, (0x0216<<2), rHISTO6_LOCK		)	//	
#define ISP_0216_T				_am(_ISP_0216, ISP_REG_BASE, (0x0216<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO7_LOCK		:21;	_rege_	_ISP_0217;

#define ISP_rHISTO7_LOCK		_bm(_ISP_0217, ISP_REG_BASE, (0x0217<<2), rHISTO7_LOCK		)	//	
#define ISP_0217_T				_am(_ISP_0217, ISP_REG_BASE, (0x0217<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO8_LOCK		:21;	_rege_	_ISP_0218;

#define ISP_rHISTO8_LOCK		_bm(_ISP_0218, ISP_REG_BASE, (0x0218<<2), rHISTO8_LOCK		)	//	
#define ISP_0218_T				_am(_ISP_0218, ISP_REG_BASE, (0x0218<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO9_LOCK		:21;	_rege_	_ISP_0219;

#define ISP_rHISTO9_LOCK		_bm(_ISP_0219, ISP_REG_BASE, (0x0219<<2), rHISTO9_LOCK		)	//	
#define ISP_0219_T				_am(_ISP_0219, ISP_REG_BASE, (0x0219<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO10_LOCK		:21;	_rege_	_ISP_021A;

#define ISP_rHISTO10_LOCK		_bm(_ISP_021A, ISP_REG_BASE, (0x021A<<2), rHISTO10_LOCK		)	//	
#define ISP_021A_T				_am(_ISP_021A, ISP_REG_BASE, (0x021A<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO11_LOCK		:21;	_rege_	_ISP_021B;

#define ISP_rHISTO11_LOCK		_bm(_ISP_021B, ISP_REG_BASE, (0x021B<<2), rHISTO11_LOCK		)	//	
#define ISP_021B_T				_am(_ISP_021B, ISP_REG_BASE, (0x021B<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO12_LOCK		:21;	_rege_	_ISP_021C;

#define ISP_rHISTO12_LOCK		_bm(_ISP_021C, ISP_REG_BASE, (0x021C<<2), rHISTO12_LOCK		)	//	
#define ISP_021C_T				_am(_ISP_021C, ISP_REG_BASE, (0x021C<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO13_LOCK		:21;	_rege_	_ISP_021D;

#define ISP_rHISTO13_LOCK		_bm(_ISP_021D, ISP_REG_BASE, (0x021D<<2), rHISTO13_LOCK		)	//	
#define ISP_021D_T				_am(_ISP_021D, ISP_REG_BASE, (0x021D<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO14_LOCK		:21;	_rege_	_ISP_021E;

#define ISP_rHISTO14_LOCK		_bm(_ISP_021E, ISP_REG_BASE, (0x021E<<2), rHISTO14_LOCK		)	//	
#define ISP_021E_T				_am(_ISP_021E, ISP_REG_BASE, (0x021E<<2))						//	

_regs_	UINT _res0				:11;
		UINT rHISTO15_LOCK		:21;	_rege_	_ISP_021F;

#define ISP_rHISTO15_LOCK		_bm(_ISP_021F, ISP_REG_BASE, (0x021F<<2), rHISTO15_LOCK		)	//	
#define ISP_021F_T				_am(_ISP_021F, ISP_REG_BASE, (0x021F<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAWBL_CCR_SUM_LOCK	:31;	_rege_	_ISP_0220;

#define ISP_rAWBL_CCR_SUM_LOCK	_bm(_ISP_0220, ISP_REG_BASE, (0x0220<<2), rAWBL_CCR_SUM_LOCK	)	//	
#define ISP_0220_T				_am(_ISP_0220, ISP_REG_BASE, (0x0220<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAWBL_CCG_SUM_LOCK	:31;	_rege_	_ISP_0221;

#define ISP_rAWBL_CCG_SUM_LOCK	_bm(_ISP_0221, ISP_REG_BASE, (0x0221<<2), rAWBL_CCG_SUM_LOCK	)	//	
#define ISP_0221_T				_am(_ISP_0221, ISP_REG_BASE, (0x0221<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAWBL_CCB_SUM_LOCK	:31;	_rege_	_ISP_0222;

#define ISP_rAWBL_CCB_SUM_LOCK	_bm(_ISP_0222, ISP_REG_BASE, (0x0222<<2), rAWBL_CCB_SUM_LOCK	)	//	
#define ISP_0222_T				_am(_ISP_0222, ISP_REG_BASE, (0x0222<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAWBL_CCT_SUM_LOCK	:21;	_rege_	_ISP_0223;

#define ISP_rAWBL_CCT_SUM_LOCK	_bm(_ISP_0223, ISP_REG_BASE, (0x0223<<2), rAWBL_CCT_SUM_LOCK	)	//	
#define ISP_0223_T				_am(_ISP_0223, ISP_REG_BASE, (0x0223<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAWBL_CY_SUM_LOCK	:31;	_rege_	_ISP_0224;

#define ISP_rAWBL_CY_SUM_LOCK	_bm(_ISP_0224, ISP_REG_BASE, (0x0224<<2), rAWBL_CY_SUM_LOCK	)	//	
#define ISP_0224_T				_am(_ISP_0224, ISP_REG_BASE, (0x0224<<2))						//	

_regs_	UINT rAWBL_CBDB_SUM_LOCK	:32;	_rege_	_ISP_0225;

#define ISP_rAWBL_CBDB_SUM_LOCK	_bm(_ISP_0225, ISP_REG_BASE, (0x0225<<2), rAWBL_CBDB_SUM_LOCK	)	//	
#define ISP_0225_T				_am(_ISP_0225, ISP_REG_BASE, (0x0225<<2))						//	

_regs_	UINT rAWBL_CRDR_SUM_LOCK	:32;	_rege_	_ISP_0226;

#define ISP_rAWBL_CRDR_SUM_LOCK	_bm(_ISP_0226, ISP_REG_BASE, (0x0226<<2), rAWBL_CRDR_SUM_LOCK	)	//	
#define ISP_0226_T				_am(_ISP_0226, ISP_REG_BASE, (0x0226<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE1_SUM0_LOCK		:31;	_rege_	_ISP_0227;

#define ISP_rAE1_SUM0_LOCK		_bm(_ISP_0227, ISP_REG_BASE, (0x0227<<2), rAE1_SUM0_LOCK	)	//	
#define ISP_0227_T				_am(_ISP_0227, ISP_REG_BASE, (0x0227<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE2_SUM0_LOCK		:31;	_rege_	_ISP_0228;

#define ISP_rAE2_SUM0_LOCK		_bm(_ISP_0228, ISP_REG_BASE, (0x0228<<2), rAE2_SUM0_LOCK	)	//	
#define ISP_0228_T				_am(_ISP_0228, ISP_REG_BASE, (0x0228<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE3_SUM0_LOCK		:31;	_rege_	_ISP_0229;

#define ISP_rAE3_SUM0_LOCK		_bm(_ISP_0229, ISP_REG_BASE, (0x0229<<2), rAE3_SUM0_LOCK	)	//	
#define ISP_0229_T				_am(_ISP_0229, ISP_REG_BASE, (0x0229<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAE4_SUM0_LOCK		:31;	_rege_	_ISP_022A;

#define ISP_rAE4_SUM0_LOCK		_bm(_ISP_022A, ISP_REG_BASE, (0x022A<<2), rAE4_SUM0_LOCK	)	//	
#define ISP_022A_T				_am(_ISP_022A, ISP_REG_BASE, (0x022A<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_SUM1_LOCK		:31;	_rege_	_ISP_0230;

#define ISP_rAF_SUM1_LOCK		_bm(_ISP_0230, ISP_REG_BASE, (0x0230<<2), rAF_SUM1_LOCK		)	//	
#define ISP_0230_T				_am(_ISP_0230, ISP_REG_BASE, (0x0230<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_SUM2_LOCK		:31;	_rege_	_ISP_0231;

#define ISP_rAF_SUM2_LOCK		_bm(_ISP_0231, ISP_REG_BASE, (0x0231<<2), rAF_SUM2_LOCK		)	//	
#define ISP_0231_T				_am(_ISP_0231, ISP_REG_BASE, (0x0231<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_CSUM1_LOCK		:31;	_rege_	_ISP_0232;

#define ISP_rAF_CSUM1_LOCK		_bm(_ISP_0232, ISP_REG_BASE, (0x0232<<2), rAF_CSUM1_LOCK	)	//	
#define ISP_0232_T				_am(_ISP_0232, ISP_REG_BASE, (0x0232<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_CSUM2_LOCK		:31;	_rege_	_ISP_0233;

#define ISP_rAF_CSUM2_LOCK		_bm(_ISP_0233, ISP_REG_BASE, (0x0233<<2), rAF_CSUM2_LOCK	)	//	
#define ISP_0233_T				_am(_ISP_0233, ISP_REG_BASE, (0x0233<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_YSUM1_LOCK		:31;	_rege_	_ISP_0234;

#define ISP_rAF_YSUM1_LOCK		_bm(_ISP_0234, ISP_REG_BASE, (0x0234<<2), rAF_YSUM1_LOCK	)	//	
#define ISP_0234_T				_am(_ISP_0234, ISP_REG_BASE, (0x0234<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rAF_YSUM2_LOCK		:31;	_rege_	_ISP_0235;

#define ISP_rAF_YSUM2_LOCK		_bm(_ISP_0235, ISP_REG_BASE, (0x0235<<2), rAF_YSUM2_LOCK	)	//	
#define ISP_0235_T				_am(_ISP_0235, ISP_REG_BASE, (0x0235<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAF_CLCNT_LOCK		:21;	_rege_	_ISP_0236;

#define ISP_rAF_CLCNT_LOCK		_bm(_ISP_0236, ISP_REG_BASE, (0x0236<<2), rAF_CLCNT_LOCK	)	//	
#define ISP_0236_T				_am(_ISP_0236, ISP_REG_BASE, (0x0236<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAF_SLCNT_LOCK		:21;	_rege_	_ISP_0237;

#define ISP_rAF_SLCNT_LOCK		_bm(_ISP_0237, ISP_REG_BASE, (0x0237<<2), rAF_SLCNT_LOCK	)	//	
#define ISP_0237_T				_am(_ISP_0237, ISP_REG_BASE, (0x0237<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAF_EGCNT1_LOCK	:21;	_rege_	_ISP_0238;

#define ISP_rAF_EGCNT1_LOCK		_bm(_ISP_0238, ISP_REG_BASE, (0x0238<<2), rAF_EGCNT1_LOCK	)	//	
#define ISP_0238_T				_am(_ISP_0238, ISP_REG_BASE, (0x0238<<2))						//	

_regs_	UINT _res0				:11;
		UINT rAF_EGCNT2_LOCK	:21;	_rege_	_ISP_0239;

#define ISP_rAF_EGCNT2_LOCK		_bm(_ISP_0239, ISP_REG_BASE, (0x0239<<2), rAF_EGCNT2_LOCK	)	//	
#define ISP_0239_T				_am(_ISP_0239, ISP_REG_BASE, (0x0239<<2))						//	

_regs_	UINT _res0				:20;
		UINT rCH1_HDET			:12;	_rege_	_ISP_0240;

#define ISP_rCH1_HDET			_bm(_ISP_0240, ISP_REG_BASE, (0x0240<<2), rCH1_HDET			)	//	
#define ISP_0240_T				_am(_ISP_0240, ISP_REG_BASE, (0x0240<<2))						//	

_regs_	UINT _res0				:20;
		UINT rCH2_HDET			:12;	_rege_	_ISP_0241;

#define ISP_rCH2_HDET			_bm(_ISP_0241, ISP_REG_BASE, (0x0241<<2), rCH2_HDET			)	//	
#define ISP_0241_T				_am(_ISP_0241, ISP_REG_BASE, (0x0241<<2))						//	

_regs_	UINT _res0				:20;
		UINT rCH3_HDET			:12;	_rege_	_ISP_0242;

#define ISP_rCH3_HDET			_bm(_ISP_0242, ISP_REG_BASE, (0x0242<<2), rCH3_HDET			)	//	
#define ISP_0242_T				_am(_ISP_0242, ISP_REG_BASE, (0x0242<<2))						//	

_regs_	UINT _res0				:20;
		UINT rCH4_HDET			:12;	_rege_	_ISP_0243;

#define ISP_rCH4_HDET			_bm(_ISP_0243, ISP_REG_BASE, (0x0243<<2), rCH4_HDET			)	//	
#define ISP_0243_T				_am(_ISP_0243, ISP_REG_BASE, (0x0243<<2))						//	

_regs_	UINT _res0				:28;
		UINT rSYNC4_STATUS		: 1;
		UINT rSYNC3_STATUS		: 1;
		UINT rSYNC2_STATUS		: 1;
		UINT rSYNC1_STATUS		: 1;	_rege_	_ISP_0244;

#define ISP_rSYNC4_STATUS		_bm(_ISP_0244, ISP_REG_BASE, (0x0244<<2), rSYNC4_STATUS		)	//	
#define ISP_rSYNC3_STATUS		_bm(_ISP_0244, ISP_REG_BASE, (0x0244<<2), rSYNC3_STATUS		)	//	
#define ISP_rSYNC2_STATUS		_bm(_ISP_0244, ISP_REG_BASE, (0x0244<<2), rSYNC2_STATUS		)	//	
#define ISP_rSYNC1_STATUS		_bm(_ISP_0244, ISP_REG_BASE, (0x0244<<2), rSYNC1_STATUS		)	//	
#define ISP_0244_T				_am(_ISP_0244, ISP_REG_BASE, (0x0244<<2))						//	

_regs_	UINT _res0				:28;
		UINT rISP_WPAGE			: 2;
		UINT rISP_WPAGE1		: 2;	_rege_	_ISP_0245;

#define ISP_rISP_WPAGE			_bm(_ISP_0245, ISP_REG_BASE, (0x0245<<2), rISP_WPAGE		)	//	
#define ISP_rISP_WPAGE1			_bm(_ISP_0245, ISP_REG_BASE, (0x0245<<2), rISP_WPAGE1		)	//	
#define ISP_0245_T				_am(_ISP_0245, ISP_REG_BASE, (0x0245<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rHCNTI_SV			:12;
		UINT _res1				: 4;
		UINT rVCNTI_SV			:12;	_rege_	_ISP_0246;

#define ISP_rHCNTI_SV			_bm(_ISP_0246, ISP_REG_BASE, (0x0246<<2), rHCNTI_SV			)	//	
#define ISP_rVCNTI_SV			_bm(_ISP_0246, ISP_REG_BASE, (0x0246<<2), rVCNTI_SV			)	//	
#define ISP_0246_T				_am(_ISP_0246, ISP_REG_BASE, (0x0246<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rHCNTI				:12;
		UINT _res1				: 4;
		UINT rVCNTI				:12;	_rege_	_ISP_0247;

#define ISP_rHCNTI				_bm(_ISP_0247, ISP_REG_BASE, (0x0247<<2), rHCNTI			)	//	
#define ISP_rVCNTI				_bm(_ISP_0247, ISP_REG_BASE, (0x0247<<2), rVCNTI			)	//	
#define ISP_0247_T				_am(_ISP_0247, ISP_REG_BASE, (0x0247<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rHCNTI2_SV			:12;
		UINT _res1				: 4;
		UINT rVCNTI2_SV			:12;	_rege_	_ISP_0248;

#define ISP_rHCNTI2_SV			_bm(_ISP_0248, ISP_REG_BASE, (0x0248<<2), rHCNTI2_SV		)	//	
#define ISP_rVCNTI2_SV			_bm(_ISP_0248, ISP_REG_BASE, (0x0248<<2), rVCNTI2_SV		)	//	
#define ISP_0248_T				_am(_ISP_0248, ISP_REG_BASE, (0x0248<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rHCNTI2			:12;
		UINT _res1				: 4;
		UINT rVCNTI2			:12;	_rege_	_ISP_0249;

#define ISP_rHCNTI2				_bm(_ISP_0249, ISP_REG_BASE, (0x0249<<2), rHCNTI2			)	//	
#define ISP_rVCNTI2				_bm(_ISP_0249, ISP_REG_BASE, (0x0249<<2), rVCNTI2			)	//	
#define ISP_0249_T				_am(_ISP_0249, ISP_REG_BASE, (0x0249<<2))						//	

_regs_	UINT _res0				: 8;
		UINT rWORD_CNT			:24;	_rege_	_ISP_024B;

#define ISP_rWORD_CNT			_bm(_ISP_024B, ISP_REG_BASE, (0x024B<<2), rWORD_CNT			)	//	
#define ISP_024B_T				_am(_ISP_024B, ISP_REG_BASE, (0x024B<<2))						//	

_regs_	UINT rLS_FLG			: 1;
		UINT rLS_FLG_POST		: 1;
		UINT _res0				: 2;
		UINT rWDR_G				:12;
		UINT _res1				: 6;
		UINT rWDR_S1			:10;	_rege_	_ISP_0250;

#define ISP_rLS_FLG				_bm(_ISP_0250, ISP_REG_BASE, (0x0250<<2), rLS_FLG			)	//	
#define ISP_rLS_FLG_POST		_bm(_ISP_0250, ISP_REG_BASE, (0x0250<<2), rLS_FLG_POST		)	//	
#define ISP_rWDR_G				_bm(_ISP_0250, ISP_REG_BASE, (0x0250<<2), rWDR_G			)	//	
#define ISP_rWDR_S1				_bm(_ISP_0250, ISP_REG_BASE, (0x0250<<2), rWDR_S1			)	//	
#define ISP_0250_T				_am(_ISP_0250, ISP_REG_BASE, (0x0250<<2))						//	

_regs_	UINT _res0				: 6;
		UINT rWDR_S2			:10;
		UINT _res1				: 6;
		UINT rWDR_SH			:10;	_rege_	_ISP_0251;

#define ISP_rWDR_S2				_bm(_ISP_0251, ISP_REG_BASE, (0x0251<<2), rWDR_S2			)	//	
#define ISP_rWDR_SH				_bm(_ISP_0251, ISP_REG_BASE, (0x0251<<2), rWDR_SH			)	//	
#define ISP_0251_T				_am(_ISP_0251, ISP_REG_BASE, (0x0251<<2))						//	

_regs_	UINT _res0				: 6;
		UINT rWDR_SH_ADJ		:10;
		UINT _res1				: 6;
		UINT rWDR_SWGT_ADJ		:10;	_rege_	_ISP_0252;

#define ISP_rWDR_SH_ADJ			_bm(_ISP_0252, ISP_REG_BASE, (0x0252<<2), rWDR_SH_ADJ		)	//	
#define ISP_rWDR_SWGT_ADJ		_bm(_ISP_0252, ISP_REG_BASE, (0x0252<<2), rWDR_SWGT_ADJ		)	//	
#define ISP_0252_T				_am(_ISP_0252, ISP_REG_BASE, (0x0252<<2))						//	

_regs_	UINT _res0				: 6;
		UINT rWDR_SC			:10;
		UINT _res1				: 6;
		UINT rWDR_SM			:10;	_rege_	_ISP_0253;

#define ISP_rWDR_SC				_bm(_ISP_0253, ISP_REG_BASE, (0x0253<<2), rWDR_SC			)	//	
#define ISP_rWDR_SM				_bm(_ISP_0253, ISP_REG_BASE, (0x0253<<2), rWDR_SM			)	//	
#define ISP_0253_T				_am(_ISP_0253, ISP_REG_BASE, (0x0253<<2))						//	

_regs_	UINT _res0				:24;
		UINT rWDR_SGF_SLOPE		: 8;	_rege_	_ISP_0254;

#define ISP_rWDR_SGF_SLOPE		_bm(_ISP_0254, ISP_REG_BASE, (0x0254<<2), rWDR_SGF_SLOPE	)	//	
#define ISP_0254_T				_am(_ISP_0254, ISP_REG_BASE, (0x0254<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rWDR_LY_SUM		:31;	_rege_	_ISP_0255;

#define ISP_rWDR_LY_SUM			_bm(_ISP_0255, ISP_REG_BASE, (0x0255<<2), rWDR_LY_SUM		)	//	
#define ISP_0255_T				_am(_ISP_0255, ISP_REG_BASE, (0x0255<<2))						//	

_regs_	UINT _res0				: 1;
		UINT rWDR_SY_SUM		:31;	_rege_	_ISP_0256;

#define ISP_rWDR_SY_SUM			_bm(_ISP_0256, ISP_REG_BASE, (0x0256<<2), rWDR_SY_SUM		)	//	
#define ISP_0256_T				_am(_ISP_0256, ISP_REG_BASE, (0x0256<<2))						//	

_regs_	UINT _res0				:11;
		UINT rWDR_HIGH_CNT		:21;	_rege_	_ISP_0257;

#define ISP_rWDR_HIGH_CNT		_bm(_ISP_0257, ISP_REG_BASE, (0x0257<<2), rWDR_HIGH_CNT		)	//	
#define ISP_0257_T				_am(_ISP_0257, ISP_REG_BASE, (0x0257<<2))						//	

_regs_	UINT _res0				:11;
		UINT rWDR_LOW_CNT		:21;	_rege_	_ISP_0258;

#define ISP_rWDR_LOW_CNT		_bm(_ISP_0258, ISP_REG_BASE, (0x0258<<2), rWDR_LOW_CNT		)	//	
#define ISP_0258_T				_am(_ISP_0258, ISP_REG_BASE, (0x0258<<2))						//	

_regs_	UINT _res0				:20;
		UINT rWDR_SL_R			:12;	_rege_	_ISP_0259;

#define ISP_rWDR_SL_R			_bm(_ISP_0259, ISP_REG_BASE, (0x0259<<2), rWDR_SL_R			)	//	
#define ISP_0259_T				_am(_ISP_0259, ISP_REG_BASE, (0x0259<<2))						//	

_regs_	UINT _res0				:14;
		UINT rWDR_G_INV			:18;	_rege_	_ISP_025A;

#define ISP_rWDR_G_INV			_bm(_ISP_025A, ISP_REG_BASE, (0x025A<<2), rWDR_G_INV		)	//	
#define ISP_025A_T				_am(_ISP_025A, ISP_REG_BASE, (0x025A<<2))						//	

_regs_	UINT _res0				:22;
		UINT rDF_COUNT			:10;	_rege_	_ISP_025B;

#define ISP_rDF_COUNT			_bm(_ISP_025B, ISP_REG_BASE, (0x025B<<2), rDF_COUNT			)	//	
#define ISP_025B_T				_am(_ISP_025B, ISP_REG_BASE, (0x025B<<2))						//	

_regs_	UINT _res0				:22;
		UINT rSHD_MAX			:10;	_rege_	_ISP_025C;

#define ISP_rSHD_MAX			_bm(_ISP_025C, ISP_REG_BASE, (0x025C<<2), rSHD_MAX			)	//	
#define ISP_025C_T				_am(_ISP_025C, ISP_REG_BASE, (0x025C<<2))						//	

_regs_	UINT rIMD_MD000			:32;	_rege_	_ISP_0260;

#define ISP_rIMD_MD000			_bm(_ISP_0260, ISP_REG_BASE, (0x0260<<2), rIMD_MD000		)	//	
#define ISP_0260_T				_am(_ISP_0260, ISP_REG_BASE, (0x0260<<2))						//	

_regs_	UINT rIMD_MD001			:32;	_rege_	_ISP_0261;

#define ISP_rIMD_MD001			_bm(_ISP_0261, ISP_REG_BASE, (0x0261<<2), rIMD_MD001		)	//	
#define ISP_0261_T				_am(_ISP_0261, ISP_REG_BASE, (0x0261<<2))						//	

_regs_	UINT rIMD_MD002			:32;	_rege_	_ISP_0262;

#define ISP_rIMD_MD002			_bm(_ISP_0262, ISP_REG_BASE, (0x0262<<2), rIMD_MD002		)	//	
#define ISP_0262_T				_am(_ISP_0262, ISP_REG_BASE, (0x0262<<2))						//	

_regs_	UINT rIMD_MD003			:32;	_rege_	_ISP_0263;

#define ISP_rIMD_MD003			_bm(_ISP_0263, ISP_REG_BASE, (0x0263<<2), rIMD_MD003		)	//	
#define ISP_0263_T				_am(_ISP_0263, ISP_REG_BASE, (0x0263<<2))						//	

_regs_	UINT rIMD_MD004			:32;	_rege_	_ISP_0264;

#define ISP_rIMD_MD004			_bm(_ISP_0264, ISP_REG_BASE, (0x0264<<2), rIMD_MD004		)	//	
#define ISP_0264_T				_am(_ISP_0264, ISP_REG_BASE, (0x0264<<2))						//	

_regs_	UINT rIMD_MD005			:32;	_rege_	_ISP_0265;

#define ISP_rIMD_MD005			_bm(_ISP_0265, ISP_REG_BASE, (0x0265<<2), rIMD_MD005		)	//	
#define ISP_0265_T				_am(_ISP_0265, ISP_REG_BASE, (0x0265<<2))						//	

_regs_	UINT rIMD_MD006			:32;	_rege_	_ISP_0266;

#define ISP_rIMD_MD006			_bm(_ISP_0266, ISP_REG_BASE, (0x0266<<2), rIMD_MD006		)	//	
#define ISP_0266_T				_am(_ISP_0266, ISP_REG_BASE, (0x0266<<2))						//	

_regs_	UINT rIMD_MD007			:32;	_rege_	_ISP_0267;

#define ISP_rIMD_MD007			_bm(_ISP_0267, ISP_REG_BASE, (0x0267<<2), rIMD_MD007		)	//	
#define ISP_0267_T				_am(_ISP_0267, ISP_REG_BASE, (0x0267<<2))						//	

_regs_	UINT rIMD_MD008			:32;	_rege_	_ISP_0268;

#define ISP_rIMD_MD008			_bm(_ISP_0268, ISP_REG_BASE, (0x0268<<2), rIMD_MD008		)	//	
#define ISP_0268_T				_am(_ISP_0268, ISP_REG_BASE, (0x0268<<2))						//	

_regs_	UINT rIMD_MD009			:32;	_rege_	_ISP_0269;

#define ISP_rIMD_MD009			_bm(_ISP_0269, ISP_REG_BASE, (0x0269<<2), rIMD_MD009		)	//	
#define ISP_0269_T				_am(_ISP_0269, ISP_REG_BASE, (0x0269<<2))						//	

_regs_	UINT rIMD_MD010			:32;	_rege_	_ISP_026A;

#define ISP_rIMD_MD010			_bm(_ISP_026A, ISP_REG_BASE, (0x026A<<2), rIMD_MD010		)	//	
#define ISP_026A_T				_am(_ISP_026A, ISP_REG_BASE, (0x026A<<2))						//	

_regs_	UINT rIMD_MD011			:32;	_rege_	_ISP_026B;

#define ISP_rIMD_MD011			_bm(_ISP_026B, ISP_REG_BASE, (0x026B<<2), rIMD_MD011		)	//	
#define ISP_026B_T				_am(_ISP_026B, ISP_REG_BASE, (0x026B<<2))						//	

_regs_	UINT rIMD_MD012			:32;	_rege_	_ISP_026C;

#define ISP_rIMD_MD012			_bm(_ISP_026C, ISP_REG_BASE, (0x026C<<2), rIMD_MD012		)	//	
#define ISP_026C_T				_am(_ISP_026C, ISP_REG_BASE, (0x026C<<2))						//	

_regs_	UINT rIMD_MD013			:32;	_rege_	_ISP_026D;

#define ISP_rIMD_MD013			_bm(_ISP_026D, ISP_REG_BASE, (0x026D<<2), rIMD_MD013		)	//	
#define ISP_026D_T				_am(_ISP_026D, ISP_REG_BASE, (0x026D<<2))						//	

_regs_	UINT rIMD_MD014			:32;	_rege_	_ISP_026E;

#define ISP_rIMD_MD014			_bm(_ISP_026E, ISP_REG_BASE, (0x026E<<2), rIMD_MD014		)	//	
#define ISP_026E_T				_am(_ISP_026E, ISP_REG_BASE, (0x026E<<2))						//	

_regs_	UINT rIMD_MD015			:32;	_rege_	_ISP_026F;

#define ISP_rIMD_MD015			_bm(_ISP_026F, ISP_REG_BASE, (0x026F<<2), rIMD_MD015		)	//	
#define ISP_026F_T				_am(_ISP_026F, ISP_REG_BASE, (0x026F<<2))						//	

_regs_	UINT rIMD_MD016			:32;	_rege_	_ISP_0270;

#define ISP_rIMD_MD016			_bm(_ISP_0270, ISP_REG_BASE, (0x0270<<2), rIMD_MD016		)	//	
#define ISP_0270_T				_am(_ISP_0270, ISP_REG_BASE, (0x0270<<2))						//	

_regs_	UINT rIMD_MD017			:32;	_rege_	_ISP_0271;

#define ISP_rIMD_MD017			_bm(_ISP_0271, ISP_REG_BASE, (0x0271<<2), rIMD_MD017		)	//	
#define ISP_0271_T				_am(_ISP_0271, ISP_REG_BASE, (0x0271<<2))						//	

_regs_	UINT rIMD_MD018			:32;	_rege_	_ISP_0272;

#define ISP_rIMD_MD018			_bm(_ISP_0272, ISP_REG_BASE, (0x0272<<2), rIMD_MD018		)	//	
#define ISP_0272_T				_am(_ISP_0272, ISP_REG_BASE, (0x0272<<2))						//	

_regs_	UINT rIMD_MD019			:32;	_rege_	_ISP_0273;

#define ISP_rIMD_MD019			_bm(_ISP_0273, ISP_REG_BASE, (0x0273<<2), rIMD_MD019		)	//	
#define ISP_0273_T				_am(_ISP_0273, ISP_REG_BASE, (0x0273<<2))						//	

_regs_	UINT rIMD_MD020			:32;	_rege_	_ISP_0274;

#define ISP_rIMD_MD020			_bm(_ISP_0274, ISP_REG_BASE, (0x0274<<2), rIMD_MD020		)	//	
#define ISP_0274_T				_am(_ISP_0274, ISP_REG_BASE, (0x0274<<2))						//	

_regs_	UINT rIMD_MD021			:32;	_rege_	_ISP_0275;

#define ISP_rIMD_MD021			_bm(_ISP_0275, ISP_REG_BASE, (0x0275<<2), rIMD_MD021		)	//	
#define ISP_0275_T				_am(_ISP_0275, ISP_REG_BASE, (0x0275<<2))						//	

_regs_	UINT rIMD_MD022			:32;	_rege_	_ISP_0276;

#define ISP_rIMD_MD022			_bm(_ISP_0276, ISP_REG_BASE, (0x0276<<2), rIMD_MD022		)	//	
#define ISP_0276_T				_am(_ISP_0276, ISP_REG_BASE, (0x0276<<2))						//	

_regs_	UINT rIMD_MD023			:32;	_rege_	_ISP_0277;

#define ISP_rIMD_MD023			_bm(_ISP_0277, ISP_REG_BASE, (0x0277<<2), rIMD_MD023		)	//	
#define ISP_0277_T				_am(_ISP_0277, ISP_REG_BASE, (0x0277<<2))						//	

_regs_	UINT rIMD_MD024			:32;	_rege_	_ISP_0278;

#define ISP_rIMD_MD024			_bm(_ISP_0278, ISP_REG_BASE, (0x0278<<2), rIMD_MD024		)	//	
#define ISP_0278_T				_am(_ISP_0278, ISP_REG_BASE, (0x0278<<2))						//	

_regs_	UINT rIMD_MD025			:32;	_rege_	_ISP_0279;

#define ISP_rIMD_MD025			_bm(_ISP_0279, ISP_REG_BASE, (0x0279<<2), rIMD_MD025		)	//	
#define ISP_0279_T				_am(_ISP_0279, ISP_REG_BASE, (0x0279<<2))						//	

_regs_	UINT rIMD_MD026			:32;	_rege_	_ISP_027A;

#define ISP_rIMD_MD026			_bm(_ISP_027A, ISP_REG_BASE, (0x027A<<2), rIMD_MD026		)	//	
#define ISP_027A_T				_am(_ISP_027A, ISP_REG_BASE, (0x027A<<2))						//	

_regs_	UINT rIMD_MD027			:32;	_rege_	_ISP_027B;

#define ISP_rIMD_MD027			_bm(_ISP_027B, ISP_REG_BASE, (0x027B<<2), rIMD_MD027		)	//	
#define ISP_027B_T				_am(_ISP_027B, ISP_REG_BASE, (0x027B<<2))						//	

_regs_	UINT rIMD_MD028			:32;	_rege_	_ISP_027C;

#define ISP_rIMD_MD028			_bm(_ISP_027C, ISP_REG_BASE, (0x027C<<2), rIMD_MD028		)	//	
#define ISP_027C_T				_am(_ISP_027C, ISP_REG_BASE, (0x027C<<2))						//	

_regs_	UINT rIMD_MD029			:32;	_rege_	_ISP_027D;

#define ISP_rIMD_MD029			_bm(_ISP_027D, ISP_REG_BASE, (0x027D<<2), rIMD_MD029		)	//	
#define ISP_027D_T				_am(_ISP_027D, ISP_REG_BASE, (0x027D<<2))						//	

_regs_	UINT rIMD_MD030			:32;	_rege_	_ISP_027E;

#define ISP_rIMD_MD030			_bm(_ISP_027E, ISP_REG_BASE, (0x027E<<2), rIMD_MD030		)	//	
#define ISP_027E_T				_am(_ISP_027E, ISP_REG_BASE, (0x027E<<2))						//	

_regs_	UINT rIMD_MD031			:32;	_rege_	_ISP_027F;

#define ISP_rIMD_MD031			_bm(_ISP_027F, ISP_REG_BASE, (0x027F<<2), rIMD_MD031		)	//	
#define ISP_027F_T				_am(_ISP_027F, ISP_REG_BASE, (0x027F<<2))						//	

_regs_	UINT rIMD_MD100			:32;	_rege_	_ISP_0280;

#define ISP_rIMD_MD100			_bm(_ISP_0280, ISP_REG_BASE, (0x0280<<2), rIMD_MD100		)	//	
#define ISP_0280_T				_am(_ISP_0280, ISP_REG_BASE, (0x0280<<2))						//	

_regs_	UINT rIMD_MD101			:32;	_rege_	_ISP_0281;

#define ISP_rIMD_MD101			_bm(_ISP_0281, ISP_REG_BASE, (0x0281<<2), rIMD_MD101		)	//	
#define ISP_0281_T				_am(_ISP_0281, ISP_REG_BASE, (0x0281<<2))						//	

_regs_	UINT rIMD_MD102			:32;	_rege_	_ISP_0282;

#define ISP_rIMD_MD102			_bm(_ISP_0282, ISP_REG_BASE, (0x0282<<2), rIMD_MD102		)	//	
#define ISP_0282_T				_am(_ISP_0282, ISP_REG_BASE, (0x0282<<2))						//	

_regs_	UINT rIMD_MD103			:32;	_rege_	_ISP_0283;

#define ISP_rIMD_MD103			_bm(_ISP_0283, ISP_REG_BASE, (0x0283<<2), rIMD_MD103		)	//	
#define ISP_0283_T				_am(_ISP_0283, ISP_REG_BASE, (0x0283<<2))						//	

_regs_	UINT rIMD_MD104			:32;	_rege_	_ISP_0284;

#define ISP_rIMD_MD104			_bm(_ISP_0284, ISP_REG_BASE, (0x0284<<2), rIMD_MD104		)	//	
#define ISP_0284_T				_am(_ISP_0284, ISP_REG_BASE, (0x0284<<2))						//	

_regs_	UINT rIMD_MD105			:32;	_rege_	_ISP_0285;

#define ISP_rIMD_MD105			_bm(_ISP_0285, ISP_REG_BASE, (0x0285<<2), rIMD_MD105		)	//	
#define ISP_0285_T				_am(_ISP_0285, ISP_REG_BASE, (0x0285<<2))						//	

_regs_	UINT rIMD_MD106			:32;	_rege_	_ISP_0286;

#define ISP_rIMD_MD106			_bm(_ISP_0286, ISP_REG_BASE, (0x0286<<2), rIMD_MD106		)	//	
#define ISP_0286_T				_am(_ISP_0286, ISP_REG_BASE, (0x0286<<2))						//	

_regs_	UINT rIMD_MD107			:32;	_rege_	_ISP_0287;

#define ISP_rIMD_MD107			_bm(_ISP_0287, ISP_REG_BASE, (0x0287<<2), rIMD_MD107		)	//	
#define ISP_0287_T				_am(_ISP_0287, ISP_REG_BASE, (0x0287<<2))						//	

_regs_	UINT rIMD_MD108			:32;	_rege_	_ISP_0288;

#define ISP_rIMD_MD108			_bm(_ISP_0288, ISP_REG_BASE, (0x0288<<2), rIMD_MD108		)	//	
#define ISP_0288_T				_am(_ISP_0288, ISP_REG_BASE, (0x0288<<2))						//	

_regs_	UINT rIMD_MD109			:32;	_rege_	_ISP_0289;

#define ISP_rIMD_MD109			_bm(_ISP_0289, ISP_REG_BASE, (0x0289<<2), rIMD_MD109		)	//	
#define ISP_0289_T				_am(_ISP_0289, ISP_REG_BASE, (0x0289<<2))						//	

_regs_	UINT rIMD_MD110			:32;	_rege_	_ISP_028A;

#define ISP_rIMD_MD110			_bm(_ISP_028A, ISP_REG_BASE, (0x028A<<2), rIMD_MD110		)	//	
#define ISP_028A_T				_am(_ISP_028A, ISP_REG_BASE, (0x028A<<2))						//	

_regs_	UINT rIMD_MD111			:32;	_rege_	_ISP_028B;

#define ISP_rIMD_MD111			_bm(_ISP_028B, ISP_REG_BASE, (0x028B<<2), rIMD_MD111		)	//	
#define ISP_028B_T				_am(_ISP_028B, ISP_REG_BASE, (0x028B<<2))						//	

_regs_	UINT rIMD_MD112			:32;	_rege_	_ISP_028C;

#define ISP_rIMD_MD112			_bm(_ISP_028C, ISP_REG_BASE, (0x028C<<2), rIMD_MD112		)	//	
#define ISP_028C_T				_am(_ISP_028C, ISP_REG_BASE, (0x028C<<2))						//	

_regs_	UINT rIMD_MD113			:32;	_rege_	_ISP_028D;

#define ISP_rIMD_MD113			_bm(_ISP_028D, ISP_REG_BASE, (0x028D<<2), rIMD_MD113		)	//	
#define ISP_028D_T				_am(_ISP_028D, ISP_REG_BASE, (0x028D<<2))						//	

_regs_	UINT rIMD_MD114			:32;	_rege_	_ISP_028E;

#define ISP_rIMD_MD114			_bm(_ISP_028E, ISP_REG_BASE, (0x028E<<2), rIMD_MD114		)	//	
#define ISP_028E_T				_am(_ISP_028E, ISP_REG_BASE, (0x028E<<2))						//	

_regs_	UINT rIMD_MD115			:32;	_rege_	_ISP_028F;

#define ISP_rIMD_MD115			_bm(_ISP_028F, ISP_REG_BASE, (0x028F<<2), rIMD_MD115		)	//	
#define ISP_028F_T				_am(_ISP_028F, ISP_REG_BASE, (0x028F<<2))						//	

_regs_	UINT rIMD_MD116			:32;	_rege_	_ISP_0290;

#define ISP_rIMD_MD116			_bm(_ISP_0290, ISP_REG_BASE, (0x0290<<2), rIMD_MD116		)	//	
#define ISP_0290_T				_am(_ISP_0290, ISP_REG_BASE, (0x0290<<2))						//	

_regs_	UINT rIMD_MD117			:32;	_rege_	_ISP_0291;

#define ISP_rIMD_MD117			_bm(_ISP_0291, ISP_REG_BASE, (0x0291<<2), rIMD_MD117		)	//	
#define ISP_0291_T				_am(_ISP_0291, ISP_REG_BASE, (0x0291<<2))						//	

_regs_	UINT rIMD_MD118			:32;	_rege_	_ISP_0292;

#define ISP_rIMD_MD118			_bm(_ISP_0292, ISP_REG_BASE, (0x0292<<2), rIMD_MD118		)	//	
#define ISP_0292_T				_am(_ISP_0292, ISP_REG_BASE, (0x0292<<2))						//	

_regs_	UINT rIMD_MD119			:32;	_rege_	_ISP_0293;

#define ISP_rIMD_MD119			_bm(_ISP_0293, ISP_REG_BASE, (0x0293<<2), rIMD_MD119		)	//	
#define ISP_0293_T				_am(_ISP_0293, ISP_REG_BASE, (0x0293<<2))						//	

_regs_	UINT rIMD_MD120			:32;	_rege_	_ISP_0294;

#define ISP_rIMD_MD120			_bm(_ISP_0294, ISP_REG_BASE, (0x0294<<2), rIMD_MD120		)	//	
#define ISP_0294_T				_am(_ISP_0294, ISP_REG_BASE, (0x0294<<2))						//	

_regs_	UINT rIMD_MD121			:32;	_rege_	_ISP_0295;

#define ISP_rIMD_MD121			_bm(_ISP_0295, ISP_REG_BASE, (0x0295<<2), rIMD_MD121		)	//	
#define ISP_0295_T				_am(_ISP_0295, ISP_REG_BASE, (0x0295<<2))						//	

_regs_	UINT rIMD_MD122			:32;	_rege_	_ISP_0296;

#define ISP_rIMD_MD122			_bm(_ISP_0296, ISP_REG_BASE, (0x0296<<2), rIMD_MD122		)	//	
#define ISP_0296_T				_am(_ISP_0296, ISP_REG_BASE, (0x0296<<2))						//	

_regs_	UINT rIMD_MD123			:32;	_rege_	_ISP_0297;

#define ISP_rIMD_MD123			_bm(_ISP_0297, ISP_REG_BASE, (0x0297<<2), rIMD_MD123		)	//	
#define ISP_0297_T				_am(_ISP_0297, ISP_REG_BASE, (0x0297<<2))						//	

_regs_	UINT rIMD_MD124			:32;	_rege_	_ISP_0298;

#define ISP_rIMD_MD124			_bm(_ISP_0298, ISP_REG_BASE, (0x0298<<2), rIMD_MD124		)	//	
#define ISP_0298_T				_am(_ISP_0298, ISP_REG_BASE, (0x0298<<2))						//	

_regs_	UINT rIMD_MD125			:32;	_rege_	_ISP_0299;

#define ISP_rIMD_MD125			_bm(_ISP_0299, ISP_REG_BASE, (0x0299<<2), rIMD_MD125		)	//	
#define ISP_0299_T				_am(_ISP_0299, ISP_REG_BASE, (0x0299<<2))						//	

_regs_	UINT rIMD_MD126			:32;	_rege_	_ISP_029A;

#define ISP_rIMD_MD126			_bm(_ISP_029A, ISP_REG_BASE, (0x029A<<2), rIMD_MD126		)	//	
#define ISP_029A_T				_am(_ISP_029A, ISP_REG_BASE, (0x029A<<2))						//	

_regs_	UINT rIMD_MD127			:32;	_rege_	_ISP_029B;

#define ISP_rIMD_MD127			_bm(_ISP_029B, ISP_REG_BASE, (0x029B<<2), rIMD_MD127		)	//	
#define ISP_029B_T				_am(_ISP_029B, ISP_REG_BASE, (0x029B<<2))						//	

_regs_	UINT rIMD_MD128			:32;	_rege_	_ISP_029C;

#define ISP_rIMD_MD128			_bm(_ISP_029C, ISP_REG_BASE, (0x029C<<2), rIMD_MD128		)	//	
#define ISP_029C_T				_am(_ISP_029C, ISP_REG_BASE, (0x029C<<2))						//	

_regs_	UINT rIMD_MD129			:32;	_rege_	_ISP_029D;

#define ISP_rIMD_MD129			_bm(_ISP_029D, ISP_REG_BASE, (0x029D<<2), rIMD_MD129		)	//	
#define ISP_029D_T				_am(_ISP_029D, ISP_REG_BASE, (0x029D<<2))						//	

_regs_	UINT rIMD_MD130			:32;	_rege_	_ISP_029E;

#define ISP_rIMD_MD130			_bm(_ISP_029E, ISP_REG_BASE, (0x029E<<2), rIMD_MD130		)	//	
#define ISP_029E_T				_am(_ISP_029E, ISP_REG_BASE, (0x029E<<2))						//	

_regs_	UINT rIMD_MD131			:32;	_rege_	_ISP_029F;

#define ISP_rIMD_MD131			_bm(_ISP_029F, ISP_REG_BASE, (0x029F<<2), rIMD_MD131		)	//	
#define ISP_029F_T				_am(_ISP_029F, ISP_REG_BASE, (0x029F<<2))						//	

_regs_	UINT rJPG_OVF			: 1;
		UINT rJPG_BIG			: 1;
		UINT _res0				: 2;
		UINT rJPG_FSIZE			:28;	_rege_	_ISP_02A0;

#define ISP_rJPG_OVF			_bm(_ISP_02A0, ISP_REG_BASE, (0x02A0<<2), rJPG_OVF			)	//	
#define ISP_rJPG_BIG			_bm(_ISP_02A0, ISP_REG_BASE, (0x02A0<<2), rJPG_BIG			)	//	
#define ISP_rJPG_FSIZE			_bm(_ISP_02A0, ISP_REG_BASE, (0x02A0<<2), rJPG_FSIZE		)	//	
#define ISP_02A0_T				_am(_ISP_02A0, ISP_REG_BASE, (0x02A0<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rJPG_FSIZE_HOLD	:28;	_rege_	_ISP_02A1;

#define ISP_rJPG_FSIZE_HOLD		_bm(_ISP_02A1, ISP_REG_BASE, (0x02A1<<2), rJPG_FSIZE_HOLD	)	//	
#define ISP_02A1_T				_am(_ISP_02A1, ISP_REG_BASE, (0x02A1<<2))						//	

_regs_	UINT _res0				: 4;
		UINT rJPG_LEN			:28;	_rege_	_ISP_02A2;

#define ISP_rJPG_LEN			_bm(_ISP_02A2, ISP_REG_BASE, (0x02A2<<2), rJPG_LEN			)	//	
#define ISP_02A2_T				_am(_ISP_02A2, ISP_REG_BASE, (0x02A2<<2))						//	

_regs_	UINT _res0				:24;
		UINT rISP_RIRQ0			: 8;	_rege_	_ISP_02AE;

#define ISP_rISP_RIRQ0			_bm(_ISP_02AE, ISP_REG_BASE, (0x02AE<<2), rISP_RIRQ0		)	//	
#define ISP_02AE_T				_am(_ISP_02AE, ISP_REG_BASE, (0x02AE<<2))						//	

_regs_	UINT _res0				:24;
		UINT rISP_RIRQ1			: 8;	_rege_	_ISP_02AF;

#define ISP_rISP_RIRQ1			_bm(_ISP_02AF, ISP_REG_BASE, (0x02AF<<2), rISP_RIRQ1		)	//	
#define ISP_02AF_T				_am(_ISP_02AF, ISP_REG_BASE, (0x02AF<<2))						//	

#endif
