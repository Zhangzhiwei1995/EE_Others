//******************************************************************************
//  MSP430P149 Demo - UART1, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM3,
//  USART1 RX interrupt triggers TX Echo.
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|--> TXD
//           |             P3.7|--> RXD
//           |                 |
//
//
//  Copyright (c) 2009.04  WH, All Rights Reserved.
//  Description : The UART1 module
//  History     :                                                           
//     [Author]   [Date]      [Version]    [Description]                     
//     [1] dragonhzw   2009/04/09  Ver 1.0.0    Initial file.
//
//  Built with IAR Assembler for MSP430V3.20A/W32 (3.20.1.9)
//******************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <msp430x14x.h>
#include "_printf.h"
/***FUNC+*********************************************************************/
/* Name   : Init_UART1                                                       */
/* Descrp : init Uart1 port.                                                 */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Init_UART1(void)
{
    //将寄存器的内容清零
    U1CTL = 0X00;   
    //数据位为8bit
    U1CTL += 0x10;  
    
    //将寄存器的内容清零
    U1TCTL = 0X00;  
    //波特率发生器选择SMCLK
    U1TCTL += SSEL1;      
    //波特率为115200
    UBR0_1 = 0X45;  
    UBR1_1 = 0X00;
    //调整寄存器
    UMCTL_1 = 0X00; 
    //使能UART1的TXD和RXD
    ME2 |= UTXE1 + URXE1; 
    //使能UART1的RX中断
    IE2 |= URXIE1;  
    //使能UART1的TX中断
    IE2 |= UTXIE1;  
    
    //设置P3.6为UART1的TXD
    P3SEL |= BIT6;  
    //设置P3.7为UART1的RXD
    P3SEL |= BIT7;  
    //P3.6为输出管脚
    P3DIR |= BIT6;  
    return;
}
/***FUNC+*********************************************************************/
/* Name   : UART1_RX_ISR                                                     */
/* Descrp : 处理来自串口 1 的发送中断                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_Printf(const char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
    return;
}
/***FUNC+*********************************************************************/
/* Name   : UART1_RX_ISR                                                     */
/* Descrp : 处理来自串口 1 的发送中断                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_SendString(char *pt)
{
    while(*pt)
    {
        Uart_SendByte(*pt++);
    }
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_SendByte                                                     */
/* Descrp : 处理来自串口 1 的发送中断                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_SendByte(short sData)
{
    if(sData=='\n')
    {
        while ((IFG2 & UTXIFG1) == 0); 
        Delay(10);                 //because the slow response of hyper_terminal 
        TXBUF1 ='\r';
    }
    while ((IFG2 & UTXIFG1) == 0);
    Delay(10);
    TXBUF1 = sData;
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_GetString                                                            */
/* Descrp : 延时                                                             */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_GetString(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte(c);
        }
    }
    *string='\0';
    Uart_SendByte('\n');
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_GetIntNum                                                   */
/* Descrp :                                                                  */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
short Uart_GetIntNum(void)
{
    char str[30];
    char *string = str;
    unsigned short base     = 10;
    unsigned short minus    = 0;
    short result   = 0;
    short lastIndex;    
    unsigned short i;
    
    Uart_GetString(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_Getch                                                       */
/* Descrp : 接收串口数据                                                     */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
char Uart_Getch(void)
{
  while (!(IFG2 & URXIFG1));
  return RXBUF1;
}
/***FUNC+*********************************************************************/
/* Name   : Delay                                                            */
/* Descrp : 延时                                                             */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Delay(short sTime)
{
    for(;sTime>0;sTime--)
    {
    }
    return; 
}