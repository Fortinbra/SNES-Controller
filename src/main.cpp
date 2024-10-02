#include <stdio.h>
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <cstdlib>
#include <string.h>
#include <hardware/gpio.h>
#include <hardware/irq.h>

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define CLK 2
#define DATA 3
#define LATCH 4

#define START 6

void gpioIRQHandler(uint gpio, uint32_t events)
{
}

int main()
{
	stdio_init_all();
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	gpio_init(DATA);
	gpio_init(CLK);

	gpio_set_dir(DATA, GPIO_OUT);
	gpio_set_dir(CLK, GPIO_IN);
	gpio_set_dir(LATCH, GPIO_IN);

	gpio_set_irq_enabled_with_callback(LATCH, GPIO_IRQ_EDGE_RISE, true, gpioIRQHandler);
}
