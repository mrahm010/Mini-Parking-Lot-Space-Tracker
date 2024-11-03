int main(void)
{
	//SPI Init
	PE_low_level_init();
	uint32_t delay;
	uint8_t ret, who;
	int8_t temp;
	int16_t accX, accY, accZ;
	int16_t magX, magY, magZ;
	int len;
	LDD_TDeviceData *SM1_DeviceData;
	SM1_DeviceData = SM1_Init(NULL);
	int lock = 0;
	FX1_Init();
	//-------------

	int val = 0;
	int openVal = 9;
	int enterSensor = 0;
	int exitSensor = 0;
	int full = 0;
	int empty = 0;

	int enterLock = 0;
	int exitLock = 0;


	unsigned int duty_cycle, tens;
	unsigned int time1, time2, time3, period, pulse_width;

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable Port B Clock Gate Control*/
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // Port C clock enable
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTC_PCR10 = 0x300; // Port C Pin 10 as FTM3_CH6 (ALT3)
	PORTC_GPCLR = 0x01BF0100; /*Configure PORTC Pins 0-5 and 7-8 as GPIO*/
	// Set prescale factor and clock source (FTMx_SC[4:0])
	GPIOC_PDDR = 0x000001BF; /*Configure PORTC Pins 0-5 and 7-8 as Output*/
	GPIOC_PDOR = numPortC[0];
	PORTB_PCR2 = 0x0100; /*Configure PORTB Pin 2 as GPIO*/
	PORTB_PCR3 = 0x0100; /*Configure PORTB Pin 3 as GPIO*/
	GPIOB_PDDR = 0x00000000; /*Configure PORTB Pin 2 and 3 as Input*/
	PORTD_PCR0 = 0x0100;
	GPIOD_PDDR = 0x000001BF;
	GPIOD_PDOR = 0x00;

	//measure period
	while (1) {
		software_delay(500000);
		enterSensor = GPIOB_PDIR & 0x04;
		exitSensor = GPIOB_PDIR & 0x08;
		if(openVal == 0) {
			GPIOD_PDOR = 0x01;
		}
		if(!(GPIOB_PDIR & 0x04)) {
			enterLock = 1;
			if(openVal > 0) {
				GPIOD_PDOR = 0x00; //added in by Mehran
				openVal = openVal - 1;
			}
			else if(!openVal) {
				full = 1;
				GPIOD_PDOR = 0x01;
				if (FX1_WhoAmI(&who)!=ERR_OK) {
					return ERR_FAILED;
				}
				if (!lock) {
					len = sprintf(write, "Parking lot is full.\n");
					SM1_SendBlock(SM1_DeviceData, &write, len);
					for(delay = 0; delay < 300000; delay++); //delay
					lock = 1;
				}
			}
		}
		else if (GPIOB_PDIR & 0x08) {
			exitLock = 1;
			if(openVal < 9) {
				GPIOD_PDOR = 0x00; //added in by Mehran
				openVal++;
				lock = 0;
			}
			else if(openVal == 9) {
				empty = 1;
			}
		}
		val = GPIOB_PDOR & 0x04;
		GPIOC_PDOR = numPortC[openVal];
	}

#ifdef PEX_RTOS_START
PEX_RTOS_START();
#endif
for(;;){}
return 0;
}
