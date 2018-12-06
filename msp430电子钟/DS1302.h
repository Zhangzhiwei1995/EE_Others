/***MODU+********************************************************************/
/* Copyright (c) 2009.04  WH, All Rights Reserved.                          */
/* FileName    : Uart.h                                                     */
/* Description : The Uart define                                            */
/* History     :                                                            */
/*     [Author]   [Date]      [Version]    [Description]                    */
/* [1] Hunag zhengwu   2009/04/09  Ver 1.0.0    Initial file.               */
/***MODU-********************************************************************/

#ifndef __MSP430_TEST_DS1302_H__
#define __MSP430_TEST_DS1302_H__
//-----------------------------------------------------------------------------------------------------------------------------------
#define DS1302_DIR      P2DIR
#define DS1302_IN       P2IN
#define DS1302_OUT      P2OUT


#define DS1302_SCLK     BIT2
#define DS1302_SDI      BIT3    //定义MSP320的端口
#define DS1302_RST      BIT4

#define DS1302_RST_LO       DS1302_OUT &= ~DS1302_RST
#define DS1302_RST_HI       DS1302_OUT |= DS1302_RST
#define DS1302_SCLK_LO      DS1302_OUT &= ~DS1302_SCLK
#define DS1302_SCLK_HI      DS1302_OUT |= DS1302_SCLK
#define DS1302_SDI_LO       DS1302_OUT &= ~DS1302_SDI
#define DS1302_SDI_HI       DS1302_OUT |= DS1302_SDI
//------------------------------------------------------------------------------------------------------------------------------------
//Time Value
typedef struct tagTimeValue
{
    unsigned char  m_ucYear;     /*year*/
    unsigned char  m_ucWeek;     /*week*/
    unsigned char  m_ucMonth;    /*month*/
    unsigned char  m_ucDay;      /*Day*/
    unsigned char  m_ucHour;     /*hour*/
    unsigned char  m_ucMin;      /*min*/
    unsigned char  m_ucSec;      /*Second*/
}stTime,*PstTime;
extern stTime g_sTime;
//------------------------------------------------------------------------------------------------------------------------------------
void DS1302_Delay(unsigned int dtime);
void DS1302_Reset(void);
void DS1302_WriteOneByte(unsigned char w_dat);
void DS1302_WriteData(unsigned char addr,unsigned char w_dat);
void DS1302_SettingData(void);
void DS1302_GetData(void);
unsigned char DS1302_ReadOneByte(void);
unsigned char DS1302_ReadData(unsigned char addr);
#endif 