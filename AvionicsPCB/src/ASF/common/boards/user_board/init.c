/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

static void clockfrequencies_configure(void)
{
	// Switch the main clock source to Osc0.
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

	// Setup PLL0 on Osc0, mul=10 ,no divisor, lockcount=16: 12Mhzx8 = 96MHz output
	pm_pll_setup(&AVR32_PM, 0,  // pll.
	8,  // mul.
	1,   // div.
	0,   // osc.
	16); // lockcount.
	// PLL output VCO frequency is 96MHz.
	// We divide it by 2 with the pll_div2=1 to get a main clock at 48MHz.
	pm_pll_set_option(&AVR32_PM, 0, // pll.
	1,  // pll_freq.
	1,  // pll_div2.
	0); // pll_wbwdisable.
	// Enable the PLL.
	pm_pll_enable(&AVR32_PM, 0);
	// Wait until the PLL output is stable.
	pm_wait_for_pll0_locked(&AVR32_PM);
	// Configure each clock domain to use the main clock divided by 2
	// => fCPU = fPBA = fPBB = 24MHz.
	pm_cksel(&AVR32_PM,
	1,   // pbadiv.
	0,   // pbasel.
	1,   // pbbdiv.
	0,   // pbbsel.
	1,   // hsbdiv=cpudiv
	0);  // hsbsel=cpusel
	// Switch the main clock source to PLL0.
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);
}

/*! \brief Initializes the HSB bus matrix (use last master as default for the
 *   flash controller)
 */
static void init_hmatrix(void)
{
  union
  {
    unsigned long                 scfg;
    avr32_hmatrix_scfg_t          SCFG;
  } u_avr32_hmatrix_scfg = {AVR32_HMATRIX.scfg[AVR32_HMATRIX_SLAVE_FLASH]};
  u_avr32_hmatrix_scfg.SCFG.defmstr_type = AVR32_HMATRIX_DEFMSTR_TYPE_LAST_DEFAULT;
  AVR32_HMATRIX.scfg[AVR32_HMATRIX_SLAVE_FLASH] = u_avr32_hmatrix_scfg.scfg;
}

void board_init(void)
{
	clockfrequencies_configure();
// 	pm_pll_set_option(&AVR32_PM, 0, // pll.
// 	1,  // pll_freq.
// 	1,  // pll_div2.
// 	0); // pll_wbwdisable.
	
	// initialize hmatrix bus
	//init_hmatrix();
	
	//Initializing GPIO Module:
	/*
		--PORT A--
		GPER: -32- 0011 0000 0111 0010
			       0000 0000 0000 0000 -0- == 0x30720000
			-Enables GPIO function for corresponding pin.
				   
		PMR0: -32- 0000 1100 0000 0000
				   0010 0000 0000 0000 -0- == 0x0C002000
				   
		PMR1: -32- 0000 0000 0000 0000
				   0000 0001 1110 0000 -0- == 0x1E0
			- [PMR1,PMR0] selects pin function if not GPIO. (00 = Function A, 01 = Function B, etc...)
				   
		--PORT B--
		GPER: -32- 0000 0000 0000 0000
				   0000 0011 1100 1111 -0- == 0x3CF
				   
		PMR0: -32- 0000 0000 0000 0000
				   0000 0000 0000 0000 -0- == 0x0
				   		
		PMR1: -32- 0000 0000 0000 0000
				   0000 1100 0000 0000 -0- == 0xC00
	*/
	volatile avr32_gpio_port_t *gpio_portA = &AVR32_GPIO.port[0];
	volatile avr32_gpio_port_t *gpio_portB = &AVR32_GPIO.port[1];
	
	gpio_portA->gper = 0x30720000UL;
	gpio_portA->pmr0 = 0x0C002000UL;
	gpio_portA->pmr1 = 0x1E0;
	
	gpio_portB->gper = 0x3CF;
	gpio_portB->pmr0 = 0;
	gpio_portB->pmr1 = 0xC00;
	
	// Initialize and enable interrupt
	irq_initialize_vectors();
	cpu_irq_enable();
	
	/* TWI == I2C
	twi_options_t opt;
	opt.pba_hz = BOARD_FREQ_HZ;
	opt.speed = I2C_SPEED;
	opt.chip = ALTIMETER_I2C_ADDR;
	
	// initialize TWI driver with options
	int status = twi_master_init(&AVR32_TWI, &opt);
	// check init result
	if (status == TWI_SUCCESS){
		
	}*/

	// Start USB stack to authorize VBus monitoring
	//udc_start();
}
