//******************************************************************************
//  MSP430P149 Demo - LCD12864, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
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
//           |           P5.0  |<----LCD_RS 
//           |           P5.1  |<----LCD_RW
//           |           P5.2  |<----LCD_EN
//           |           P5.3  |<----LCD_PSB
//           |           P5.5  |<----LCD_LED
//           |      P4.0~P4.7  |<----LCD_DATA
//           |                 |
//
//  Copyright (c) 2009.04  WH, All Rights Reserved.
//  WebSite:www.smart-dz.cn
//  Email:dragonhzw@163.com
//  Description : The LCD12864 module
//  History     :                                                           
//     [Author]   [Date]      [Version]    [Description]                     
//     [1] dragonhzw   2009/04/09  Ver 1.0.0    Initial file.
//
//  Built with IAR Assembler for MSP430V3.20A/W32 (3.20.1.9)
//******************************************************************************
#include <msp430x14x.h>
#include "LCD12864.h"
//#include "_printf.h"



/*****���ֵ�ַ��*****/
uchar  addr_tab[]=
{     //���ڸ��ݺ������������ַ
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//�����к���λ��
};
/***FUNC+*********************************************************************/
/* Name   : delayms                                                          */
/* Descrp : n��ms����ʱ�ӳ���                                                */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void delayms(uint t)     //Լ��ʱn��ms��
{
 uint i;
 while(t--)
 {
  for(i=0;i<125;i++);
 }        
}
/***FUNC+*********************************************************************/
/* Name   : WaitBusy                                                         */
/* Descrp : �ȴ�LCDæ                                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void WaitBusy(void)      //��ʱһС��ʱ�䣬�ȴ�LCD����
{
 uchar i=5;
    while(i--);
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_WriteCmd                                                     */
/* Descrp : дָ�����                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
/*****дָ�����*****/
void Lcd_WriteCmd(uchar cmdcode)
{
    CLR_LCD_RS; 
    CLR_LCD_RW; 
    SET_LCD_EN; 
    WaitBusy();
    LCM_Data_Out = cmdcode;
    CLR_LCD_EN; 
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_WriteData                                                    */
/* Descrp : д����                                                           */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Lcd_WriteData(uchar dispdata)
{
    SET_LCD_RS; 
    CLR_LCD_RW; 
    SET_LCD_EN; 
    WaitBusy();
    LCM_Data_Out = dispdata;
    CLR_LCD_EN; 
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_Init                                                         */
/* Descrp : Lcd Init                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Lcd_Init()
{ 
    CLR_LCD_LED; //LED on  
    SET_LCD_PSB; 
    delayms(50);
    Lcd_WriteCmd(0x30);        //ѡ�����ָ�
    delayms(1);
    Lcd_WriteCmd(0x30);          //ѡ��8bit������
    delayms(1);
    Lcd_WriteCmd(0x0c);          //����ʾ(���αꡢ������)
    delayms(1);
    Lcd_WriteCmd(0x01);          //�����ʾ�������趨��ַָ��Ϊ00H
    delayms(20);
}
/***FUNC+*********************************************************************/
/* Name   : hanzi_Disp                                                       */
/* Descrp : ��ʾ����                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void hanzi_Disp(uchar x,uchar y,uchar  *s)
{          //x��yΪ��������
    Lcd_WriteCmd(addr_tab[8*x+y]);  //д��ַ
    while(*s>0)
    { 
        Lcd_WriteData(*s);    //д����
        s++;     
    }
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Level                                                    */
/* Descrp : ��ͼ��ʾ�Ĳ���                                                   */
/*          1 �رջ�ͼ��ʾ����                                               */
/*          2 �Ƚ���ֱ������(Y)д���ͼRAM��ַ                               */
/*          3 �ٽ�ˮƽ��λԪ������(X)д���ͼRAM��ַ                         */
/*          4 ��D15��D8д��RAM��                                             */
/*          5 ��D7��D0д�뵽RAM��                                            */
/*          6 �򿪻�ͼ��ʾ����                                               */ 
/*          ͼƬȡģ��ʽ������ȡģ���ֽ�����                                 */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void pic_Disp(uchar *pic)    //��ʾGdram���ݣ���ʾͼƬ��
{
 uchar x,y,i;
 for(i=0;i<9;i=i+8)
 for(y=0;y<32;y++)
 {
  for(x=0;x<8;x++)
  {
   Lcd_WriteCmd(0x36);   //����ָ�����ͼ��ʾ
   Lcd_WriteCmd(0x80+y);  //�е�ַ
   Lcd_WriteCmd(0x80+x+i);  //�е�ַ
   Lcd_WriteData(*pic++);  //д���� D15��D8
   Lcd_WriteData(*pic++);  //д���� D7��D0
   Lcd_WriteCmd(0x30);
  }
 }
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Vertical                                                 */
/* Descrp : ������ʾ����                                                     */
/*           ��i=0ʱ��ʾ����128��32,��i=8ʱ��ʾ����128��32                   */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void LcdFill_Vertical()
{  
    uchar x,y,i;
    for(i=0;i<9;i=i+8)
    {
        for(y=0;y<0x20;y++)
        {     
            for(x=0;x<8;x++)
            { 
                Lcd_WriteCmd(0x36);   //����ָ�� ��ͼ��ʾ
                Lcd_WriteCmd(y+0x80);       //�е�ַ
                Lcd_WriteCmd(x+0x80+i);     //�е�ַ     
                Lcd_WriteData(0x55);   //д���� D15��D8 
                Lcd_WriteData(0x55);   //д���� D7��D0  
                Lcd_WriteCmd(0x30);
            }
        }
    } 
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Level                                                    */
/* Descrp : ������ʾ����                                                     */
/*           ��i=0ʱ��ʾ����128��32,��i=8ʱ��ʾ����128��32                   */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void LcdFill_Level()
{ 
    uchar x,y,i;
    uchar k;   
    for(i=0;i<9;i+=8)
    {
       for(y=0;y<0x20;y++)
       {     
            if((y%2)==1) 
            {
                k=0xff;
            }
            else 
            {
                k=0;
            }
            for(x=0;x<8;x++)
            {  
                Lcd_WriteCmd(0x36);   //����ָ�� ��ͼ��ʾ
                Lcd_WriteCmd(y+0x80); //�е�ַ
                Lcd_WriteCmd(x+0x80+i); //�е�ַ            
                Lcd_WriteData(k);
                Lcd_WriteData(k);
                Lcd_WriteCmd(0x30);
            }
        }  
    }
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_Fill                                                         */
/* Descrp : ������������򰵵�                                               */
/*           ��i=0ʱ��ʾ����128��32,��i=8ʱ��ʾ����128��32                   */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Lcd_Fill(uchar test)
{  
    uchar x,y,i;
    for(i=0;i<9;i=i+8)
    {
        for(y=0;y<0x20;y++)
        {     
            for(x=0;x<8;x++)
            { 
                Lcd_WriteCmd(0x36);   //����ָ�� ��ͼ��ʾ
                Lcd_WriteCmd(y+0x80);       //�е�ַ
                Lcd_WriteCmd(x+0x80+i);     //�е�ַ     
                Lcd_WriteData(test);   //д���� D15��D8 
                Lcd_WriteData(test);   //д���� D7��D0  
                Lcd_WriteCmd(0x30);
            }
       } 
    }
}
