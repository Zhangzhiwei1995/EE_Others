/***MODU+********************************************************************/
/* Copyright (c) 2009.04  WH, All Rights Reserved.                          */
/* FileName    : LCD12864.h                                                  */
/* Description : The Lcd1602 define                                         */
/* History     :                                                            */
/*     [Author]   [Date]      [Version]    [Description]                    */
/* [1] dragonhwz   2009/04/09  Ver 1.0.0    Initial file.                   */
/***MODU-********************************************************************/
//------------------------------------------------------------------------------------------------------
#ifndef __MSP430_TEST_LCD12864_H__
#define __MSP430_TEST_LCD12864_H__

#define uchar unsigned char
#define uint unsigned int    //宏定义


#define LCM_Data_Out  P4OUT 
#define LCM_Data_In   P4IN 
//P5.0管脚输出高低电平
#define SET_LCD_RS  do{P5OUT |= BIT0;}while(0)     // LCD_RS=1
#define CLR_LCD_RS  do{P5OUT &= ~(BIT0);}while(0)  // LCD_RS=0
//P5.1管脚输出高低电平
#define SET_LCD_RW  do{P5OUT |= BIT1;}while(0)     // LCD_RW=1
#define CLR_LCD_RW  do{P5OUT &= ~(BIT1);}while(0)  // LCD_RW=0
//P5.2管脚输出高低电平
#define SET_LCD_EN  do{P5OUT |= BIT2;}while(0)     // LCD_EN=1
#define CLR_LCD_EN  do{P5OUT &= ~(BIT2);}while(0)  // LCD_EN=0

//P5.3管脚输出高低电平
#define SET_LCD_PSB  do{P5OUT |= BIT3;}while(0)     // LCD_PSB=1
#define CLR_LCD_PSB  do{P5OUT &= ~(BIT3);}while(0)  // LCD_PSB=0

//P5.5管脚输出高低电平
#define SET_LCD_LED  do{P5OUT |= BIT5;}while(0)     // LCD_LED=1
#define CLR_LCD_LED  do{P5OUT &= ~(BIT5);}while(0)  // LCD_LED=0
//-----------------------------------------------------------------------------------
void delayms(uint t);
void WaitBusy(void);
void Lcd_WriteCmd(uchar cmdcode);
void Lcd_WriteData(uchar dispdata);
void Lcd_Init();
void hanzi_Disp(uchar x,uchar y,uchar  *s);
void pic_Disp(uchar *pic);
void LcdFill_Vertical();
void LcdFill_Level();
void Lcd_Fill(uchar test);

#endif