#ifndef _INIT_H_
#define _INIT_H_



//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//


//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void InitHw(void);
extern void FlsFatInit(void);
extern void MemFatInit(void);
extern void RtcInit(void);

extern void InitUsr(void);
extern void LoadDefUserInfo(void);

extern void InitSw(void);

//-------------------------------------------------------------------------------------------------
// Variable
#ifdef __ETH__
extern struct netif gtMyNetIf;	// ETH
#endif
#ifdef __WIFI__
extern struct netif gtwlif_sta;// Wi-Fi STA mode
extern struct netif gtwlif_uap;// Wi-Fi UAP mode
#endif

#endif // _INIT_H_
