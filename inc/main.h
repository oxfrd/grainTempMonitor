#pragma once

#ifdef __cplusplus
 extern "C" {
#endif
    __attribute__((interrupt)) void TIM2_IRQHandler();
#ifdef __cplusplus
}
#endif

int main();