#include "bsp_commun.h"
#include "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include "math.h"
#include "bsp_dma.h"
#include "string.h"
#include <stdlib.h>
#include "crc.h"
#include "bsp_usart.h"
#include "cmsis_os.h"
#include  "ws2812_app.h"
#define   light_control_num 128
#define   baotou1      0xff
#define   baotou2      0xfe
#define   baowei1      0xfe
#define   baowei2      0xff
#define   dev_addr     0x01  //logo dev addr
#define   functioncode_light_control  0x01
#define   shade_light_effects  0x02
#define   random_light_effects 0x03
#define   brightness_light_effects 0x04

uint8_t  light_control_auto=0;
extern uint8_t ws28128_color_buf[1000][3];
extern USART_RECEIVETYPE  UsartType3;
extern osThreadId defaultTaskHandle;
extern osThreadId WS12TASKHandle;
extern osThreadId myTask03Handle;
extern UART_HandleTypeDef huart3;
void  light_control_ack(uint16_t data_lenght)
{
    uint8_t  index;
    uint8_t light_control_buf[light_control_num];
    light_control_buf[index++] = baotou1;
    light_control_buf[index++] = baotou2;
    light_control_buf[index++] = dev_addr;
    light_control_buf[index++] = functioncode_light_control;  //功能码
    light_control_buf[index++] = light_control_num; //数据长度
    light_control_buf[index++] = 1;  //当前段数
    light_control_buf[index++] = 1;  //总段数
    light_control_buf[index++] = 1;  //数据
    light_control_buf[index++] = baowei1; //crc
    light_control_buf[index++] = baowei2;
    light_control_buf[index++] = Calc_CRC16(light_control_buf, light_control_num - 2) / 256;
    light_control_buf[index++] = Calc_CRC16(light_control_buf, light_control_num - 2) % 256;
}
void usart3_data_process()
{
	 uint16_t crc_data=0;
		
	 if(UsartType3.RX_flag==1)
	 {    
//		 	  printf("haha");
//		   for(uint8_t i=0;i<UsartType3.RX_Size;i++)
//		 {
//			 printf("i=%d-0x%x\r\n",i,UsartType3.RX_pData[i]);
//			 
//		 }
		    if((UsartType3.RX_pData[0]==baotou1)&&(UsartType3.RX_pData[1]==baotou2)&&
					(UsartType3.RX_pData[UsartType3.RX_Size-3]==baowei2)&&(UsartType3.RX_pData[UsartType3.RX_Size-4]==baowei1)&&
					(UsartType3.RX_pData[2]==dev_addr))
				{
            crc_data=Calc_CRC16(UsartType3.RX_pData,UsartType3.RX_Size-2); 	
					  printf("crc_data=%d\r\n",crc_data);
					  printf("crc_data1=%d\r\n",UsartType3.RX_pData[UsartType3.RX_Size-1]+UsartType3.RX_pData[UsartType3.RX_Size-2]*256);
              if(crc_data==(UsartType3.RX_pData[UsartType3.RX_Size-1]+(UsartType3.RX_pData[UsartType3.RX_Size-2]*256)))
		               {            
							        	switch( UsartType3.RX_pData[3])
							      	{           
													case  functioncode_light_control :  
					                       if(UsartType3.RX_pData[7]==1)
																 {
																	   vTaskSuspend(WS12TASKHandle);//挂起任务
																	   DMA_WS2812_Mie(190) ; 
																	 	 memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
												     light_control_auto=1;
																 }
																 else if(UsartType3.RX_pData[7]==2)
																 {
																	 light_control_auto=0;
																	   vTaskResume(WS12TASKHandle);	//恢复任务1
																 }
                                     break;
													case  shade_light_effects   :          break;
													case random_light_effects   :          break;
												  case	brightness_light_effects :   break;
													default :             break;
													
												
								}
							}
									 					
	 
		  }
	   UsartType3.RX_flag=0;
		 UsartType3.RX_Size=0;
 memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
	 }
	
}



