#ifndef INCLUDE_AIC3204_H_
#define INCLUDE_AIC3204_H_

#include "tistdtypes.h"

Int16 AIC3204_rset(Uint16 regnum, Uint16 regval);
void Init_AIC3204(Uint32 sf, Int16 gDAC, Uint16 gADC);


#endif /* INCLUDE_AIC3204_H_ */
