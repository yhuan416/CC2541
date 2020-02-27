/*
BLE4.0 CC2541 DEVICE CODE
蓝牙4.0 cc2541 AT指令驱动

作者 : YHUAN

源文件 :
	cc2541.c
	cc2541.h

----------------------------------------------------------------------------------

VERSION 
	V1.0 第一个版本
	DATE : 2020/2/28
		测试/重启/设置模式功能
		获取/设置模块名字
*/
#include <stdio.h>
#include <string.h>

#include "cc2541.h"

/* 宏函数 如果模块工作于透传模式 返回BLE_TRANSMITTING */
#define cc2541_check_status()  \
	if(CC2541_STATUS == 1) { return BLE_TRANSMITTING; }

/**
INIT DEVICE STATUS GPIO
*/
void cc2541_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	//初始化时钟线
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//初始化引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz翻转速度
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化PE6
	
	BLE_DEBUG("");
}

/**
CHECK DEVICE

RETURN VALUE：
	BLE_TRANSMITTING
	BLE_OK
	BLE_ERROR
*/
uint8_t cc2541_check(void) {
	uint16_t time = 0;
	
	//检查是否是透传模式
	cc2541_check_status();
	
	//发送检查指令
	CC2541_PRINTF("AT\r\n");
	
	//等待返回
	while(CC2541_RX_STA==0) 
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);
		
		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//超时处理
	}
	
	//清空标记位
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);
	
	//判断返回值
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
	
	//检查是否是透传模式
	cc2541_check_status();
	
	//发送重启指令
	CC2541_PRINTF("AT+RESET\r\n");
	
	//等待返回
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//超时处理
	}

	//清空标记位
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);

	//判断返回值
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
	
	//检查是否是透传模式
	cc2541_check_status();
	
	//发送重命名指令
	CC2541_PRINTF("AT+NAME%s\r\n", name);
	
	//等待返回
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//超时处理
	}
	
	//清空标记位
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);
	
	//判断返回值
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return cc2541_reset();//重启模块以生效
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

	//检查是否是透传模式
	cc2541_check_status();
	
	//发送重命名指令
	CC2541_PRINTF("AT+NAME\r\n");

	//等待返回
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//超时处理
	}
	
	//清空标记位
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
	
	//检查是否是透传模式
	cc2541_check_status();
	
	//修改主机模式指令
	CC2541_PRINTF("AT+ROLE%s\r\n", mode);
	
	//等待返回
	while(CC2541_RX_STA==0)
	{
		time+=CC2541_TICKS;
		CC2541_DELAY_MS(CC2541_TICKS);

		if(time >= CC2541_OUT_TIME) return BLE_TIMEOUT;//超时处理
	}
	
	//清空标记位
	CC2541_RX_CFLAG();

	BLE_DEBUG("%s", CC2541_RX_BUF);

	//判断返回值
	if(strstr((char *)CC2541_RX_BUF, "OK")) {
		return BLE_OK;//配置成功
	}
	
	return BLE_ERROR;
}
