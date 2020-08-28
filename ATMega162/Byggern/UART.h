
#ifndef UART_H
#define UART_H

void uart_init(unsigned int baud_rate);
void uart_send_char(unsigned char data);
unsigned char uart_receive();
void uart_send_string(char* data);

#endif UART_H