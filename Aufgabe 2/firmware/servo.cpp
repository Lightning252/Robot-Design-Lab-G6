#include "servo.hpp"
#include <stdint.h>
#include "inc/stm32f10x_tim.h"
#include "inc/stm32f10x_gpio.h"

// Gibt dem angegebenen Servo das Signal sich in den angegebenen Winkel zu begeben. 
void servo_setAngle(enum Servos servo, int value){
if(value < 0 || value > 1700){
return;
}
int angle = value / 10;
int signal = (115200 / 170) * angle + 43200;

switch(servo){
case 0:
TIM_SetCompare1 (TIM3, signal);
break;
case 1:
TIM_SetCompare2 (TIM3, signal);
break;
case 2:
TIM_SetCompare3 (TIM3, signal);
break;
case 3:
TIM_SetCompare4 (TIM3, signal);
break;
case 4:
TIM_SetCompare1 (TIM4, signal);
break;
case 5:
TIM_SetCompare2 (TIM4, signal);
break;
case 6:
TIM_SetCompare3 (TIM4, signal);
break;
case 7:
TIM_SetCompare4 (TIM4, signal);
break;

case 8:

break;

default:
break;
}
}

// Initialisiert alle Timer und Channel
void servo_init(){

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

TIM_TimeBaseInitTypeDef timBase;
TIM_TimeBaseStructInit (&timBase);
timBase.TIM_Prescaler = 22;
timBase.TIM_ClockDivision = TIM_CKD_DIV1; //?
timBase.TIM_CounterMode = TIM_CounterMode_Up; //?
timBase.TIM_Period = 65455;
TIM_TimeBaseInit (TIM3, &timBase);
TIM_ARRPreloadConfig (TIM3, ENABLE); //?
TIM_TimeBaseInit (TIM4, &timBase);
TIM_ARRPreloadConfig (TIM4, ENABLE); //?

TIM_OCInitTypeDef oc;
TIM_OCStructInit (&oc);

oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;
oc.TIM_OCMode = TIM_OCMode_PWM1; //?
oc.TIM_OCPolarity = TIM_OCPolarity_Low; //?
oc.TIM_OutputState = TIM_OutputState_Enable; //?
oc.TIM_Pulse = 0; 

TIM_OC1Init (TIM3, &oc);
TIM_OC1PreloadConfig (TIM3, TIM_OCPreload_Enable);

TIM_OC2Init (TIM3, &oc);
TIM_OC2PreloadConfig (TIM3, TIM_OCPreload_Enable);

TIM_OC3Init (TIM3, &oc);
TIM_OC3PreloadConfig (TIM3, TIM_OCPreload_Enable);

TIM_OC4Init (TIM3, &oc);
TIM_OC4PreloadConfig (TIM3, TIM_OCPreload_Enable);

TIM_OC1Init (TIM4, &oc);
TIM_OC1PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC2Init (TIM4, &oc);
TIM_OC2PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC3Init (TIM4, &oc);
TIM_OC3PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC4Init (TIM4, &oc);
TIM_OC4PreloadConfig (TIM4, TIM_OCPreload_Enable);

GPIO_InitTypeDef gpioStructure;
GPIO_StructInit(&gpioStructure);
gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP; //?
gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
gpioStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_Init(GPIOA,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_7;
GPIO_Init(GPIOA,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_Init(GPIOB,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_Init(GPIOB,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_Init(GPIOB,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_7;
GPIO_Init(GPIOB,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_Init(GPIOB,&gpioStructure);
gpioStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_Init(GPIOB,&gpioStructure);
    
TIM_Cmd (TIM3, ENABLE);
TIM_Cmd (TIM4, ENABLE);
}
/**
void overruntest(){
	//prescaler, periode, pulse anpassen! Zähler?

	TIM_TimeBaseInitTypeDef timBase;
	TIM_TimeBaseStructInit(&timBase);
	timBase.TIM_Prescaler = 22;
	timBase.TIM_ClockDivision = TIM_CKD_DIV1; //?
	timBase.TIM_CounterMode = TIM_CounterMode_Up; //?
	timBase.TIM_Period = 65455;
	TIM_TimeBaseInit(TIM3, &timBase);

	TIM_OCInitTypeDef oc;
	TIM_OCStructInit(&oc);

	oc.TIM_OCMode = TIM_OCMode_PWM1; //?
	oc.TIM_OCPolarity = TIM_OCPolarity_High; //?
	oc.TIM_OutputState = TIM_OutputState_Enable; //?
	oc.TIM_Pulse = 0;

	TIM_OC1Init(TIM3, &oc);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3, ENABLE);
}
*/
void cameraclock(){

	GPIO_InitTypeDef gpioStructure;
	GPIO_StructInit(&gpioStructure);
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB,&gpioStructure);
	gpioStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB,&gpioStructure);

	TIM_OCInitTypeDef oc;
	TIM_OCStructInit (&oc);

	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	oc.TIM_OCMode = TIM_OCMode_PWM1;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 1000; 

	TIM_OC1Init (TIM1, &oc);

	TIM_TimeBaseInitTypeDef timBase;
	TIM_TimeBaseStructInit(&timBase);
	//(Formel : 
	timBase.TIM_Prescaler = 22;
	timBase.TIM_ClockDivision = TIM_CKD_DIV1;
	timBase.TIM_CounterMode = TIM_CounterMode_Up;
	timBase.TIM_Period = 2000;
	TIM_TimeBaseInit(TIM1, &timBase);

	TIM_OC1Init(TIM1, &oc);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}





