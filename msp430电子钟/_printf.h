/***MODU+********************************************************************/
/* Copyright (c) 2009.04  WH, All Rights Reserved.                          */
/* FileName    : Lcd1602.h                                                  */
/* Description : The LED define                                             */
/* History     :                                                            */
/*     [Author]   [Date]      [Version]    [Description]                    */
/* [1] dragonhwz   2009/04/09  Ver 1.0.0    Initial file.                   */
/***MODU-********************************************************************/
#ifndef __MSP430_TEST_PRINTF_H__
#define __MSP430_TEST_PRINTF_H__
void Init_UART1(void);
void Uart_Printf(const char *fmt,...);
void Uart_SendString(char *pt);
void Uart_SendByte(short sData);
void Uart_GetString(char *string);
short Uart_GetIntNum(void);
void Delay(short sTime);
char Uart_Getch(void);
#endif/* __MSP430_TEST_PRINTF_H__ */