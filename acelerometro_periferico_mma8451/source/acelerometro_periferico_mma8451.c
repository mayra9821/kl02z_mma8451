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

#define MMA851_I2C_DEVICE_ADDRESS	0x1D

#define MMA8451_WHO_AM_I_MEMORY_ADDRESS  0x0D
#define MMA8451_CTRL_REG1_ADDRESS  0x2A
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	enum MMA8451_DirMemory { //lista de direcciones de memoria para los 3 ejes del acelerometro
		OUT_X_MSB = 0x01,
		OUT_X_LSB, //0x02
		OUT_Y_MSB, //0x03
		OUT_Y_LSB,  //0x04
		OUT_Z_MSB,  //0x05
		OUT_Z_LSB,  //0x06
	};

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	(void) uart0Init(115200);

	(void) i2c0MasterInit(100000);	//100kbps

	PRINTF("Usar teclado para controlar LEDs \r\n");

	PRINTF("r-R led ROJO \r\n ");
	PRINTF("v-V LED verde\r\n");
	PRINTF("a-A LED azul\r\n");

	//resultado = gpioPutToggle(KPTB7);
	status_t status;
	uint8_t letra;
	uint8_t nuevo_dato_i2c;
	uint16_t dato_eje;
	uint16_t dato_MSB;
	uint8_t dato_LSB;

	status = i2c0MasterWriteByte(0x01, MMA851_I2C_DEVICE_ADDRESS, MMA8451_CTRL_REG1_ADDRESS); //Con esto activamos el Acelerometro

	/* Force the counter to be placed into memory. */
	/* Enter an infinite loop, just incrementing a counter. */
	while (1) {

		if (uart0Ready() > 0) {
			status = uart0Read(&letra);
			printf("dato entrante: %c \r\n", letra);

			switch (letra) {

			case 'a':
			case 'A':
				gpioPutToggle(KPTB10);
				break;
			case 'r':
				gpioPutValue(KPTB6, 0);
				break;
			case 'R':
				gpioPutValue(KPTB6, 1);
				break;
			case 'v':
				gpioPutHigh(KPTB7);
				break;
			case 'V':
				gpioPutLow(KPTB7);
				break;

			case 'M':
			case 'm':
				i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS,
				MMA8451_WHO_AM_I_MEMORY_ADDRESS);
				if (nuevo_dato_i2c == 0x1A)
					printf("MMA8451 encontrado!!\r\n");
				else
					printf("MMA8451 error\r\n");
				break;

			case 'x':
			case 'X':
				// VARIABLE DONDE SE ALMACENA EL DATO,  DIRECCION DEL ESCLAVO,  VARIABLE QUE VAS A LEER DEL ESCLAVO)
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,OUT_X_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,OUT_X_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje X:  %d \r\n", dato_eje);
				break;
			case 'y':
			case 'Y':
				// VARIABLE DONDE SE ALMACENA EL DATO,  DIRECCION DEL ESCLAVO,  VARIABLE QUE VAS A LEER DEL ESCLAVO)
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Y_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Y_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje Y: %d \r\n", dato_eje);
				break;
			case 'z':
			case 'Z':
				// VARIABLE DONDE SE ALMACENA EL DATO,  DIRECCION DEL ESCLAVO,  VARIABLE QUE VAS A LEER DEL ESCLAVO)
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,
						OUT_Z_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,
						OUT_Z_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje Z: %d \r\n", dato_eje);
				break;

			case 't':
			case 'T':
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,OUT_X_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,OUT_X_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje x: %d,", dato_eje);
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Y_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Y_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje y: %d,", dato_eje);
				i2c0MasterReadByte(&dato_MSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Z_MSB);
				i2c0MasterReadByte(&dato_LSB, MMA851_I2C_DEVICE_ADDRESS,OUT_Z_LSB);
				dato_MSB <<= 8;
				dato_eje = dato_MSB | dato_LSB;
				dato_eje >>= 2;
				printf("Lectura Eje Z: %d \r\n", dato_eje);
				break;

			default:
				printf("letra incorrecta :( \r\n");
				break;

			}

		}
	}
	return 0;
}
