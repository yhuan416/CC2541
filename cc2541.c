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
#include <stdio.h>
#include <string.h>

#include "cc2541.h"

/* �꺯�� ���ģ�鹤����͸��ģʽ ����BLE_TRANSMITTING */
#define cc2541_check_status()  \
	if(CC2541_STATUS == 1) { return BLE_TRANSMITTING; }

/**
INIT DEVICE STATUS GPIO
*/
void cc2541_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	//��ʼ��ʱ����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//��ʼ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz��ת�ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��PE6
	
	BLE_DEBUG("");
}

/**
CHECK DEVICE

RETURN VALUE��
	BLE_TRANSMITTING
	BLE_OK
	BLE_ERROR
*/
uint8_t cc2541_check(void) {
	uint16_t time = 0;
	
	//����Ƿ���͸��ģʽ
	cc2541_check_status();
	
	//���ͼ��ָ��
	CC2541_PRINTF("AT\r\n");
	
	//�ȴ�����
	while(CC2541_RX_STA==0) 
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);
		
		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//��ʱ����
	}
	
	//��ձ��λ
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);
	
	//�жϷ���ֵ
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return BLE_OK;
	}
	
	return BLE_ERROR;
}

/**
RESET DEVICE

RETURN VALUE:
	BLE_TRANSMITTING
	BLE_OK
	BLE_RESET_FAIL
*/
uint8_t cc2541_reset(void) {
	uint16_t time = 0;
	
	//����Ƿ���͸��ģʽ
	cc2541_check_status();
	
	//��������ָ��
	CC2541_PRINTF("AT+RESET\r\n");
	
	//�ȴ�����
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//��ʱ����
	}

	//��ձ��λ
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);

	//�жϷ���ֵ
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return BLE_OK;
	}
	
	return BLE_RESET_FAIL;
}

/**
SET DEVICE NAME
PARAM:
	name : new device name

RETURN VALUE:
	BLE_TRANSMITTING
	BLE_OK
	BLE_RESET_FAIL
	LE_ERROR
*/
uint8_t cc2541_set_name(const char *name) {
	uint16_t time = 0;
	
	//����Ƿ���͸��ģʽ
	cc2541_check_status();
	
	//����������ָ��
	CC2541_PRINTF("AT+NAME%s\r\n", name);
	
	//�ȴ�����
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//��ʱ����
	}
	
	//��ձ��λ
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);
	
	//�жϷ���ֵ
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return cc2541_reset();//����ģ������Ч
	}
	
	return BLE_ERROR;
}

/**
GET DEVICE NAME
PARAM:
	name : save device name

RETURN VALUE:
	BLE_TRANSMITTING
	BLE_OK
	BLE_ERROR
*/
uint8_t cc2541_get_name(char *name) {
	uint16_t time = 0;

	//����Ƿ���͸��ģʽ
	cc2541_check_status();
	
	//����������ָ��
	CC2541_PRINTF("AT+NAME\r\n");

	//�ȴ�����
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//��ʱ����
	}
	
	//��ձ��λ
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);
	
	if(strstr((char *)CC2541_RX_BUF, "+NAME=")) {
		strcpy(name, ((char *)CC2541_RX_BUF) + 6);
		return BLE_OK;
	}

	return BLE_ERROR;
}

/**
SET DEVICE MODE
PARAM:
	mode : new device mode
		BLE_SLAVE
		BLE_MASTER
RETURN VALUE:
	BLE_TRANSMITTING
	BLE_OK
	BLE_ERROR
*/
uint8_t cc2541_mode(enum BLE_MODE mode) {
	uint16_t time = 0;
	
	//����Ƿ���͸��ģʽ
	cc2541_check_status();
	
	//�޸�����ģʽָ��
	CC2541_PRINTF("AT+ROLE%s\r\n", mode);
	
	//�ȴ�����
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//��ʱ����
	}
	
	//��ձ��λ
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);

	//�жϷ���ֵ
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return BLE_OK;//���óɹ�
	}
	
	return BLE_ERROR;
}
