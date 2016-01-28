/********************HARDWARE********************
 * �ļ���  ��main.c
 * ����    ��AGV
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0
**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h" 
#include "led.h"
#include "AGV_pwm.h" 
#include "rc522.h"
#include "stm32f10x_it.h"
#include "Time_test.h"
#include "SysTick.h"

/**�������**/

#define  PWMPulseLow       80	
#define  PWMPulseMid1      80
#define  PWMPulseMid2      80
#define  timeeeer1          45
#define  timeeeer2          70
#define  timeeeer3          150
//////////////////////////////////////////////////////////
//M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
//�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
//��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
//ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
//ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��

/*******************************
PA4��5��6��7��SPI1_NSS��SPI1_SCK��SPI1_MISO��SPI1_MOSI
*RDID����˵����
*1--SS��sda�� <----->PF0  pa2   �ӻ�ѡ��  
*2--SCK <----->PB13   ʱ�����
*3--MOSI<----->PB15   ��Ƶ�������
*4--MISO<----->PB14   ��Ƶģ������
*5--����
*6--GND <----->GND
*7--RST <----->PF1   pa3  ���߿���
*8--VCC <----->VCC   
************************************/
/*ȫ�ֱ���*/
unsigned char CT[20];//������
unsigned char SN[4]; //����
unsigned char RFID[16];			//���RFID 
unsigned char RFIDWrite[16]={0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0x0A};			//RFID writedata 
u8 printout[6]={0xBB, 0x5B, 0x00, 0x00,0x00,0xB5 };
// unsigned char card1_bit=0;
// unsigned char card2_bit=0;
// unsigned char card3_bit=0;
// unsigned char card4_bit=0;
//unsigned char total=0;
//unsigned char lxl[4]={6,109,250,186};
// unsigned char card_1[4]={66,193,88,0};
// unsigned char card_2[4]={66,191,104,0};
// unsigned char card_3[4]={62,84,28,11};
// unsigned char card_4[4]={126,252,248,12};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
//unsigned char RFID1[16]={0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0x01};
unsigned char senddata1[]={0x41 ,0x54 ,0x2B ,0x43 ,0x49 ,0x50 ,0x53 ,0x54 ,0x41 ,0x52 ,0x54 ,0x3D ,0x22 ,
	                         0x54 ,0x43 ,0x50 ,0x22 ,0x2C ,0x22 ,0x31 ,0x30 ,0x2E ,0x30 ,0x2E ,0x31 ,0x2E ,
                           0x31,0X37 ,0x22 ,0x2C ,0x38 ,0x30 ,0x0D ,0x0A };  //10.0.1.17:80
unsigned char senddata2[]={0x41 ,0x54 ,0x2B ,0x43 ,0x49 ,0x50 ,0x4D ,0x4F ,0x44 ,0x45 ,0x3D ,0x31 ,0x0D ,0x0A };
unsigned char senddata3[]={0x41 ,0x54 ,0x2B ,0x43 ,0x49 ,0x50  ,0x53 ,0x45 ,0x4E ,0x44, 0x0D ,0x0A };
u8 USFlag=0;
volatile u32 T3time=0; // ms ��ʱ����

/*����������*/
	u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
	u8 USART_RX_BUF_LINK[64];     //LinkWifiʱ����ջ���,���64���ֽ�.
	u8 USART_RX_STA=0;       //����״̬���
	u8 tempNumInc=0;//���ռ���
	uint8_t T1,T2,T3,T4,T5,clp,distance;  //����DI����
	u8 counter_BUF=0;
	u8 counter_BUF_LINK=0;
	u8 testt=0;
	u8 espFlag=0;
  u8 Wifi_Flag=0;
	u8 Wifi_Touchuan=0; //͸����־
  u8 WifiStartR=0;
  u8 UploadCardNumber=0,DownloadCardNumber=0,Standby=0;//ͨ��ָ��ָ��agvװ��λ�á�ж��λ�á�����λ��
  u8 ReadedCard;
//bool T1,T2,T3,T4,T5,clp;  //����DI����
u8 Track; 	 //�ж�·��
u8 FLAG;//ͨ����ѭ����ʽ����agv����

u8 TouYanZheng=0;//ͨ��ָ��ͷ��֤�����ͷ��֤ͨ������ʼ����ָ�� ��ʱ���գ��ڹ涨ʱ���ڽ��ղ�������������������
u8 timeout=0;//���ڽ��ճ�ʱ�ж�
u32 SysTickCountFlag =0;  //SysTick�жϼ���
u32 SysTickCountFlag_1_0ms =(1000*100);  //SysTick�ж�1ms����
u32 SysTickCountFlag_1_5ms =(1500*100);  //SysTick�ж�1.5ms����
u32 SysTickCountFlag_2_0ms =(2000*100);  //SysTick�ж�2ms����
u32 SysTickCountFlag_Max =100000;  //SysTick�жϼ�����
u32 HighSysTick =0;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�ߵ�ƽʱ�ӿ���)HighSysTick ȡֵ50~250
u32 LowSysTick = 0;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�͵�ƽʱ�ӿ���)LowSysTick = 2000-HighSysTick��
u8  High_Low = 0;   // �ڸߵ�ƽ���ʱ�̻��ǵ͵�ƽ���ʱ��(0-�ߵ�ƽʱ�̣�1-�͵�ƽʱ��)
//u32 SysTick_TimeSet=100000;     //SysTick��ʱ�����

u16  PWMPulseHigh=80;
unsigned char GetData;
extern __IO u16 ADC_ConvertedValue;	 
// �����ʱ
void Delay(unsigned long time)
{unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}

// ���PA11��PA12��ʹ�����������ͨGPIOʹ��  ȡ��ӳ��can�ӿ�   ����AFIO->MAPR��14��13λ=01
void USBCAN_GPIO_NoRemap(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ�ܶ�ӦGPIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //ʹ��AFIO��ӳ��ʱ��
		AFIO->MAPR |= 0x2000;				//������ӳ������ 
		AFIO->MAPR &=~0x4000;
}	

//���PA15��PB3��PB4��ʹ�����������ͨGPIOʹ��
void JTAGDisable_GPIO_NoRemap(void) 
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ�ܶ�ӦGPIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ�ܶ�ӦGPIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //ʹ��AFIO��ӳ��ʱ��
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�ر�JTAG-DP,����SW-DP
		
// 		AFIO->MAPR &=~0x01000000;										//ֱ�Ӳ����Ĵ���ʵ��								
// 		AFIO->MAPR |= 0x02000000;
// 		AFIO->MAPR &=~0x04000000;
}
	


//������������
void LED_GPIO_Config(void);
void ControlDI_AGV_GPIO_Config(void);
void ControlDO_AGV_GPIO_Config(void);
u8 RFIDReader(void);
void MotoBelt(u16 direction);
void MotoBeltControl(void);//Ƥ�����ͨ�ſ���
u8  InfraredDetection(void);
void USART1_IRQHandler(void);
unsigned char UART1GetByte(void);
void NVIC_Configuration(void);
void Wifi_Connect(void);
void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);
void BOOT1_ReleaseToGPIO(void);
//void RFID_SN_Control(void);
void AGVRun(void);
void DuoJi(u16 jiaodu);
void uartsend(u8 data);


int main(void)
{ 

 	USBCAN_GPIO_NoRemap();   // ���PA11��PA12
 	JTAGDisable_GPIO_NoRemap(); 	//���PA15��PB3��PB4
	BOOT1_ReleaseToGPIO();//���boot1�˿�
  /* ����ϵͳʱ��Ϊ72M */      
  SystemInit();	
  /* ���ô��� */
  USART1_Config();
  /* ��ʼ��ADC1 */
//  ADC1_Init();
	/*��ʼ��״̬�ƶ˿ڣ����Ѿ���Ϊ����Ŀ�������*/
	LED_GPIO_Config();
	/*��ʼ������*/
	ControlDI_AGV_GPIO_Config();
	/*��ʼ�����*/
	ControlDO_AGV_GPIO_Config();	
	/*��ʼ��PWM����*/
	PWM_RCC_Configuration();
	PWM_GPIO_Configuration();
	TIM2_Configuration();
	//TIM3_Configuration();
	motor_control();//��ʼ�����ֿ���
	//PC�ڳ�ʼ��
	ControlDOC_AGV_GPIO_Config();
	NVIC_Configuration();
	/*�����ʼ������*/
	HighSysTick =150;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�ߵ�ƽʱ�ӿ���)HighSysTick ȡֵ50~250
  LowSysTick =  2000-HighSysTick;  //SysTick ������ƣ�20ms���ڣ�0.5ms~2.5ms�ߵ�ƽռ�ձ�-------0��~180��(�͵�ƽʱ�ӿ���)LowSysTick = 2000-HighSysTick��
  High_Low = 0; 
  /* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
	
	//GPIO_SetBits(GPIOB, GPIO_Pin_8);//led off
  /*ת��Ƥ����ʼ��*/
	MotoBelt(0);//0=ֹͣ��1=��ת��2=��ת
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);		//USART1�����ж�ʹ��
	TIM3_NVIC_Configuration(); /* TIM3(�ж����ȼ�) ��ʱ���� */
  TIM3_Configuration(); 	 /* TIM3(��ʼ��) ��ʱ���� */
	
//  	printf("Uart init OK            \n");	
	InitRc522();				//��ʼ����Ƶ��ģ��
	ReadedCard=0;//��ʼ����ȡ���Ŀ���
	Standby=0;
	Delay(5000);
	RFID[15]=0;
  DuoJi(300);//���ת���ڵ�λ�
	
// // TEST MOTOR CONVEYOR
// while(1)
// {
// // 																		 MotoBelt(1);

// // 																		 Delay(2000);	
// // 																		 MotoBelt(2);
// // 																		 Delay(2000);
// // 																		 MotoBelt(0);
// 	GPIO_ResetBits(GPIOA, GPIO_Pin_15);  //3
// 	Delay(2000);
// 	GPIO_SetBits(GPIOA, GPIO_Pin_15);  //3
// 	Delay(2000);
// }



// //***********SysTick Testing*********************

// while(1)
// 	{
// 	if(SysTickCountFlag==SysTickCountFlag_Max)
// 	{
// 		GPIO_SetBits(GPIOB,GPIO_Pin_8);
// 	}
// 	
// 	if(SysTickCountFlag==SysTickCountFlag_Max *2)
// 	{
// 		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
// 		SysTickCountFlag=0;
// 		
// 	}
// 	}
 //***********SysTick Testing*********************
	
	

		while(1)
	 { 

			if(espFlag==0) //��δ������WIFIʱ�򣬲Ž���WIFI���Ӳ����������Ϻ󽫲���ִ�д˲�����
			{ 
				Wifi_Connect();			
			}						
			while (espFlag)
			{					
				
				//***********�ֲ����Թ���*********************
// 				while(1)
// 				{
//   				//AGVRun();	//С���н����ҿ���
//  				RFIDReader();	//��Ƶ�����
// 				}
								
				//***********�ֲ����Թ���*********************	
			
				// UART1GetByte();//ָ�����
				AGVRun();	//С���н����ҿ���													
				if( (UploadCardNumber!=0) || (DownloadCardNumber!=0) )//���յ�װж��ָ��
				{     			
					if(RFID[15]==0)
					{				
						RFIDReader();	//��Ƶ�����
					}
					else if(RFID[15]!=0)
					{ 
						if((RFID[15]==UploadCardNumber)||(RFID[15]==DownloadCardNumber))
						{ ReadedCard =RFID[15];}
						else if((RFID[15]!=UploadCardNumber)&&(RFID[15]!=DownloadCardNumber))
						{ RFID[15]=0;
            PWMPulseHigh=80;//�ָ�agv�н��ٶ�
             }
					}
						
					if((ReadedCard == UploadCardNumber)&&(UploadCardNumber!=0) && (DownloadCardNumber!=0))  //������Ƭ   get into Upload process   
					{
							PWMPulseHigh=80;//�ָ�agv�н��ٶ�	
							//FLAG=0;// AGV STOP
							 motorQZ_control(TIM2,0,1,1);//stop
							 motorQY_control(TIM2,0,2,1);
							 motorHZ_control(TIM2,0,3,1);
							 motorHY_control(TIM2,0,4,1);		
               Delay(500);	//�ȴ����ת����λ							
							 DuoJi(300);//���ת���ڵ�λ��	
							 Delay(1000);	//�ȴ����ת����λ
							 motorQZ_control(TIM2,PWMPulseHigh,1,2);//run
							 motorQY_control(TIM2,PWMPulseHigh,2,2);
							 motorHZ_control(TIM2,PWMPulseHigh,3,2);
							 motorHY_control(TIM2,PWMPulseHigh,4,2);							
							 Delay(300);	//�ȴ����ת����λ
							 motorQZ_control(TIM2,0,1,1);//stop
							 motorQY_control(TIM2,0,2,1);
							 motorHZ_control(TIM2,0,3,1);
							 motorHY_control(TIM2,0,4,1);			
							 Delay(500);	//�ȴ����ת����λ
							 motorQZ_control(TIM2,PWMPulseHigh,1,3);//back
						   motorQY_control(TIM2,PWMPulseHigh,2,3);
						   motorHZ_control(TIM2,PWMPulseHigh,3,3);
						   motorHY_control(TIM2,PWMPulseHigh,4,3);
							 Delay(200);	//�ȴ����ת����λ
							 motorQZ_control(TIM2,0,1,1);//stop
							 motorQY_control(TIM2,0,2,1);
							 motorHZ_control(TIM2,0,3,1);
							 motorHY_control(TIM2,0,4,1);	
// 							 motorQZ_control(TIM2,PWMPulseHigh,1,2);//run
// 							 motorQY_control(TIM2,PWMPulseHigh,2,2);
// 							 motorHZ_control(TIM2,PWMPulseHigh,3,2);
// 							 motorHY_control(TIM2,PWMPulseHigh,4,2);							
// 							 Delay(30);
// 							 motorQZ_control(TIM2,0,1,1);//stop
// 							 motorQY_control(TIM2,0,2,1);
// 							 motorHZ_control(TIM2,0,3,1);
// 							 motorHY_control(TIM2,0,4,1);	
// 	             Delay(100);	//�ȴ����ת����λ							 
// 							 motorQZ_control(TIM2,PWMPulseHigh,1,2);//run
// 							 motorQY_control(TIM2,PWMPulseHigh,2,2);
// 							 motorHZ_control(TIM2,PWMPulseHigh,3,2);
// 							 motorHY_control(TIM2,PWMPulseHigh,4,2);							
// 							 Delay(30);	//�ȴ����ת����λ
// 							 motorQZ_control(TIM2,0,1,1);//stop
// 							 motorQY_control(TIM2,0,2,1);
// 							 motorHZ_control(TIM2,0,3,1);
// 							 motorHY_control(TIM2,0,4,1);							
// 							 Delay(100);	//�ȴ����ת����λ

							 //PWMPulseHigh=60;//�ָ�agv�н��ٶ�
 							//FLAG=1;//AGV RUN ��ˮƿװ��
               //Delay(1000);//�ȴ�װ�����
              // DuoJi(180);	//���ת��ͨ��λ��
							 Delay(500);//�ȴ�agvж�ص�λ
							 DuoJi(220);	//���ת��ͨ��λ��
							 MotoBelt(1);//ж�� 
							 Delay(500);//�ȴ�agvж�ص�λ
						   MotoBelt(0);//ж��
 							 Delay(700);	//�ȴ����ת����λ
  						 //PWMPulseHigh=80;//�ָ�agv�н��ٶ�			
               RFID[15]=0;							
  						 ReadedCard = 0;   //  get out Upload process	
							 //printf("%02x %02x %02x %02x %02x %02x \n", 187, 91,0,0, 179, 181);					     				
							 UploadCardNumber=0;
							 uartsend(0xbb);
							 uartsend(0x5b);
							 uartsend(0x00);
							 uartsend(0x00);
							 uartsend(0xb3);
							 uartsend(0xb5);
											
// 						 printf("Upload finished");
							 
						}
						
			
												
				  if((ReadedCard==DownloadCardNumber) && (UploadCardNumber==0)&& (DownloadCardNumber!=0))
					{												
							
							 motorQZ_control(TIM2,0,1,1);// AGV STOP
							 motorQY_control(TIM2,0,2,1);
							 motorHZ_control(TIM2,0,3,1);
							 motorHY_control(TIM2,0,4,1);
							 MotoBelt(2);//ж�� 
							 Delay(4000);//�ȴ�agvж�ص�λ
						   MotoBelt(0);//ж��
						   PWMPulseHigh=80;//�ָ�agv�н��ٶ�	
						  
							 motorQZ_control(TIM2,PWMPulseHigh,1,2);//AGV RUN
							 motorQY_control(TIM2,PWMPulseHigh,2,2);
							 motorHZ_control(TIM2,PWMPulseHigh,3,2);
							 motorHY_control(TIM2,PWMPulseHigh,4,2);
							 RFID[15]=0;						
							 ReadedCard = 0;   //  get out Download process
					 //  printf("Download finished");										 		  
							 DownloadCardNumber=0;	
							 Standby = 1;
						   uartsend(0xbb);
							 uartsend(0x5b);
							 uartsend(0x00);
							 uartsend(0x00);
							 uartsend(0xb4);
							 uartsend(0xb5);
							
					}
					if((ReadedCard==DownloadCardNumber) && (UploadCardNumber!=0))//�������ж�ؿ�Ƭ�����ǻ�δװ�أ���ô�ͻָ�agv�ٶ�
					{
		           RFID[15]=0;	
						   ReadedCard = 0;   //  get out Download process									 
							 PWMPulseHigh=80;//�ָ�agv�н��ٶ�	

					}
			
				}
				if(Standby == 1)   //ж����ɺ�
				{  
									
					if(RFID[15] == 0)
					{				
						RFIDReader();	//��Ƶ�����
					}
				  if(RFID[15] == 0xff)
					{ 
						
						 Standby =0;
						 FLAG=0;// AGV STOP
						 motorQZ_control(TIM2,0,1,1);//stops
						 motorQY_control(TIM2,0,2,1);
						 motorHZ_control(TIM2,0,3,1);
						 motorHY_control(TIM2,0,4,1);	
						 PWMPulseHigh=80;//�ָ�agv�н��ٶ�
						 RFID[15]=0;
					//	 Delay(1000);	//�ȴ����ת����λ
					}										
					if((RFID[15]!= 0xff) && (RFID[15]!= 0))
					{		 			   
				    	RFID[15] = 0;		
						  PWMPulseHigh=80;//�ָ�agv�н��ٶ�			
	            						
					}
					
			 }
		
		  }
	}
}

//���ڷ���һ���ֽ� 
void uartsend(u8 data)
{
  USART_SendData(USART1, data);
	while (!(USART1->SR & USART_FLAG_TXE));	
}


//��C�Ƕȿ��� 50-250֮�g�O�ã�����0-180��  
void DuoJi(u16 jiaodu)
{
// 	 High_Low = 0;
	 HighSysTick=jiaodu;   
	 SysTick->CTRL |= 0x00000003; 
}


//С���н�����
void AGVRun(void)
		{
			u8 testu8;
			if(FLAG!=0)
			{
				testu8=InfraredDetection();//�������ѭ�����
			}
			else testu8=FLAG;
			
			/*********������AGV Control*************/
					if(FLAG==0||testu8==0)
					{ //printf("stop all 0(0),Track= %d %d \n",Track,testu8);
						
							 motorQZ_control(TIM2,0,1,1);
							 motorQY_control(TIM2,0,2,1);
							 motorHZ_control(TIM2,0,3,1);
							 motorHY_control(TIM2,0,4,1);
					}
					if(FLAG!=0&&testu8==1)
				 { //printf("Rollback 1(1) ,Track= %d %d \n",Track,testu8);
							 motorQZ_control(TIM2,PWMPulseHigh,1,3);
						   motorQY_control(TIM2,PWMPulseHigh,2,3);
						   motorHZ_control(TIM2,PWMPulseHigh,3,3);
						   motorHY_control(TIM2,PWMPulseHigh,4,3);
				 }
				
					if(FLAG!=0&&testu8==2)
					{// printf("Go along(2) ,Track= %d %d \n",Track,testu8);
						 motorQZ_control(TIM2,PWMPulseHigh,1,2);
						 motorQY_control(TIM2,PWMPulseHigh,2,2);
						 motorHZ_control(TIM2,PWMPulseHigh,3,2);
						 motorHY_control(TIM2,PWMPulseHigh,4,2);
					}
						 
						
				if(FLAG!=0&&testu8==3)
				{ //printf("Turn Left 1(3) ,Track= %d %d \n",Track,testu8);
						motorQZ_control(TIM2,PWMPulseLow,1,2);
						 motorQY_control(TIM2,PWMPulseLow,2,3);
						 motorHZ_control(TIM2,PWMPulseLow,3,2);
						 motorHY_control(TIM2,PWMPulseLow,4,3);	
// 						Delay(timeeeer1);
// 						motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
				}
				if(FLAG!=0&&testu8==4)
				{	//printf("Turn Left 2(4) ,Track= %d %d \n",Track,testu8);	
						  motorQZ_control(TIM2,PWMPulseMid1,1,2);
						 motorQY_control(TIM2,PWMPulseMid1,2,3);
						 motorHZ_control(TIM2,PWMPulseMid1,3,2);
						 motorHY_control(TIM2,PWMPulseMid1,4,3);
// 					Delay(timeeeer2);
// 						motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
				}
				if(FLAG!=0&&testu8==5)		 
				{	//printf("Turn Left 3(5) ,Track= %d %d \n",Track,testu8);
						motorQZ_control(TIM2,PWMPulseMid2,1,2);
						 motorQY_control(TIM2,PWMPulseMid2,2,3);
						 motorHZ_control(TIM2,PWMPulseMid2,3,2);
						 motorHY_control(TIM2,PWMPulseMid2,4,3);
// 					Delay(timeeeer3);
// 					motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
				}
				if(FLAG!=0&&testu8==6)
				{	//printf("Turn Right 1(6) ,Track= %d %d \n",Track,testu8);
							motorQZ_control(TIM2,PWMPulseLow,1,3);
						 motorQY_control(TIM2,PWMPulseLow,2,2);
						 motorHZ_control(TIM2,PWMPulseLow,3,3);
						 motorHY_control(TIM2,PWMPulseLow,4,2);
// 					Delay(timeeeer1);
// 			motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
					
				
				}
				if(FLAG!=0&&testu8==7)
				{	//printf("Turn Right 2(7) ,Track= %d %d \n",Track,testu8);	
					
					   motorQZ_control(TIM2,PWMPulseMid1,1,3);
						 motorQY_control(TIM2,PWMPulseMid1,2,2);
						 motorHZ_control(TIM2,PWMPulseMid1,3,3);
						 motorHY_control(TIM2,PWMPulseMid1,4,2);
// 					Delay(timeeeer2);
// 					motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
						
				}
				if(FLAG!=0&&testu8==8)
				{	//printf("Turn Right 3(8) ,Track= %d %d \n",Track,testu8); 
						
					 motorQZ_control(TIM2,PWMPulseMid2,1,3);
						 motorQY_control(TIM2,PWMPulseMid2,2,2);
						 motorHZ_control(TIM2,PWMPulseMid2,3,3);
						 motorHY_control(TIM2,PWMPulseMid2,4,2);
// 						Delay(timeeeer3);
// 				motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);
					
				}
				if(FLAG!=0&&testu8==9)
				{//	printf("closely(9) ,Track= %d %d \n",Track,testu8);
						 motorQZ_control(TIM2,0,1,4);
						 motorQY_control(TIM2,0,2,4);
						 motorHZ_control(TIM2,0,3,4);
						 motorHY_control(TIM2,0,4,4);
				}

				if(FLAG!=0&&testu8==10)
				{ //printf("Estop(10) ,Track= %d %d \n",Track,testu8);
						 motorQZ_control(TIM2,0,1,4);
						 motorQY_control(TIM2,0,2,4);
						 motorHZ_control(TIM2,0,3,4);
						 motorHY_control(TIM2,0,4,4);
				}

			}




//С��Ƥ������ת����
void MotoBelt(u16 direction)	//0=ֹͣ��1=��ת��2=��ת
	{
		if(direction==1)  //��ת
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);  //1
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);   //4	
      GPIO_SetBits(GPIOB, GPIO_Pin_11);    //2	
			GPIO_SetBits(GPIOA, GPIO_Pin_15);    //3			
		}
		if(direction==2)  //��ת
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_11);  //2
			GPIO_ResetBits(GPIOA, GPIO_Pin_15);  //3
      GPIO_SetBits(GPIOB, GPIO_Pin_10);    //1	
      GPIO_SetBits(GPIOB, GPIO_Pin_2);     //4				
		}
		if(direction==0)  //ֹͣ
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);  //1
			GPIO_SetBits(GPIOB, GPIO_Pin_11);  //2	
			GPIO_SetBits(GPIOA, GPIO_Pin_15);  //3
			GPIO_SetBits(GPIOB, GPIO_Pin_2);   //4		
		}
	
	}
	
		
void MotoBeltControl(void)//Ƥ�����ͨ�ſ���
{
	if(USART_RX_STA==2)//������ȷ
			{			
				
				GPIO_ResetBits(GPIOB, GPIO_Pin_8);//LED��
				MotoBelt(1);
				printf("\n Encompass21  \n");
				USART_RX_STA=0;

			}		
		else if(USART_RX_STA==3)
			{  
				MotoBelt(2);
				printf("\n Encompass22  \n");
				USART_RX_STA=0;
			}
		else if(USART_RX_STA==4)
			{  
				MotoBelt(0);
				printf("\n Encompass24  \n");
				USART_RX_STA=0;
			}
// 			Delay(50);
}	


//ѭ�����
u8 InfraredDetection(void)
{
	u8 HongWaiStatus=2;
	
	T1 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
	//	printf("\n T1 = %d mV  \r\n",T1);
// 		Delay(50);
	
	T2 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
	//printf("\n T2 = %d mV  \r\n",T2);
// 		Delay(50);
	T3 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
	//printf("\n T3 = %d mV  \r\n",T3);
// 		Delay(50);
	T4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
//	printf("\n T4 = %d mV  \r\n",T4);
// 		Delay(50);
	T5 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	//printf("\n T5 = %d mV  \r\n",T5);
// 		Delay(50);
	clp = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	//printf("\n clp = %d mV  \r\n",clp);
// 		Delay(50);
	distance = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
	//printf("\n distance = %d mV  \r\n",distance);
// 		Delay(50);
		
	if( (!T1) && (!T2) && (!T3)&& (!T4)&& (!T5)&&((UploadCardNumber!=0) || (DownloadCardNumber!=0) || (Standby==1) ))//ȫ�����ڵ�ʱ,��Ϊ������ٴ���׼��RFID���
	{

				PWMPulseHigh=60;
	}
	if( (T1) && (T2) && (T3)&& (T4)&& (T5))
	{

		HongWaiStatus=1;	//roll back (1)
	}
	if( (T1) && (T2) && (!T3)&& (T4)&& (T5))
	{

		HongWaiStatus=2;	//go along (2)
		
	}
	
	if( (T1) && (!T2) && (!T3)&& (!T4)&& (T5))
	{

		HongWaiStatus=2;	//go along (2)
	}
	
	
	if( (T1) && (!T2) && (!T3)&& (T4)&& (T5))
	{

		HongWaiStatus=2;  //turn left 1(3)
	}
	
	if( (!T1) && (!T2) && (T3)&& (T4)&& (T5))
	{

		HongWaiStatus=4;  //turn left 2(4)
	}
	if( (T1) && (!T2) && (T3)&& (T4)&& (T5))
	{

		HongWaiStatus=4;  //turn left 2(4)
	}
	if( (!T1) && (T2) && (T3)&& (T4)&& (T5))
	{

		HongWaiStatus=5;  //turn left 3(5)
	}
	if( (!T1) && (!T2) && (!T3)&& (T4)&& (T5))
	{

		HongWaiStatus=5;  //turn left 3(5)
	}
	if( (T1) && (T2) && (!T3)&& (!T4)&& (T5))
	{
		
		HongWaiStatus=2; //turn right 1(6)
	}
	
	if( (T1) && (T2) && (T3)&& (!T4)&& (!T5))
	{

		HongWaiStatus=7; //turn right 2(7)
	}
	if( (T1) && (T2) && (T3)&& (!T4)&& (T5))
	{

		HongWaiStatus=7; //turn right 2(7)
	}
	
	if( (T1) && (T2) && (T3)&& (T4)&& (!T5))
	{

		HongWaiStatus=8; //turn right 3(8)
	}
	if( (T1) && (T2) && (!T3)&& (!T4)&& (!T5))
	{

		HongWaiStatus=8; //turn right 3(8)
	}
	if(distance)
	{
		HongWaiStatus=9; 		 //closely (9)
	}

  if(clp)
	{
		HongWaiStatus=10; 		//Estop (10)
	}
	
	
	return HongWaiStatus;
	
}

// Wifi���Ӳ���
void  Wifi_Connect(void)
{
	u8 ifor;
 		
	if(Wifi_Flag==0)
		{
			for(ifor=0;ifor<34;ifor++)
		{USART_SendData(USART1, senddata1[ifor] );
  while (!(USART1->SR & USART_FLAG_TXE));
		}
		Wifi_Flag=1;
		Delay(1000);
		}
 		
	if(Wifi_Flag==2)
	{
		for(ifor=0;ifor<14;ifor++)
		{USART_SendData(USART1, senddata2[ifor] );
  while (!(USART1->SR & USART_FLAG_TXE));
		}
		Wifi_Flag=3;
		Delay(1000);
	}	
		
 		
	if(Wifi_Flag==4)
		{
			for(ifor=0;ifor<12;ifor++)
		{USART_SendData(USART1, senddata3[ifor] );
		while (!(USART1->SR & USART_FLAG_TXE));
		}
		Wifi_Flag=5;
		Delay(1000);
		}
 		
		if(Wifi_Flag==6)
		{
		Wifi_Flag=7;
		Delay(1000);
		}
		if(Wifi_Flag==7)
		{
			Wifi_Flag=8;  //WIFI����OK��־(Wifi_Flag=8;)
			espFlag=1;
			Wifi_Touchuan=1;
// 	  printf("WIFI connected");
       uartsend(0xbb);
			 uartsend(0x5b);
			 uartsend(0x00);
			 uartsend(0x00);
			 uartsend(0xb1);
			 uartsend(0xb5);
			DuoJi(220);//���ת���ڵ�λ�
			Delay(5000);
		}
	}
//RFID����
u8 RFIDReader(void)
{
	unsigned char status;
	unsigned char RFID_status = RFID_NO ;
	unsigned char s=0x08;
	u8 CardNumber;
// /*>>>>>>>>>>>RFID>>>>>>>>>>>*/				
	switch (RFID_status)	{
		case RFID_NO :    status = PcdRequest(PICC_REQALL,CT);/*����*/
											if (status==MI_OK)
											{
														 
												RFID_status = RFID_XunKa_OK;
// 											printf("PcdRequest_MI_OK,  TagType is %02x%02x     \n",CT[0],CT[1]);	
												status=MI_ERR;
												status = PcdAnticoll(SN);/*����ײ*/	
											}
		case RFID_XunKa_OK: 			
												if (status==MI_OK)//���nײ
												{      
                         													
													RFID_status = RFID_FangChongZhuang_OK ;
// 												printf("PcdAnticoll_MI_OK,  SN is %02x%02x%02x%02x     \n",SN[0],SN[1],SN[2],SN[3]);	
													status=MI_ERR;
													status =PcdSelect(SN);//�x��	
														
												}									
	case RFID_FangChongZhuang_OK: 
																if (status==MI_OK)
																{  
                            															
																	RFID_status = RFID_XuanKa_OK  ;
																	status=MI_ERR;
																	status =PcdAuthState(0x60,0x09,KEY,SN);//��C
																}
												
	case 	RFID_XuanKa_OK:		
													if (status==MI_OK)
													{         
														
														RFID_status = RFID_YanZheng_OK   ;
														status=MI_ERR;
// status=PcdWrite(s,RFIDWrite); //д��
// status=MI_ERR;
														 status=PcdRead(s,RFID); //����
														
													}					
		
	case RFID_YanZheng_OK:				
																if (status==MI_OK)
																{  

																	  CardNumber=RFID[15]; 																						
// 																	printf("READ_Card the %d area data is  %02x  \n",s,CardNumber);																									
	      uartsend(0xbb);
			 uartsend(0x5b);
			 uartsend(CardNumber);
			 uartsend(0x00);
			 uartsend(0xb6);
			 uartsend(0xb5);

																	
      USART_SendData(USART1, 0xbb);
			while (!(USART1->SR & USART_FLAG_TXE));
			USART_SendData(USART1, 0x5b);
			while (!(USART1->SR & USART_FLAG_TXE));
			USART_SendData(USART1, CardNumber);
			while (!(USART1->SR & USART_FLAG_TXE));
			USART_SendData(USART1, 0x00);
			while (!(USART1->SR & USART_FLAG_TXE));
			USART_SendData(USART1, 0xb6);
			while (!(USART1->SR & USART_FLAG_TXE));
			USART_SendData(USART1, 0xb5);
			while (!(USART1->SR & USART_FLAG_TXE));


																	//printf("%2x %2x %2x %2x %2x %2x \n", 0xBB, 0x5B,CardNumber,0x00, 0xB6, 0xB5);				
																		RFID_status = RFID_DuKa_OK   ;																	
																		status=MI_ERR;
																
																}										
	case 	RFID_DuKa_OK :			
                             
				                     RFID_status = RFID_NO;							
	default: RFID_status = RFID_NO;
	
	return CardNumber;
	

// /*<<<<<<<<<<<<<<<<RFID<<<<<<<<<<<<<<*/	
/**********************����ΪRFID-RC522����********************/
// 	 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
// 		 // Delay(500);
//       GPIO_SetBits(GPIOB,GPIO_Pin_8);
// 		 // Delay(500);
// 		
// 			status = PcdRequest(PICC_REQALL,CT);/*����*/
// 				
// 			if(status==MI_OK)//�����ɹ�
// 			{
// 				//Ѱ���ɹ�֮��ͣһ��
// 						 motorQZ_control(TIM2,0,1,4);
// 						 motorQY_control(TIM2,0,2,4);
// 						 motorHZ_control(TIM2,0,3,4);
// 						 motorHY_control(TIM2,0,4,4);	
// 						 Delay(50);
// 						if(USFlag==1)	
// 								{
// 									MotoBelt(2);
// 									printf("OKOKOKOKOKOK");
// 									 Delay(5000);
// 									MotoBelt(0);
// 									USFlag=0;
// 								}		
// 						
// 				
// // 				printf("PcdRequest_MI_OK,  TagType is %02x%02x     \n",CT[0],CT[1]);	
// 				GPIO_SetBits(GPIOB,GPIO_Pin_8);
// // 				printf("PcdRequest_MI_OK         \n");				
// 			  status=MI_ERR;
//         status = PcdAnticoll(SN);/*����ײ*/		
// 				
// 				if (status==MI_OK)//���nײ
// 			  { 
// // 					printf("PcdAnticoll_MI_OK,  SN is %02x%02x%02x%02x     \n",SN[0],SN[1],SN[2],SN[3]);	
// // 					printf("PcdAnticoll_MI_OK         \n");
// 					status=MI_ERR;	
// 					status =PcdSelect(SN);//�x��
// 					if(status==MI_OK)
// 					{
// // 						printf("PcdSelect_MI_OK         \n ");
// 						status=MI_ERR;	
// 						status =PcdAuthState(0x60,0x09,KEY,SN);//��C
// 						if(status==MI_OK)
// 						{
// // 							printf("PcdAuthState_MI_OK         \n ");
// 							status=MI_ERR;	
// 							status=PcdRead(s,RFID);
// 							if(status==MI_OK)//�x���ɹ�
// 			        {
// // 									printf("READ_MI_OK the %d area data is  %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x     \n",s,RFID[0],RFID[1],RFID[2],RFID[3],RFID[4],RFID[5],RFID[6],RFID[7],RFID[8],RFID[9],RFID[10],RFID[11],RFID[12],RFID[13],RFID[14],RFID[15]);
// 								  status=MI_ERR;
// 									PWMPulseHigh=80;
					
// 						  }
// 							
// // 							status=PcdWrite(s,RFIDWrite);
// // 							if(status==MI_OK)//д���ɹ�
// // 			        {
// // // 									printf("Write_MI_OK the %d area data is  %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x     \n",s,RFIDWrite[0],RFIDWrite[1],RFIDWrite[2],RFIDWrite[3],RFIDWrite[4],RFIDWrite[5],RFIDWrite[6],RFIDWrite[7],RFIDWrite[8],RFIDWrite[9],RFIDWrite[10],RFIDWrite[11],RFIDWrite[12],RFIDWrite[13],RFIDWrite[14],RFIDWrite[15]);
// // 								testt=1;  
// // 								status=MI_ERR; 						
// // 						  }
// 						
// 					}
// 			
// 			 }

// 		}
	}			
		
/****************����ΪRFID-RC522����******************************/
}	


