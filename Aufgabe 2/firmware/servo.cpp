#include "servo.hpp"
#include <stdint.h>
#include "inc/stm32f10x_tim.h"

/**
I/O_VCC1 FR_1 front right hip PA6
I/O_VCC2 FR_2 front right knee PA7
I/O_VCC3 BR_1 back right hip PB0
I/O_VCC4 BR_2 back right knee PB1
I/O_VCC5 BL_1 back left hip PB6
I/O_VCC6 BL_2 back left knee PB7
I/O_VCC7 FL_1 front left hip PB8
I/O_VCC8 FL_2 front left knee PB9
*/

//einmal zu Beginn initialisieren oder immer neu? https://www.mikrocontroller.net/topic/245863

void servo_setAngle(enum Servos servo, int value){
if(value < 0 || value > 1700){
return;
}

//do the math
float impTime = 1.6 / 170;
impTime = (impTime * value) + 0.6;


int signal = 0;

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

void servo_init(){
TIM_TimeBaseInitTypeDef timBase;

TIM_TimeBaseStructInit (&timBase);
timBase.TIM_Prescaler = ((SystemCoreClock / 2 ) / 1000000)-1; //?
timBase.TIM_ClockDivision = TIM_CKD_DIV1;
timBase.TIM_CounterMode = TIM_CounterMode_Up;
timBase.TIM_Period = 2000;
TIM_TimeBaseInit (TIM3, &timBase);
TIM_ARRPreloadConfig (TIM3, ENABLE);
TIM_TimeBaseInit (TIM4, &timBase);
TIM_ARRPreloadConfig (TIM4, ENABLE);

// Output compare match
TIM_OCInitTypeDef oc;
TIM_OCStructInit (&oc);

oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;
oc.TIM_OCMode = TIM_OCMode_PWM1;
oc.TIM_OCPolarity = TIM_OCPolarity_High;
oc.TIM_OutputState = TIM_OutputState_Enable;
oc.TIM_Pulse = 1; // Initiale Pulsweite in Millisekunden

TIM_OC1Init (TIM3, &oc);
TIM_OC1PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC2Init (TIM3, &oc);
TIM_OC2PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC3Init (TIM3, &oc);
TIM_OC3PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC4Init (TIM3, &oc);
TIM_OC4PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC1Init (TIM4, &oc);
TIM_OC1PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC2Init (TIM4, &oc);
TIM_OC2PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC3Init (TIM4, &oc);
TIM_OC3PreloadConfig (TIM4, TIM_OCPreload_Enable);

TIM_OC4Init (TIM4, &oc);
TIM_OC4PreloadConfig (TIM4, TIM_OCPreload_Enable);

/**
TIM_BDTRInitTypeDef bdtr;
TIM_BDTRStructInit (&bdtr);
bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
TIM_BDTRConfig (TIM4, &bdtr);*/

//connect channels to GPIO -> PA6, PA7, PB0, PB1, PB6, PB7, PB8, PB9 ?

/**
void TIM_DMACmd(TIM_TypeDef* TIMx, u16 TIM_DMASource, FunctionalState NewState);
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState);
*/

// Start the timer
TIM_Cmd (TIM3, ENABLE);
TIM_Cmd (TIM4, ENABLE);
}



