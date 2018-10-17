#ifndef __TDC_GPX2_H__
#define __TDC_GPX2_H__
#include "sys.h"

void ReadTdc1Regs(u8 clksource);
void ReadTdc2Regs(u8 clksource);
void startTDC(void);
void readTDC(void);

#define OSCCLK 0
#define REFCLK 1

#define TDC_INTERRUPT1 PCin(6)
#define TDC_INTERRUPT2 PDin(15)

#define SPI1_TDC1_DISABLE PBout(0) = 1
#define SPI1_TDC1_ENABLE  PBout(0) = 0

#define SPI1_TDC2_DISABLE PEout(4) = 1
#define SPI1_TDC2_ENABLE  PEout(4) = 0

#define TDC_ENABLE   	  PCout(5) = 0
#define TDC_DISABLE  	  PCout(5) = 1

#define TDC_RSTENABLE	  PCout(4) = 1
#define TDC_RSTDISABLE    PCout(4) = 0
#endif


