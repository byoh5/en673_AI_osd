#ifndef _UXSR_H_
#define _UXSR_H_



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void XsrBuserr(void);
extern void XsrAlign(void);
extern void XsrIllInsn(void);
extern void XsrFloatPoint(void);
extern void XsrSyscall(void);
extern void XsrBreak(void);
extern void XsrTrap(void);

extern void default_xsr(unsigned xsr_adr, unsigned epc);
extern void add_xsr(int n, void (*handler)(void));



#ifndef	DEF_CPU1	//...........................
extern void XsrDpFault(void);					//
extern void XsrIpFault(void);					//
extern void XsrDtlbMiss(void);					//
extern void XsrItlbMiss(void);					//
#endif	// ......................................

//-------------------------------------------------------------------------------------------------
// Variable



#endif // _UXSR_H_
