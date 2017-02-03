
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_adc.h"
#include "inc/hw_types.h"
#include "inc/hw_udma.h"
#include "inc/hw_emac.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/udma.h"
#include "driverlib/emac.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/timer.h"

//UART
#include "inc/hw_uart.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include <string.h>

//SPI
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"

//I2C
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

//USB
#define EXECUTE_USB 1
#if EXECUTE_USB
	#include "usbDiskController.h"
	#define ADC_BUF_SIZE 512
	#define LED_GREEN 0x08
	uint32_t ADC_BUF[ADC_BUF_SIZE];
	int BUF_COUNT = 0;
	static int ADC_BUFFER_STORE_ENABLE = 0;
#endif

//FFT
#define EXECUTE_FFT 0
#if EXECUTE_FFT
	#include "fix_fft.h"
	static fft_vector fft;
#endif

//FIR FILTER
#define EXECUTE_FIR 1
#if EXECUTE_FIR
	#include "fir.h"
	static FIR_filter fir;
#endif

//SPI DAR
#define EXECUTE_DAC_OUTPUT 1
#if EXECUTE_DAC_OUTPUT
	#include "spi_dac.h"
	static spi_dac_config dac;
#endif


//----------------------------------------------------------------

//#define ADC_SAMPLE_BUF_SIZE 128
//uint16_t ADC_OUT[8];
#define ADC_SAMPLE_FREQUENCY 192000//50000
#define ADC0_SEQUENCER0   0
#define ADC_SAMPLE_BUF_SIZE 1
uint32_t ADC_OUT[ADC_SAMPLE_BUF_SIZE];

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void InitConsole(void) {
	// Enable GPIO port A which is used for UART0 pins.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Configure the pin muxing for UART0 functions on port A0 and A1.
	// This step is not necessary if your part does not support pin muxing.
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	// Enable UART0 so that we can configure the clock.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	// Use the internal 16MHz oscillator as the UART clock source.
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	// Select the alternate (UART) function for these pins.
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Initialize the UART for console I/O.
	UARTStdioConfig(0, 115200, 16000000);
}

void ADCInit(void) {
    //  ADC + GPIO (ADC Pins)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
    SysCtlDelay(10);

    ADCSequenceDisable(ADC0_BASE, 1);
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_IE | ADC_CTL_END);
    //ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    ADCSequenceEnable(ADC0_BASE, 1);
    ADCIntEnable(ADC0_BASE, 1);
    IntEnable(INT_ADC0SS1);

    //  Timer (ADC Trigger)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/ADC_SAMPLE_FREQUENCY - 1); // 10KHz
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
    TimerEnable(TIMER0_BASE, TIMER_A);

    IntEnable(INT_TIMER0A);
}

//static uint8_t toggle = 0xff;
void ADC0SS1IntHandler(void) {
	//TimerDisable(TIMER0_BASE, TIMER_A);
    // Ack Interrupt
    ADCIntClear(ADC0_BASE, 1);
    // Read ADC Data
    ADCSequenceDataGet(ADC0_BASE, 1, ADC_OUT);

    //Tolgo l'offset del circuito d'ingresso (2.5V = 2048)
    //ADC_OUT[0] = ADC_OUT[0] - 2048;

	//GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, toggle);
	//toggle = toggle == 0x00 ? 0xff : 0x00;
	//SysCtlDelay(80000);
    /*FIR FILTER*/
    #if EXECUTE_FIR
    fir.valid = 1;
    #endif

    /*FFT*/
    #if EXECUTE_FFT
    fftAddSample(&fft, ADC_OUT[0]);
    #endif

	#if EXECUTE_USB
    //Come da chiacchierata con Walter iniziamo a riempire un buffer di 512 campioni.
    if(ADC_BUFFER_STORE_ENABLE && BUF_COUNT < ADC_BUF_SIZE)
    {
        ADC_BUF[BUF_COUNT]=ADC_OUT[0];
        BUF_COUNT++;
    }
	#endif
    //TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


int newFile = 1;
void SW1_IntHandler(void) {
	if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {
		/*
		// Call the USB stack to keep it running.
		USBHCDMain();

		// PF4 was interrupt cause
		//UARTprintf("SW1_IntHandler\n");
		static char text[4+2] = "1234\n";
		text[0] = (ADC_OUT[0]/1000)%10 +'0';
		text[1] = (ADC_OUT[0]/100)%10 +'0';
		text[2] = (ADC_OUT[0]/10)%10 +'0';
		text[3] = ADC_OUT[0]%10 +'0';
		//sprintf(text,"%d\n", (int)ADC_OUT[0]);
		usbEnableCreateFile("samples.txt", text, sizeof(text), newFile);
		newFile = 0;
		*/
		#if EXECUTE_USB
        ADC_BUFFER_STORE_ENABLE = 1;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED_GREEN);
		#endif
	}
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
}
#if EXECUTE_USB
//Come da indicazione, iniziamo a leggere la funzione per il buffer
void BUF_Reader(void){
    int i = 0;
    newFile = 1;
    while(i < ADC_BUF_SIZE){
        // Call the USB stack to keep it running.
        USBHCDMain();

        // PF4 was interrupt cause
        //UARTprintf("SW1_IntHandler\n");
        static char text[4+2] = "1234\n";
        text[0] = (ADC_BUF[i]/1000)%10 +'0';
        text[1] = (ADC_BUF[i]/100)%10 +'0';
        text[2] = (ADC_BUF[i]/10)%10 +'0';
        text[3] = ADC_BUF[i]%10 +'0';

        //sprintf(text,"%d\n", (int)ADC_OUT[0]);
        usbEnableCreateFile("samples.txt", text, sizeof(text), newFile);
        usbDiskMainLoop();
        newFile = 0;
        ++i;
	}
    BUF_COUNT = 0;
    ADC_BUFFER_STORE_ENABLE = 0;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
}
#endif
void InitSW1(void) {
	//Abilito i led
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable port F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	// Pin F4 setup
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // Init PF4 as input
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4

	// Interrupt setup
	GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4); // Disable interrupt for PF4 (in case it was enabled)
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4); // Clear pending interrupts for PF4
	GPIOIntRegister(GPIO_PORTF_BASE, SW1_IntHandler); // Register our handler function for port F
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE); // Configure PF4 for falling edge trigger
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);     // Enable interrupt for PF4
}

static int sample;
int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	#if EXECUTE_FIR
	FIR_init(&fir);
	#endif

	#if EXECUTE_FFT
	fftInit(&fft);
	#endif

	#if EXECUTE_DAC_OUTPUT
	SpiDACInit(&dac);
	#endif

	InitConsole();
	ADCInit();

	#if EXECUTE_USB
	InitSW1();
	usbDiskInit();
	#endif

	IntMasterEnable();
	while(1)
	{
		#if EXECUTE_USB
		usbDiskMainLoop();
		if(BUF_COUNT == ADC_BUF_SIZE){
			BUF_Reader();
		}
		#endif

		#if EXECUTE_FIR
		if(fir.valid){
			FIR_put_sample(&fir, ADC_OUT[0]);
			sample = FIR_get_sample(&fir);
			fir.valid = 0;
			//UARTprintf("%d", fir.last_filtered_sample);
			//UARTprintf("\n");
		}
		#endif

		#if EXECUTE_FFT
		if(fft.valid){
			calculateFft(&fft);
			int i = 0;
			for ( i = 0; i < FFT_VECTOR_SIZE/2; i++) {
				UARTprintf("%d", fft.out[i]);
				UARTprintf("\n");
			}
			fft.valid = 0;
		}
		#endif

	    /*SPI DAC*/
		#if EXECUTE_DAC_OUTPUT
			#if EXECUTE_FIR
			SpiDACWrite(&dac, (uint16_t)sample);
			#else
	    		SpiDACWrite(&dac, (uint16_t)ADC_OUT[0]);
			#endif
		#endif
	}
}
