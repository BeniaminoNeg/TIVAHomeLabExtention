#include "spi_dac.h"

/* spi_dac.c - SPI Communication with MCP4921/MCP4922  */
/*
  All data are fixed-point int integers, in which -32768
  to +32768 represent -1.0 to +1.0 respectively. Integer
  arithmetic is used for speed, instead of the more natural
  floating-point.

  For the forward FFT (time -> freq), fixed scaling is
  performed to prevent arithmetic overflow, and to map a 0dB
  sine/cosine wave (i.e. amplitude = 32767) to two -6dB freq
  coefficients. The return value is always 0.

  For the inverse FFT (freq -> time), fixed scaling cannot be
  done, as two 0dB coefficients would sum to a peak amplitude
  of 64K, overflowing the 32k range of the fixed-point integers.
  Thus, the fix_fft() routine performs variable scaling, and
  returns a value which is the number of bits LEFT by which
  the output must be shifted to get the actual amplitude
  (i.e. if fix_fft() returns 3, each value of fr[] and fi[]
  must be multiplied by 8 (2**3) for proper scaling.
  Clearly, this cannot be done within fixed-point int
  integers. In practice, if the result is to be used as a
  filter, the scale_shift can usually be ignored, as the
  result will be approximately correctly normalized as is.

  Written by:  Tom Roberts  11/8/89
  Made portable:  Malcolm Slaney 12/15/94 malcolm@interval.com
  Enhanced:  Dimitrios P. Bouras  14 Jun 2006 dbouras@ieee.org
*/

static uint16_t SpiDACTxBuf[1];
void SSI0IntHandler(void){
    uint32_t ui32Status;

    // Read the interrupt status of the SPI.
    ui32Status = SSIIntStatus(SSI0_BASE, 1);//eval board

    // Clear any pending status, even though there should be none since no SPI
    // interrupts were enabled.  If SPI error interrupts were enabled, then
    // those interrupts could occur here and should be handled.  Since uDMA is
    // used for both the RX and TX, then neither of those interrupts should be
    // enabled. solo DMA-Int è enabled
    SSIIntClear(SSI0_BASE, ui32Status);
}
void InitSPI(void) {
	/* Abilita l'SPI a 1Mhz, 16bit
	 * Sui PIN:
	 * 			PA2 => SCK
	 * 			PA3 => CS/SS
	 * 			PA4 => RX (MiSo) (Non utilizzato)
	 * 			PA5 => Tx (MoSi)
	 */

	// SSI0 enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_SSI0);

	// GPIOA pins for SSI0 enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);
	GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_2);

	// SSI0 config
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 16);

	// SSI0 enable
	SSIEnable(SSI0_BASE);

	SSIDMAEnable(SSI0_BASE, SSI_DMA_TX);

	uDMAChannelAttributeDisable(UDMA_CHANNEL_SSI0TX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);
	uDMAChannelAttributeEnable(UDMA_CHANNEL_SSI0TX, UDMA_ATTR_USEBURST);
	uDMAChannelControlSet(UDMA_CHANNEL_SSI0TX | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_4);
	uDMAChannelTransferSet(UDMA_CHANNEL_SSI0TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, SpiDACTxBuf, (void *)(SSI0_BASE + SSI_O_DR), sizeof(SpiDACTxBuf));
	uDMAChannelAssign(UDMA_CH11_SSI0TX);
	uDMAChannelEnable(UDMA_CHANNEL_SSI0TX);

	// Enable the SPI peripheral interrupts.
	SSIIntEnable(SSI0_BASE, SSI_DMATX);
	IntEnable(INT_SSI0);
}

void data_transfer_dma(){
	//SPI
    uDMAChannelTransferSet(UDMA_CHANNEL_SSI0TX | UDMA_PRI_SELECT,
    						  UDMA_MODE_BASIC, SpiDACTxBuf,
					      (void *) (SSI0_BASE + SSI_O_DR),
						  sizeof(SpiDACTxBuf));
    uDMAChannelEnable(UDMA_CHANNEL_SSI0TX);
}

void SpiDACInit(spi_dac_config *dac)
{
	//Init struct
	dac->dac_selector = 0;
	dac->buffer = 0;
	dac->output_gain = 1;
	dac->output_power_control = 0;

	//Init SPI
	InitSPI();
	/*
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);
	GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_3|GPIO_PIN_2);

	//Il dac supporta al max 20Mhz 16bit
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 20000000, 16);
	SSIEnable(SSI0_BASE);
	*/
}
void SpiDACWrite(spi_dac_config *dac, uint16_t sample)
{
	uint16_t spiValue = 0x0000;
	spiValue += dac->output_power_control << 12;
	spiValue += dac->output_gain << 13;
	spiValue += dac->buffer << 14;
	spiValue += dac->dac_selector << 15;
	spiValue += sample;

	SpiDACTxBuf[0] = spiValue;
	data_transfer_dma();
	//SSIDataPut(SSI0_BASE, spiValue);
	//while(SSIBusy(SSI0_BASE)){}
}