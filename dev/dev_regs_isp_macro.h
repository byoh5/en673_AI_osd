/* **************************************************************************
 File Name	: 	dev_regs_isp_macro.h
 Description:	EN673 - ISP Reg macro define
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 8. 27 / 15. 4. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */


//*************************************************************************************************
// ISP (EN673)
//-------------------------------------------------------------------------------------------------

#define _REG_BASE_				ISP_BASE

#define AWBL_CM_BASE			0x88

#define ENC_POS					0x030
#define ENC_XXX					0x1A0
#define ENC_SET					0x1A1

#define YGAMMA_BASE				0x106
#define CGAMMA_BASE				0x110

#define ENC_DS_BASE				0x1F4
#define DS0_BASE				0x621	// 0x175
#define DS1_BASE				0x625


#define BOXTON_BASE				0x123
#define BOXFIL_BASE				0x125
#define BOXPOS_BASE				0x126
#define BOXPLT_BASE				0x167

//161010 - ROI WDR Position Base Address
#define ROIWDR_BASE				0x0051


#define BOXFMT_BASE				0x187	// 2015112 - WHL : BIT BOX
#define BBOXFMT_BASE			0x188


//-------------------------------------------------------------------------------------------------
#define FORCE_ABT_SOFFr			(ValSft_R29(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define FORCE_ABT_SOFFw(val)	_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define FORCE_ABT_SONr			(ValSft_R28(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define FORCE_ABT_SONw(val)		_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define SYNC_UPr				(ValSft_R31(_rd32(_REG_BASE_+(0x0003<<2)))&BitMask_01)
#define SYNC_UPw(val)			_wr32(_REG_BASE_+(0x0003<<2),(UINT)((_rd32(_REG_BASE_+(0x0003<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PCLKI1_PDr				(ValSft_R31(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCLKI1_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FPCK_PDr				(ValSft_R30(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define FPCK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PPCK_PDr				(ValSft_R29(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PPCK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PSCK_PDr				(ValSft_R28(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PSCK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PR_CK_PDr				(ValSft_R27(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PR_CK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define FN_CK_PDr				(ValSft_R26(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define FN_CK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define YCW_CK0_PDr				(ValSft_R24(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define YCW_CK0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define YCW_CK1_PDr				(ValSft_R23(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define YCW_CK1_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define YCW_CK2_PDr				(ValSft_R22(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define YCW_CK2_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IMD_CK_PDr				(ValSft_R21(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IMD_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define JPGCK_PDr				(ValSft_R20(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define JPGCK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define CDS0_PCK_PDr			(ValSft_R19(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define CDS0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CDS1_PCK_PDr			(ValSft_R18(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define CDS1_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define PIP_PPCK_PDr			(ValSft_R17(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PIP_PPCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define IBT_PCK_PDr				(ValSft_R16(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IBT_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define IBT0_PCK_PDr			(ValSft_R15(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IBT0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define BT_PCK_PDr				(ValSft_R14(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define BT_PCK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define BT0_PCK_PDr				(ValSft_R13(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define BT0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define YCR_CK0_PDr				(ValSft_R12(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define YCR_CK0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define IPUIRQ0_PDr				(ValSft_R11(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IPUIRQ0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IPUIRQ1_PDr				(ValSft_R10(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IPUIRQ1_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define PCLK_ONr				(ValSft_R09(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCLK_ONw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define PCKO0_ONr				(ValSft_R08(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCKO0_ONw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define PCKO1_ONr				(ValSft_R07(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCKO1_ONw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define PCKO_OENr				(ValSft_R05(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCKO_OENw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define BT1120_PCK_PDr			(ValSft_R04(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define BT1120_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT1120_PCK0_PDr			(ValSft_R03(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define BT1120_PCK0_PDw(val)	_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define PCLK1_DLYr				(ValSft_R12(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK1_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define PCLK_DLYr				(ValSft_R08(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define VIRQI_ENr				(ValSft_R31(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQI_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VIRQW_ENr				(ValSft_R30(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQW_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define VIRQO_ENr				(ValSft_R29(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQO_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define UIRQ0_ENr				(ValSft_R28(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define UIRQ0_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define UIRQ1_ENr				(ValSft_R27(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define UIRQ1_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define VIRQW2_ENr				(ValSft_R26(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQW2_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define CLI_VLOCKIr				(ValSft_R07(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKIw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define CLI_VLOCKWr				(ValSft_R06(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKWw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define CLI_VLOCKOr				(ValSft_R05(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKOw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CLI_UIRQ0r				(ValSft_R04(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_UIRQ0w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define CLI_UIRQ1r				(ValSft_R03(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_UIRQ1w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define CLI_VLOCKW2r			(ValSft_R02(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKW2w(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define BT_TRG_DIN0r			(ValSft_R29(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_TRG_DIN0w(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT_TRG_HSI0r			(ValSft_R28(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_TRG_HSI0w(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT_TRG_VSI0r			(ValSft_R27(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_TRG_VSI0w(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BT_POL_HSI0r			(ValSft_R26(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_POL_HSI0w(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BT_YC_HSPI0r			(ValSft_R13(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_13)
#define BT_YC_HSPI0w(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L13(BitMask_13)))|ValSft_L13((val)&BitMask_13)))

#define BT_YC_HW0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_13)
#define BT_YC_HW0w(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define TRG_DIN0r				(ValSft_R31(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define TRG_DIN0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define TRG_HSI0r				(ValSft_R30(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define TRG_HSI0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define TRG_VSI0r				(ValSft_R29(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define TRG_VSI0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define POL_HSI0r				(ValSft_R28(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define POL_HSI0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define POL_VSI0r				(ValSft_R27(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define POL_VSI0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ASYNC_ON0r				(ValSft_R26(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define ASYNC_ON0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSYN_NAON0r				(ValSft_R25(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define VSYN_NAON0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BT_MODESD0r				(ValSft_R24(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BT_MODESD0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_MOD0r			(ValSft_R23(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BT1120_MOD0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define BTBLK_RON0r				(ValSft_R22(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BTBLK_RON0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define YC_MOD0r				(ValSft_R21(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define YC_MOD0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define YCBCR_MOD0r				(ValSft_R20(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define YCBCR_MOD0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define YC_FLG_INV0r			(ValSft_R19(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define YC_FLG_INV0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CBR_FLG_INV0r			(ValSft_R18(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define CBR_FLG_INV0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define TRS_REON0r				(ValSft_R17(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define TRS_REON0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define DSENS_ONr				(ValSft_R16(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define DSENS_ONw(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define ASYNC_VPOS0r			(ValSft_R03(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_13)
#define ASYNC_VPOS0w(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L03(BitMask_13)))|ValSft_L03((val)&BitMask_13)))

#define INSEL0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_03)
#define INSEL0w(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define DI0_BYPr				(ValSft_R31(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define DI0_BYPw(val)			_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT0_656_SELr			(ValSft_R30(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT0_656_SELw(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT_OBUF_RST0r			(ValSft_R29(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_OBUF_RST0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define VLOCKI_POS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_12)
#define VLOCKI_POS0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define BT0_656_DSELr			(ValSft_R15(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT0_656_DSELw(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define HLOCKI_POS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_12)
#define HLOCKI_POS0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BT_SMr					(ValSft_R31(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT_SMw(val)				_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_SM0r					(ValSft_R30(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT_SM0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT0_MD_OSD_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT0_MD_OSD_ONw(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define HSPI0r					(ValSft_R16(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_12)
#define HSPI0w(val)				_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define BIT_EXT_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BIT_EXT_ONw(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define BIT_EXT_ON0r			(ValSft_R14(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BIT_EXT_ON0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define VSPI0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_12)
#define VSPI0w(val)				_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWI0r					(ValSft_R16(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_12)
#define HWI0w(val)				_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWI0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_12)
#define VWI0w(val)				_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BT_OBUF_RSTr			(ValSft_R31(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT_OBUF_RSTw(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ASYNC_VST0r				(ValSft_R16(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_13)
#define ASYNC_VST0w(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_HW0r				(ValSft_R07(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_09)
#define ASYNC_HW0w(val)			_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L07(BitMask_09)))|ValSft_L07((val)&BitMask_09)))

#define BT1120_MODr				(ValSft_R06(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT1120_MODw(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define BTBLK_RONr				(ValSft_R05(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BTBLK_RONw(val)			_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define YC_MODr					(ValSft_R04(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define YC_MODw(val)			_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT_MODESDr				(ValSft_R03(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT_MODESDw(val)			_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define BT_656_SELr				(ValSft_R02(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT_656_SELw(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define BT_656_DSELr			(ValSft_R01(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT_656_DSELw(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define BT_POL_HSIr				(ValSft_R00(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_01)
#define BT_POL_HSIw(val)		_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define BT_MD_OSD_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define BT_MD_OSD_ONw(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define YCBCR_MODr				(ValSft_R30(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define YCBCR_MODw(val)			_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YC_FLG_INVr				(ValSft_R29(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define YC_FLG_INVw(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CBR_FLG_INVr			(ValSft_R28(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define CBR_FLG_INVw(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT_DIN_SELr				(ValSft_R27(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define BT_DIN_SELw(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define TRS_REONr				(ValSft_R26(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define TRS_REONw(val)			_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BT_YC_HSPIr				(ValSft_R13(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_13)
#define BT_YC_HSPIw(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L13(BitMask_13)))|ValSft_L13((val)&BitMask_13)))

#define BT_YC_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_13)
#define BT_YC_HWw(val)			_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IBT_PCK_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define IBT_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FPCK_SELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_02)
#define FPCK_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BT_PCK_SELr				(ValSft_R26(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_02)
#define BT_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define PPCK_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_02)
#define PPCK_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PSCK_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define PSCK_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define PCKO0_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define PCKO0_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define FN_CK_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define FN_CK_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define PR_CK_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define PR_CK_SELw(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define IMD_CK_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define IMD_CK_SELw(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L04(BitMask_03)))|ValSft_L04((val)&BitMask_03)))

#define PIP_PPCK_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_03)
#define PIP_PPCK_SELw(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define IBT0_PCK_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define IBT0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT0_PCK_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PCKO1_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define PCKO1_SELw(val)			_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define YCW_CK0_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define YCW_CK0_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define YCW_CK1_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define YCW_CK1_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define YCW_CK2_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define YCW_CK2_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define YCR_CK0_SELr			(ValSft_R09(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define YCR_CK0_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L09(BitMask_03)))|ValSft_L09((val)&BitMask_03)))

#define CDS0_PCK_SELr			(ValSft_R06(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define CDS0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L06(BitMask_03)))|ValSft_L06((val)&BitMask_03)))

#define CDS1_PCK_SELr			(ValSft_R03(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define CDS1_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L03(BitMask_03)))|ValSft_L03((val)&BitMask_03)))

#define JPGCK_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_03)
#define JPGCK_SELw(val)			_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define SLVr					(ValSft_R30(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_02)
#define SLVw(val)				_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define CH_DSELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CH_DSELw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CH_HSSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CH_HSSELw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define CH_VSSELr				(ValSft_R27(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CH_VSSELw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define TRG_DINr				(ValSft_R26(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define TRG_DINw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define TRG_HSIr				(ValSft_R25(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define TRG_HSIw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define TRG_VSIr				(ValSft_R24(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define TRG_VSIw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define POL_HSIr				(ValSft_R23(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define POL_HSIw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define POL_VSIr				(ValSft_R22(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define POL_VSIw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define SHSO_INVr				(ValSft_R21(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SHSO_INVw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define SVSO_INVr				(ValSft_R20(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SVSO_INVw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define SHSO_TRGr				(ValSft_R19(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SHSO_TRGw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define SVSO_TRGr				(ValSft_R18(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SVSO_TRGw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IDDR_ONr				(ValSft_R17(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define IDDR_ONw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define ISDR_OPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define ISDR_OPw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define DDRFF_RSr				(ValSft_R15(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define DDRFF_RSw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define SORDr					(ValSft_R14(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SORDw(val)				_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define CATPr					(ValSft_R13(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CATPw(val)				_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define ASYNC_ONr				(ValSft_R12(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define ASYNC_ONw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define VSYN_NAONr				(ValSft_R11(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define VSYN_NAONw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define PDI_MODr				(ValSft_R09(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_02)
#define PDI_MODw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L09(BitMask_02)))|ValSft_L09((val)&BitMask_02)))

#define SYNC_BYSr				(ValSft_R08(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define SYNC_BYSw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define ICSELr					(ValSft_R04(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_02)
#define ICSELw(val)				_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define INSEL_MODr				(ValSft_R03(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define INSEL_MODw(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define INSELr					(ValSft_R00(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_03)
#define INSELw(val)				_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define HTWIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_16)
#define HTWIw(val)				_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define VTWIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_16)
#define VTWIw(val)				_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define PRS_HZr					(ValSft_R31(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_01)
#define PRS_HZw(val)			_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define POS_HZr					(ValSft_R30(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_01)
#define POS_HZw(val)			_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define VLOCKI_POSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_12)
#define VLOCKI_POSw(val)		_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_13)
#define HLOCKI_POSw(val)		_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_12)
#define HSPIw(val)				_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSPIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_12)
#define VSPIw(val)				_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_12)
#define HWIw(val)				_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_12)
#define VWIw(val)				_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HTWOr					(ValSft_R16(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_16)
#define HTWOw(val)				_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define VTWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_16)
#define VTWOw(val)				_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define OSYNC_MODr				(ValSft_R31(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define OSYNC_MODw(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VLOCKI2_POSr			(ValSft_R16(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_12)
#define VLOCKI2_POSw(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI2_POSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_12)
#define HLOCKI2_POSw(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSPOr					(ValSft_R16(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_12)
#define HSPOw(val)				_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSPOr					(ValSft_R00(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_12)
#define VSPOw(val)				_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HVWIO_SYNr				(ValSft_R31(_rd32(_REG_BASE_+(0x001a<<2)))&BitMask_01)
#define HVWIO_SYNw(val)			_wr32(_REG_BASE_+(0x001a<<2),(UINT)((_rd32(_REG_BASE_+(0x001a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HWOr					(ValSft_R16(_rd32(_REG_BASE_+(0x001a<<2)))&BitMask_12)
#define HWOw(val)				_wr32(_REG_BASE_+(0x001a<<2),(UINT)((_rd32(_REG_BASE_+(0x001a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x001a<<2)))&BitMask_12)
#define VWOw(val)				_wr32(_REG_BASE_+(0x001a<<2),(UINT)((_rd32(_REG_BASE_+(0x001a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSYNC_OFS_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define HSYNC_OFS_ONw(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HSYNC_OFS_VFLG_SELr		(ValSft_R30(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define HSYNC_OFS_VFLG_SELw(val)	_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HSYNC_OFS_INVr			(ValSft_R29(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define HSYNC_OFS_INVw(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define HSYNC_OFSr				(ValSft_R24(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_03)
#define HSYNC_OFSw(val)			_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define OCSELr					(ValSft_R30(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_02)
#define OCSELw(val)				_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define YDLYr					(ValSft_R24(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_04)
#define YDLYw(val)				_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define REG_UPMODEr				(ValSft_R31(_rd32(_REG_BASE_+(0x001f<<2)))&BitMask_01)
#define REG_UPMODEw(val)		_wr32(_REG_BASE_+(0x001f<<2),(UINT)((_rd32(_REG_BASE_+(0x001f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define SHSO_FPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_16)
#define SHSO_FPw(val)			_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SHSO_RPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_16)
#define SHSO_RPw(val)			_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SVSO_FPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_16)
#define SVSO_FPw(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SVSO_RPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_16)
#define SVSO_RPw(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define ASYNC_VPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_13)
#define ASYNC_VPOSw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_VSTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_13)
#define ASYNC_VSTw(val)			_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ASYNC_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_09)
#define ASYNC_HWw(val)			_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define POST_MASKr				(ValSft_R31(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_01)
#define POST_MASKw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ONSELr					(ValSft_R28(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_02)
#define ONSELw(val)				_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define PCBR_CINVr				(ValSft_R27(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_01)
#define PCBR_CINVw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define YCBR2YC_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_02)
#define YCBR2YC_SELw(val)		_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define YC_OSELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_05)
#define YC_OSELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L12(BitMask_05)))|ValSft_L12((val)&BitMask_05)))

#define POST_OSELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_04)
#define POST_OSELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define PRE_OSELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_05)
#define PRE_OSELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define UIRQ_FHPOS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_13)
#define UIRQ_FHPOS0w(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define UIRQ_FVPOS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_12)
#define UIRQ_FVPOS0w(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define UIRQ_FHPOS1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_12)
#define UIRQ_FHPOS1w(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define UIRQ_FVPOS1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_12)
#define UIRQ_FVPOS1w(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define IRIS_MUXr				(ValSft_R16(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_01)
#define IRIS_MUXw(val)			_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define SDES_PCr				(ValSft_R08(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_05)
#define SDES_PCw(val)			_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L08(BitMask_05)))|ValSft_L08((val)&BitMask_05)))

#define SDES_PDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_05)
#define SDES_PDw(val)			_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define LVDS_RXONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LVDS_RXONw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LVDS_TCHKr				(ValSft_R30(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LVDS_TCHKw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LVDS_FONr				(ValSft_R29(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LVDS_FONw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define LMOD_BITr				(ValSft_R27(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define LMOD_BITw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L27(BitMask_02)))|ValSft_L27((val)&BitMask_02)))

#define LDI_POLr				(ValSft_R26(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LDI_POLw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define LDI_PNSELr				(ValSft_R25(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LDI_PNSELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define LVDS_LSBr				(ValSft_R24(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define LVDS_LSBw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LDO3_SELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define LDO3_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define LDO2_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define LDO2_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define LDO1_SELr				(ValSft_R18(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define LDO1_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define LDO0_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define LDO0_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define CH2_RFLG_INVr			(ValSft_R15(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define CH2_RFLG_INVw(val)		_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define LDIDLY3r				(ValSft_R12(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_03)
#define LDIDLY3w(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define LDIDLY2r				(ValSft_R09(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_03)
#define LDIDLY2w(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L09(BitMask_03)))|ValSft_L09((val)&BitMask_03)))

#define LDIDLY1r				(ValSft_R06(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_03)
#define LDIDLY1w(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L06(BitMask_03)))|ValSft_L06((val)&BitMask_03)))

#define LDIDLY0r				(ValSft_R03(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_03)
#define LDIDLY0w(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L03(BitMask_03)))|ValSft_L03((val)&BitMask_03)))

#define LCK_DLYr				(ValSft_R00(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_03)
#define LCK_DLYw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define SYNC_COD0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_16)
#define SYNC_COD0w(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SYNC_COD1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_16)
#define SYNC_COD1w(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SYNC_COD2r				(ValSft_R16(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_16)
#define SYNC_COD2w(val)			_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SYNC_COD3_0r			(ValSft_R00(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_16)
#define SYNC_COD3_0w(val)		_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SYNC_COD3_1r			(ValSft_R16(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_16)
#define SYNC_COD3_1w(val)		_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SYNC_COD3_2r			(ValSft_R00(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_16)
#define SYNC_COD3_2w(val)		_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define LBUF_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define LBUF_ONw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LBUF_DONr				(ValSft_R30(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define LBUF_DONw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LVDS_FIFO_RSTr			(ValSft_R29(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define LVDS_FIFO_RSTw(val)		_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define LVDS_2CH_ONr			(ValSft_R28(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define LVDS_2CH_ONw(val)		_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define LV_RPOSr				(ValSft_R26(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_02)
#define LV_RPOSw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define LDCAP_POSr				(ValSft_R24(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_02)
#define LDCAP_POSw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BLANK_SOL_ONr			(ValSft_R23(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define BLANK_SOL_ONw(val)		_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define SONY_V_ONr				(ValSft_R22(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define SONY_V_ONw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define LBUF_WENr				(ValSft_R21(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_01)
#define LBUF_WENw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define MERGE_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_12)
#define MERGE_HWw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SHD_MODEr				(ValSft_R30(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_02)
#define SHD_MODEw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define SHD_TESTr				(ValSft_R28(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_02)
#define SHD_TESTw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define SHD_DETOSDr				(ValSft_R27(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define SHD_DETOSDw(val)		_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define SHD_ECHKr				(ValSft_R26(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define SHD_ECHKw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define SHD_KHr					(ValSft_R16(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_10)
#define SHD_KHw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define SHD_KVr					(ValSft_R00(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_10)
#define SHD_KVw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define SHD_DHOFFr				(ValSft_R16(_rd32(_REG_BASE_+(0x0031<<2)))&BitMask_13)
#define SHD_DHOFFw(val)			_wr32(_REG_BASE_+(0x0031<<2),(UINT)((_rd32(_REG_BASE_+(0x0031<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SHD_DVOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0031<<2)))&BitMask_13)
#define SHD_DVOFFw(val)			_wr32(_REG_BASE_+(0x0031<<2),(UINT)((_rd32(_REG_BASE_+(0x0031<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define SHD_CHOFFr				(ValSft_R16(_rd32(_REG_BASE_+(0x0032<<2)))&BitMask_12)
#define SHD_CHOFFw(val)			_wr32(_REG_BASE_+(0x0032<<2),(UINT)((_rd32(_REG_BASE_+(0x0032<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SHD_CVOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0032<<2)))&BitMask_12)
#define SHD_CVOFFw(val)			_wr32(_REG_BASE_+(0x0032<<2),(UINT)((_rd32(_REG_BASE_+(0x0032<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SHD_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_08)
#define SHD_GAINw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define SHD_DHWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0034<<2)))&BitMask_13)
#define SHD_DHWw(val)			_wr32(_REG_BASE_+(0x0034<<2),(UINT)((_rd32(_REG_BASE_+(0x0034<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SHD_DVWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0034<<2)))&BitMask_13)
#define SHD_DVWw(val)			_wr32(_REG_BASE_+(0x0034<<2),(UINT)((_rd32(_REG_BASE_+(0x0034<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define GV_RSELr				(ValSft_R30(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_02)
#define GV_RSELw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define GV_656r					(ValSft_R29(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define GV_656w(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define GV_RSTNr				(ValSft_R28(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define GV_RSTNw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define SENSOR_WIDr				(ValSft_R08(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_08)
#define SENSOR_WIDw(val)		_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PARALLEL_SENSORr		(ValSft_R07(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define PARALLEL_SENSORw(val)	_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define ISSPILOGICr				(ValSft_R05(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define ISSPILOGICw(val)		_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define LSPI_RSTNr				(ValSft_R03(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define LSPI_RSTNw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define SSTYPE_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define SSTYPE_SELw(val)		_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define CMP_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_01)
#define CMP_ONw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CMP_OSD_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_01)
#define CMP_OSD_ONw(val)		_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define CMP_YGINVr				(ValSft_R29(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_01)
#define CMP_YGINVw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CMP_RBINVr				(ValSft_R28(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_01)
#define CMP_RBINVw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define CMP_HWr					(ValSft_R24(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_03)
#define CMP_HWw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define CMP_VWr					(ValSft_R20(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_03)
#define CMP_VWw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define CMP_MODr				(ValSft_R16(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_01)
#define CMP_MODw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define CMP_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_12)
#define CMP_VSPw(val)			_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define CMP_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0037<<2)))&BitMask_12)
#define CMP_HSPw(val)			_wr32(_REG_BASE_+(0x0037<<2),(UINT)((_rd32(_REG_BASE_+(0x0037<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CMP_DTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x0037<<2)))&BitMask_12)
#define CMP_DTHw(val)			_wr32(_REG_BASE_+(0x0037<<2),(UINT)((_rd32(_REG_BASE_+(0x0037<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define CMP_GROFSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0038<<2)))&BitMask_10)
#define CMP_GROFSw(val)			_wr32(_REG_BASE_+(0x0038<<2),(UINT)((_rd32(_REG_BASE_+(0x0038<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CMP_GBOFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0038<<2)))&BitMask_10)
#define CMP_GBOFSw(val)			_wr32(_REG_BASE_+(0x0038<<2),(UINT)((_rd32(_REG_BASE_+(0x0038<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CMP_ROFSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_10)
#define CMP_ROFSw(val)			_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CMP_BOFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_10)
#define CMP_BOFSw(val)			_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PREGAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_12)
#define PREGAINw(val)			_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DNR3D_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_01)
#define DNR3D_ONw(val)			_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DNR3D_GAr				(ValSft_R24(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_06)
#define DNR3D_GAw(val)			_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define DNR3D_FKr				(ValSft_R16(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_08)
#define DNR3D_FKw(val)			_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DNR3D_THr				(ValSft_R08(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_08)
#define DNR3D_THw(val)			_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DNROUT_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_02)
#define DNROUT_SELw(val)		_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define DF_DIFGAr				(ValSft_R24(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_08)
#define DF_DIFGAw(val)			_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define DF_DIFTHr				(ValSft_R16(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_08)
#define DF_DIFTHw(val)			_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DF_ALPHAr				(ValSft_R08(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_06)
#define DF_ALPHAw(val)			_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define DF_BETAr				(ValSft_R00(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_08)
#define DF_BETAw(val)			_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DF_CWEGHTr				(ValSft_R30(_rd32(_REG_BASE_+(0x0041<<2)))&BitMask_02)
#define DF_CWEGHTw(val)			_wr32(_REG_BASE_+(0x0041<<2),(UINT)((_rd32(_REG_BASE_+(0x0041<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DF_DETHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0041<<2)))&BitMask_12)
#define DF_DETHSPw(val)			_wr32(_REG_BASE_+(0x0041<<2),(UINT)((_rd32(_REG_BASE_+(0x0041<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_DETHEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0041<<2)))&BitMask_12)
#define DF_DETHEPw(val)			_wr32(_REG_BASE_+(0x0041<<2),(UINT)((_rd32(_REG_BASE_+(0x0041<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DF_TSELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0042<<2)))&BitMask_03)
#define DF_TSELw(val)			_wr32(_REG_BASE_+(0x0042<<2),(UINT)((_rd32(_REG_BASE_+(0x0042<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define DF_DETVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0042<<2)))&BitMask_12)
#define DF_DETVSPw(val)			_wr32(_REG_BASE_+(0x0042<<2),(UINT)((_rd32(_REG_BASE_+(0x0042<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_DETVEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0042<<2)))&BitMask_12)
#define DF_DETVEPw(val)			_wr32(_REG_BASE_+(0x0042<<2),(UINT)((_rd32(_REG_BASE_+(0x0042<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DF_OSDONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_01)
#define DF_OSDONw(val)			_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DF_CMDr					(ValSft_R28(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_03)
#define DF_CMDw(val)			_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define DF_DTHRESr				(ValSft_R16(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_08)
#define DF_DTHRESw(val)			_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DFS_RBG_SELr			(ValSft_R15(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_01)
#define DFS_RBG_SELw(val)		_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define DFS_DIFGAr				(ValSft_R08(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_07)
#define DFS_DIFGAw(val)			_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define DFS_DIFTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_08)
#define DFS_DIFTHw(val)			_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DF_TESTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0044<<2)))&BitMask_01)
#define DF_TESTONw(val)			_wr32(_REG_BASE_+(0x0044<<2),(UINT)((_rd32(_REG_BASE_+(0x0044<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DF_TESTPH1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0044<<2)))&BitMask_12)
#define DF_TESTPH1w(val)		_wr32(_REG_BASE_+(0x0044<<2),(UINT)((_rd32(_REG_BASE_+(0x0044<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_TESTPV1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0044<<2)))&BitMask_11)
#define DF_TESTPV1w(val)		_wr32(_REG_BASE_+(0x0044<<2),(UINT)((_rd32(_REG_BASE_+(0x0044<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define DF_TESTPH2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0045<<2)))&BitMask_12)
#define DF_TESTPH2w(val)		_wr32(_REG_BASE_+(0x0045<<2),(UINT)((_rd32(_REG_BASE_+(0x0045<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_TESTPV2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0045<<2)))&BitMask_11)
#define DF_TESTPV2w(val)		_wr32(_REG_BASE_+(0x0045<<2),(UINT)((_rd32(_REG_BASE_+(0x0045<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define DF_TESTPH3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0046<<2)))&BitMask_12)
#define DF_TESTPH3w(val)		_wr32(_REG_BASE_+(0x0046<<2),(UINT)((_rd32(_REG_BASE_+(0x0046<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_TESTPV3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0046<<2)))&BitMask_11)
#define DF_TESTPV3w(val)		_wr32(_REG_BASE_+(0x0046<<2),(UINT)((_rd32(_REG_BASE_+(0x0046<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define DF_TESTPH4r				(ValSft_R16(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_12)
#define DF_TESTPH4w(val)		_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DF_TESTPV4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_11)
#define DF_TESTPV4w(val)		_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define DF_TESTPLVr				(ValSft_R16(_rd32(_REG_BASE_+(0x0048<<2)))&BitMask_12)
#define DF_TESTPLVw(val)		_wr32(_REG_BASE_+(0x0048<<2),(UINT)((_rd32(_REG_BASE_+(0x0048<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RGB_DIR_THRr			(ValSft_R00(_rd32(_REG_BASE_+(0x0048<<2)))&BitMask_08)
#define RGB_DIR_THRw(val)		_wr32(_REG_BASE_+(0x0048<<2),(UINT)((_rd32(_REG_BASE_+(0x0048<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define WDR_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_ONw(val)			_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define WDR_SCHM_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_SCHM_ONw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define WDR_SGAMMA_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_SGAMMA_ONw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define WDR_SWGT_LMT_ONr		(ValSft_R28(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_SWGT_LMT_ONw(val)	_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define LS_FLG_INVr				(ValSft_R27(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define LS_FLG_INVw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define WDR_LS_SWCH_ONr			(ValSft_R26(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_LS_SWCH_ONw(val)	_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define WDR_G_SWCH_ONr			(ValSft_R25(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_G_SWCH_ONw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define WDR_MOTION_ONr			(ValSft_R24(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define WDR_MOTION_ONw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LS_FLG_POST_Hr			(ValSft_R23(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_01)
#define LS_FLG_POST_Hw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define WDR_LD_MSTEPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_07)
#define WDR_LD_MSTEPw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define WDR_LOW_THr				(ValSft_R08(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_06)
#define WDR_LOW_THw(val)		_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define WDR_TSELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_05)
#define WDR_TSELw(val)			_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define WDR_SATVr				(ValSft_R16(_rd32(_REG_BASE_+(0x004a<<2)))&BitMask_10)
#define WDR_SATVw(val)			_wr32(_REG_BASE_+(0x004a<<2),(UINT)((_rd32(_REG_BASE_+(0x004a<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define WDR_SWGTr				(ValSft_R00(_rd32(_REG_BASE_+(0x004a<<2)))&BitMask_10)
#define WDR_SWGTw(val)			_wr32(_REG_BASE_+(0x004a<<2),(UINT)((_rd32(_REG_BASE_+(0x004a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_LSHUTr				(ValSft_R16(_rd32(_REG_BASE_+(0x004b<<2)))&BitMask_12)
#define WDR_LSHUTw(val)			_wr32(_REG_BASE_+(0x004b<<2),(UINT)((_rd32(_REG_BASE_+(0x004b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define WDR_SSHUTr				(ValSft_R00(_rd32(_REG_BASE_+(0x004b<<2)))&BitMask_12)
#define WDR_SSHUTw(val)			_wr32(_REG_BASE_+(0x004b<<2),(UINT)((_rd32(_REG_BASE_+(0x004b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_SGM_RANGEr			(ValSft_R16(_rd32(_REG_BASE_+(0x004c<<2)))&BitMask_08)
#define WDR_SGM_RANGEw(val)		_wr32(_REG_BASE_+(0x004c<<2),(UINT)((_rd32(_REG_BASE_+(0x004c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define WDR_SCH_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x004c<<2)))&BitMask_10)
#define WDR_SCH_MAXw(val)		_wr32(_REG_BASE_+(0x004c<<2),(UINT)((_rd32(_REG_BASE_+(0x004c<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_LS_MSTEPr			(ValSft_R16(_rd32(_REG_BASE_+(0x004d<<2)))&BitMask_09)
#define WDR_LS_MSTEPw(val)		_wr32(_REG_BASE_+(0x004d<<2),(UINT)((_rd32(_REG_BASE_+(0x004d<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define WDR_BIT_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x004d<<2)))&BitMask_10)
#define WDR_BIT_MAXw(val)		_wr32(_REG_BASE_+(0x004d<<2),(UINT)((_rd32(_REG_BASE_+(0x004d<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_HIGH_CNT_THr		(ValSft_R00(_rd32(_REG_BASE_+(0x004e<<2)))&BitMask_21)
#define WDR_HIGH_CNT_THw(val)	_wr32(_REG_BASE_+(0x004e<<2),(UINT)((_rd32(_REG_BASE_+(0x004e<<2))&(~ValSft_L00(BitMask_21)))|ValSft_L00((val)&BitMask_21)))

#define WDR_SL_R_IIRr			(ValSft_R24(_rd32(_REG_BASE_+(0x004f<<2)))&BitMask_08)
#define WDR_SL_R_IIRw(val)		_wr32(_REG_BASE_+(0x004f<<2),(UINT)((_rd32(_REG_BASE_+(0x004f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define WDR_LOW_CNT_THr			(ValSft_R00(_rd32(_REG_BASE_+(0x004f<<2)))&BitMask_21)
#define WDR_LOW_CNT_THw(val)	_wr32(_REG_BASE_+(0x004f<<2),(UINT)((_rd32(_REG_BASE_+(0x004f<<2))&(~ValSft_L00(BitMask_21)))|ValSft_L00((val)&BitMask_21)))

#define WDR_SR_GAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x0050<<2)))&BitMask_11)
#define WDR_SR_GAINw(val)		_wr32(_REG_BASE_+(0x0050<<2),(UINT)((_rd32(_REG_BASE_+(0x0050<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define WDR_SB_GAINr			(ValSft_R00(_rd32(_REG_BASE_+(0x0050<<2)))&BitMask_11)
#define WDR_SB_GAINw(val)		_wr32(_REG_BASE_+(0x0050<<2),(UINT)((_rd32(_REG_BASE_+(0x0050<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define WDR_BACKGND_SELr		(ValSft_R30(_rd32(_REG_BASE_+(0x0051<<2)))&BitMask_02)
#define WDR_BACKGND_SELw(val)	_wr32(_REG_BASE_+(0x0051<<2),(UINT)((_rd32(_REG_BASE_+(0x0051<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define WDR_WINDOW_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0051<<2)))&BitMask_02)
#define WDR_WINDOW_SELw(val)	_wr32(_REG_BASE_+(0x0051<<2),(UINT)((_rd32(_REG_BASE_+(0x0051<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define WDR_WND_ONr				(ValSft_R24(_rd32(_REG_BASE_+(0x0051<<2)))&BitMask_04)
#define WDR_WND_ONw(val)		_wr32(_REG_BASE_+(0x0051<<2),(UINT)((_rd32(_REG_BASE_+(0x0051<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define WDR_WND0_HSPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0051<<2)))&BitMask_12)
#define WDR_WND0_HSPw(val)		_wr32(_REG_BASE_+(0x0051<<2),(UINT)((_rd32(_REG_BASE_+(0x0051<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND0_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0051<<2)))&BitMask_12)
#define WDR_WND0_VSPw(val)		_wr32(_REG_BASE_+(0x0051<<2),(UINT)((_rd32(_REG_BASE_+(0x0051<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND0_HEPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0052<<2)))&BitMask_12)
#define WDR_WND0_HEPw(val)		_wr32(_REG_BASE_+(0x0052<<2),(UINT)((_rd32(_REG_BASE_+(0x0052<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND0_VEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0052<<2)))&BitMask_12)
#define WDR_WND0_VEPw(val)		_wr32(_REG_BASE_+(0x0052<<2),(UINT)((_rd32(_REG_BASE_+(0x0052<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND1_HSPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0053<<2)))&BitMask_12)
#define WDR_WND1_HSPw(val)		_wr32(_REG_BASE_+(0x0053<<2),(UINT)((_rd32(_REG_BASE_+(0x0053<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND1_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0053<<2)))&BitMask_12)
#define WDR_WND1_VSPw(val)		_wr32(_REG_BASE_+(0x0053<<2),(UINT)((_rd32(_REG_BASE_+(0x0053<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND1_HEPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0054<<2)))&BitMask_12)
#define WDR_WND1_HEPw(val)		_wr32(_REG_BASE_+(0x0054<<2),(UINT)((_rd32(_REG_BASE_+(0x0054<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND1_VEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0054<<2)))&BitMask_12)
#define WDR_WND1_VEPw(val)		_wr32(_REG_BASE_+(0x0054<<2),(UINT)((_rd32(_REG_BASE_+(0x0054<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND2_HSPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0055<<2)))&BitMask_12)
#define WDR_WND2_HSPw(val)		_wr32(_REG_BASE_+(0x0055<<2),(UINT)((_rd32(_REG_BASE_+(0x0055<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND2_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0055<<2)))&BitMask_12)
#define WDR_WND2_VSPw(val)		_wr32(_REG_BASE_+(0x0055<<2),(UINT)((_rd32(_REG_BASE_+(0x0055<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND2_HEPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0056<<2)))&BitMask_12)
#define WDR_WND2_HEPw(val)		_wr32(_REG_BASE_+(0x0056<<2),(UINT)((_rd32(_REG_BASE_+(0x0056<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND2_VEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0056<<2)))&BitMask_12)
#define WDR_WND2_VEPw(val)		_wr32(_REG_BASE_+(0x0056<<2),(UINT)((_rd32(_REG_BASE_+(0x0056<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND3_HSPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0057<<2)))&BitMask_12)
#define WDR_WND3_HSPw(val)		_wr32(_REG_BASE_+(0x0057<<2),(UINT)((_rd32(_REG_BASE_+(0x0057<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND3_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0057<<2)))&BitMask_12)
#define WDR_WND3_VSPw(val)		_wr32(_REG_BASE_+(0x0057<<2),(UINT)((_rd32(_REG_BASE_+(0x0057<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_WND3_HEPr			(ValSft_R12(_rd32(_REG_BASE_+(0x0058<<2)))&BitMask_12)
#define WDR_WND3_HEPw(val)		_wr32(_REG_BASE_+(0x0058<<2),(UINT)((_rd32(_REG_BASE_+(0x0058<<2))&(~ValSft_L12(BitMask_12)))|ValSft_L12((val)&BitMask_12)))

#define WDR_WND3_VEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0058<<2)))&BitMask_12)
#define WDR_WND3_VEPw(val)		_wr32(_REG_BASE_+(0x0058<<2),(UINT)((_rd32(_REG_BASE_+(0x0058<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define IMD_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_01)
#define IMD_ONw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_FLTr				(ValSft_R30(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_01)
#define IMD_FLTw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMD_TESTr				(ValSft_R28(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_02)
#define IMD_TESTw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define IMD_FTTYPEr				(ValSft_R27(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_01)
#define IMD_FTTYPEw(val)		_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IMD_COLORr				(ValSft_R24(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_03)
#define IMD_COLORw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IMD_THr					(ValSft_R16(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_08)
#define IMD_THw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IMD_HWr					(ValSft_R08(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_07)
#define IMD_HWw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_ISELr				(ValSft_R06(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_02)
#define IMD_ISELw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define IMD_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_06)
#define IMD_VWw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define IMD_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0061<<2)))&BitMask_12)
#define IMD_HSPw(val)			_wr32(_REG_BASE_+(0x0061<<2),(UINT)((_rd32(_REG_BASE_+(0x0061<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define IMD_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0061<<2)))&BitMask_11)
#define IMD_VSPw(val)			_wr32(_REG_BASE_+(0x0061<<2),(UINT)((_rd32(_REG_BASE_+(0x0061<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define IMD_KMAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_08)
#define IMD_KMAXw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define IMD_KMINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_08)
#define IMD_KMINw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IMD_KGAr				(ValSft_R08(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_08)
#define IMD_KGAw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define IMD_KTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_08)
#define IMD_KTHw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define IMD_BUPCr				(ValSft_R24(_rd32(_REG_BASE_+(0x0063<<2)))&BitMask_08)
#define IMD_BUPCw(val)			_wr32(_REG_BASE_+(0x0063<<2),(UINT)((_rd32(_REG_BASE_+(0x0063<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define IMD_AUPCr				(ValSft_R16(_rd32(_REG_BASE_+(0x0063<<2)))&BitMask_08)
#define IMD_AUPCw(val)			_wr32(_REG_BASE_+(0x0063<<2),(UINT)((_rd32(_REG_BASE_+(0x0063<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IMD_BDWr				(ValSft_R31(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_01)
#define IMD_BDWw(val)			_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_GMAr				(ValSft_R30(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_01)
#define IMD_GMAw(val)			_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMD_OSDONr				(ValSft_R24(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_06)
#define IMD_OSDONw(val)			_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define IMD_OSDLVr				(ValSft_R14(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_10)
#define IMD_OSDLVw(val)			_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L14(BitMask_10)))|ValSft_L14((val)&BitMask_10)))

#define IMD_IIRONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_01)
#define IMD_IIRONw(val)			_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_IIRAUTOr			(ValSft_R30(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_01)
#define IMD_IIRAUTOw(val)		_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMD_ACCONr				(ValSft_R29(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_01)
#define IMD_ACCONw(val)			_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IMD_ACCTHr				(ValSft_R16(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_07)
#define IMD_ACCTHw(val)			_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_ACCMAXr				(ValSft_R08(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_07)
#define IMD_ACCMAXw(val)		_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_ACCMINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_07)
#define IMD_ACCMINw(val)		_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_ACCINCr				(ValSft_R24(_rd32(_REG_BASE_+(0x0066<<2)))&BitMask_07)
#define IMD_ACCINCw(val)		_wr32(_REG_BASE_+(0x0066<<2),(UINT)((_rd32(_REG_BASE_+(0x0066<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_ACCDECr				(ValSft_R16(_rd32(_REG_BASE_+(0x0066<<2)))&BitMask_07)
#define IMD_ACCDECw(val)		_wr32(_REG_BASE_+(0x0066<<2),(UINT)((_rd32(_REG_BASE_+(0x0066<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_IIRKr				(ValSft_R08(_rd32(_REG_BASE_+(0x0066<<2)))&BitMask_08)
#define IMD_IIRKw(val)			_wr32(_REG_BASE_+(0x0066<<2),(UINT)((_rd32(_REG_BASE_+(0x0066<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define IMD_NUMTHr				(ValSft_R24(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_08)
#define IMD_NUMTHw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define IMD_HTHr				(ValSft_R16(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_06)
#define IMD_HTHw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L16(BitMask_06)))|ValSft_L16((val)&BitMask_06)))

#define IMD_VTHr				(ValSft_R08(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_06)
#define IMD_VTHw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define IMD_WON1r				(ValSft_R31(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_01)
#define IMD_WON1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WHSP1r				(ValSft_R24(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_07)
#define IMD_WHSP1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WHEP1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_07)
#define IMD_WHEP1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WVSP1r				(ValSft_R08(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_06)
#define IMD_WVSP1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define IMD_WVEP1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_06)
#define IMD_WVEP1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define IMD_WON2r				(ValSft_R31(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_01)
#define IMD_WON2w(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WHSP2r				(ValSft_R24(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_07)
#define IMD_WHSP2w(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WHEP2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_07)
#define IMD_WHEP2w(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WVSP2r				(ValSft_R08(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_06)
#define IMD_WVSP2w(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define IMD_WVEP2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_06)
#define IMD_WVEP2w(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define IMD_WON3r				(ValSft_R31(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_01)
#define IMD_WON3w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WHSP3r				(ValSft_R24(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_07)
#define IMD_WHSP3w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WHEP3r				(ValSft_R16(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_07)
#define IMD_WHEP3w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WVSP3r				(ValSft_R08(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_06)
#define IMD_WVSP3w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define IMD_WVEP3r				(ValSft_R00(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_06)
#define IMD_WVEP3w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define IMD_WON4r				(ValSft_R31(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_01)
#define IMD_WON4w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WHSP4r				(ValSft_R24(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_07)
#define IMD_WHSP4w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WHEP4r				(ValSft_R16(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_07)
#define IMD_WHEP4w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WVSP4r				(ValSft_R08(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_06)
#define IMD_WVSP4w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define IMD_WVEP4r				(ValSft_R00(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_06)
#define IMD_WVEP4w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define OZONE_SELr				(ValSft_R28(_rd32(_REG_BASE_+(0x006d<<2)))&BitMask_03)
#define OZONE_SELw(val)			_wr32(_REG_BASE_+(0x006d<<2),(UINT)((_rd32(_REG_BASE_+(0x006d<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define AE_LOCK_SELr			(ValSft_R27(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_01)
#define AE_LOCK_SELw(val)		_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define AF_YSELr				(ValSft_R25(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_02)
#define AF_YSELw(val)			_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L25(BitMask_02)))|ValSft_L25((val)&BitMask_02)))

#define AF_YLPFr				(ValSft_R24(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_01)
#define AF_YLPFw(val)			_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define AE_WIN_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_04)
#define AE_WIN_SELw(val)		_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define AE1_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0070<<2)))&BitMask_12)
#define AE1_HSPw(val)			_wr32(_REG_BASE_+(0x0070<<2),(UINT)((_rd32(_REG_BASE_+(0x0070<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE1_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0070<<2)))&BitMask_12)
#define AE1_HEPw(val)			_wr32(_REG_BASE_+(0x0070<<2),(UINT)((_rd32(_REG_BASE_+(0x0070<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE1_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0071<<2)))&BitMask_12)
#define AE1_VSPw(val)			_wr32(_REG_BASE_+(0x0071<<2),(UINT)((_rd32(_REG_BASE_+(0x0071<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE1_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0071<<2)))&BitMask_12)
#define AE1_VEPw(val)			_wr32(_REG_BASE_+(0x0071<<2),(UINT)((_rd32(_REG_BASE_+(0x0071<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE2_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0072<<2)))&BitMask_12)
#define AE2_HSPw(val)			_wr32(_REG_BASE_+(0x0072<<2),(UINT)((_rd32(_REG_BASE_+(0x0072<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE2_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0072<<2)))&BitMask_12)
#define AE2_HEPw(val)			_wr32(_REG_BASE_+(0x0072<<2),(UINT)((_rd32(_REG_BASE_+(0x0072<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE2_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0073<<2)))&BitMask_12)
#define AE2_VSPw(val)			_wr32(_REG_BASE_+(0x0073<<2),(UINT)((_rd32(_REG_BASE_+(0x0073<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE2_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0073<<2)))&BitMask_12)
#define AE2_VEPw(val)			_wr32(_REG_BASE_+(0x0073<<2),(UINT)((_rd32(_REG_BASE_+(0x0073<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE3_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0074<<2)))&BitMask_12)
#define AE3_HSPw(val)			_wr32(_REG_BASE_+(0x0074<<2),(UINT)((_rd32(_REG_BASE_+(0x0074<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE3_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0074<<2)))&BitMask_12)
#define AE3_HEPw(val)			_wr32(_REG_BASE_+(0x0074<<2),(UINT)((_rd32(_REG_BASE_+(0x0074<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE3_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0075<<2)))&BitMask_12)
#define AE3_VSPw(val)			_wr32(_REG_BASE_+(0x0075<<2),(UINT)((_rd32(_REG_BASE_+(0x0075<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE3_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0075<<2)))&BitMask_12)
#define AE3_VEPw(val)			_wr32(_REG_BASE_+(0x0075<<2),(UINT)((_rd32(_REG_BASE_+(0x0075<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE4_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0076<<2)))&BitMask_12)
#define AE4_HSPw(val)			_wr32(_REG_BASE_+(0x0076<<2),(UINT)((_rd32(_REG_BASE_+(0x0076<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE4_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0076<<2)))&BitMask_12)
#define AE4_HEPw(val)			_wr32(_REG_BASE_+(0x0076<<2),(UINT)((_rd32(_REG_BASE_+(0x0076<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE4_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0077<<2)))&BitMask_12)
#define AE4_VSPw(val)			_wr32(_REG_BASE_+(0x0077<<2),(UINT)((_rd32(_REG_BASE_+(0x0077<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE4_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0077<<2)))&BitMask_12)
#define AE4_VEPw(val)			_wr32(_REG_BASE_+(0x0077<<2),(UINT)((_rd32(_REG_BASE_+(0x0077<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE1_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0078<<2)))&BitMask_10)
#define AE1_CLIPw(val)			_wr32(_REG_BASE_+(0x0078<<2),(UINT)((_rd32(_REG_BASE_+(0x0078<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AE1_SLICEr				(ValSft_R16(_rd32(_REG_BASE_+(0x0078<<2)))&BitMask_10)
#define AE1_SLICEw(val)			_wr32(_REG_BASE_+(0x0078<<2),(UINT)((_rd32(_REG_BASE_+(0x0078<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AE2_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_10)
#define AE2_CLIPw(val)			_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AE2_SLICEr				(ValSft_R16(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_10)
#define AE2_SLICEw(val)			_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AE3_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_10)
#define AE3_CLIPw(val)			_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AE3_SLICEr				(ValSft_R16(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_10)
#define AE3_SLICEw(val)			_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AE4_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_10)
#define AE4_CLIPw(val)			_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AE4_SLICEr				(ValSft_R16(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_10)
#define AE4_SLICEw(val)			_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define HISTO0_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_07)
#define HISTO0_MAXw(val)		_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO1_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_07)
#define HISTO1_MAXw(val)		_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO2_MAXr				(ValSft_R08(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_07)
#define HISTO2_MAXw(val)		_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO3_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_07)
#define HISTO3_MAXw(val)		_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO4_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x007d<<2)))&BitMask_07)
#define HISTO4_MAXw(val)		_wr32(_REG_BASE_+(0x007d<<2),(UINT)((_rd32(_REG_BASE_+(0x007d<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO5_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x007d<<2)))&BitMask_07)
#define HISTO5_MAXw(val)		_wr32(_REG_BASE_+(0x007d<<2),(UINT)((_rd32(_REG_BASE_+(0x007d<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO6_MAXr				(ValSft_R08(_rd32(_REG_BASE_+(0x007d<<2)))&BitMask_07)
#define HISTO6_MAXw(val)		_wr32(_REG_BASE_+(0x007d<<2),(UINT)((_rd32(_REG_BASE_+(0x007d<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO7_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x007d<<2)))&BitMask_07)
#define HISTO7_MAXw(val)		_wr32(_REG_BASE_+(0x007d<<2),(UINT)((_rd32(_REG_BASE_+(0x007d<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO8_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x007e<<2)))&BitMask_07)
#define HISTO8_MAXw(val)		_wr32(_REG_BASE_+(0x007e<<2),(UINT)((_rd32(_REG_BASE_+(0x007e<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO9_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x007e<<2)))&BitMask_07)
#define HISTO9_MAXw(val)		_wr32(_REG_BASE_+(0x007e<<2),(UINT)((_rd32(_REG_BASE_+(0x007e<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO10_MAXr			(ValSft_R08(_rd32(_REG_BASE_+(0x007e<<2)))&BitMask_07)
#define HISTO10_MAXw(val)		_wr32(_REG_BASE_+(0x007e<<2),(UINT)((_rd32(_REG_BASE_+(0x007e<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO11_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x007e<<2)))&BitMask_07)
#define HISTO11_MAXw(val)		_wr32(_REG_BASE_+(0x007e<<2),(UINT)((_rd32(_REG_BASE_+(0x007e<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO12_MAXr			(ValSft_R24(_rd32(_REG_BASE_+(0x007f<<2)))&BitMask_07)
#define HISTO12_MAXw(val)		_wr32(_REG_BASE_+(0x007f<<2),(UINT)((_rd32(_REG_BASE_+(0x007f<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO13_MAXr			(ValSft_R16(_rd32(_REG_BASE_+(0x007f<<2)))&BitMask_07)
#define HISTO13_MAXw(val)		_wr32(_REG_BASE_+(0x007f<<2),(UINT)((_rd32(_REG_BASE_+(0x007f<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO14_MAXr			(ValSft_R08(_rd32(_REG_BASE_+(0x007f<<2)))&BitMask_07)
#define HISTO14_MAXw(val)		_wr32(_REG_BASE_+(0x007f<<2),(UINT)((_rd32(_REG_BASE_+(0x007f<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define AWB_WIN1_HSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_12)
#define AWB_WIN1_HSPw(val)		_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWB_WIN1_HWr			(ValSft_R00(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_12)
#define AWB_WIN1_HWw(val)		_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWB_WIN1_VSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_12)
#define AWB_WIN1_VSPw(val)		_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWB_WIN1_VWr			(ValSft_R00(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_12)
#define AWB_WIN1_VWw(val)		_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWBL_DBGAIN0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0082<<2)))&BitMask_08)
#define AWBL_DBGAIN0w(val)		_wr32(_REG_BASE_+(0x0082<<2),(UINT)((_rd32(_REG_BASE_+(0x0082<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBL_DRGAIN0r			(ValSft_R08(_rd32(_REG_BASE_+(0x0082<<2)))&BitMask_08)
#define AWBL_DRGAIN0w(val)		_wr32(_REG_BASE_+(0x0082<<2),(UINT)((_rd32(_REG_BASE_+(0x0082<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define AWBL0_DBOFFSETr			(ValSft_R16(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_10)
#define AWBL0_DBOFFSETw(val)	_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AWBL0_DROFFSETr			(ValSft_R00(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_10)
#define AWBL0_DROFFSETw(val)	_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AWBLCBCRO_SELr			(ValSft_R30(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_01)
#define AWBLCBCRO_SELw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define AWBLNOR_SELr			(ValSft_R29(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_01)
#define AWBLNOR_SELw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define AWB_LOCK_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_01)
#define AWB_LOCK_SELw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define AWBL_HTAB_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_02)
#define AWBL_HTAB_SELw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define AWBLYEDGE_THr			(ValSft_R00(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_10)
#define AWBLYEDGE_THw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AWBLOSD_ONr				(ValSft_R08(_rd32(_REG_BASE_+(0x0085<<2)))&BitMask_01)
#define AWBLOSD_ONw(val)		_wr32(_REG_BASE_+(0x0085<<2),(UINT)((_rd32(_REG_BASE_+(0x0085<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define AWBL_OSD_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0085<<2)))&BitMask_08)
#define AWBL_OSD_SELw(val)		_wr32(_REG_BASE_+(0x0085<<2),(UINT)((_rd32(_REG_BASE_+(0x0085<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBL_CYCLIPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0086<<2)))&BitMask_10)
#define AWBL_CYCLIPw(val)		_wr32(_REG_BASE_+(0x0086<<2),(UINT)((_rd32(_REG_BASE_+(0x0086<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AWBL_CYSLICEr			(ValSft_R00(_rd32(_REG_BASE_+(0x0086<<2)))&BitMask_10)
#define AWBL_CYSLICEw(val)		_wr32(_REG_BASE_+(0x0086<<2),(UINT)((_rd32(_REG_BASE_+(0x0086<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AWBL_CM_HSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0087<<2)))&BitMask_12)
#define AWBL_CM_HSPw(val)		_wr32(_REG_BASE_+(0x0087<<2),(UINT)((_rd32(_REG_BASE_+(0x0087<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWBL_CM_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0087<<2)))&BitMask_12)
#define AWBL_CM_VSPw(val)		_wr32(_REG_BASE_+(0x0087<<2),(UINT)((_rd32(_REG_BASE_+(0x0087<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWBL_CM00_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0088<<2)))&BitMask_06)
#define AWBL_CM00_HSPw(val)		_wr32(_REG_BASE_+(0x0088<<2),(UINT)((_rd32(_REG_BASE_+(0x0088<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM00_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0088<<2)))&BitMask_06)
#define AWBL_CM00_HEPw(val)		_wr32(_REG_BASE_+(0x0088<<2),(UINT)((_rd32(_REG_BASE_+(0x0088<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM01_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0089<<2)))&BitMask_06)
#define AWBL_CM01_HSPw(val)		_wr32(_REG_BASE_+(0x0089<<2),(UINT)((_rd32(_REG_BASE_+(0x0089<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM01_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0089<<2)))&BitMask_06)
#define AWBL_CM01_HEPw(val)		_wr32(_REG_BASE_+(0x0089<<2),(UINT)((_rd32(_REG_BASE_+(0x0089<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM02_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008a<<2)))&BitMask_06)
#define AWBL_CM02_HSPw(val)		_wr32(_REG_BASE_+(0x008a<<2),(UINT)((_rd32(_REG_BASE_+(0x008a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM02_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008a<<2)))&BitMask_06)
#define AWBL_CM02_HEPw(val)		_wr32(_REG_BASE_+(0x008a<<2),(UINT)((_rd32(_REG_BASE_+(0x008a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM03_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008b<<2)))&BitMask_06)
#define AWBL_CM03_HSPw(val)		_wr32(_REG_BASE_+(0x008b<<2),(UINT)((_rd32(_REG_BASE_+(0x008b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM03_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008b<<2)))&BitMask_06)
#define AWBL_CM03_HEPw(val)		_wr32(_REG_BASE_+(0x008b<<2),(UINT)((_rd32(_REG_BASE_+(0x008b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM04_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008c<<2)))&BitMask_06)
#define AWBL_CM04_HSPw(val)		_wr32(_REG_BASE_+(0x008c<<2),(UINT)((_rd32(_REG_BASE_+(0x008c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM04_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008c<<2)))&BitMask_06)
#define AWBL_CM04_HEPw(val)		_wr32(_REG_BASE_+(0x008c<<2),(UINT)((_rd32(_REG_BASE_+(0x008c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM05_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008d<<2)))&BitMask_06)
#define AWBL_CM05_HSPw(val)		_wr32(_REG_BASE_+(0x008d<<2),(UINT)((_rd32(_REG_BASE_+(0x008d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM05_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008d<<2)))&BitMask_06)
#define AWBL_CM05_HEPw(val)		_wr32(_REG_BASE_+(0x008d<<2),(UINT)((_rd32(_REG_BASE_+(0x008d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM06_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_06)
#define AWBL_CM06_HSPw(val)		_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM06_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_06)
#define AWBL_CM06_HEPw(val)		_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM07_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x008f<<2)))&BitMask_06)
#define AWBL_CM07_HSPw(val)		_wr32(_REG_BASE_+(0x008f<<2),(UINT)((_rd32(_REG_BASE_+(0x008f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM07_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x008f<<2)))&BitMask_06)
#define AWBL_CM07_HEPw(val)		_wr32(_REG_BASE_+(0x008f<<2),(UINT)((_rd32(_REG_BASE_+(0x008f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM08_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0090<<2)))&BitMask_06)
#define AWBL_CM08_HSPw(val)		_wr32(_REG_BASE_+(0x0090<<2),(UINT)((_rd32(_REG_BASE_+(0x0090<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM08_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0090<<2)))&BitMask_06)
#define AWBL_CM08_HEPw(val)		_wr32(_REG_BASE_+(0x0090<<2),(UINT)((_rd32(_REG_BASE_+(0x0090<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM09_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0091<<2)))&BitMask_06)
#define AWBL_CM09_HSPw(val)		_wr32(_REG_BASE_+(0x0091<<2),(UINT)((_rd32(_REG_BASE_+(0x0091<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM09_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0091<<2)))&BitMask_06)
#define AWBL_CM09_HEPw(val)		_wr32(_REG_BASE_+(0x0091<<2),(UINT)((_rd32(_REG_BASE_+(0x0091<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM10_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0092<<2)))&BitMask_06)
#define AWBL_CM10_HSPw(val)		_wr32(_REG_BASE_+(0x0092<<2),(UINT)((_rd32(_REG_BASE_+(0x0092<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM10_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0092<<2)))&BitMask_06)
#define AWBL_CM10_HEPw(val)		_wr32(_REG_BASE_+(0x0092<<2),(UINT)((_rd32(_REG_BASE_+(0x0092<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM11_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0093<<2)))&BitMask_06)
#define AWBL_CM11_HSPw(val)		_wr32(_REG_BASE_+(0x0093<<2),(UINT)((_rd32(_REG_BASE_+(0x0093<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM11_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0093<<2)))&BitMask_06)
#define AWBL_CM11_HEPw(val)		_wr32(_REG_BASE_+(0x0093<<2),(UINT)((_rd32(_REG_BASE_+(0x0093<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM12_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0094<<2)))&BitMask_06)
#define AWBL_CM12_HSPw(val)		_wr32(_REG_BASE_+(0x0094<<2),(UINT)((_rd32(_REG_BASE_+(0x0094<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM12_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0094<<2)))&BitMask_06)
#define AWBL_CM12_HEPw(val)		_wr32(_REG_BASE_+(0x0094<<2),(UINT)((_rd32(_REG_BASE_+(0x0094<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM13_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_06)
#define AWBL_CM13_HSPw(val)		_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM13_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_06)
#define AWBL_CM13_HEPw(val)		_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM14_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0096<<2)))&BitMask_06)
#define AWBL_CM14_HSPw(val)		_wr32(_REG_BASE_+(0x0096<<2),(UINT)((_rd32(_REG_BASE_+(0x0096<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM14_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0096<<2)))&BitMask_06)
#define AWBL_CM14_HEPw(val)		_wr32(_REG_BASE_+(0x0096<<2),(UINT)((_rd32(_REG_BASE_+(0x0096<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM15_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_06)
#define AWBL_CM15_HSPw(val)		_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM15_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_06)
#define AWBL_CM15_HEPw(val)		_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM16_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_06)
#define AWBL_CM16_HSPw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM16_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_06)
#define AWBL_CM16_HEPw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM17_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0099<<2)))&BitMask_06)
#define AWBL_CM17_HSPw(val)		_wr32(_REG_BASE_+(0x0099<<2),(UINT)((_rd32(_REG_BASE_+(0x0099<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM17_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0099<<2)))&BitMask_06)
#define AWBL_CM17_HEPw(val)		_wr32(_REG_BASE_+(0x0099<<2),(UINT)((_rd32(_REG_BASE_+(0x0099<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM18_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009a<<2)))&BitMask_06)
#define AWBL_CM18_HSPw(val)		_wr32(_REG_BASE_+(0x009a<<2),(UINT)((_rd32(_REG_BASE_+(0x009a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM18_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009a<<2)))&BitMask_06)
#define AWBL_CM18_HEPw(val)		_wr32(_REG_BASE_+(0x009a<<2),(UINT)((_rd32(_REG_BASE_+(0x009a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM19_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_06)
#define AWBL_CM19_HSPw(val)		_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM19_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_06)
#define AWBL_CM19_HEPw(val)		_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM20_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009c<<2)))&BitMask_06)
#define AWBL_CM20_HSPw(val)		_wr32(_REG_BASE_+(0x009c<<2),(UINT)((_rd32(_REG_BASE_+(0x009c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM20_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009c<<2)))&BitMask_06)
#define AWBL_CM20_HEPw(val)		_wr32(_REG_BASE_+(0x009c<<2),(UINT)((_rd32(_REG_BASE_+(0x009c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM21_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009d<<2)))&BitMask_06)
#define AWBL_CM21_HSPw(val)		_wr32(_REG_BASE_+(0x009d<<2),(UINT)((_rd32(_REG_BASE_+(0x009d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM21_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009d<<2)))&BitMask_06)
#define AWBL_CM21_HEPw(val)		_wr32(_REG_BASE_+(0x009d<<2),(UINT)((_rd32(_REG_BASE_+(0x009d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM22_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009e<<2)))&BitMask_06)
#define AWBL_CM22_HSPw(val)		_wr32(_REG_BASE_+(0x009e<<2),(UINT)((_rd32(_REG_BASE_+(0x009e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM22_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009e<<2)))&BitMask_06)
#define AWBL_CM22_HEPw(val)		_wr32(_REG_BASE_+(0x009e<<2),(UINT)((_rd32(_REG_BASE_+(0x009e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM23_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x009f<<2)))&BitMask_06)
#define AWBL_CM23_HSPw(val)		_wr32(_REG_BASE_+(0x009f<<2),(UINT)((_rd32(_REG_BASE_+(0x009f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM23_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x009f<<2)))&BitMask_06)
#define AWBL_CM23_HEPw(val)		_wr32(_REG_BASE_+(0x009f<<2),(UINT)((_rd32(_REG_BASE_+(0x009f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM24_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_06)
#define AWBL_CM24_HSPw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM24_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_06)
#define AWBL_CM24_HEPw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM25_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a1<<2)))&BitMask_06)
#define AWBL_CM25_HSPw(val)		_wr32(_REG_BASE_+(0x00a1<<2),(UINT)((_rd32(_REG_BASE_+(0x00a1<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM25_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a1<<2)))&BitMask_06)
#define AWBL_CM25_HEPw(val)		_wr32(_REG_BASE_+(0x00a1<<2),(UINT)((_rd32(_REG_BASE_+(0x00a1<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM26_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a2<<2)))&BitMask_06)
#define AWBL_CM26_HSPw(val)		_wr32(_REG_BASE_+(0x00a2<<2),(UINT)((_rd32(_REG_BASE_+(0x00a2<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM26_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a2<<2)))&BitMask_06)
#define AWBL_CM26_HEPw(val)		_wr32(_REG_BASE_+(0x00a2<<2),(UINT)((_rd32(_REG_BASE_+(0x00a2<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM27_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a3<<2)))&BitMask_06)
#define AWBL_CM27_HSPw(val)		_wr32(_REG_BASE_+(0x00a3<<2),(UINT)((_rd32(_REG_BASE_+(0x00a3<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM27_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a3<<2)))&BitMask_06)
#define AWBL_CM27_HEPw(val)		_wr32(_REG_BASE_+(0x00a3<<2),(UINT)((_rd32(_REG_BASE_+(0x00a3<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM28_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a4<<2)))&BitMask_06)
#define AWBL_CM28_HSPw(val)		_wr32(_REG_BASE_+(0x00a4<<2),(UINT)((_rd32(_REG_BASE_+(0x00a4<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM28_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a4<<2)))&BitMask_06)
#define AWBL_CM28_HEPw(val)		_wr32(_REG_BASE_+(0x00a4<<2),(UINT)((_rd32(_REG_BASE_+(0x00a4<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM29_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a5<<2)))&BitMask_06)
#define AWBL_CM29_HSPw(val)		_wr32(_REG_BASE_+(0x00a5<<2),(UINT)((_rd32(_REG_BASE_+(0x00a5<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM29_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a5<<2)))&BitMask_06)
#define AWBL_CM29_HEPw(val)		_wr32(_REG_BASE_+(0x00a5<<2),(UINT)((_rd32(_REG_BASE_+(0x00a5<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM30_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a6<<2)))&BitMask_06)
#define AWBL_CM30_HSPw(val)		_wr32(_REG_BASE_+(0x00a6<<2),(UINT)((_rd32(_REG_BASE_+(0x00a6<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM30_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a6<<2)))&BitMask_06)
#define AWBL_CM30_HEPw(val)		_wr32(_REG_BASE_+(0x00a6<<2),(UINT)((_rd32(_REG_BASE_+(0x00a6<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM31_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a7<<2)))&BitMask_06)
#define AWBL_CM31_HSPw(val)		_wr32(_REG_BASE_+(0x00a7<<2),(UINT)((_rd32(_REG_BASE_+(0x00a7<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM31_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a7<<2)))&BitMask_06)
#define AWBL_CM31_HEPw(val)		_wr32(_REG_BASE_+(0x00a7<<2),(UINT)((_rd32(_REG_BASE_+(0x00a7<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM32_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a8<<2)))&BitMask_06)
#define AWBL_CM32_HSPw(val)		_wr32(_REG_BASE_+(0x00a8<<2),(UINT)((_rd32(_REG_BASE_+(0x00a8<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM32_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a8<<2)))&BitMask_06)
#define AWBL_CM32_HEPw(val)		_wr32(_REG_BASE_+(0x00a8<<2),(UINT)((_rd32(_REG_BASE_+(0x00a8<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM33_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00a9<<2)))&BitMask_06)
#define AWBL_CM33_HSPw(val)		_wr32(_REG_BASE_+(0x00a9<<2),(UINT)((_rd32(_REG_BASE_+(0x00a9<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM33_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a9<<2)))&BitMask_06)
#define AWBL_CM33_HEPw(val)		_wr32(_REG_BASE_+(0x00a9<<2),(UINT)((_rd32(_REG_BASE_+(0x00a9<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM34_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00aa<<2)))&BitMask_06)
#define AWBL_CM34_HSPw(val)		_wr32(_REG_BASE_+(0x00aa<<2),(UINT)((_rd32(_REG_BASE_+(0x00aa<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM34_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00aa<<2)))&BitMask_06)
#define AWBL_CM34_HEPw(val)		_wr32(_REG_BASE_+(0x00aa<<2),(UINT)((_rd32(_REG_BASE_+(0x00aa<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM35_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00ab<<2)))&BitMask_06)
#define AWBL_CM35_HSPw(val)		_wr32(_REG_BASE_+(0x00ab<<2),(UINT)((_rd32(_REG_BASE_+(0x00ab<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM35_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00ab<<2)))&BitMask_06)
#define AWBL_CM35_HEPw(val)		_wr32(_REG_BASE_+(0x00ab<<2),(UINT)((_rd32(_REG_BASE_+(0x00ab<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM36_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00ac<<2)))&BitMask_06)
#define AWBL_CM36_HSPw(val)		_wr32(_REG_BASE_+(0x00ac<<2),(UINT)((_rd32(_REG_BASE_+(0x00ac<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM36_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00ac<<2)))&BitMask_06)
#define AWBL_CM36_HEPw(val)		_wr32(_REG_BASE_+(0x00ac<<2),(UINT)((_rd32(_REG_BASE_+(0x00ac<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM37_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00ad<<2)))&BitMask_06)
#define AWBL_CM37_HSPw(val)		_wr32(_REG_BASE_+(0x00ad<<2),(UINT)((_rd32(_REG_BASE_+(0x00ad<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM37_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00ad<<2)))&BitMask_06)
#define AWBL_CM37_HEPw(val)		_wr32(_REG_BASE_+(0x00ad<<2),(UINT)((_rd32(_REG_BASE_+(0x00ad<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM38_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00ae<<2)))&BitMask_06)
#define AWBL_CM38_HSPw(val)		_wr32(_REG_BASE_+(0x00ae<<2),(UINT)((_rd32(_REG_BASE_+(0x00ae<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM38_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00ae<<2)))&BitMask_06)
#define AWBL_CM38_HEPw(val)		_wr32(_REG_BASE_+(0x00ae<<2),(UINT)((_rd32(_REG_BASE_+(0x00ae<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM39_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00af<<2)))&BitMask_06)
#define AWBL_CM39_HSPw(val)		_wr32(_REG_BASE_+(0x00af<<2),(UINT)((_rd32(_REG_BASE_+(0x00af<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM39_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00af<<2)))&BitMask_06)
#define AWBL_CM39_HEPw(val)		_wr32(_REG_BASE_+(0x00af<<2),(UINT)((_rd32(_REG_BASE_+(0x00af<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM40_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_06)
#define AWBL_CM40_HSPw(val)		_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM40_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_06)
#define AWBL_CM40_HEPw(val)		_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM41_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b1<<2)))&BitMask_06)
#define AWBL_CM41_HSPw(val)		_wr32(_REG_BASE_+(0x00b1<<2),(UINT)((_rd32(_REG_BASE_+(0x00b1<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM41_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b1<<2)))&BitMask_06)
#define AWBL_CM41_HEPw(val)		_wr32(_REG_BASE_+(0x00b1<<2),(UINT)((_rd32(_REG_BASE_+(0x00b1<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM42_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b2<<2)))&BitMask_06)
#define AWBL_CM42_HSPw(val)		_wr32(_REG_BASE_+(0x00b2<<2),(UINT)((_rd32(_REG_BASE_+(0x00b2<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM42_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b2<<2)))&BitMask_06)
#define AWBL_CM42_HEPw(val)		_wr32(_REG_BASE_+(0x00b2<<2),(UINT)((_rd32(_REG_BASE_+(0x00b2<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM43_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b3<<2)))&BitMask_06)
#define AWBL_CM43_HSPw(val)		_wr32(_REG_BASE_+(0x00b3<<2),(UINT)((_rd32(_REG_BASE_+(0x00b3<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM43_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b3<<2)))&BitMask_06)
#define AWBL_CM43_HEPw(val)		_wr32(_REG_BASE_+(0x00b3<<2),(UINT)((_rd32(_REG_BASE_+(0x00b3<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM44_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b4<<2)))&BitMask_06)
#define AWBL_CM44_HSPw(val)		_wr32(_REG_BASE_+(0x00b4<<2),(UINT)((_rd32(_REG_BASE_+(0x00b4<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM44_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b4<<2)))&BitMask_06)
#define AWBL_CM44_HEPw(val)		_wr32(_REG_BASE_+(0x00b4<<2),(UINT)((_rd32(_REG_BASE_+(0x00b4<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM45_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b5<<2)))&BitMask_06)
#define AWBL_CM45_HSPw(val)		_wr32(_REG_BASE_+(0x00b5<<2),(UINT)((_rd32(_REG_BASE_+(0x00b5<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM45_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b5<<2)))&BitMask_06)
#define AWBL_CM45_HEPw(val)		_wr32(_REG_BASE_+(0x00b5<<2),(UINT)((_rd32(_REG_BASE_+(0x00b5<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM46_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b6<<2)))&BitMask_06)
#define AWBL_CM46_HSPw(val)		_wr32(_REG_BASE_+(0x00b6<<2),(UINT)((_rd32(_REG_BASE_+(0x00b6<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM46_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b6<<2)))&BitMask_06)
#define AWBL_CM46_HEPw(val)		_wr32(_REG_BASE_+(0x00b6<<2),(UINT)((_rd32(_REG_BASE_+(0x00b6<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM47_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b7<<2)))&BitMask_06)
#define AWBL_CM47_HSPw(val)		_wr32(_REG_BASE_+(0x00b7<<2),(UINT)((_rd32(_REG_BASE_+(0x00b7<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM47_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b7<<2)))&BitMask_06)
#define AWBL_CM47_HEPw(val)		_wr32(_REG_BASE_+(0x00b7<<2),(UINT)((_rd32(_REG_BASE_+(0x00b7<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM48_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b8<<2)))&BitMask_06)
#define AWBL_CM48_HSPw(val)		_wr32(_REG_BASE_+(0x00b8<<2),(UINT)((_rd32(_REG_BASE_+(0x00b8<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM48_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b8<<2)))&BitMask_06)
#define AWBL_CM48_HEPw(val)		_wr32(_REG_BASE_+(0x00b8<<2),(UINT)((_rd32(_REG_BASE_+(0x00b8<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM49_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00b9<<2)))&BitMask_06)
#define AWBL_CM49_HSPw(val)		_wr32(_REG_BASE_+(0x00b9<<2),(UINT)((_rd32(_REG_BASE_+(0x00b9<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM49_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00b9<<2)))&BitMask_06)
#define AWBL_CM49_HEPw(val)		_wr32(_REG_BASE_+(0x00b9<<2),(UINT)((_rd32(_REG_BASE_+(0x00b9<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM50_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00ba<<2)))&BitMask_06)
#define AWBL_CM50_HSPw(val)		_wr32(_REG_BASE_+(0x00ba<<2),(UINT)((_rd32(_REG_BASE_+(0x00ba<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM50_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00ba<<2)))&BitMask_06)
#define AWBL_CM50_HEPw(val)		_wr32(_REG_BASE_+(0x00ba<<2),(UINT)((_rd32(_REG_BASE_+(0x00ba<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM51_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00bb<<2)))&BitMask_06)
#define AWBL_CM51_HSPw(val)		_wr32(_REG_BASE_+(0x00bb<<2),(UINT)((_rd32(_REG_BASE_+(0x00bb<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM51_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00bb<<2)))&BitMask_06)
#define AWBL_CM51_HEPw(val)		_wr32(_REG_BASE_+(0x00bb<<2),(UINT)((_rd32(_REG_BASE_+(0x00bb<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM52_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00bc<<2)))&BitMask_06)
#define AWBL_CM52_HSPw(val)		_wr32(_REG_BASE_+(0x00bc<<2),(UINT)((_rd32(_REG_BASE_+(0x00bc<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM52_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00bc<<2)))&BitMask_06)
#define AWBL_CM52_HEPw(val)		_wr32(_REG_BASE_+(0x00bc<<2),(UINT)((_rd32(_REG_BASE_+(0x00bc<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM53_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_06)
#define AWBL_CM53_HSPw(val)		_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM53_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_06)
#define AWBL_CM53_HEPw(val)		_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM54_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_06)
#define AWBL_CM54_HSPw(val)		_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM54_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_06)
#define AWBL_CM54_HEPw(val)		_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM55_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_06)
#define AWBL_CM55_HSPw(val)		_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM55_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_06)
#define AWBL_CM55_HEPw(val)		_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM56_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_06)
#define AWBL_CM56_HSPw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM56_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_06)
#define AWBL_CM56_HEPw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM57_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_06)
#define AWBL_CM57_HSPw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM57_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_06)
#define AWBL_CM57_HEPw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM58_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_06)
#define AWBL_CM58_HSPw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM58_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_06)
#define AWBL_CM58_HEPw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM59_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c3<<2)))&BitMask_06)
#define AWBL_CM59_HSPw(val)		_wr32(_REG_BASE_+(0x00c3<<2),(UINT)((_rd32(_REG_BASE_+(0x00c3<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM59_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c3<<2)))&BitMask_06)
#define AWBL_CM59_HEPw(val)		_wr32(_REG_BASE_+(0x00c3<<2),(UINT)((_rd32(_REG_BASE_+(0x00c3<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM60_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c4<<2)))&BitMask_06)
#define AWBL_CM60_HSPw(val)		_wr32(_REG_BASE_+(0x00c4<<2),(UINT)((_rd32(_REG_BASE_+(0x00c4<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM60_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c4<<2)))&BitMask_06)
#define AWBL_CM60_HEPw(val)		_wr32(_REG_BASE_+(0x00c4<<2),(UINT)((_rd32(_REG_BASE_+(0x00c4<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM61_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_06)
#define AWBL_CM61_HSPw(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM61_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_06)
#define AWBL_CM61_HEPw(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM62_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_06)
#define AWBL_CM62_HSPw(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM62_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_06)
#define AWBL_CM62_HEPw(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM63_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_06)
#define AWBL_CM63_HSPw(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM63_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_06)
#define AWBL_CM63_HEPw(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define PRR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00c8<<2)))&BitMask_11)
#define PRR_GAINw(val)			_wr32(_REG_BASE_+(0x00c8<<2),(UINT)((_rd32(_REG_BASE_+(0x00c8<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PRG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00c8<<2)))&BitMask_12)
#define PRG_GAINw(val)			_wr32(_REG_BASE_+(0x00c8<<2),(UINT)((_rd32(_REG_BASE_+(0x00c8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PRB_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00c9<<2)))&BitMask_12)
#define PRB_GAINw(val)			_wr32(_REG_BASE_+(0x00c9<<2),(UINT)((_rd32(_REG_BASE_+(0x00c9<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PGR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00c9<<2)))&BitMask_12)
#define PGR_GAINw(val)			_wr32(_REG_BASE_+(0x00c9<<2),(UINT)((_rd32(_REG_BASE_+(0x00c9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PGG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ca<<2)))&BitMask_11)
#define PGG_GAINw(val)			_wr32(_REG_BASE_+(0x00ca<<2),(UINT)((_rd32(_REG_BASE_+(0x00ca<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PGB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ca<<2)))&BitMask_12)
#define PGB_GAINw(val)			_wr32(_REG_BASE_+(0x00ca<<2),(UINT)((_rd32(_REG_BASE_+(0x00ca<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PBR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00cb<<2)))&BitMask_12)
#define PBR_GAINw(val)			_wr32(_REG_BASE_+(0x00cb<<2),(UINT)((_rd32(_REG_BASE_+(0x00cb<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PBG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00cb<<2)))&BitMask_12)
#define PBG_GAINw(val)			_wr32(_REG_BASE_+(0x00cb<<2),(UINT)((_rd32(_REG_BASE_+(0x00cb<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PBB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00cd<<2)))&BitMask_11)
#define PBB_GAINw(val)			_wr32(_REG_BASE_+(0x00cd<<2),(UINT)((_rd32(_REG_BASE_+(0x00cd<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define PYR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ce<<2)))&BitMask_08)
#define PYR_GAINw(val)			_wr32(_REG_BASE_+(0x00ce<<2),(UINT)((_rd32(_REG_BASE_+(0x00ce<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PYG_GAINr				(ValSft_R08(_rd32(_REG_BASE_+(0x00ce<<2)))&BitMask_08)
#define PYG_GAINw(val)			_wr32(_REG_BASE_+(0x00ce<<2),(UINT)((_rd32(_REG_BASE_+(0x00ce<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PYB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ce<<2)))&BitMask_08)
#define PYB_GAINw(val)			_wr32(_REG_BASE_+(0x00ce<<2),(UINT)((_rd32(_REG_BASE_+(0x00ce<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ODM_M1r					(ValSft_R16(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_10)
#define ODM_M1w(val)			_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M2r					(ValSft_R00(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_10)
#define ODM_M2w(val)			_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M3r					(ValSft_R16(_rd32(_REG_BASE_+(0x00d1<<2)))&BitMask_10)
#define ODM_M3w(val)			_wr32(_REG_BASE_+(0x00d1<<2),(UINT)((_rd32(_REG_BASE_+(0x00d1<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M4r					(ValSft_R00(_rd32(_REG_BASE_+(0x00d1<<2)))&BitMask_10)
#define ODM_M4w(val)			_wr32(_REG_BASE_+(0x00d1<<2),(UINT)((_rd32(_REG_BASE_+(0x00d1<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M5r					(ValSft_R16(_rd32(_REG_BASE_+(0x00d2<<2)))&BitMask_10)
#define ODM_M5w(val)			_wr32(_REG_BASE_+(0x00d2<<2),(UINT)((_rd32(_REG_BASE_+(0x00d2<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M6r					(ValSft_R00(_rd32(_REG_BASE_+(0x00d2<<2)))&BitMask_10)
#define ODM_M6w(val)			_wr32(_REG_BASE_+(0x00d2<<2),(UINT)((_rd32(_REG_BASE_+(0x00d2<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M7r					(ValSft_R16(_rd32(_REG_BASE_+(0x00d3<<2)))&BitMask_10)
#define ODM_M7w(val)			_wr32(_REG_BASE_+(0x00d3<<2),(UINT)((_rd32(_REG_BASE_+(0x00d3<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M8r					(ValSft_R00(_rd32(_REG_BASE_+(0x00d3<<2)))&BitMask_10)
#define ODM_M8w(val)			_wr32(_REG_BASE_+(0x00d3<<2),(UINT)((_rd32(_REG_BASE_+(0x00d3<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M9r					(ValSft_R00(_rd32(_REG_BASE_+(0x00d4<<2)))&BitMask_10)
#define ODM_M9w(val)			_wr32(_REG_BASE_+(0x00d4<<2),(UINT)((_rd32(_REG_BASE_+(0x00d4<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define IRIS_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define IRIS_ONw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IRIS_INVr				(ValSft_R30(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define IRIS_INVw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IRIS_MODr				(ValSft_R28(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_02)
#define IRIS_MODw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define IRIS_SWr				(ValSft_R27(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define IRIS_SWw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IRIS_WSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_03)
#define IRIS_WSELw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IRIS_FBKr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_08)
#define IRIS_FBKw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IRIS_TLVr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_10)
#define IRIS_TLVw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define IRIS_TGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d6<<2)))&BitMask_10)
#define IRIS_TGAw(val)			_wr32(_REG_BASE_+(0x00d6<<2),(UINT)((_rd32(_REG_BASE_+(0x00d6<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define IRIS_OFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d6<<2)))&BitMask_16)
#define IRIS_OFSw(val)			_wr32(_REG_BASE_+(0x00d6<<2),(UINT)((_rd32(_REG_BASE_+(0x00d6<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define IRIS_CLPLr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d7<<2)))&BitMask_16)
#define IRIS_CLPLw(val)			_wr32(_REG_BASE_+(0x00d7<<2),(UINT)((_rd32(_REG_BASE_+(0x00d7<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define IRIS_CLPHr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d7<<2)))&BitMask_16)
#define IRIS_CLPHw(val)			_wr32(_REG_BASE_+(0x00d7<<2),(UINT)((_rd32(_REG_BASE_+(0x00d7<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define IRIS_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d8<<2)))&BitMask_16)
#define IRIS_VSPw(val)			_wr32(_REG_BASE_+(0x00d8<<2),(UINT)((_rd32(_REG_BASE_+(0x00d8<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define IRIS_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d8<<2)))&BitMask_16)
#define IRIS_VEPw(val)			_wr32(_REG_BASE_+(0x00d8<<2),(UINT)((_rd32(_REG_BASE_+(0x00d8<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define IRIS_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d9<<2)))&BitMask_16)
#define IRIS_HSPw(val)			_wr32(_REG_BASE_+(0x00d9<<2),(UINT)((_rd32(_REG_BASE_+(0x00d9<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define IRIS_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d9<<2)))&BitMask_16)
#define IRIS_HEPw(val)			_wr32(_REG_BASE_+(0x00d9<<2),(UINT)((_rd32(_REG_BASE_+(0x00d9<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define INTP_DIR_THr			(ValSft_R16(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_08)
#define INTP_DIR_THw(val)		_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define HSUB_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_02)
#define HSUB_SELw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define VSUB_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_02)
#define VSUB_SELw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define HVSUB_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_02)
#define HVSUB_SELw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define DSUB_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_02)
#define DSUB_SELw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define POST_YBGAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x00db<<2)))&BitMask_08)
#define POST_YBGAINw(val)		_wr32(_REG_BASE_+(0x00db<<2),(UINT)((_rd32(_REG_BASE_+(0x00db<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define POST_YGGAINr			(ValSft_R08(_rd32(_REG_BASE_+(0x00db<<2)))&BitMask_08)
#define POST_YGGAINw(val)		_wr32(_REG_BASE_+(0x00db<<2),(UINT)((_rd32(_REG_BASE_+(0x00db<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define POST_YRGAINr			(ValSft_R00(_rd32(_REG_BASE_+(0x00db<<2)))&BitMask_08)
#define POST_YRGAINw(val)		_wr32(_REG_BASE_+(0x00db<<2),(UINT)((_rd32(_REG_BASE_+(0x00db<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define POST_CRGAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x00dc<<2)))&BitMask_08)
#define POST_CRGAINw(val)		_wr32(_REG_BASE_+(0x00dc<<2),(UINT)((_rd32(_REG_BASE_+(0x00dc<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define POST_CGGAINr			(ValSft_R08(_rd32(_REG_BASE_+(0x00dc<<2)))&BitMask_08)
#define POST_CGGAINw(val)		_wr32(_REG_BASE_+(0x00dc<<2),(UINT)((_rd32(_REG_BASE_+(0x00dc<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define POST_CBGAINr			(ValSft_R00(_rd32(_REG_BASE_+(0x00dc<<2)))&BitMask_08)
#define POST_CBGAINw(val)		_wr32(_REG_BASE_+(0x00dc<<2),(UINT)((_rd32(_REG_BASE_+(0x00dc<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define WPRG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e0<<2)))&BitMask_13)
#define WPRG_GAINw(val)			_wr32(_REG_BASE_+(0x00e0<<2),(UINT)((_rd32(_REG_BASE_+(0x00e0<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPRR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e0<<2)))&BitMask_13)
#define WPRR_GAINw(val)			_wr32(_REG_BASE_+(0x00e0<<2),(UINT)((_rd32(_REG_BASE_+(0x00e0<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPGR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e1<<2)))&BitMask_13)
#define WPGR_GAINw(val)			_wr32(_REG_BASE_+(0x00e1<<2),(UINT)((_rd32(_REG_BASE_+(0x00e1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPRB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e1<<2)))&BitMask_13)
#define WPRB_GAINw(val)			_wr32(_REG_BASE_+(0x00e1<<2),(UINT)((_rd32(_REG_BASE_+(0x00e1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPGB_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e2<<2)))&BitMask_13)
#define WPGB_GAINw(val)			_wr32(_REG_BASE_+(0x00e2<<2),(UINT)((_rd32(_REG_BASE_+(0x00e2<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPGG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e2<<2)))&BitMask_13)
#define WPGG_GAINw(val)			_wr32(_REG_BASE_+(0x00e2<<2),(UINT)((_rd32(_REG_BASE_+(0x00e2<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPBG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e3<<2)))&BitMask_13)
#define WPBG_GAINw(val)			_wr32(_REG_BASE_+(0x00e3<<2),(UINT)((_rd32(_REG_BASE_+(0x00e3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPBR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e3<<2)))&BitMask_13)
#define WPBR_GAINw(val)			_wr32(_REG_BASE_+(0x00e3<<2),(UINT)((_rd32(_REG_BASE_+(0x00e3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPBB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e4<<2)))&BitMask_13)
#define WPBB_GAINw(val)			_wr32(_REG_BASE_+(0x00e4<<2),(UINT)((_rd32(_REG_BASE_+(0x00e4<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RMYGr					(ValSft_R16(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_10)
#define RMYGw(val)				_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define RMYRr					(ValSft_R00(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_10)
#define RMYRw(val)				_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BMYRr					(ValSft_R16(_rd32(_REG_BASE_+(0x00e6<<2)))&BitMask_10)
#define BMYRw(val)				_wr32(_REG_BASE_+(0x00e6<<2),(UINT)((_rd32(_REG_BASE_+(0x00e6<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define RMYBr					(ValSft_R00(_rd32(_REG_BASE_+(0x00e6<<2)))&BitMask_10)
#define RMYBw(val)				_wr32(_REG_BASE_+(0x00e6<<2),(UINT)((_rd32(_REG_BASE_+(0x00e6<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BMYBr					(ValSft_R16(_rd32(_REG_BASE_+(0x00e7<<2)))&BitMask_10)
#define BMYBw(val)				_wr32(_REG_BASE_+(0x00e7<<2),(UINT)((_rd32(_REG_BASE_+(0x00e7<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define BMYGr					(ValSft_R00(_rd32(_REG_BASE_+(0x00e7<<2)))&BitMask_10)
#define BMYGw(val)				_wr32(_REG_BASE_+(0x00e7<<2),(UINT)((_rd32(_REG_BASE_+(0x00e7<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BYGAINNr				(ValSft_R24(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_08)
#define BYGAINNw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define BYGAINPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_08)
#define BYGAINPw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define RYGAINNr				(ValSft_R08(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_08)
#define RYGAINNw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define RYGAINPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_08)
#define RYGAINPw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define BYHUENr					(ValSft_R24(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_08)
#define BYHUENw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define BYHUEPr					(ValSft_R16(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_08)
#define BYHUEPw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define RYHUENr					(ValSft_R08(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_08)
#define RYHUENw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define RYHUEPr					(ValSft_R00(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_08)
#define RYHUEPw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PR_OFSr					(ValSft_R16(_rd32(_REG_BASE_+(0x00ea<<2)))&BitMask_11)
#define PR_OFSw(val)			_wr32(_REG_BASE_+(0x00ea<<2),(UINT)((_rd32(_REG_BASE_+(0x00ea<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PG_OFSr					(ValSft_R00(_rd32(_REG_BASE_+(0x00ea<<2)))&BitMask_11)
#define PG_OFSw(val)			_wr32(_REG_BASE_+(0x00ea<<2),(UINT)((_rd32(_REG_BASE_+(0x00ea<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define PB_OFSr					(ValSft_R00(_rd32(_REG_BASE_+(0x00eb<<2)))&BitMask_11)
#define PB_OFSw(val)			_wr32(_REG_BASE_+(0x00eb<<2),(UINT)((_rd32(_REG_BASE_+(0x00eb<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define NEGA_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_01)
#define NEGA_ONw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BW_ONr					(ValSft_R30(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_01)
#define BW_ONw(val)				_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

//	#define MIRR_ONr				(ValSft_R29(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_00)
//	#define MIRR_ONw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L29(BitMask_00)))|ValSft_L29((val)&BitMask_00)))
//	
//	#define MIRR_RINVr				(ValSft_R28(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_00)
//	#define MIRR_RINVw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L28(BitMask_00)))|ValSft_L28((val)&BitMask_00)))
//	
//	#define MIRR_YHWr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_00)
//	#define MIRR_YHWw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L16(BitMask_00)))|ValSft_L16((val)&BitMask_00)))
//	
//	#define MIRR_CHWr				(ValSft_R04(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_00)
//	#define MIRR_CHWw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L04(BitMask_00)))|ValSft_L04((val)&BitMask_00)))

#define RBFLG_INV2r				(ValSft_R01(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_01)
#define RBFLG_INV2w(val)		_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define RBFLG_INVr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ec<<2)))&BitMask_01)
#define RBFLG_INVw(val)			_wr32(_REG_BASE_+(0x00ec<<2),(UINT)((_rd32(_REG_BASE_+(0x00ec<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define HLMASK_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define HLMASK_ONw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CSUP_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define CSUP_ONw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define CSUP_GAr				(ValSft_R24(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_06)
#define CSUP_GAw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define CSUP_THr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_08)
#define CSUP_THw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define CES_DLYr				(ValSft_R08(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_04)
#define CES_DLYw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define CVLPF_SELr				(ValSft_R07(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define CVLPF_SELw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define CVLPF_ONr				(ValSft_R06(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define CVLPF_ONw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define CHLPF_SELr				(ValSft_R05(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define CHLPF_SELw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CHLPF_ONr				(ValSft_R04(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define CHLPF_ONw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define HSUP_GAr				(ValSft_R01(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_02)
#define HSUP_GAw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L01(BitMask_02)))|ValSft_L01((val)&BitMask_02)))

#define HSUP_ONr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ed<<2)))&BitMask_01)
#define HSUP_ONw(val)			_wr32(_REG_BASE_+(0x00ed<<2),(UINT)((_rd32(_REG_BASE_+(0x00ed<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define HLMASK_BLVr				(ValSft_R24(_rd32(_REG_BASE_+(0x00ee<<2)))&BitMask_08)
#define HLMASK_BLVw(val)		_wr32(_REG_BASE_+(0x00ee<<2),(UINT)((_rd32(_REG_BASE_+(0x00ee<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define HLMASK_RLVr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ee<<2)))&BitMask_08)
#define HLMASK_RLVw(val)		_wr32(_REG_BASE_+(0x00ee<<2),(UINT)((_rd32(_REG_BASE_+(0x00ee<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define HLMASK_YLVr				(ValSft_R08(_rd32(_REG_BASE_+(0x00ee<<2)))&BitMask_08)
#define HLMASK_YLVw(val)		_wr32(_REG_BASE_+(0x00ee<<2),(UINT)((_rd32(_REG_BASE_+(0x00ee<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define HLMASK_THr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ee<<2)))&BitMask_08)
#define HLMASK_THw(val)			_wr32(_REG_BASE_+(0x00ee<<2),(UINT)((_rd32(_REG_BASE_+(0x00ee<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LSUP_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_01)
#define LSUP_ONw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LSUP_GAr				(ValSft_R24(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_06)
#define LSUP_GAw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define LSUP_THr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_08)
#define LSUP_THw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define CES_ONr					(ValSft_R15(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_01)
#define CES_ONw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define CES_GAINr				(ValSft_R08(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_06)
#define CES_GAINw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define CES_LCLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ef<<2)))&BitMask_08)
#define CES_LCLIPw(val)			_wr32(_REG_BASE_+(0x00ef<<2),(UINT)((_rd32(_REG_BASE_+(0x00ef<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DCBAR_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define DCBAR_ONw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DCBAR_STr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_12)
#define DCBAR_STw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DCBAR_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_10)
#define DCBAR_HWw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ACE_BPr					(ValSft_R31(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_01)
#define ACE_BPw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ACE_ONr					(ValSft_R30(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_01)
#define ACE_ONw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ACE_TESTr				(ValSft_R29(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_01)
#define ACE_TESTw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define ACE_OSDr				(ValSft_R28(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_01)
#define ACE_OSDw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define ACE_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_12)
#define ACE_HSPw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define ACE_DTHr				(ValSft_R12(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_04)
#define ACE_DTHw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define ACE_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_12)
#define ACE_VSPw(val)			_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define ACE_LPFr				(ValSft_R31(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_01)
#define ACE_LPFw(val)			_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ACE_TH1r				(ValSft_R16(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_08)
#define ACE_TH1w(val)			_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ACE_TH2r				(ValSft_R08(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_08)
#define ACE_TH2w(val)			_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ACE_HBSr				(ValSft_R06(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_02)
#define ACE_HBSw(val)			_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define ACE_VBSr				(ValSft_R04(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_02)
#define ACE_VBSw(val)			_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define ACE_HBr					(ValSft_R20(_rd32(_REG_BASE_+(0x00f3<<2)))&BitMask_04)
#define ACE_HBw(val)			_wr32(_REG_BASE_+(0x00f3<<2),(UINT)((_rd32(_REG_BASE_+(0x00f3<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define ACE_VBr					(ValSft_R16(_rd32(_REG_BASE_+(0x00f3<<2)))&BitMask_04)
#define ACE_VBw(val)			_wr32(_REG_BASE_+(0x00f3<<2),(UINT)((_rd32(_REG_BASE_+(0x00f3<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define ACE_IIRr				(ValSft_R08(_rd32(_REG_BASE_+(0x00f3<<2)))&BitMask_06)
#define ACE_IIRw(val)			_wr32(_REG_BASE_+(0x00f3<<2),(UINT)((_rd32(_REG_BASE_+(0x00f3<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define ACE_GMGNr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f3<<2)))&BitMask_08)
#define ACE_GMGNw(val)			_wr32(_REG_BASE_+(0x00f3<<2),(UINT)((_rd32(_REG_BASE_+(0x00f3<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ACE_GM00r				(ValSft_R24(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_08)
#define ACE_GM00w(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ACE_GM01r				(ValSft_R16(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_08)
#define ACE_GM01w(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ACE_GM02r				(ValSft_R08(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_08)
#define ACE_GM02w(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ACE_GM03r				(ValSft_R00(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_08)
#define ACE_GM03w(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ACE_GM04r				(ValSft_R24(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_08)
#define ACE_GM04w(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ACE_GM05r				(ValSft_R16(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_08)
#define ACE_GM05w(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ACE_GM06r				(ValSft_R08(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_08)
#define ACE_GM06w(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ACE_GM07r				(ValSft_R00(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_08)
#define ACE_GM07w(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ACE_GM08r				(ValSft_R24(_rd32(_REG_BASE_+(0x00f6<<2)))&BitMask_08)
#define ACE_GM08w(val)			_wr32(_REG_BASE_+(0x00f6<<2),(UINT)((_rd32(_REG_BASE_+(0x00f6<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ACE_GM09r				(ValSft_R16(_rd32(_REG_BASE_+(0x00f6<<2)))&BitMask_08)
#define ACE_GM09w(val)			_wr32(_REG_BASE_+(0x00f6<<2),(UINT)((_rd32(_REG_BASE_+(0x00f6<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ACE_GM10r				(ValSft_R08(_rd32(_REG_BASE_+(0x00f6<<2)))&BitMask_08)
#define ACE_GM10w(val)			_wr32(_REG_BASE_+(0x00f6<<2),(UINT)((_rd32(_REG_BASE_+(0x00f6<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ACE_GM11r				(ValSft_R00(_rd32(_REG_BASE_+(0x00f6<<2)))&BitMask_08)
#define ACE_GM11w(val)			_wr32(_REG_BASE_+(0x00f6<<2),(UINT)((_rd32(_REG_BASE_+(0x00f6<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ACE_GM12r				(ValSft_R24(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_08)
#define ACE_GM12w(val)			_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ACE_GM13r				(ValSft_R16(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_08)
#define ACE_GM13w(val)			_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ACE_GM14r				(ValSft_R08(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_08)
#define ACE_GM14w(val)			_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ACE_GM15r				(ValSft_R00(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_08)
#define ACE_GM15w(val)			_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define HEQ_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_01)
#define HEQ_ONw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HEQ_OSDr				(ValSft_R30(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_01)
#define HEQ_OSDw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HEQ_WGr					(ValSft_R16(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_09)
#define HEQ_WGw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define HEQ_THr					(ValSft_R00(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_15)
#define HEQ_THw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L00(BitMask_15)))|ValSft_L00((val)&BitMask_15)))

#define HEQ_HWr					(ValSft_R28(_rd32(_REG_BASE_+(0x00f9<<2)))&BitMask_04)
#define HEQ_HWw(val)			_wr32(_REG_BASE_+(0x00f9<<2),(UINT)((_rd32(_REG_BASE_+(0x00f9<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define HEQ_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f9<<2)))&BitMask_12)
#define HEQ_HSPw(val)			_wr32(_REG_BASE_+(0x00f9<<2),(UINT)((_rd32(_REG_BASE_+(0x00f9<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HEQ_VWr					(ValSft_R12(_rd32(_REG_BASE_+(0x00f9<<2)))&BitMask_04)
#define HEQ_VWw(val)			_wr32(_REG_BASE_+(0x00f9<<2),(UINT)((_rd32(_REG_BASE_+(0x00f9<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define HEQ_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f9<<2)))&BitMask_12)
#define HEQ_VSPw(val)			_wr32(_REG_BASE_+(0x00f9<<2),(UINT)((_rd32(_REG_BASE_+(0x00f9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HEQ_IIRKr				(ValSft_R00(_rd32(_REG_BASE_+(0x00fa<<2)))&BitMask_08)
#define HEQ_IIRKw(val)			_wr32(_REG_BASE_+(0x00fa<<2),(UINT)((_rd32(_REG_BASE_+(0x00fa<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_01)
#define APT_ONw(val)			_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_GAIN_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_01)
#define APT_GAIN_ONw(val)		_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_TESTr				(ValSft_R28(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_02)
#define APT_TESTw(val)			_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define APT_TH3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_10)
#define APT_TH3w(val)			_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_TH1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0101<<2)))&BitMask_16)
#define APT_TH1w(val)			_wr32(_REG_BASE_+(0x0101<<2),(UINT)((_rd32(_REG_BASE_+(0x0101<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define APT_TH2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0101<<2)))&BitMask_16)
#define APT_TH2w(val)			_wr32(_REG_BASE_+(0x0101<<2),(UINT)((_rd32(_REG_BASE_+(0x0101<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define APT_GAIN2_NEGr			(ValSft_R24(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define APT_GAIN2_NEGw(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_GAIN1_NEGr			(ValSft_R16(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define APT_GAIN1_NEGw(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_GAIN2_POSr			(ValSft_R08(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define APT_GAIN2_POSw(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_GAIN1_POSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define APT_GAIN1_POSw(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_SLIC3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0103<<2)))&BitMask_14)
#define APT_SLIC3w(val)			_wr32(_REG_BASE_+(0x0103<<2),(UINT)((_rd32(_REG_BASE_+(0x0103<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define APT_CLIP3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0103<<2)))&BitMask_10)
#define APT_CLIP3w(val)			_wr32(_REG_BASE_+(0x0103<<2),(UINT)((_rd32(_REG_BASE_+(0x0103<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_GAIN3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_08)
#define APT_GAIN3w(val)			_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_LPFKr				(ValSft_R24(_rd32(_REG_BASE_+(0x0105<<2)))&BitMask_08)
#define APT_LPFKw(val)			_wr32(_REG_BASE_+(0x0105<<2),(UINT)((_rd32(_REG_BASE_+(0x0105<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_LPFGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x0105<<2)))&BitMask_08)
#define APT_LPFGAw(val)			_wr32(_REG_BASE_+(0x0105<<2),(UINT)((_rd32(_REG_BASE_+(0x0105<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_LPFTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x0105<<2)))&BitMask_10)
#define APT_LPFTHw(val)			_wr32(_REG_BASE_+(0x0105<<2),(UINT)((_rd32(_REG_BASE_+(0x0105<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMA_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_01)
#define YGMA_ONw(val)			_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CGMA_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_01)
#define CGMA_ONw(val)			_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YGMY_00r				(ValSft_R16(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_10)
#define YGMY_00w(val)			_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_01r				(ValSft_R00(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_10)
#define YGMY_01w(val)			_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_02r				(ValSft_R16(_rd32(_REG_BASE_+(0x0107<<2)))&BitMask_10)
#define YGMY_02w(val)			_wr32(_REG_BASE_+(0x0107<<2),(UINT)((_rd32(_REG_BASE_+(0x0107<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_03r				(ValSft_R00(_rd32(_REG_BASE_+(0x0107<<2)))&BitMask_10)
#define YGMY_03w(val)			_wr32(_REG_BASE_+(0x0107<<2),(UINT)((_rd32(_REG_BASE_+(0x0107<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_04r				(ValSft_R16(_rd32(_REG_BASE_+(0x0108<<2)))&BitMask_10)
#define YGMY_04w(val)			_wr32(_REG_BASE_+(0x0108<<2),(UINT)((_rd32(_REG_BASE_+(0x0108<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_05r				(ValSft_R00(_rd32(_REG_BASE_+(0x0108<<2)))&BitMask_10)
#define YGMY_05w(val)			_wr32(_REG_BASE_+(0x0108<<2),(UINT)((_rd32(_REG_BASE_+(0x0108<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_06r				(ValSft_R16(_rd32(_REG_BASE_+(0x0109<<2)))&BitMask_10)
#define YGMY_06w(val)			_wr32(_REG_BASE_+(0x0109<<2),(UINT)((_rd32(_REG_BASE_+(0x0109<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_07r				(ValSft_R00(_rd32(_REG_BASE_+(0x0109<<2)))&BitMask_10)
#define YGMY_07w(val)			_wr32(_REG_BASE_+(0x0109<<2),(UINT)((_rd32(_REG_BASE_+(0x0109<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_08r				(ValSft_R16(_rd32(_REG_BASE_+(0x010a<<2)))&BitMask_10)
#define YGMY_08w(val)			_wr32(_REG_BASE_+(0x010a<<2),(UINT)((_rd32(_REG_BASE_+(0x010a<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_09r				(ValSft_R00(_rd32(_REG_BASE_+(0x010a<<2)))&BitMask_10)
#define YGMY_09w(val)			_wr32(_REG_BASE_+(0x010a<<2),(UINT)((_rd32(_REG_BASE_+(0x010a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_10r				(ValSft_R16(_rd32(_REG_BASE_+(0x010b<<2)))&BitMask_10)
#define YGMY_10w(val)			_wr32(_REG_BASE_+(0x010b<<2),(UINT)((_rd32(_REG_BASE_+(0x010b<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_11r				(ValSft_R00(_rd32(_REG_BASE_+(0x010b<<2)))&BitMask_10)
#define YGMY_11w(val)			_wr32(_REG_BASE_+(0x010b<<2),(UINT)((_rd32(_REG_BASE_+(0x010b<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_12r				(ValSft_R16(_rd32(_REG_BASE_+(0x010c<<2)))&BitMask_10)
#define YGMY_12w(val)			_wr32(_REG_BASE_+(0x010c<<2),(UINT)((_rd32(_REG_BASE_+(0x010c<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_13r				(ValSft_R00(_rd32(_REG_BASE_+(0x010c<<2)))&BitMask_10)
#define YGMY_13w(val)			_wr32(_REG_BASE_+(0x010c<<2),(UINT)((_rd32(_REG_BASE_+(0x010c<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_14r				(ValSft_R16(_rd32(_REG_BASE_+(0x010d<<2)))&BitMask_10)
#define YGMY_14w(val)			_wr32(_REG_BASE_+(0x010d<<2),(UINT)((_rd32(_REG_BASE_+(0x010d<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_15r				(ValSft_R00(_rd32(_REG_BASE_+(0x010d<<2)))&BitMask_10)
#define YGMY_15w(val)			_wr32(_REG_BASE_+(0x010d<<2),(UINT)((_rd32(_REG_BASE_+(0x010d<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_16r				(ValSft_R16(_rd32(_REG_BASE_+(0x010e<<2)))&BitMask_10)
#define YGMY_16w(val)			_wr32(_REG_BASE_+(0x010e<<2),(UINT)((_rd32(_REG_BASE_+(0x010e<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_00r				(ValSft_R16(_rd32(_REG_BASE_+(0x0110<<2)))&BitMask_10)
#define CGMY_00w(val)			_wr32(_REG_BASE_+(0x0110<<2),(UINT)((_rd32(_REG_BASE_+(0x0110<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_01r				(ValSft_R00(_rd32(_REG_BASE_+(0x0110<<2)))&BitMask_10)
#define CGMY_01w(val)			_wr32(_REG_BASE_+(0x0110<<2),(UINT)((_rd32(_REG_BASE_+(0x0110<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_02r				(ValSft_R16(_rd32(_REG_BASE_+(0x0111<<2)))&BitMask_10)
#define CGMY_02w(val)			_wr32(_REG_BASE_+(0x0111<<2),(UINT)((_rd32(_REG_BASE_+(0x0111<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_03r				(ValSft_R00(_rd32(_REG_BASE_+(0x0111<<2)))&BitMask_10)
#define CGMY_03w(val)			_wr32(_REG_BASE_+(0x0111<<2),(UINT)((_rd32(_REG_BASE_+(0x0111<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_04r				(ValSft_R16(_rd32(_REG_BASE_+(0x0112<<2)))&BitMask_10)
#define CGMY_04w(val)			_wr32(_REG_BASE_+(0x0112<<2),(UINT)((_rd32(_REG_BASE_+(0x0112<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_05r				(ValSft_R00(_rd32(_REG_BASE_+(0x0112<<2)))&BitMask_10)
#define CGMY_05w(val)			_wr32(_REG_BASE_+(0x0112<<2),(UINT)((_rd32(_REG_BASE_+(0x0112<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_06r				(ValSft_R16(_rd32(_REG_BASE_+(0x0113<<2)))&BitMask_10)
#define CGMY_06w(val)			_wr32(_REG_BASE_+(0x0113<<2),(UINT)((_rd32(_REG_BASE_+(0x0113<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_07r				(ValSft_R00(_rd32(_REG_BASE_+(0x0113<<2)))&BitMask_10)
#define CGMY_07w(val)			_wr32(_REG_BASE_+(0x0113<<2),(UINT)((_rd32(_REG_BASE_+(0x0113<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_08r				(ValSft_R16(_rd32(_REG_BASE_+(0x0114<<2)))&BitMask_10)
#define CGMY_08w(val)			_wr32(_REG_BASE_+(0x0114<<2),(UINT)((_rd32(_REG_BASE_+(0x0114<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_09r				(ValSft_R00(_rd32(_REG_BASE_+(0x0114<<2)))&BitMask_10)
#define CGMY_09w(val)			_wr32(_REG_BASE_+(0x0114<<2),(UINT)((_rd32(_REG_BASE_+(0x0114<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_10r				(ValSft_R16(_rd32(_REG_BASE_+(0x0115<<2)))&BitMask_10)
#define CGMY_10w(val)			_wr32(_REG_BASE_+(0x0115<<2),(UINT)((_rd32(_REG_BASE_+(0x0115<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_11r				(ValSft_R00(_rd32(_REG_BASE_+(0x0115<<2)))&BitMask_10)
#define CGMY_11w(val)			_wr32(_REG_BASE_+(0x0115<<2),(UINT)((_rd32(_REG_BASE_+(0x0115<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_12r				(ValSft_R16(_rd32(_REG_BASE_+(0x0116<<2)))&BitMask_10)
#define CGMY_12w(val)			_wr32(_REG_BASE_+(0x0116<<2),(UINT)((_rd32(_REG_BASE_+(0x0116<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_13r				(ValSft_R00(_rd32(_REG_BASE_+(0x0116<<2)))&BitMask_10)
#define CGMY_13w(val)			_wr32(_REG_BASE_+(0x0116<<2),(UINT)((_rd32(_REG_BASE_+(0x0116<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_14r				(ValSft_R16(_rd32(_REG_BASE_+(0x0117<<2)))&BitMask_10)
#define CGMY_14w(val)			_wr32(_REG_BASE_+(0x0117<<2),(UINT)((_rd32(_REG_BASE_+(0x0117<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_15r				(ValSft_R00(_rd32(_REG_BASE_+(0x0117<<2)))&BitMask_10)
#define CGMY_15w(val)			_wr32(_REG_BASE_+(0x0117<<2),(UINT)((_rd32(_REG_BASE_+(0x0117<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_16r				(ValSft_R16(_rd32(_REG_BASE_+(0x0118<<2)))&BitMask_10)
#define CGMY_16w(val)			_wr32(_REG_BASE_+(0x0118<<2),(UINT)((_rd32(_REG_BASE_+(0x0118<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define FONT_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_01)
#define FONT_ONw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BOAD_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_01)
#define BOAD_ONw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BOAD_MODEr				(ValSft_R29(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_01)
#define BOAD_MODEw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define FONT_MUTEr				(ValSft_R28(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_01)
#define FONT_MUTEw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define FONT_OFYr				(ValSft_R16(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_12)
#define FONT_OFYw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define FONT_OFXr				(ValSft_R00(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_12)
#define FONT_OFXw(val)			_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define FONT_LV0r				(ValSft_R00(_rd32(_REG_BASE_+(0x011a<<2)))&BitMask_24)
#define FONT_LV0w(val)			_wr32(_REG_BASE_+(0x011a<<2),(UINT)((_rd32(_REG_BASE_+(0x011a<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV1r				(ValSft_R00(_rd32(_REG_BASE_+(0x011b<<2)))&BitMask_24)
#define FONT_LV1w(val)			_wr32(_REG_BASE_+(0x011b<<2),(UINT)((_rd32(_REG_BASE_+(0x011b<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV2r				(ValSft_R00(_rd32(_REG_BASE_+(0x011c<<2)))&BitMask_24)
#define FONT_LV2w(val)			_wr32(_REG_BASE_+(0x011c<<2),(UINT)((_rd32(_REG_BASE_+(0x011c<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV3r				(ValSft_R00(_rd32(_REG_BASE_+(0x011d<<2)))&BitMask_24)
#define FONT_LV3w(val)			_wr32(_REG_BASE_+(0x011d<<2),(UINT)((_rd32(_REG_BASE_+(0x011d<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOAD_LV0r				(ValSft_R00(_rd32(_REG_BASE_+(0x011e<<2)))&BitMask_24)
#define BOAD_LV0w(val)			_wr32(_REG_BASE_+(0x011e<<2),(UINT)((_rd32(_REG_BASE_+(0x011e<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_VBLKr				(ValSft_R16(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_08)
#define FONT_VBLKw(val)			_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define FONT_HBLKr				(ValSft_R08(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_08)
#define FONT_HBLKw(val)			_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define FONT_VSIZr				(ValSft_R04(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_04)
#define FONT_VSIZw(val)			_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define FONT_HSIZr				(ValSft_R00(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_04)
#define FONT_HSIZw(val)			_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define MASK_CHADr				(ValSft_R21(_rd32(_REG_BASE_+(0x0120<<2)))&BitMask_09)
#define MASK_CHADw(val)			_wr32(_REG_BASE_+(0x0120<<2),(UINT)((_rd32(_REG_BASE_+(0x0120<<2))&(~ValSft_L21(BitMask_09)))|ValSft_L21((val)&BitMask_09)))

#define HALF_CHADr				(ValSft_R12(_rd32(_REG_BASE_+(0x0120<<2)))&BitMask_09)
#define HALF_CHADw(val)			_wr32(_REG_BASE_+(0x0120<<2),(UINT)((_rd32(_REG_BASE_+(0x0120<<2))&(~ValSft_L12(BitMask_09)))|ValSft_L12((val)&BitMask_09)))

#define FONT_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0120<<2)))&BitMask_12)
#define FONT_HWw(val)			_wr32(_REG_BASE_+(0x0120<<2),(UINT)((_rd32(_REG_BASE_+(0x0120<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define OSD_MUTEr				(ValSft_R31(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_01)
#define OSD_MUTEw(val)			_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define OSD_ISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_02)
#define OSD_ISELw(val)			_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BOX_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_12)
#define BOX_HSPw(val)			_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define OSD_IVSELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_02)
#define OSD_IVSELw(val)			_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define BOX_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_12)
#define BOX_VSPw(val)			_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BOSD_ONr				(ValSft_R00(_rd32(_REG_BASE_+(0x0122<<2)))&BitMask_32)
#define BOSD_ONw(val)			_wr32(_REG_BASE_+(0x0122<<2),(UINT)((_rd32(_REG_BASE_+(0x0122<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define BTONE31r				(ValSft_R30(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE31w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define BTONE30r				(ValSft_R28(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE30w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BTONE29r				(ValSft_R26(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE29w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define BTONE28r				(ValSft_R24(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE28w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BTONE27r				(ValSft_R22(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE27w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define BTONE26r				(ValSft_R20(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE26w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BTONE25r				(ValSft_R18(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE25w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define BTONE24r				(ValSft_R16(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE24w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define BTONE23r				(ValSft_R14(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE23w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define BTONE22r				(ValSft_R12(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE22w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define BTONE21r				(ValSft_R10(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE21w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define BTONE20r				(ValSft_R08(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE20w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define BTONE19r				(ValSft_R06(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE19w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define BTONE18r				(ValSft_R04(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE18w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define BTONE17r				(ValSft_R02(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE17w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L02(BitMask_02)))|ValSft_L02((val)&BitMask_02)))

#define BTONE16r				(ValSft_R00(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_02)
#define BTONE16w(val)			_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define BTONE15r				(ValSft_R30(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE15w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define BTONE14r				(ValSft_R28(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE14w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BTONE13r				(ValSft_R26(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE13w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define BTONE12r				(ValSft_R24(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE12w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BTONE11r				(ValSft_R22(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE11w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define BTONE10r				(ValSft_R20(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE10w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BTONE9r					(ValSft_R18(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE9w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define BTONE8r					(ValSft_R16(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE8w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define BTONE7r					(ValSft_R14(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE7w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define BTONE6r					(ValSft_R12(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE6w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define BTONE5r					(ValSft_R10(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE5w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define BTONE4r					(ValSft_R08(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE4w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define BTONE3r					(ValSft_R06(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE3w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define BTONE2r					(ValSft_R04(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE2w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define BTONE1r					(ValSft_R02(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE1w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L02(BitMask_02)))|ValSft_L02((val)&BitMask_02)))

#define BTONE0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_02)
#define BTONE0w(val)			_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define BFL_ON31r				(ValSft_R31(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON31w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BFL_ON30r				(ValSft_R30(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON30w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BFL_ON29r				(ValSft_R29(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON29w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BFL_ON28r				(ValSft_R28(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON28w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BFL_ON27r				(ValSft_R27(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON27w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BFL_ON26r				(ValSft_R26(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON26w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BFL_ON25r				(ValSft_R25(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON25w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BFL_ON24r				(ValSft_R24(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON24w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BFL_ON23r				(ValSft_R23(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON23w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define BFL_ON22r				(ValSft_R22(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON22w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define BFL_ON21r				(ValSft_R21(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON21w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define BFL_ON20r				(ValSft_R20(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON20w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define BFL_ON19r				(ValSft_R19(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON19w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define BFL_ON18r				(ValSft_R18(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON18w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define BFL_ON17r				(ValSft_R17(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON17w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BFL_ON16r				(ValSft_R16(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON16w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define BFL_ON15r				(ValSft_R15(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON15w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define BFL_ON14r				(ValSft_R14(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON14w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define BFL_ON13r				(ValSft_R13(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON13w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define BFL_ON12r				(ValSft_R12(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON12w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define BFL_ON11r				(ValSft_R11(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON11w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define BFL_ON10r				(ValSft_R10(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON10w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define BFL_ON9r				(ValSft_R09(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON9w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define BFL_ON8r				(ValSft_R08(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON8w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define BFL_ON7r				(ValSft_R07(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON7w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define BFL_ON6r				(ValSft_R06(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON6w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define BFL_ON5r				(ValSft_R05(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON5w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define BFL_ON4r				(ValSft_R04(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON4w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BFL_ON3r				(ValSft_R03(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON3w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define BFL_ON2r				(ValSft_R02(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON2w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define BFL_ON1r				(ValSft_R01(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON1w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define BFL_ON0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_01)
#define BFL_ON0w(val)			_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define B0_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0126<<2)))&BitMask_12)
#define B0_VSPw(val)			_wr32(_REG_BASE_+(0x0126<<2),(UINT)((_rd32(_REG_BASE_+(0x0126<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B0_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0126<<2)))&BitMask_12)
#define B0_VEDw(val)			_wr32(_REG_BASE_+(0x0126<<2),(UINT)((_rd32(_REG_BASE_+(0x0126<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B0_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0127<<2)))&BitMask_12)
#define B0_HSPw(val)			_wr32(_REG_BASE_+(0x0127<<2),(UINT)((_rd32(_REG_BASE_+(0x0127<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B0_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0127<<2)))&BitMask_12)
#define B0_HEDw(val)			_wr32(_REG_BASE_+(0x0127<<2),(UINT)((_rd32(_REG_BASE_+(0x0127<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B1_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0128<<2)))&BitMask_12)
#define B1_VSPw(val)			_wr32(_REG_BASE_+(0x0128<<2),(UINT)((_rd32(_REG_BASE_+(0x0128<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B1_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0128<<2)))&BitMask_12)
#define B1_VEDw(val)			_wr32(_REG_BASE_+(0x0128<<2),(UINT)((_rd32(_REG_BASE_+(0x0128<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B1_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0129<<2)))&BitMask_12)
#define B1_HSPw(val)			_wr32(_REG_BASE_+(0x0129<<2),(UINT)((_rd32(_REG_BASE_+(0x0129<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B1_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0129<<2)))&BitMask_12)
#define B1_HEDw(val)			_wr32(_REG_BASE_+(0x0129<<2),(UINT)((_rd32(_REG_BASE_+(0x0129<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B2_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012a<<2)))&BitMask_12)
#define B2_VSPw(val)			_wr32(_REG_BASE_+(0x012a<<2),(UINT)((_rd32(_REG_BASE_+(0x012a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B2_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012a<<2)))&BitMask_12)
#define B2_VEDw(val)			_wr32(_REG_BASE_+(0x012a<<2),(UINT)((_rd32(_REG_BASE_+(0x012a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B2_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012b<<2)))&BitMask_12)
#define B2_HSPw(val)			_wr32(_REG_BASE_+(0x012b<<2),(UINT)((_rd32(_REG_BASE_+(0x012b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B2_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012b<<2)))&BitMask_12)
#define B2_HEDw(val)			_wr32(_REG_BASE_+(0x012b<<2),(UINT)((_rd32(_REG_BASE_+(0x012b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B3_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012c<<2)))&BitMask_12)
#define B3_VSPw(val)			_wr32(_REG_BASE_+(0x012c<<2),(UINT)((_rd32(_REG_BASE_+(0x012c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B3_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012c<<2)))&BitMask_12)
#define B3_VEDw(val)			_wr32(_REG_BASE_+(0x012c<<2),(UINT)((_rd32(_REG_BASE_+(0x012c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B3_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012d<<2)))&BitMask_12)
#define B3_HSPw(val)			_wr32(_REG_BASE_+(0x012d<<2),(UINT)((_rd32(_REG_BASE_+(0x012d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B3_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012d<<2)))&BitMask_12)
#define B3_HEDw(val)			_wr32(_REG_BASE_+(0x012d<<2),(UINT)((_rd32(_REG_BASE_+(0x012d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B4_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012e<<2)))&BitMask_12)
#define B4_VSPw(val)			_wr32(_REG_BASE_+(0x012e<<2),(UINT)((_rd32(_REG_BASE_+(0x012e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B4_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012e<<2)))&BitMask_12)
#define B4_VEDw(val)			_wr32(_REG_BASE_+(0x012e<<2),(UINT)((_rd32(_REG_BASE_+(0x012e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B4_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x012f<<2)))&BitMask_12)
#define B4_HSPw(val)			_wr32(_REG_BASE_+(0x012f<<2),(UINT)((_rd32(_REG_BASE_+(0x012f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B4_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x012f<<2)))&BitMask_12)
#define B4_HEDw(val)			_wr32(_REG_BASE_+(0x012f<<2),(UINT)((_rd32(_REG_BASE_+(0x012f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B5_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0130<<2)))&BitMask_12)
#define B5_VSPw(val)			_wr32(_REG_BASE_+(0x0130<<2),(UINT)((_rd32(_REG_BASE_+(0x0130<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B5_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0130<<2)))&BitMask_12)
#define B5_VEDw(val)			_wr32(_REG_BASE_+(0x0130<<2),(UINT)((_rd32(_REG_BASE_+(0x0130<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B5_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0131<<2)))&BitMask_12)
#define B5_HSPw(val)			_wr32(_REG_BASE_+(0x0131<<2),(UINT)((_rd32(_REG_BASE_+(0x0131<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B5_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0131<<2)))&BitMask_12)
#define B5_HEDw(val)			_wr32(_REG_BASE_+(0x0131<<2),(UINT)((_rd32(_REG_BASE_+(0x0131<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B6_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0132<<2)))&BitMask_12)
#define B6_VSPw(val)			_wr32(_REG_BASE_+(0x0132<<2),(UINT)((_rd32(_REG_BASE_+(0x0132<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B6_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0132<<2)))&BitMask_12)
#define B6_VEDw(val)			_wr32(_REG_BASE_+(0x0132<<2),(UINT)((_rd32(_REG_BASE_+(0x0132<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B6_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0133<<2)))&BitMask_12)
#define B6_HSPw(val)			_wr32(_REG_BASE_+(0x0133<<2),(UINT)((_rd32(_REG_BASE_+(0x0133<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B6_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0133<<2)))&BitMask_12)
#define B6_HEDw(val)			_wr32(_REG_BASE_+(0x0133<<2),(UINT)((_rd32(_REG_BASE_+(0x0133<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B7_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0134<<2)))&BitMask_12)
#define B7_VSPw(val)			_wr32(_REG_BASE_+(0x0134<<2),(UINT)((_rd32(_REG_BASE_+(0x0134<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B7_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0134<<2)))&BitMask_12)
#define B7_VEDw(val)			_wr32(_REG_BASE_+(0x0134<<2),(UINT)((_rd32(_REG_BASE_+(0x0134<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B7_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0135<<2)))&BitMask_12)
#define B7_HSPw(val)			_wr32(_REG_BASE_+(0x0135<<2),(UINT)((_rd32(_REG_BASE_+(0x0135<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B7_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0135<<2)))&BitMask_12)
#define B7_HEDw(val)			_wr32(_REG_BASE_+(0x0135<<2),(UINT)((_rd32(_REG_BASE_+(0x0135<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B8_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0136<<2)))&BitMask_12)
#define B8_VSPw(val)			_wr32(_REG_BASE_+(0x0136<<2),(UINT)((_rd32(_REG_BASE_+(0x0136<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B8_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0136<<2)))&BitMask_12)
#define B8_VEDw(val)			_wr32(_REG_BASE_+(0x0136<<2),(UINT)((_rd32(_REG_BASE_+(0x0136<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B8_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0137<<2)))&BitMask_12)
#define B8_HSPw(val)			_wr32(_REG_BASE_+(0x0137<<2),(UINT)((_rd32(_REG_BASE_+(0x0137<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B8_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0137<<2)))&BitMask_12)
#define B8_HEDw(val)			_wr32(_REG_BASE_+(0x0137<<2),(UINT)((_rd32(_REG_BASE_+(0x0137<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B9_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0138<<2)))&BitMask_12)
#define B9_VSPw(val)			_wr32(_REG_BASE_+(0x0138<<2),(UINT)((_rd32(_REG_BASE_+(0x0138<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B9_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0138<<2)))&BitMask_12)
#define B9_VEDw(val)			_wr32(_REG_BASE_+(0x0138<<2),(UINT)((_rd32(_REG_BASE_+(0x0138<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B9_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x0139<<2)))&BitMask_12)
#define B9_HSPw(val)			_wr32(_REG_BASE_+(0x0139<<2),(UINT)((_rd32(_REG_BASE_+(0x0139<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B9_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x0139<<2)))&BitMask_12)
#define B9_HEDw(val)			_wr32(_REG_BASE_+(0x0139<<2),(UINT)((_rd32(_REG_BASE_+(0x0139<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B10_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013a<<2)))&BitMask_12)
#define B10_VSPw(val)			_wr32(_REG_BASE_+(0x013a<<2),(UINT)((_rd32(_REG_BASE_+(0x013a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B10_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013a<<2)))&BitMask_12)
#define B10_VEDw(val)			_wr32(_REG_BASE_+(0x013a<<2),(UINT)((_rd32(_REG_BASE_+(0x013a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B10_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013b<<2)))&BitMask_12)
#define B10_HSPw(val)			_wr32(_REG_BASE_+(0x013b<<2),(UINT)((_rd32(_REG_BASE_+(0x013b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B10_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013b<<2)))&BitMask_12)
#define B10_HEDw(val)			_wr32(_REG_BASE_+(0x013b<<2),(UINT)((_rd32(_REG_BASE_+(0x013b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B11_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013c<<2)))&BitMask_12)
#define B11_VSPw(val)			_wr32(_REG_BASE_+(0x013c<<2),(UINT)((_rd32(_REG_BASE_+(0x013c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B11_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013c<<2)))&BitMask_12)
#define B11_VEDw(val)			_wr32(_REG_BASE_+(0x013c<<2),(UINT)((_rd32(_REG_BASE_+(0x013c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B11_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013d<<2)))&BitMask_12)
#define B11_HSPw(val)			_wr32(_REG_BASE_+(0x013d<<2),(UINT)((_rd32(_REG_BASE_+(0x013d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B11_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013d<<2)))&BitMask_12)
#define B11_HEDw(val)			_wr32(_REG_BASE_+(0x013d<<2),(UINT)((_rd32(_REG_BASE_+(0x013d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B12_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013e<<2)))&BitMask_12)
#define B12_VSPw(val)			_wr32(_REG_BASE_+(0x013e<<2),(UINT)((_rd32(_REG_BASE_+(0x013e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B12_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013e<<2)))&BitMask_12)
#define B12_VEDw(val)			_wr32(_REG_BASE_+(0x013e<<2),(UINT)((_rd32(_REG_BASE_+(0x013e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B12_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x013f<<2)))&BitMask_12)
#define B12_HSPw(val)			_wr32(_REG_BASE_+(0x013f<<2),(UINT)((_rd32(_REG_BASE_+(0x013f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B12_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x013f<<2)))&BitMask_12)
#define B12_HEDw(val)			_wr32(_REG_BASE_+(0x013f<<2),(UINT)((_rd32(_REG_BASE_+(0x013f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B13_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0140<<2)))&BitMask_12)
#define B13_VSPw(val)			_wr32(_REG_BASE_+(0x0140<<2),(UINT)((_rd32(_REG_BASE_+(0x0140<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B13_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0140<<2)))&BitMask_12)
#define B13_VEDw(val)			_wr32(_REG_BASE_+(0x0140<<2),(UINT)((_rd32(_REG_BASE_+(0x0140<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B13_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0141<<2)))&BitMask_12)
#define B13_HSPw(val)			_wr32(_REG_BASE_+(0x0141<<2),(UINT)((_rd32(_REG_BASE_+(0x0141<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B13_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0141<<2)))&BitMask_12)
#define B13_HEDw(val)			_wr32(_REG_BASE_+(0x0141<<2),(UINT)((_rd32(_REG_BASE_+(0x0141<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B14_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0142<<2)))&BitMask_12)
#define B14_VSPw(val)			_wr32(_REG_BASE_+(0x0142<<2),(UINT)((_rd32(_REG_BASE_+(0x0142<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B14_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0142<<2)))&BitMask_12)
#define B14_VEDw(val)			_wr32(_REG_BASE_+(0x0142<<2),(UINT)((_rd32(_REG_BASE_+(0x0142<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B14_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0143<<2)))&BitMask_12)
#define B14_HSPw(val)			_wr32(_REG_BASE_+(0x0143<<2),(UINT)((_rd32(_REG_BASE_+(0x0143<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B14_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0143<<2)))&BitMask_12)
#define B14_HEDw(val)			_wr32(_REG_BASE_+(0x0143<<2),(UINT)((_rd32(_REG_BASE_+(0x0143<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B15_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0144<<2)))&BitMask_12)
#define B15_VSPw(val)			_wr32(_REG_BASE_+(0x0144<<2),(UINT)((_rd32(_REG_BASE_+(0x0144<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B15_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0144<<2)))&BitMask_12)
#define B15_VEDw(val)			_wr32(_REG_BASE_+(0x0144<<2),(UINT)((_rd32(_REG_BASE_+(0x0144<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B15_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0145<<2)))&BitMask_12)
#define B15_HSPw(val)			_wr32(_REG_BASE_+(0x0145<<2),(UINT)((_rd32(_REG_BASE_+(0x0145<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B15_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0145<<2)))&BitMask_12)
#define B15_HEDw(val)			_wr32(_REG_BASE_+(0x0145<<2),(UINT)((_rd32(_REG_BASE_+(0x0145<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B16_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0146<<2)))&BitMask_12)
#define B16_VSPw(val)			_wr32(_REG_BASE_+(0x0146<<2),(UINT)((_rd32(_REG_BASE_+(0x0146<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B16_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0146<<2)))&BitMask_12)
#define B16_VEDw(val)			_wr32(_REG_BASE_+(0x0146<<2),(UINT)((_rd32(_REG_BASE_+(0x0146<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B16_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0147<<2)))&BitMask_12)
#define B16_HSPw(val)			_wr32(_REG_BASE_+(0x0147<<2),(UINT)((_rd32(_REG_BASE_+(0x0147<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B16_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0147<<2)))&BitMask_12)
#define B16_HEDw(val)			_wr32(_REG_BASE_+(0x0147<<2),(UINT)((_rd32(_REG_BASE_+(0x0147<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B17_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0148<<2)))&BitMask_12)
#define B17_VSPw(val)			_wr32(_REG_BASE_+(0x0148<<2),(UINT)((_rd32(_REG_BASE_+(0x0148<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B17_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0148<<2)))&BitMask_12)
#define B17_VEDw(val)			_wr32(_REG_BASE_+(0x0148<<2),(UINT)((_rd32(_REG_BASE_+(0x0148<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B17_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0149<<2)))&BitMask_12)
#define B17_HSPw(val)			_wr32(_REG_BASE_+(0x0149<<2),(UINT)((_rd32(_REG_BASE_+(0x0149<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B17_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0149<<2)))&BitMask_12)
#define B17_HEDw(val)			_wr32(_REG_BASE_+(0x0149<<2),(UINT)((_rd32(_REG_BASE_+(0x0149<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B18_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014a<<2)))&BitMask_12)
#define B18_VSPw(val)			_wr32(_REG_BASE_+(0x014a<<2),(UINT)((_rd32(_REG_BASE_+(0x014a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B18_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014a<<2)))&BitMask_12)
#define B18_VEDw(val)			_wr32(_REG_BASE_+(0x014a<<2),(UINT)((_rd32(_REG_BASE_+(0x014a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B18_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014b<<2)))&BitMask_12)
#define B18_HSPw(val)			_wr32(_REG_BASE_+(0x014b<<2),(UINT)((_rd32(_REG_BASE_+(0x014b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B18_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014b<<2)))&BitMask_12)
#define B18_HEDw(val)			_wr32(_REG_BASE_+(0x014b<<2),(UINT)((_rd32(_REG_BASE_+(0x014b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B19_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014c<<2)))&BitMask_12)
#define B19_VSPw(val)			_wr32(_REG_BASE_+(0x014c<<2),(UINT)((_rd32(_REG_BASE_+(0x014c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B19_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014c<<2)))&BitMask_12)
#define B19_VEDw(val)			_wr32(_REG_BASE_+(0x014c<<2),(UINT)((_rd32(_REG_BASE_+(0x014c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B19_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014d<<2)))&BitMask_12)
#define B19_HSPw(val)			_wr32(_REG_BASE_+(0x014d<<2),(UINT)((_rd32(_REG_BASE_+(0x014d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B19_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014d<<2)))&BitMask_12)
#define B19_HEDw(val)			_wr32(_REG_BASE_+(0x014d<<2),(UINT)((_rd32(_REG_BASE_+(0x014d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B20_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014e<<2)))&BitMask_12)
#define B20_VSPw(val)			_wr32(_REG_BASE_+(0x014e<<2),(UINT)((_rd32(_REG_BASE_+(0x014e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B20_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014e<<2)))&BitMask_12)
#define B20_VEDw(val)			_wr32(_REG_BASE_+(0x014e<<2),(UINT)((_rd32(_REG_BASE_+(0x014e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B20_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x014f<<2)))&BitMask_12)
#define B20_HSPw(val)			_wr32(_REG_BASE_+(0x014f<<2),(UINT)((_rd32(_REG_BASE_+(0x014f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B20_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x014f<<2)))&BitMask_12)
#define B20_HEDw(val)			_wr32(_REG_BASE_+(0x014f<<2),(UINT)((_rd32(_REG_BASE_+(0x014f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B21_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_12)
#define B21_VSPw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B21_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_12)
#define B21_VEDw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B21_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0151<<2)))&BitMask_12)
#define B21_HSPw(val)			_wr32(_REG_BASE_+(0x0151<<2),(UINT)((_rd32(_REG_BASE_+(0x0151<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B21_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0151<<2)))&BitMask_12)
#define B21_HEDw(val)			_wr32(_REG_BASE_+(0x0151<<2),(UINT)((_rd32(_REG_BASE_+(0x0151<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B22_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0152<<2)))&BitMask_12)
#define B22_VSPw(val)			_wr32(_REG_BASE_+(0x0152<<2),(UINT)((_rd32(_REG_BASE_+(0x0152<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B22_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0152<<2)))&BitMask_12)
#define B22_VEDw(val)			_wr32(_REG_BASE_+(0x0152<<2),(UINT)((_rd32(_REG_BASE_+(0x0152<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B22_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0153<<2)))&BitMask_12)
#define B22_HSPw(val)			_wr32(_REG_BASE_+(0x0153<<2),(UINT)((_rd32(_REG_BASE_+(0x0153<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B22_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0153<<2)))&BitMask_12)
#define B22_HEDw(val)			_wr32(_REG_BASE_+(0x0153<<2),(UINT)((_rd32(_REG_BASE_+(0x0153<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B23_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_12)
#define B23_VSPw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B23_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_12)
#define B23_VEDw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B23_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_12)
#define B23_HSPw(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B23_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_12)
#define B23_HEDw(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B24_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_12)
#define B24_VSPw(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B24_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_12)
#define B24_VEDw(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B24_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_12)
#define B24_HSPw(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B24_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_12)
#define B24_HEDw(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B25_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_12)
#define B25_VSPw(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B25_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_12)
#define B25_VEDw(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B25_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0159<<2)))&BitMask_12)
#define B25_HSPw(val)			_wr32(_REG_BASE_+(0x0159<<2),(UINT)((_rd32(_REG_BASE_+(0x0159<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B25_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0159<<2)))&BitMask_12)
#define B25_HEDw(val)			_wr32(_REG_BASE_+(0x0159<<2),(UINT)((_rd32(_REG_BASE_+(0x0159<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B26_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015a<<2)))&BitMask_12)
#define B26_VSPw(val)			_wr32(_REG_BASE_+(0x015a<<2),(UINT)((_rd32(_REG_BASE_+(0x015a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B26_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015a<<2)))&BitMask_12)
#define B26_VEDw(val)			_wr32(_REG_BASE_+(0x015a<<2),(UINT)((_rd32(_REG_BASE_+(0x015a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B26_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015b<<2)))&BitMask_12)
#define B26_HSPw(val)			_wr32(_REG_BASE_+(0x015b<<2),(UINT)((_rd32(_REG_BASE_+(0x015b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B26_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015b<<2)))&BitMask_12)
#define B26_HEDw(val)			_wr32(_REG_BASE_+(0x015b<<2),(UINT)((_rd32(_REG_BASE_+(0x015b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B27_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015c<<2)))&BitMask_12)
#define B27_VSPw(val)			_wr32(_REG_BASE_+(0x015c<<2),(UINT)((_rd32(_REG_BASE_+(0x015c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B27_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015c<<2)))&BitMask_12)
#define B27_VEDw(val)			_wr32(_REG_BASE_+(0x015c<<2),(UINT)((_rd32(_REG_BASE_+(0x015c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B27_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015d<<2)))&BitMask_12)
#define B27_HSPw(val)			_wr32(_REG_BASE_+(0x015d<<2),(UINT)((_rd32(_REG_BASE_+(0x015d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B27_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015d<<2)))&BitMask_12)
#define B27_HEDw(val)			_wr32(_REG_BASE_+(0x015d<<2),(UINT)((_rd32(_REG_BASE_+(0x015d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B28_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015e<<2)))&BitMask_12)
#define B28_VSPw(val)			_wr32(_REG_BASE_+(0x015e<<2),(UINT)((_rd32(_REG_BASE_+(0x015e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B28_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015e<<2)))&BitMask_12)
#define B28_VEDw(val)			_wr32(_REG_BASE_+(0x015e<<2),(UINT)((_rd32(_REG_BASE_+(0x015e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B28_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x015f<<2)))&BitMask_12)
#define B28_HSPw(val)			_wr32(_REG_BASE_+(0x015f<<2),(UINT)((_rd32(_REG_BASE_+(0x015f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B28_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x015f<<2)))&BitMask_12)
#define B28_HEDw(val)			_wr32(_REG_BASE_+(0x015f<<2),(UINT)((_rd32(_REG_BASE_+(0x015f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B29_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0160<<2)))&BitMask_12)
#define B29_VSPw(val)			_wr32(_REG_BASE_+(0x0160<<2),(UINT)((_rd32(_REG_BASE_+(0x0160<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B29_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0160<<2)))&BitMask_12)
#define B29_VEDw(val)			_wr32(_REG_BASE_+(0x0160<<2),(UINT)((_rd32(_REG_BASE_+(0x0160<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B29_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0161<<2)))&BitMask_12)
#define B29_HSPw(val)			_wr32(_REG_BASE_+(0x0161<<2),(UINT)((_rd32(_REG_BASE_+(0x0161<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B29_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0161<<2)))&BitMask_12)
#define B29_HEDw(val)			_wr32(_REG_BASE_+(0x0161<<2),(UINT)((_rd32(_REG_BASE_+(0x0161<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B30_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0162<<2)))&BitMask_12)
#define B30_VSPw(val)			_wr32(_REG_BASE_+(0x0162<<2),(UINT)((_rd32(_REG_BASE_+(0x0162<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B30_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0162<<2)))&BitMask_12)
#define B30_VEDw(val)			_wr32(_REG_BASE_+(0x0162<<2),(UINT)((_rd32(_REG_BASE_+(0x0162<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B30_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0163<<2)))&BitMask_12)
#define B30_HSPw(val)			_wr32(_REG_BASE_+(0x0163<<2),(UINT)((_rd32(_REG_BASE_+(0x0163<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B30_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0163<<2)))&BitMask_12)
#define B30_HEDw(val)			_wr32(_REG_BASE_+(0x0163<<2),(UINT)((_rd32(_REG_BASE_+(0x0163<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B31_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0164<<2)))&BitMask_12)
#define B31_VSPw(val)			_wr32(_REG_BASE_+(0x0164<<2),(UINT)((_rd32(_REG_BASE_+(0x0164<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B31_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0164<<2)))&BitMask_12)
#define B31_VEDw(val)			_wr32(_REG_BASE_+(0x0164<<2),(UINT)((_rd32(_REG_BASE_+(0x0164<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B31_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0165<<2)))&BitMask_12)
#define B31_HSPw(val)			_wr32(_REG_BASE_+(0x0165<<2),(UINT)((_rd32(_REG_BASE_+(0x0165<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B31_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x0165<<2)))&BitMask_12)
#define B31_HEDw(val)			_wr32(_REG_BASE_+(0x0165<<2),(UINT)((_rd32(_REG_BASE_+(0x0165<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BOX0_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0167<<2)))&BitMask_24)
#define BOX0_PLTw(val)			_wr32(_REG_BASE_+(0x0167<<2),(UINT)((_rd32(_REG_BASE_+(0x0167<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX1_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_24)
#define BOX1_PLTw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX2_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0169<<2)))&BitMask_24)
#define BOX2_PLTw(val)			_wr32(_REG_BASE_+(0x0169<<2),(UINT)((_rd32(_REG_BASE_+(0x0169<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX3_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016a<<2)))&BitMask_24)
#define BOX3_PLTw(val)			_wr32(_REG_BASE_+(0x016a<<2),(UINT)((_rd32(_REG_BASE_+(0x016a<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX4_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016b<<2)))&BitMask_24)
#define BOX4_PLTw(val)			_wr32(_REG_BASE_+(0x016b<<2),(UINT)((_rd32(_REG_BASE_+(0x016b<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX5_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016c<<2)))&BitMask_24)
#define BOX5_PLTw(val)			_wr32(_REG_BASE_+(0x016c<<2),(UINT)((_rd32(_REG_BASE_+(0x016c<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX6_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016d<<2)))&BitMask_24)
#define BOX6_PLTw(val)			_wr32(_REG_BASE_+(0x016d<<2),(UINT)((_rd32(_REG_BASE_+(0x016d<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX7_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016e<<2)))&BitMask_24)
#define BOX7_PLTw(val)			_wr32(_REG_BASE_+(0x016e<<2),(UINT)((_rd32(_REG_BASE_+(0x016e<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX8_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x016f<<2)))&BitMask_24)
#define BOX8_PLTw(val)			_wr32(_REG_BASE_+(0x016f<<2),(UINT)((_rd32(_REG_BASE_+(0x016f<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX9_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_24)
#define BOX9_PLTw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX10_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_24)
#define BOX10_PLTw(val)			_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX11_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_24)
#define BOX11_PLTw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX12_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0173<<2)))&BitMask_24)
#define BOX12_PLTw(val)			_wr32(_REG_BASE_+(0x0173<<2),(UINT)((_rd32(_REG_BASE_+(0x0173<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX13_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0174<<2)))&BitMask_24)
#define BOX13_PLTw(val)			_wr32(_REG_BASE_+(0x0174<<2),(UINT)((_rd32(_REG_BASE_+(0x0174<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX14_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0175<<2)))&BitMask_24)
#define BOX14_PLTw(val)			_wr32(_REG_BASE_+(0x0175<<2),(UINT)((_rd32(_REG_BASE_+(0x0175<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX15_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0176<<2)))&BitMask_24)
#define BOX15_PLTw(val)			_wr32(_REG_BASE_+(0x0176<<2),(UINT)((_rd32(_REG_BASE_+(0x0176<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX16_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0177<<2)))&BitMask_24)
#define BOX16_PLTw(val)			_wr32(_REG_BASE_+(0x0177<<2),(UINT)((_rd32(_REG_BASE_+(0x0177<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX17_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_24)
#define BOX17_PLTw(val)			_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX18_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0179<<2)))&BitMask_24)
#define BOX18_PLTw(val)			_wr32(_REG_BASE_+(0x0179<<2),(UINT)((_rd32(_REG_BASE_+(0x0179<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX19_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_24)
#define BOX19_PLTw(val)			_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX20_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017b<<2)))&BitMask_24)
#define BOX20_PLTw(val)			_wr32(_REG_BASE_+(0x017b<<2),(UINT)((_rd32(_REG_BASE_+(0x017b<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX21_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017c<<2)))&BitMask_24)
#define BOX21_PLTw(val)			_wr32(_REG_BASE_+(0x017c<<2),(UINT)((_rd32(_REG_BASE_+(0x017c<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX22_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017d<<2)))&BitMask_24)
#define BOX22_PLTw(val)			_wr32(_REG_BASE_+(0x017d<<2),(UINT)((_rd32(_REG_BASE_+(0x017d<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX23_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017e<<2)))&BitMask_24)
#define BOX23_PLTw(val)			_wr32(_REG_BASE_+(0x017e<<2),(UINT)((_rd32(_REG_BASE_+(0x017e<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX24_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x017f<<2)))&BitMask_24)
#define BOX24_PLTw(val)			_wr32(_REG_BASE_+(0x017f<<2),(UINT)((_rd32(_REG_BASE_+(0x017f<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX25_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0180<<2)))&BitMask_24)
#define BOX25_PLTw(val)			_wr32(_REG_BASE_+(0x0180<<2),(UINT)((_rd32(_REG_BASE_+(0x0180<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX26_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0181<<2)))&BitMask_24)
#define BOX26_PLTw(val)			_wr32(_REG_BASE_+(0x0181<<2),(UINT)((_rd32(_REG_BASE_+(0x0181<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX27_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0182<<2)))&BitMask_24)
#define BOX27_PLTw(val)			_wr32(_REG_BASE_+(0x0182<<2),(UINT)((_rd32(_REG_BASE_+(0x0182<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX28_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0183<<2)))&BitMask_24)
#define BOX28_PLTw(val)			_wr32(_REG_BASE_+(0x0183<<2),(UINT)((_rd32(_REG_BASE_+(0x0183<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX29_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0184<<2)))&BitMask_24)
#define BOX29_PLTw(val)			_wr32(_REG_BASE_+(0x0184<<2),(UINT)((_rd32(_REG_BASE_+(0x0184<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX30_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0185<<2)))&BitMask_24)
#define BOX30_PLTw(val)			_wr32(_REG_BASE_+(0x0185<<2),(UINT)((_rd32(_REG_BASE_+(0x0185<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX31_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0186<<2)))&BitMask_24)
#define BOX31_PLTw(val)			_wr32(_REG_BASE_+(0x0186<<2),(UINT)((_rd32(_REG_BASE_+(0x0186<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BITBOXr					(ValSft_R00(_rd32(_REG_BASE_+(0x0187<<2)))&BitMask_32)
#define BITBOXw(val)			_wr32(_REG_BASE_+(0x0187<<2),(UINT)((_rd32(_REG_BASE_+(0x0187<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define BIT_CBr					(ValSft_R20(_rd32(_REG_BASE_+(0x0188<<2)))&BitMask_10)
#define BIT_CBw(val)			_wr32(_REG_BASE_+(0x0188<<2),(UINT)((_rd32(_REG_BASE_+(0x0188<<2))&(~ValSft_L20(BitMask_10)))|ValSft_L20((val)&BitMask_10)))

#define BIT_CRr					(ValSft_R10(_rd32(_REG_BASE_+(0x0188<<2)))&BitMask_10)
#define BIT_CRw(val)			_wr32(_REG_BASE_+(0x0188<<2),(UINT)((_rd32(_REG_BASE_+(0x0188<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define BITMAP_THRESr			(ValSft_R00(_rd32(_REG_BASE_+(0x0188<<2)))&BitMask_10)
#define BITMAP_THRESw(val)		_wr32(_REG_BASE_+(0x0188<<2),(UINT)((_rd32(_REG_BASE_+(0x0188<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_ADRr				(ValSft_R00(_rd32(_REG_BASE_+(0x0190<<2)))&BitMask_24)
#define WDR_ADRw(val)			_wr32(_REG_BASE_+(0x0190<<2),(UINT)((_rd32(_REG_BASE_+(0x0190<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_YADR_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0191<<2)))&BitMask_24)
#define ISP_YADR_P0w(val)		_wr32(_REG_BASE_+(0x0191<<2),(UINT)((_rd32(_REG_BASE_+(0x0191<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_CADR_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0192<<2)))&BitMask_24)
#define ISP_CADR_P0w(val)		_wr32(_REG_BASE_+(0x0192<<2),(UINT)((_rd32(_REG_BASE_+(0x0192<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_YADR_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0193<<2)))&BitMask_24)
#define ISP_YADR_P1w(val)		_wr32(_REG_BASE_+(0x0193<<2),(UINT)((_rd32(_REG_BASE_+(0x0193<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_CADR_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0194<<2)))&BitMask_24)
#define ISP_CADR_P1w(val)		_wr32(_REG_BASE_+(0x0194<<2),(UINT)((_rd32(_REG_BASE_+(0x0194<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_YADR_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0195<<2)))&BitMask_24)
#define ISP_YADR_P2w(val)		_wr32(_REG_BASE_+(0x0195<<2),(UINT)((_rd32(_REG_BASE_+(0x0195<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_CADR_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_24)
#define ISP_CADR_P2w(val)		_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_YADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0197<<2)))&BitMask_24)
#define ISP_YADR0w(val)			_wr32(_REG_BASE_+(0x0197<<2),(UINT)((_rd32(_REG_BASE_+(0x0197<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_CADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0198<<2)))&BitMask_24)
#define ISP_CADR0w(val)			_wr32(_REG_BASE_+(0x0198<<2),(UINT)((_rd32(_REG_BASE_+(0x0198<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_YADR1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0199<<2)))&BitMask_24)
#define ISP_YADR1w(val)			_wr32(_REG_BASE_+(0x0199<<2),(UINT)((_rd32(_REG_BASE_+(0x0199<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define ISP_CADR1r				(ValSft_R00(_rd32(_REG_BASE_+(0x019a<<2)))&BitMask_24)
#define ISP_CADR1w(val)			_wr32(_REG_BASE_+(0x019a<<2),(UINT)((_rd32(_REG_BASE_+(0x019a<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define DDR_RD_MODr				(ValSft_R31(_rd32(_REG_BASE_+(0x019b<<2)))&BitMask_01)
#define DDR_RD_MODw(val)		_wr32(_REG_BASE_+(0x019b<<2),(UINT)((_rd32(_REG_BASE_+(0x019b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DDR_RWDR_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x019b<<2)))&BitMask_12)
#define DDR_RWDR_LTCw(val)		_wr32(_REG_BASE_+(0x019b<<2),(UINT)((_rd32(_REG_BASE_+(0x019b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DDR_RYC0_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x019c<<2)))&BitMask_12)
#define DDR_RYC0_LTCw(val)		_wr32(_REG_BASE_+(0x019c<<2),(UINT)((_rd32(_REG_BASE_+(0x019c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define BT0_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_02)
#define BT0_VISELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BT0_ISELr				(ValSft_R24(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_03)
#define BT0_ISELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define BT0_HWSELr				(ValSft_R21(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_02)
#define BT0_HWSELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L21(BitMask_02)))|ValSft_L21((val)&BitMask_02)))

#define BT1_VISELr				(ValSft_R18(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_02)
#define BT1_VISELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define BT1_ISELr				(ValSft_R14(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_03)
#define BT1_ISELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L14(BitMask_03)))|ValSft_L14((val)&BitMask_03)))

#define BT1_HWSELr				(ValSft_R11(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_02)
#define BT1_HWSELw(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L11(BitMask_02)))|ValSft_L11((val)&BitMask_02)))

#define YC_FRC0_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define YC_FRC0_ONw(val)		_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ISP_ISELr				(ValSft_R27(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_03)
#define ISP_ISELw(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L27(BitMask_03)))|ValSft_L27((val)&BitMask_03)))

#define ISP_HWSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_02)
#define ISP_HWSELw(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define ISP_VISELr				(ValSft_R20(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_02)
#define ISP_VISELw(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define ISP_WCINV1r				(ValSft_R06(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCINV1w(val)		_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define ISP_WCLINE_INV1r		(ValSft_R05(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCLINE_INV1w(val)	_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define ISP_WCINV0r				(ValSft_R04(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCINV0w(val)		_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define ISP_WCLINE_INV0r		(ValSft_R03(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCLINE_INV0w(val)	_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define ISP_WCINVr				(ValSft_R01(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCINVw(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define ISP_WCLINE_INVr			(ValSft_R00(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_01)
#define ISP_WCLINE_INVw(val)	_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define BT_DS_HWI0r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a2<<2)))&BitMask_12)
#define BT_DS_HWI0w(val)		_wr32(_REG_BASE_+(0x01a2<<2),(UINT)((_rd32(_REG_BASE_+(0x01a2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BT_DS_HWI1r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_12)
#define BT_DS_HWI1w(val)		_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BT_DS_HWI2r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_12)
#define BT_DS_HWI2w(val)		_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define ISP_RADR0_AONr			(ValSft_R31(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define ISP_RADR0_AONw(val)		_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ISP_RON0r				(ValSft_R30(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define ISP_RON0w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ISP_CINV0r				(ValSft_R29(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define ISP_CINV0w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RYC0_FR_ONr				(ValSft_R23(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define RYC0_FR_ONw(val)		_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define ISP_GOr					(ValSft_R31(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_GOw(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ISP_CGOr				(ValSft_R30(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_CGOw(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ISP_GO0r				(ValSft_R29(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_GO0w(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define ISP_CGO0r				(ValSft_R28(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_CGO0w(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define ISP_GO1r				(ValSft_R27(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_GO1w(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ISP_CGO1r				(ValSft_R26(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define ISP_CGO1w(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define DDR_MEM_WADR_RSTr		(ValSft_R23(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define DDR_MEM_WADR_RSTw(val)	_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define DDR_MEM_RADR_RSTr		(ValSft_R22(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_01)
#define DDR_MEM_RADR_RSTw(val)	_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define DS0_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_01)
#define DS0_ONw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS0_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_01)
#define DS0_KXY_ONw(val)		_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS0_HLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_02)
#define DS0_HLPFw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define DS0_VLPFr				(ValSft_R26(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_02)
#define DS0_VLPFw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define CDS0_ISELr				(ValSft_R20(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_03)
#define CDS0_ISELw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define DS0_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_10)
#define DS0_DKXw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS0_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_10)
#define DS0_DKYw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS0_DKY_STr				(ValSft_R26(_rd32(_REG_BASE_+(0x01a9<<2)))&BitMask_06)
#define DS0_DKY_STw(val)		_wr32(_REG_BASE_+(0x01a9<<2),(UINT)((_rd32(_REG_BASE_+(0x01a9<<2))&(~ValSft_L26(BitMask_06)))|ValSft_L26((val)&BitMask_06)))

#define DS0_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x01a9<<2)))&BitMask_13)
#define DS0_HOFFw(val)			_wr32(_REG_BASE_+(0x01a9<<2),(UINT)((_rd32(_REG_BASE_+(0x01a9<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS0_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x01a9<<2)))&BitMask_12)
#define DS0_VOFFw(val)			_wr32(_REG_BASE_+(0x01a9<<2),(UINT)((_rd32(_REG_BASE_+(0x01a9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS0_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_01)
#define DS0_APTONw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS0_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_08)
#define DS0_APTGAw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS0_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_08)
#define DS0_APTSLw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS0_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_08)
#define DS0_APTCLw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS0_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x01ab<<2)))&BitMask_03)
#define CDS0_VISELw(val)		_wr32(_REG_BASE_+(0x01ab<<2),(UINT)((_rd32(_REG_BASE_+(0x01ab<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define DS0_HWr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ab<<2)))&BitMask_12)
#define DS0_HWw(val)			_wr32(_REG_BASE_+(0x01ab<<2),(UINT)((_rd32(_REG_BASE_+(0x01ab<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS0_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ab<<2)))&BitMask_12)
#define DS0_VWw(val)			_wr32(_REG_BASE_+(0x01ab<<2),(UINT)((_rd32(_REG_BASE_+(0x01ab<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_01)
#define DS1_ONw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS1_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_01)
#define DS1_KXY_ONw(val)		_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS1_HLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_02)
#define DS1_HLPFw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define DS1_VLPFr				(ValSft_R26(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_02)
#define DS1_VLPFw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define CDS1_ISELr				(ValSft_R20(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_03)
#define CDS1_ISELw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define DS1_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_10)
#define DS1_DKXw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS1_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_10)
#define DS1_DKYw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS1_DKY_STr				(ValSft_R26(_rd32(_REG_BASE_+(0x01b1<<2)))&BitMask_06)
#define DS1_DKY_STw(val)		_wr32(_REG_BASE_+(0x01b1<<2),(UINT)((_rd32(_REG_BASE_+(0x01b1<<2))&(~ValSft_L26(BitMask_06)))|ValSft_L26((val)&BitMask_06)))

#define DS1_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x01b1<<2)))&BitMask_13)
#define DS1_HOFFw(val)			_wr32(_REG_BASE_+(0x01b1<<2),(UINT)((_rd32(_REG_BASE_+(0x01b1<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS1_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x01b1<<2)))&BitMask_12)
#define DS1_VOFFw(val)			_wr32(_REG_BASE_+(0x01b1<<2),(UINT)((_rd32(_REG_BASE_+(0x01b1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_01)
#define DS1_APTONw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS1_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_08)
#define DS1_APTGAw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS1_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_08)
#define DS1_APTSLw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS1_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_08)
#define DS1_APTCLw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS1_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x01b3<<2)))&BitMask_03)
#define CDS1_VISELw(val)		_wr32(_REG_BASE_+(0x01b3<<2),(UINT)((_rd32(_REG_BASE_+(0x01b3<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define DS1_HWr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b3<<2)))&BitMask_12)
#define DS1_HWw(val)			_wr32(_REG_BASE_+(0x01b3<<2),(UINT)((_rd32(_REG_BASE_+(0x01b3<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS1_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b3<<2)))&BitMask_12)
#define DS1_VWw(val)			_wr32(_REG_BASE_+(0x01b3<<2),(UINT)((_rd32(_REG_BASE_+(0x01b3<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP0_RONr				(ValSft_R31(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_01)
#define PIP0_RONw(val)			_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP0_ONr				(ValSft_R29(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_01)
#define PIP0_ONw(val)			_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PIP0_BOX_ONr			(ValSft_R28(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_01)
#define PIP0_BOX_ONw(val)		_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PIP_CTOG_INVr			(ValSft_R26(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_01)
#define PIP_CTOG_INVw(val)		_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define PIP_CSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_02)
#define PIP_CSELw(val)			_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PIP0_BOX_Yr				(ValSft_R16(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_08)
#define PIP0_BOX_Yw(val)		_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PIP0_BOX_CBr			(ValSft_R08(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_08)
#define PIP0_BOX_CBw(val)		_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PIP0_BOX_CRr			(ValSft_R00(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_08)
#define PIP0_BOX_CRw(val)		_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PIP0_SUB_ISELr			(ValSft_R31(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_01)
#define PIP0_SUB_ISELw(val)		_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP_MAIN_ISELr			(ValSft_R28(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_03)
#define PIP_MAIN_ISELw(val)		_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define PIP0_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_10)
#define PIP0_HSPw(val)			_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define PIP0_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_10)
#define PIP0_VSPw(val)			_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PIP0_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x01b7<<2)))&BitMask_10)
#define PIP0_HWw(val)			_wr32(_REG_BASE_+(0x01b7<<2),(UINT)((_rd32(_REG_BASE_+(0x01b7<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define PIP0_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x01b7<<2)))&BitMask_10)
#define PIP0_VWw(val)			_wr32(_REG_BASE_+(0x01b7<<2),(UINT)((_rd32(_REG_BASE_+(0x01b7<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PIP_DDR_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x01b8<<2)))&BitMask_12)
#define PIP_DDR_LTCw(val)		_wr32(_REG_BASE_+(0x01b8<<2),(UINT)((_rd32(_REG_BASE_+(0x01b8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define JPG_GOr					(ValSft_R31(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_01)
#define JPG_GOw(val)			_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_ZRL_CUTr			(ValSft_R30(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_01)
#define JPG_ZRL_CUTw(val)		_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_RST_IVLr			(ValSft_R28(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_02)
#define JPG_RST_IVLw(val)		_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define JPG_QP0r				(ValSft_R22(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_06)
#define JPG_QP0w(val)			_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L22(BitMask_06)))|ValSft_L22((val)&BitMask_06)))

#define JPG_QP1r				(ValSft_R20(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_02)
#define JPG_QP1w(val)			_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define JPG_IVSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x01bb<<2)))&BitMask_02)
#define JPG_IVSELw(val)			_wr32(_REG_BASE_+(0x01bb<<2),(UINT)((_rd32(_REG_BASE_+(0x01bb<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define JPG_ISELr				(ValSft_R24(_rd32(_REG_BASE_+(0x01bb<<2)))&BitMask_04)
#define JPG_ISELw(val)			_wr32(_REG_BASE_+(0x01bb<<2),(UINT)((_rd32(_REG_BASE_+(0x01bb<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define JPG_ADRr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bb<<2)))&BitMask_24)
#define JPG_ADRw(val)			_wr32(_REG_BASE_+(0x01bb<<2),(UINT)((_rd32(_REG_BASE_+(0x01bb<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define JPG_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x01bc<<2)))&BitMask_01)
#define JPG_ONw(val)			_wr32(_REG_BASE_+(0x01bc<<2),(UINT)((_rd32(_REG_BASE_+(0x01bc<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bc<<2)))&BitMask_27)
#define JPG_MAXw(val)			_wr32(_REG_BASE_+(0x01bc<<2),(UINT)((_rd32(_REG_BASE_+(0x01bc<<2))&(~ValSft_L00(BitMask_27)))|ValSft_L00((val)&BitMask_27)))

#define JPG_HWr					(ValSft_R16(_rd32(_REG_BASE_+(0x01bd<<2)))&BitMask_12)
#define JPG_HWw(val)			_wr32(_REG_BASE_+(0x01bd<<2),(UINT)((_rd32(_REG_BASE_+(0x01bd<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define JPG_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x01bd<<2)))&BitMask_12)
#define JPG_VWw(val)			_wr32(_REG_BASE_+(0x01bd<<2),(UINT)((_rd32(_REG_BASE_+(0x01bd<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define JPG_HBLK_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x01be<<2)))&BitMask_01)
#define JPG_HBLK_SELw(val)		_wr32(_REG_BASE_+(0x01be<<2),(UINT)((_rd32(_REG_BASE_+(0x01be<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_HBLK_CYCr			(ValSft_R16(_rd32(_REG_BASE_+(0x01be<<2)))&BitMask_14)
#define JPG_HBLK_CYCw(val)		_wr32(_REG_BASE_+(0x01be<<2),(UINT)((_rd32(_REG_BASE_+(0x01be<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define JPG_VOFSr				(ValSft_R04(_rd32(_REG_BASE_+(0x01be<<2)))&BitMask_06)
#define JPG_VOFSw(val)			_wr32(_REG_BASE_+(0x01be<<2),(UINT)((_rd32(_REG_BASE_+(0x01be<<2))&(~ValSft_L04(BitMask_06)))|ValSft_L04((val)&BitMask_06)))

#define JVLOCK_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bf<<2)))&BitMask_06)
#define JVLOCK_POSw(val)		_wr32(_REG_BASE_+(0x01bf<<2),(UINT)((_rd32(_REG_BASE_+(0x01bf<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define MYCW_HWOr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c0<<2)))&BitMask_12)
#define MYCW_HWOw(val)			_wr32(_REG_BASE_+(0x01c0<<2),(UINT)((_rd32(_REG_BASE_+(0x01c0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define VIRQI_EN1r				(ValSft_R31(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define VIRQI_EN1w(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VIRQW_EN1r				(ValSft_R30(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define VIRQW_EN1w(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define VIRQO_EN1r				(ValSft_R29(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define VIRQO_EN1w(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define JPGIRQ_ENr				(ValSft_R28(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define JPGIRQ_ENw(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define VIRQW2_EN1r				(ValSft_R27(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define VIRQW2_EN1w(val)		_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define CLI_VLOCKW2_1r			(ValSft_R04(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define CLI_VLOCKW2_1w(val)		_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define CLI_JPGr				(ValSft_R03(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define CLI_JPGw(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define CLI_VLOCKI1r			(ValSft_R02(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define CLI_VLOCKI1w(val)		_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define CLI_VLOCKW1r			(ValSft_R01(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define CLI_VLOCKW1w(val)		_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define CLI_VLOCKO1r			(ValSft_R00(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_01)
#define CLI_VLOCKO1w(val)		_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define IP_UIRQ0_CK_SELr		(ValSft_R12(_rd32(_REG_BASE_+(0x01c2<<2)))&BitMask_03)
#define IP_UIRQ0_CK_SELw(val)	_wr32(_REG_BASE_+(0x01c2<<2),(UINT)((_rd32(_REG_BASE_+(0x01c2<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define IP_UIRQ1_CK_SELr		(ValSft_R08(_rd32(_REG_BASE_+(0x01c2<<2)))&BitMask_03)
#define IP_UIRQ1_CK_SELw(val)	_wr32(_REG_BASE_+(0x01c2<<2),(UINT)((_rd32(_REG_BASE_+(0x01c2<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define IP_UIRQ0_CNT_SELr		(ValSft_R30(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_02)
#define IP_UIRQ0_CNT_SELw(val)	_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define IP_CLIUIRQ0r			(ValSft_R29(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_01)
#define IP_CLIUIRQ0w(val)		_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IP_UIRQ0_ENr			(ValSft_R28(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_01)
#define IP_UIRQ0_ENw(val)		_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IP_UIRQ_VPOS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_12)
#define IP_UIRQ_VPOS0w(val)		_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define IP_UIRQ_HPOS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_12)
#define IP_UIRQ_HPOS0w(val)		_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define IP_UIRQ1_CNT_SELr		(ValSft_R30(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_02)
#define IP_UIRQ1_CNT_SELw(val)	_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define IP_CLIUIRQ1r			(ValSft_R29(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_01)
#define IP_CLIUIRQ1w(val)		_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IP_UIRQ1_ENr			(ValSft_R28(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_01)
#define IP_UIRQ1_ENw(val)		_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IP_UIRQ_VPOS1r			(ValSft_R16(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_12)
#define IP_UIRQ_VPOS1w(val)		_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define IP_UIRQ_HPOS1r			(ValSft_R00(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_12)
#define IP_UIRQ_HPOS1w(val)		_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define ITU_SCON_EX0r			(ValSft_R30(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define ITU_SCON_EX0w(val)		_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ITU_ORDR_EX0r			(ValSft_R29(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define ITU_ORDR_EX0w(val)		_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define LNUM_EXT_ONr			(ValSft_R16(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define LNUM_EXT_ONw(val)		_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define PLGL_TOG_ONr			(ValSft_R14(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define PLGL_TOG_ONw(val)		_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define CTOG_INVr				(ValSft_R13(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define CTOG_INVw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define ITU_LCONr				(ValSft_R11(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define ITU_LCONw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define ITU_VIDr				(ValSft_R10(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define ITU_VIDw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define ITU_FIDr				(ValSft_R09(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define ITU_FIDw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define SDI_YCCH0r				(ValSft_R08(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_01)
#define SDI_YCCH0w(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define BT20_OSEL0r				(ValSft_R00(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_02)
#define BT20_OSEL0w(val)		_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

//	#define LAST_YCLIPr				(ValSft_R24(_rd32(_REG_BASE_+(0x01c7<<2)))&BitMask_08)
//	#define LAST_YCLIPw(val)		_wr32(_REG_BASE_+(0x01c7<<2),(UINT)((_rd32(_REG_BASE_+(0x01c7<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))
//	
//	#define LAST_YSLICr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c7<<2)))&BitMask_08)
//	#define LAST_YSLICw(val)		_wr32(_REG_BASE_+(0x01c7<<2),(UINT)((_rd32(_REG_BASE_+(0x01c7<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))
//	
//	#define LAST_YOFFSr				(ValSft_R08(_rd32(_REG_BASE_+(0x01c7<<2)))&BitMask_08)
//	#define LAST_YOFFSw(val)		_wr32(_REG_BASE_+(0x01c7<<2),(UINT)((_rd32(_REG_BASE_+(0x01c7<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))
//	
//	#define LAST_CGAINr				(ValSft_R24(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_08)
//	#define LAST_CGAINw(val)		_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))
//	
//	#define LAST_YGAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_08)
//	#define LAST_YGAINw(val)		_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))
//	
//	#define LAST_CCLIPr				(ValSft_R08(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_08)
//	#define LAST_CCLIPw(val)		_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))
//	
//	#define LAST_CSLICr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_08)
//	#define LAST_CSLICw(val)		_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PV_SELr					(ValSft_R30(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_02)
#define PV_SELw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DCKO0_INVr				(ValSft_R28(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_01)
#define DCKO0_INVw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define DCKO0_ONr				(ValSft_R27(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_01)
#define DCKO0_ONw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define DCKO0_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_03)
#define DCKO0_SELw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define DCKO0_DLYr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_04)
#define DCKO0_DLYw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define AF_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ca<<2)))&BitMask_12)
#define AF_HSPw(val)			_wr32(_REG_BASE_+(0x01ca<<2),(UINT)((_rd32(_REG_BASE_+(0x01ca<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF_HEPr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ca<<2)))&BitMask_12)
#define AF_HEPw(val)			_wr32(_REG_BASE_+(0x01ca<<2),(UINT)((_rd32(_REG_BASE_+(0x01ca<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01cb<<2)))&BitMask_12)
#define AF_VSPw(val)			_wr32(_REG_BASE_+(0x01cb<<2),(UINT)((_rd32(_REG_BASE_+(0x01cb<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF_VEPr					(ValSft_R00(_rd32(_REG_BASE_+(0x01cb<<2)))&BitMask_12)
#define AF_VEPw(val)			_wr32(_REG_BASE_+(0x01cb<<2),(UINT)((_rd32(_REG_BASE_+(0x01cb<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF_EDGE_TH1r			(ValSft_R16(_rd32(_REG_BASE_+(0x01cc<<2)))&BitMask_10)
#define AF_EDGE_TH1w(val)		_wr32(_REG_BASE_+(0x01cc<<2),(UINT)((_rd32(_REG_BASE_+(0x01cc<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AF_EDGE_TH2r			(ValSft_R00(_rd32(_REG_BASE_+(0x01cc<<2)))&BitMask_10)
#define AF_EDGE_TH2w(val)		_wr32(_REG_BASE_+(0x01cc<<2),(UINT)((_rd32(_REG_BASE_+(0x01cc<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AF_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01ce<<2)))&BitMask_10)
#define AF_CLIPw(val)			_wr32(_REG_BASE_+(0x01ce<<2),(UINT)((_rd32(_REG_BASE_+(0x01ce<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define AF_SLICEr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ce<<2)))&BitMask_10)
#define AF_SLICEw(val)			_wr32(_REG_BASE_+(0x01ce<<2),(UINT)((_rd32(_REG_BASE_+(0x01ce<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AF_PDr					(ValSft_R31(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_01)
#define AF_PDw(val)				_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AF_CLIP_ONr				(ValSft_R28(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_01)
#define AF_CLIP_ONw(val)		_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define AF_WIN_SELr				(ValSft_R26(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_02)
#define AF_WIN_SELw(val)		_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define AF_CWD_HSELr			(ValSft_R24(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_02)
#define AF_CWD_HSELw(val)		_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define AF_CWD_VSELr			(ValSft_R22(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_02)
#define AF_CWD_VSELw(val)		_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define AF_CWD_WGTr				(ValSft_R16(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_03)
#define AF_CWD_WGTw(val)		_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define HLOCKO_POSr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d0<<2)))&BitMask_12)
#define HLOCKO_POSw(val)		_wr32(_REG_BASE_+(0x01d0<<2),(UINT)((_rd32(_REG_BASE_+(0x01d0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VLOCKO_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d0<<2)))&BitMask_12)
#define VLOCKO_POSw(val)		_wr32(_REG_BASE_+(0x01d0<<2),(UINT)((_rd32(_REG_BASE_+(0x01d0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSPCr					(ValSft_R16(_rd32(_REG_BASE_+(0x01d1<<2)))&BitMask_12)
#define HSPCw(val)				_wr32(_REG_BASE_+(0x01d1<<2),(UINT)((_rd32(_REG_BASE_+(0x01d1<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSPCr					(ValSft_R00(_rd32(_REG_BASE_+(0x01d1<<2)))&BitMask_12)
#define VSPCw(val)				_wr32(_REG_BASE_+(0x01d1<<2),(UINT)((_rd32(_REG_BASE_+(0x01d1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWCr					(ValSft_R16(_rd32(_REG_BASE_+(0x01d2<<2)))&BitMask_12)
#define HWCw(val)				_wr32(_REG_BASE_+(0x01d2<<2),(UINT)((_rd32(_REG_BASE_+(0x01d2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWCr					(ValSft_R00(_rd32(_REG_BASE_+(0x01d2<<2)))&BitMask_12)
#define VWCw(val)				_wr32(_REG_BASE_+(0x01d2<<2),(UINT)((_rd32(_REG_BASE_+(0x01d2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE1_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0200<<2)))&BitMask_31)
#define AE1_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0201<<2)))&BitMask_31)
#define AE2_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0202<<2)))&BitMask_31)
#define AE2_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0203<<2)))&BitMask_31)
#define AE3_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0204<<2)))&BitMask_31)
#define AE3_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0205<<2)))&BitMask_31)
#define AE4_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0206<<2)))&BitMask_31)
#define AE4_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0207<<2)))&BitMask_31)
#define AE1_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0208<<2)))&BitMask_21)
#define AE2_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0209<<2)))&BitMask_21)
#define AE3_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020a<<2)))&BitMask_21)
#define AE4_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020b<<2)))&BitMask_21)
#define AE1_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020c<<2)))&BitMask_21)
#define AE2_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020d<<2)))&BitMask_21)
#define AE3_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020e<<2)))&BitMask_21)
#define AE4_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x020f<<2)))&BitMask_21)
#define HISTO0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_21)
#define HISTO1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_21)
#define HISTO2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_21)
#define HISTO3_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0213<<2)))&BitMask_21)
#define HISTO4_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0214<<2)))&BitMask_21)
#define HISTO5_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_21)
#define HISTO6_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_21)
#define HISTO7_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0217<<2)))&BitMask_21)
#define HISTO8_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0218<<2)))&BitMask_21)
#define HISTO9_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0219<<2)))&BitMask_21)
#define HISTO10_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021a<<2)))&BitMask_21)
#define HISTO11_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021b<<2)))&BitMask_21)
#define HISTO12_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_21)
#define HISTO13_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_21)
#define HISTO14_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_21)
#define HISTO15_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x021f<<2)))&BitMask_21)
#define AWBL_CCR_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_31)
#define AWBL_CCG_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_31)
#define AWBL_CCB_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0222<<2)))&BitMask_31)
#define AWBL_CCT_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0223<<2)))&BitMask_21)
#define AWBL_CY_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_31)
#define AWBL_CBDB_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0225<<2)))&BitMask_32)
#define AWBL_CRDR_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0226<<2)))&BitMask_32)
#define AE1_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_31)
#define AE2_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0228<<2)))&BitMask_31)
#define AE3_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0229<<2)))&BitMask_31)
#define AE4_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_31)
#define AF_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0230<<2)))&BitMask_31)
#define AF_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_31)
#define AF_CSUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0232<<2)))&BitMask_31)
#define AF_CSUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0233<<2)))&BitMask_31)
#define AF_YSUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0234<<2)))&BitMask_31)
#define AF_YSUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_31)
#define AF_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0236<<2)))&BitMask_21)
#define AF_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_21)
#define AF_EGCNT1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0238<<2)))&BitMask_21)
#define AF_EGCNT2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0239<<2)))&BitMask_21)
#define CH1_HDETr				(ValSft_R00(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_12)
#define CH2_HDETr				(ValSft_R00(_rd32(_REG_BASE_+(0x0241<<2)))&BitMask_12)
#define CH3_HDETr				(ValSft_R00(_rd32(_REG_BASE_+(0x0242<<2)))&BitMask_12)
#define CH4_HDETr				(ValSft_R00(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_12)
#define SYNC4_STATUSr			(ValSft_R03(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_01)
#define SYNC3_STATUSr			(ValSft_R02(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_01)
#define SYNC2_STATUSr			(ValSft_R01(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_01)
#define SYNC1_STATUSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_01)
#define ISP_WPAGEr				(ValSft_R02(_rd32(_REG_BASE_+(0x0245<<2)))&BitMask_02)
#define ISP_WPAGE1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0245<<2)))&BitMask_02)
#define HCNTI_SVr				(ValSft_R16(_rd32(_REG_BASE_+(0x0246<<2)))&BitMask_12)
#define VCNTI_SVr				(ValSft_R00(_rd32(_REG_BASE_+(0x0246<<2)))&BitMask_12)
#define HCNTIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0247<<2)))&BitMask_12)
#define VCNTIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0247<<2)))&BitMask_12)
#define HCNTI2_SVr				(ValSft_R16(_rd32(_REG_BASE_+(0x0248<<2)))&BitMask_12)
#define VCNTI2_SVr				(ValSft_R00(_rd32(_REG_BASE_+(0x0248<<2)))&BitMask_12)
#define HCNTI2r					(ValSft_R16(_rd32(_REG_BASE_+(0x0249<<2)))&BitMask_12)
#define VCNTI2r					(ValSft_R00(_rd32(_REG_BASE_+(0x0249<<2)))&BitMask_12)
#define WORD_CNTr				(ValSft_R00(_rd32(_REG_BASE_+(0x024b<<2)))&BitMask_24)
#define LS_FLGr					(ValSft_R31(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_01)
#define LS_FLG_POSTr			(ValSft_R30(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_01)
#define WDR_Gr					(ValSft_R16(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_12)
#define WDR_S1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_10)
#define WDR_S2r					(ValSft_R16(_rd32(_REG_BASE_+(0x0251<<2)))&BitMask_10)
#define WDR_SHr					(ValSft_R00(_rd32(_REG_BASE_+(0x0251<<2)))&BitMask_10)
#define WDR_SH_ADJr				(ValSft_R16(_rd32(_REG_BASE_+(0x0252<<2)))&BitMask_10)
#define WDR_SWGT_ADJr			(ValSft_R00(_rd32(_REG_BASE_+(0x0252<<2)))&BitMask_10)
#define WDR_SCr					(ValSft_R16(_rd32(_REG_BASE_+(0x0253<<2)))&BitMask_10)
#define WDR_SMr					(ValSft_R00(_rd32(_REG_BASE_+(0x0253<<2)))&BitMask_10)
#define WDR_SGF_SLOPEr			(ValSft_R00(_rd32(_REG_BASE_+(0x0254<<2)))&BitMask_08)
#define WDR_LY_SUMr				(ValSft_R00(_rd32(_REG_BASE_+(0x0255<<2)))&BitMask_31)
#define WDR_SY_SUMr				(ValSft_R00(_rd32(_REG_BASE_+(0x0256<<2)))&BitMask_31)
#define WDR_HIGH_CNTr			(ValSft_R00(_rd32(_REG_BASE_+(0x0257<<2)))&BitMask_21)
#define WDR_LOW_CNTr			(ValSft_R00(_rd32(_REG_BASE_+(0x0258<<2)))&BitMask_21)
#define WDR_SL_Rr				(ValSft_R00(_rd32(_REG_BASE_+(0x0259<<2)))&BitMask_12)
#define WDR_G_INVr				(ValSft_R00(_rd32(_REG_BASE_+(0x025a<<2)))&BitMask_18)
#define DF_COUNTr				(ValSft_R00(_rd32(_REG_BASE_+(0x025b<<2)))&BitMask_10)
#define SHD_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x025c<<2)))&BitMask_10)
#define IMD_MD000r				(ValSft_R00(_rd32(_REG_BASE_+(0x0260<<2)))&BitMask_32)
#define IMD_MD001r				(ValSft_R00(_rd32(_REG_BASE_+(0x0261<<2)))&BitMask_32)
#define IMD_MD002r				(ValSft_R00(_rd32(_REG_BASE_+(0x0262<<2)))&BitMask_32)
#define IMD_MD003r				(ValSft_R00(_rd32(_REG_BASE_+(0x0263<<2)))&BitMask_32)
#define IMD_MD004r				(ValSft_R00(_rd32(_REG_BASE_+(0x0264<<2)))&BitMask_32)
#define IMD_MD005r				(ValSft_R00(_rd32(_REG_BASE_+(0x0265<<2)))&BitMask_32)
#define IMD_MD006r				(ValSft_R00(_rd32(_REG_BASE_+(0x0266<<2)))&BitMask_32)
#define IMD_MD007r				(ValSft_R00(_rd32(_REG_BASE_+(0x0267<<2)))&BitMask_32)
#define IMD_MD008r				(ValSft_R00(_rd32(_REG_BASE_+(0x0268<<2)))&BitMask_32)
#define IMD_MD009r				(ValSft_R00(_rd32(_REG_BASE_+(0x0269<<2)))&BitMask_32)
#define IMD_MD010r				(ValSft_R00(_rd32(_REG_BASE_+(0x026a<<2)))&BitMask_32)
#define IMD_MD011r				(ValSft_R00(_rd32(_REG_BASE_+(0x026b<<2)))&BitMask_32)
#define IMD_MD012r				(ValSft_R00(_rd32(_REG_BASE_+(0x026c<<2)))&BitMask_32)
#define IMD_MD013r				(ValSft_R00(_rd32(_REG_BASE_+(0x026d<<2)))&BitMask_32)
#define IMD_MD014r				(ValSft_R00(_rd32(_REG_BASE_+(0x026e<<2)))&BitMask_32)
#define IMD_MD015r				(ValSft_R00(_rd32(_REG_BASE_+(0x026f<<2)))&BitMask_32)
#define IMD_MD016r				(ValSft_R00(_rd32(_REG_BASE_+(0x0270<<2)))&BitMask_32)
#define IMD_MD017r				(ValSft_R00(_rd32(_REG_BASE_+(0x0271<<2)))&BitMask_32)
#define IMD_MD018r				(ValSft_R00(_rd32(_REG_BASE_+(0x0272<<2)))&BitMask_32)
#define IMD_MD019r				(ValSft_R00(_rd32(_REG_BASE_+(0x0273<<2)))&BitMask_32)
#define IMD_MD020r				(ValSft_R00(_rd32(_REG_BASE_+(0x0274<<2)))&BitMask_32)
#define IMD_MD021r				(ValSft_R00(_rd32(_REG_BASE_+(0x0275<<2)))&BitMask_32)
#define IMD_MD022r				(ValSft_R00(_rd32(_REG_BASE_+(0x0276<<2)))&BitMask_32)
#define IMD_MD023r				(ValSft_R00(_rd32(_REG_BASE_+(0x0277<<2)))&BitMask_32)
#define IMD_MD024r				(ValSft_R00(_rd32(_REG_BASE_+(0x0278<<2)))&BitMask_32)
#define IMD_MD025r				(ValSft_R00(_rd32(_REG_BASE_+(0x0279<<2)))&BitMask_32)
#define IMD_MD026r				(ValSft_R00(_rd32(_REG_BASE_+(0x027a<<2)))&BitMask_32)
#define IMD_MD027r				(ValSft_R00(_rd32(_REG_BASE_+(0x027b<<2)))&BitMask_32)
#define IMD_MD028r				(ValSft_R00(_rd32(_REG_BASE_+(0x027c<<2)))&BitMask_32)
#define IMD_MD029r				(ValSft_R00(_rd32(_REG_BASE_+(0x027d<<2)))&BitMask_32)
#define IMD_MD030r				(ValSft_R00(_rd32(_REG_BASE_+(0x027e<<2)))&BitMask_32)
#define IMD_MD031r				(ValSft_R00(_rd32(_REG_BASE_+(0x027f<<2)))&BitMask_32)
#define IMD_MD100r				(ValSft_R00(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_32)
#define IMD_MD101r				(ValSft_R00(_rd32(_REG_BASE_+(0x0281<<2)))&BitMask_32)
#define IMD_MD102r				(ValSft_R00(_rd32(_REG_BASE_+(0x0282<<2)))&BitMask_32)
#define IMD_MD103r				(ValSft_R00(_rd32(_REG_BASE_+(0x0283<<2)))&BitMask_32)
#define IMD_MD104r				(ValSft_R00(_rd32(_REG_BASE_+(0x0284<<2)))&BitMask_32)
#define IMD_MD105r				(ValSft_R00(_rd32(_REG_BASE_+(0x0285<<2)))&BitMask_32)
#define IMD_MD106r				(ValSft_R00(_rd32(_REG_BASE_+(0x0286<<2)))&BitMask_32)
#define IMD_MD107r				(ValSft_R00(_rd32(_REG_BASE_+(0x0287<<2)))&BitMask_32)
#define IMD_MD108r				(ValSft_R00(_rd32(_REG_BASE_+(0x0288<<2)))&BitMask_32)
#define IMD_MD109r				(ValSft_R00(_rd32(_REG_BASE_+(0x0289<<2)))&BitMask_32)
#define IMD_MD110r				(ValSft_R00(_rd32(_REG_BASE_+(0x028a<<2)))&BitMask_32)
#define IMD_MD111r				(ValSft_R00(_rd32(_REG_BASE_+(0x028b<<2)))&BitMask_32)
#define IMD_MD112r				(ValSft_R00(_rd32(_REG_BASE_+(0x028c<<2)))&BitMask_32)
#define IMD_MD113r				(ValSft_R00(_rd32(_REG_BASE_+(0x028d<<2)))&BitMask_32)
#define IMD_MD114r				(ValSft_R00(_rd32(_REG_BASE_+(0x028e<<2)))&BitMask_32)
#define IMD_MD115r				(ValSft_R00(_rd32(_REG_BASE_+(0x028f<<2)))&BitMask_32)
#define IMD_MD116r				(ValSft_R00(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_32)
#define IMD_MD117r				(ValSft_R00(_rd32(_REG_BASE_+(0x0291<<2)))&BitMask_32)
#define IMD_MD118r				(ValSft_R00(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_32)
#define IMD_MD119r				(ValSft_R00(_rd32(_REG_BASE_+(0x0293<<2)))&BitMask_32)
#define IMD_MD120r				(ValSft_R00(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_32)
#define IMD_MD121r				(ValSft_R00(_rd32(_REG_BASE_+(0x0295<<2)))&BitMask_32)
#define IMD_MD122r				(ValSft_R00(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_32)
#define IMD_MD123r				(ValSft_R00(_rd32(_REG_BASE_+(0x0297<<2)))&BitMask_32)
#define IMD_MD124r				(ValSft_R00(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_32)
#define IMD_MD125r				(ValSft_R00(_rd32(_REG_BASE_+(0x0299<<2)))&BitMask_32)
#define IMD_MD126r				(ValSft_R00(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_32)
#define IMD_MD127r				(ValSft_R00(_rd32(_REG_BASE_+(0x029b<<2)))&BitMask_32)
#define IMD_MD128r				(ValSft_R00(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_32)
#define IMD_MD129r				(ValSft_R00(_rd32(_REG_BASE_+(0x029d<<2)))&BitMask_32)
#define IMD_MD130r				(ValSft_R00(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_32)
#define IMD_MD131r				(ValSft_R00(_rd32(_REG_BASE_+(0x029f<<2)))&BitMask_32)
#define JPG_OVFr				(ValSft_R31(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_01)
#define JPG_BIGr				(ValSft_R30(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_01)
#define JPG_FSIZEr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_28)
#define JPG_FSIZE_HOLDr			(ValSft_R00(_rd32(_REG_BASE_+(0x02a1<<2)))&BitMask_28)
#define JPG_LENr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a2<<2)))&BitMask_28)
#define ISP_RIRQ0r				(ValSft_R00(_rd32(_REG_BASE_+(0x02ae<<2)))&BitMask_08)
#define ISP_RIRQ1r				(ValSft_R00(_rd32(_REG_BASE_+(0x02af<<2)))&BitMask_08)





///////////////////////////////////////////////
// Add for Naming Rule
///////////////////////////////////////////////
// ADC
#define ADC0_RDATr				(ADC_CH0_DAT&0x3FF)
#define ADC1_RDATr				(ADC_CH1_DAT&0x3FF)
#define ADC2_RDATr				(ADC_CH2_DAT&0x3FF)

// Clock Power Down Mode
#define PCKO_PDw      			PCKO_OENw	// PCKO_SELw
#define PCKO0_PDw 				PCKO0_ONw	// PCKO0_SELw
#define PCKO1_PDw 				PCKO1_ONw	// PCKO1_SELw
#define DCKO0_PDw      			DCKO0_ONw	// DCKO0_SELw

#define ISPCLK_SEL(TITLE,SEL)	TITLE##_PDw(0);	\
                            	TITLE##_SELw(SEL); \
                            	TITLE##_PDw(1);


/* EOF */

