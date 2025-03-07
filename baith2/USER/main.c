#include "stm32f10x.h"
#include "dht11.h"
#include <stdio.h>
#include <string.h>

void UART_Send_String(char *str);

void Timer2_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInit;
    timerInit.TIM_CounterMode = TIM_CounterMode_Up;
    timerInit.TIM_Period = 0xFFFF;
    timerInit.TIM_Prescaler = 72 - 1; // 1 tick = 1µs
    timerInit.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &timerInit);
    TIM_Cmd(TIM2, ENABLE);
}

void Delay_Ms(uint16_t ms) {
    while (ms--) {
        TIM_SetCounter(TIM2, 0);
        while (!(TIM2->SR & TIM_SR_UIF));  
        TIM2->SR &= ~TIM_SR_UIF;          
    }
}

void UART_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    // TX - PA9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // RX - PA10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);

    USART_Cmd(USART1, ENABLE);
}

void UART_Send_String(char *str) {
    while (*str) {
        USART_SendData(USART1, *str++);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

int main(void) {
    uint8_t temperature = 0, humidity = 0;
    char buffer[32];

    Timer2_Init();
    UART_Init();
    DHT11_Init();

    while (1) {
        if (DHT11_Read_Data(&temperature, &humidity)) {
            snprintf(buffer, sizeof(buffer), "Nhiet do: %dC\r\n", temperature);
            UART_Send_String(buffer);
            snprintf(buffer, sizeof(buffer), "Do am: %d%%\r\n", humidity);
            UART_Send_String(buffer);
        } else {
            UART_Send_String("Loi DHT11\r\n");
        }
        Delay_Ms(2000);
    }
}
