#include    "bsp_nrf24l01.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/                              
/* ˽�б��� ------------------------------------------------------------------*/
SPI_HandleTypeDef hspi_NRF24L01;
extern SPI_HandleTypeDef hspi1;
 uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0xb0,0x43,0x10,0x10,0x01}; //���͵�ַ
 uint8_t RX_ADDRESS0[RX_ADR_WIDTH]={0xb0,0x43,0x10,0x10,0x01};	//���յ�ַ

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/


/* SPI1 init function */
 void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}




/**
  * ��������: ������Flash��ȡд��һ���ֽ����ݲ�����һ���ֽ�����
  * �������: byte������������
  * �� �� ֵ: uint8_t�����յ�������
  * ˵    ������
  */
uint8_t SPIx_ReadWriteByte(SPI_HandleTypeDef* hspi,uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(hspi,&d_send,&d_read,1,0xFF)!=HAL_OK)
  {
    d_read=0xFF;
  }
  return d_read; 
}

/**
  * ��������: ���24L01�Ƿ����
  * �������: ��
  * �� �� ֵ: 0���ɹ�;1��ʧ��
  * ˵    ������          
  */ 
uint8_t NRF24L01_Check(SPI_HandleTypeDef* hspi)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(hspi,TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 

/**
  * ��������: SPIд�Ĵ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:ָ���Ĵ�����ַ
  *           
  */ 
uint8_t NRF24L01_Write_Reg(SPI_HandleTypeDef* hspi,uint8_t reg,uint8_t value)
{
	uint8_t status;	
  NRF24L01_SPI_CS_ENABLE();                 //ʹ��SPI����
  status =SPIx_ReadWriteByte(hspi,reg);//���ͼĴ����� 
  SPIx_ReadWriteByte(hspi,value);      //д��Ĵ�����ֵ
  NRF24L01_SPI_CS_DISABLE();                 //��ֹSPI����	   
  return(status);       			//����״ֵ̬
}

/**
  * ��������: ��ȡSPI�Ĵ���ֵ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:Ҫ���ļĴ���
  *           
  */ 
uint8_t NRF24L01_Read_Reg(SPI_HandleTypeDef* hspi,uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF24L01_SPI_CS_ENABLE();          //ʹ��SPI����		
  SPIx_ReadWriteByte(hspi,reg);   //���ͼĴ�����
  reg_val=SPIx_ReadWriteByte(hspi,0XFF);//��ȡ�Ĵ�������
  NRF24L01_SPI_CS_DISABLE();          //��ֹSPI����		    
  return(reg_val);           //����״ֵ̬
}	

/**
  * ��������: ��ָ��λ�ö���ָ�����ȵ�����
  * �������: ��
  * �� �� ֵ: �˴ζ�����״̬�Ĵ���ֵ 
  * ˵    ������
  *           
  */ 
uint8_t NRF24L01_Read_Buf(SPI_HandleTypeDef* hspi,uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	   
  
  NRF24L01_SPI_CS_ENABLE();           //ʹ��SPI����
  status=SPIx_ReadWriteByte(hspi,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)
  {
    pBuf[uint8_t_ctr]=SPIx_ReadWriteByte(hspi,0XFF);//��������
  }
  NRF24L01_SPI_CS_DISABLE();       //�ر�SPI����
  return status;        //���ض�����״ֵ̬
}

/**
  * ��������: ��ָ��λ��дָ�����ȵ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:�Ĵ���(λ��)  *pBuf:����ָ��  len:���ݳ���
  *           
  */ 
uint8_t NRF24L01_Write_Buf(SPI_HandleTypeDef* hspi,uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
 	NRF24L01_SPI_CS_ENABLE();          //ʹ��SPI����
  status = SPIx_ReadWriteByte(hspi,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
  {
    SPIx_ReadWriteByte(hspi,*pBuf++); //д������	 
  }
  NRF24L01_SPI_CS_DISABLE();       //�ر�SPI����
  return status;          //���ض�����״ֵ̬
}				   

/**
  * ��������: ����NRF24L01����һ������
  * �������: ��
  * �� �� ֵ: �������״��
  * ˵    ����txbuf:�����������׵�ַ
  *           
  */ 
uint8_t NRF24L01_TxPacket(SPI_HandleTypeDef* hspi,uint8_t *txbuf)
{
	uint8_t sta;
	NRF24L01_CE_LOW();
  NRF24L01_Write_Buf(hspi, WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_HIGH();//��������	 
  
	while(NRF24L01_IRQ_PIN_READ()!=0);//�ȴ��������
  
	sta=NRF24L01_Read_Reg(hspi,STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(hspi,FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

/**
  * ��������:����NRF24L01����һ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
uint8_t NRF24L01_RxPacket(SPI_HandleTypeDef* hspi,uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	sta=NRF24L01_Read_Reg(hspi,STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(hspi,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(hspi,FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    

/**
  * ��������: �ú�����ʼ��NRF24L01��RXģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
void NRF24L01_RX_Mode(SPI_HandleTypeDef* hspi)
{
	NRF24L01_CE_LOW();	  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+CONFIG, 0x0F);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC 
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
    
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);//дRX�ڵ��ַ
	
  NRF24L01_CE_HIGH(); //CEΪ��,�������ģʽ 
  HAL_Delay(1);
}						 

/**
  * ��������: �ú�����ʼ��NRF24L01��TXģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
void NRF24L01_TX_Mode(SPI_HandleTypeDef* hspi)
{														 
	NRF24L01_CE_LOW();	    
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+SETUP_RETR,0xff);//�����Զ��ط����ʱ��:4000us + 86us;����Զ��ط�����:15��
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE_HIGH();//CEΪ��,10us����������
  HAL_Delay(1);
}
