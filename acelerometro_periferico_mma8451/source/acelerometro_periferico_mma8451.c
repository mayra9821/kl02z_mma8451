#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"

#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		/* Init FSL debug console. */
		BOARD_InitDebugConsole();
	#endif
	(void)uart0Init(115200);
	PRINTF("Usar teclado para controlar LEDs \r\n");

	PRINTF("r-R led ROJO \r\n ");
	PRINTF("v-V LED verde\r\n");
	PRINTF("a-A LED azul\r\n");


	//resultado = gpioPutToggle(KPTB7);
	status_t status;
	uint8_t letra;
	/* Force the counter to be placed into memory. */
	/* Enter an infinite loop, just incrementing a counter. */
	while (1) {

		if (uart0Ready()>0){
			status=uart0Read(&letra);
			printf("dato entrante: %c \r\n", letra );

			switch (letra){

			case'a':
			case'A':
				gpioPutToggle(KPTB10);
				break;
			case'r':
				gpioPutValue(KPTB6,0);
				break;
			case'R':
				gpioPutValue(KPTB6,1);
				break;
			case'v':
				gpioPutHigh(KPTB7);
				break;
			case'V':
				gpioPutLow(KPTB7);
				break;

			default:
				printf("letra incorrecta :( \r\n");
				break;
			}

		}
	}
	return 0;
}
