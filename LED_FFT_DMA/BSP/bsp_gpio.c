#include     "bsp_gpio.h"
#include "stm32f1xx_hal.h"






/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, NRF24l01_CE_Pin|SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : NRF24l01_CE_Pin SPI1_CS_Pin */
  GPIO_InitStruct.Pin = NRF24l01_CE_Pin|SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : NRF24L01_IRN_Pin */
  GPIO_InitStruct.Pin = NRF24L01_IRN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NRF24L01_IRN_GPIO_Port, &GPIO_InitStruct);

}
