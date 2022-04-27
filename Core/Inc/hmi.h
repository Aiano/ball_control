/**
 * @file him.h
 * @brief 串口屏处理文件
 */
#ifndef BALL_CONTROL_HMI_H
#define BALL_CONTROL_HMI_H

#include "main.h"
#include "usart.h"

#define MAX_BUFFER_LENGTH 10
extern uint32_t target_position;

void hmi_init();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif //BALL_CONTROL_HMI_H
