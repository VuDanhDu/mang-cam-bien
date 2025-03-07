 #ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
void Timer2_Init(void);
void UART_Init(void);

void Delay_Ms(uint16_t ms);


void DHT11_Init(void);                     // Kh?i t?o c?m bi?n DHT11
void DHT11_Start(void);                    // G?i tín hi?u Start
uint8_t DHT11_Check_Response(void);        // Ki?m tra ph?n h?i t? DHT11
uint8_t DHT11_Read_Bit(void);              // Ð?c 1 bit t? DHT11
uint8_t DHT11_Read_Byte(void);             // Ð?c 1 byte t? DHT11
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);  // Ð?c nhi?t d? và d? ?m

#endif
