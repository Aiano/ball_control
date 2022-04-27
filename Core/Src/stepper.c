#include "stepper.h"

int delay_ms = 2; // 延时时长
int target_pulse = 0; // 目标脉冲数
int now_pulse = 0; // 当前脉冲数

char get_pulse_command[] = {0xe0, 0x36};
char rx_pulse_buffer[5];
int rx_pulse;

void get_pulse() {
    HAL_UART_Transmit(&huart1, get_pulse_command, 2, 0xff);
    HAL_Delay(10);
    HAL_UART_Receive(&huart1, rx_pulse_buffer, 5, 0xff);
}

/**
 * @brief 初始化步进电机
 * @details 启用定时器中断和拉高使能电平
 */
void stepper_init() {
    get_pulse();

    HAL_TIM_Base_Start_IT(&htim2); // 只需要启动一次
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET); // 拉高使能EN
}

/**
 * @brief 产生一次脉冲给步进电机
 */
void stepper_pulse() {
    HAL_GPIO_WritePin(STP_GPIO_Port, STP_Pin, GPIO_PIN_SET);
    HAL_Delay(delay_ms);
    HAL_GPIO_WritePin(STP_GPIO_Port, STP_Pin, GPIO_PIN_RESET);
    HAL_Delay(delay_ms);
}

/**
 * @brief 定时器溢出中断回调函数
 * @details 每5ms（200Hz）调用一次
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    static char pulse_flag = 0;

    // 如果已经到达目标脉冲数，则跳出函数
    if (now_pulse == target_pulse) {
        return;
    }

    if (now_pulse < target_pulse) {
        HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
    } else if (now_pulse > target_pulse) {
        HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
    }

    if (pulse_flag == 0) { // 上升沿
        pulse_flag = 1;
        HAL_GPIO_WritePin(STP_GPIO_Port, STP_Pin, GPIO_PIN_SET);
    } else { // 下降沿
        pulse_flag = 0;
        HAL_GPIO_WritePin(STP_GPIO_Port, STP_Pin, GPIO_PIN_RESET);

        if (now_pulse < target_pulse) {
            now_pulse++;
        } else if (now_pulse > target_pulse) {
            now_pulse--;
        }
    }

}
