#include "bsp_usart.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include <stdlib.h>
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

 USART_RECEIVETYPE  UsartType3;
extern uint8_t   ws28128_color_buf[256][3];
/*Define print function*/

int fputc(int ch, FILE* f)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xffff);
    return ch;
}
/* USART1 init function */
 void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}



/* USART3 init function */
 void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/**************DMA_START_receive***********************/
 void  DMA_START(USART_RECEIVETYPE  *Usart,UART_HandleTypeDef *huart)
	 {
      
 		 HAL_UART_Receive_DMA(huart,(uint8_t *)Usart->RX_pData, RX_LEN);  
	   __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	 }
/*********receive data*******************/
	void UsartReceive_IDLE(UART_HandleTypeDef *huart,USART_RECEIVETYPE   *Usart)  
{  
    uint32_t temp;  
  
    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {   
	


        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  
         Usart->RX_Size =  RX_LEN - temp;   
         Usart->RX_flag=1;  
        HAL_UART_Receive_DMA(huart,(uint8_t *)Usart->RX_pData,RX_LEN);
 					 
			 
		 
    }  

} 


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	

	
	
	
}
void DMA_USART_IDLE_INIT()
{

//	DMA_START(&UsartType2,&huart2);
	DMA_START(&UsartType3,&huart3);
	
	
}