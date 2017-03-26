/*
 * usb_debug.c
 *
 * Created: Mar 25, 2017 8:24:16 PM
 *  Author: Fadi
 */ 

#include <asf.h>

//Prints message through USB (as a virtual COM Port)
int print_usb_debug(char* message) {
	while (*message != '\0') {
		// Transfer char  to CDC TX
		if (!udi_cdc_is_tx_ready()) {
			// Fifo full
			udi_cdc_signal_overrun();
			} else {
			udi_cdc_putc(*message);
		}
		message++;
	}
}