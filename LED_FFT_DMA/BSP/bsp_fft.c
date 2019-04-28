#include  "bsp_fft.h"
#include "stm32f1xx_hal.h"
#include   "bsp_ws2812.h"

extern uint16_t ab[100];
extern uint8_t   ws28128_color_buf[300][3];


#define BIT_1                      61u //1比较值为61 850us
#define BIT_0                      28u //0比较值为28 400us 

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
/*FUNCTION_ERROR**************/
void led_fft(uint16_t x_1,uint16_t y_1,colors_kind  color,volatile uint8_t pwm,float *arr)
{     
	     //10K  采样频率,人听到是5Khz,采样点为512,   10000/512=19.5HZ,去10个坐标显示  最大显示频率 10*19.5=195HZ
		uint16_t    amount_size = 0;
	     uint8_t  pwm_1 = 0;
	       uint8_t        x1[x_1]; //一列显示的灯数
	     amount_size = (x_1*y_1*24+1);

	         pwm_1 = pwm;
	/***************10列的灯数********************/

	    for(uint16_t i=0;i<x_1;i++)
     {
         
                   x1[i]  =(arr[i+7]/4095);   //一列显示的灯数
           printf("%d=%d\r\n", i,x1[i]);

     } 
//		   printf("一组\r\n" );
		 for(uint16_t i=0;i<x_1;i++) //列
		 {
           for(uint16_t j=0;j<y_1;j++)  //行数
			 {     
				            //1列有多少行灯亮
				         if((i%2)==0)
								 {
		                if(j< x1[i])
										{
                                pwm = pwm_1;
											      for(uint16_t k=0;k<24;k++)
											  {
													
													   if(color == 1)
                          {
                               if(k < 8)
                                {

                                  if(pwm & 0x80)
                               {
                                 ab[(i*24*y_1)+(j*24) + k] = BIT_1; //1

                               }
                                 else
                                 {
                                 ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0
                                 }
                              pwm = pwm << 1;

                                 }
                           else
                            {
  

                            ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0


                             }
                          }
														   if(color == 2)
                          {
                                if(k < 8)
                                    {
                                    ab[(i*24*y_1)+(j*24) + k] = BIT_1; //1

                                      }
                              else if((7 < k) && (k < 16))
                {

                     ab[(i*24*y_1)+(j*24) + k] = BIT_1; //1


                }
                else
                {


                   ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0

                }
                          }
//                             
								    }
										}											
                 else	
								 {
                            for(uint16_t k=0;k<24;k++)
											  {
                               ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0
										    }
                       
								 }	
							 }
								 else
								 {
									 
									    if(j<(y_1-x1[i]))
										{
											    for(uint16_t k=0;k<24;k++)
											  {
                               ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0
										    }
                       
											
                             
//                             
								    }
																			
                 else	
								 {
                       pwm = pwm_1;
											      for(uint16_t k=0;k<24;k++)
											  {
													
													   if(color == 1)
                          {
                               if(k < 8)
                                {

                                  if(pwm & 0x80)
                               {
                                 ab[(i*24*y_1)+(j*24) + k] = BIT_1; //1

                               }
                                 else
                                 {
                                 ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0
                                 }
                              pwm = pwm << 1;

                                 }
                           else
                            {
  

                            ab[(i*24*y_1)+(j*24) + k] = BIT_0; //0


                             }
                          }    
								 }	
									 	}	
								 }
								 
				            
				 
				 
			 }
			 
			 
		 }

		       ab[amount_size] = 0;
		 
//	    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ab,amount_size);  
}


//   arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_f32_10khz, ifftFlag, doBitReverse);
//  /* Process the data through the Complex Magnitude Module for
//  calculating the magnitude at each bin */
//    arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);


/********************************************************************
             *one_y_dspaly    一列颜色
*********************************************************************/
   
/*
 *x_location  列位置  color  颜色  pwm 颜色亮度    led_n 某列亮灯数目        y_ledmax 该列最大能显示灯数
*/
void one_y_dspaly(uint16_t x_location ,colors_kind color, uint8_t pwm,uint8_t led_n,uint16_t y_ledmax)
{   
	       if(led_n>8)
				 {
					 led_n=8;
					 
				 }

	  if(color==green)
		{
		    for(uint16_t j=0;j<8;j++)
		 {
			   if((x_location%2)!=0)
				 {
			    if(j<led_n)
					{
	    ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=pwm;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					else
					{
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
				}
				 else
				 {
					 	    if(j<(y_ledmax-led_n))
					{
	    ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					else
					{
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=pwm;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					 
				 }
		 }
	 }else if(color==red)
		{
			    for(uint16_t j=0;j<8;j++)
		 {
			  if((x_location%2)!=0)
				 {
			   if(j<led_n)
					{
	    ws28128_color_buf[(x_location-1)*8+j][0]=pwm; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					else{
						
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
						
					}
				}
				 else
				 {
					 	    if(j<(y_ledmax-led_n))
					{
	    ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					else
					{
			ws28128_color_buf[(x_location-1)*8+j][0]=pwm; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					 
					 
				 }
		 }
			
		}
		else if(color==blue)
		{
			
			    for(uint16_t j=0;j<8;j++)
		 {
			 if((x_location%2!=0))
				 {
			  if(j<led_n)
				{
	    ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=pwm;//B
				}
				else
				{
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					
				}
			}else
				 {
					 	 	    if(j<(y_ledmax-led_n))
					{
	    ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					}
					else
					{
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=pwm;//B
					}
					 
					 
				 }
		 }
			
		}	else if(color==mie)
		{
			
			    for(uint16_t j=0;j<8;j++)
		 {
			
			ws28128_color_buf[(x_location-1)*8+j][0]=0; //R
	    ws28128_color_buf[(x_location-1)*8+j][1]=0;//G
	    ws28128_color_buf[(x_location-1)*8+j][2]=0;//B
					
					 
					 
				 
		 }
			
		}
}

void DMA_WS2812_FFT_DSPALY(uint16_t fft_x,uint16_t fft_y, uint8_t pwm,float *arr)
{
	         
	            //10K  采样频率,人听到是5Khz,采样点为64,   10000/64=156.25HZ,取10个坐标显示  最大显示频率 10*156.25=1562.5
	
	  uint16_t   led_x[32];  //32列
	    uint8_t t=0;
	  for(uint16_t i=0;i<32;i=i+3)
     {
        t++;
        led_x[i]=(arr[i+10]*2/512)*0.5 ;   //一个频率点的幅值
	   if(t%2==0)
		 {
	   one_y_dspaly(i+1, 1,180,led_x[i],8);  
     one_y_dspaly(i+2, 1,180,led_x[i],8);
		 one_y_dspaly(i+3 ,4,180,0,8);   //第三列为灭
		 }
		 else
		 {
		 one_y_dspaly(i+1, 2,180,led_x[i],8);
     one_y_dspaly(i+2, 2,180,led_x[i],8);
		 one_y_dspaly(i+3 ,4,180,0,8); //第三列为灭
		 }
		
     } 
       DMA_WS2812_light(fft_y*fft_x);		 
}
extern uint8_t reset_flag;
extern uint8_t light_flag;
void aaa()
{
	one_y_dspaly(1, 1,2,2,8);
	one_y_dspaly(2 ,1,2,2,8);
	one_y_dspaly(3 ,3,2,2,8);
  one_y_dspaly(4 ,3,2,2,8);
	one_y_dspaly(5 ,3,2,2,8);
   
	  if(reset_flag == 0)
		{
          		
    DMA_WS2812_light(40);
			HAL_Delay(10);
		}
		if ( light_flag ==0)
		{
			
       DMA_WS2812_Reset();
			HAL_Delay(10);
		}
	
       
	
}