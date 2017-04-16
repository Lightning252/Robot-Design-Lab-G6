#ifndef __STM32_SPECIFIC_H
#define __STM32_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif

void Assert_Configuration();
void baseNvicInit();
void GPIO_Configuration(void);
void systick_init();

#ifdef __cplusplus
}
#endif

#endif
