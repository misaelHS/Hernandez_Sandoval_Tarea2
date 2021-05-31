//TAREA 2: SECUENCIA DE ENCENDIDO DE LEDs, ASCENDENTE Y DESCENDENTE MEDIANTE PUSH BOTON, A TRAVÉS DE POLLING

#include <stdio.h>
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"

#define CORE_FREQ 21000000u		//SE DEFINE LA VARIABLE CON A LA FRECUENCIA DEL SISTEMA
#define DELAY 100000u			//SE DEFINE LA VARIABLE CON UN VALOR PARA RETARDO

gpio_pin_config_t led1_config={  // SE CONFIGURA EL LED 1 COMO SALIDA A TRAVES DEL GPIO
		kGPIO_DigitalOutput,
		1,
};


gpio_pin_config_t led2_config={ // SE CONFIGURA EL LED 2 COMO SALIDA A TRAVES DEL GPIO
		kGPIO_DigitalOutput,
		1,
};

gpio_pin_config_t led3_config={ // SE CONFIGURA EL LED 3 COMO SALIDA A TRAVES DEL GPIO
		kGPIO_DigitalOutput,
		1,
};

gpio_pin_config_t sw1_config={	// SE CONFIGURA EL SW1 COMO ENTRADA
		kGPIO_DigitalInput,
		0,
};

gpio_pin_config_t sw2_config={	// SE CONFIGURA EL SW2 COMO ENTRADA
		kGPIO_DigitalInput,
		0,
};


#define PIN21 21u	// SE CONFIGURA EL PIN21, PARA EL LED AZUL
#define PIN22 22u   // SE CONFIGURA EL PIN22, PARA EL LED ROJO
#define PIN26 26u	// SE CONFIGURA EL PIN26, PARA EL LED VERDE


#define PIN06 6u	// SE CONFIGURA EL PIN06, PARA EL SW2
#define PIN04 4u	// SE CONFIGURA EL PIN04, PARA EL SW1

volatile uint32_t i=0;	// DECLARACION Y ASIGNACION DE LA VARIABLE i CON EL MODIFICADOR VOLATILE
						// PARA OPTIMIZAR EN EL PROCESO DE COMPILACION

int main(void) {

	const port_pin_config_t porta_pin4_config = {	// CONFIGURACION PARA EL SW 1
		    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
		    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		  };


	const port_pin_config_t porta_pin6_config = { 	// CONFIGURACION PARA EL SW 2
		    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		    kPORT_LowDriveStrength,                                  /* High drive strength is configured */
		    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		  };

	CLOCK_EnableClock(kCLOCK_PortA); // SE HABILITA EL CLOCK DEL PUERTO A, PARA EL USO DE SW1
	CLOCK_EnableClock(kCLOCK_PortB); // SE HABILITA EL CLOCK DEL PUERTO B, PARA EL USO DE LED AZUL Y LED ROJO
	CLOCK_EnableClock(kCLOCK_PortC); // SE HABILITA EL CLOCK DEL PUERTO C, PARA EL USO DE SW2
	CLOCK_EnableClock(kCLOCK_PortE); // SE HABILITA EL CLOCK DEL PUERTO A, PARA EL USO DE LED VERDE

	PORT_SetPinMux(PORTB, PIN21, kPORT_MuxAsGpio); //CONFIGURACION DEL PIN21 DEL PUERTO B MULTIPLEXADO, PARA USO DEL LED AZUL
	PORT_SetPinMux(PORTB, PIN22, kPORT_MuxAsGpio); //CONFIGURACION DEL PIN22 DEL PUERTO B MULTIPLEXADO, PARA USO DEL LED ROJO
	PORT_SetPinMux(PORTE, PIN26, kPORT_MuxAsGpio); //CONFIGURACION DEL PIN26 DEL PUERTO E MULTIPLEXADO, PARA USO DEL LED VERDE

	PORT_SetPinMux(PORTA, PIN04, kPORT_MuxAsGpio); //CONFIGURACION DEL PIN04 DEL PUERTO A MULTIPLEXADO, PARA USO DEL SW3
	PORT_SetPinMux(PORTC, PIN06, kPORT_MuxAsGpio); //CONFIGURACION DEL PIN06 DEL PUERTO C MULTIPLEXADO, PARA USO DEL SW2

	GPIO_PinInit(GPIOB, PIN21, &led1_config); // CONFIGURACION DEL GPIO DEL PIN21, PUERTO B, COMO SALIDA
	GPIO_PinInit(GPIOB, PIN22, &led2_config); // CONFIGURACION DEL GPIO DEL PIN22, PUERTO B, COMO SALIDA
	GPIO_PinInit(GPIOE, PIN26, &led3_config); // CONFIGURACION DEL GPIO DEL PIN26, PUERTO E, COMO SALIDA

	GPIO_PinInit(GPIOA, PIN04, &sw1_config); // CONFIGURACION DEL GPIO DEL PIN04, PUERTO A, COMO SALIDA
	GPIO_PinInit(GPIOC, PIN06, &sw2_config); // CONFIGURACION DEL GPIO DEL PIN06, PUERTO C, COMO SALIDA


	while(1) {

		if(!GPIO_PinRead(GPIOC, PIN06)) // INCREMENTA EL CONTADOR i EN CADA PULSO DEL SW2 DE LA TARJETA
		{
			SDK_DelayAtLeastUs(DELAY, CORE_FREQ); // RETARDO PARA REDUCIR LOS REBOTES DEL SW
			++i;
		}

		if(!GPIO_PinRead(GPIOA, PIN04)) // DECREMENTA EL CONTADOR i EN CADA PULSO DEL SW3 DE LA TARJETA
		{
					SDK_DelayAtLeastUs(DELAY, CORE_FREQ); // RETARDO PARA REDUCIR LOS REBOTES DEL SW
					--i;
		}


		if(i==2){ // SE HABILITA EL COLOR VERDE CUANDO SE HA PULSADO

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ); // RETARDO
			GPIO_PortSet(GPIOB, 1u << PIN21); // OFF
			GPIO_PortSet(GPIOB, 1u << PIN22); // OFF
			GPIO_PortClear(GPIOE, 1u << PIN26); // ON

		}
		if(i==4){ // SE HABILITA EL COLOR AZUL CUANDO SE HA PULSADO

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ); // RETARDO
			GPIO_PortClear(GPIOB, 1u << PIN21);	// ON
			GPIO_PortSet(GPIOB, 1u << PIN22);	// OFF
			GPIO_PortSet(GPIOE, 1u << PIN26);	// OFF


		}
		if(i==6){ // SE HABILITA EL COLOR MORADO CUANDO SE HA PULSADO

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ);  //COLOR MORADO
			GPIO_PortClear(GPIOB, 1u << PIN21);	// ON
			GPIO_PortClear(GPIOB, 1u << PIN22);	// ON
			GPIO_PortSet(GPIOE, 1u << PIN26);	// OFF

		}

		if(i==8){ // SE HABILITA EL COLOR ROJO CUANDO SE HA PULSADO

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ);  // RETARDO
			GPIO_PortSet(GPIOB, 1u << PIN21);	// OFF
			GPIO_PortClear(GPIOB, 1u << PIN22);	// ON
			GPIO_PortSet(GPIOE, 1u << PIN26);	// OFF

		}

		if(i==10){ // SE HABILITA EL COLOR AMARILLO CUANDO SE HA PULSADO

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ); // RETARDO
			GPIO_PortSet(GPIOB, 1u << PIN21);	// OFF
			GPIO_PortClear(GPIOB, 1u << PIN22);	// ON
			GPIO_PortClear(GPIOE, 1u << PIN26);	// ON

		}

		// SE HABILITA EL COLOR BLANCO CUANDO SE HA PULSADO LOS DOS PULSADORES AL MISMO TIEMPO

		if(!GPIO_PinRead(GPIOA, PIN04)&!GPIO_PinRead(GPIOC, PIN06)){

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ);	// RETARDO
			GPIO_PortClear(GPIOB, 1u << PIN21);	// ON
			GPIO_PortClear(GPIOB, 1u << PIN22);	// ON
			GPIO_PortClear(GPIOE, 1u << PIN26);	// ON
		}

		if(i>=12){	// SE REINICIA LA SECUENCIA CUANDO SE HA PULSADO MAS VECES LOS SW

			SDK_DelayAtLeastUs(DELAY, CORE_FREQ);	// RETARDO
			GPIO_PortSet(GPIOB, 1u << PIN21);	// OFF
			GPIO_PortSet(GPIOB, 1u << PIN22);	// OFF
			GPIO_PortClear(GPIOE, 1u << PIN26);	// ON

			i=0;

		}
	}

    return 0 ;
}

