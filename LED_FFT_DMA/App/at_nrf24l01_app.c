#include "at_nrf24l01_app.h"
#include "stm32f1xx_hal.h"
#include    "bsp_usart.h"
#include    "bsp_nrf24l01.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os.h"
extern UART_HandleTypeDef huart3;
extern USART_RECEIVETYPE  UsartType3;
extern uint8_t TX_ADDRESS[TX_ADR_WIDTH]; //SEND ADDR
extern uint8_t RX_ADDRESS0[RX_ADR_WIDTH];	//RECEIVE ADDR
uint8_t set_nrf24l01_baud(uint32_t baud, uint16_t wait_time)
{
    char* p_baud;
    if(baud == 4800)
    {
        p_baud = "AT+BAUD=1\r\n";
    }
    else if(baud == 9600)
    {
        p_baud = "AT+BAUD=2\r\n";
    }
    else if(baud == 14400)
    {
        p_baud = "AT+BAUD=3\r\n";
    }
    else if(baud == 19200)
    {
        p_baud = "AT+BAUD=4\r\n";
    }
    else if(baud == 38400)
    {
        p_baud = "AT+BAUD=5\r\n";
    }
    else if(baud == 57600)
    {
        p_baud = "AT+BAUD=6\r\n";
    }
    else if(baud == 115200)
    {
        p_baud = "AT+BAUD=7\r\n";
    }
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)p_baud, 11);
    HAL_Delay(wait_time);
//	 if(UsartType3.RX_flag==1)
//	 {
//       for(uint8_t i=0;i<UsartType3.RX_Size;i++)
//				{
//					printf("%c",UsartType3.RX_pData[i]);
//
//				}
//	}
    if(strstr((const char*)UsartType3.RX_pData, "���óɹ�") != NULL)
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 1;

    }
    else
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 0;
    }



}

uint8_t set_nrf24l01_rate(uint16_t rate, uint16_t wait_time)
{

    char* p_rate;
    if(rate == 250)
    {
        p_rate = "AT+RATE=1\r\n";
    }
    else if(rate == 1000)
    {
        p_rate = "AT+RATE=2\r\n";
    }
    else if(rate == 2000)
    {
        p_rate = "AT+RATE=3\r\n";
    }


    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)p_rate, 11);
    HAL_Delay(wait_time);
//	 if(UsartType3.RX_flag==1)
//	 {
////       for(uint8_t i=0;i<UsartType3.RX_Size;i++)
////				{
////					printf("%c",UsartType3.RX_pData[i]);
////
////				}
//	}
    if(strstr((const char*)UsartType3.RX_pData, "���óɹ�") != NULL)
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 1;

    }
    else
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 0;
    }



}

uint8_t set_nrf24l01_recevie_addr(uint8_t* recaddr, uint16_t wait_time)
{


//    char* p_receive;
//    p_receive = "AT+RXA=0xb0,0x43,0x10,0x10,0x01";
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)recaddr, 50);
    HAL_Delay(wait_time);
//	 if(UsartType3.RX_flag==1)
//	 {
//       for(uint8_t i=0;i<UsartType3.RX_Size;i++)
//				{
//					printf("%c",UsartType3.RX_pData[i]);
//
//				}
//	}
    if(strstr((const char*)UsartType3.RX_pData, "���óɹ�") != NULL)
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 1;

    }
    else
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 0;
    }


}
uint8_t test()
{
    return	set_nrf24l01_recevie_addr(RX_ADDRESS0, 50);

}
uint8_t set_nrf24l01_send_addr(uint8_t* sentaddr, uint16_t wait_time)
{
//    char* p_send;
//    p_send = "AT+TXA=0xb0,0x43,0x10,0x10,0x01";
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)sentaddr, 50);
          HAL_Delay(wait_time);
//	 if(UsartType3.RX_flag==1)
//	 {
//       for(uint8_t i=0;i<UsartType3.RX_Size;i++)
//				{
//					printf("%c",UsartType3.RX_pData[i]);
//
//				}
//	}
    if(strstr((const char*)UsartType3.RX_pData, "���óɹ�") != NULL)
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 1;

    }
    else
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 0;
    }

}
uint8_t set_nrf24l01_freq(uint8_t* freq, uint16_t wait_time)
{
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)freq, 30);
    HAL_Delay(wait_time);
//    if(UsartType3.RX_flag == 1)
//    {
//        for(uint8_t i = 0; i < UsartType3.RX_Size; i++)
//        {
//            printf("%c", UsartType3.RX_pData[i]);

//        }
//    }
    if(strstr((const char*)UsartType3.RX_pData, "���óɹ�") != NULL)
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 1;

    }
    else
    {
        UsartType3.RX_flag = 0;
        memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));
        return 0;
    }
}
uint8_t set_crc(uint16_t crc_bit)
{

}
/*********************************************************
       *app  function
**********************************************************/

void nrf24l01_init(uint32_t baud,uint16_t rate,uint8_t* recaddr,uint8_t* sentaddr,uint8_t* freq,uint16_t wait_time)
{
	if(set_nrf24l01_baud(baud, wait_time))
	{
		  printf("���������óɹ�\r\n");
		
	}else
	{
		 printf("����������ʧ��\r\n");
	}
	if(set_nrf24l01_rate(rate, wait_time))
	{
		
		printf("���ñ����ʳɹ�\r\n");
		
	}else
	{
  	printf("���ñ�����ʧ��\r\n");
	}
		if(set_nrf24l01_recevie_addr(recaddr,wait_time))
	{
		
		printf("���ý��յ�ַ�ɹ�\r\n");
		
	}else
	{
  	printf("���ý��յ�ַʧ��\r\n");
	}
			if(set_nrf24l01_send_addr(sentaddr,wait_time))
	{
		
		printf("���÷��͵�ַ�ɹ�\r\n");
		
	}else
	{
  	printf("���÷��͵�ַʧ��\r\n");
	}
				if(set_nrf24l01_freq(freq,wait_time))
	{
		
		printf("����Ƶ��Ƶ�ʳɹ�\r\n");
		
	}else
	{
  	printf("����Ƶ��Ƶ��ʧ��\r\n");
	}
	
}
	
	
	
	
	
	
	
	
	
	
