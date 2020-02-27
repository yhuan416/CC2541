BLE4.0 CC2541 DEVICE CODE

AUTHOR : YHUAN
DATE : 2020/2/28

source :
	cc2541.c
	cc2541.h

VERSION V1.0
	first version

HOW TO USE:

CONFIG IN cc2541.h

	1.define your own delay function
		#define CC2541_DELAY_MS		delay_ms
		#define CC2541_DELAY_US		delay_us

	2.set out time and delay tick

		#define CC2541_OUT_TIME		(500)
		#define CC2541_TICKS		(10)

	3.set your transmit port
		#define CC2541_RX_STA		(USART2_RX_STA&0x8000) 
			uart recv data end flag.
			if (CC2541_RX_STA==1) means one data frame recv end.
		#define CC2541_RX_BUF		USART2_RX_BUF
			uart recv buf address.
			after you recv end please add '\0' after your data

		#define CC2541_PRINTF		u2_printf
			uart send string function.
			printf type
		#define CC2541_RX_CFLAG()	{USART2_RX_STA=0;}
			clear recv data flag.
			call this function after recv each data frame
	
	4.define device mode gpio
		#define CC2541_STATUS		PEin(6)
			this port is HIGH level with cc2541 is transmitting or LOW level with AT config mode
		
	5.define DEBUG function(not necessary)
		#define BLE_DEBUG			DEBUG
			if you DEFINED '__DEBUG', DEBUG function has called

FUNCTION RETURN VALUE
	enum BLE_RETURN{
		BLE_OK = 0,//success
		BLE_TRANSMITTING,//transmitting
		BLE_TIMEOUT,//out time
		BLE_RESET_FAIL,//reset fail
		BLE_ERROR//config error
	};
	
CALL FUNCTION
	1./* init your uart with your own function */
	2.you can EDIT cc2541_init() to init this GPIO or use your own function;
	3.use function to set&get device config
