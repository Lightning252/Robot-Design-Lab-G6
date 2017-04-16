#ifndef CST_ASSERT_H
#define CST_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/stm32f10x_gpio.h"

enum ASSERT_UASRT {
  USE_USART1,
  USE_USART2,
};

void Assert_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, enum ASSERT_UASRT used_usart);

#ifdef NDEBUG
#define assert(x)
#else
#define assert(x) do {					\
    if (!(x)) {						\
      assert_failed((uint8_t *)__FILE__, __LINE__);	\
    }							\
  } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif
