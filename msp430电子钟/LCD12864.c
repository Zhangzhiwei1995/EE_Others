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



/*****汉字地址表*****/
uchar  addr_tab[]=
{     //便于根据汉字坐标求出地址
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//第四行汉字位置
};
/***FUNC+*********************************************************************/
/* Name   : delayms                                                          */
/* Descrp : n（ms）延时子程序                                                */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void delayms(uint t)     //约延时n（ms）
{
 uint i;
 while(t--)
 {
  for(i=0;i<125;i++);
 }        
}
/***FUNC+*********************************************************************/
/* Name   : WaitBusy                                                         */
/* Descrp : 等待LCD忙                                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void WaitBusy(void)      //延时一小段时间，等待LCD空闲
{
 uchar i=5;
    while(i--);
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_WriteCmd                                                     */
/* Descrp : 写指令代码                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
/*****写指令代码*****/
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
/* Descrp : 写数据                                                           */
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
    Lcd_WriteCmd(0x30);        //选择基本指令集
    delayms(1);
    Lcd_WriteCmd(0x30);          //选择8bit数据流
    delayms(1);
    Lcd_WriteCmd(0x0c);          //开显示(无游标、不反白)
    delayms(1);
    Lcd_WriteCmd(0x01);          //清除显示，并且设定地址指针为00H
    delayms(20);
}
/***FUNC+*********************************************************************/
/* Name   : hanzi_Disp                                                       */
/* Descrp : 显示汉字                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void hanzi_Disp(uchar x,uchar y,uchar  *s)
{          //x、y为汉字坐标
    Lcd_WriteCmd(addr_tab[8*x+y]);  //写地址
    while(*s>0)
    { 
        Lcd_WriteData(*s);    //写数据
        s++;     
    }
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Level                                                    */
/* Descrp : 绘图显示的步骤                                                   */
/*          1 关闭绘图显示功能                                               */
/*          2 先将垂直的坐标(Y)写入绘图RAM地址                               */
/*          3 再将水平的位元组坐标(X)写入绘图RAM地址                         */
/*          4 将D15－D8写入RAM中                                             */
/*          5 将D7－D0写入到RAM中                                            */
/*          6 打开绘图显示功能                                               */ 
/*          图片取模方式：横向取模，字节正序                                 */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void pic_Disp(uchar *pic)    //显示Gdram内容（显示图片）
{
 uchar x,y,i;
 for(i=0;i<9;i=i+8)
 for(y=0;y<32;y++)
 {
  for(x=0;x<8;x++)
  {
   Lcd_WriteCmd(0x36);   //扩充指令，开绘图显示
   Lcd_WriteCmd(0x80+y);  //行地址
   Lcd_WriteCmd(0x80+x+i);  //列地址
   Lcd_WriteData(*pic++);  //写数据 D15－D8
   Lcd_WriteData(*pic++);  //写数据 D7－D0
   Lcd_WriteCmd(0x30);
  }
 }
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Vertical                                                 */
/* Descrp : 整屏显示竖条                                                     */
/*           当i=0时显示上面128×32,当i=8时显示下面128×32                   */
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
                Lcd_WriteCmd(0x36);   //扩充指令 绘图显示
                Lcd_WriteCmd(y+0x80);       //行地址
                Lcd_WriteCmd(x+0x80+i);     //列地址     
                Lcd_WriteData(0x55);   //写数据 D15－D8 
                Lcd_WriteData(0x55);   //写数据 D7－D0  
                Lcd_WriteCmd(0x30);
            }
        }
    } 
}
/***FUNC+*********************************************************************/
/* Name   : LcdFill_Level                                                    */
/* Descrp : 整屏显示横条                                                     */
/*           当i=0时显示上面128×32,当i=8时显示下面128×32                   */
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
                Lcd_WriteCmd(0x36);   //扩充指令 绘图显示
                Lcd_WriteCmd(y+0x80); //行地址
                Lcd_WriteCmd(x+0x80+i); //列地址            
                Lcd_WriteData(k);
                Lcd_WriteData(k);
                Lcd_WriteCmd(0x30);
            }
        }  
    }
}
/***FUNC+*********************************************************************/
/* Name   : Lcd_Fill                                                         */
/* Descrp : 整屏充满亮点或暗点                                               */
/*           当i=0时显示上面128×32,当i=8时显示下面128×32                   */
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
                Lcd_WriteCmd(0x36);   //扩充指令 绘图显示
                Lcd_WriteCmd(y+0x80);       //行地址
                Lcd_WriteCmd(x+0x80+i);     //列地址     
                Lcd_WriteData(test);   //写数据 D15－D8 
                Lcd_WriteData(test);   //写数据 D7－D0  
                Lcd_WriteCmd(0x30);
            }
       } 
    }
}
