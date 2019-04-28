/**
  ******************************************************************************
  * @file    ws2812_app.c
  * @author  FuBiao.Guo
  * @version V1.1.0
  * @date    2-4-2019
  * @brief   The modulation effect of ws2812 function file.
	**/

#include  "ws2812_app.h"
#include  "bsp_ws2812.h"
#include <stdlib.h>
#include "string.h"
#include   "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include  "math.h"
#include   "bsp_dma.h"
#include "string.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"


/*************重要说明*******************/
extern uint8_t   ws28128_color_buf[1000][3];
extern uint8_t light_flag;
uint8_t tx_comple_flag;
/**
  * @brief Randomly different colors
  * @param  uint16_t amount :  maximum LED number of ws28128
  * @retval None
  */


void ws2812_rand_light(volatile uint16_t  amount)
{
    uint16_t rand_data = 0, rand_color = 0;
    for(uint16_t i = 0; i < 360; i++)
    {
        rand_data = rand() % 256;
        rand_buff_data(amount, i);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);
    }
}

/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********每个色循环往下跑**********未完成****/
void  arrange_display_one_run(volatile uint16_t  amount, uint16_t color_type_n)
{
    uint8_t r, g, b;

    for(uint16_t m = 1; m < 90; m++)
    {
        for(uint16_t i = 0; i < amount * 2; i++)
        {
            if(i < (amount / 4)) //i=44
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //5

            }

            else if(i > (amount / 4 - 1) && (i < (amount / 2))) //i=45  i=89
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf);  //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //1

            }
            else if(i > (amount / 2 - 1) && (i < (amount / 4) * 3))//i=90  i=135
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //3
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 20, 0.30, 1, (i - (amount / 4) * 2), ws28128_color_buf); //3
            }
            else if((i > (amount / 4) * 3 - 1) && (i < amount * 2)) //i=135  i=180
            {
                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //4ge
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //3
                HLS_TO_RGB_ONE(&r, &g, &b, m * 20, 0.30, 1, (i - (amount / 4) * 2), ws28128_color_buf); //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 30, 0.30, 1, (i - (amount / 4) * 3), ws28128_color_buf); //1
            }

            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
           HAL_Delay((uint32_t)(amount*30/1000)+1);

        }
    }
}
/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********每个色转一遍**************/
void  arrange_display_two_run(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < 10; m++)
    {
    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
        for(uint16_t i = 0; i < amount / 2; i++)
        {
   
            HLS_TO_RGB_ONE(&r, &g, &b, m * 15, 0.30, 1, i, ws28128_color_buf);
            HLS_TO_RGB_ONE(&r, &g, &b, m * 15, 0.30, 1, amount - i, ws28128_color_buf);
            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
        HAL_Delay((uint32_t)(amount*30/1000)+1);
				}
   
		
		
        memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
        for(uint16_t i = 0; i < amount / 2; i++)
        {

            HLS_TO_RGB_ONE(&r, &g, &b, m * 15, 0.30, 1, amount / 2 - i, ws28128_color_buf);
            HLS_TO_RGB_ONE(&r, &g, &b, m * 15, 0.30, 1, amount / 2 + i, ws28128_color_buf);
            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
         HAL_Delay((uint32_t)(amount*30/1000)+1);
         }
	}
}
/***多种颜色一起渐变***************/
void DMA_WS2812_Rampping(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
	memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    for(uint16_t m = 0; m < pwm; m++)
    {


        DMA_WS2812_SIN_More(amount, m, 1);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay((uint32_t)(amount*30/1000)+1);


    }

}

/***********色表**************/
void  arrange_display(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1, 0.30, 1, m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay((amount*30/1000)+1);
		
    }
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((amount*30/1000)+1);
	
    }


}
/***********色表2合一**************/
void  arrange_display_two(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, m, ws28128_color_buf);
        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay((uint32_t)(amount*30/1000)+1);
			
    }
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);
			 
    }


}
/*********单中颜色渐变***************/
void DMA_WS2812_Rampping_1(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
    uint8_t r, g, b;
     	reset_led_light();
    for(uint16_t m = 0; m < pwm; m++)
    {

//      HLS2RGB( uint8_t *r, uint8_t *g, uint8_t *b, double h, double l, double s);
//        DMA_WS2812_SIN(amount, m,  color);
        //		HLS_TO_RGB( uint8_t *r, uint8_t *g, uint8_t *b, double h, double l, double s,uint16_t led_n,uint8_t arr[][3])
        HLS_TO_RGB_ALL(&r, &g, &b, m * 1.4, 0.30, 1, amount, ws28128_color_buf);
//		   	DMA_WS2812_SIN(amount, m,  2);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);


    }


}

void DMA_WS2812_Running(volatile uint16_t  amount)
{
    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift(m);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);
    }
		  for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift(amount-m);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);
    }



}
void DMA_WS2812_Run(volatile uint16_t  amount)
{
    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift_light(m, amount);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
          HAL_Delay((uint32_t)(amount*30/1000)+1);
			 }
	
       

   memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    for(uint16_t m = 0; m < amount; m++)
    {
     
        DMA_WS2812_data_shift_light(amount - m, amount);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
       HAL_Delay((uint32_t)(amount*30/1000)+1);
			 
      }

			  
	   
memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));

//     DMA_WS2812_Mie(amount);

}
void DMA_WS2812_Ramp(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
//	   reset_led_light();
    for(uint16_t m = 0; m < pwm; m++)
    {
	    
 
     
			 DMA_WS2812_SIN(amount, m, color);
  	     DMA_WS2812_Reset();
          osDelay(1);
        DMA_WS2812_light(amount);
			osDelay((uint32_t)(amount*30/1000)+1);
 
		 

//		  HAL_Delay(1);  //调节渐变快慢
    }

}
/**单中颜色全亮**************/
void DMA_WS2812_one_light_run(volatile uint16_t  amount, colors_kind color)
{
    reset_led_light();//添加这个白光会抖，注释掉白光很稳定
    
        DMA_WS2812_Reset();
        osDelay(1);
	 DMA_WS2812_one_light(amount,color);
        DMA_WS2812_light(amount);
        osDelay((uint32_t)(amount*30/1000)+1);
	  DMA_WS2812_Reset();
        
			 

}

/***********多条灯跑起来****************/
void DMA_WS2812_Running_more(volatile uint16_t  amount, volatile uint16_t run_number)
{
    for(uint16_t m = 0; m < amount; m++)
    {

     
        DMA_WS2812_data_shift_more(m, 10);
        DMA_WS2812_Reset();
         HAL_Delay(1);
        DMA_WS2812_light(amount);
         HAL_Delay((uint32_t)(amount*30/1000)+1);
			
		}
    for(uint16_t m = 0; m < amount; m++)
    {
     
        DMA_WS2812_data_shift_more(amount - m, 10);
        DMA_WS2812_Reset();
         HAL_Delay(1);
        DMA_WS2812_light(amount);
   HAL_Delay((uint32_t)(amount*30/1000)+1);
			 
		}
}
/**************logo颜色逐步亮起来*******************/
  //    1.第一步，创建一个二维数组
/****************************************************/
/***多个灭数据移位************/
void DMA_WS2812_circulation_buf(uint16_t led_location, uint16_t run_number)
{

// memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
	/**start****1---4******/
	   if((led_location>=0)&&(led_location<4))
		 {
    ws28128_color_buf[led_location][0] = 255; //R

		 }
		 //***2-----1***/
		 else if((led_location>=4)&&(led_location<5))
		 {
		ws28128_color_buf[led_location][1] = 255; //G
		 }
		 /**3------6****/
		 else if((led_location>=5)&&(led_location<11))
		 {
			ws28128_color_buf[led_location][2] = 255; //G
		 }
		 /***4-----14*******/
		 else if((led_location>=11)&&(led_location<25))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		 	/***5-----28*******/
		 else if((led_location>=25)&&(led_location<53))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 255; //r
		 }
		 /****6-----8**********/
		 	 else if((led_location>=53)&&(led_location<61))
		 {
			 ws28128_color_buf[led_location][2] = 255; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
	  /****7-----6**********/
		 	 else if((led_location>=61)&&(led_location<67))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		  /****8-----19**********/
		 	 else if((led_location>=67)&&(led_location<86))
		 {
			 ws28128_color_buf[led_location][2] = 255; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		 		  /****9-----28**********/
		 	 else if((led_location>=86)&&(led_location<114))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }	 		  /****10-----10**********/
		 	 else if((led_location>=114)&&(led_location<124))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 255; //r
		 }		
		 	 		  /****11-----22**********/
		 	 else if((led_location>=124)&&(led_location<146))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		  	 		  /****12-----33**********/
		 	 else if((led_location>=146)&&(led_location<179))
		 {
			 ws28128_color_buf[led_location][2] = 255; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }		  	 		
          		 /****13-----5**********/
		 	 else if((led_location>=179)&&(led_location<184))
		 {
			 ws28128_color_buf[led_location][2] = 255; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
          		 /****14-----3**********/
		 	 else if((led_location>=184)&&(led_location<187))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 255; //r
		 }
		         		 /****15-----14**********/
		 	 else if((led_location>=187)&&(led_location<201))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 255; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		 		         		 /****16-----14**********/
		 	 else if((led_location>=201)&&(led_location<215))
		 {
			 ws28128_color_buf[led_location][2] = 255; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 0; //r
		 }
		 	 		         		 /****16-----14**********/
		 	 else if((led_location>=215)&&(led_location<229))
		 {
			 ws28128_color_buf[led_location][2] = 0; //b
			 ws28128_color_buf[led_location][1] = 0; //g
			 ws28128_color_buf[led_location][0] = 255; //r
		 }


}
void DMA_WS2812_circulation(volatile uint16_t  amount)
{
	
	
	 for(uint16_t m = 0; m < amount; m++)
    {

  
        DMA_WS2812_circulation_buf(m, 10);  //M代表数组第一例
        DMA_WS2812_Reset();
      	   HAL_Delay(1);
        DMA_WS2812_light(amount);
         HAL_Delay((uint32_t)(amount*30/1000)+1);
			   HAL_Delay(10);
			
		}
	
}
void DMA_WS2812_circulation_buf_more(uint16_t led_location,uint8_t color_ty)
{

// memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
 
	   if(led_location==0)
		 {
			  for(uint8_t t=0;t<28;t++)
			 {
     ws28128_color_buf[t+25][color_ty] = 255; //g
			 }
		 ws28128_color_buf[66][color_ty] = 255; //g
	   ws28128_color_buf[65][color_ty] = 255; //g
		 }
    if(led_location==1)
		 {
	          		 ws28128_color_buf[24][color_ty] = 255; //g
			 		       ws28128_color_buf[23][color_ty] = 255; //g
			           ws28128_color_buf[22][color_ty] = 255; //g
			           ws28128_color_buf[64][color_ty] = 255; //g
			 
		 }
		if(led_location==2)
		 {
	          		 ws28128_color_buf[63][color_ty] = 255; //g
			 	         ws28128_color_buf[62][color_ty] = 255; //g
			 	         ws28128_color_buf[61][color_ty] = 255; //g
			 		       ws28128_color_buf[21][color_ty] = 255; //g
			           ws28128_color_buf[20][color_ty] = 255; //g
			           ws28128_color_buf[19][color_ty] = 255; //g
			 
		 }
		 		if(led_location==3)
		 {
	          		 ws28128_color_buf[63][color_ty] = 255; //g
			 	         ws28128_color_buf[62][color_ty] = 255; //g
			 	         ws28128_color_buf[61][color_ty] = 255; //g
			 		       ws28128_color_buf[21][color_ty] = 255; //g
			           ws28128_color_buf[20][color_ty] = 255; //g
			           ws28128_color_buf[19][color_ty] = 255; //g
			 
		 }
		 	 		if(led_location==4)
		 {
			           ws28128_color_buf[0][color_ty] = 255; //g
	          		 ws28128_color_buf[1][color_ty] = 255; //g
			 	         ws28128_color_buf[2][color_ty] = 255; //g
			 	         ws28128_color_buf[3][color_ty] = 255; //g
			 		       ws28128_color_buf[4][color_ty] = 255; //g
			           ws28128_color_buf[18][color_ty] = 255; //g
			           ws28128_color_buf[53][color_ty] = 255; //g
			 
		 }
		 	 	 		if(led_location==5)
		 {
			           ws28128_color_buf[67][color_ty] = 255; //g
	          		 ws28128_color_buf[54][color_ty] = 255; //g
			 	         ws28128_color_buf[55][color_ty] = 255; //g
			 	         ws28128_color_buf[56][color_ty] = 255; //g
			 		       ws28128_color_buf[57][color_ty] = 255; //g
			           ws28128_color_buf[17][color_ty] = 255; //g
			           ws28128_color_buf[16][color_ty] = 255; //g
			           ws28128_color_buf[15][color_ty] = 255; //g
			           ws28128_color_buf[14][color_ty] = 255; //g
			 
		 }
		 	 	 	 		if(led_location==6)
		 {
			           ws28128_color_buf[58][color_ty] = 255; //g
	          		 ws28128_color_buf[59][color_ty] = 255; //g
			 	         ws28128_color_buf[60][color_ty] = 255; //g
			 	         ws28128_color_buf[68][color_ty] = 255; //g
			           ws28128_color_buf[69][color_ty] = 255; //g
			           ws28128_color_buf[70][color_ty] = 255; //g
			           ws28128_color_buf[71][color_ty] = 255; //g
			           ws28128_color_buf[5][color_ty] = 255; //g
	          		 ws28128_color_buf[6][color_ty] = 255; //g
			 	         ws28128_color_buf[7][color_ty] = 255; //g
			 	         ws28128_color_buf[8][color_ty] = 255; //g
			 		       ws28128_color_buf[9][color_ty] = 255; //g
			           ws28128_color_buf[10][color_ty] = 255; //g
			           ws28128_color_buf[13][color_ty] = 255; //g
			           ws28128_color_buf[12][color_ty] = 255; //g
			           ws28128_color_buf[11][color_ty] = 255; //g
			 
		 }
		 		 	 	 	 		if(led_location==7)
		 {
			           ws28128_color_buf[72][color_ty] = 255; //g
	          		 ws28128_color_buf[73][color_ty] = 255; //g
			 	         ws28128_color_buf[74][color_ty] = 255; //g
			 	         ws28128_color_buf[75][color_ty] = 255; //g
			           ws28128_color_buf[76][color_ty] = 255; //g
			      
		 }
		 		 		 	 	 	 		if(led_location==8)
		 {
			           ws28128_color_buf[77][color_ty] = 255; //g
	          		 ws28128_color_buf[78][color_ty] = 255; //g
			 	         ws28128_color_buf[79][color_ty] = 255; //g
			 	         ws28128_color_buf[80][color_ty] = 255; //g
			           ws28128_color_buf[81][color_ty] = 255; //g
			           ws28128_color_buf[146][color_ty] = 255; //g
	          		 ws28128_color_buf[147][color_ty] = 255; //g
			 	         ws28128_color_buf[148][color_ty] = 255; //g
			 	         ws28128_color_buf[149][color_ty] = 255; //g
			           ws28128_color_buf[150][color_ty] = 255; //g
			           ws28128_color_buf[151][color_ty] = 255; //g
			           ws28128_color_buf[152][color_ty] = 255; //g
			      
		 }
		 		           if(led_location==9)
		 {
			           ws28128_color_buf[82][color_ty] = 255; //g
	          		 ws28128_color_buf[83][color_ty] = 255; //g
			 	         ws28128_color_buf[84][color_ty] = 255; //g
			 	         ws28128_color_buf[85][color_ty] = 255; //g
			 
			           ws28128_color_buf[145][color_ty] = 255; //g
			           ws28128_color_buf[144][color_ty] = 255; //g
	          		 ws28128_color_buf[143][color_ty] = 255; //g
			 	         ws28128_color_buf[142][color_ty] = 255; //g
			 
			 	         ws28128_color_buf[153][color_ty] = 255; //g
			           ws28128_color_buf[154][color_ty] = 255; //g
			           ws28128_color_buf[155][color_ty] = 255; //g
			           ws28128_color_buf[156][color_ty] = 255; //g
			           ws28128_color_buf[157][color_ty] = 255; //g
			           ws28128_color_buf[158][color_ty] = 255; //g
			      
		 }
		 		 		           if(led_location==10)
		 {
			           ws28128_color_buf[86][color_ty] = 255; //g
	          		 ws28128_color_buf[87][color_ty] = 255; //g
			 	         ws28128_color_buf[88][color_ty] = 255; //g
			 	         ws28128_color_buf[89][color_ty] = 255; //g
			 
			           ws28128_color_buf[141][color_ty] = 255; //g
			           ws28128_color_buf[140][color_ty] = 255; //g
	          		 ws28128_color_buf[139][color_ty] = 255; //g
			 	         ws28128_color_buf[138][color_ty] = 255; //g
			 
			 
			           ws28128_color_buf[159][color_ty] = 255; //g
			           ws28128_color_buf[160][color_ty] = 255; //g
			           ws28128_color_buf[161][color_ty] = 255; //g
			           ws28128_color_buf[162][color_ty] = 255; //g
			           ws28128_color_buf[163][color_ty] = 255; //g
			 
			 
			           ws28128_color_buf[183][color_ty] = 255; //g
			           ws28128_color_buf[184][color_ty] = 255; //g
			           ws28128_color_buf[185][color_ty] = 255; //g
			           ws28128_color_buf[186][color_ty] = 255; //g
			 
			 
			 	        
			         
			      
		 }
		 	 		 		           if(led_location==11)
		 {
			           ws28128_color_buf[90][color_ty] = 255; //g
	          		 ws28128_color_buf[91][color_ty] = 255; //g
			 	         ws28128_color_buf[92][color_ty] = 255; //g
			 	  
			 
			           ws28128_color_buf[137][color_ty] = 255; //g
			           ws28128_color_buf[136][color_ty] = 255; //g
			          
	          	
			           ws28128_color_buf[164][color_ty] = 255; //g
			           ws28128_color_buf[165][color_ty] = 255; //g
			           ws28128_color_buf[166][color_ty] = 255; //g
			 
			           ws28128_color_buf[182][color_ty] = 255; //g
			           ws28128_color_buf[181][color_ty] = 255; //g
			  			             
		 }
		 		 	 		 		    if(led_location==12)
		 {
			           ws28128_color_buf[93][color_ty] = 255; //g
	          		 ws28128_color_buf[94][color_ty] = 255; //g
			 	         ws28128_color_buf[95][color_ty] = 255; //g
			 	  
	          		 ws28128_color_buf[135][color_ty] = 255; //g
			 	         ws28128_color_buf[134][color_ty] = 255; //g
			           ws28128_color_buf[133][color_ty] = 255; //g 
			           ws28128_color_buf[132][color_ty] = 255; //g 
			           ws28128_color_buf[131][color_ty] = 255; //g
			       
			 
			           ws28128_color_buf[167][color_ty] = 255; //g
			           ws28128_color_buf[168][color_ty] = 255; //g
			           ws28128_color_buf[169][color_ty] = 255; //g
		          	 ws28128_color_buf[170][color_ty] = 255; //g
			 
			  			   ws28128_color_buf[180][color_ty] = 255; //g
			           ws28128_color_buf[179][color_ty] = 255; //g
			 
			         
			 
			          
		
		 }		 		 	 		 		    if(led_location==13)
		 {
			           ws28128_color_buf[96][color_ty] = 255; //g
	          		 ws28128_color_buf[97][color_ty] = 255; //g
			 	         ws28128_color_buf[98][color_ty] = 255; //g
			           ws28128_color_buf[99][color_ty] = 255; //g
			  	  
	         
			           ws28128_color_buf[130][color_ty] = 255; //g
			 	         ws28128_color_buf[129][color_ty] = 255; //g
			           ws28128_color_buf[128][color_ty] = 255; //g
			 	  
			       
			 
			           ws28128_color_buf[171][color_ty] = 255; //g
			           ws28128_color_buf[172][color_ty] = 255; //g
			  			   ws28128_color_buf[173][color_ty] = 255; //g
			 
			       
		
		 } 		 	 		 		    if(led_location==14)
		 {
			           ws28128_color_buf[100][color_ty] = 255; //g
	          		 ws28128_color_buf[101][color_ty] = 255; //g
			 	         ws28128_color_buf[102][color_ty] = 255; //g
			           ws28128_color_buf[103][color_ty] = 255; //g
			  	  
	         
			           ws28128_color_buf[127][color_ty] = 255; //g
			 	         ws28128_color_buf[126][color_ty] = 255; //g
			           ws28128_color_buf[125][color_ty] = 255; //g
			 	  
			       
			 
			           ws28128_color_buf[174][color_ty] = 255; //g
			           ws28128_color_buf[175][color_ty] = 255; //g
			  			   ws28128_color_buf[176][color_ty] = 255; //g
			 
			       
		
		 }
		 		 	 		 		    if(led_location==15)
		 {
			           ws28128_color_buf[104][color_ty] = 255; //g
	          		 ws28128_color_buf[105][color_ty] = 255; //g
			 	         ws28128_color_buf[106][color_ty] = 255; //g
			           ws28128_color_buf[107][color_ty] = 255; //g
			  	  
	         
			           ws28128_color_buf[124][color_ty] = 255; //g
			 	      
			 	  
			       
			 
			           ws28128_color_buf[177][color_ty] = 255; //g
			           ws28128_color_buf[178][color_ty] = 255; //g
			 
			           ws28128_color_buf[187][color_ty] = 255; //g
			           ws28128_color_buf[188][color_ty] = 255; //g
			           ws28128_color_buf[189][color_ty] = 255; //g
			           ws28128_color_buf[190][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
		 	 	 		 		    if(led_location==16)
		 {
			           ws28128_color_buf[108][color_ty] = 255; //g
	          		 ws28128_color_buf[109][color_ty] = 255; //g
			 	         ws28128_color_buf[110][color_ty] = 255; //g
			           ws28128_color_buf[111][color_ty] = 255; //g
			  	  
	         
			           ws28128_color_buf[123][color_ty] = 255; //g
			 	          ws28128_color_buf[122][color_ty] = 255; //g
			            ws28128_color_buf[121][color_ty] = 255; //g
			           ws28128_color_buf[120][color_ty] = 255; //g
			 	          ws28128_color_buf[119][color_ty] = 255; //g
			            ws28128_color_buf[118][color_ty] = 255; //g
			 	  
			     
			 
			           ws28128_color_buf[191][color_ty] = 255; //g
			           ws28128_color_buf[192][color_ty] = 255; //g
			           ws28128_color_buf[193][color_ty] = 255; //g
			           ws28128_color_buf[194][color_ty] = 255; //g
			 
			           ws28128_color_buf[228][color_ty] = 255; //g
								 ws28128_color_buf[229][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
		 
		 		 	 	 		 		    if(led_location==17)
		 {
			           ws28128_color_buf[112][color_ty] = 255; //g
	          		 ws28128_color_buf[113][color_ty] = 255; //g
			 	        
	         
			           ws28128_color_buf[117][color_ty] = 255; //g
			 	         ws28128_color_buf[116][color_ty] = 255; //g
			          
			 	  
			     
			 
			           ws28128_color_buf[195][color_ty] = 255; //g
			           ws28128_color_buf[196][color_ty] = 255; //g
			           ws28128_color_buf[197][color_ty] = 255; //g
			           
			 
			           ws28128_color_buf[227][color_ty] = 255; //g
								 ws28128_color_buf[226][color_ty] = 255; //g
			           ws28128_color_buf[225][color_ty] = 255; //g
			           ws28128_color_buf[224][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
		 		 	 	 		 		    if(led_location==18)
		 {
			          
	         
			           ws28128_color_buf[115][color_ty] = 255; //g
			 	         ws28128_color_buf[114][color_ty] = 255; //g
			          
			 	  
			     
			 
			           ws28128_color_buf[198][color_ty] = 255; //g
			           ws28128_color_buf[199][color_ty] = 255; //g
			           ws28128_color_buf[200][color_ty] = 255; //g
			           
			 
			           ws28128_color_buf[223][color_ty] = 255; //g
								 ws28128_color_buf[222][color_ty] = 255; //g
			           ws28128_color_buf[221][color_ty] = 255; //g
			           ws28128_color_buf[220][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
		  	 	 		 		    if(led_location==19)
		 {
			          
	         
			          
			          
			 	  
			     
			         
			           ws28128_color_buf[201][color_ty] = 255; //g
			           ws28128_color_buf[202][color_ty] = 255; //g
			           ws28128_color_buf[203][color_ty] = 255; //g
			           ws28128_color_buf[204][color_ty] = 255; //g
			           ws28128_color_buf[205][color_ty] = 255; //g
			           ws28128_color_buf[206][color_ty] = 255; //g
			           ws28128_color_buf[207][color_ty] = 255; //g
			           ws28128_color_buf[208][color_ty] = 255; //g
			           ws28128_color_buf[209][color_ty] = 255; //g
			           ws28128_color_buf[210][color_ty] = 255; //g
			           ws28128_color_buf[211][color_ty] = 255; //g
			           ws28128_color_buf[212][color_ty] = 255; //g
			           ws28128_color_buf[213][color_ty] = 255; //g
			           ws28128_color_buf[214][color_ty] = 255; //g
			         
			           ws28128_color_buf[219][color_ty] = 255; //g
								 ws28128_color_buf[218][color_ty] = 255; //g
			           ws28128_color_buf[217][color_ty] = 255; //g
			           ws28128_color_buf[216][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
		 	 	 		 		    if(led_location==19)
		 {
			          
	         
			          
			          
			 	  
			     
			       
			         
			           ws28128_color_buf[215][color_ty] = 255; //g
			 
			  			 
			 
			       
		
		 }
	 
	 




}
void DMA_WS2812_circulation_more(volatile uint16_t  amount,colors_kind color)
{
	memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
	 
	 for(uint16_t m = 0; m < 20; m++)
    {

  
        DMA_WS2812_circulation_buf_more(m, color);  //M代表数组第一例
        DMA_WS2812_Reset();
      	   osDelay(1);
        DMA_WS2812_light(amount);
         osDelay((uint32_t)(amount*30/1000)+1);
	         osDelay(30);
			
		}
			memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
			 for(uint16_t m = 19; m >0; m--)
    {

  
        DMA_WS2812_circulation_buf_more(m, color);  //M代表数组第一例
        DMA_WS2812_Reset();
      	   osDelay(1);
        DMA_WS2812_light(amount);
         osDelay((uint32_t)(amount*30/1000)+1);
	         osDelay(30);
			
		}
	
}








