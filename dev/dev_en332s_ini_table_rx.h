// ==================================================================================
// No input (1080@30p test pattern out)
// ==================================================================================
#define TP_VD_Size	20
const UINT TP_VD[2][TP_VD_Size][3] = {
{
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	                                      
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000000, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000040, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
//	{ 0x000000c5, 0x00000040, 0x00000070},	// RPCK_A select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00000000, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00004000, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
	                                      
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	                                      
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x00000000, 0x00010100}	// EXD_A, EXM_A Enable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x0001ffff},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000000, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000040, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000004, 0x00000007},	// Internal clock selection
	{ 0x000000cb, 0x00000000, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00001000, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// No input (1440@30p(4M) test pattern out)
// ==================================================================================
//#define TP_VD_Size	23
//const UINT TP_VD[2][TP_VD_Size][3] = {
//{
//	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable      
//	{ 0x000000ca, 0x00000000, 0x0001ffff},	// EXD_A, EXM_A Disable
//	
//	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
//	{ 0x00000041, 0x00000000, 0x0000000f},	// In-buffer out data select
//	{ 0x0000004a, 0x00000040, 0x00000040},	// Rx data select
//	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
//	{ 0x000000c5, 0x00000030, 0x00000070},	// RPCK_A select                  
//	{ 0x000000ca, 0x00000000, 0x00007cff},	// EXD, EXM clock select
//	{ 0x000000d1, 0x00004000, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select  
//	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
//	{ 0x000000e4, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
//	
//	{ 0x000000e1, 0x00000ce3, 0x00007fff},	// Horizontal total                     
//	{ 0x000000e2, 0x05db0a00, 0x7fff7fff},	// Vertical total, Horizontal active    
//	{ 0x000000e3, 0x05a00030, 0x7fff3fff},	// Vertical active, Vertical start point
//	
//	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
//	{ 0x00000112, 0x20000000, 0xf0800000},	// EX-SDI Version & Bit Mode select
//	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
//	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
//	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
//	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
//	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
//	
//	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
//	{ 0x000000ca, 0x00000000, 0x00010100}	// EXD_A, EXM_A Enable
//},
//{
//	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
//	{ 0x000000cb, 0x00000000, 0x0001ffff},	// EXD_B, EXM_B Disable
//	
//	{ 0x00000051, 0x00000000, 0x0000000f},	// In-buffer out data select
//	{ 0x0000005a, 0x00000040, 0x00000040},	// Rx data select
//	{ 0x000000c5, 0x00000003, 0x00000007},	// Internal clock selection
//	{ 0x000000cb, 0x00000000, 0x00007eff},	// EXD, EXM clock select
//	{ 0x000000d1, 0x00001000, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
//	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
//	{ 0x000000eb, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
//	
//	{ 0x000000e8, 0x00000ce3, 0x00007fff},	// Horizontal total                     
//	{ 0x000000e9, 0x05db0a00, 0x7fff7fff},	// Vertical total, Horizontal active    
//	{ 0x000000ea, 0x05a00030, 0x7fff3fff},	// Vertical active, Vertical start point
//	
//	{ 0x00000132, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
//	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
//	{ 0x0000014a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
//	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
//	
//	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
//	{ 0x000000cb, 0x00000000, 0x00008100}	// EXD_B, EXM_B Disable
//}
//};

// ==================================================================================
// No input (2160@30p(4K) test pattern out)
// ==================================================================================
//#define TP_VD_Size	23
//const UINT TP_VD[2][TP_VD_Size][3] = {
//{
//	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
//	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
//	                                      
//	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
//	{ 0x00000041, 0x00000000, 0x0000000f},	// In-buffer out data select
//	{ 0x0000004a, 0x00000040, 0x00000040},	// Rx data select
//	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
//	{ 0x000000c5, 0x00000020, 0x00000070},	// RPCK_A select
//	{ 0x000000ca, 0x00000000, 0x00007cff},	// EXD, EXM clock select
//	{ 0x000000d1, 0x00004000, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
//	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
//	{ 0x000000e4, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
//	
//	{ 0x000000e1, 0x0000112f, 0x00007fff},	// Horizontal total
//	{ 0x000000e2, 0x08c90f00, 0x7fff7fff},	// Vertical total, Horizontal active
//	{ 0x000000e3, 0x0870005a, 0x7fff3fff},	// Vertical active, Vertical start point
//	                                      
//	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
//	{ 0x00000112, 0x20000000, 0xf0800000},	// EX-SDI Version & Bit Mode select
//	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
//	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
//	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
//	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
//	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
//	                                      
//	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
//	{ 0x000000ca, 0x00000000, 0x00010100}	// EXD_A, EXM_A Enable
//}
//};

// ==================================================================================
// HD-SDI (1.485Gbps)
// ==================================================================================
#define RX_HDSDI_Size	20
const UINT RX_HDSDI[2][RX_HDSDI_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000000, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00000000, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000000, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x00000000, 0x00010100}	// EXD_A, EXM_A Disable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000000, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000000, 0x00000040},	// Internal data MUX selection
	{ 0x000000c5, 0x00000001, 0x00000007},	// Internal clock selection
	{ 0x000000cb, 0x00000000, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000000, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x00000000, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI 1.0 (270Mbps)
// ==================================================================================
#define RX_EX270_HD_Size	20
const UINT RX_EX270_HD[2][RX_EX270_HD_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	                                      
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000002, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00003452, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	                                      
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x00000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	                                      
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x00010100, 0x00010100}	// EXD_A, EXM_A Enable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000002, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000000, 0x00000040},	// Internal data MUX selection
	{ 0x000000c5, 0x00000001, 0x00000007},	// Internal clock selection
	{ 0x000000cb, 0x00003252, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000002, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x00000000, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x00000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B Enable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI 2.0 (135Mbps)
// ==================================================================================
#define RX_EX135_HD_Size	20
const UINT RX_EX135_HD[2][RX_EX135_HD_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000006, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00003050, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20800000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x00010100, 0x00010100}	// EXD_A, EXM_A Enable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000006, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000001, 0x00000007},	// RPCK_B select
	{ 0x000000cb, 0x00003050, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000002, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x00000000, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20800000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B Enable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI 3G/4M (270Mbps)
// ==================================================================================
#define RX_EX270_3G_4M_Size	20
const UINT RX_EX270_3G_4M[2][RX_EX270_3G_4M_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080}, 	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000006, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000000, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00002040, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20800000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8b0c0e0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x110b0c14, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000080, 0x00000080}, 	// RPCK_A enable
	{ 0x000000ca, 0x00010100, 0x00010100}	// EXD_A, EXM_A Enable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008}, 	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000006, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000000, 0x00000007},	// RPCK_B select
	{ 0x000000cb, 0x00002040, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000002, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x00000000, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20800000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8b0c0e0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x110b0c14, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B Enable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI 4K (1.485Gbps)
// ==================================================================================
#define RX_EX1G5_4K_Size	20
const UINT RX_EX1G5_4K[2][RX_EX1G5_4K_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x10010100},	// EXD_A, EXM_A Disable
	                                        
	{ 0x00000031, 0x000000c0, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000003, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x00000001, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00008422, 0x0000fcff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	                                        
	{ 0x00000111, 0x00002000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20020000, 0xf0830040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00110000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x08000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x00000009, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x0607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	                                        
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x10010100, 0x10010100}	// EXD_A, EXM_A Enable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable

	{ 0x00000051, 0x00000000, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000040, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000001, 0x00000007},	// Internal clock selection
	{ 0x000000cb, 0x00000626, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000000, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x010020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00006000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20020000, 0xf0830040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00110000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x00000009, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x0607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B disable
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B enable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI TDM (1.485Gbps, RxPHY)
// ==================================================================================
#define RX_EX1G5_TDM_RX_Size	33
const UINT RX_EX1G5_TDM_RX[2][RX_EX1G5_TDM_RX_Size][3] = {
{	
	// EXTDM Init
	{ 0x000000cc, 0x00000000, 0x00001000},	// EXTDM Disable
	{ 0x000000c3, 0x00000000, 0x00000080},	// EXTDM IC Disable
	{ 0x000000cc, 0x00000005, 0x00000015},	// EXTDM clock select
	{ 0x000001a1, 0x80000000, 0x80000000},	// EX-TDM On
	{ 0x000001a2, 0x00200001, 0x00300001},	// EX-TDM FMT 10bit do
	{ 0x00000031, 0x000000c0, 0x000000c0},	// TX PLL Rate Sel
	{ 0x000000c3, 0x000004a1, 0x000007bf},	// TX PLL clock select
	{ 0x000000d4, 0x00000000, 0x00000100},	// EXTDM data input select
	{ 0x000000de, 0x00000000, 0x00000600},	// ICO pin direction output
	{ 0x00000041, 0x00000005, 0x0000000f},	// In-buffer out data select
	{ 0x000000cc, 0x00001000, 0x00001000},	// EXTDM Enable
	{ 0x000000c3, 0x00000080, 0x00000080},	// EXTDM IC Enable
		
	// EXTDM CH.A Init
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000c8, 0x00000000, 0x80000000},	// VD_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000c8, 0x04430000, 0x0fef0000},	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00001030, 0x00007cf1},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003000, 0x00003000},	// Internal data MUX selection
//	{ 0x000000d3, 0x00000010, 0x000000f0},	// EXD_VDI_A_SEL (Output Channel Select)
	{ 0x00000112, 0x20800040, 0xf0800040},	// EX-SDI Version & Bit Mode select & DSD DEFMT bypass
	{ 0x00000117, 0x04420000, 0x0fff0000},	// DSD WFREQ, RFREQ setting
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000c8, 0x80000000, 0x80000000},	// PCK_A enable
	{ 0x000000ca, 0x00010100, 0x00010100},	// EXD_A, EXM_A Enable
	
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Disable
	{ 0x000000c6, 0x00000000, 0x00730000},	// MUX_PCK_A, MUX_DPCK_A select 
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Enable 
	
	{ 0x000000de, 0x00000000, 0x0000c000}	// VD_A output
},
{	
	// EXTDM CH.B Init
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000c8, 0x00000000, 0x00008000},	// VD_B Disable 
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable

	{ 0x0000005a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000001, 0x00000007},	// RPCK_B select
	{ 0x000000c8, 0x00000443, 0x00000fef},	// PCK_B, Internal clock selection
	{ 0x000000cb, 0x00001434, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000002, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d2, 0x00000020, 0x00000030},	// Internal data MUX selection
//	{ 0x000000d3, 0x00000002, 0x0000000f},	// EXD_VDI_B_SEL (Output Channel Select)
	{ 0x00000132, 0x20800040, 0xf0800040},	// EX-SDI Version & Bit Mode select & DSD DEFMT bypass
	{ 0x00000137, 0x04420000, 0x0fff0000},	// DSD WFREQ, RFREQ setting
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000c8, 0x00008000, 0x00008000},	// PCK_B enable 
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B Enable
	
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Disable
	{ 0x000000c6, 0x00000000, 0x00007300},	// MUX_PCK_B, MUX_DPCK_B select 
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Enable
	
	{ 0x000000de, 0x00000000, 0x00003800},	// VD_B output
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// EX-SDI TDM (1.485Gbps, ICI)
// ==================================================================================
#define RX_EX1G5_TDM_IC_Size	33
const UINT RX_EX1G5_TDM_IC[2][RX_EX1G5_TDM_IC_Size][3] = {
{	
	// EXTDM Init
	{ 0x000000cc, 0x00000000, 0x00001000},	// EXTDM Disable
	{ 0x000000c3, 0x00000000, 0x00000080},	// EXTDM IC Disable
	{ 0x000000cc, 0x00000015, 0x00000015},	// EXTDM clock select
	{ 0x000001a1, 0x80000000, 0x80000000},	// EX-TDM On
	{ 0x000001a2, 0x00200001, 0x00300001},	// EX-TDM FMT 10bit do
	{ 0x00000031, 0x00000000, 0x000000c0},	// TX PLL Rate Sel
	{ 0x000000c3, 0x000004aa, 0x000007bf},	// TX PLL clock select
	{ 0x000000d4, 0x00000100, 0x00000100},	// EXTDM data input select
	{ 0x000000de, 0x00000000, 0x00000600},	// ICO pin direction output
	{ 0x00000041, 0x00000005, 0x0000000f},	// In-buffer out data select
	{ 0x000000cc, 0x00001000, 0x00001000},	// EXTDM Enable
	{ 0x000000c3, 0x00000080, 0x00000080},	// EXTDM IC Enable
	
	// EXTDM CH.A Init
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000c8, 0x00000000, 0x80000000},	// VD_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000010, 0x00000070},	// RPCK_A select
	{ 0x000000c8, 0x09a70000, 0x0fef0000},	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00006cb6, 0x00007df1},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000008, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003000, 0x00003000},	// Internal data MUX selection
//	{ 0x000000d3, 0x00000030, 0x000000f0},	// EXD_VDI_A_SEL (Output Channel Select)
	{ 0x00000112, 0x20800040, 0xf0800040},	// EX-SDI Version & Bit Mode select & DSD DEFMT bypass
	{ 0x00000117, 0x04420000, 0x0fff0000},	// DSD WFREQ, RFREQ setting
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A enable
	{ 0x000000c8, 0x80000000, 0x80000000},	// PCK_A enable
	{ 0x000000ca, 0x00010100, 0x00010100},	// EXD_A, EXM_A Enable
	
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Disable
	{ 0x000000c6, 0x00000000, 0x00730000},	// MUX_PCK_A, MUX_DPCK_A select 
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Enable 
	
	{ 0x000000de, 0x00000000, 0x0000c000}	// VD_A output
},
{	
	// EXTDM CH.B Init
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000c8, 0x00000000, 0x00008000},	// VD_B Disable 
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable

	{ 0x0000005a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c5, 0x00000001, 0x00000007},	// RPCK_B select
	{ 0x000000c8, 0x000009a7, 0x00000fef},	// PCK_B, Internal clock selection
	{ 0x000000cb, 0x00006a8a, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000002, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d2, 0x00000020, 0x00000030},	// Internal data MUX selection
//	{ 0x000000d3, 0x00000004, 0x0000000f},	// EXD_VDI_B_SEL (Output Channel Select)
	{ 0x00000132, 0x20800040, 0xf0800040},	// EX-SDI Version & Bit Mode select & DSD DEFMT bypass
	{ 0x00000137, 0x04420000, 0x0fff0000},	// DSD WFREQ, RFREQ setting
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8a0b0c0f, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x140b1217, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B enable
	{ 0x000000c8, 0x00008000, 0x00008000},	// PCK_B enable 
	{ 0x000000cb, 0x00008100, 0x00008100},	// EXD_B, EXM_B Enable
	
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Disable
	{ 0x000000c6, 0x00000000, 0x00007300},	// MUX_PCK_B, MUX_DPCK_B select 
	{ 0x000000c6, 0x00000000, 0x01000000},	// MUX Enable 
	
	{ 0x000000de, 0x00000000, 0x00003800},	// VD_B output
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

// ==================================================================================
// 3G-SDI (2.97Gbps)
// ==================================================================================
#define RX_3GSDI_Size	20
const UINT RX_3GSDI[2][RX_3GSDI_Size][3] = {
{	
	{ 0x000000c5, 0x00000000, 0x00000080},	// RPCK_A Disable
	{ 0x000000ca, 0x00000000, 0x00010100},	// EXD_A, EXM_A Disable
	
	{ 0x00000031, 0x00000000, 0x000000c0},	// TSDI_RATE select
	{ 0x00000041, 0x00000001, 0x0000000f},	// In-buffer out data select
	{ 0x0000004a, 0x00000000, 0x00000040},	// Rx data select
	{ 0x000000c3, 0x0000000a, 0x0000000f},	// TXREFCK clock select
	{ 0x000000c5, 0x00000000, 0x00000070},	// RPCK_A select
	{ 0x000000ca, 0x00000000, 0x00007cff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000000, 0x0000c0cc},	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d3, 0x00000000, 0x000000f0},	// EXD_VDI_A_SEL
	{ 0x000000e4, 0x000020c0, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000111, 0x00000000, 0x00002000},	// DSD_ID_MATCH
	{ 0x00000112, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000117, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x0000011d, 0x10000000, 0x18000000},	// EX-SDI Merge on
	{ 0x00000129, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000012a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000080, 0x00000080},	// RPCK_A enable
	{ 0x000000ca, 0x00000000, 0x00010100}	// EXD_A, EXM_A Disable
},
{
	{ 0x000000c5, 0x00000000, 0x00000008},	// RPCK_B Disable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x00000051, 0x00000001, 0x0000000f},	// In-buffer out data select
	{ 0x0000005a, 0x00000000, 0x00000040},	// Internal data MUX selection
	{ 0x000000c5, 0x00000000, 0x00000007},	// Internal clock selection
	{ 0x000000cb, 0x00000000, 0x00007eff},	// EXD, EXM clock select
	{ 0x000000d1, 0x00000000, 0x00003033},	// VDO_DI_B, MUX_DI_B Select
	{ 0x000000d3, 0x00000000, 0x0000000f},	// EXD_VDI_B_SEL
	{ 0x000000eb, 0x00000000, 0x0100ffff},	// Test Pattern Generation ON
	
	{ 0x00000131, 0x00000000, 0x0000e000},	// DSD_ID_MATCH
	{ 0x00000132, 0x20000000, 0xf0800040},	// EX-SDI Version & Bit Mode select
	{ 0x00000137, 0x00a50000, 0x0fff0000},	// EX-SDI WFREQ, RFREQ
	{ 0x00000149, 0x0000000a, 0x0000003f},	// EX-SDI Voffset
	{ 0x0000014a, 0x8607080b, 0xffffffff},	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a, 0xffffffff},	// EX-SDI Voffset
	
	{ 0x000000c5, 0x00000008, 0x00000008},	// RPCK_B enable
	{ 0x000000cb, 0x00000000, 0x00008100},	// EXD_B, EXM_B Disable
	
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000},	// Dummy Register
	{ 0x000000fa, 0x00000000, 0x00000000}	// Dummy Register
}
};

#ifdef DEF_CPU0

extern BYTE EN331_Write(WORD appWaddr, UINT anBuf);
extern BYTE EN331_Read(WORD awAddr, UINT* anBuf);

#define EN332T_HANDLING(NAME, Num, buf)		EN331_Read((WORD)NAME[0][Num][0], &buf);/* A Channel */\
											buf = (buf & (~NAME[0][Num][2]))|(NAME[0][Num][1] & (NAME[0][Num][2]));\
											EN331_Write((WORD)NAME[0][Num][0], buf);\
											EN331_Read((WORD)NAME[1][Num][0], &buf);/* B Channel */\
											buf = (buf & (~NAME[1][Num][2]))|(NAME[1][Num][1] & (NAME[1][Num][2]));\
											EN331_Write((WORD)NAME[1][Num][0], buf);

#define EN332T_HANDLING_AOnly(NAME, Num, buf)	EN331_Read((WORD)NAME[0][Num][0], &buf);/* A Channel */\
												buf = (buf & (~NAME[0][Num][2]))|(NAME[0][Num][1] & (NAME[0][Num][2]));\
												EN331_Write((WORD)NAME[0][Num][0], buf);
#endif

/* EOF */

