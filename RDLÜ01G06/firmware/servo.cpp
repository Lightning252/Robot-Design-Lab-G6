#include "servo.hpp"
#include <stdint.h>
#include "inc/stm32f10x_tim.h"

TIM_OCInitTypeDef* TIM_OCInitStruct;
TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct;

void servo_setAngle(enum Servos servo, int value){
if(value < -1800 || value > 1800){
return;
}

servo_init();

uint16_t period = 50;
TIM_TimeBaseInitTypeDef timerInitStructure;
timerInitStructure.TIM_Prescaler = 40000;
timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
timerInitStructure.TIM_Period = period;
timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
timerInitStructure.TIM_RepetitionCounter = 0;

switch(servo){
case 0://SERVO_FR_HIP
TIM_TimeBaseInit(TIM3, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM3,TIM_DMA_CC3,ENABLE);

TIM_CtrlPWMOutputs(TIM3,ENABLE);

TIM_OC3Init(TIM3,TIM_OCInitStruct);

TIM_SetCompare1 (TIM3, 1500);

TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM3, ENABLE);
break;
case 1://SERVO_FR_KNEE
TIM_TimeBaseInit(TIM3, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM3,TIM_DMA_CC3,ENABLE);

TIM_CtrlPWMOutputs(TIM3,ENABLE);

TIM_OC3Init(TIM3,TIM_OCInitStruct);

TIM_SetCompare1 (TIM3, 1500);

TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM3, ENABLE);
break;
case 2://SERVO_BR_HIP
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM3,TIM_DMA_CC3,ENABLE);

TIM_CtrlPWMOutputs(TIM3,ENABLE);

TIM_OC1Init(TIM3,TIM_OCInitStruct);

TIM_SetCompare3 (TIM3, 1500);

TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
case 3://SERVO_BR_KNEE
TIM_TimeBaseInit(TIM3, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM3,TIM_DMA_CC3,ENABLE);

TIM_CtrlPWMOutputs(TIM3,ENABLE);

TIM_OC3Init(TIM3,TIM_OCInitStruct);

TIM_SetCompare1 (TIM3, 1500);

TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM3, ENABLE);
break;
case 4://SERVO_BL_HIP
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM4,TIM_DMA_CC4,ENABLE);

TIM_CtrlPWMOutputs(TIM4,ENABLE);

TIM_OC4Init(TIM1,TIM_OCInitStruct);

TIM_SetCompare1 (TIM4, 1500);

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
case 5://SERVO_BL_KNEE
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM4,TIM_DMA_CC4,ENABLE);

TIM_CtrlPWMOutputs(TIM4,ENABLE);

TIM_OC4Init(TIM4,TIM_OCInitStruct);

TIM_SetCompare1 (TIM4, 1500);

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
case 6://SERVO_FL_HIP
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM4,TIM_DMA_CC4,ENABLE);

TIM_CtrlPWMOutputs(TIM4,ENABLE);

TIM_OC4Init(TIM4,TIM_OCInitStruct);

TIM_SetCompare1 (TIM4, 1500);

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
case 7://SERVO_FL_KNEE
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM4,TIM_DMA_CC4,ENABLE);

TIM_CtrlPWMOutputs(TIM4,ENABLE);

TIM_OC4Init(TIM4,TIM_OCInitStruct);

TIM_SetCompare1 (TIM4, 1500);

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
case 8://SERVO_NUM_SERVOS
TIM_TimeBaseInit(TIM4, &timerInitStructure);
//nach servo id initialisieren
TIM_DMACmd(TIM4,TIM_DMA_CC4,ENABLE);

TIM_CtrlPWMOutputs(TIM4,ENABLE);

TIM_OC4Init(TIM1,TIM_OCInitStruct);

TIM_SetCompare1 (TIM4, 1500);

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); 
	
TIM_Cmd(TIM4, ENABLE);
break;
default:
break;
}

}

void servo_init(){
TIM_OCStructInit(TIM_OCInitStruct);

//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

TIM_TimeBaseStructInit(TIM_TimeBaseInitStruct);


}
