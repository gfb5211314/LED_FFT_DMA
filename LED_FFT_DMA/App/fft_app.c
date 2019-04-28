#include "fft_app.h"
/* -------------------------------------------------------------------- */
#include "stm32f1xx_hal.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "arm_const_structs.h"
#include "bsp_usart.h"
#define TEST_LENGTH_SAMPLES 1024

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern UART_HandleTypeDef huart3;
extern uint8_t light_flag;
extern uint8_t TX_flag;
extern uint8_t   ws28128_color_buf[1000][3];
extern  USART_RECEIVETYPE  UsartType3;
/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
static float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES / 2];
/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
static uint32_t fftSize = TEST_LENGTH_SAMPLES / 2;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
/* Reference index at which max energy of bin ocuurs */

uint8_t  CON_FALG = 0;
uint8_t CON_FALG_count;
uint16_t  uhADCxConvertedValue1[TEST_LENGTH_SAMPLES / 2];
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /* Prevent unused argument(s) compilation warning */

//     CON_FALG
// if(__HAL_DMA_GET_FLAG(hdma_adc1, DMA_FLAG_TC1))
//	{
//	     HAL_ADC_Stop_DMA(&hadc1);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

//	}
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ConvCpltCallback must be implemented in the user file.
     */
}
void DMA1_Channel1_IRQHandler(void)
{
		
    if(__HAL_DMA_GET_FLAG(hdma_adc1, DMA_FLAG_TC1))
    {
        CON_FALG = 1;
          HAL_ADC_Stop_DMA(&hadc1);
			/***********半字******************/
//    for(uint16_t i = 0; i < (TEST_LENGTH_SAMPLES / 2); i++)
//        {
//            /* 虚部全部置零 */
//            testInput_f32_10khz[i * 2 + 1] = 0;

//            /* ADC的值作为实部 */
//            testInput_f32_10khz[i * 2] = uhADCxConvertedValue1[i];
//        }
//				
//        arm_cfft_f32(&arm_cfft_sR_f32_len512, testInput_f32_10khz, ifftFlag, doBitReverse);
//        /* Process the data through the Complex Magnitude Module for
//        calculating the magnitude at each bin */
//        arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);
			/***************byte****************/
			
  for(uint16_t i = 0; i<256; i=i+2)
        {
             UsartType3.TX_pData[i]= uhADCxConvertedValue1[i]/256;  //装高八位
				     UsartType3.TX_pData[i+1]= uhADCxConvertedValue1[i]%256;//装低八位
						
        }
			
//			  
    }




    /* USER CODE END DMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc1);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}
void fft_adc_tim_start()
{
	  HAL_TIM_Base_Start(&htim3);
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1, (TEST_LENGTH_SAMPLES/2));
}
void send_fft_data()
{
			
	  HAL_UART_Transmit_DMA(&huart3, UsartType3.TX_pData,128);	
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1, (TEST_LENGTH_SAMPLES/2));

}