#include "dht11.h"
#include "stm32f10x_tim.h"


void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DHT11_Start(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);  
    Delay_Ms(18);                       
    GPIO_SetBits(GPIOB, GPIO_Pin_12);    
    Delay_Ms(1);
}


uint8_t DHT11_Check_Response(void) {
   // uint16_t time = 0;

    
    TIM_SetCounter(TIM2, 0);
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) {
        if (TIM_GetCounter(TIM2) > 100) return 0;
    }

   
    TIM_SetCounter(TIM2, 0);
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) {
        if (TIM_GetCounter(TIM2) > 100) return 0;
    }

    return 1;
}


uint8_t DHT11_Read_Bit(void) {
    //uint16_t time = 0;

   
    TIM_SetCounter(TIM2, 0);
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) {
        if (TIM_GetCounter(TIM2) > 100) return 0;
    }

    
    TIM_SetCounter(TIM2, 0);
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) {
        if (TIM_GetCounter(TIM2) > 100) return 0;
    }

   
    return (TIM_GetCounter(TIM2) > 28) ? 1 : 0;
}


uint8_t DHT11_Read_Byte(void) {
    uint8_t i, byte = 0;

    for (i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= DHT11_Read_Bit();
    }

    return byte;
}


uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi) {
    uint8_t data[5] = {0};
    uint8_t i;

    DHT11_Start();
    if (!DHT11_Check_Response()) return 0;

    
    for (i = 0; i < 5; i++) {
        data[i] = DHT11_Read_Byte();
    }

    
    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) return 0;

    *humi = data[0];
    *temp = data[2];

    return 1;
}
