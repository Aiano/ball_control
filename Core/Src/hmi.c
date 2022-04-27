/**
 * @file him.c
 * @brief 串口屏处理文件
 */

#include <stdio.h>
#include <string.h>
#include "hmi.h"

uint8_t hmiRxBuffer[MAX_BUFFER_LENGTH]; // 接收缓冲数组
uint8_t openmvRxBuffer[MAX_BUFFER_LENGTH];

void hmi_init() {
    HAL_UART_Receive_IT(&huart3, hmiRxBuffer, 4);
    HAL_UART_Receive_IT(&huart2, openmvRxBuffer, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // 当进入到这个函数，说明串口接收中断被触发了
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    if (huart == &huart3) {
        static uint32_t RxNumber; // 每次按确认键，一次性传4Byte过来，故总共32bit
        RxNumber = 0; // 清零
        for (int i = 0; i < 4; i++) {
            RxNumber += hmiRxBuffer[i] << (i * 8); // 每Byte比前一Byte多左移8bit
        }
        target_position = RxNumber;
        // 重新设置串口接收中断
        HAL_UART_Receive_IT(huart, hmiRxBuffer, 4);
    } else if (huart == &huart2) {
        int i = 0;
        while (openmvRxBuffer[i++] != '\n') {
            HAL_UART_Receive(&huart2, openmvRxBuffer + i, 1, 0xff);
        }
        static int cx;
        sscanf(openmvRxBuffer, "%d\n", &cx);
        memset(openmvRxBuffer, 0, sizeof(openmvRxBuffer));
        // 重新设置串口接收中断
        HAL_UART_Receive_IT(huart, hmiRxBuffer, 4);
    }

}