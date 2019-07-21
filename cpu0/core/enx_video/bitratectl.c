
#include "board.h"
#include "h264.h"
//#include "exr1k_asm.h"

#include "dev_isp.h"

#include "bitratectl.h"

#ifdef __H264_MCH__
#include "h264_mch.h"
#endif

#ifdef BITCTL_DEBUG
	#define dbtprintf1(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dbtprintf1(fmt, args...) do {} while(0);
#endif

#ifdef BITCTL_DEBUG2
	#define dbtprintf2(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dbtprintf2(fmt, args...) do {} while(0);
#endif

#ifdef BITCTL_DEBUG3
	#define dbtprintf3(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dbtprintf3(fmt, args...) do {} while(0);
#endif

#ifdef BITCTL_DEBUG4
	#define dbtprintf4(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dbtprintf4(fmt, args...) do {} while(0);
#endif


BitCtl_t mBitCtl[2];

float AF_RATIO_VAL[2][20];

UINT H264BPS[2];

#ifdef ISP_AE_CHECK
INT AEControlErr;
#endif

const float AF_RATIO[2][20] = {
	{1.0f, 1.05f, 1.1f, 1.15f, 1.2f, 1.25f,	1.3f, 1.35f, 1.4f, 1.45f, 
		2.0f, 2.1f, 2.15f, 2.2f, 3.25f, 4.3f, 5.35f, 6.4f, 7.45f, 8.5f},
	{1.0f, 1.05f, 1.1f, 1.15f, 1.2f, 1.25f, 1.3f, 1.35f, 1.4f, 1.45f, 
		2.0f, 2.1f, 2.15f, 2.2f, 3.25f, 4.3f, 5.35f, 6.4f, 7.45f, 8.5f}
	};
	
const UINT AF_INFO_LEVEL[20] = {
	20000, 40000, 60000, 80000, 100000,
	120000, 140000, 160000, 180000, 200000,
	300000, 400000, 500000, 600000, 700000,
	800000, 900000, 1000000, 2000000, 3000000 
	};

void bitctl_set_overbitsize(int ratio, int ch)
{
	mBitCtl[ch].nOverBitRatio = ratio;
}

void bitctl_set_iframe_maxsize(int ratio, int ch)
{
	mBitCtl[ch].nIfrmRatio = ratio;
}

void bitctl_set_pframe_maxsize(int ratio, int ch)
{
	mBitCtl[ch].nPfrmRatio = ratio;
}

void bitctl_set_previous_iframe_quant(int ch, int iQuantize)
{	
	mBitCtl[ch].nPreIfrmQuant = iQuantize;
}

int bitctl_get_previous_iframe_quant(int ch)
{	
	return mBitCtl[ch].nPreIfrmQuant;
}

void bitctl_set_previous_pframe_quant(int ch, int pQuantize)
{
	mBitCtl[ch].nPrePfrmQuant = pQuantize;
}

int bitctl_get_previous_pframe_quant(int ch)
{	
	return mBitCtl[ch].nPrePfrmQuant;
}

void bitctl_set_previous_iframe_byte(int ch, int iQuantize)
{	
	mBitCtl[ch].nPreIFrmBytes = iQuantize;
}

int bitctl_get_previous_iframe_byte(int ch)
{	
	return mBitCtl[ch].nPreIFrmBytes;
}

void bitctl_set_previous_pframe_byte(int ch, int pQuantize)
{	
	mBitCtl[ch].nPrePFrmBytes = pQuantize;
}

int bitctl_get_previous_pframe_byte(int ch)
{	
	return mBitCtl[ch].nPrePFrmBytes;
}

void bitctl_set_current_af_iframe(int ch, UINT af)
{	
	mBitCtl[ch].nCurAfInfoIfrm = af;
}

UINT bitctl_get_current_af_iframe(int ch)
{	
	return mBitCtl[ch].nCurAfInfoIfrm;
}

void bitctl_set_current_af_pframe(int ch, UINT af)
{	
	mBitCtl[ch].nCurAfInfoPfrm = af;
}

UINT bitctl_get_current_af_pframe(int ch)
{	
	return mBitCtl[ch].nCurAfInfoPfrm;
}

void bitctl_set_previous_af_iframe(int ch, UINT af)
{	
	mBitCtl[ch].nPreAfInfoIfrm = af;
}

UINT bitctl_get_previous_af_iframe(int ch)
{	
	return mBitCtl[ch].nPreAfInfoIfrm;
}

void bitctl_set_previous_af_pframe(int ch, UINT af)
{	
	mBitCtl[ch].nPreAfInfoPfrm = af;
}

UINT bitctl_get_previous_af_pframe(int ch)
{	
	return mBitCtl[ch].nPreAfInfoPfrm;
}

void bitctl_set_current_ae(int ch, int ae)
{
	mBitCtl[ch].nCurAeInfo = ae;
}

int bitctl_get_current_ae(int ch)
{
	return mBitCtl[ch].nCurAeInfo;
}

void bitctl_set_previous_ae(int ch, int ae)
{
	mBitCtl[ch].nPreAeInfo = ae;
}

int bitctl_get_previous_ae(int ch)
{
	return mBitCtl[ch].nPreAeInfo;
}

void bitctl_set_ae_min_diff(int ch, int min)
{
	mBitCtl[ch].nMinAeDiff = min;
}

int bitctl_get_ae_min_diff(int ch)
{
	return mBitCtl[ch].nMinAeDiff;
}

void bitctl_set_iframe_quantize(int ch, int iQuantize)
{
	mBitCtl[ch].nCurIfrmQuant = iQuantize;
}

int bitctl_get_iframe_quantize(int ch)
{
	return mBitCtl[ch].nCurIfrmQuant;
}

void bitctl_set_pframe_quantize(int ch, int pQuantize)
{
	mBitCtl[ch].nCurPfrmQuant = pQuantize;
}

int bitctl_get_pframe_quantize(int ch)
{
	return mBitCtl[ch].nCurPfrmQuant;
}

void bitctl_set_iframe_target_bit(int ch, int byte)
{
	mBitCtl[ch].nIfrmTgByte = byte;
}

int bitctl_get_iframe_target_bit(int ch)
{
	return mBitCtl[ch].nIfrmTgByte;
}

void bitctl_set_pframe_target_bit(int ch, int byte)
{
	mBitCtl[ch].nPfrmTgByte = byte;
}

int bitctl_get_pframe_target_bit(int ch)
{
	return mBitCtl[ch].nPfrmTgByte;
}

void bitctl_set_min_bit_iframe(int ch, int bit)
{
	mBitCtl[ch].nIfrmMinBitSz = bit;
}

int bitctl_get_min_bit_iframe(int ch)
{
	return mBitCtl[ch].nIfrmMinBitSz;
}

void bitctl_set_min_bit_pframe(int ch, int bit)
{
	mBitCtl[ch].nPfrmMinBitSz = bit;
}

int bitctl_get_min_bit_pframe(int ch)
{
	return mBitCtl[ch].nPfrmMinBitSz;
}

int bitctl_get_previous_byte(int ch)
{
	return mBitCtl[ch].nPreFrmBytes;
}

int bitctl_get_warning(int ch, int iframe)
{
	if(iframe == 0)//i frame
		return mBitCtl[ch].nIfrmWarning;
	else
		return mBitCtl[ch].nPfrmWarning;
}

int bitctl_get_warning_size(int ch, int iframe)
{
	if(iframe == 0)//i frame
		return mBitCtl[ch].nIfrmWarningSize;
	else
		return mBitCtl[ch].nPfrmWarningSize;
}

void bitctl_check_warning(int ch)
{

	int iframe,bytes;
	
	iframe = enx_h264_get_slice_type(ch);

	bytes = (int)enx_h264_get_prebits();

	
	if(iframe == 0)//i frame
	{
		if( bytes > enx_h264_get_stp_size(ch) )
		{
			mBitCtl[ch].nIfrmWarning = OVERWRITTEN;
			mBitCtl[ch].nIfrmWarningSize = bytes; 
		}
		if( bytes > ((enx_h264_get_stp_size(ch)*2)/3) )
		{
			mBitCtl[ch].nIfrmWarning = WARNING1;
			mBitCtl[ch].nIfrmWarningSize = bytes; 
		}
		else if( bytes > (enx_h264_get_stp_size(ch)/2) )
		{
			mBitCtl[ch].nIfrmWarning = WARNING2;
			mBitCtl[ch].nIfrmWarningSize = bytes; 
		}
		else
		{
			mBitCtl[ch].nIfrmWarning = NONE;
			mBitCtl[ch].nIfrmWarningSize = 0;
		}
	}
	else
	{
		if( bytes > enx_h264_get_stp_size(ch) )
		{
			mBitCtl[ch].nPfrmWarning = OVERWRITTEN;
			mBitCtl[ch].nPfrmWarningSize = bytes; 
		}
		if( bytes > ((enx_h264_get_stp_size(ch)*2)/3) )
		{
			mBitCtl[ch].nPfrmWarning = WARNING1;
			mBitCtl[ch].nPfrmWarningSize = bytes; 
		}
		else if( bytes > (enx_h264_get_stp_size(ch)/2) )
		{
			mBitCtl[ch].nPfrmWarning = WARNING2;
			mBitCtl[ch].nPfrmWarningSize = bytes; 
		}	
		else
		{
			mBitCtl[ch].nPfrmWarning = NONE;
			mBitCtl[ch].nPfrmWarningSize = 0;
		}
	}
}

int bitctl_get_ae_level (int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int nAeErrLevel = 0;
	
	int diffAEerr = 0;

	diffAEerr = ABSDIFF(AEControlErr, bitctl_get_previous_ae(ch));	


#ifdef ISP_AE_CHECK // ISP information check

	if(diffAEerr > bitctl_get_ae_min_diff(ch)){
		
		if(diffAEerr > 1000){
				nAeErrLevel = 7;		
		}else if(diffAEerr > 800){
				nAeErrLevel = 6;
		}else if(diffAEerr > 600){
				nAeErrLevel = 5;
		}else if(diffAEerr > 400){
				nAeErrLevel = 4;
		}else if(diffAEerr > 200){
				nAeErrLevel = 3;
		}else if(diffAEerr > 100) {
				nAeErrLevel = 2;
		}else if(diffAEerr > 1){
				nAeErrLevel = 1;
		}
	}

	//debug osd print
	if(gbMnDebugFnc == 1)
	{
		if(diffAEerr!=0){
			char buffer[100];
			int buf_len = sprintf(buffer, "ae diff__ %10d", diffAEerr);
			DispStr(buffer, 18, 24, buf_len);
		}	
		else
		{
			char buffer[100];
			int buf_len = sprintf(buffer, "ae stay__  %10d", bitctl_get_current_ae(ch));
			DispStr(buffer, 18, 24, buf_len);
		}
	}
	
	dbtprintf4("%dch current_ae %d previous_ae %d diff %d \r\n", ch, bitctl_get_current_ae(ch), bitctl_get_previous_ae(ch), diffAEerr); 
	
	bitctl_set_previous_ae(ch, bitctl_get_current_ae(ch));
	bitctl_set_current_ae(ch, AEControlErr);
	dbtprintf4("%dch AEControlErr %d\r\n", ch, AEControlErr); 
#endif    
	return nAeErrLevel;
}

UINT bitctl_get_af(void)
{
	//printf("AF_SUM2_LOCKr %x\r\n",AF_SUM2_LOCKr);
	//printf("AF_YSUM2_LOCKr %x\r\n",AF_YSUM2_LOCKr);
	//printf("AF_SUM2_LOCKr /( AF_YSUM2_LOCKr >>0x14) %x\r\n",AF_SUM2_LOCKr /( AF_YSUM2_LOCKr >>0x14));
	
	return (AF_SUM2_LOCKr /( AF_YSUM2_LOCKr >>0x14));
}

void bitctl_set_previous_af_level_iframe(int ch, int index)
{
	mBitCtl[ch].nPreAfLevelIfrm = index;
}

void bitctl_set_previous_af_level_pframe(int ch, int index)
{
	mBitCtl[ch].nPreAfLevelPfrm = index;
}

int bitctl_get_af_change_iframe(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int nAfUpdown;
	UINT nAfInfo;
	UINT nPreAfInfo;
	UINT nAfInfoDiff;
	
	nAfUpdown = AFSTAY;
	nAfInfoDiff = 0;
	
	nAfInfo = bitctl_get_current_af_iframe(ch);//bitctl_get_af();
	dbtprintf4("I nAfInfo %d\r\n",nAfInfo);

	if(nAfInfo > 0)
	{
		nPreAfInfo = bitctl_get_previous_af_iframe(ch);
		dbtprintf4("I nPreAfInfo %d\r\n",nPreAfInfo);
		
		if(nAfInfo > nPreAfInfo){
			// AF 정보 증가
			nAfInfoDiff = nAfInfo - nPreAfInfo;
			
			if(nAfInfoDiff > AF_DIFF_MIN_10){
				nAfUpdown = 11;
			}else if(nAfInfoDiff > AF_DIFF_MIN_9){
				nAfUpdown = 10;
			}else if(nAfInfoDiff > AF_DIFF_MIN_8){
				nAfUpdown = 9;
			}else if(nAfInfoDiff > AF_DIFF_MIN_7){
				nAfUpdown = 8;
			}else if(nAfInfoDiff > AF_DIFF_MIN_6){
				nAfUpdown = 7;
			}else if(nAfInfoDiff > AF_DIFF_MIN_5){
				nAfUpdown = 6;
			}else if(nAfInfoDiff > AF_DIFF_MIN_4){
				nAfUpdown = 5;
			}else if(nAfInfoDiff > AF_DIFF_MIN_3){
				nAfUpdown = 4;
			}else if(nAfInfoDiff > AF_DIFF_MIN_2){
				nAfUpdown = 3;
			}else if(nAfInfoDiff > AF_DIFF_MIN_1){
				nAfUpdown = 2;
			}else if(nAfInfoDiff > AF_DIFF_MIN_0){
				nAfUpdown = AFUP;
			}else {		
				nAfUpdown = AFSTAY;
			}
			
		}
		else if(nAfInfo < nPreAfInfo){
			
			//AF 정보 감소					
			nAfInfoDiff = nPreAfInfo - nAfInfo;

			if(nAfInfoDiff > AF_DIFF_MIN_10){
				nAfUpdown = -11;
			}else if(nAfInfoDiff > AF_DIFF_MIN_9){
				nAfUpdown = -10;
			}else if(nAfInfoDiff > AF_DIFF_MIN_8){
				nAfUpdown = -9;
			}else if(nAfInfoDiff > AF_DIFF_MIN_7){
				nAfUpdown = -8;
			}else if(nAfInfoDiff > AF_DIFF_MIN_6){
				nAfUpdown = -7;
			}else if(nAfInfoDiff > AF_DIFF_MIN_5){
				nAfUpdown = -6;
			}else if(nAfInfoDiff > AF_DIFF_MIN_4){
				nAfUpdown = -5;
			}else if(nAfInfoDiff > AF_DIFF_MIN_3){
				nAfUpdown = -4;
			}else if(nAfInfoDiff > AF_DIFF_MIN_2){
				nAfUpdown = -3;
			}else if(nAfInfoDiff > AF_DIFF_MIN_1){
				nAfUpdown = -2;
			}else if(nAfInfoDiff > AF_DIFF_MIN_0){
				nAfUpdown = AFDOWN;
			}else {		
				nAfUpdown = AFSTAY;
			}
			
		}
		if(gbMnDebugFnc == 1)
		{
			if(nAfUpdown >= AFUP){
				char buffer[100];
				int buf_len = sprintf(buffer, "af i up__ +%10d", nAfInfoDiff);
				DispStr(buffer, 19, 24, buf_len);
			}
			if(nAfUpdown == AFSTAY){
				char buffer[100];
				int buf_len = sprintf(buffer, "af i stay  %10d", bitctl_get_current_af_iframe(ch));
				DispStr(buffer, 19, 24, buf_len);
			}
			if(nAfUpdown <= AFDOWN){
				char buffer[100];
				int buf_len = sprintf(buffer, "af i down -%10d", nAfInfoDiff);
				DispStr(buffer, 19, 24, buf_len);
			}
		}
		dbtprintf4("nAfUpdown %d\r\n",nAfUpdown);
	}
	
		bitctl_set_previous_af_iframe(ch, nAfInfo);
		
		bitctl_set_current_af_pframe(ch, bitctl_get_af());
		bitctl_set_current_af_iframe(ch, bitctl_get_af());
	return nAfUpdown;
}

int bitctl_get_af_change_pframe(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int nAfUpdown;
	UINT nAfInfo;
	UINT nPreAfInfo;
	UINT nAfInfoDiff;
	int nAfLevel;	
	
	nAfUpdown = AFSTAY;
	
	nAfInfo = bitctl_get_current_af_pframe(ch);//bitctl_get_af();
	dbtprintf4("p nAfInfo %d\r\n",nAfInfo);
	
	if(nAfInfo > 0)
	{
		nPreAfInfo = bitctl_get_previous_af_pframe(ch);
		dbtprintf4("p nPreAfInfo %d\r\n",nPreAfInfo);
		nAfLevel = bitctl_get_pframe_aflevel(ch, nAfInfo);

		dbtprintf4(" %dch Aflevel %d \r\n", ch, nAfLevel);
		
		if(nAfInfo > nPreAfInfo){
			nAfInfoDiff = nAfInfo - nPreAfInfo;
			
			if(nAfInfoDiff > AF_DIFF_MIN_10){
				nAfUpdown = 11 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_9){
				nAfUpdown = 10 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_8){
				nAfUpdown = 9 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_7){
				nAfUpdown = 8 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_6){
				nAfUpdown = 7 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_5){
				nAfUpdown = 6 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_4){
				nAfUpdown = 5 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_3){
				nAfUpdown = 4 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_2){
				nAfUpdown = 3 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_1){
				nAfUpdown = 2 + (nAfLevel/2);
			}else if(nAfInfoDiff > AF_DIFF_MIN_0){
				nAfUpdown = AFUP + (nAfLevel/2);
			}else {		
				nAfUpdown = AFSTAY;
			}
		}
		else{
			nAfInfoDiff = nPreAfInfo - nAfInfo;

			if(nAfInfoDiff > AF_DIFF_MIN_10){
				nAfUpdown = -11;
			}else if(nAfInfoDiff > AF_DIFF_MIN_9){
				nAfUpdown = -10;
			}else if(nAfInfoDiff > AF_DIFF_MIN_8){
				nAfUpdown = -9;
			}else if(nAfInfoDiff > AF_DIFF_MIN_7){
				nAfUpdown = -8;
			}else if(nAfInfoDiff > AF_DIFF_MIN_6){
				nAfUpdown = -7;
			}else if(nAfInfoDiff > AF_DIFF_MIN_5){
				nAfUpdown = -6;
			}else if(nAfInfoDiff > AF_DIFF_MIN_4){
				nAfUpdown = -5;
			}else if(nAfInfoDiff > AF_DIFF_MIN_3){
				nAfUpdown = -4;
			}else if(nAfInfoDiff > AF_DIFF_MIN_2){
				nAfUpdown = -3;
			}else if(nAfInfoDiff > AF_DIFF_MIN_1){
				nAfUpdown = -2;
			}else if(nAfInfoDiff > AF_DIFF_MIN_0){
				nAfUpdown = AFDOWN;
			}else {		
				nAfUpdown = AFSTAY;
			}
		}
		
		if(gbMnDebugFnc == 1)
		{
			if(nAfUpdown >= AFUP){
				char buffer[100];
				int buf_len = sprintf(buffer, "af p up__ +%10d", nAfInfoDiff);
				DispStr(buffer, 20, 24, buf_len);
			}
			if(nAfUpdown == AFSTAY){
				char buffer[100];
				int buf_len = sprintf(buffer, "af p stay  %10d", bitctl_get_current_af_pframe(ch));
				DispStr(buffer, 20, 24, buf_len);
			}
			if(nAfUpdown <= AFDOWN){
				char buffer[100];
				int buf_len = sprintf(buffer, "af p down -%10d", nAfInfoDiff);
				DispStr(buffer, 20, 24, buf_len);
			}
		}
		dbtprintf4("nAfInfo %d, nPreAfInfo %d nAfInfoDiff %d \r\n", nAfInfo, nPreAfInfo, nAfInfoDiff);
		bitctl_set_previous_af_pframe(ch, nAfInfo);

		bitctl_set_current_af_pframe(ch, bitctl_get_af());
		bitctl_set_current_af_iframe(ch, bitctl_get_af());
	}
	return nAfUpdown;
}

int bitctl_get_bit_change_pframe(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);
	
	int result = BITSTAY;
	int ndiff = 0;
	int height;
	
	//compare previous bytes and target bytes. [[
	int nPrePfrmQuant = bitctl_get_previous_pframe_quant(ch);
	
	float fQuantBytes = TOFLOAT(mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant]);
	int nPrebytes = (int)fQuantBytes;
	//print value
	//printf("Previous P frame Quant %d previous Pframe size %hfKbytes \r\n", mBitCtl[ch].nPrePfrmQuant, fQuantKBytes);
	//printf("Target Bitrate of P frame %dKbyte previous frame %dKbytes\r\n", nPfrmTgBit, nPreKbytes);
	int nPframeTarget;

	if(mBitCtl[ch].nGop > 1)
	{
		if(mBitCtl[ch].FrameCount <= (UINT)((mBitCtl[ch].nFps*mBitCtl[ch].nGop)/5))
		{
			nPframeTarget = mBitCtl[ch].nPfrmTgByte1;
		}
		else if(mBitCtl[ch].FrameCount <= (UINT)((mBitCtl[ch].nFps*mBitCtl[ch].nGop)*4/5))
		{			
			nPframeTarget = mBitCtl[ch].nPfrmTgByte2;
		}
		else
		{
			nPframeTarget = mBitCtl[ch].nPfrmTgByte3;
		}
	}
	else
		nPframeTarget = mBitCtl[ch].nPfrmTgByte3;//bitctl_get_pframe_target_bit(ch);
	
//	printf("Target Bitrate of P frame %dbyte previous frame %dbytes\r\n", nPframeTarget, nPrebytes);
	height = h264_get_height(ch);
		
	if(nPframeTarget < nPrebytes){
		ndiff = nPrebytes - nPframeTarget;
		
		if(ndiff > BITRATE_MIN_DIFF10*height){
			result = 11;
		}else if(ndiff > BITRATE_MIN_DIFF9*height){
			result = 10;
		}else if(ndiff > BITRATE_MIN_DIFF8*height){
			result = 9;
		}else if(ndiff > BITRATE_MIN_DIFF7*height){
			result = 8;
		}else if(ndiff > BITRATE_MIN_DIFF6*height){
			result = 7;
		}else if(ndiff > BITRATE_MIN_DIFF5*height){
			result = 6;
		}else if(ndiff > BITRATE_MIN_DIFF4*height){
			result = 5;
		}else if(ndiff > BITRATE_MIN_DIFF3*height){
			result = 4;
		}else if(ndiff > BITRATE_MIN_DIFF2*height){
			result = 3;
		}else if(ndiff > BITRATE_MIN_DIFF1*height){
			result = 2;
		}else if(ndiff > BITRATE_MIN_DIFF0*height){
			result = BITUP;
		}else{
			result = BITSTAY;
		}
		
	}
	else{
		ndiff = nPframeTarget - nPrebytes;

		if(ndiff > BITRATE_MIN2_DIFF10*height){
			result = -11;
		}else if(ndiff > BITRATE_MIN2_DIFF9*height){
			result = -10;
		}else if(ndiff > BITRATE_MIN2_DIFF8*height){
			result = -9;
		}else if(ndiff > BITRATE_MIN2_DIFF7*height){
			result = -8;
		}else if(ndiff > BITRATE_MIN2_DIFF6*height){
			result = -7;
		}else if(ndiff > BITRATE_MIN2_DIFF5*height){
			result = -6;
		}else if(ndiff > BITRATE_MIN2_DIFF4*height){
			result = -5;
		}else if(ndiff > BITRATE_MIN2_DIFF3*height){
			result = -4;
		}else if(ndiff > BITRATE_MIN2_DIFF2*height){
			result = -3;
		}else if(ndiff > BITRATE_MIN2_DIFF1*height){
			result = -2;
		}else if(ndiff > BITRATE_MIN2_DIFF0*height){
			result = BITDOWN;
		}else{
			result = BITSTAY;
		}
	}
	

	return result;
}

int bitctl_get_quantize_iframe_by_af(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);
	
	int i;
	int nAfQuantize;
	UINT nAfInfo;
	int nIfrmTgByte;
	int height;
	
	nAfQuantize = H264_Quantize_MAX;
	
	nAfInfo = bitctl_get_current_af_iframe(ch);
	dbtprintf4("%dch nAfInfo %d \r\n", ch, nAfInfo);

	if(nAfInfo > 0)
	{
		nIfrmTgByte = bitctl_get_iframe_target_bit(ch);
		
		//printf("%dch AF info %d\r\n", ch, nAfInfoSumt);

		height = h264_get_height(ch);
		//check level
		for(i=0; i<MAX_AF_LEVEL; i++){
			if(nAfInfo <= AF_INFO_LEVEL[i]){		

				//Calculate Af info
				nAfQuantize = (int)((float)(nIfrmTgByte / (AF_BASE_BYTE * height))/ AF_RATIO_VAL[ch][i]);
				nAfQuantize = H264_Quantize_MAX - nAfQuantize;

				if(nAfQuantize > H264_Quantize_MAX) nAfQuantize = H264_Quantize_MAX;
				else if(nAfQuantize < H264_Quantize_MIN ) nAfQuantize = H264_Quantize_MIN; 

				dbtprintf4("%dch Af Level %d \r\n", ch, i);
				dbtprintf4("%dch Iframe nSelectQ %d \r\n", ch, nAfQuantize);
				
				break;
			}	
		}

	}
	
		bitctl_set_previous_af_level_iframe(ch, i);
		bitctl_set_previous_af_iframe(ch, nAfInfo);	
		bitctl_set_current_af_pframe(ch, bitctl_get_af());
		bitctl_set_current_af_iframe(ch, bitctl_get_af());
	
	return nAfQuantize;
}

int bitctl_get_quantize_pframe_by_af(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);
	
	int i;
	int nAfQuantize;
	UINT nAfInfo;
	int nPfrmTgByte;
	
	int height;
	
	nAfQuantize = H264_Quantize_MAX;
	
	nAfInfo = bitctl_get_current_af_pframe(ch);
	
	dbtprintf4("%dch nAfInfo %d \r\n", ch, nAfInfo);
	
	if(nAfInfo > 0)
	{
		nPfrmTgByte = bitctl_get_pframe_target_bit(ch);
		
	//	printf("%dch AF info %d\r\n", ch, nAfInfo);

			
		height = h264_get_height(ch);

		//check level
		for(i=0; i<MAX_AF_LEVEL; i++){
			if(nAfInfo <= AF_INFO_LEVEL[i]){		

				//Calculate Af info
				nAfQuantize = (int)((float)(nPfrmTgByte / (AF_BASE_BYTE * height))/ AF_RATIO_VAL[ch][i]);
				nAfQuantize = H264_Quantize_MAX - nAfQuantize;

				if(nAfQuantize > H264_Quantize_MAX) nAfQuantize = H264_Quantize_MAX;
				else if(nAfQuantize < H264_Quantize_MIN ) nAfQuantize = H264_Quantize_MIN; 

				printf("%dch Af Level %d \r\n", ch, i);
				printf("%dch Pframe nSelectQ %d \r\n", ch, nAfQuantize);
				
				break;
			}	
		}

		bitctl_set_previous_af_level_pframe(ch, i);
		bitctl_set_previous_af_pframe(ch, nAfInfo);
		bitctl_set_current_af_pframe(ch, bitctl_get_af());
		bitctl_set_current_af_iframe(ch, bitctl_get_af());
	}
	return nAfQuantize;
}

int bitctl_get_pframe_aflevel(int ch, UINT nAfInfo)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int i;
	int nAfLevel;	
		
//	printf("%dch AF info %d\r\n", ch, nAfInfo);
		
	//check level
	for(i=0; i<MAX_AF_LEVEL; i++){
		if(nAfInfo <= AF_INFO_LEVEL[i]){	
			break;
		}	
	}
	
	nAfLevel = i;

	bitctl_set_previous_af_level_pframe(ch, i);
	
	return nAfLevel;
}

int bitctl_get_quantize_iframe_by_prebit(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

 	int i, minQ, preQ, selectQ;
	int tagetbyte;
	int nAfQuantize;
	int nPreIfrmByte;
	int height;
	
	float ftargetbyte;
	float32 cminbyte;	
	float32 cdiffbyte;
	float32 ctargetbyte;
	
	preQ = bitctl_get_previous_iframe_quant(ch);
	nAfQuantize = bitctl_get_quantize_iframe_by_af(ch);
	tagetbyte = bitctl_get_iframe_target_bit(ch);
	nPreIfrmByte = bitctl_get_previous_iframe_byte(ch);
	
	//printf("%ch iframe previousQ %d tagetbyte %d nAfQuantize %d nPreIfrmByte %d\r\n", ch, preQ, tagetbyte, nAfQuantize, nPreIfrmByte);
	
	ftargetbyte = (float) tagetbyte;
	
	//printf("%ch iframe ftargetbyte %lf \r\n", ch, ftargetbyte);
	
	ctargetbyte = TOFLOAT32(ftargetbyte);
	cminbyte = ctargetbyte;
	minQ = -1; 
	selectQ = 0;
	
	height = h264_get_height(ch);

	for(i=0;i<= H264_Quantize_MAX; i++)
	{
		if(_fne(mBitCtl[ch].cIfrmQuantByByte[selectQ], 0) == 1)
		{			
			if(_fgt(ctargetbyte, mBitCtl[ch].cIfrmQuantByByte[selectQ]) == 1)
			{
				cdiffbyte = _fsub(ctargetbyte, mBitCtl[ch].cIfrmQuantByByte[selectQ]);

				if(cdiffbyte < cminbyte)
				{
					cminbyte = cdiffbyte;
					minQ = selectQ;
					dbtprintf4("selectQ %d !\r\n", selectQ);
				}
				
				if( _flt(cdiffbyte,0.0) == 1 )
					break;
			}
			else
			{
				cdiffbyte = _fsub(mBitCtl[ch].cIfrmQuantByByte[selectQ], ctargetbyte);

				if(cdiffbyte < cminbyte)
				{
					cminbyte = cdiffbyte;
					minQ = selectQ;
					dbtprintf4("selectQ %d !\r\n", selectQ);
				}
				
				if( _flt(cdiffbyte,0.0) == 1 )
					break;
			}
		}
		selectQ++;
	}

	if(minQ == -1 || minQ == preQ || minQ != nAfQuantize)
	{
		int BitDiff = ABSDIFF(tagetbyte, nPreIfrmByte);

		minQ = preQ;
		
//		printf("BitDiff %d / %d\r\n", BitDiff, (BITRATE_IFRM_MIN_DIFF*height));
		if(BitDiff > (BITRATE_IFRM_MIN_DIFF*height))
		{
			int limitgap = BitDiff/(BITRATE_IFRM_MIN_DIFF*height);
			int QuantGap = 0;
						
			if(tagetbyte > nPreIfrmByte)
			{
//				printf("minQ %d nAfQuantize %d limitgap %d\r\n", minQ, nAfQuantize, limitgap);
				if(minQ > nAfQuantize)
				{
				 	QuantGap = minQ - nAfQuantize;
				}
				else if(minQ == nAfQuantize && limitgap < 4)					
				{
					nAfQuantize = 0;
				}
				else
				{
					QuantGap = 1;
				}
				
				dbtprintf4(" preQ %d limitgap %d minQ%d nAfQuantize %d QuantGap %d\r\n",preQ, limitgap, minQ, nAfQuantize, QuantGap);
				if(limitgap > QuantGap && QuantGap > 2)
					minQ = ((nAfQuantize+(QuantGap/2)) < H264_Quantize_MAX) ? (nAfQuantize+(QuantGap/2)) : H264_Quantize_MAX;
				else if(QuantGap > 1)
					minQ = ((preQ-1) > H264_Quantize_MIN) ? preQ-1 : H264_Quantize_MIN;
				else					
					minQ = ((preQ) > H264_Quantize_MIN) ? preQ : H264_Quantize_MIN;
			}
			else
			{
//				printf("minQ %d nAfQuantize %d limitgap %d\r\n", minQ, nAfQuantize, limitgap);
				
				if(nAfQuantize > minQ)
					QuantGap = nAfQuantize - minQ;
				
				else if(minQ == nAfQuantize && limitgap < 4)					
				{
					nAfQuantize = 0;
				}
				else
				{
					QuantGap = 1;
				}
				
				dbtprintf4(" preQ %d limitgap %d minQ%d nAfQuantize%d QuantGap %d\r\n",preQ,limitgap, minQ, nAfQuantize, QuantGap);
				if(limitgap > QuantGap && QuantGap > 2)
					minQ = ((minQ+(QuantGap/2)) < H264_Quantize_MAX) ? (minQ+(QuantGap/2)) : H264_Quantize_MAX;
				else if(QuantGap > 1)
					minQ = ((preQ+1) < H264_Quantize_MAX) ? preQ+1 : H264_Quantize_MAX;
				else
					minQ = ((preQ) < H264_Quantize_MAX) ? preQ : H264_Quantize_MAX;
			}
		}
	}

	if(minQ < H264_Quantize_MIN) 
	{
//		printf("minQ min!\r\n");
		minQ = H264_Quantize_MIN;
	}
	
	if(minQ > H264_Quantize_MAX) 
	{
//		printf("minQ max!\r\n");
		minQ = H264_Quantize_MAX;
	}
	
	return minQ;
}

int bitctl_get_previous_iframe_bitupdown(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);
	
	int nDiff, nUpDown = QSTAY;
	int nPreIfrmQuant;
	
	nPreIfrmQuant = bitctl_get_previous_iframe_quant(ch);	
	float fPrebyte = TOFLOAT(mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant]);
	int nPrebytes = (int)fPrebyte;

	//print value
	//printf("%dch Previous I frame Quant %d previous Iframe size %dbytes \r\n", ch, nPreIfrmQuant, nPrebytes);
	
	int height;

	height = h264_get_height(ch);

	if(mBitCtl[ch].nIfrmTgByte < nPrebytes)
	{					
		nDiff = nPrebytes - mBitCtl[ch].nIfrmTgByte;
		
		if(nDiff > height)
			nUpDown = QUP;
	}
	else
	{					
		nDiff = mBitCtl[ch].nIfrmTgByte - nPrebytes;
		
		if(nDiff > height)
			nUpDown = QDOWN;
	}
		
	return nUpDown;
}

int bitctl_get_up_quantize_iframe_by_af_prebit(int ch, int nAfInfoUpdown)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int nQDiff;
	int iQuant;
	int nBitUpdown;
	int nAfQuantize;
	int nPreIfrmQuant;

	iQuant = H264_Quantize_MAX;
		
	nBitUpdown = bitctl_get_previous_iframe_bitupdown(ch);
	nAfQuantize = bitctl_get_quantize_iframe_by_af(ch);
	nPreIfrmQuant = bitctl_get_previous_iframe_quant(ch);
	
	if(nAfQuantize > nPreIfrmQuant)
	{
		nQDiff = nAfQuantize - nPreIfrmQuant;

		if(nQDiff > H264_QSTEP_MAX)
		{
		
			if( nAfInfoUpdown > 4)
			{
				if(nAfQuantize > H264_Quantize_MAX)
					iQuant = H264_Quantize_MAX;
				else
					iQuant = nAfQuantize;
			}						
			else if( nAfInfoUpdown > 3)
			{
				if(nAfQuantize > H264_Quantize_MAX)
					iQuant = H264_Quantize_MAX;
				else
					iQuant = nPreIfrmQuant + (nQDiff/2);
			}						
			else if( nAfInfoUpdown > 2)
			{
				if(nAfQuantize > H264_Quantize_MAX)
					iQuant = H264_Quantize_MAX;
				else
					iQuant = nPreIfrmQuant + (nQDiff/3);
			}
			else if (nAfInfoUpdown > 1)
			{
				iQuant = (nPreIfrmQuant+ (nQDiff/4) < H264_Quantize_MAX) ? nPreIfrmQuant+ (nQDiff/4) : H264_Quantize_MAX;
			}
			else
			{
				iQuant = (nPreIfrmQuant+2 < H264_Quantize_MAX) ? nPreIfrmQuant+2 : H264_Quantize_MAX;
			}
		}
		else
		{
			if (nAfInfoUpdown > 1)
			{
				iQuant = nAfQuantize;
			}
			else
			{
				iQuant = (nPreIfrmQuant+1 < H264_Quantize_MAX) ? nPreIfrmQuant+1 : H264_Quantize_MAX;
			}
		}
	}
	else
	{
		if(nBitUpdown == QUP)
		{
			if( nAfInfoUpdown > 4)
			{
				iQuant = (nPreIfrmQuant+5 < H264_Quantize_MAX) ? nPreIfrmQuant+5 : H264_Quantize_MAX;
			}
			else if( nAfInfoUpdown > 3)
			{
				iQuant = (nPreIfrmQuant+4 < H264_Quantize_MAX) ? nPreIfrmQuant+4 : H264_Quantize_MAX;
			}						
			else if( nAfInfoUpdown > 2)
			{
				iQuant = (nPreIfrmQuant+3 < H264_Quantize_MAX) ? nPreIfrmQuant+3 : H264_Quantize_MAX;
			}
			else if (nAfInfoUpdown > 1)
			{
				iQuant =(nPreIfrmQuant+2 < H264_Quantize_MAX) ? nPreIfrmQuant+2 : H264_Quantize_MAX;
			}
			else
			{
				iQuant = (nPreIfrmQuant+1 < H264_Quantize_MAX) ? nPreIfrmQuant+1 : H264_Quantize_MAX;
			}
		}
		else if(nBitUpdown ==QDOWN)
		{
			iQuant = (nPreIfrmQuant-1 > H264_Quantize_MIN) ? nPreIfrmQuant-1 : H264_Quantize_MIN;
		}
		else
		{
			iQuant = nPreIfrmQuant;
		}
	}

	return iQuant;
}

int bitctl_get_down_quantize_iframe_by_af_prebit(int ch, int nAfInfoUpdown)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int nQDiff;
	int iQuant;
	int nBitUpdown;
	int nAfQuantize;
	int nPreIfrmQuant;

	iQuant = H264_Quantize_MAX;
	
	nBitUpdown = bitctl_get_previous_iframe_bitupdown(ch);
	nAfQuantize = bitctl_get_quantize_iframe_by_af(ch);
	nPreIfrmQuant = bitctl_get_previous_iframe_quant(ch);
					
	if(nAfQuantize < nPreIfrmQuant)
	{
		nQDiff = nPreIfrmQuant - nAfQuantize;

		if(nQDiff > H264_QSTEP_MAX)
		{
			if( nAfInfoUpdown < -10)
			{
				iQuant = nAfQuantize;
			}
			else if (nAfInfoUpdown < -1)
			{
				iQuant = (nPreIfrmQuant-H264_QSTEP_MAX > H264_Quantize_MIN) ? nPreIfrmQuant-H264_QSTEP_MAX : H264_Quantize_MIN;
			}
			else
			{
				iQuant = (nPreIfrmQuant-2 > H264_Quantize_MIN) ? nPreIfrmQuant-2 : H264_Quantize_MIN;
			}
		}
		else
		{
			if (nAfInfoUpdown < -1)
			{
				iQuant = nAfQuantize;
			}
			else
			{
				iQuant = (nPreIfrmQuant-1 > H264_Quantize_MIN) ? nPreIfrmQuant-1 : H264_Quantize_MIN;
			}
		}
	}
	else
	{
		if(nBitUpdown == QDOWN)
		{
			if( nAfInfoUpdown < -10)
			{
				iQuant = (nPreIfrmQuant-3 > H264_Quantize_MIN) ? nPreIfrmQuant-3 : H264_Quantize_MIN;
			}
			else if (nAfInfoUpdown < -1)
			{
				iQuant =(nPreIfrmQuant-2 > H264_Quantize_MIN) ? nPreIfrmQuant-2 : H264_Quantize_MIN;
			}
			else
			{
				iQuant = (nPreIfrmQuant-1 > H264_Quantize_MIN) ? nPreIfrmQuant-1 : H264_Quantize_MIN;
			}
		}
		else if(nBitUpdown == QUP)
		{
			iQuant = (nPreIfrmQuant+1 < H264_Quantize_MAX) ? nPreIfrmQuant+1 : H264_Quantize_MAX;
		}
		else
		{
			iQuant = nPreIfrmQuant;
		}
	}		
	
	return iQuant;
}



void bitctl_init_seq(int ch)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	int i;
	float fDefVal;
	float32 cDefVal;
	int quant;
	
	fDefVal = 0.0f;
	cDefVal = TOFLOAT32(fDefVal);
	
	mBitCtl[ch].nPreFrmBytes   	= 	0;
//	mBitCtl[ch].nRemainToBytes 	= 	0;
	mBitCtl[ch].nEndIFrmCnt 	= 	0;
	mBitCtl[ch].nEndPFrmCnt 	= 	0;
	mBitCtl[ch].nTotalBytes		= 	0;

	quant = enx_h264_get_quantize(ch);

	//if(mBitCtl[ch].nPreIfrmQuant == 0)
	//	mBitCtl[ch].nPreIfrmQuant = quant;
	
	//if(mBitCtl[ch].nPrePfrmQuant == 0)
	//	mBitCtl[ch].nPrePfrmQuant = quant;
	mBitCtl[ch].nOverBytes = 0;
	mBitCtl[ch].nTotalBytes = 0;
	mBitCtl[ch].nPreIfrmQuant = quant;
		mBitCtl[ch].nPrePfrmQuant = quant;
	bitctl_set_iframe_quantize(ch, quant);
	bitctl_set_pframe_quantize(ch, quant);

	for(i=0;i<MAX_AF_LEVEL;i++)
	{
		AF_RATIO_VAL[ch][i] = AF_RATIO[ch][i];
	}
	
	for(i=0; i<52; i++){
		mBitCtl[ch].cIfrmQuantByByte[i] = cDefVal;
		mBitCtl[ch].cPfrmQuantByByte[i] = cDefVal;
		
		//fQuantKBytes = TOFLOAT(mBitCtl[ch].cIfrmQuantByByte[i]);
		//printf("%dth fQuantKBytes %hfKbytes \r\n", i, fQuantKBytes);
	}
	
	bitctl_set_current_ae(ch, AEControlErr);

	bitctl_set_iframe_maxsize(H264_IFRAME_RATIO, ch);
	bitctl_set_overbitsize(H264_OVERBIT_RATIO, ch);

}


void bitctl_init_gop(int ch)
{
	mBitCtl[ch].FrameCount = 0;
	
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);
	
	int nFps;//FPS 	
	int nGop;//GOP	
	int nIdr;//iframe interval
	int nOverBytes;
	int nFpsTgByte;//Target Bitrate of Fps	
	int nGopTgByte;//Target Bitrate of Gop	
	int nIfrmTgByte;
	int nIfrmTgByte1;
	int nIfrmTgByte2;
	int nIfrmTgByte3;
	int nPfrmTgByte;
	
	float32 cGopTgByte;
	float32 cIfrmTgByte;//I frame Target Bitrate	
	float32 fIfrmRatio;//I frame Bitrate Ratio	
	float32 nIfrmPercent;//I frame Percent	

	int interval;
	int period;
	int maxframe;
	int bitrate;
	int height;
	int passover;

	interval = enx_h264_get_idr_interval(ch);
	period = h264_get_period(ch);
	maxframe = enx_h264_get_maxframe();
	bitrate = enx_h264_get_bit_rate(ch);
	height = h264_get_height(ch);
	
	dbtprintf2("i_interval %d \r\n", interval);
	dbtprintf2("frame per second %d \r\n", maxframe/period);

	nOverBytes = 0;

	nIdr = interval;
	
	//Calculate Fps
	nFps = (maxframe/period);
	mBitCtl[ch].nFps = nFps;
	dbtprintf2("Fps %d \r\n", nFps);
	
	//Calculate Gop
	nGop = (nIdr/nFps);
	if(nGop<1) nGop = 1;
	
	mBitCtl[ch].nGop = nGop;
	dbtprintf2("Gop %d \r\n", nGop);
	
	dbtprintf2("%dch nOverBitRatio %d\r\n", ch, mBitCtl[ch].nOverBitRatio);
	if(mBitCtl[ch].nOverBitRatio < 1)
		passover = 1;
	else if(mBitCtl[ch].nOverBitRatio > 80)
		passover = 80;
	else
		passover = mBitCtl[ch].nOverBitRatio;
	
	//Calculate Target Bitrate of GOP
	nFpsTgByte = bitrate+(bitrate*passover/100);	

	if(mBitCtl[ch].nTotalBytes > 0)
	{
		H264BPS[ch] = (mBitCtl[ch].nTotalBytes*8*nFps/nIdr)/1024;
		
#if 1 //it's for high bitrate mode.
		nOverBytes = 0;

		nOverBytes = nFpsTgByte - (mBitCtl[ch].nTotalBytes/nGop);
		dbtprintf2("%dch %dKbps total byte %d nOverBytes %d\r\n", ch, H264BPS[ch], mBitCtl[ch].nTotalBytes, nOverBytes);
		
		//nOverBytes = ((nOverBytes)*mBitCtl[ch].nOverBitRatio)/100 ;
		
		mBitCtl[ch].nOverBytes += nOverBytes;
		dbtprintf2("mBitCtl[ch].nOverBytes %d \r\n", mBitCtl[ch].nOverBytes);

#endif		
		
//		printf("%dch %dKbps t %d nGop %d/ %d, %d, %d\r\n", ch, H264BPS[ch], mBitCtl[ch].nTotalBytes, nGop, (mBitCtl[ch].nTotalBytes*8), ((mBitCtl[ch].nTotalBytes*8)/(nGop)), ((mBitCtl[ch].nTotalBytes*8)/(nGop))/1024);
	}
//	mBitCtl[ch].nRemainToBytes 	= 	0;
	mBitCtl[ch].nTotalBytes 	=  	0;
	
	
	dbtprintf2("%dch Target cbr.bitrate %d \r\n", ch, nFpsTgByte);
	
	float m;

	m = mBitCtl[ch].nIfrmRatio; 
	dbtprintf2("passover %d \r\n", passover);
	
	//Set I frame Percent
	nIfrmPercent = TOFLOAT32(m);
	float m3 = 100.0f;
	float32 m2 = TOFLOAT32(m3);
	//Calculate I frame Ratio
	fIfrmRatio = _fdiv( nIfrmPercent, m2);
		
	
	mBitCtl[ch].nOverBytes = mBitCtl[ch].nOverBytes*(100-passover)/100;
	dbtprintf2("%dch nOverBytes %d\r\n", ch, mBitCtl[ch].nOverBytes);

	if(mBitCtl[ch].nOverBytes > nFpsTgByte)
	{
		mBitCtl[ch].nOverBytes	= nFpsTgByte*60/100;
	}
	else if(mBitCtl[ch].nOverBytes < 0 && ((int)nFpsTgByte) + mBitCtl[ch].nOverBytes < 0 )
	{
		mBitCtl[ch].nOverBytes = nFpsTgByte*60/100 * (-1);
	}
	
	//Calculate Total Target Bitrate
	nGopTgByte = (nFpsTgByte * nGop) + mBitCtl[ch].nOverBytes;
	dbtprintf2("%dch nGopTgByte %d nOverBytes %d mBitCtl[ch].nOverBytes %d\r\n", ch, nGopTgByte, nOverBytes, mBitCtl[ch].nOverBytes);

	if(nIdr != 1)
	{
		//Calculate I frame Target Bitrate
		cGopTgByte = TOFLOAT32(nGopTgByte);
		cIfrmTgByte = (int)_fmul(cGopTgByte, fIfrmRatio);
		nIfrmTgByte = TOUINT(cIfrmTgByte);	
		nIfrmTgByte = nIfrmTgByte*nIdr/nFps;	
	}
	else
	{
		nIfrmTgByte = nGopTgByte/nFps;
	}
	
	nIfrmTgByte1 = nIfrmTgByte;
	nIfrmTgByte2 = 0;//nIfrmTgByte;
	//nIfrmTgByte3 = nIfrmTgByte/4;
	nIfrmTgByte3 = nIfrmTgByte/8;
	
//	printf("%dch Target Bitrate of I frame %dbyte nFpsTgByte %dbyte nGopTgByte %dbyte\r\n", ch, nIfrmTgByte, nFpsTgByte, nGopTgByte);	

	if((nFpsTgByte) < nIfrmTgByte)
	{
		nIfrmTgByte = ((nFpsTgByte*2)/3);
		nIfrmTgByte1 = (nFpsTgByte);
		nIfrmTgByte2 = 0;//(nFpsTgByte/6);
		nIfrmTgByte3 = (nFpsTgByte/2);
	}
	
	int maxsize = height * 600;
	//printf("maxsize %d iframe tg size %d \r\n",maxsize, mBitCtl[ch].nIfrmTgByte);
	if(maxsize < nIfrmTgByte)
	{
		nIfrmTgByte = maxsize;
		nIfrmTgByte1 = (maxsize);
		nIfrmTgByte2 =  0;//(maxsize/4);
		nIfrmTgByte3 = (maxsize/2);
	}

	
	dbtprintf2("%dch Target Bitrate of I frame %dbyte nFpsTgByte %dbyte nGopTgByte %dbyte\r\n", ch, nIfrmTgByte, nFpsTgByte, nGopTgByte);	
	//printf("%dch Target Bitrate of I frame %dbyte \r\n", ch, mBitCtl[ch].nIfrmTgByte);	

	if(interval > 1)
	{
	//last gop pframe
		nPfrmTgByte = (nGopTgByte - (nIfrmTgByte*nFps/nIdr))/(interval-1);
	//middle gop pframe
		mBitCtl[ch].nPfrmTgByte1 = (nGopTgByte - nIfrmTgByte1*nFps/nIdr)/(interval-1);
	//first gop pframe
		mBitCtl[ch].nPfrmTgByte2 = (nGopTgByte - nIfrmTgByte2*nFps/nIdr)/(interval-1);
	//only p frame gop
		mBitCtl[ch].nPfrmTgByte3 = (nGopTgByte - nIfrmTgByte3*nFps/nIdr)/(interval-1);
	
#ifdef __H264_MCH__
	//Calculate Target Bitrate of GOP
	if(enx_h264_mch_get_profile(ch) == e_pmMainCB && enx_h264_mch_get_profile_decrease(ch) > 0)	
	{
		nIfrmTgByte = nIfrmTgByte - ((nIfrmTgByte/20)*enx_h264_mch_get_profile_decrease(ch));
		dbtprintf2("%dch Target cbr.bitrate %d Main profile nIfrmTgByte %d devide %d \r\n", ch, nFpsTgByte, nIfrmTgByte, ((nIfrmTgByte/20)*enx_h264_mch_get_profile_decrease(ch)));
		nPfrmTgByte = nPfrmTgByte - ((nPfrmTgByte/10)*enx_h264_mch_get_profile_decrease(ch));
		
		mBitCtl[ch].nPfrmTgByte1 = mBitCtl[ch].nPfrmTgByte1 - ((mBitCtl[ch].nPfrmTgByte1/10)*enx_h264_mch_get_profile_decrease(ch));
		mBitCtl[ch].nPfrmTgByte2 = mBitCtl[ch].nPfrmTgByte2 - ((mBitCtl[ch].nPfrmTgByte2/20)*enx_h264_mch_get_profile_decrease(ch));
		mBitCtl[ch].nPfrmTgByte3 = mBitCtl[ch].nPfrmTgByte3 - ((mBitCtl[ch].nPfrmTgByte3/10)*enx_h264_mch_get_profile_decrease(ch));
		dbtprintf2("%dch Target cbr.bitrate %d Main profile nPfrmTgByte %d devide %d \r\n", ch, nFpsTgByte, nPfrmTgByte, ((nPfrmTgByte/10)*enx_h264_mch_get_profile_decrease(ch)));
	}	
#endif
		dbtprintf2("%dch Target Bitrate of P frame %dbyte/%dbyte/%dbyte/%dbyte \r\n", ch, mBitCtl[ch].nPfrmTgByte, mBitCtl[ch].nPfrmTgByte1, mBitCtl[ch].nPfrmTgByte2, mBitCtl[ch].nPfrmTgByte3);

	}
	else
	{
		
		nPfrmTgByte = 1;

		mBitCtl[ch].nPfrmTgByte1 = 1;
		mBitCtl[ch].nPfrmTgByte2 = 1;
		mBitCtl[ch].nPfrmTgByte3 = 1;
	}
	bitctl_set_iframe_target_bit(ch, nIfrmTgByte);
	bitctl_set_pframe_target_bit(ch, nPfrmTgByte);
	
}


void bitctl_init_frame_i_p(int ch, int slicetype)
{
	dbtprintf1(" %s(%d) Start %dch slicetype %d warning %d\r\n",__func__, __LINE__, ch, slicetype,  bitctl_get_warning(ch, slicetype));

	if(bitctl_get_warning(ch, slicetype) != NONE)
	{
		
		printf("Warning encoded size! \r\n");

		if(slicetype == 0)//i frame
		{
			int iQuantize;
			int overcnt;
			int oversize;
			int nIfrmTgByte;
			
			oversize = bitctl_get_warning_size(ch, slicetype);
			nIfrmTgByte = bitctl_get_iframe_target_bit(ch);
			
			if (oversize > nIfrmTgByte)
			{
				overcnt = oversize/nIfrmTgByte;
			}
			else
			{
				overcnt = 0;
			}
			
			iQuantize = ((mBitCtl[ch].nPreIfrmQuant+(1+overcnt)) < H264_Quantize_MAX) ? (mBitCtl[ch].nPrePfrmQuant+(1+overcnt)) : H264_Quantize_MAX;
			
			dbtprintf3("%dch set i frame Q %d\r\n", ch,  iQuantize);

			//set quant
			bitctl_set_iframe_quantize(ch, iQuantize);
				
			//set quant previous value.
			bitctl_set_previous_iframe_quant(ch, iQuantize);

			
			H264_Quantize(ch, bitctl_get_iframe_quantize(ch));
		}
		else
		{
			int pQuantize;
			int overcnt;
			int oversize;
			int nPfrmTgByte;
			
			oversize = bitctl_get_warning_size(ch, slicetype);
			nPfrmTgByte = bitctl_get_pframe_target_bit(ch);
			
			if (oversize > nPfrmTgByte)
			{
				overcnt = oversize/nPfrmTgByte;
			}
			else
			{
				overcnt = 0;
			}
			
			pQuantize = ((mBitCtl[ch].nPrePfrmQuant+(1+overcnt)) < H264_Quantize_MAX) ? (mBitCtl[ch].nPrePfrmQuant+(1+overcnt)) : H264_Quantize_MAX;
			
			dbtprintf3("%dch set pframe Q %d\r\n",  ch, pQuantize);
			
			bitctl_set_pframe_quantize(ch, pQuantize);
			//set quant previous value.
			bitctl_set_previous_pframe_quant(ch, pQuantize);
			
			H264_Quantize(ch, bitctl_get_pframe_quantize(ch));
		}
	}
    else if(slicetype == 0)	//i frame
    {
		int iQuantize;

		if(mBitCtl[ch].nEndIFrmCnt == 0)
		{//first I frame after booting.
			iQuantize = bitctl_get_quantize_iframe_by_af(ch);
			dbtprintf3("%dch iframe bitctl_get_quantize_iframe_by_af Q %d \r\n", ch, iQuantize);	
		} 
		else 
		{//normal I frame
			int nAfInfoUpdown;				
			int nAeErrLevel; 
			
			nAeErrLevel = bitctl_get_ae_level(ch);
			dbtprintf3("%dch nAeErrLevel %d\r\n",  ch, nAeErrLevel);
			nAfInfoUpdown = bitctl_get_af_change_iframe(ch);

			dbtprintf3("%dch iframe nAfInfoUpdown %d \r\n", ch, nAfInfoUpdown);	

			if(nAfInfoUpdown >= AFUP)
			{				
				dbtprintf3("%dch iframe nAfInfoUp \r\n", ch);					
				iQuantize = bitctl_get_up_quantize_iframe_by_af_prebit(ch, nAfInfoUpdown);				
			}
			else if(nAfInfoUpdown <= AFDOWN)
			{
				dbtprintf3("%dch iframe nAfInfoDown \r\n", ch);						
				iQuantize = bitctl_get_down_quantize_iframe_by_af_prebit(ch, nAfInfoUpdown);	
			}
			else
			{			
				if(nAeErrLevel > 1)
				{
					iQuantize = bitctl_get_quantize_iframe_by_prebit(ch);
					dbtprintf3("%dch iframe Af stay AE err %d iQuantize %d\r\n", ch, nAeErrLevel, iQuantize);		
					iQuantize += 1;
				}
				else
				{
				iQuantize = bitctl_get_quantize_iframe_by_prebit(ch);
				dbtprintf3("%dch iframe Af stay iQuantize %d\r\n", ch, iQuantize);		
			}
		}
		}
		
		if(iQuantize  > H264_Quantize_MAX)
			 iQuantize = H264_Quantize_MAX;
		else if(iQuantize < H264_Quantize_MIN)
			iQuantize = H264_Quantize_MIN;
								
		dbtprintf3("%dch set iframe Q %d\r\n", ch, iQuantize);

		//set quant
		bitctl_set_iframe_quantize(ch, iQuantize);
			
		//set quant previous value.
		bitctl_set_previous_iframe_quant(ch, iQuantize);		
				
		H264_Quantize(ch, bitctl_get_iframe_quantize(ch));
    }
	else 	//p frame
	{
		int pQuantize;
		int nAfInfoUpdown = AFSTAY;
		
		//AE info level
		int nAeErrLevel, nDiff;	
		
		nAeErrLevel = bitctl_get_ae_level(ch);
		dbtprintf3("%dch nAeErrLevel %d\r\n",  ch, nAeErrLevel);
		
		nAfInfoUpdown = bitctl_get_af_change_pframe(ch);

		//Control Q of I frame
		if(mBitCtl[ch].nEndPFrmCnt == 0)
		{
			pQuantize = bitctl_get_quantize_pframe_by_af(ch);	
			dbtprintf3("%dch bitctl_get_quantize_pframe_by_af %d\r\n",  ch, pQuantize);
		}	
		/*else if(mBitCtl[ch].FrameCount == 1 && mBitCtl[ch].nCurPfrmQuant < mBitCtl[ch].nCurIfrmQuant)
		{
			int diffQ = mBitCtl[ch].nCurIfrmQuant - mBitCtl[ch].nCurPfrmQuant;
			printf("%dch I Q %d P Q %d\r\n",  ch, mBitCtl[ch].nCurIfrmQuant, mBitCtl[ch].nCurPfrmQuant);
			pQuantize = (mBitCtl[ch].nCurPfrmQuant+(diffQ/4) < H264_Quantize_MAX) ? mBitCtl[ch].nCurPfrmQuant+(diffQ/4): H264_Quantize_MAX;
			dbtprintf3("%dch pQuantize %d\r\n",  ch, pQuantize);
		}*/
		else// if(mBitCtl[ch].FrameCount % 2 == 0)
		{		
			nDiff = bitctl_get_bit_change_pframe(ch);
			
			if(nDiff >= BITUP)
			{				
				if(nAfInfoUpdown >= AFUP)
				{
					dbtprintf3("AFUP level high birate %d\r\n", nAfInfoUpdown);
					pQuantize = ((mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown)) < H264_Quantize_MAX) ? (mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown)) : H264_Quantize_MAX;
				}
				else if(nAfInfoUpdown <= AFDOWN)
				{
					dbtprintf3("AFDOWN level high birate %d\r\n", nAfInfoUpdown);
					pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown/2) > H264_Quantize_MIN) ? mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown/2) : H264_Quantize_MIN;
				}			
				else
				{
					if(nAeErrLevel > 1)
					{
						dbtprintf3("High birate nDiff  %d AE nAeErrLevel %d \r\n", nDiff, nAeErrLevel);
						nDiff += 1;
						pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nDiff) < H264_Quantize_MAX) ? mBitCtl[ch].nPrePfrmQuant+(nDiff) : H264_Quantize_MAX;
					}
					else
					{
					dbtprintf3("High birate different level %d\r\n", nDiff);
					pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nDiff) < H264_Quantize_MAX) ? mBitCtl[ch].nPrePfrmQuant+(nDiff) : H264_Quantize_MAX;
				}
			}
			}
			else if(nDiff <= BITDOWN)
			{
				if(nAfInfoUpdown >= AFUP)
				{
					dbtprintf3("AFUP level %d lower birate\r\n", nAfInfoUpdown);
					pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown) < H264_Quantize_MAX) ? mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown) : H264_Quantize_MAX;
				}
				else if(nAfInfoUpdown <= AFDOWN)
				{
					dbtprintf3("AFDOWN level %d lower birate\r\n", nAfInfoUpdown);
					pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown/2) > H264_Quantize_MIN) ? mBitCtl[ch].nPrePfrmQuant+(nAfInfoUpdown/2) : H264_Quantize_MIN;
				}				
				else
				{
					if(nAeErrLevel > 1)
					{
						dbtprintf3("lower birate nDiff  %d AE nAeErrLevel %d \r\n", nDiff, nAeErrLevel);
						nDiff += 1;
						pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nDiff) > H264_Quantize_MIN) ? mBitCtl[ch].nPrePfrmQuant+(nDiff) : H264_Quantize_MIN;
					}
					else
					{
					dbtprintf3("lower birate different level %d\r\n", nDiff);
					pQuantize = (mBitCtl[ch].nPrePfrmQuant+(nDiff) > H264_Quantize_MIN) ? mBitCtl[ch].nPrePfrmQuant+(nDiff) : H264_Quantize_MIN;
				}
				}

			}
			else
			{
				if(nAeErrLevel > 1)
				{
				pQuantize = bitctl_get_previous_pframe_quant(ch);
					dbtprintf3("stay Bitrate Q of Pframe  %d AE nAeErrLevel %d \r\n", pQuantize, nAeErrLevel);
					pQuantize += 1;
				}
				else
				{
					pQuantize = bitctl_get_previous_pframe_quant(ch);
					dbtprintf3("stay Bitrate Q of Pframe %d \r\n", pQuantize);
				}
			}
		}
		//else
		//{
		//	pQuantize = bitctl_get_previous_pframe_quant(ch);
		//}

		if(pQuantize  > H264_Quantize_MAX)
			 pQuantize = H264_Quantize_MAX;
		else if(pQuantize < H264_Quantize_MIN)
			pQuantize = H264_Quantize_MIN;

		dbtprintf3("%dch set pframe Q %d\r\n",  ch, pQuantize);
		
		bitctl_set_pframe_quantize(ch, pQuantize);
		//set quant previous value.
		bitctl_set_previous_pframe_quant(ch, pQuantize);
				
		H264_Quantize(ch, bitctl_get_pframe_quantize(ch));
	}

}


// hpenc_ver1.5
void bitctl_update_frame_merge(int iframe, int ch, int Bytes)
{
	dbtprintf1(" %s(%d) Start\r\n",__func__, __LINE__);

	float fDivVal = 2;
	float32 cBytes;
	float32 cDivVal;
	int nPreIfrmQuant;
	int nPrePfrmQuant;
	int height;
		
	mBitCtl[ch].nPreFrmBytes   	= 	Bytes;
//	mBitCtl[ch].nRemainToBytes 	-= 	Bytes;
	mBitCtl[ch].nTotalBytes 	+= 	Bytes;
	mBitCtl[ch].nEndIFrmCnt++;
	mBitCtl[ch].nEndPFrmCnt++;
	
	if(mBitCtl[ch].nEndIFrmCnt == 0xFFFFFFFF)
		mBitCtl[ch].nEndIFrmCnt = 1;

	if(mBitCtl[ch].nEndPFrmCnt == 0xFFFFFFFF)
		mBitCtl[ch].nEndPFrmCnt = 2;

	height = h264_get_height(ch);

	if(iframe == 1)//iframe, test ch0
	{
		nPreIfrmQuant = bitctl_get_previous_iframe_quant(ch);
		dbtprintf2("[[[[[[[[[[%dch Iframe Q%d %dbytes]]]]]]]]]]\r\n", ch, nPreIfrmQuant, Bytes);
		
		float32 v1;
		float32 v2;
		
		//calculate byte to Kbyte
		float m3 = (float)Bytes;
		//set byte
		cBytes = TOFLOAT32(m3);
		
		//set div value float32
		cDivVal = TOFLOAT32(fDivVal);
				
		v1 = cBytes;
		
		//print value		
		//printf("[%dch Iframe nPreIfrmQuant %d v1 %hf]\r\n", ch, nPreIfrmQuant, cBytes);
		
		if(mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant] == 0)
		{
			v2 = cBytes;
//			printf("%dch v2 %hf\r\n", ch, v2);
			
			mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant] = cBytes;			
			
		}
		else
		{			
			float32 m4;
			
			v2 = mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant];
			
				
			m4 = _fadd(v1, v2);
			
			mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant] = _fdiv(m4, cDivVal);
//			printf("%dch v1 %hf v2 %hf/ %hf =  m4 %hf \r\n", ch, v1, v2, cDivVal, m4);
			
		}

		mBitCtl[ch].cIfrmDiffByByte[nPreIfrmQuant] = _fsub(v1,v2);
		
		//print value		
		//printf("[%dch encoded Iframe Q %d encoded I frame size %dbytes QB %hfbytes var(%hf)]\r\n", ch, mBitCtl[ch].nPreIfrmQuant, Bytes, mBitCtl[ch].cIfrmQuantByByte[nPreIfrmQuant], mBitCtl[ch].cIfrmDiffByByte[nPreIfrmQuant]);
		
		int nAfInfoLevel = mBitCtl[ch].nPreAfLevelIfrm;
		mBitCtl[ch].nPreAfLevelIfrm = 0;

		float fAfRatioL1_temp;

		//check level
		//printf("%dch Af Level %d\r\n", ch, nAfInfoLevel);

		float target;
		float base;
		float preq;
		
		target = (float)Bytes;
		preq = (float)(H264_Quantize_MAX - nPreIfrmQuant);
		base = (float)(AF_BASE_BYTE*height);

		if(target > base)
		{
			//Calculate Af info
			if(preq<=0)
				preq = 1;
			
			fAfRatioL1_temp = ((target / base)/ preq);
			//printf("%dch target %f base %f preq %f fAfRatioL1_temp %f lv %d fAfRatio %f\r\n", ch, target, base, preq, fAfRatioL1_temp, nAfInfoLevel, AF_RATIO_VAL[ch][nAfInfoLevel]);

			AF_RATIO_VAL[ch][nAfInfoLevel] += fAfRatioL1_temp;
			AF_RATIO_VAL[ch][nAfInfoLevel] /= 2.0f;

			//printf("%dch fAfRatioL1_temp %f fAfRatio Lv%d %f\r\n", ch, fAfRatioL1_temp, nAfInfoLevel, AF_RATIO_VAL[ch][nAfInfoLevel]);
		}
		
		bitctl_set_previous_iframe_byte(ch,Bytes);

	}
	else
	{
		float m3 = (float)Bytes;		
		//set byte 
		cBytes = TOFLOAT32(m3);
		

		//set div value float32

		cDivVal = TOFLOAT32(fDivVal);
				
		//calcuate encode bytes
		nPrePfrmQuant = bitctl_get_previous_pframe_quant(ch);	
		
		//print value
		dbtprintf2("[%dch Pframe preQ %d %dbytes]\r\n", ch, nPrePfrmQuant, Bytes);
		
		if(mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant] == 0)
		{
			mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant] = cBytes;
		}
		else
		{
			mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant] = _fadd(mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant], cBytes);
			
			mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant] = _fdiv(mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant], cDivVal);
		}
		
		//print value
		//printf("[_fadd %hfKbytes]\r\n", mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant]);
		
		//print value
		//printf("[%dch encoded Pframe Q %d encoded P frame size %dbytes QB %hfbytes]\r\n", ch,  mBitCtl[ch].nPrePfrmQuant, Bytes, mBitCtl[ch].cPfrmQuantByByte[nPrePfrmQuant]);

		
		bitctl_set_previous_pframe_byte( ch, Bytes);
	}
	

	
	mBitCtl[ch].FrameCount++;
	//printf("bitctl_update_frame_merge E\r\n");
}
