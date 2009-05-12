#ifndef AXUMCORE_H
#define AXUMCORE_H

extern far cregister volatile unsigned int CSR;
extern far cregister volatile unsigned int IER;

#define NUMBEROFDMAINPUTCHANNELS 		64
#define NUMBEROFDMAOUTPUTCHANNELS 		64
#define PINGPONG 2
signed int DMABuffer_RCV[PINGPONG][NUMBEROFDMAINPUTCHANNELS];
signed int DMABuffer_XMIT[PINGPONG][NUMBEROFDMAOUTPUTCHANNELS];

void Delay_us();
void ISR_dMAX_TransferComplete();
void InitializeUHPI();
void InitializeC6727B_PLL();
void InitializeMcASP0_dMAX_IRQ();
									
#endif
