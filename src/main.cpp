#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define CLK 2
#define DATA 3
#define LATCH 4

#define SNES_BUTTON_A_PIN 6
#define SNES_BUTTON_B_PIN 7
#define SNES_BUTTON_X_PIN 8
#define SNES_BUTTON_Y_PIN 9
#define SNES_BUTTON_L_PIN 10
#define SNES_BUTTON_R_PIN 11
#define SNES_BUTTON_SELECT_PIN 12
#define SNES_BUTTON_START_PIN 13
#define SNES_BUTTON_UP_PIN 14
#define SNES_BUTTON_DOWN_PIN 15
#define SNES_BUTTON_LEFT_PIN 16
#define SNES_BUTTON_RIGHT_PIN 17

const uint16_t SNES_BUTTON_A = 0x01;
const uint16_t SNES_BUTTON_B = 0x02;
const uint16_t SNES_BUTTON_X = 0x04;
const uint16_t SNES_BUTTON_Y = 0x08;
const uint16_t SNES_BUTTON_L = 0x10;
const uint16_t SNES_BUTTON_R = 0x20;
const uint16_t SNES_BUTTON_SELECT = 0x40;
const uint16_t SNES_BUTTON_START = 0x80;
const uint16_t SNES_BUTTON_UP = 0x100;
const uint16_t SNES_BUTTON_DOWN = 0x200;
const uint16_t SNES_BUTTON_LEFT = 0x400;
const uint16_t SNES_BUTTON_RIGHT = 0x800;
const uint16_t buttonOrder[16] = {
	SNES_BUTTON_B,
	SNES_BUTTON_Y,
	SNES_BUTTON_SELECT,
	SNES_BUTTON_START,
	SNES_BUTTON_UP,
	SNES_BUTTON_DOWN,
	SNES_BUTTON_LEFT,
	SNES_BUTTON_RIGHT,
	SNES_BUTTON_A,
	SNES_BUTTON_X,
	SNES_BUTTON_L,
	SNES_BUTTON_R,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF // Unknown buttons always report high
};

uint16_t snesState;

void gpioIRQHandler(uint gpio, uint32_t events)
{
	gpio_put(DATA, ((snesState & buttonOrder[0]) != 0));
	for (int i = 1; i < 16; i++)
	{
		while (gpio_get(CLK) == 1)
		{
			tight_loop_contents();
		}
		if (i < 12)
		{
			gpio_put(DATA, ((snesState & buttonOrder[i]) != 0));
		}
		else
		{
			gpio_put(DATA, 1);
		}
	}
	gpio_put(DATA, 0);
}

int main()
{
	stdio_init_all();
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	snesState = 0x00;
	gpio_init(DATA);
	gpio_init(CLK);
	gpio_init(LATCH);
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_init(SNES_BUTTON_START_PIN);
	gpio_init(SNES_BUTTON_A_PIN);
	gpio_init(SNES_BUTTON_B_PIN);
	gpio_init(SNES_BUTTON_X_PIN);
	gpio_init(SNES_BUTTON_Y_PIN);
	gpio_init(SNES_BUTTON_L_PIN);
	gpio_init(SNES_BUTTON_R_PIN);
	gpio_init(SNES_BUTTON_SELECT_PIN);
	gpio_init(SNES_BUTTON_UP_PIN);
	gpio_init(SNES_BUTTON_DOWN_PIN);
	gpio_init(SNES_BUTTON_LEFT_PIN);
	gpio_init(SNES_BUTTON_RIGHT_PIN);

	gpio_set_dir(DATA, GPIO_OUT);
	gpio_set_dir(CLK, GPIO_IN);
	gpio_set_dir(LATCH, GPIO_IN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	gpio_set_dir(SNES_BUTTON_START_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_A_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_B_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_X_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_Y_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_L_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_R_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_SELECT_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_UP_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_DOWN_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_LEFT_PIN, GPIO_IN);
	gpio_set_dir(SNES_BUTTON_RIGHT_PIN, GPIO_IN);

	gpio_pull_up(SNES_BUTTON_START_PIN);
	gpio_pull_up(SNES_BUTTON_A_PIN);
	gpio_pull_up(SNES_BUTTON_B_PIN);
	gpio_pull_up(SNES_BUTTON_X_PIN);
	gpio_pull_up(SNES_BUTTON_Y_PIN);
	gpio_pull_up(SNES_BUTTON_L_PIN);
	gpio_pull_up(SNES_BUTTON_R_PIN);
	gpio_pull_up(SNES_BUTTON_SELECT_PIN);
	gpio_pull_up(SNES_BUTTON_UP_PIN);
	gpio_pull_up(SNES_BUTTON_DOWN_PIN);
	gpio_pull_up(SNES_BUTTON_LEFT_PIN);
	gpio_pull_up(SNES_BUTTON_RIGHT_PIN);

	gpio_set_irq_enabled_with_callback(LATCH, GPIO_IRQ_EDGE_RISE, true, gpioIRQHandler);

	while (true)
	{
		snesState = 0; // Reset state

		if (gpio_get(SNES_BUTTON_START_PIN) == 0)
		{
			snesState |= SNES_BUTTON_START;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_A_PIN) == 0)
		{
			snesState |= SNES_BUTTON_A;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_B_PIN) == 0)
		{
			snesState |= SNES_BUTTON_B;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_X_PIN) == 0)
		{
			snesState |= SNES_BUTTON_X;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_Y_PIN) == 0)
		{
			snesState |= SNES_BUTTON_Y;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_L_PIN) == 0)
		{
			snesState |= SNES_BUTTON_L;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_R_PIN) == 0)
		{
			snesState |= SNES_BUTTON_R;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_SELECT_PIN) == 0)
		{
			snesState |= SNES_BUTTON_SELECT;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_UP_PIN) == 0)
		{
			snesState |= SNES_BUTTON_UP;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_DOWN_PIN) == 0)
		{
			snesState |= SNES_BUTTON_DOWN;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_LEFT_PIN) == 0)
		{
			snesState |= SNES_BUTTON_LEFT;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}
		if (gpio_get(SNES_BUTTON_RIGHT_PIN) == 0)
		{
			snesState |= SNES_BUTTON_RIGHT;
			gpio_put(PICO_DEFAULT_LED_PIN, true);
		}

		sleep_ms(100);
	}
}
