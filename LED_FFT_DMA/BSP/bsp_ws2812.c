#include   "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include  "math.h"
#include   "bsp_dma.h"
#include "string.h"
#include <stdlib.h>
#include  "ws2812_app.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim2_ch1;


#define  HTIM         htim2
#define  CHANAL       TIM_CHANNEL_1
#define   DMA_TIM_CH   hdma_tim2_ch1
/****  Macro definition parameter  ****************/
#define  reset_max        50
#define BIT_1                      61u //1比较值为61 850us
#define BIT_0                      29u //0比较值为28 400us 
#define LED_NUM                   24*200        //200

/* Private variables ---------------------------------------------------------*/
uint16_t ab[LED_NUM];
uint16_t reset_buf[200];
color_type    color_buf[35] = { {0xff, 0x00, 0x00}, //绿色
    {0x00, 0xff, 0x00}, //红色
    {0x00, 0x00, 0xff}, //蓝色
    {255, 255, 31}, //黄色
    {238, 118, 198},
    { 255, 84,  159},
    { 191,  0, 255},
    { 164, 244,  96},
    {0, 51, 51}
};
color_type    color_mie[35] = { 0 };




uint8_t    ws2812_state = 0;
uint16_t   ws28128_reset_buf[50] = {0}; //62.5us      大于50us低电平复位
uint16_t   ws28128_end_reset_buf[50] = {0};
uint16_t   ws28128_mie_buf[48] = {28};
uint8_t   ws28128_color_buf[1000][3] =
{
0
};
//接收colorbuf
uint16_t   ws28128_color_data[48] = {0};   //PWM数据



uint8_t   Shift_mode = 0;
/*******************/
uint8_t   reset_flag = 0;
uint8_t   send_flag = 0;
uint8_t  updata_flag = 0;
uint8_t   mie_flag = 0;
uint16_t   mie_led_count;
uint16_t   mie_led_max;
volatile uint8_t half_tx ;
volatile uint8_t full_tx ;
volatile uint8_t reset_end_flag = 0;
extern   uint8_t  systick_500ms_flag;
extern uint16_t  k3;

uint16_t light_led_max;
uint8_t  light_flag;
uint16_t light_led_count;



void HLS_TO_RGB(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n, uint8_t (*arr)[3]);
void HLS_TO_RGB_ONE(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n_LOC, uint8_t (*arr)[3]);

extern uint8_t  tx_comple_flag;
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */
//	
  			 if(reset_flag==1)
	{
		   reset_flag=0;
			
	  	HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
	
	
	} 

if(light_flag==1)
	 {		 
		 			 
             light_led_count++; 
		 		if(light_led_count>light_led_max)
				{
//				 memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
//	       memset(ws28128_color_data, 0, sizeof(ws28128_color_data));
					light_flag=0;
				 light_led_count=0;
			    tx_comple_flag=1;
					
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1); 
					
			
				}
		   
		       if((light_led_count%2)==0)
					 {
						shift_buf_to_data_one(light_led_count);
						 
					 }
					 else
					 {
						shift_buf_to_data_two(light_led_count); 
					 }
      	
		
	}

		if((mie_flag==1)&&(light_flag==0))
   {
   	    
	      	mie_led_count++;
		  if(mie_led_count>(mie_led_max-1))
			{
				mie_flag=0;
				mie_led_count=0;
			
		 HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
			
				
			}
			
		} 
  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_tim2_ch1);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}
/******注册完成回调不行************/
/*DMA传输一半回调*/

/**
  * @brief   Half transfer callback function of DMA
  * @param  DMA_HandleTypeDef: DMA HAND
  * @retval None
  */
void DMA_HalfCpltCallback(DMA_HandleTypeDef* _hdma)
{


}

/*DMA传输完成回调*/

/**
  * @brief  transfer complete callback function of DMA
  * @param  DMA_HandleTypeDef: DMA HAND
  * @retval None
  */
void DMA_CpltCallback(DMA_HandleTypeDef* _hdma)
{


}
/*注册DMA传输一半回调中断函数**********/
/**
  * @brief  Register  Half transfer callback function of DMA
  * @retval None
  */

void DMA_RegisterHalfCallback()
{
    HAL_DMA_RegisterCallback(&DMA_TIM_CH, HAL_DMA_XFER_HALFCPLT_CB_ID, DMA_HalfCpltCallback);
}
/*注册DMA传输完成回调中断函数**********/
/**
  * @brief  Register  transfer complete  callback function of DMA
  * @retval None
  */

void DMA_RegisterCpltCallback()
{
    HAL_DMA_RegisterCallback(&DMA_TIM_CH, HAL_DMA_XFER_CPLT_CB_ID, DMA_CpltCallback);
}
/**
  * @brief  intitialize callback function of DMA
  * @retval None
  */
void DMA_Callback_init()
{
    DMA_RegisterHalfCallback();
    DMA_RegisterCpltCallback();

}
/**
  * @brief  Convert RGB data to GRB
  * @param  count : number of led  0-6556
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */

void  RGBTOGRB(uint16_t count, uint8_t p[][3])
{
    uint8_t tem_data;
    for(uint16_t i = 0; i < count; i++)
    {
        tem_data = p[i][0];;
        p[i][0] = p[i][1];
        p[i][1] = tem_data;
        p[i][2] = p[i][2];
    }
}
/**
  * @brief  Shift the array's LED data to the 0 position of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */
void shift_buf_to_data_one(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
    /*Prevents the original data from being shifted to 0*/
    gree_data = ws28128_color_buf[led_location][1];
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 0; i < 24; i++)
    {
        if(i < 8)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((7 < i) && (i < 16))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}
/**
  * @brief  Move the array's LED data to position 1 of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None
  */
void shift_buf_to_data_two(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
    gree_data = ws28128_color_buf[led_location][1];
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 24; i < 48; i++)
    {
        if(i < 32)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((31 < i) && (i < 40))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}

/**
  * @brief  Convert GRB data into PWM arrays
  * @param  led_location :  number of led
  * @retval None
  */


void GRBTOPWM(uint16_t led_max)
{
    uint8_t    gree_data;
    uint8_t    blue_data;
    uint8_t    red_data;
    for(uint16_t j = 0; j < led_max; j++)
    {
        gree_data = ws28128_color_buf[j][1];
        red_data = ws28128_color_buf[j][0];
        blue_data = ws28128_color_buf[j][2];
        for(uint16_t i = 0; i < 24; i++)
        {

            if(i < 8)
            {

                if(gree_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                gree_data = gree_data << 1;

            }
            else if((7 < i) && (i < 16))
            {

                if(red_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                red_data = red_data << 1;


            }
            else
            {
                if(blue_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                blue_data = blue_data << 1;


            }


        }
    }
}

void ws2812_init()
{


}
void  SET_PWM_DMA_DODE(uint32_t mode)
{

    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 DMA Init */
    /* TIM2_CH1 Init */
    hdma_tim2_ch1.Instance = DMA1_Channel5;
    hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim2_ch1.Init.Mode = mode;
    hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
    if(HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&HTIM, hdma[TIM_DMA_ID_CC1], hdma_tim2_ch1);

    /* Prepare the DMA Channel configuration */


    /* Write to DMA Channel CR register */


}









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
void DMA_WS2812_Reset()
{


//    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_NORMAL);
    reset_flag = 1;
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, reset_max);

}
void DMA_WS2812_light(uint16_t led_n)
{
	

    light_led_max = led_n;
//    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    GRBTOPWM(2);
    light_flag = 1;
    light_led_count = 1;
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);
}
void DMA_WS2812_Mie(uint16_t led_n)
{
    mie_led_max = led_n;
//    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    mie_flag = 1;
    mie_led_count = 0;
    for(uint16_t i = 0; i < 48; i++)
    {
        ws28128_mie_buf[i] = 29;
    }
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_mie_buf, 48);
}
/**********单种颜色数据渐变的值*****************/
void DMA_WS2812_SIN(uint16_t amount, uint8_t pwm, colors_kind color)
{
    uint8_t     sin_y = 0;
//    printf("START");

    float sin_x;  //弧度
    float sin_jiao;//角度 分度角
    sin_jiao = 360.000 / 255;
   

    sin_x = sin_jiao * pwm; //得到角度值
    sin_x = sin_x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
    sin_y = 128 * sin(sin_x + 4.75) + 128;

    for(uint16_t j = 0; j < amount; j++)
    {


        if(color == green)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = sin_y; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == red)
        {
            ws28128_color_buf[j][0] = sin_y; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == blue)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = sin_y;	 //blue
        }
//         else if(color == yellow)
//        {
//            ws28128_color_buf[j][0] = sin_y; //red
//            ws28128_color_buf[j][1] = sin_y; //gree
//            ws28128_color_buf[j][2] = 0;	 //blue
//        }
    }

}
/**********多种颜色组合渐变*****************/
void DMA_WS2812_SIN_More(uint16_t amount, uint8_t pwm, uint8_t color_ty)
{
    uint8_t     sin_y = 0;
//    printf("START");

    float sin_x;  //弧度
    float sin_jiao;//角度 分度角
    sin_jiao = 360.000 / 255;
 

    sin_x = sin_jiao * pwm; //得到角度值
    sin_x = sin_x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
    sin_y = 128 * sin(sin_x + 4.75) + 128;

    for(uint16_t j = 0; j < amount; j++)
    {


        if(j <amount/4 )
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = sin_y; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if((j > (amount/4-1)) && (j < (amount/4)*2))
        {

            ws28128_color_buf[j][0] = sin_y; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if((j > ((amount/4)*2-1)) && (j < (amount/4)*3))
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = sin_y;	 //blue
        }
        else
        {
            ws28128_color_buf[j][0] = sin_y; //red
            ws28128_color_buf[j][1] = sin_y; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
    }




}
void DMA_WS2812_one_light(uint16_t amount, colors_kind color)
{
  
    for(uint16_t j = 0; j < amount; j++)
    {


        if(color == green)
        {
            ws28128_color_buf[j][0] = 0xff; //red
            ws28128_color_buf[j][1] = 0xff; //gree
            ws28128_color_buf[j][2] = 0xff;	 //blue
        }
        else if(color == red)
        {
            ws28128_color_buf[j][0] = 0xff; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == blue)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0xff;	 //blue
        }
//         else if(color == yellow)
//        {
//            ws28128_color_buf[j][0] = sin_y; //red
//            ws28128_color_buf[j][1] = sin_y; //gree
//            ws28128_color_buf[j][2] = 0;	 //blue
//        }
    }


}

/***灭数据移位************/
void DMA_WS2812_data_shift(uint16_t led_location)
{

    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    ws28128_color_buf[led_location][0] = 0; //R
    ws28128_color_buf[led_location][1] = 0; //G
    ws28128_color_buf[led_location][2] = 255; //blue

    ws28128_color_buf[led_location + 1][0] = 0; //R
    ws28128_color_buf[led_location + 1][1] = 255; //G
    ws28128_color_buf[led_location + 1][2] = 0; //blue
    ws28128_color_buf[led_location + 2][0] = 0; //R
    ws28128_color_buf[led_location + 2][1] = 0; //G
    ws28128_color_buf[led_location + 2][2] = 255; //blue

    ws28128_color_buf[led_location + 3][0] = 0; //R
    ws28128_color_buf[led_location + 3][1] = 255; //G
    ws28128_color_buf[led_location + 3][2] = 0; //blue
    ws28128_color_buf[led_location + 4][0] = 0; //R
    ws28128_color_buf[led_location + 4][1] = 0; //G
    ws28128_color_buf[led_location + 4][2] = 255; //blue

    ws28128_color_buf[led_location + 5][0] = 0; //R
    ws28128_color_buf[led_location + 5][1] = 255; //G
    ws28128_color_buf[led_location + 5][2] = 0; //blue



    ws28128_color_buf[led_location + 10][0] = 0; //R
    ws28128_color_buf[led_location + 11][1] = 255; //G
    ws28128_color_buf[led_location + 12][2] = 0; //blue
    ws28128_color_buf[led_location + 24][0] = 0; //R
    ws28128_color_buf[led_location + 26][1] = 0; //G
    ws28128_color_buf[led_location + 27][2] = 255; //blue

    ws28128_color_buf[led_location + 30][0] = 0; //R
    ws28128_color_buf[led_location + 31][1] = 255; //G
    ws28128_color_buf[led_location + 32][2] = 0; //blue
    ws28128_color_buf[led_location + 43][0] = 0; //R
    ws28128_color_buf[led_location + 41][1] = 0; //G
    ws28128_color_buf[led_location + 41][2] = 255; //blue

    ws28128_color_buf[led_location + 50][0] = 0; //R
    ws28128_color_buf[led_location + 51][1] = 255; //G
    ws28128_color_buf[led_location + 52][2] = 0; //blue

}
/***多个灭数据移位************/
void DMA_WS2812_data_shift_more(uint16_t led_location, uint16_t run_number)
{

    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    ws28128_color_buf[led_location][0] = 255; //R
    ws28128_color_buf[led_location][1] = 0; //G
    ws28128_color_buf[led_location][2] = 0; //blue

    ws28128_color_buf[led_location + 1][0] = 255; //R
    ws28128_color_buf[led_location + 1][1] = 0; //G
    ws28128_color_buf[led_location + 1][2] = 0; //blue
    ws28128_color_buf[led_location + 2][0] = 255; //R
    ws28128_color_buf[led_location + 2][1] = 0; //G
    ws28128_color_buf[led_location + 2][2] = 0; //blue

    ws28128_color_buf[led_location + 3][0] = 255; //R
    ws28128_color_buf[led_location + 3][1] = 0; //G
    ws28128_color_buf[led_location + 3][2] = 0; //blue
    ws28128_color_buf[led_location + 4][0] = 255; //R
    ws28128_color_buf[led_location + 4][1] = 0; //G
    ws28128_color_buf[led_location + 4][2] = 0; //blue

    ws28128_color_buf[led_location + 5][0] = 255; //R
    ws28128_color_buf[led_location + 5][1] = 0; //G
    ws28128_color_buf[led_location + 5][2] = 0; //blue

    if(led_location > 20)
    {
        ws28128_color_buf[led_location - 20][0] = 0; //R
        ws28128_color_buf[led_location - 20][1] = 255; //G
        ws28128_color_buf[led_location - 20][2] = 0; //blue

        ws28128_color_buf[led_location - 19][0] = 0; //R
        ws28128_color_buf[led_location - 19][1] = 255; //G
        ws28128_color_buf[led_location - 19][2] = 0; //blue
        ws28128_color_buf[led_location - 19][0] = 0; //R
        ws28128_color_buf[led_location - 19][1] = 255; //G
        ws28128_color_buf[led_location - 19][2] = 0; //blue

        ws28128_color_buf[led_location - 18][0] = 0; //R
        ws28128_color_buf[led_location - 18][1] = 255; //G
        ws28128_color_buf[led_location - 18][2] = 0; //blue
        ws28128_color_buf[led_location - 18][0] = 0; //R
        ws28128_color_buf[led_location - 18][1] = 255; //G
        ws28128_color_buf[led_location - 18][2] = 0; //blue

        ws28128_color_buf[led_location - 17][0] = 0; //R
        ws28128_color_buf[led_location - 17][1] = 255; //G
        ws28128_color_buf[led_location - 17][2] = 0; //blue


    }
    if(led_location > 30)
    {
        ws28128_color_buf[led_location - 30][0] = 0; //R
        ws28128_color_buf[led_location - 30][1] = 0; //G
        ws28128_color_buf[led_location - 30][2] = 255; //blue

        ws28128_color_buf[led_location - 29][0] = 0; //R
        ws28128_color_buf[led_location - 29][1] = 0; //G
        ws28128_color_buf[led_location - 29][2] = 255; //blue
        ws28128_color_buf[led_location - 29][0] = 0; //R
        ws28128_color_buf[led_location - 29][1] = 0; //G
        ws28128_color_buf[led_location - 29][2] = 255; //blue

        ws28128_color_buf[led_location - 28][0] = 0; //R
        ws28128_color_buf[led_location - 28][1] = 0; //G
        ws28128_color_buf[led_location - 28][2] = 255; //blue
        ws28128_color_buf[led_location - 28][0] = 0; //R
        ws28128_color_buf[led_location - 28][1] = 0; //G
        ws28128_color_buf[led_location - 28][2] = 255; //blue

        ws28128_color_buf[led_location - 27][0] = 0; //R
        ws28128_color_buf[led_location - 27][1] = 0; //G
        ws28128_color_buf[led_location - 27][2] = 255; //blue


    }



}




/***全亮数据移位,第几个位置灯的颜色，由我设置**/
/***配色函数***********************/
void DMA_WS2812_data_shift_light(uint16_t led_location, uint16_t led_max)
{

    if(led_location < (led_max / 4)) //gree
    {
        ws28128_color_buf[led_location][0] = 255; //R
        ws28128_color_buf[led_location][1] = 0; //G
        ws28128_color_buf[led_location][2] = 0; //blue
    }

    else if(led_location > (led_max / 4 - 1) && (led_location < (led_max / 2))) //blue
    {
        ws28128_color_buf[led_location][0] = 0; //R
        ws28128_color_buf[led_location][1] = 0; //G
        ws28128_color_buf[led_location][2] = 255; //blue

    }
    else if(led_location > (led_max / 2 - 1) && (led_location < (led_max / 4) * 3)) //blue
    {
        ws28128_color_buf[led_location][0] = 255; //R
        ws28128_color_buf[led_location][1] = 0; //G
        ws28128_color_buf[led_location][2] = 0; //blue

    }
    else if((led_location > (led_max / 4) * 3 - 1) && (led_location < led_max)) //blue
    {

        ws28128_color_buf[led_location][0] = 0; //R
        ws28128_color_buf[led_location][1] = 255; //G
        ws28128_color_buf[led_location][2] = 0; //blue

    }


}





void  arrange_buf()
{




}



void DMA_WS2812_data_shift_light_one_run(uint16_t led_location, uint16_t led_max, uint16_t color_ytpr)
{
    uint8_t r, g, b;


    for(uint16_t i = 0; i < led_max; i++)
    {
        if(i < (led_max / 4))
        {

            HLS_TO_RGB_ONE(&r, &g, &b, 1 * 5, 0.30, 1, i + led_location, ws28128_color_buf);

        }

        else if(i > (led_max / 4 - 1) && (i < (led_max / 2)))
        {

            HLS_TO_RGB_ONE(&r, &g, &b, 1 * 300, 0.30, 1, i + led_location, ws28128_color_buf);
        }
        else if(i > (led_max / 2 - 1) && (i < (led_max / 4) * 3))
        {

            HLS_TO_RGB_ONE(&r, &g, &b, 1 * 200, 0.30, 1, i + led_location, ws28128_color_buf);
        }
        else if((i > (led_max / 4) * 3 - 1) && (i < led_max))
        {
            HLS_TO_RGB_ONE(&r, &g, &b, 1 * 100, 0.30, 1, i + led_location, ws28128_color_buf);
        }
    }


}




void  	rand_buff_data(volatile uint16_t  amount, uint16_t color_type_n)
{

    uint16_t rand_data, rand_color, rand_col;

    uint8_t r, g, b;
    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    for(uint16_t t = 0; t < 30; t++)
    {

        srand(HAL_GetTick());
        rand_data = rand() % amount;
        rand_col = rand() % 100;
        rand_color = rand() % 360;
        HLS_TO_RGB_ONE(&r, &g, &b, rand_color, rand_col * 0.01, 1, rand_data, ws28128_color_buf);
    }


}



/**
  *@brief gets the maximun value of rgb
	*@param r_data   value of red range( 0-255)
  *@param g_data   value of red range( 0-255)
	*@param b_data   value of red range( 0-255)
	*@retval   renturn RGB maximum number
  */
uint8_t get_rgb_max_value(uint8_t r_data, uint8_t g_data, uint8_t b_data)
{
    uint8_t temp_value;

    temp_value = (r_data) > (g_data) ? (r_data) : (g_data);
    temp_value = (b_data) > (temp_value) ? (b_data) : (temp_value);


    return temp_value;
}
/**
  *@brief gets the minimum value of rgb
  *@param r_data   value of red range( 0-255)
  *@param g_data   value of red range( 0-255)
	*@param b_data   value of red range( 0-255)
  */
double get_rgb_min_value(double r_data, double g_data, double b_data)
{
    double temp_value;

    temp_value = (r_data) < (g_data) ? (r_data) : (g_data);
    temp_value = (b_data) < (temp_value) ? (b_data) : (temp_value);


    return temp_value;
}
/**
  *@brizf
  *
  */

void RGB2HLS(double* h, double* l, double* s, uint8_t r_data, uint8_t g_data, uint8_t b_data)
{
    double dr = (double)r_data / 255;
    double dg = (double)g_data / 255;
    double db = (double)b_data / 255;

    double cmax = get_rgb_max_value(dr, dg, db);
    double cmin = get_rgb_min_value(dr, dg, db);
    double cdes = cmax - cmin;
    double hh, ll, ss;

    ll = (cmax + cmin) / 2.0;
    if(cdes == 0)
    {
        hh = 0;
        ss = 0;
    }
    else
    {
        if(ll < 0.5)
            ss = (cmax - cmin) / (cmax + cmin);
        else
            ss = (cmax - cmin) / (2 - cmax - cmin);

        if(cmax == dr)
            hh = (0 + (dg - db) / cdes) * 60;
        else if(cmax == dg)
            hh = (2 + (db - dr) / cdes) * 60;
        else// if(cmax == b)
            hh = (4 + (dr - dg) / cdes) * 60;
        if(hh < 0)
            hh += 360;
    }

    *h = hh;
    *l = ll;
    *s = ss;
}

double HLS2RGBvalue(double n1, double n2, double hue)
{
    if(hue > 360)
        hue -= 360;
    else if(hue < 0)
        hue += 360;
    if(hue < 60)
        return n1 + (n2 - n1) * hue / 60;
    else if(hue < 180)
        return n2;
    else if(hue < 240)
        return n1 + (n2 - n1) * (240 - hue) / 60;
    else
        return n1;
}
void HLS2RGB(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s)
{
    double cmax, cmin;

    if(l < 0.5)
        cmax = l * (1 + s);
    else
        cmax = l * (1 - s) + s;
    cmin = 2 * l - cmax;

    if(s == 0)
    {
        *r = *g = *b = l * 255;
    }
    else
    {
        *r = HLS2RGBvalue(cmin, cmax, h + 120) * 255;
        *g = HLS2RGBvalue(cmin, cmax, h) * 255;
        *b = HLS2RGBvalue(cmin, cmax, h - 120) * 255;
    }
}

void HLS_TO_RGB_ALL(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n, uint8_t (*arr)[3])
{
    double cmax, cmin;

    if(l < 0.5)
        cmax = l * (1 + s);
    else
        cmax = l * (1 - s) + s;
    cmin = 2 * l - cmax;

    if(s == 0)
    {
        *r = *g = *b = l * 255;
        for(uint16_t i = 0; i < led_n; i++)
        {
            for(uint16_t j = 0; j < 3; j++)
            {
                ws28128_color_buf[i][j] = l * 255;
//						 	printf("i0=%d:%d\r\n", i, ws28128_color_buf[i][j]);
            }
        }
    }
    else
    {
        *r = HLS2RGBvalue(cmin, cmax, h + 120) * 255;
        *g = HLS2RGBvalue(cmin, cmax, h) * 255;
        *b = HLS2RGBvalue(cmin, cmax, h - 120) * 255;
        for(uint16_t i = 0; i < led_n; i++)
        {

            ws28128_color_buf[i][0] = HLS2RGBvalue(cmin, cmax, h + 120) * 255;
            ws28128_color_buf[i][1] = HLS2RGBvalue(cmin, cmax, h) * 255;
            ws28128_color_buf[i][2] = HLS2RGBvalue(cmin, cmax, h - 120) * 255;
//					printf("i0=%d:%d\r\n", i, ws28128_color_buf[i][0]);
//					printf("i1=%d:%d\r\n",  i,ws28128_color_buf[i][1]);
//					printf("i2=%d:%d\r\n",  i,ws28128_color_buf[i][2]);

        }
    }
}

void HLS_TO_RGB_ONE(uint8_t* r, uint8_t* g, uint8_t* b, double h, double l, double s, uint16_t led_n_LOC, uint8_t (*arr)[3])
{
    double cmax, cmin;

    if(l < 0.5)
        cmax = l * (1 + s);
    else
        cmax = l * (1 - s) + s;
    cmin = 2 * l - cmax;

    if(s == 0)
    {
        *r = *g = *b = l * 255;

        for(uint16_t j = 0; j < 3; j++)
        {
            ws28128_color_buf[led_n_LOC][j] = l * 255;

        }

    }
    else
    {
        *r = HLS2RGBvalue(cmin, cmax, h + 120) * 255;
        *g = HLS2RGBvalue(cmin, cmax, h) * 255;
        *b = HLS2RGBvalue(cmin, cmax, h - 120) * 255;

        ws28128_color_buf[led_n_LOC][0] = HLS2RGBvalue(cmin, cmax, h + 120) * 255;
        ws28128_color_buf[led_n_LOC][1] = HLS2RGBvalue(cmin, cmax, h) * 255;
        ws28128_color_buf[led_n_LOC][2] = HLS2RGBvalue(cmin, cmax, h - 120) * 255;

    }
}
void reset_led_light()
{
     memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
	      DMA_WS2812_Reset();
          osDelay(1);
        DMA_WS2812_light(LED_MAX);
			osDelay((uint32_t)(LED_MAX*30/1000)+1);
}