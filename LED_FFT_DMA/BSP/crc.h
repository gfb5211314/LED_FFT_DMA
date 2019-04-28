#ifndef __CRC_H
#define __CRC_H
#include "main.h"
#include "stm32f1xx_hal.h"

uint16_t Calc_CRC16( uint8_t *pData, long nLength);

#endif
