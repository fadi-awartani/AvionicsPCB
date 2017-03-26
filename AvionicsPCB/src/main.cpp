/**
 * Main code file of Ryerson Rocketry avionics board application.
 *
 * 2017
 **/
#include <asf.h>

int main (void)
{	
	//Initializes clocks and modules.
	//sysclk_init();
	board_init();
	
	// USART options.
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = 19200,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
	
	usart_init_rs232(&AVR32_USART1, &USART_OPTIONS, 24000000);
	usart_write_line(&AVR32_USART1, "Hello, this is the AVR UC3 MCU saying hello!\r\n");
	//print_usb_debug("Began.\r\n");

	//GPIO Functions:
	//gpio_clr_gpio_pin(PIN);
	//gpio_set_gpio_pin(PIN);
	//gpio_tgl_gpio_pin(PIN);
	//gpio_get_pin_value(PIN);
	while(1) {
		gpio_tgl_gpio_pin(BLUE_LED_PIN);
		delay_ms(500);
	}
}
