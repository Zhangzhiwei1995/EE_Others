//******************************************************************************
//  MSP430P149 Demo - DS1302, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
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
//           |           P2.2  |<----CLOCK_SCLK 
//           |           P2.3  |<----CLOCK_DATA
//           |           P2.4  |<----CLOCK_CE
//           |                 |
//           |                 |
//
//  Copyright (c) 2009.04  WH, All Rights Reserved.
//  WebSite:www.smart-dz.cn
//  Description : The DS1302 module
//  History     :                                                           
//     [Author]   [Date]      [Version]    [Description]                     
//     [1] dragonhzw   2009/04/09  Ver 1.0.0    Initial file.
//
//  Built with IAR Assembler for MSP430V3.20A/W32 (3.20.1.9)
//******************************************************************************
#include <msp430x14x.h>
#include "DS1302.h"
#include "_printf.h"
/***FUNC+*********************************************************************/
/* Name   : DS1302_Delay                                                     */
/* Descrp : ��ʱ����                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_Delay(unsigned int dtime) 
{
    unsigned int temp;
    for(temp=0;temp<dtime;temp++);
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_Reset                                                     */
/* Descrp : DS1302��λ                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_Reset(void) 
{
    DS1302_DIR |= (DS1302_RST + DS1302_SCLK);
    DS1302_SCLK_LO;
    DS1302_RST_LO;
    DS1302_Delay(10);
    DS1302_SCLK_HI;
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_WriteOneByte                                              */
/* Descrp : ��DS1302д��һ���ֽ�                                             */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_WriteOneByte(unsigned char w_dat) {
    unsigned char temp;
    
    DS1302_RST_HI;
    DS1302_DIR |= DS1302_SDI;
    
    for(temp=8;temp>0;temp--) {
        DS1302_SDI_LO;
        if(w_dat&BIT0) DS1302_SDI_HI;
        DS1302_SCLK_LO;
        DS1302_Delay(10);
        DS1302_SCLK_HI;
        DS1302_Delay(10);
        w_dat >>=1;
    } 
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_ReadOneByte                                               */
/* Descrp : ��DS1302�ж�ȡһ���ֽ�                                           */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
unsigned char DS1302_ReadOneByte(void) 
{ 
    unsigned char temp,rdata;
    rdata = 0x00;
    
    DS1302_RST_HI;
    DS1302_DIR &= ~DS1302_SDI;
    
    for(temp=0;temp<7;temp++)
    {
        DS1302_SCLK_HI;
        DS1302_Delay(10);
        DS1302_SCLK_LO;
        DS1302_Delay(10);
        if((DS1302_IN&DS1302_SDI)==DS1302_SDI)
        rdata |= BIT7;
        rdata >>= 1;
    }
    
    return(rdata); 
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_WriteData                                                 */
/* Descrp : ��DS1302��д���ַ��д������                                     */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_WriteData(unsigned char addr,unsigned char w_dat) 
{
    DS1302_RST_LO;
    DS1302_SCLK_LO;
    DS1302_RST_HI;
    
    DS1302_WriteOneByte(addr);  //д���ַ
    DS1302_WriteOneByte(w_dat); //д������
    
    DS1302_SCLK_HI;
    DS1302_RST_LO;
} 
/***FUNC+*********************************************************************/
/* Name   : DS1302_ReadData                                                  */
/* Descrp : ��DS1302д���ַ��,��DS1302�ж�ȡ����                            */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
unsigned char DS1302_ReadData(unsigned char addr) 
{
    unsigned char r_dat;
    DS1302_RST_LO;
    DS1302_SCLK_LO;
    DS1302_RST_HI;
        
    DS1302_WriteOneByte(addr);  //д���ַ
    r_dat = DS1302_ReadOneByte();   //��������
    
    DS1302_SCLK_LO;
    DS1302_RST_LO;
        
    return(r_dat);
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_SettingData                                               */
/* Descrp : ����SettingData����������DS1302��ʱ��                            */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_SettingData(void) 
{
    unsigned char addr = 0x8C;       
    DS1302_WriteData(0x8E,0x00);    //д���������,����д����  
    
    DS1302_WriteData(addr,(g_sTime.m_ucYear/10)*16 + g_sTime.m_ucYear%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucWeek/10)*16 + g_sTime.m_ucWeek%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucMonth/10)*16 + g_sTime.m_ucMonth%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucDay/10)*16 + g_sTime.m_ucDay%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucHour/10)*16 + g_sTime.m_ucHour%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucMin/10)*16 + g_sTime.m_ucMin%10);
    addr -= 2;
    DS1302_WriteData(addr,(g_sTime.m_ucSec/10)*16 + g_sTime.m_ucSec%10);   
    DS1302_WriteData(0x8E,0x80);    //д���������,����д����
}
/***FUNC+*********************************************************************/
/* Name   : DS1302_GetData                                                   */
/* Descrp : ��ȡDS1302ʱ�䵽ReadingData��                                    */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DS1302_GetData(void) 
{
    unsigned char addr = 0x8D;
    unsigned char temp;
    temp = DS1302_ReadData(addr);//��
    g_sTime.m_ucYear= ((temp)/16)*10 + (temp)%16;
    addr -= 2;
    temp = DS1302_ReadData(addr);//����
    g_sTime.m_ucWeek = ((temp)/16)*10 + (temp)%16;
    addr -= 2;
    
    temp = DS1302_ReadData(addr);//��
    g_sTime.m_ucMonth= ((temp)/16)*10 + (temp)%16; 
    addr -= 2;
    temp = DS1302_ReadData(addr);//��
    g_sTime.m_ucDay = ((temp)/16)*10 + (temp)%16; 
    addr -= 2;
    temp = DS1302_ReadData(addr);//Сʱ
    g_sTime.m_ucHour = ((temp)/16)*10 + (temp)%16;
    addr -= 2;
    temp = DS1302_ReadData(addr);//��
    g_sTime.m_ucMin = ((temp)/16)*10 + (temp)%16;
    addr -= 2;
    temp = DS1302_ReadData(addr);//��
    g_sTime.m_ucSec = ((temp)/16)*10 + (temp)%16;
}

