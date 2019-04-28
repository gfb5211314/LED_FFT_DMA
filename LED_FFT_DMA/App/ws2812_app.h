#ifndef __WS2812_APP_H__
#define __WS2812_APP_H__
#include "stm32f1xx_hal.h"
#include   "bsp_ws2812.h"

#define LED_MAX                228   //��Ŀ������
extern void ws2812_rand_light(volatile uint16_t  amount);

/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********ÿ��ɫѭ��������**********δ���****/
extern void  arrange_display_one_run(volatile uint16_t  amount, uint16_t color_type_n);

/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********ÿ��ɫתһ��**************/
extern void  arrange_display_two_run(volatile uint16_t  amount);

/***������ɫһ�𽥱�***************/
extern void DMA_WS2812_Rampping(volatile uint16_t  amount, uint8_t pwm, colors_kind color);

/***********ɫ��**************/
extern void  arrange_display(volatile uint16_t  amount);

/***********ɫ��2��һ**************/
extern void  arrange_display_two(volatile uint16_t  amount);

/*********������ɫ����***************/
extern void DMA_WS2812_Rampping_1(volatile uint16_t  amount, uint8_t pwm, colors_kind color);


extern void DMA_WS2812_Running(volatile uint16_t  amount);

extern void DMA_WS2812_Run(volatile uint16_t  amount);

extern void DMA_WS2812_Ramp(volatile uint16_t  amount, uint8_t pwm, colors_kind color);

/***********������������****************/
extern void DMA_WS2812_Running_more(volatile uint16_t  amount, volatile uint16_t run_number);


#endif

