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
/* Descrp : ��ʱ�ӳ���                                                       */
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
/* Descrp : ��ʼ��DS18B20                                                    */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Init_DS18B20(void)
{
 unsigned char x=0;
 SET_TEMP_DQ;      //DQ= 1��λ
 Delay_DS18B20(8);   //������ʱ
 CLR_TEMP_DQ;      //DQ = 0��Ƭ����DQ����
 Delay_DS18B20(80);  //��ȷ��ʱ������480us
 SET_TEMP_DQ;      //DQ= 1��������
 Delay_DS18B20(14);
 P2DIR &= ~(BIT5);
 x = TEMP_DQ_In;        //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
 P2DIR |= BIT5;
 Delay_DS18B20(20);
}
/***FUNC+*********************************************************************/
/* Name   : ReadOneChar                                                      */
/* Descrp : ��һ���ֽ�                                                       */
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
  
  CLR_TEMP_DQ;  // DQ = 0�������ź�
  dat>>=1;
  SET_TEMP_DQ;  // DQ = 1�������ź�
  //����Ϊ����
  P2DIR &= ~(BIT5);
  
  //if(DQ)
  //dat|=0x80;
  if(TEMP_DQ_In)
  {
    dat|=0x80;
  } 
  //����Ϊ���
  P2DIR |= BIT5;
  Delay_DS18B20(4);
 }
 return(dat);
}
/***FUNC+*********************************************************************/
/* Name   : WriteOneChar                                                     */
/* Descrp : дһ���ֽ�                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  CLR_TEMP_DQ;      // DQ = 0�������ź�
  //DQ = dat&0x01;
  if(dat&0x01)
  {
    SET_TEMP_DQ;    // DQ = 1�������ź�
  }
  else
  {
    CLR_TEMP_DQ;    // DQ = 0�������ź�
  }
  Delay_DS18B20(5);
  SET_TEMP_DQ;      // DQ = 0�������ź�
  dat>>=1;
 }
}
/***FUNC+*********************************************************************/
/* Name   : ReadTemperature                                                  */
/* Descrp : ��ȡ�¶�                                                         */
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
 WriteOneChar(0xCC);  //����������кŵĲ���
 WriteOneChar(0x44);  //�����¶�ת��
 Init_DS18B20();
 WriteOneChar(0xCC);  //����������кŵĲ���
 WriteOneChar(0xBE);  //��ȡ�¶ȼĴ���
 a = ReadOneChar();   //����8λ
 b = ReadOneChar();  //����8λ
 data=b;
 data<<=8;
 t=data|a;
 tt=t*0.0625;
 t= tt*10+0.5;   //�Ŵ�10���������������
 return(t/10);
}