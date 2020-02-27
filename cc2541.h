/*
BLE4.0 CC2541 DEVICE CODE
����4.0 cc2541 ATָ������

���� : YHUAN

Դ�ļ� :
	cc2541.c
	cc2541.h

----------------------------------------------------------------------------------

VERSION 
	V1.0 ��һ���汾
	DATE : 2020/2/28
		����/����/����ģʽ����
		��ȡ/����ģ������
*/
#ifndef _CC2541_H_
#define _CC2541_H_

/* ��ͷ�ļ� */
#include "sys.h"

/* delay function */
#include "delay.h"
#define CC2541_DELAY_MS		delay_ms
#define CC2541_DELAY_US		delay_us

/* out time */
#define CC2541_OUT_TIME		(500)
#define CC2541_TICKS		(10)

/* transmit port */
#include "uart2.h"
#define CC2541_RX_STA		(USART2_RX_STA&0x8000)
#define CC2541_RX_BUF		USART2_RX_BUF

#define CC2541_PRINTF		u2_printf
#define CC2541_RX_CFLAG()	{USART2_RX_STA=0;}

/* device mode gpio */
#define CC2541_STATUS		PEin(6)

/* DEBUG */
#define BLE_DEBUG			DEBUG

enum BLE_RETURN{
	BLE_OK = 0,//success
	BLE_TRANSMITTING,//transmitting
	BLE_TIMEOUT,//out time
	BLE_RESET_FAIL,//reset fail
	BLE_ERROR//config error
};

//�����豸ģʽ
enum BLE_MODE{
	BLE_SLAVE = 0,
	BLE_MASTER
};

/* INIT */
void cc2541_init(void);

uint8_t cc2541_check(void);
uint8_t cc2541_reset(void);

/* GET/SET NAME */
uint8_t cc2541_set_name(const char *name);
uint8_t cc2541_get_name(char *name);

/* SET MODE */
uint8_t cc2541_mode(enum BLE_MODE mode);

#endif
