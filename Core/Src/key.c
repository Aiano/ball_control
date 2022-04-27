#include "key.h"


/**
 * @brief 重写中断回调函数
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static char on_push_button = 0; // 按键按下标志

    if (on_push_button == 0) {
        on_push_button = 1;
        switch (GPIO_Pin) {
            case KEY1_Pin:
                target_pulse += 1;
                break;
            case KEY2_Pin:
                target_pulse -= 1;
                break;
            case KEY3_Pin:
                break;
        }

        // HAL_Delay(10);
        on_push_button = 0;
    }
}