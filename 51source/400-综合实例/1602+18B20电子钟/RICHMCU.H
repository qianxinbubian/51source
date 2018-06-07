#ifndef __JCHBASIC_H__
#define __JCHBASIC_H__

#include"intrins.h"
#define nop _nop_()

typedef  unsigned char    uint8;
typedef  signed char      int8;
typedef  unsigned int     uint16;
typedef  signed int       int16;
typedef  unsigned long    uint32;
typedef  signed long      int32;
 
#define ABS(value) ((value) > 0 )? (value) : (-(value))
#define DELAY1US_M _nop_()
#endif