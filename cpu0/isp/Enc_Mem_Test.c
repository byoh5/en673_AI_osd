/* **************************************************************************
 File Name	:	Enc_Mem_Test.c
 Description:	Ex-Risc Encryption & Memory R/W Test
 Designer	:	Lee, Wanhee
 Date		:	15. 08. 13
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"

#if 0

unsigned char US_1[4] ; // User_Val1
unsigned char US_0[4] ; // User_Val0

/***********************************************************************************
 * Variables
 ***********************************************************************************/
unsigned char super_serial[16];
unsigned char user_serial[8];
unsigned char encrypt_tx_data[8];
unsigned char encrypt_rx_data[10];
unsigned char encrypt_ex_data1[8];
unsigned char encrypt_ex_data2[8];
unsigned char sub_address;
unsigned char feedback1[4];
unsigned char feedback2[4];
unsigned char bypass_encryption;
unsigned char fix_random;
unsigned char flag_init ;

unsigned char init_data[4];

unsigned char EDR_U=0;
unsigned short EDR_S=0;

unsigned char month = 1;
unsigned char day = 1;
unsigned char hour = 0;
unsigned char shift_reg;
unsigned short access_count1=0;
unsigned short access_count2=0;

extern unsigned char art_comm_en ;

unsigned char _encrypt_rand(void)
{
	static BYTE bShift_Random=0;
	static unsigned long seed;

	bShift_Random = (bShift_Random>=8) ? 0 : bShift_Random+1;

//	seed = seed + (unsigned char)((rand()>>bShift_Random)&0xFF); // Random Data Gen
	seed = seed + (unsigned char)(((AE1_SUM1_LOCKr+AE1_SUM2_LOCKr)>>bShift_Random)&0xFF); // Random Data Gen

	seed = seed * 1103515245 + 12345;

	return (seed/65536) % 32768;
}

void ISPM _encrypt_enc1(unsigned char enc_step_u, unsigned short enc_step, unsigned char *random_data, unsigned char *xn_data, unsigned char *yn_data)
{
    unsigned char enc_key_base[20][8];
    unsigned char enc_key_step_1[8];
    unsigned char enc_key_step[8];
    unsigned char data_sel;
    unsigned char data_not[8];
    unsigned char data_xor[8];
    unsigned char data_sft[8];
    unsigned char data_sfl[8];
    unsigned char access_cnt[2];
    unsigned char shift_addr;
    unsigned char time[2];
    unsigned char bypass, enc_enable, double_enc, time_en, feedback_en, random_en, temp;

    int i, j, step_fin_sub, main_loop, sub_loop, loop_block;

    // Encryption Option Setting
    if (sub_address == 0x80) {
        bypass = 1;
        enc_enable = 0;
        double_enc = 0;
        time_en = 0;
        feedback_en = 0;
        random_en = 0;
    }
    else {
        bypass = 0;
        if (sub_address & 0x80) enc_enable = 1;
        else enc_enable = 0;
        if (sub_address & 0x40) double_enc = 1;
        else double_enc = 0;
        if (sub_address & 0x20) time_en = 1;
        else time_en = 0;
        if (sub_address & 0x10) feedback_en = 1;
        else feedback_en = 0;
        if (sub_address & 0x08) random_en = 1;
        else random_en = 0;
    }
    // Encryption Depth Parameter Generation
    step_fin_sub = (((int)(sub_address & 0x07)) + 1) * 8 ;

    // Feedback Clear
    if ( bypass | !feedback_en ) for ( i=0 ; i<4 ; i++ ) feedback1[i] = 0x00 ;

    // Shift Encryption Parameter Generation
    shift_addr = ( shift_reg & 0x80 ) ? shift_reg : 0x00 ;

    // Time Data Setting
    if ( time_en ) {
        time[0] = 0xc0 | ( ( month << 2 ) & 0x3c ) | ( ( day >> 3 ) & 0x03 ) ;
        time[1] = ( ( day << 5) & 0xe0 ) | ( hour & 0x1f ) ;
    }
    else {
        time[0] = 0x00 ;
        time[1] = 0x00 ;
    }

	// Random Data Generation
    if ( !random_en ) {
        random_data[0] = 0x00 ;
        random_data[1] = 0x00 ;
    }

#ifdef NoCountAccess
	access_count1 = 0x0000 ;
#else
    // Access Count Generation
    if ( bypass) access_count1 = 0x0000 ;
    else access_count1++ ;
#endif

    access_cnt[0] = (unsigned char)((access_count1>>8) & 0x00ff) ;
    access_cnt[1] = (unsigned char) (access_count1&0x00ff) ;

	// Feedback Clear
    if ( bypass | !feedback_en ) for ( i=0 ; i<4 ; i++ ) feedback1[i] = 0x00 ;

    // Encryption Key Setting
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i][j] = user_serial[j] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+4][j] = super_serial[j] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+8][j] = super_serial[j+8] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+12][j] = super_serial[j+4] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+16][j] = user_serial[j] ;

	enc_key_base[0][0] ^= shift_addr ;
    enc_key_base[0][1] ^= shift_addr ;
    enc_key_base[0][2] ^= random_data[0] ;
    enc_key_base[0][3] ^= random_data[1] ;
    enc_key_base[0][4] ^= feedback1[0] ;
    enc_key_base[0][5] ^= feedback1[1] ;
    enc_key_base[0][6] ^= feedback1[2] ;
    enc_key_base[0][7] ^= feedback1[3] ;

    enc_key_base[1][0] ^= random_data[0] ;
    enc_key_base[1][1] ^= random_data[1] ;
    enc_key_base[1][2] ^= random_data[0] ;
    enc_key_base[1][3] ^= random_data[1] ;
    enc_key_base[1][4] ^= feedback1[0] ;
    enc_key_base[1][5] ^= feedback1[1] ;
    enc_key_base[1][6] ^= feedback1[2] ;
    enc_key_base[1][7] ^= feedback1[3] ;

    enc_key_base[2][0] ^= feedback1[0] ;
    enc_key_base[2][1] ^= feedback1[1] ;
    enc_key_base[2][2] ^= feedback1[2] ;
    enc_key_base[2][3] ^= feedback1[3] ;
    enc_key_base[2][4] ^= random_data[0] ;
    enc_key_base[2][5] ^= random_data[1] ;
    enc_key_base[2][6] ^= random_data[0] ;
    enc_key_base[2][7] ^= random_data[1] ;

    enc_key_base[3][0] ^= feedback1[0] ;
    enc_key_base[3][1] ^= feedback1[1] ;
    enc_key_base[3][2] ^= feedback1[2] ;
    enc_key_base[3][3] ^= feedback1[3] ;
    enc_key_base[3][4] ^= random_data[0] ;
    enc_key_base[3][5] ^= random_data[1] ;
    enc_key_base[3][6] ^= random_data[0] ;
    enc_key_base[3][7] ^= random_data[1] ;

    enc_key_base[5][0] ^= feedback1[0] ;
    enc_key_base[5][1] ^= feedback1[1] ;
    enc_key_base[5][2] ^= feedback1[2] ;
    enc_key_base[5][3] ^= feedback1[3] ;
    enc_key_base[5][4] ^= random_data[0] ;
    enc_key_base[5][5] ^= random_data[1] ;
    enc_key_base[5][6] ^= access_cnt[0] ;
    enc_key_base[5][7] ^= access_cnt[1] ;

    enc_key_base[6][0] ^= feedback1[2] ;
    enc_key_base[6][1] ^= feedback1[3] ;
    enc_key_base[6][2] ^= random_data[0] ;
    enc_key_base[6][3] ^= random_data[1] ;
    enc_key_base[6][4] ^= access_cnt[0] ;
    enc_key_base[6][5] ^= access_cnt[1] ;
    enc_key_base[6][6] ^= time[0] ;
    enc_key_base[6][7] ^= time[1] ;

    enc_key_base[7][0] ^= random_data[0] ;
    enc_key_base[7][1] ^= random_data[1] ;
    enc_key_base[7][2] ^= access_cnt[0] ;
    enc_key_base[7][3] ^= access_cnt[1] ;
    enc_key_base[7][4] ^= time[0] ;
    enc_key_base[7][5] ^= time[1] ;
    enc_key_base[7][6] ^= feedback1[0] ;
    enc_key_base[7][7] ^= feedback1[1] ;

    enc_key_base[8][0] ^= access_cnt[0] ;
    enc_key_base[8][1] ^= access_cnt[1] ;
    enc_key_base[8][2] ^= time[0] ;
    enc_key_base[8][3] ^= time[1] ;
    enc_key_base[8][4] ^= feedback1[0] ;
    enc_key_base[8][5] ^= feedback1[1] ;
    enc_key_base[8][6] ^= feedback1[2] ;
    enc_key_base[8][7] ^= feedback1[3] ;

    enc_key_base[9][0] ^= time[0] ;
    enc_key_base[9][1] ^= time[1] ;
    enc_key_base[9][2] ^= feedback1[0] ;
    enc_key_base[9][3] ^= feedback1[1] ;
    enc_key_base[9][4] ^= feedback1[2] ;
    enc_key_base[9][5] ^= feedback1[3] ;
    enc_key_base[9][6] ^= random_data[0] ;
    enc_key_base[9][7] ^= random_data[1] ;

    enc_key_base[10][0] ^= time[0] ;
    enc_key_base[10][1] ^= time[1] ;
    enc_key_base[10][2] ^= access_cnt[0] ;
    enc_key_base[10][3] ^= access_cnt[1] ;
    enc_key_base[10][4] ^= random_data[0] ;
    enc_key_base[10][5] ^= random_data[1] ;
    enc_key_base[10][6] ^= feedback1[0] ;
    enc_key_base[10][7] ^= feedback1[1] ;

    enc_key_base[11][0] ^= access_cnt[0] ;
    enc_key_base[11][1] ^= access_cnt[1] ;
    enc_key_base[11][2] ^= random_data[0] ;
    enc_key_base[11][3] ^= random_data[1] ;
    enc_key_base[11][4] ^= feedback1[0] ;
    enc_key_base[11][5] ^= feedback1[1] ;
    enc_key_base[11][6] ^= feedback1[2] ;
    enc_key_base[11][7] ^= feedback1[3] ;

    enc_key_base[12][0] ^= random_data[0] ;
    enc_key_base[12][1] ^= random_data[1] ;
    enc_key_base[12][2] ^= feedback1[0] ;
    enc_key_base[12][3] ^= feedback1[1] ;
    enc_key_base[12][4] ^= feedback1[2] ;
    enc_key_base[12][5] ^= feedback1[3] ;
    enc_key_base[12][6] ^= time[0] ;
    enc_key_base[12][7] ^= time[1] ;

    enc_key_base[13][0] ^= feedback1[0] ;
    enc_key_base[13][1] ^= feedback1[1] ;
    enc_key_base[13][2] ^= feedback1[2] ;
    enc_key_base[13][3] ^= feedback1[3] ;
    enc_key_base[13][4] ^= time[0] ;
    enc_key_base[13][5] ^= time[1] ;
    enc_key_base[13][6] ^= access_cnt[0] ;
    enc_key_base[13][7] ^= access_cnt[1] ;

    enc_key_base[14][0] ^= feedback1[2] ;
    enc_key_base[14][1] ^= feedback1[3] ;
    enc_key_base[14][2] ^= time[0] ;
    enc_key_base[14][3] ^= time[1] ;
    enc_key_base[14][4] ^= access_cnt[0] ;
    enc_key_base[14][5] ^= access_cnt[1] ;
    enc_key_base[14][6] ^= random_data[0] ;
    enc_key_base[14][7] ^= random_data[1] ;

    enc_key_base[15][0] ^= access_cnt[0] ;
    enc_key_base[15][1] ^= access_cnt[1] ;
    enc_key_base[15][2] ^= random_data[0] ;
    enc_key_base[15][3] ^= random_data[1] ;
    enc_key_base[15][4] ^= time[0] ;
    enc_key_base[15][5] ^= time[1] ;
    enc_key_base[15][6] ^= feedback1[0] ;
    enc_key_base[15][7] ^= feedback1[1] ;

    enc_key_base[16][0] ^= random_data[0] ;
    enc_key_base[16][1] ^= random_data[1] ;
    enc_key_base[16][2] ^= time[0] ;
    enc_key_base[16][3] ^= time[1] ;
    enc_key_base[16][4] ^= feedback1[0] ;
    enc_key_base[16][5] ^= feedback1[1] ;
    enc_key_base[16][6] ^= feedback1[2] ;
    enc_key_base[16][7] ^= feedback1[3] ;

    enc_key_base[17][0] ^= time[0] ;
    enc_key_base[17][1] ^= time[1] ;
    enc_key_base[17][2] ^= feedback1[0] ;
    enc_key_base[17][3] ^= feedback1[1] ;
    enc_key_base[17][4] ^= feedback1[2] ;
    enc_key_base[17][5] ^= feedback1[3] ;
    enc_key_base[17][6] ^= access_cnt[0] ;
    enc_key_base[17][7] ^= access_cnt[1] ;

    enc_key_base[18][0] ^= feedback1[0] ;
    enc_key_base[18][1] ^= feedback1[1] ;
    enc_key_base[18][2] ^= feedback1[2] ;
    enc_key_base[18][3] ^= feedback1[3] ;
    enc_key_base[18][4] ^= access_cnt[0] ;
    enc_key_base[18][5] ^= access_cnt[1] ;
    enc_key_base[18][6] ^= random_data[0] ;
    enc_key_base[18][7] ^= random_data[1] ;

    enc_key_base[19][0] ^= feedback1[2] ;
    enc_key_base[19][1] ^= feedback1[3] ;
    enc_key_base[19][2] ^= access_cnt[0] ;
    enc_key_base[19][3] ^= access_cnt[1] ;
    enc_key_base[19][4] ^= random_data[0] ;
    enc_key_base[19][5] ^= random_data[1] ;
    enc_key_base[19][6] ^= time[0] ;
    enc_key_base[19][7] ^= time[1] ;

    // Memory Initial
    for ( i=0 ; i<8 ; i++ ) {
        enc_key_step[i] = 0x00 ;
        enc_key_step_1[i] = 0x00 ;
    }

    // Double / Normal Data Loading
    if ( double_enc ) {
        if ( ( (~xn_data[0] & 0xff) == random_data[0]) & (xn_data[7] == random_data[1]) ) {
			for (i=0 ; i<8 ; i++) yn_data[i] = xn_data[i] ;
        }
        else {
			for (i=0 ; i<8 ; i++) yn_data[i] = 0x00 ;
        }
    }
    else for ( i=0 ; i<8 ; i++ ) yn_data[i] = xn_data[i] ;

    for ( main_loop=0 ; main_loop<20 ; main_loop++ ) {
        // Source Open Encryption
        if ( main_loop < 4 ) {
            for ( sub_loop = 0 ; sub_loop < step_fin_sub ; sub_loop++ ) {
            	// Encryption Key Base/Step Loading
                temp = enc_key_step[0] ;
                for ( i=0 ; i<7 ; i++ ) enc_key_step[i] = ((enc_key_step[i]<<3)&0xf8) | ((enc_key_step[i+1]>>5)&0x07) ;
                enc_key_step[7] = ((enc_key_step[7]<<3)&0xf8) | ((temp>>5)&0x07) ;

                if ( sub_loop == 1 ) for ( i=0 ; i<8 ; i++ ) enc_key_step[i]   = enc_key_step_1[i] ;
                if ( sub_loop == 0 ) for ( i=0 ; i<8 ; i++ ) enc_key_step_1[i] = enc_key_base[main_loop][i] ;

                // Data Selection Signal
                data_sel = (enc_key_step[0]>>6) & 0x03 ;

                for ( i=0 ; i<4 ; i++ ) {
                    data_not[i] = ~yn_data[i+4] ;
                    data_not[i+4] = yn_data[i] ;
                    data_xor[i] = yn_data[i+4] ^ enc_key_step[i] ;
                    data_xor[i+4] = yn_data[i] ^ enc_key_step[i+4] ;
                }

                data_sft[3] = ((yn_data[7]<<7)&0x80) | ((yn_data[4]>>1)&0x7f) ;
                for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<7)&0x80) | ((yn_data[i+5]>>1)&0x7f) ;
                data_sft[7] = ((yn_data[3]<<7)&0x80) | ((yn_data[0]>>1)&0x7f) ;
                for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<7)&0x80) | ((yn_data[i+1]>>1)&0x7f) ;

                data_sfl[0] = ((yn_data[7]<<7)&0x80)|((yn_data[2]<<1)&0x7e)|((yn_data[3]>>7)&0x01) ;
                data_sfl[1] = ((yn_data[3]<<1)&0xc0)|((yn_data[0]>>2)&0x3f) ;
                data_sfl[2] = ((yn_data[0]<<6)&0xc0)|((yn_data[1]>>2)&0x20)|((yn_data[4]<<1)&0x1e)|((yn_data[5]>>7)&0x01) ;
                data_sfl[3] = ((yn_data[5]<<1)&0xf0)|((yn_data[6]<<2)&0x0c)|((yn_data[7]>>6)&0x03) ;
                data_sfl[4] = ((yn_data[7]<<2)&0xf8)|((yn_data[1]>>4)&0x07) ;
                data_sfl[5] = ((yn_data[1]<<4)&0xf0)|((yn_data[2]>>4)&0x0c)|((yn_data[5]>>1)&0x03) ;
                data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x7e)|((yn_data[3]>>4)&0x01) ;
                data_sfl[7] = ((yn_data[3]<<4)&0xf0)|((yn_data[4]>>4)&0x0f) ;

                // yn_data Signal
                if ( (enc_step_u<<(main_loop)) & 0x08 ) {
                    switch( data_sel ) {
                        case 0x00 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_not[i] ; break ;
                        case 0x01 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_xor[i] ; break ;
                        case 0x02 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sft[i] ; break ;
                        case 0x03 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sfl[i] ; break ;
                        default   : break ;
                    }
                }
            }
        }//Source Open End

		else {

            loop_block = ( main_loop - 4 ) / 4 ;
            for ( sub_loop = 0 ; sub_loop < step_fin_sub ; sub_loop++ ) {

                // Encryption Key Base/Step Loading
                temp = enc_key_step[0] ;
                for ( i=0 ; i<7 ; i++ ) enc_key_step[i] = ((enc_key_step[i]<<3)&0xf8) | ((enc_key_step[i+1]>>5)&0x07) ;
                enc_key_step[7] = ((enc_key_step[7]<<3)&0xf8) | ((temp>>5)&0x07) ;

                if ( sub_loop == 1 ) for ( i=0 ; i<8 ; i++ ) enc_key_step[i]   = enc_key_step_1[i] ;
                if ( sub_loop == 0 ) for ( i=0 ; i<8 ; i++ ) enc_key_step_1[i] = enc_key_base[main_loop][i] ;

                // Data Selection Signal
                data_sel = ((enc_key_step[0]>>6) & 0x02) | ((enc_key_step[0]>>5) & 0x01) ;

                for ( i=0 ; i<4 ; i++ ) {
                    data_not[i] = ~yn_data[i+4] ;
                    data_not[i+4] = yn_data[i] ;
                    data_xor[i] = yn_data[i+4] ^ enc_key_step[i] ;
                    data_xor[i+4] = yn_data[i] ^ enc_key_step[i+4] ;
                }

                if ( loop_block==0 ) {
                    data_sft[3] = ((yn_data[7]<<1)&0xfe) | ((yn_data[4]>>7)&0x01) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<1)&0xfe) | ((yn_data[i+5]>>7)&0x01) ;
                    data_sft[7] = ((yn_data[3]<<1)&0xfe) | ((yn_data[0]>>7)&0x01) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<1)&0xfe) | ((yn_data[i+1]>>7)&0x01) ;

                    data_sfl[0] = ((yn_data[3]<<6)&0xc0)|((yn_data[4]>>2)&0x20)|((yn_data[6]   )&0x1c)|((yn_data[2]>>4)&0x03) ;
                    data_sfl[1] = ((yn_data[2]<<4)&0x80)|((yn_data[4]<<3)&0x70)|((yn_data[1]>>3)&0x0e)|((yn_data[7]   )&0x01) ;
                    data_sfl[2] = ((yn_data[0]<<3)&0xe0)|((yn_data[6]<<3)&0x18)|((yn_data[7]>>5)&0x04)|((yn_data[5]>>4)&0x03) ;
                    data_sfl[3] = ((yn_data[5]<<4)&0x80)|((yn_data[0]>>1)&0x70)|((yn_data[1]<<3)&0x08)|((yn_data[2]>>5)&0x06)|((yn_data[6]>>7)&0x01) ;
                    data_sfl[4] = ((yn_data[6]<<1)&0xc0)|((yn_data[7]<<2)&0x38)|((yn_data[0]<<1)&0x06)|((yn_data[1]>>7)&0x01) ;
                    data_sfl[5] = ((yn_data[5]<<5)&0xe0)|((yn_data[1]<<1)&0x1c)|((yn_data[4]>>5)&0x03) ;
                    data_sfl[6] = ((yn_data[4]<<3)&0x80)|((yn_data[2]<<4)&0x70)|((yn_data[4]<<3)&0x08)|((yn_data[5]>>5)&0x06)|((yn_data[3]>>4)&0x01) ;
                    data_sfl[7] = ((yn_data[3]<<4)&0xc0)|((yn_data[7]>>1)&0x38)|((yn_data[3]>>5)&0x07) ;
                }

                if ( loop_block==1 ) {

                    for ( i=0 ; i<3 ; i++ ) data_sft[i]   = ((yn_data[i+4]<<3)&0xf8) | ((yn_data[i+5]>>5)&0x07) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<3)&0xf8)   | ((yn_data[i+1]>>5)&0x07) ;
                    data_sft[3] = ((yn_data[7]<<3)&0xf8)|((yn_data[4]>>5)&0x07) ;
                    data_sft[7] = ((yn_data[3]<<3)&0xf8)|((yn_data[0]>>5)&0x07) ;
                    data_sfl[0] = ((yn_data[7]<<4)&0xf0)|((yn_data[4]>>1)&0x0f) ;
                    data_sfl[1] = ((yn_data[4]<<7)&0x80)|((yn_data[6]<<1)&0x7c)|((yn_data[0]>>6)&0x03) ;
                    data_sfl[2] = ((yn_data[0]<<2)&0xe0)|((yn_data[3]<<3)&0x18)|((yn_data[4]>>5)&0x07) ;
                    data_sfl[3] = ((yn_data[1]<<2)&0xf8)|((yn_data[5]>>5)&0x07) ;
                    data_sfl[4] = ((yn_data[5]<<3)&0xc0)|((yn_data[1]<<5)&0x20)|((yn_data[2]>>3)&0x1e)|((yn_data[3]>>6)&0x01) ;
                    data_sfl[5] = ((yn_data[3]<<2)&0xf0)|((yn_data[0]<<1)&0x0e)|((yn_data[1]>>7)&0x01) ;
                    data_sfl[6] = ((yn_data[1]<<1)&0x80)|((yn_data[6]<<6)&0x40)|((yn_data[7]>>2)&0x3c)|((yn_data[2]>>2)&0x03) ;
                    data_sfl[7] = ((yn_data[2]<<6)&0xc0)|((yn_data[3]>>2)&0x20)|((yn_data[5]<<2)&0x1c)|((yn_data[6]>>6)&0x03) ;
                }

                if ( loop_block==2 ) {
                    data_sft[3] = ((yn_data[7]<<5)&0xe0) | ((yn_data[4]>>3)&0x1f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<5)&0xe0) | ((yn_data[i+5]>>3)&0x1f) ;
                    data_sft[7] = ((yn_data[3]<<5)&0xe0) | ((yn_data[0]>>3)&0x1f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<5)&0xe0) | ((yn_data[i+1]>>3)&0x1f) ;

                    data_sfl[0] = ((yn_data[2]<<5)&0xe0)|((yn_data[3]>>3)&0x1e)|((yn_data[7]>>7)&0x01) ;
                    data_sfl[1] = ((yn_data[7]<<1)&0xfc)|((yn_data[4]>>3)&0x03) ;
                    data_sfl[2] = ((yn_data[4]<<5)&0xe0)|((yn_data[5]>>3)&0x18)|((yn_data[1]<<1)&0x06)|((yn_data[2]>>7)&0x01) ;
                    data_sfl[3] = ((yn_data[2]<<1)&0xf0)|((yn_data[6]>>3)&0x0f) ;
                    data_sfl[4] = ((yn_data[6]<<5)&0xe0)|((yn_data[7]<<4)&0x10)|((yn_data[0]>>4)&0x0f) ;
                    data_sfl[5] = ((yn_data[0]<<4)&0xe0)|((yn_data[5]>>1)&0x1f) ;
                    data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x40)|((yn_data[3]<<2)&0x3c)|((yn_data[4]>>6)&0x03) ;
                    data_sfl[7] = ((yn_data[4]<<2)&0x80)|((yn_data[0]<<6)&0x40)|((yn_data[1]>>2)&0x3f) ;
                }

                if ( loop_block==3 ) {
                    data_sft[3] = ((yn_data[7]<<7)&0x80) | ((yn_data[4]>>1)&0x7f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<7)&0x80) | ((yn_data[i+5]>>1)&0x7f) ;
                    data_sft[7] = ((yn_data[3]<<7)&0x80) | ((yn_data[0]>>1)&0x7f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<7)&0x80) | ((yn_data[i+1]>>1)&0x7f) ;

                    data_sfl[0] = ((yn_data[7]<<7)&0x80)|((yn_data[2]<<1)&0x7e)|((yn_data[3]>>7)&0x01) ;
                    data_sfl[1] = ((yn_data[3]<<1)&0xc0)|((yn_data[0]>>2)&0x3f) ;
                    data_sfl[2] = ((yn_data[0]<<6)&0xc0)|((yn_data[1]>>2)&0x20)|((yn_data[4]<<1)&0x1e)|((yn_data[5]>>7)&0x01) ;
                    data_sfl[3] = ((yn_data[5]<<1)&0xf0)|((yn_data[6]<<2)&0x0c)|((yn_data[7]>>6)&0x03) ;
                    data_sfl[4] = ((yn_data[7]<<2)&0xf8)|((yn_data[1]>>4)&0x07) ;
                    data_sfl[5] = ((yn_data[1]<<4)&0xf0)|((yn_data[2]>>4)&0x0c)|((yn_data[5]>>1)&0x03) ;
                    data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x7e)|((yn_data[3]>>4)&0x01) ;
                    data_sfl[7] = ((yn_data[3]<<4)&0xf0)|((yn_data[4]>>4)&0x0f) ;
                }

                // yn_data Signal
//                if ( (enc_step<<main_loop-4) & 0x8000 ) {
                if ( ((enc_step<<main_loop)-4) & 0x8000 ) {
                    switch( data_sel ) {
                        case 0x00 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_not[i] ; break ;
                        case 0x01 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_xor[i] ; break ;
                        case 0x02 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sft[i] ; break ;
                        case 0x03 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sfl[i] ; break ;
                        default   : break ;
                    }
                }
            }
        }// Neowine Original Encryption End

    }

    // Bypass Setting
    if ( bypass ) for ( i=0; i<8; i++ ) yn_data[i] = xn_data[i] ^ 0x01 ;

    // Feedback Setting
    if ( feedback_en ) {
        feedback1[0] = yn_data[4] ;
        feedback1[1] = yn_data[5] ;
        feedback1[2] = yn_data[0] ;
        feedback1[3] = yn_data[1] ;
    }
}

void ISPM _encrypt_enc2(unsigned char enc_step_u, unsigned short enc_step, unsigned char *random_data, unsigned char *xn_data, unsigned char *yn_data)
{
    unsigned char enc_key_base[20][8];
    unsigned char enc_key_step_1[8];
    unsigned char enc_key_step[8];
    unsigned char data_sel;
    unsigned char data_not[8];
    unsigned char data_xor[8];
    unsigned char data_sft[8];
    unsigned char data_sfl[8];
    unsigned char access_cnt[2];
    unsigned char shift_addr;
    unsigned char time[2];
    unsigned char bypass, enc_enable, double_enc, time_en, feedback_en, random_en, temp;

    int i, j, step_fin_sub, main_loop, sub_loop, loop_block;

    // Encryption Option Setting
    if (sub_address == 0x80) {
        bypass = 1;
        enc_enable = 0;
        double_enc = 0;
        time_en = 0;
        feedback_en = 0;
        random_en = 0;
    }
    else {
        bypass = 0;
        if (sub_address & 0x80) enc_enable = 1;
        else enc_enable = 0;
        if (sub_address & 0x40) double_enc = 1;
        else double_enc = 0;
        if (sub_address & 0x20) time_en = 1;
        else time_en = 0;
        if (sub_address & 0x10) feedback_en = 1;
        else feedback_en = 0;
        if (sub_address & 0x08) random_en = 1;
        else random_en = 0;
    }
    // Encryption Depth Parameter Generation
    step_fin_sub = (((int)(sub_address & 0x07)) + 1) * 8 ;

    // Feedback Clear
    if ( bypass | !feedback_en ) for ( i=0 ; i<4 ; i++ ) feedback2[i] = 0x00 ;

    // Shift Encryption Parameter Generation
    shift_addr = ( shift_reg & 0x80 ) ? shift_reg : 0x00 ;

    // Time Data Setting
    if ( time_en ) {
        time[0] = 0xc0 | ( ( month << 2 ) & 0x3c ) | ( ( day >> 3 ) & 0x03 ) ;
        time[1] = ( ( day << 5) & 0xe0 ) | ( hour & 0x1f ) ;
    }
    else {
        time[0] = 0x00 ;
        time[1] = 0x00 ;
    }

	// Random Data Generation
    if ( !random_en ) {
        random_data[0] = 0x00 ;
        random_data[1] = 0x00 ;
    }


#ifdef NoCountAccess
	access_count2 = 0x0000 ;
#else
    // Access Count Generation
    if ( bypass) access_count2 = 0x0000 ;
    else access_count2++ ;
#endif

    access_cnt[0] = (unsigned char)((access_count2>>8) & 0x00ff) ;
    access_cnt[1] = (unsigned char) (access_count2&0x00ff) ;

	// Feedback Clear
    if ( bypass | !feedback_en ) for ( i=0 ; i<4 ; i++ ) feedback2[i] = 0x00 ;

    // Encryption Key Setting
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i][j] = user_serial[j] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+4][j] = super_serial[j] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+8][j] = super_serial[j+8] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+12][j] = super_serial[j+4] ;
    for (i=0; i<4; i++) for (j=0; j<8; j++) enc_key_base[i+16][j] = user_serial[j] ;

	enc_key_base[0][0] ^= shift_addr ;
    enc_key_base[0][1] ^= shift_addr ;
    enc_key_base[0][2] ^= random_data[0] ;
    enc_key_base[0][3] ^= random_data[1] ;
    enc_key_base[0][4] ^= feedback2[0] ;
    enc_key_base[0][5] ^= feedback2[1] ;
    enc_key_base[0][6] ^= feedback2[2] ;
    enc_key_base[0][7] ^= feedback2[3] ;

    enc_key_base[1][0] ^= random_data[0] ;
    enc_key_base[1][1] ^= random_data[1] ;
    enc_key_base[1][2] ^= random_data[0] ;
    enc_key_base[1][3] ^= random_data[1] ;
    enc_key_base[1][4] ^= feedback2[0] ;
    enc_key_base[1][5] ^= feedback2[1] ;
    enc_key_base[1][6] ^= feedback2[2] ;
    enc_key_base[1][7] ^= feedback2[3] ;

    enc_key_base[2][0] ^= feedback2[0] ;
    enc_key_base[2][1] ^= feedback2[1] ;
    enc_key_base[2][2] ^= feedback2[2] ;
    enc_key_base[2][3] ^= feedback2[3] ;
    enc_key_base[2][4] ^= random_data[0] ;
    enc_key_base[2][5] ^= random_data[1] ;
    enc_key_base[2][6] ^= random_data[0] ;
    enc_key_base[2][7] ^= random_data[1] ;

    enc_key_base[3][0] ^= feedback2[0] ;
    enc_key_base[3][1] ^= feedback2[1] ;
    enc_key_base[3][2] ^= feedback2[2] ;
    enc_key_base[3][3] ^= feedback2[3] ;
    enc_key_base[3][4] ^= random_data[0] ;
    enc_key_base[3][5] ^= random_data[1] ;
    enc_key_base[3][6] ^= random_data[0] ;
    enc_key_base[3][7] ^= random_data[1] ;

    enc_key_base[5][0] ^= feedback2[0] ;
    enc_key_base[5][1] ^= feedback2[1] ;
    enc_key_base[5][2] ^= feedback2[2] ;
    enc_key_base[5][3] ^= feedback2[3] ;
    enc_key_base[5][4] ^= random_data[0] ;
    enc_key_base[5][5] ^= random_data[1] ;
    enc_key_base[5][6] ^= access_cnt[0] ;
    enc_key_base[5][7] ^= access_cnt[1] ;

    enc_key_base[6][0] ^= feedback2[2] ;
    enc_key_base[6][1] ^= feedback2[3] ;
    enc_key_base[6][2] ^= random_data[0] ;
    enc_key_base[6][3] ^= random_data[1] ;
    enc_key_base[6][4] ^= access_cnt[0] ;
    enc_key_base[6][5] ^= access_cnt[1] ;
    enc_key_base[6][6] ^= time[0] ;
    enc_key_base[6][7] ^= time[1] ;

    enc_key_base[7][0] ^= random_data[0] ;
    enc_key_base[7][1] ^= random_data[1] ;
    enc_key_base[7][2] ^= access_cnt[0] ;
    enc_key_base[7][3] ^= access_cnt[1] ;
    enc_key_base[7][4] ^= time[0] ;
    enc_key_base[7][5] ^= time[1] ;
    enc_key_base[7][6] ^= feedback2[0] ;
    enc_key_base[7][7] ^= feedback2[1] ;

    enc_key_base[8][0] ^= access_cnt[0] ;
    enc_key_base[8][1] ^= access_cnt[1] ;
    enc_key_base[8][2] ^= time[0] ;
    enc_key_base[8][3] ^= time[1] ;
    enc_key_base[8][4] ^= feedback2[0] ;
    enc_key_base[8][5] ^= feedback2[1] ;
    enc_key_base[8][6] ^= feedback2[2] ;
    enc_key_base[8][7] ^= feedback2[3] ;

    enc_key_base[9][0] ^= time[0] ;
    enc_key_base[9][1] ^= time[1] ;
    enc_key_base[9][2] ^= feedback2[0] ;
    enc_key_base[9][3] ^= feedback2[1] ;
    enc_key_base[9][4] ^= feedback2[2] ;
    enc_key_base[9][5] ^= feedback2[3] ;
    enc_key_base[9][6] ^= random_data[0] ;
    enc_key_base[9][7] ^= random_data[1] ;

    enc_key_base[10][0] ^= time[0] ;
    enc_key_base[10][1] ^= time[1] ;
    enc_key_base[10][2] ^= access_cnt[0] ;
    enc_key_base[10][3] ^= access_cnt[1] ;
    enc_key_base[10][4] ^= random_data[0] ;
    enc_key_base[10][5] ^= random_data[1] ;
    enc_key_base[10][6] ^= feedback2[0] ;
    enc_key_base[10][7] ^= feedback2[1] ;

    enc_key_base[11][0] ^= access_cnt[0] ;
    enc_key_base[11][1] ^= access_cnt[1] ;
    enc_key_base[11][2] ^= random_data[0] ;
    enc_key_base[11][3] ^= random_data[1] ;
    enc_key_base[11][4] ^= feedback2[0] ;
    enc_key_base[11][5] ^= feedback2[1] ;
    enc_key_base[11][6] ^= feedback2[2] ;
    enc_key_base[11][7] ^= feedback2[3] ;

    enc_key_base[12][0] ^= random_data[0] ;
    enc_key_base[12][1] ^= random_data[1] ;
    enc_key_base[12][2] ^= feedback2[0] ;
    enc_key_base[12][3] ^= feedback2[1] ;
    enc_key_base[12][4] ^= feedback2[2] ;
    enc_key_base[12][5] ^= feedback2[3] ;
    enc_key_base[12][6] ^= time[0] ;
    enc_key_base[12][7] ^= time[1] ;

    enc_key_base[13][0] ^= feedback2[0] ;
    enc_key_base[13][1] ^= feedback2[1] ;
    enc_key_base[13][2] ^= feedback2[2] ;
    enc_key_base[13][3] ^= feedback2[3] ;
    enc_key_base[13][4] ^= time[0] ;
    enc_key_base[13][5] ^= time[1] ;
    enc_key_base[13][6] ^= access_cnt[0] ;
    enc_key_base[13][7] ^= access_cnt[1] ;

    enc_key_base[14][0] ^= feedback2[2] ;
    enc_key_base[14][1] ^= feedback2[3] ;
    enc_key_base[14][2] ^= time[0] ;
    enc_key_base[14][3] ^= time[1] ;
    enc_key_base[14][4] ^= access_cnt[0] ;
    enc_key_base[14][5] ^= access_cnt[1] ;
    enc_key_base[14][6] ^= random_data[0] ;
    enc_key_base[14][7] ^= random_data[1] ;

    enc_key_base[15][0] ^= access_cnt[0] ;
    enc_key_base[15][1] ^= access_cnt[1] ;
    enc_key_base[15][2] ^= random_data[0] ;
    enc_key_base[15][3] ^= random_data[1] ;
    enc_key_base[15][4] ^= time[0] ;
    enc_key_base[15][5] ^= time[1] ;
    enc_key_base[15][6] ^= feedback2[0] ;
    enc_key_base[15][7] ^= feedback2[1] ;

    enc_key_base[16][0] ^= random_data[0] ;
    enc_key_base[16][1] ^= random_data[1] ;
    enc_key_base[16][2] ^= time[0] ;
    enc_key_base[16][3] ^= time[1] ;
    enc_key_base[16][4] ^= feedback2[0] ;
    enc_key_base[16][5] ^= feedback2[1] ;
    enc_key_base[16][6] ^= feedback2[2] ;
    enc_key_base[16][7] ^= feedback2[3] ;

    enc_key_base[17][0] ^= time[0] ;
    enc_key_base[17][1] ^= time[1] ;
    enc_key_base[17][2] ^= feedback2[0] ;
    enc_key_base[17][3] ^= feedback2[1] ;
    enc_key_base[17][4] ^= feedback2[2] ;
    enc_key_base[17][5] ^= feedback2[3] ;
    enc_key_base[17][6] ^= access_cnt[0] ;
    enc_key_base[17][7] ^= access_cnt[1] ;

    enc_key_base[18][0] ^= feedback2[0] ;
    enc_key_base[18][1] ^= feedback2[1] ;
    enc_key_base[18][2] ^= feedback2[2] ;
    enc_key_base[18][3] ^= feedback2[3] ;
    enc_key_base[18][4] ^= access_cnt[0] ;
    enc_key_base[18][5] ^= access_cnt[1] ;
    enc_key_base[18][6] ^= random_data[0] ;
    enc_key_base[18][7] ^= random_data[1] ;

    enc_key_base[19][0] ^= feedback2[2] ;
    enc_key_base[19][1] ^= feedback2[3] ;
    enc_key_base[19][2] ^= access_cnt[0] ;
    enc_key_base[19][3] ^= access_cnt[1] ;
    enc_key_base[19][4] ^= random_data[0] ;
    enc_key_base[19][5] ^= random_data[1] ;
    enc_key_base[19][6] ^= time[0] ;
    enc_key_base[19][7] ^= time[1] ;

    // Memory Initial
    for ( i=0 ; i<8 ; i++ ) {
        enc_key_step[i] = 0x00 ;
        enc_key_step_1[i] = 0x00 ;
    }

    // Double / Normal Data Loading
    if ( double_enc ) {
        if ( ( (~xn_data[0] & 0xff) == random_data[0]) & (xn_data[7] == random_data[1]) ) {
			for (i=0 ; i<8 ; i++) yn_data[i] = xn_data[i] ;
        }
        else {
			for (i=0 ; i<8 ; i++) yn_data[i] = 0x00 ;
        }
    }
    else for ( i=0 ; i<8 ; i++ ) yn_data[i] = xn_data[i] ;

    for ( main_loop=0 ; main_loop<20 ; main_loop++ ) {
        // Source Open Encryption
        if ( main_loop < 4 ) {
            for ( sub_loop = 0 ; sub_loop < step_fin_sub ; sub_loop++ ) {
            	// Encryption Key Base/Step Loading
                temp = enc_key_step[0] ;
                for ( i=0 ; i<7 ; i++ ) enc_key_step[i] = ((enc_key_step[i]<<3)&0xf8) | ((enc_key_step[i+1]>>5)&0x07) ;
                enc_key_step[7] = ((enc_key_step[7]<<3)&0xf8) | ((temp>>5)&0x07) ;

                if ( sub_loop == 1 ) for ( i=0 ; i<8 ; i++ ) enc_key_step[i]   = enc_key_step_1[i] ;
                if ( sub_loop == 0 ) for ( i=0 ; i<8 ; i++ ) enc_key_step_1[i] = enc_key_base[main_loop][i] ;

                // Data Selection Signal
                data_sel = (enc_key_step[0]>>6) & 0x03 ;

                for ( i=0 ; i<4 ; i++ ) {
                    data_not[i] = ~yn_data[i+4] ;
                    data_not[i+4] = yn_data[i] ;
                    data_xor[i] = yn_data[i+4] ^ enc_key_step[i] ;
                    data_xor[i+4] = yn_data[i] ^ enc_key_step[i+4] ;
                }

                data_sft[3] = ((yn_data[7]<<7)&0x80) | ((yn_data[4]>>1)&0x7f) ;
                for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<7)&0x80) | ((yn_data[i+5]>>1)&0x7f) ;
                data_sft[7] = ((yn_data[3]<<7)&0x80) | ((yn_data[0]>>1)&0x7f) ;
                for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<7)&0x80) | ((yn_data[i+1]>>1)&0x7f) ;

                data_sfl[0] = ((yn_data[7]<<7)&0x80)|((yn_data[2]<<1)&0x7e)|((yn_data[3]>>7)&0x01) ;
                data_sfl[1] = ((yn_data[3]<<1)&0xc0)|((yn_data[0]>>2)&0x3f) ;
                data_sfl[2] = ((yn_data[0]<<6)&0xc0)|((yn_data[1]>>2)&0x20)|((yn_data[4]<<1)&0x1e)|((yn_data[5]>>7)&0x01) ;
                data_sfl[3] = ((yn_data[5]<<1)&0xf0)|((yn_data[6]<<2)&0x0c)|((yn_data[7]>>6)&0x03) ;
                data_sfl[4] = ((yn_data[7]<<2)&0xf8)|((yn_data[1]>>4)&0x07) ;
                data_sfl[5] = ((yn_data[1]<<4)&0xf0)|((yn_data[2]>>4)&0x0c)|((yn_data[5]>>1)&0x03) ;
                data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x7e)|((yn_data[3]>>4)&0x01) ;
                data_sfl[7] = ((yn_data[3]<<4)&0xf0)|((yn_data[4]>>4)&0x0f) ;

                // yn_data Signal
                if ( (enc_step_u<<(main_loop)) & 0x08 ) {
                    switch( data_sel ) {
                        case 0x00 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_not[i] ; break ;
                        case 0x01 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_xor[i] ; break ;
                        case 0x02 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sft[i] ; break ;
                        case 0x03 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sfl[i] ; break ;
                        default   : break ;
                    }
                }
            }
        }//Source Open End

		else {

            loop_block = ( main_loop - 4 ) / 4 ;
            for ( sub_loop = 0 ; sub_loop < step_fin_sub ; sub_loop++ ) {

                // Encryption Key Base/Step Loading
                temp = enc_key_step[0] ;
                for ( i=0 ; i<7 ; i++ ) enc_key_step[i] = ((enc_key_step[i]<<3)&0xf8) | ((enc_key_step[i+1]>>5)&0x07) ;
                enc_key_step[7] = ((enc_key_step[7]<<3)&0xf8) | ((temp>>5)&0x07) ;

                if ( sub_loop == 1 ) for ( i=0 ; i<8 ; i++ ) enc_key_step[i]   = enc_key_step_1[i] ;
                if ( sub_loop == 0 ) for ( i=0 ; i<8 ; i++ ) enc_key_step_1[i] = enc_key_base[main_loop][i] ;

                // Data Selection Signal
                data_sel = ((enc_key_step[0]>>6) & 0x02) | ((enc_key_step[0]>>5) & 0x01) ;

                for ( i=0 ; i<4 ; i++ ) {
                    data_not[i] = ~yn_data[i+4] ;
                    data_not[i+4] = yn_data[i] ;
                    data_xor[i] = yn_data[i+4] ^ enc_key_step[i] ;
                    data_xor[i+4] = yn_data[i] ^ enc_key_step[i+4] ;
                }

                if ( loop_block==0 ) {
                    data_sft[3] = ((yn_data[7]<<1)&0xfe) | ((yn_data[4]>>7)&0x01) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<1)&0xfe) | ((yn_data[i+5]>>7)&0x01) ;
                    data_sft[7] = ((yn_data[3]<<1)&0xfe) | ((yn_data[0]>>7)&0x01) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<1)&0xfe) | ((yn_data[i+1]>>7)&0x01) ;

                    data_sfl[0] = ((yn_data[3]<<6)&0xc0)|((yn_data[4]>>2)&0x20)|((yn_data[6]   )&0x1c)|((yn_data[2]>>4)&0x03) ;
                    data_sfl[1] = ((yn_data[2]<<4)&0x80)|((yn_data[4]<<3)&0x70)|((yn_data[1]>>3)&0x0e)|((yn_data[7]   )&0x01) ;
                    data_sfl[2] = ((yn_data[0]<<3)&0xe0)|((yn_data[6]<<3)&0x18)|((yn_data[7]>>5)&0x04)|((yn_data[5]>>4)&0x03) ;
                    data_sfl[3] = ((yn_data[5]<<4)&0x80)|((yn_data[0]>>1)&0x70)|((yn_data[1]<<3)&0x08)|((yn_data[2]>>5)&0x06)|((yn_data[6]>>7)&0x01) ;
                    data_sfl[4] = ((yn_data[6]<<1)&0xc0)|((yn_data[7]<<2)&0x38)|((yn_data[0]<<1)&0x06)|((yn_data[1]>>7)&0x01) ;
                    data_sfl[5] = ((yn_data[5]<<5)&0xe0)|((yn_data[1]<<1)&0x1c)|((yn_data[4]>>5)&0x03) ;
                    data_sfl[6] = ((yn_data[4]<<3)&0x80)|((yn_data[2]<<4)&0x70)|((yn_data[4]<<3)&0x08)|((yn_data[5]>>5)&0x06)|((yn_data[3]>>4)&0x01) ;
                    data_sfl[7] = ((yn_data[3]<<4)&0xc0)|((yn_data[7]>>1)&0x38)|((yn_data[3]>>5)&0x07) ;
                }

                if ( loop_block==1 ) {

                    for ( i=0 ; i<3 ; i++ ) data_sft[i]   = ((yn_data[i+4]<<3)&0xf8) | ((yn_data[i+5]>>5)&0x07) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<3)&0xf8)   | ((yn_data[i+1]>>5)&0x07) ;
                    data_sft[3] = ((yn_data[7]<<3)&0xf8)|((yn_data[4]>>5)&0x07) ;
                    data_sft[7] = ((yn_data[3]<<3)&0xf8)|((yn_data[0]>>5)&0x07) ;
                    data_sfl[0] = ((yn_data[7]<<4)&0xf0)|((yn_data[4]>>1)&0x0f) ;
                    data_sfl[1] = ((yn_data[4]<<7)&0x80)|((yn_data[6]<<1)&0x7c)|((yn_data[0]>>6)&0x03) ;
                    data_sfl[2] = ((yn_data[0]<<2)&0xe0)|((yn_data[3]<<3)&0x18)|((yn_data[4]>>5)&0x07) ;
                    data_sfl[3] = ((yn_data[1]<<2)&0xf8)|((yn_data[5]>>5)&0x07) ;
                    data_sfl[4] = ((yn_data[5]<<3)&0xc0)|((yn_data[1]<<5)&0x20)|((yn_data[2]>>3)&0x1e)|((yn_data[3]>>6)&0x01) ;
                    data_sfl[5] = ((yn_data[3]<<2)&0xf0)|((yn_data[0]<<1)&0x0e)|((yn_data[1]>>7)&0x01) ;
                    data_sfl[6] = ((yn_data[1]<<1)&0x80)|((yn_data[6]<<6)&0x40)|((yn_data[7]>>2)&0x3c)|((yn_data[2]>>2)&0x03) ;
                    data_sfl[7] = ((yn_data[2]<<6)&0xc0)|((yn_data[3]>>2)&0x20)|((yn_data[5]<<2)&0x1c)|((yn_data[6]>>6)&0x03) ;
                }

                if ( loop_block==2 ) {
                    data_sft[3] = ((yn_data[7]<<5)&0xe0) | ((yn_data[4]>>3)&0x1f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<5)&0xe0) | ((yn_data[i+5]>>3)&0x1f) ;
                    data_sft[7] = ((yn_data[3]<<5)&0xe0) | ((yn_data[0]>>3)&0x1f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<5)&0xe0) | ((yn_data[i+1]>>3)&0x1f) ;

                    data_sfl[0] = ((yn_data[2]<<5)&0xe0)|((yn_data[3]>>3)&0x1e)|((yn_data[7]>>7)&0x01) ;
                    data_sfl[1] = ((yn_data[7]<<1)&0xfc)|((yn_data[4]>>3)&0x03) ;
                    data_sfl[2] = ((yn_data[4]<<5)&0xe0)|((yn_data[5]>>3)&0x18)|((yn_data[1]<<1)&0x06)|((yn_data[2]>>7)&0x01) ;
                    data_sfl[3] = ((yn_data[2]<<1)&0xf0)|((yn_data[6]>>3)&0x0f) ;
                    data_sfl[4] = ((yn_data[6]<<5)&0xe0)|((yn_data[7]<<4)&0x10)|((yn_data[0]>>4)&0x0f) ;
                    data_sfl[5] = ((yn_data[0]<<4)&0xe0)|((yn_data[5]>>1)&0x1f) ;
                    data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x40)|((yn_data[3]<<2)&0x3c)|((yn_data[4]>>6)&0x03) ;
                    data_sfl[7] = ((yn_data[4]<<2)&0x80)|((yn_data[0]<<6)&0x40)|((yn_data[1]>>2)&0x3f) ;
                }

                if ( loop_block==3 ) {
                    data_sft[3] = ((yn_data[7]<<7)&0x80) | ((yn_data[4]>>1)&0x7f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i] = ((yn_data[i+4]<<7)&0x80) | ((yn_data[i+5]>>1)&0x7f) ;
                    data_sft[7] = ((yn_data[3]<<7)&0x80) | ((yn_data[0]>>1)&0x7f) ;
                    for ( i=0 ; i<3 ; i++ ) data_sft[i+4] = ((yn_data[i]<<7)&0x80) | ((yn_data[i+1]>>1)&0x7f) ;

                    data_sfl[0] = ((yn_data[7]<<7)&0x80)|((yn_data[2]<<1)&0x7e)|((yn_data[3]>>7)&0x01) ;
                    data_sfl[1] = ((yn_data[3]<<1)&0xc0)|((yn_data[0]>>2)&0x3f) ;
                    data_sfl[2] = ((yn_data[0]<<6)&0xc0)|((yn_data[1]>>2)&0x20)|((yn_data[4]<<1)&0x1e)|((yn_data[5]>>7)&0x01) ;
                    data_sfl[3] = ((yn_data[5]<<1)&0xf0)|((yn_data[6]<<2)&0x0c)|((yn_data[7]>>6)&0x03) ;
                    data_sfl[4] = ((yn_data[7]<<2)&0xf8)|((yn_data[1]>>4)&0x07) ;
                    data_sfl[5] = ((yn_data[1]<<4)&0xf0)|((yn_data[2]>>4)&0x0c)|((yn_data[5]>>1)&0x03) ;
                    data_sfl[6] = ((yn_data[5]<<7)&0x80)|((yn_data[6]>>1)&0x7e)|((yn_data[3]>>4)&0x01) ;
                    data_sfl[7] = ((yn_data[3]<<4)&0xf0)|((yn_data[4]>>4)&0x0f) ;
                }

                // yn_data Signal
//                if ( (enc_step<<main_loop-4) & 0x8000 ) {
                if ( ((enc_step<<main_loop)-4) & 0x8000 ) {
                    switch( data_sel ) {
                        case 0x00 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_not[i] ; break ;
                        case 0x01 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_xor[i] ; break ;
                        case 0x02 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sft[i] ; break ;
                        case 0x03 : for ( i=0 ; i<8 ; i++ ) yn_data[i] = data_sfl[i] ; break ;
                        default   : break ;
                    }
                }
            }
        }// Neowine Original Encryption End

    }

    // Bypass Setting
    if ( bypass ) for ( i=0; i<8; i++ ) yn_data[i] = xn_data[i] ^ 0x01 ;

    // Feedback Setting
    if ( feedback_en ) {
        feedback2[0] = yn_data[4] ;
        feedback2[1] = yn_data[5] ;
        feedback2[2] = yn_data[0] ;
        feedback2[3] = yn_data[1] ;
    }
}

UINT Calculation_TestA(BYTE abCalStatus, UINT anCal_A, UINT anCal_B)
{
	UINT nCal_Buf=0;

	switch(abCalStatus) {
		case 0 : nCal_Buf=(anCal_A+anCal_B); break;
		case 1 : nCal_Buf=(anCal_A-anCal_B); break;
		case 2 : nCal_Buf=(anCal_A*anCal_B); break;
		case 3 : nCal_Buf=(anCal_A/anCal_B); break;
	}
	return nCal_Buf;
}

UINT ISPM Calculation_TestB(BYTE abCalStatus, UINT anCal_A, UINT anCal_B)
{
	UINT nCal_Buf=0;

	switch(abCalStatus) {
		case 0 : nCal_Buf=(anCal_A+anCal_B); break;
		case 1 : nCal_Buf=(anCal_A-anCal_B); break;
		case 2 : nCal_Buf=(anCal_A*anCal_B); break;
		case 3 : nCal_Buf=(anCal_A/anCal_B); break;
	}
	return nCal_Buf;
}

void Encryption_Test(void)
{

#if 1 /////////////// ENCRYPTION TEST ////////////////////////////////////////////////

	unsigned char double_en, rand_data[2];
	UINT j=0;

	sub_address = (_encrypt_rand()|0x89) ;
//	sub_address &= 0xEF ;	// Feedback Disble

	double_en = (sub_address >> 6) & 0x01;

	for (j=0; j<8; j++) encrypt_tx_data[j] = _encrypt_rand();
//	encrypt_tx_data[7] = 0xE2;
//	encrypt_tx_data[6] = 0x90;
//	encrypt_tx_data[5] = 0x1B;
//	encrypt_tx_data[4] = 0x22;
//	encrypt_tx_data[3] = 0x65;
//	encrypt_tx_data[2] = 0x2F;
//	encrypt_tx_data[1] = 0xB8;
//	encrypt_tx_data[0] = 0x2F;

//	printf("\r\n-----------------------");

	#if 0
//	TxStr("Seed Data#1 : ", (UINT)((encrypt_tx_data[7]<<24)|(encrypt_tx_data[6]<<16)|(encrypt_tx_data[5]<<8)|encrypt_tx_data[4]<<0),8);
//	TxStr("Seed Data#2 : ", (UINT)((encrypt_tx_data[3]<<24)|(encrypt_tx_data[2]<<16)|(encrypt_tx_data[1]<<8)|encrypt_tx_data[0]<<0),8);
	printf("\r\nSeed Data#1 : %0x", (UINT)((encrypt_tx_data[7]<<24)|(encrypt_tx_data[6]<<16)|(encrypt_tx_data[5]<<8)|encrypt_tx_data[4]<<0));
	printf("\r\nSeed Data#1 : %0x", (UINT)((encrypt_tx_data[3]<<24)|(encrypt_tx_data[2]<<16)|(encrypt_tx_data[1]<<8)|encrypt_tx_data[0]<<0));

	_encrypt_enc1(EDR_U, EDR_S, rand_data, encrypt_tx_data, encrypt_ex_data1);
//	TxStr("Encryption#1 : ", (UINT)((encrypt_ex_data1[7]<<24)|(encrypt_ex_data1[6]<<16)|(encrypt_ex_data1[5]<<8)|encrypt_ex_data1[4]<<0),8);
//	TxStr("Encryption#2 : ", (UINT)((encrypt_ex_data1[3]<<24)|(encrypt_ex_data1[2]<<16)|(encrypt_ex_data1[1]<<8)|encrypt_ex_data1[0]<<0),8);
	printf("\r\nEncryption#1 : %0x", (UINT)((encrypt_ex_data1[7]<<24)|(encrypt_ex_data1[6]<<16)|(encrypt_ex_data1[5]<<8)|encrypt_ex_data1[4]<<0));
	printf("\r\nEncryption#2 : %0x", (UINT)((encrypt_ex_data1[3]<<24)|(encrypt_ex_data1[2]<<16)|(encrypt_ex_data1[1]<<8)|encrypt_ex_data1[0]<<0));

	_encrypt_enc2(EDR_U, EDR_S, rand_data, encrypt_tx_data, encrypt_ex_data2);
//	TxStr("Encryption#3 : ", (UINT)((encrypt_ex_data2[7]<<24)|(encrypt_ex_data2[6]<<16)|(encrypt_ex_data2[5]<<8)|encrypt_ex_data2[4]<<0),8);
//	TxStr("Encryption#4 : ", (UINT)((encrypt_ex_data2[3]<<24)|(encrypt_ex_data2[2]<<16)|(encrypt_ex_data2[1]<<8)|encrypt_ex_data2[0]<<0),8);
	printf("\r\nEncryption#3 : %0x", (UINT)((encrypt_ex_data2[7]<<24)|(encrypt_ex_data2[6]<<16)|(encrypt_ex_data2[5]<<8)|encrypt_ex_data2[4]<<0));
	printf("\r\nEncryption#4 : %0x", (UINT)((encrypt_ex_data2[3]<<24)|(encrypt_ex_data2[2]<<16)|(encrypt_ex_data2[1]<<8)|encrypt_ex_data2[0]<<0));

	BYTE bErrCnt=0;

	for (j=0; j<8; j++) if(encrypt_ex_data1[j]!=encrypt_ex_data2[j]) bErrCnt++;

	if(bErrCnt) {
//		TxStrDec("CPU Test Fail!!", bErrCnt,10);
//		TxStrDec("", bErrCnt,10);
		printf("CPU Test Fail!! (%d)", bErrCnt);
		while(1);
	}
	else {
//		TxStrDec("CPU Test Pass!!", bErrCnt,10);
//		TxStrDec("", bErrCnt,10);
		printf("CPU Test Pass!! (%d)", bErrCnt);
	}

	#else
	DispDatHex("Tx Data:", 15, 24, (UINT)((encrypt_tx_data[7]<<24)|(encrypt_tx_data[6]<<16)|(encrypt_tx_data[5]<<8)|encrypt_tx_data[4]<<0));
	DispDatHex("       :", 16, 24, (UINT)((encrypt_tx_data[3]<<24)|(encrypt_tx_data[2]<<16)|(encrypt_tx_data[1]<<8)|encrypt_tx_data[0]<<0));

	_encrypt_enc1(EDR_U, EDR_S, rand_data, encrypt_tx_data, encrypt_ex_data1);
	DispDatHex("E1 Data:", 17, 24, (UINT)((encrypt_ex_data1[7]<<24)|(encrypt_ex_data1[6]<<16)|(encrypt_ex_data1[5]<<8)|encrypt_ex_data1[4]<<0));
	DispDatHex("       :", 18, 24, (UINT)((encrypt_ex_data1[3]<<24)|(encrypt_ex_data1[2]<<16)|(encrypt_ex_data1[1]<<8)|encrypt_ex_data1[0]<<0));

	_encrypt_enc2(EDR_U, EDR_S, rand_data, encrypt_tx_data, encrypt_ex_data2);
	DispDatHex("E2 Data:", 19, 24, (UINT)((encrypt_ex_data2[7]<<24)|(encrypt_ex_data2[6]<<16)|(encrypt_ex_data2[5]<<8)|encrypt_ex_data2[4]<<0));
	DispDatHex("       :", 20, 24, (UINT)((encrypt_ex_data2[3]<<24)|(encrypt_ex_data2[2]<<16)|(encrypt_ex_data2[1]<<8)|encrypt_ex_data2[0]<<0));

	BYTE bErrCnt=0;

	for (j=0; j<8; j++) if(encrypt_ex_data1[j]!=encrypt_ex_data2[j]) bErrCnt++;

	if(bErrCnt) {
		DispDatDec("Enc Fail", 21, 24, bErrCnt);
		while(1);
	}
	else DispDatDec("Enc Pass", 21, 24, bErrCnt);
	#endif
#else

	static BYTE bCalState=0;
	UINT nCal_A=0, nCal_B=0, nRst_A=0, nRst_B=0;

	nCal_A = _encrypt_rand();
	nCal_B = _encrypt_rand();
	nRst_A = Calculation_TestA(bCalState,nCal_A,nCal_B);
	nRst_B = Calculation_TestB(bCalState,nCal_A,nCal_B);

//	if     (bCalState==0) { DispDatHex("CalA+B :", 19, 24, nRst_A); DispDatHex("CalA+B :", 20, 24, nRst_B); }
//	else if(bCalState==1) { DispDatHex("CalA-B :", 19, 24, nRst_A); DispDatHex("CalA-B :", 20, 24, nRst_B); }
//	else if(bCalState==2) { DispDatHex("CalA*B :", 19, 24, nRst_A); DispDatHex("CalA*B :", 20, 24, nRst_B); }
//	else if(bCalState==3) { DispDatHex("CalA/B :", 19, 24, nRst_A); DispDatHex("CalA/B :", 20, 24, nRst_B); }
	if     (bCalState==0) { printf("\r\n%d + %d - Flash : %d, SRAM : %d", nCal_A, nCal_B, nRst_A, nRst_B); }
	else if(bCalState==1) { printf("\r\n%d - %d - Flash : %d, SRAM : %d", nCal_A, nCal_B, nRst_A, nRst_B); }
	else if(bCalState==2) { printf("\r\n%d * %d - Flash : %d, SRAM : %d", nCal_A, nCal_B, nRst_A, nRst_B); }
	else if(bCalState==3) { printf("\r\n%d / %d - Flash : %d, SRAM : %d", nCal_A, nCal_B, nRst_A, nRst_B); }

	if(nRst_A!=nRst_B) {
		printf("\r\nChk Fail");
//		printf("\r\n CPU Tick Time : %d", gTimerTicks);
		while(1);
	}
	else printf("\r\nChk Pass");

	bCalState = (bCalState<3) ? (bCalState+1) : 0;

#endif
}


#define ITEST i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;i++;		// odd num 15
#define PTEST *p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;*p = (*p)+1;		// odd num 15

#define TEST_MEMORY_SIZE	(0x800-4)/*128*/

#define TxStrDbg	TxStr // TxStrNoIRQ


//#define W5_50

#define TEST_MEM_ADDR1	0x04004000 // 0x10008000
//#define TEST_MEM_ADDR2	0x00003AEC // 0x10001000
#define TEST_MEM_ADDR2	0x04004000 // 0x10001000


void mytest1(void)
{
//	volatile int* p = (volatile int* )0x10008000;
	volatile int* p = (volatile int* )TEST_MEM_ADDR1;
	int i=0x200;
	*p=0x200;

	ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST;
	PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST;

	if(*p != i){
//		TxStrDbg("X:",i,"!=");
//		TxStrDbg("",*p," ");

		printf("\r\n(mt1)X:!=%d", i);
		printf("\r\n%d", *p);

//		TxStrDbg("(mt1)X:!=", i, 8);
//		TxStrDbg("\r\n", *p, 8);
	}
	else{
//		TxStrDbg("O:",i,"=");
//		TxStrDbg("",*p," ");

		printf("\r\n(mt1)O:!=%d", i);
		printf("\r\n%d", *p);

//		TxStrDbg("(mt1)O:=", i, 8);
//		TxStrDbg("\r\n", *p, 8);
	}
}

void mytest2(void)
{
//	volatile int* p = (volatile int* )0x10008000;
	volatile int* p = (volatile int* )TEST_MEM_ADDR1;
	int i=0x300;
	*p=0x300;

	ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST;
	PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST;

	if(*p != i){
//		TxStrDbg("(mt2)X:!=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt2)X:!=%d", i);
		printf("\r\n%d", *p);
	}
	else{
//		TxStrDbg("(mt2)O:=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt2)O:!=%d", i);
		printf("\r\n%d", *p);
	}
}

void mytest3(void)
{
//	volatile int* p = (volatile int* )0x10008000;
	volatile int* p = (volatile int* )TEST_MEM_ADDR1;
	int i=0x400;
	*p=0x400;

	ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST;
	PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST;

	if(*p != i){
//		TxStrDbg("(mt3)X:!=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt3)X:!=%d", i);
		printf("\r\n%d", *p);
	}
	else{
//		TxStrDbg("(mt3)O:=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt3)O:!=%d", i);
		printf("\r\n%d", *p);
	}
}

void mytest4(void)
{
//	volatile int* p = (volatile int* )0x10008000;
	volatile int* p = (volatile int* )TEST_MEM_ADDR1;
	int i=0x500;
	*p=0x500;

	ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST; ITEST;// ITEST; ITEST; ITEST; ITEST;
	PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST; PTEST;// PTEST; PTEST; PTEST; PTEST;

	if(*p != i){
//		TxStrDbg("(mt4)X:!=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt4)X:!=%d", i);
		printf("\r\n%d", *p);
	}
	else{
//		TxStrDbg("(mt4)O:=", i, 8);
//		TxStrDbg("\r\n", *p, 8);

		printf("\r\n(mt4)O:!=%d", i);
		printf("\r\n%d", *p);
	}
}


//void memory_write(U32 addr, U8* buf, U32 length)
void memory_write(UINT addr, BYTE* buf, UINT length)
{
//	int i;
	UINT i;

	for(i=0;i<length;i++)
	{
//		*((volatile U8*)(addr + (U8)i)) = buf[i];
		*((volatile BYTE*)(addr + (BYTE)i)) = buf[i];
	}
}

//void memory_read(U32 addr, U8 *buf, U32 length)
void memory_read(UINT addr, BYTE* buf, UINT length)
{
//	int i;
	UINT i;

	for(i=0;i<length;i++)
	{
//		buf[i] = *((volatile U8*)(addr + (U8)i));
		buf[i] = *((volatile BYTE*)(addr + (BYTE)i));
	}
}

void ISPM Mem_Test(void)
{
	UINT i=0,j=0;
	BYTE w_buf[TEST_MEMORY_SIZE];
	BYTE r_buf[TEST_MEMORY_SIZE];

//	Init();

//	TxStrDbg("w_buf  : ", (UINT)w_buf , 8);
//	TxStrDbg("r_buf  : ", (UINT)r_buf , 8);

	printf("\r\nw_buf  : %d", (UINT)w_buf);
	printf("\r\nr_buf  : %d", (UINT)r_buf);

//	TxStrDbg("*((UINT*)_data_e : ", *((UINT*)_data_e), 8);
//	TxStrDbg("_data_e : ", _data_e, 8);
//	TxStrDbg("_bss_e  : ", _bss_e , 8);

	printf("\r\n_data_e : %d", _data_e);
	printf("\r\n_bss_e  : %d", _bss_e );

#if 1
	//---------------------------
	// Cache Test Program
	//---------------------------
	mytest1();	// #77  = 0x4D
	mytest2();	// #91  = 0x5B
	mytest3();	// #119 = 0x77
	mytest4();	// #105 = 0x69
#endif

#if 1 // Internal SRAM Test Program

	UINT nErrCnt=0;

//	for(j=0;j<30;j++)
	for(j=0;j<5;j++)	// 201565 - WHL - SRAM Test Area : 0x4500 ~ 0x7C96
	{
		for(i=0;i<TEST_MEMORY_SIZE;i++)
		{
			w_buf[i] = i + j*0x800;
			r_buf[i] = 0;
		}

//		memory_write(0x10001000+j*0x800, w_buf, TEST_MEMORY_SIZE);		// be careful with stack address
//		memory_read (0x10001000+j*0x800, r_buf, TEST_MEMORY_SIZE);
		memory_write(TEST_MEM_ADDR2+j*0x800, w_buf, TEST_MEMORY_SIZE);		// be careful with stack address
		memory_read (TEST_MEM_ADDR2+j*0x800, r_buf, TEST_MEMORY_SIZE);



		for(i=0;i<TEST_MEMORY_SIZE;i++)
		{
			if(w_buf[i] != r_buf[i]) {
				printf("\r\n%d - Test Fail[%d] - Tx : %d, Rx : %d", j, i, w_buf[i], r_buf[i]);
				nErrCnt++;
			}
			else printf("\r\n%d - Test Pass[%d] - Tx : %d, Rx : %d", j, i, w_buf[i], r_buf[i]);
//			if(w_buf[i] != r_buf[i]) nErrCnt++;
		}
	}
	if(nErrCnt) {
		printf("\r\n Memory Test Fail! (%d)", nErrCnt);
//		printf("\r\n CPU Tick Time : %d", gTimerTicks);
		while(1);
	}
	else printf("\r\n Memory Test Pass! (%d)", nErrCnt);

#endif
	printf("\r\n----- EN673 Mem Test End -----");



//	while(1){
//		WaitXus(1);
//	}
}

void sub_test(void)
{
	int i=0;

//#if 0
	for(i=0; i<0x1000; i++) Encryption_Test();
//#else
	Mem_Test();
//#endif
}
#endif

/* EOF */
