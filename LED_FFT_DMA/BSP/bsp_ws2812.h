#ifndef   __BSP_WS2812_H__
#define   __BSP_WS2812_H__


#include "stm32f1xx_hal.h"

#define  ledmax          66  
#define lamp_bead_amount           15          //lamp bead amount 
typedef  struct
{
    uint8_t  gree ;
    uint8_t red;
    uint8_t blue;
} color_type;


typedef enum
{  
	  w28_idle=0,
    w28_start_reset=1,
    w28_reset_check=2,
	  w28_data_update=3,
	  w28_end_reset=4,
	 
	 
} ws128_state_type;
typedef enum
{  
      w28_start=5,
	    w28_check_state=6
	 
} ws128_ctrl_type;


/**
  * @brief  Color selection enumeration
  */
typedef enum
{
    white = 0,
    green,
    red,
    blue,
    mie,
    yellow,
    blue_green,

} colors_kind;


extern void reset_led_light();


void  led_run(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm);






/**
 * @brief  Control single LED flashing.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @param  time    :    Flicker time size(ms)
 * @retval None
 */
void  led_runing(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time);

/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @retval None
 */

void  led_open(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm);

    





   


/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @param  time   :    Turn on the light up time(ms)
 * @retval None
 */

void  led_opening(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time);





/**
 * @brief  Light color type.
 * @param  amount :   Maximum number of lighting leds
 * @param  p      :    Color combination array
 * @retval None
 */
void  led_type(volatile uint16_t  amount,  color_type* p);


void  RGBTOGRB(uint16_t count, uint8_t p[][3]);

/**
  * @brief  Shift the array's LED data to the 0 position of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */
void shift_buf_to_data_one(uint16_t led_location);

/**
  * @brief  Move the array's LED data to position 1 of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */
void shift_buf_to_data_two(uint16_t led_location);

/**
  * @brief  Convert GRB data into PWM arrays
  * @param  led_location :  number of led
  * @retval None
  */


void GRBTOPWM(uint16_t led_max);

void ws2812_init( void);

void  SET_PWM_DMA_DODE(uint32_t mode);










/********************************************************************
       以下函数采用半传输中断更新数据，优化的底层函数和应用函数
                    BOTTOM function
               *DMA_WS2812_Reset              ws2812复位
               *DMA_WS2812_light              ws2812亮
               *DMA_WS2812_Mie                ws2812灭
               *DMA_WS2812_SIN                正弦函数
                     APP
               *DMA_WS2812_Ramp                渐变效果一会灭一会亮,采用正弦
               *DMA_WS2812_Run                 依次亮(每个都亮，每个灯位置颜色不同)
               *DMA_WS2812_Running             几个跑(亮度相同,几个跑起来)
               *DMA_WS2812_Ramping             同一类颜色渐变 (深红,浅红)

**********************************************************************/
/********复位************/
void DMA_WS2812_Reset(void);

void DMA_WS2812_light(uint16_t led_n);

void DMA_WS2812_Mie(uint16_t led_n);

/**********单种颜色数据的值*****************/
void DMA_WS2812_SIN(uint16_t amount, uint8_t pwm, colors_kind color);

/**********多种颜色组合渐变*****************/
void DMA_WS2812_SIN_More(uint16_t amount, uint8_t pwm, uint8_t color_ty);



/***灭数据移位************/
void DMA_WS2812_data_shift(uint16_t led_location);


void DMA_WS2812_Ramp(volatile uint16_t  amount, uint8_t pwm, colors_kind color);





/***全亮数据移位,第几个位置灯的颜色，由我设置**/
/***配色函数***********************/
void DMA_WS2812_data_shift_light(uint16_t led_location, uint16_t led_max);


void DMA_WS2812_Run(volatile uint16_t  amount);

/***多种颜色一起渐变***************/
void DMA_WS2812_Rampping(volatile uint16_t  amount, uint8_t pwm, colors_kind color);

/*********单中颜色渐变***************/
void DMA_WS2812_Rampping_1(volatile uint16_t  amount, uint8_t pwm, colors_kind color);


void DMA_WS2812_Running(volatile uint16_t  amount);

/**
  *@brief gets the maximun value of rgb
	*@param r_data   value of red range( 0-255)
  *@param g_data   value of red range( 0-255)
	*@param b_data   value of red range( 0-255)
	*@retval   renturn RGB maximum number
  */
uint8_t get_rgb_max_value(uint8_t r_data, uint8_t g_data, uint8_t b_data);

/**
  *@brief gets the minimum value of rgb
  *@param r_data   value of red range( 0-255)
  *@param g_data   value of red range( 0-255)
	*@param b_data   value of red range( 0-255)
  */
double get_rgb_min_value(double r_data, double g_data, double b_data);

void RGB2HLS(double* h, double* l, double* s, uint8_t r_data, uint8_t g_data, uint8_t b_data);


double HLS2RGBvalue(double n1, double n2, double hue);

void HLS2RGB(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s);

void HLS_TO_RGB_ALL(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n, uint8_t (*arr)[3]);


void  	rand_buff_data(volatile uint16_t  amount, uint16_t color_type_n);

extern void HLS_TO_RGB(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n, uint8_t (*arr)[3]);
extern void HLS_TO_RGB_ONE(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n_LOC, uint8_t (*arr)[3]);
extern void DMA_WS2812_data_shift_more(uint16_t led_location, uint16_t run_number);


extern void shift_buf_to_data_one(uint16_t led_location);

/**
  * @brief  Move the array's LED data to position 1 of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */
extern void shift_buf_to_data_two(uint16_t led_location);
#endif

