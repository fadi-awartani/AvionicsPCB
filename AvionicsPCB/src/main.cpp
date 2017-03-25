/**
 * Main code file of Ryerson Rocketry avionics board application.
 **/
#include <asf.h>

int main (void)
{
	//Initializes clocks and modules.
	board_init();

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
