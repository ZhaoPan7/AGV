/******************** ��ʢ���ӹ����� ********************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����
 *          
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�-----------------
 *          |   PB15 - LED1   |
 *          |   PB14 - LED2   |
 *          |                 |
 *           ----------------- 
 * ��汾  ��ST3.0.0  																										  
 * �Ա��꣺http://shop66177872.taobao.com
*********************************************************/
#include "stm32f10x.h"   
#include "led.h"


 /***************  ����LED�õ���I/O�� *******************/
void LED_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB�˿�
  GPIO_SetBits(GPIOB,  GPIO_Pin_8 );	 // �ر�����LED
}

/***************  ����LED�õ���I/O�� *******************/
void ControlDI_AGV_GPIO_Config(void)	
{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		/* Enable the GPIO_LED Clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 ;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //��������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//�����
		
		/* Enable the GPIO_LED Clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_4 ;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //��������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	//�����
  
}

/***************  ����LED�õ���I/O�� *******************/
void ControlDO_AGV_GPIO_Config(void)	
{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_2  ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	   //����Ƥ�����ת��
	
			/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);	  //����Ƥ�����ת��
		
		
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	   //���Ƶ��ת��
	
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);	   //���Ƶ��ת��
	
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);	   //���Ƶ��ת��
		/* Enable the GPIO_ Clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
		GPIO_SetBits(GPIOB, GPIO_Pin_11| GPIO_Pin_10 |GPIO_Pin_2);    //	Ƥ�������ʼ��
		GPIO_SetBits(GPIOA, GPIO_Pin_15);    //	Ƥ�������ʼ��
}

void BOOT1_ReleaseToGPIO(void)
{ 
	//ע�ͣ�BOOT0�ⲿ��������ΪBOOT0=1������£�BOOT1�����ͷų�������ͨGPIO��ʹ�á�
	GPIO_InitTypeDef  GPIO_InitStructure;	
		
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;  //BOOT1���ⲿ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	   //����Ƥ�����ת��
}
	

/***************  �����õ���PC�� *******************/
void ControlDOC_AGV_GPIO_Config(void)	
{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		/* Configure the GPIO pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);	   //���Ƶ��ת��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

}


