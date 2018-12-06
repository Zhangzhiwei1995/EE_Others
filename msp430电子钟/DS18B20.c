//******************************************************************************
//  MSP430P149 Demo - DS18B20, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
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
//           |                 |
//           |           P2.5  |<----TEMP_DQ 
//           |                 |
//
//  Copyright (c) 2009.04  WH, All Rights Reserved.
//  WebSite:www.smart-dz.com
//  Description : The DS18B20 module
//  History     :                                                           
//     [Author]   [Date]      [Version]    [Description]                     
//     [1] dragonhzw   2009/04/09  Ver 1.0.0    Initial file.
//
//  Built with IAR Assembler for MSP430V3.20A/W32 (3.20.1.9)
//******************************************************************************
#include <msp430x14x.h>
#include "DS18B20.h"
//------------------------------------------------------------------------------------
/***FUNC+*********************************************************************/
/* Name   : Delay_DS18B20                                                    */
/* Descrp : 延时子程序                                                       */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Delay_DS18B20(unsigned int num)
{
 while(num--) ;
}
/***FUNC+*********************************************************************/
/* Name   : Init_DS18B20                                                     */
/* Descrp : 初始化DS18B20                                                    */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Init_DS18B20(void)
{
 unsigned char x=0;
 SET_TEMP_DQ;      //DQ= 1复位
 Delay_DS18B20(8);   //稍做延时
 CLR_TEMP_DQ;      //DQ = 0单片机将DQ拉低
 Delay_DS18B20(80);  //精确延时，大于480us
 SET_TEMP_DQ;      //DQ= 1拉高总线
 Delay_DS18B20(14);
 P2DIR &= ~(BIT5);
 x = TEMP_DQ_In;        //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
 P2DIR |= BIT5;
 Delay_DS18B20(20);
}
/***FUNC+*********************************************************************/
/* Name   : ReadOneChar                                                      */
/* Descrp : 读一个字节                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
unsigned char ReadOneChar(void)
{
 unsigned char i=0;
 unsigned char dat = 0;
 for (i=8;i>0;i--)
 {
  
  CLR_TEMP_DQ;  // DQ = 0给脉冲信号
  dat>>=1;
  SET_TEMP_DQ;  // DQ = 1给脉冲信号
  //设置为输入
  P2DIR &= ~(BIT5);
  
  //if(DQ)
  //dat|=0x80;
  if(TEMP_DQ_In)
  {
    dat|=0x80;
  } 
  //设置为输出
  P2DIR |= BIT5;
  Delay_DS18B20(4);
 }
 return(dat);
}
/***FUNC+*********************************************************************/
/* Name   : WriteOneChar                                                     */
/* Descrp : 写一个字节                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  CLR_TEMP_DQ;      // DQ = 0给脉冲信号
  //DQ = dat&0x01;
  if(dat&0x01)
  {
    SET_TEMP_DQ;    // DQ = 1给脉冲信号
  }
  else
  {
    CLR_TEMP_DQ;    // DQ = 0给脉冲信号
  }
  Delay_DS18B20(5);
  SET_TEMP_DQ;      // DQ = 0给脉冲信号
  dat>>=1;
 }
}
/***FUNC+*********************************************************************/
/* Name   : ReadTemperature                                                  */
/* Descrp : 读取温度                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
float ReadTemperature(void)
{
 unsigned char a=0;
 unsigned char b=0;
 unsigned int data = 0;
 float t=0;
 float tt=0;
 Init_DS18B20();
 WriteOneChar(0xCC);  //跳过读序号列号的操作
 WriteOneChar(0x44);  //启动温度转换
 Init_DS18B20();
 WriteOneChar(0xCC);  //跳过读序号列号的操作
 WriteOneChar(0xBE);  //读取温度寄存器
 a = ReadOneChar();   //读低8位
 b = ReadOneChar();  //读高8位
 data=b;
 data<<=8;
 t=data|a;
 tt=t*0.0625;
 t= tt*10+0.5;   //放大10倍输出并四舍五入
 return(t/10);
}