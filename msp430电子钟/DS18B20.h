/***MODU+********************************************************************/
/* Copyright (c) 2009.04  WH, All Rights Reserved.                          */
/* FileName    : DS18B20.h                                                  */
/* Description : The Led define                                             */
/* History     :                                                            */
/*     [Author]   [Date]      [Version]    [Description]                    */
/* [1] Hunag zhengwu   2009/04/09  Ver 1.0.0    Initial file.               */
/***MODU-********************************************************************/
#ifndef __MSP430_TEST_DS18B20_H__
#define __MSP430_TEST_DS18B20_H__
//-----------------------------------------------------------------------------------------
#define TEMP_DQ_In     P2IN&BIT5
//P6.4管脚输出高低电平
#define SET_TEMP_DQ  do{P2OUT |= BIT5;}while(0)     // TEMP_DQ=1
#define CLR_TEMP_DQ  do{P2OUT &= ~(BIT5);}while(0)  // TEMP_DQ=0

//------------------------------------------------------------------------------------
void Delay_DS18B20(unsigned int num);
void Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
float ReadTemperature(void);
#endif/* __MSP430_TEST_DS18B20_H__ */

