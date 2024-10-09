#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define CLK 2
#define DATA 3
#define LATCH 4

#define START 6

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

	gpio_set_dir(DATA, GPIO_OUT);
	gpio_set_dir(CLK, GPIO_IN);
	gpio_set_dir(LATCH, GPIO_IN);

	gpio_set_irq_enabled_with_callback(LATCH, GPIO_IRQ_EDGE_RISE, true, gpioIRQHandler);
	while (true)
	{
		if (gpio_get(START))
		{
			snesState |= SNES_BUTTON_START;
		}
	}
}
