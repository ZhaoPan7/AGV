/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart1.h"
#include "adc.h" 
#include "led.h"
#include "AGV_pwm.h" 
#include "Time_test.h"


#define START_TIME4  T4time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);TIM_Cmd(TIM4, ENABLE)
#define STOP_TIME4  TIM_Cmd(TIM4, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE)

extern 	u8 Track,FLAG; 	 //�ж�·��
extern	u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
extern	u8 counter_BUF;
extern	u8 counter_BUF_LINK;
extern  u8 USFlag;
extern  u8 Wifi_Flag;
extern  u8 Wifi_Touchuan; //͸����־
extern	u8 USART_RX_BUF_LINK[64];     //LinkWifiʱ����ջ���,���64���ֽ�.
extern  u8 WifiStartR;
extern  volatile u32 T3time; // ms ��ʱ����  'volatile' �Ƕ����ױ��������Ҫ����ÿ��ɨ�趼Ҫ��ȡ�ڲ�����ʵ����
extern  volatile u32 T4time; // us ��ʱ����  'volatile' �Ƕ����ױ��������Ҫ����ÿ��ɨ�趼Ҫ��ȡ�ڲ�����ʵ����
extern  u8 TouYanZheng;
extern  u8 timeout;//���ڽ��ճ�ʱ�ж�
extern  u32 SysTickCountFlag ;  //SysTick�жϼ���
extern  u32 SysTickCountFlag_1_0ms;  //SysTick�ж�1ms����
extern  u32 SysTickCountFlag_1_5ms;  //SysTick�ж�1.5ms����
extern  u32 SysTickCountFlag_2_0ms;  //SysTick�ж�2ms����
extern  u32 SysTickCountFlag_Max;  //SysTick�жϼ�����
extern  u32 HighSysTick ;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�ߵ�ƽʱ�ӿ���)
extern  u32 LowSysTick  ;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�͵�ƽʱ�ӿ���)LowSysTick = 2000-HighSysTick��
extern  u8  High_Low ;   // �ڸߵ�ƽ���ʱ�̻��ǵ͵�ƽ���ʱ��(0-�ߵ�ƽʱ�̣�1-�͵�ƽʱ��)
u32 flag1=0;
extern u8 Standby;
extern   u8 UploadCardNumber,DownloadCardNumber;
extern  void uartsend(u8 data);
extern u8 ReadedCard;//��ʼ����ȡ���Ŀ���
extern volatile u8  chaoshengboF; //��������־
extern volatile u32 chaoshengjishu;  //��������ʱ����������
extern volatile u32 chaoshengjuli;   ////������������(��λum)
extern volatile u32 chaoshengjuliceshi;   ////������������ (��λum)

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{

	flag1++;
	SysTickCountFlag++;
	
	//***********SysTick Testing*********************


//	if(SysTickCountFlag_Max<=SysTickCountFlag && SysTickCountFlag<=SysTickCountFlag_Max+10)
	if( (High_Low==0) && (SysTickCountFlag==HighSysTick))
	{
// // 		SysTick->CTRL &= 0xFFFFFFFC;          //��ʱ�ر�SysTickʱ���ж�������,��ʱ�ر�SysTick��ʱ��ʹ��
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //�ߵ�ƽ����
		SysTickCountFlag=0;
		High_Low=1;
		
	}
	
	if(  (High_Low==1) && (SysTickCountFlag==(2000-HighSysTick)))
	{
 		//SysTick->CTRL &= 0xFFFFFFFC;          //��ʱ�ر�SysTickʱ���ж�������,��ʱ�ر�SysTick��ʱ��ʹ��
		GPIO_SetBits(GPIOB,GPIO_Pin_8);  //�͵�ƽ����
		SysTickCountFlag=0;
		High_Low=0 ;  //������һ���׶�
		
	}
		
//***********SysTick Testing*********************	
	
	if(flag1==300000)
	{
		flag1=0; 
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //�͵�ƽ����         
		SysTick->CTRL &= 0xFFFFFFFC;  
		
  }
	
	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
//u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
////����״̬
////bit7��������ɱ�־
////bit6�����յ�0x0d
////bit5~0�����յ�����Ч�ֽ���Ŀ
//u8 USART_RX_STA=0;       //����״̬���

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
void USART1_IRQHandler(void)	//����1�жϷ������
{
	u8  saveBUF_LINK,saveBUF;
	u16 icfor;	
		
	if(Wifi_Touchuan==0)   //��������ʱ������ݴ���
	{
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
			{
				saveBUF_LINK=USART_ReceiveData(USART1);
				USART_RX_BUF_LINK[counter_BUF_LINK]=USART_ReceiveData(USART1);
		
		if(saveBUF_LINK=='O')  //'O' = 0x4F  ,'K'=0x4B  ,'R'=0x4E
				{
					WifiStartR=1;
					
				}
		if(WifiStartR==1)
			{
				counter_BUF_LINK++;
			}
				
		if(counter_BUF_LINK>=2)
			{
				if(USART_RX_BUF_LINK[0]==0x4F && USART_RX_BUF_LINK[1]==0x4B )
				{
					if((Wifi_Flag==1)||(Wifi_Flag==3)||(Wifi_Flag==5))
					{
						Wifi_Flag=(Wifi_Flag+1);  //ָ����ɣ�������һ��
					}
					
/*					
					if(Wifi_Flag==1)
					{
						Wifi_Flag=2;     //ָ����ɣ�������һ��
					}
					if(Wifi_Flag==3)
					{
						Wifi_Flag=4;     //ָ����ɣ�������һ��
					}
					if(Wifi_Flag==5)
					{
						Wifi_Flag=6;     //ָ����ɣ�������һ��
					}
*/				
				}
				
				if(USART_RX_BUF_LINK[0]==0x4F && USART_RX_BUF_LINK[1]==0x4E )
				{
					if((Wifi_Flag==1)||(Wifi_Flag==3)||(Wifi_Flag==5))
					{
						Wifi_Flag=(Wifi_Flag+1);  //ָ����ɣ�������һ��
					}
					
/*											
					if(Wifi_Flag==1)
					{
						Wifi_Flag=2;     //ָ����ɣ�������һ��
					}
					if(Wifi_Flag==3)
					{
						Wifi_Flag=4;     //ָ����ɣ�������һ��
					}
					if(Wifi_Flag==5)
					{
						Wifi_Flag=6;     //ָ����ɣ�������һ��
					}
*/
				}
				
				//*********
				if(USART_RX_BUF_LINK[0]==0x4F && USART_RX_BUF_LINK[1]==0x52 )
				{
					if((Wifi_Flag==1)||(Wifi_Flag==3)||(Wifi_Flag==5))
					{
						Wifi_Flag=Wifi_Flag-1;  //ָ��ʧ�ܣ��˻ص���һ��
					}
/*					
					if(Wifi_Flag==1)
					{
						Wifi_Flag=0;    //ָ��ʧ�ܣ��˻ص���һ��
					}
					if(Wifi_Flag==3)
					{
						Wifi_Flag=2;    //ָ��ʧ�ܣ��˻ص���һ��
					}
					if(Wifi_Flag==5)
					{
						Wifi_Flag=4;    //ָ��ʧ�ܣ��˻ص���һ��
					}					
*/					
				
					}
				
			
				//***********
					//counter_BUF_LINK=0;
					for(icfor=0;icfor<64;icfor++)
					{
						USART_RX_BUF_LINK[icfor]=0;
					}
					counter_BUF_LINK=0;
					WifiStartR=0;
					
			}
			saveBUF_LINK=0; //��һ����һ��
		}	
	}
	
	
	
	if(Wifi_Touchuan==1)   //͸��ʱ������ݴ���
	{	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
			{
				saveBUF=USART_ReceiveData(USART1);								
				//USART_RX_BUF[counter_BUF]= saveBUF;
				
				
				//***************
					
		if(saveBUF==0xAA)
		{
			START_TIME;	 /* TIM3 ��ʼ��ʱ */
		  TouYanZheng=1;	 
		  timeout=0;
		}
		if(TouYanZheng==1)
		{
			USART_RX_BUF[counter_BUF]= saveBUF;
			counter_BUF++;
			if(counter_BUF==6)
			{
				STOP_TIME;   /* TIM3 ֹͣ��ʱ */
				counter_BUF=0;  //�����
				timeout=1;
				

					
					if(USART_RX_BUF[0]==0xAA &&USART_RX_BUF[1]==0x5A &&USART_RX_BUF[4]==0xA5 &&USART_RX_BUF[5]==0xA5) //װ�غ�ж��λ��ͨ��
 					{

						if((USART_RX_BUF[2]==0) && (USART_RX_BUF[3]==0))  //�ش���λ��
						{
								 FLAG=1;
							   UploadCardNumber=0;
							   DownloadCardNumber =0;
							   Standby = 1;

						}		
					  else	if((USART_RX_BUF[2]==0xff) && (USART_RX_BUF[3]==0xff))//����ֹͣ
						{
               FLAG=0;
               UploadCardNumber=0;
							 DownloadCardNumber =0;
							
						}		
						
					 else      //����װж��
						{							
								UploadCardNumber = USART_RX_BUF[2];
								DownloadCardNumber = USART_RX_BUF[3];
								FLAG=1;                          //��ʼagv�Զ�ѭ������							
								Standby = 0;
						}
						uartsend(0xbb);
						uartsend(0x5b);
						uartsend(USART_RX_BUF[2]);
						uartsend(USART_RX_BUF[3]);
						uartsend(0xb2);
						uartsend(0xb5);
         //   printf("%2x %2x %2x %2x %2x %2x \n", 0xBB, 0x5B,USART_RX_BUF[2],USART_RX_BUF[3], 0xB2, 0xB5);

}

					
// 					//SysTick����
// 					if(USART_RX_BUF[0]==0xAA &&USART_RX_BUF[1]==0xBB  &&USART_RX_BUF[4]==0xBB &&USART_RX_BUF[5]==0xAA)
//  					{
// 					
// 						if(USART_RX_BUF[2]==0x10)
// 						{SysTickCountFlag_Max=SysTickCountFlag_1_0ms;
//  						SysTick->CTRL |= 0x00000003;          //��SysTickʱ���ж�������,��SysTick��ʱ��ʹ��	
// 						GPIO_SetBits(GPIOB,GPIO_Pin_8);   //�����ؿ�ʼ
// 						printf("10");
// 						}
// 						if(USART_RX_BUF[2]==0x15)
// 						{SysTickCountFlag_Max=SysTickCountFlag_1_5ms;
//  						SysTick->CTRL |= 0x00000003;          //��SysTickʱ���ж�������,��SysTick��ʱ��ʹ��		
// 						GPIO_SetBits(GPIOB,GPIO_Pin_8);   //�����ؿ�ʼ
// 						printf("15");
// 						}
// 						if(USART_RX_BUF[2]==0x20)
// 						{SysTickCountFlag_Max=SysTickCountFlag_2_0ms;
//  						SysTick->CTRL |= 0x00000003;          //��SysTickʱ���ж�������,��SysTick��ʱ��ʹ��		
// 						GPIO_SetBits(GPIOB,GPIO_Pin_8);   //�����ؿ�ʼ
// 						printf("20");
// 						}
// 						if(USART_RX_BUF[2]==0x30)
// 						{SysTickCountFlag_Max=0;						
//  						SysTick->CTRL &= 0xFFFFFFFC;          //��ʱ�ر�SysTickʱ���ж�������,��ʱ�ر�SysTick��ʱ��ʹ��
// 						GPIO_ResetBits(GPIOB,GPIO_Pin_8);   //�½��ؽ���
// 						printf("30");
// 						}
// 					}
					
				
					for(icfor=0;icfor<=5;icfor++)
					{
						USART_RX_BUF[icfor]=0;
					}
			  }
	    }
				
				//***************
				
				//printf("received %d \n",USART_RX_BUF[counter_BUF]);
				
				saveBUF_LINK=0; //��һ����һ��
			}			
		}
	}

	
	//TIM3�Ķ�ʱ���жϴ�����
void TIM3_IRQHandler(void)
{
	u8 i;
	
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    
  	T3time++;
	}	

	 //*************��ʱ������*********************
		
	 if ( T3time >=20 ) /***** 20ms ʱ�䵽 *****/
    {
			
			STOP_TIME;   /* TIM3 ֹͣ��ʱ */
      T3time = 0;				     
			if(timeout==0)
			{
				
			for(i=0;i<=5;i++)
			{
				USART_RX_BUF[i]=0;
			}
			TouYanZheng=0;
			counter_BUF=0;  //�����	
			} 
			
    } 
	 //************��ʱ������**********************
		
	
}



	//TIM4�Ķ�ʱ���жϴ�����
void TIM4_IRQHandler(void)
{

	
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);    
  	T4time++;
		//chaoshengjuliceshi=1;
	}	

	 //*************��ʱ������********************* 
	if ( (chaoshengboF==0) && (T4time == 1) ) /*****��ʼ���Ϳ��ƿ�ʼ���*****/
    {
			
				GPIO_SetBits(GPIOB, GPIO_Pin_7);		
			chaoshengboF=1;  //����ȴ�20us״̬
		//chaoshengjuliceshi=2;
			
    } 	
	
	 if ( (chaoshengboF==1) && (T4time >=3) ) /***** 20us ʱ�䵽 *****/
    {
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);		
		//	STOP_TIME4;   /* TIM4 ֹͣ��ʱ */
      T4time = 0;				     
			chaoshengboF=2;  //����ȴ��ز�״̬
	//chaoshengjuliceshi=3;
    } 
		
		if ( (chaoshengboF==2) && (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)==1) ) /***** �ȴ��ز�*****/
    {
			
// 			STOP_TIME4;   /* TIM4 ֹͣ��ʱ */
      	 T4time = 0;				     
			chaoshengboF=3;  //������ز�ʱ���׶�
		//chaoshengjuliceshi=4;
			
    } 
		
		if ( (chaoshengboF==3) && (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) != 1) ) /***** �ȴ��ز�*****/
    {
			
 			//STOP_TIME4;   /* TIM4 ֹͣ��ʱ */
			chaoshengjishu = T4time ;
      			     
			chaoshengboF=4;  //������ڽ���
			T4time = 0;
		//	chaoshengjuliceshi=5;
    } 
		
		
		if ( (chaoshengboF==4) && (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) != 1) ) /***** �ȴ��´ο�ʼ���*****/
    {
			
// 			STOP_TIME4;   /* TIM4 ֹͣ��ʱ */
			
      if(T4time >= 8000)	////�ȴ�80msʱ�䵽
			{				
			chaoshengboF=0;  //�ȵȴ�ʱ�䵽
			T4time = 0;
				chaoshengjuliceshi=6;
			}
    } 
		
		
	 //************��ʱ������**********************
		
	
}



/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
