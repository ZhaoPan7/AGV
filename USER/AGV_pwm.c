/**PWM����**/
/**PWM����:ע�⣬���ļ���Ҫ����<stm32f10x.tim.h>**/
/**PWM����:ע�⣬���ļ���Ҫ����<stm32f10x.tim.h>,������STM32F10X.CONF.H�зſ�#include "stm32f10x_tim.h" **/
#include "stm32f10x.h"
#include "AGV_pwm.h"    
#include "stm32f10x_tim.h"



/**ǰ����ģʽ����**/
#define   MotorQZ_IN1_Set     GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define   MotorQZ_IN1_Reset   GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define   MotorQZ_IN2_Set     GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define   MotorQZ_IN2_Reset   GPIO_ResetBits(GPIOA, GPIO_Pin_7)			

/**ǰ�ҵ��ģʽ����**/
#define   MotorQY_IN1_Set     GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define   MotorQY_IN1_Reset   GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define   MotorQY_IN2_Set     GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define   MotorQY_IN2_Reset   GPIO_ResetBits(GPIOB, GPIO_Pin_1)	

/**������ģʽ����**/
#define   MotorHZ_IN1_Set     GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define   MotorHZ_IN1_Reset   GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define   MotorHZ_IN2_Set     GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define   MotorHZ_IN2_Reset   GPIO_ResetBits(GPIOC, GPIO_Pin_13)		

/**���ҵ��ģʽ����**/
#define   MotorHY_IN1_Set     GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define   MotorHY_IN1_Reset   GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define   MotorHY_IN2_Set     GPIO_SetBits(GPIOC, GPIO_Pin_15)
#define   MotorHY_IN2_Reset   GPIO_ResetBits(GPIOC, GPIO_Pin_15)	


void PWM_RCC_Configuration(void)//SYSTEN CLOCK INITIALIZATION
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 

}


/***************  ����PWM��������õ���I/O�� *******************/
/**TIM2��4·Channel_PA0��ǰ��ת��__PA1��ǰ��ת��__PA2�����ת��__PA3�Һ���ת��**/
/**TIM3��4·Channel__PA6����__PA7����__PB0����__PB1����_**/
void PWM_GPIO_Configuration(void)	//PORT INITIALIZATION
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/**TIM2ѡ��û����Ӱ��ʽ**/
void TIM2_Configuration(void)//TIMER INITIALIZATION
{	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
//	GPIO_PinRemapConfig(_,_);//û����ӳ��
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	TIM_TimeBaseStructure.TIM_Period = 100-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =36-1;   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);   
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��ʱ���ж�

	//PWM��ʼ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);	
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);	
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);	
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2,ENABLE);
}



// /**TIM3ѡ��û����Ӱ��ʽ**/
// void TIM3_Configuration(void)//TIMER INITIALIZATION
// {	
// 	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure; 
// 	TIM_OCInitTypeDef TIM_OCInitStructure;
// 	
// //	GPIO_PinRemapConfig(_,_);//û����ӳ��
// //	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
// 	
// 	TIM_TimeBaseStructure.TIM_Period = 100-1; 
// 	TIM_TimeBaseStructure.TIM_Prescaler =36-1;   
// 	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 	
// 	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);   
// //	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��ʱ���ж�

// 	//PWM��ʼ��
// 	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
// 	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
// 	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
// 	
// 	TIM_OC1Init(TIM3,&TIM_OCInitStructure);	
// 	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
// 	TIM_OC2Init(TIM3,&TIM_OCInitStructure);	
// 	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
// 	TIM_OC3Init(TIM3,&TIM_OCInitStructure);	
// 	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
// 	TIM_OC3Init(TIM3,&TIM_OCInitStructure);	
// 	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
// 	
// 	TIM_Cmd(TIM3,ENABLE);
// }



//������ƺ������������PWM��ռ�ձȡ�
void motor_control(void)  //��ʼ���������
{
		TIM_SetCompare1(TIM2, 0);
		TIM_SetCompare2(TIM2, 0);
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, 0);
// 		TIM_SetCompare1(TIM3, 20);
// 		TIM_SetCompare2(TIM3, 40);
// 		TIM_SetCompare3(TIM3, 60);
// 		TIM_SetCompare4(TIM3, 80);
	
}
//ǰ��������
void motorQZ_control(TIM_TypeDef* TMM , uint16_t PWMPulse, u8 Channel, u8 Cmd)
{
		//PWM����
		if(Channel==1)
			TIM_SetCompare1(TMM, PWMPulse);
		else if(Channel==2)
			TIM_SetCompare2(TMM, PWMPulse);
		else if(Channel==3)
			TIM_SetCompare3(TMM, PWMPulse);
		else if(Channel==4)
			TIM_SetCompare4(TMM, PWMPulse);
		
		//ģʽ����
				if(Cmd==1)  //ģʽ0��ֹͣ
		{
			MotorQZ_IN1_Reset;
			MotorQZ_IN2_Reset;
		}
				if(Cmd==2)  //ģʽ1����ת
		{
			MotorQZ_IN1_Set;
			MotorQZ_IN2_Reset;
		}
				if(Cmd==3)  //ģʽ2����ת
		{
			MotorQZ_IN1_Reset;
			MotorQZ_IN2_Set;
		}
				if(Cmd==4)  //ģʽ3��ɲ��
		{
			MotorQZ_IN1_Set;
			MotorQZ_IN2_Set;
		}
		
}

//ǰ�ҵ������
void motorQY_control(TIM_TypeDef* TMM , uint16_t PWMPulse, u8 Channel, u8 Cmd)
{
		//PWM����
		if(Channel==1)
			TIM_SetCompare1(TMM, PWMPulse);
		else if(Channel==2)
			TIM_SetCompare2(TMM, PWMPulse);
		else if(Channel==3)
			TIM_SetCompare3(TMM, PWMPulse);
		else if(Channel==4)
			TIM_SetCompare4(TMM, PWMPulse);
		
		//ģʽ����
				if(Cmd==1)  //ģʽ0��ֹͣ
		{
			MotorQY_IN1_Reset;
			MotorQY_IN2_Reset;
		}
				if(Cmd==2)  //ģʽ1����ת
		{
			MotorQY_IN1_Set;
			MotorQY_IN2_Reset;
		}
				if(Cmd==3)  //ģʽ2����ת
		{
			MotorQY_IN1_Reset;
			MotorQY_IN2_Set;
		}
				if(Cmd==4)  //ģʽ3��ɲ��
		{
			MotorQY_IN1_Set;
			MotorQY_IN2_Set;
		}
		
}

//����������
void motorHZ_control(TIM_TypeDef* TMM , uint16_t PWMPulse, u8 Channel, u8 Cmd)
{
		//PWM����
		if(Channel==1)
			TIM_SetCompare1(TMM, PWMPulse);
		else if(Channel==2)
			TIM_SetCompare2(TMM, PWMPulse);
		else if(Channel==3)
			TIM_SetCompare3(TMM, PWMPulse);
		else if(Channel==4)
			TIM_SetCompare4(TMM, PWMPulse);
		
		//ģʽ����
				if(Cmd==1)  //ģʽ0��ֹͣ
		{
			MotorHZ_IN1_Reset;
			MotorHZ_IN2_Reset;
		}
				if(Cmd==2)  //ģʽ1����ת
		{
			MotorHZ_IN1_Set;
			MotorHZ_IN2_Reset;
		}
				if(Cmd==3)  //ģʽ2����ת
		{
			MotorHZ_IN1_Reset;
			MotorHZ_IN2_Set;
		}
				if(Cmd==4)  //ģʽ3��ɲ��
		{
			MotorHZ_IN1_Set;
			MotorHZ_IN2_Set;
		}
		
}

//���ҵ������
void motorHY_control(TIM_TypeDef* TMM , uint16_t PWMPulse, u8 Channel, u8 Cmd)
{
		//PWM����
		if(Channel==1)
			TIM_SetCompare1(TMM, PWMPulse);
		else if(Channel==2)
			TIM_SetCompare2(TMM, PWMPulse);
		else if(Channel==3)
			TIM_SetCompare3(TMM, PWMPulse);
		else if(Channel==4)
			TIM_SetCompare4(TMM, PWMPulse);
		
		//ģʽ����
				if(Cmd==1)  //ģʽ0��ֹͣ
		{
			MotorHY_IN1_Reset;
			MotorHY_IN2_Reset;
		}
				if(Cmd==2)  //ģʽ1����ת
		{
			MotorHY_IN1_Set;
			MotorHY_IN2_Reset;
		}
				if(Cmd==3)  //ģʽ2����ת
		{
			MotorHY_IN1_Reset;
			MotorHY_IN2_Set;
		}
				if(Cmd==4)  //ģʽ3��ɲ��
		{
			MotorHY_IN1_Set;
			MotorHY_IN2_Set;
		}
		
}






