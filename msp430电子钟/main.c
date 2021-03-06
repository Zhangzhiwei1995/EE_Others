#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <msp430x14x.h>
#include "LCD12864.h"
#include "DS18B20.h"
#include "DS1302.h"
//---------------------------------------------------------------------------
stTime g_sTime;
static unsigned char w = 0; //调时标志位
static unsigned char key;
//-------------------------------------------------------------------------------
unsigned char SettingData[7]={  //bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
    0x09,               //|-------个位--------|年份(当前09年)
    0x03,               //|-------个位--------|星期(当前周日)
    0x08,               //|-------个位--------|月份(当前07月)
    0x13,               //|-------个位--------|日期(当前01号)
    0x05,               //|-------个位--------|小时(当前18点)
    0x20,               //|-------个位--------|分钟(当前20分)
    0x30                //|-------个位--------|秒钟(当前30秒)
};
/***FUNC+*********************************************************************/
/* Name   : Delay1ms                                                         */
/* Descrp : delay time                                                       */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Delay1ms(unsigned int count)
{
   unsigned int i,j;
    for(i = 0;i<count;i++)
    {
        for(j=0;j<120;j++)
        {
            ;
        }

    }
}
/***FUNC+*********************************************************************/
/* Name   : days                                                             */
/* Descrp :                                                                  */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void days (viod) 
{   
    
        if ( g_sTime.m_ucMonth== 0x01 && g_sTime.m_ucDay== 0x01 ){ hanzi_Disp(3,0,"Happy New Year!!"); }//1月
        if ( g_sTime.m_ucMonth == 0x01 && g_sTime.m_ucDay == 0x28 ){ hanzi_Disp(3,0,"  世界麻风日    "); }
    
        if ( g_sTime.m_ucMonth == 0x02 && g_sTime.m_ucDay == 0x02 ){ hanzi_Disp(3,0,"  世界湿地日    "); }//2月
        if ( g_sTime.m_ucMonth == 0x02 && g_sTime.m_ucDay == 0x13 ){ hanzi_Disp(3,0,"  明天情人节了  "); }  
        if ( g_sTime.m_ucMonth == 0x02 && g_sTime.m_ucDay == 0x14 ){ hanzi_Disp(3,0,"  今天是情人节  "); }
            
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  国际海豹日    "); }//3月 
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x03 ){ hanzi_Disp(3,0,"  全国爱耳日    "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x08 ){ hanzi_Disp(3,0,"   3.8妇女节    "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x12 ){ hanzi_Disp(3,0,"    植树节      "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x14 ){ hanzi_Disp(3,0,"  国际警察日    "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x15 ){ hanzi_Disp(3,0,"  消费者权益日  "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x17 ){ hanzi_Disp(3,0,"  国际航海日    "); }      
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x21 ){ hanzi_Disp(3,0,"  世界森林日    "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x22 ){ hanzi_Disp(3,0,"    世界水日    "); }      
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x23 ){ hanzi_Disp(3,0,"  世界气象日    "); }  
        if ( g_sTime.m_ucMonth == 0x03 && g_sTime.m_ucDay == 0x24 ){ hanzi_Disp(3,0,"世界防治结核病日"); }
                
        if ( g_sTime.m_ucMonth == 0x04 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"愚人节  小心上当"); }//4       
        if ( g_sTime.m_ucMonth == 0x04 && g_sTime.m_ucDay == 0x07 ){ hanzi_Disp(3,0,"  世界卫生日    "); }  
        if ( g_sTime.m_ucMonth == 0x04 && g_sTime.m_ucDay == 0x08 ){ hanzi_Disp(3,0,"    复活节      "); }      
        if ( g_sTime.m_ucMonth == 0x04 && g_sTime.m_ucDay == 0x13 ){ hanzi_Disp(3,0,"  黑色星期五    "); }
        
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  劳动节  放假  "); }//5   
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x04 ){ hanzi_Disp(3,0,"    青年节      "); }      
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x08 ){ hanzi_Disp(3,0,"  世界红十字日  "); }      
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x12 ){ hanzi_Disp(3,0,"  国际护士节    "); }      
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x05 ){ hanzi_Disp(3,0,"近日注意母亲节  "); }      
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x15 ){ hanzi_Disp(3,0,"  国际家庭日    "); }
        if ( g_sTime.m_ucMonth == 0x05 && g_sTime.m_ucDay == 0x31 ){ hanzi_Disp(3,0,"  世界无烟日    "); }      
        
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  国际儿童节    "); }//6
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x05 ){ hanzi_Disp(3,0,"  世界环境日    "); }      
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x26 ){ hanzi_Disp(3,0,"  国际禁毒日    "); }      
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x06 ){ hanzi_Disp(3,0,"  全国爱眼日    "); }  
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x13 ){ hanzi_Disp(3,0,"近日注意父亲节  "); }
        if ( g_sTime.m_ucMonth == 0x06 && g_sTime.m_ucDay == 0x15 ){ hanzi_Disp(3,0,"近日注意父亲节  "); }
    
        if ( g_sTime.m_ucMonth == 0x07 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"香港回归记念日  "); }//7
        if ( g_sTime.m_ucMonth == 0x07 && g_sTime.m_ucDay == 0x07 ){ hanzi_Disp(3,0,"抗日战争记念日  "); }
        if ( g_sTime.m_ucMonth == 0x07 && g_sTime.m_ucDay == 0x11 ){ hanzi_Disp(3,0,"  世界人口日    "); }

        if ( g_sTime.m_ucMonth == 0x08 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  八一建军节    "); }//8
        if ( g_sTime.m_ucMonth == 0x08 && g_sTime.m_ucDay == 0x08 ){ hanzi_Disp(3,0,"  中国男子节    "); }
        if ( g_sTime.m_ucMonth == 0x08 && g_sTime.m_ucDay == 0x15 ){ hanzi_Disp(3,0,"抗战胜利记念日  "); }

        if ( g_sTime.m_ucMonth == 0x09 && g_sTime.m_ucDay == 0x10 ){ hanzi_Disp(3,0,"  中国教师节    "); }//9
        if ( g_sTime.m_ucMonth == 0x09 && g_sTime.m_ucDay == 0x18 ){ hanzi_Disp(3,0,"九·一八事变记念"); }
        if ( g_sTime.m_ucMonth == 0x09 && g_sTime.m_ucDay == 0x20 ){ hanzi_Disp(3,0,"  国际爱牙日    "); }
        if ( g_sTime.m_ucMonth == 0x09 && g_sTime.m_ucDay == 0x27 ){ hanzi_Disp(3,0,"  世界旅游日    "); }

        if ( g_sTime.m_ucMonth == 0x10 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  中国国庆节    "); }//10
        if ( g_sTime.m_ucMonth == 0x10 && g_sTime.m_ucDay == 0x04 ){ hanzi_Disp(3,0,"  世界动物日    "); }
        if ( g_sTime.m_ucMonth == 0x10 && g_sTime.m_ucDay == 0x24 ){ hanzi_Disp(3,0,"    联合国日    "); }
        if ( g_sTime.m_ucMonth == 0x10 && g_sTime.m_ucDay == 0x12 ){ hanzi_Disp(3,0,"明天国际教师节  "); }
        if ( g_sTime.m_ucMonth == 0x10 && g_sTime.m_ucDay == 0x13 ){ hanzi_Disp(3,0,"  国际教师节    "); }

        if ( g_sTime.m_ucMonth == 0x11 && g_sTime.m_ucDay == 0x10 ){ hanzi_Disp(3,0,"  世界青年节    "); }//11
        if ( g_sTime.m_ucMonth == 0x11 && g_sTime.m_ucDay == 0x17 ){ hanzi_Disp(3,0,"  世界学生节    "); }

        if ( g_sTime.m_ucMonth == 0x12 && g_sTime.m_ucDay == 0x01 ){ hanzi_Disp(3,0,"  世界艾滋病日  "); }//12
        if ( g_sTime.m_ucMonth == 0x12 && g_sTime.m_ucDay == 0x23 ){ hanzi_Disp(3,0,"  明晚平安夜    "); }
        if ( g_sTime.m_ucMonth == 0x12 && g_sTime.m_ucDay == 0x24 ){ hanzi_Disp(3,0,"  今晚平安夜    "); }
        if ( g_sTime.m_ucMonth == 0x12 && g_sTime.m_ucDay == 0x25 ){ hanzi_Disp(3,0,"    圣诞快乐    "); }
        if ( g_sTime.m_ucMonth == 0x12 && g_sTime.m_ucDay == 0x31 ){ hanzi_Disp(3,0,"    明日新年    "); }

        else
        { 
            if ( g_sTime.m_ucHour >= 4 && g_sTime.m_ucHour <  6 ){ hanzi_Disp(3,0,"★★__▲▲__凌晨"); }
            if ( g_sTime.m_ucHour >= 6 && g_sTime.m_ucHour <  8 ){ hanzi_Disp(3,0,"☆○__▲△__早晨"); }
            if ( g_sTime.m_ucHour >= 8 && g_sTime.m_ucHour <  12 ){ hanzi_Disp(3,0,"__●__▲▲__上午"); }
            if ( g_sTime.m_ucHour == 12)                             { hanzi_Disp(3,0,"____▲●▲__中午"); }
            if ( g_sTime.m_ucHour >= 13 && g_sTime.m_ucHour <  18 ){ hanzi_Disp(3,0,"__▲▲__●__下午"); }
            if ( g_sTime.m_ucHour >= 18 && g_sTime.m_ucHour <  22 ){ hanzi_Disp(3,0,"△▲__●☆__晚上"); }
            if ( g_sTime.m_ucHour >= 22 && g_sTime.m_ucHour <= 23 ){ hanzi_Disp(3,0,"△▲__★☆__夜里"); }
            if ( g_sTime.m_ucHour >= 0 && g_sTime.m_ucHour <  4 ){ hanzi_Disp(3,0,"__★▲▲★__深夜"); }
        }



}

void updata(void)
{
    char Buff[32];
    unsigned char sec;
    float temp;
    char sel;
    sec = DS1302_ReadData(0x81);
    if( ((sec)/16)*10 + (sec)%16!= g_sTime.m_ucSec)
    {
//        Lcd_Init();
        DS1302_GetData();
        sprintf(Buff,"20%02d",g_sTime.m_ucYear);
        hanzi_Disp(0,3,(unsigned char*)Buff);
        sel = (g_sTime.m_ucWeek)%16; //字节低4位的BCD码放入sel
        if(sel==7)  {hanzi_Disp(1,7,"日");}    //
        if(sel==6)  {hanzi_Disp(1,7,"六");}    //
        if(sel==5)  {hanzi_Disp(1,7,"五");}    //
        if(sel==4)  {hanzi_Disp(1,7,"四");}    //
        if(sel==3)  {hanzi_Disp(1,7,"三");} //此指令等于lcm_w_word("三"); 因为“三”的内码失效。
        if(sel==2)  {hanzi_Disp(1,7,"二");}    //
        if(sel==1)  {hanzi_Disp(1,7,"一");}    //星期一
        hanzi_Disp(1,5,"星期");
        sprintf(Buff,"%02d月%02d日  ",g_sTime.m_ucMonth,g_sTime.m_ucDay);
        hanzi_Disp(1,0,(unsigned char*)Buff);
        temp = ReadTemperature();
        sprintf(Buff,"%02d:%02d:%02d  %.1f℃",g_sTime.m_ucHour,g_sTime.m_ucMin,g_sTime.m_ucSec,temp);
        hanzi_Disp(2,0,(unsigned char*)Buff);
        
    }
}
/***FUNC+*********************************************************************/
/* Name   : days                                                             */
/* Descrp :                                                                  */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void W_Menu1(void)
{
    hanzi_Disp(0,0,"[Dis] ");
    hanzi_Disp(0,5," [Set]");
    updata();
    days(); 
}
/***FUNC+*********************************************************************/
/* Name   : Display                                                          */
/* Descrp :                                                                  */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Display(unsigned char item, unsigned char addr)
{
    char sel;
    char Buff[32];
    switch(addr)
    {
        //display sec
        case 0x80:
          sprintf(Buff,"%02d",item);
          hanzi_Disp(2,3,(unsigned char*)Buff); 
        break;
        
        //display min
        case 0x82:
          sprintf(Buff,":%02d",item);
          hanzi_Disp(2,1,(unsigned char*)Buff); 
        break;
        //display hour
        case 0x84:
          sprintf(Buff,"%02d",item);
          hanzi_Disp(2,0,(unsigned char*)Buff);   
        break;
        //display week
        case 0x8a:
         sel = (item)%16; //字节低4位的BCD码放入sel
        if(sel==7)  {hanzi_Disp(1,7,"日");}    //
        if(sel==6)  {hanzi_Disp(1,7,"六");}    //
        if(sel==5)  {hanzi_Disp(1,7,"五");}    //
        if(sel==4)  {hanzi_Disp(1,7,"四");}    //
        if(sel==3)  {hanzi_Disp(1,7,"三");} //此指令等于lcm_w_word("三"); 因为“三”的内码失效。
        if(sel==2)  {hanzi_Disp(1,7,"二");}    //
        if(sel==1)  {hanzi_Disp(1,7,"一");}    //星期一
        hanzi_Disp(1,5,"星期");
        break;
        //display day
        case 0x86:
         sprintf(Buff,"%02d",item);
         hanzi_Disp(1,2,(unsigned char*)Buff);  
        break;
        //display moth
        case 0x88:
        sprintf(Buff,"%02d",item);
        hanzi_Disp(1,0,(unsigned char*)Buff); 
        break;
        //display year
        case 0x8c:        
         sprintf(Buff,"20%02d",item);
         hanzi_Disp(0,3,(unsigned char*)Buff);         
        break;
        
        default:
        
            break;
    }
    
}
/***FUNC+*********************************************************************/
/* Name   : Set_time                                                         */
/* Descrp :                                                                  */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Set_time(unsigned char sel)
{
    //根据选择调整的相应项目加1并写入DS1302
  unsigned char address,item;
  unsigned char max,mini;
  DS1302_GetData();
  if(sel==6)  {hanzi_Disp(3,2,"调整秒钟");address=0x80; max=59;mini=0;item = g_sTime.m_ucSec;}     //秒7
  if(sel==5)  {hanzi_Disp(3,2,"调整分钟");address=0x82; max=59;mini=0;item = g_sTime.m_ucMin;}    //分钟6
  if(sel==4)  {hanzi_Disp(3,2,"调整小时");address=0x84; max=23;mini=0;item = g_sTime.m_ucHour;}    //小时5
  if(sel==3)  {hanzi_Disp(3,2,"调整星期");address=0x8a; max=7;mini=1;item = g_sTime.m_ucWeek;}    //星期4  
  if(sel==2)  {hanzi_Disp(3,2,"调整日期");address=0x86; max=31;mini=1;item = g_sTime.m_ucDay;}    //日3
  if(sel==1)  {hanzi_Disp(3,2,"调整月份");address=0x88; max=12;mini=1;item = g_sTime.m_ucMonth;}    //月2
  if(sel==0)  {hanzi_Disp(3,2,"调整年份");address=0x8c; max=99; mini=0;item = g_sTime.m_ucYear;}    //年1
  
  if(2 == key)
  {
    item++;//数加 1  
    while(2 == key)
        ;
  }
  if(5 == key)
  {
    item--;//数减 1 
    while(5 == key)
        ;    
  }
  if(item>max) 
  {
    item=mini;//查看数值有效范围   
  }
  if(item<mini) 
  {
    item=max;
  }
  Display(item,address);
  DS1302_WriteData(0x8e,0x00);//允许写操作
  DS1302_WriteData(address,(item/10)*16 + item%10);//转换成16进制写入1302
  DS1302_WriteData(0x8e,0x80);//写保护，禁止写操作 
}
/***FUNC+*********************************************************************/
/* Name   : GetKey                                                           */
/* Descrp : get key value                                                    */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
unsigned char GetKey(void)
{
    //P1DIR =0xf0;
    P1OUT = 0xef;//扫描第一列
    _NOP();
    _NOP();
    //P1DIR &= 0x0f;//读列
    if((P1IN & 0x0f)==0x0e)return 0x1;//'1';
    if((P1IN & 0x0f)==0x0d)return 0x5;//'5';
    if((P1IN & 0x0f)==0x0b)return 0x9;//'9';
    if((P1IN & 0x0f)==0x07)return 0xD;//'D';

    //P1DIR =0x0f;
    P1OUT = 0xdf;//扫描第二行
    _NOP();
    _NOP();
    //P1DIR &= 0x0f;//读列
    if((P1IN & 0x0f)==0x0e)return 0x2;//'2';
    if((P1IN & 0x0f)==0x0d)return 0x6;//'6';
    if((P1IN & 0x0f)==0x0b)return 0xA;//'A';
    if((P1IN & 0x0f)==0x07)return 0xE;//'E';


    //P1DIR =0xff;
    P1OUT = 0xbf;//扫描第三行
    _NOP();
    _NOP();
    //P1DIR &= 0x0f;//读列
    if((P1IN & 0x0f)==0x0e)return 0x3;//'3';
    if((P1IN & 0x0f)==0x0d)return 0x7;//'7';
    if((P1IN & 0x0f)==0x0b)return 0xB;//'B';
    if((P1IN & 0x0f)==0x07)return 0xF;//'F';


    //P1DIR =0xff;
    P1OUT = 0x7f;//扫描第四行
    _NOP();
    _NOP();
    //P1DIR &= 0x0f;//读列
    if((P1IN & 0x0f)==0x0e)return 0x4;//'4';
    if((P1IN & 0x0f)==0x0d)return 0x8;//'8';
    if((P1IN & 0x0f)==0x0b)return 0xC;//'C';
    if((P1IN & 0x0f)==0x07)return 0x0;//'0';
    return 0;
}

/***FUNC+*********************************************************************/
/* Name   : Portinit                                                         */
/* Descrp : port init                                                        */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Portinit(void)
{
    /****************************************************************************/
    /* 1、初始化4*4 Key引脚                                                     */
    /****************************************************************************/
    P1DIR =0xf0;
    /****************************************************************************/
    /* 2、初始化DS18B20引脚                                                     */
    /****************************************************************************/
    P2DIR &= ~BIT5;
    // 将P2设置为I/O口
    P2SEL = 0;
     //设置为输出
    P2DIR |= BIT5;
    //先输出低电平
    P2OUT &= ~(BIT5);
    /****************************************************************************/
    /* 3、初始化DS1302                                                          */
    /****************************************************************************/
    DS1302_Reset();
    /****************************************************************************/
    /* 4、初始化LCD12864引脚                                                     */
    /****************************************************************************/
    // 将P5设置为I/O口
    P5SEL = 0;
    // 将P4设置为I/O口
    P4SEL = 0;
    // 将P4设置为输出方向
    P4DIR |= 0xFF;
    // 将P5.0 P5.1 P5.2 设置为输出方向
    P5DIR |= BIT0;
    P5DIR |= BIT1;
    P5DIR |= BIT2;
    P5DIR |= BIT3;
    P5DIR |= BIT5;   
}
/***FUNC+*********************************************************************/
/* Name   : Init_CLK                                                         */
/* Descrp : Set PLL Clock.                                                   */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Init_CLK(void)
{
   int index;
   BCSCTL1&=~0X00;   //打开XT2振荡器
   do
   {
      IFG1 &= ~OFIFG;       // 清除振荡器失效标志
      for (index = 0xFF; index > 0; index--)// 延时，等待XT2起振
    {
        ;
    }
    } while ((IFG1 & OFIFG) != 0);// 判断XT2是否起振
    
    BCSCTL2 =SELM_2+SELS;   //选择MCLK、SMCLK为XT2   
}
/***FUNC+*********************************************************************/
/* Name   : main                                                             */
/* Descrp : main process                                                     */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void main(void)
{
    unsigned char e=0;
    // 停止WDT
    WDTCTL = WDTPW + WDTHOLD;
    Init_CLK(); 
    Portinit();
    Lcd_Init();
    Init_DS18B20();
   (void)memcpy((unsigned char *)&g_sTime,SettingData,sizeof(g_sTime)/sizeof(unsigned char ));
//    DS1302_SettingData();
    hanzi_Disp(1,1,"电子万年历钟");
    Delay1ms(0X800);
    Lcd_Init();
    Delay1ms(0X100);
    hanzi_Disp(0,0,"按键1 退出取消");
    hanzi_Disp(1,0,"按键2 加1");
    hanzi_Disp(2,0,"按键3 设置下一项");
    hanzi_Disp(3,0,"按键4 减1");
    Delay1ms(0X800);
    while(1)
    {
        
        
        key = GetKey();
        if(key!=0xFF)
        {
//--------------------------------------------------------------------------------------------------------
            if (3 == key) // 设置时间
            {
                if(3 == key && w == 1)      //当是调时状态 本键用于调整下一项
                {
                    e++;
                    if (e >= 7 ){e = 0;}
                    Set_time(e);//调整  
                }
        
                if(3 == key && w == 0)      //当是正常状态时就进入调时状态
                {
                 
                    Lcd_Fill(0x00);         //清屏
                    w=1;                    //进入调时
                    
                    updata();
                    hanzi_Disp(0,0,"[Esc] ");
                    hanzi_Disp(0,5,"[Next]");
                    hanzi_Disp(3,0,"[+] ");
                    hanzi_Disp(3,6," [-]");
                    Set_time(e);    //调整
                }
            }

        //----------------------------------------------------------------------------------------------------
            if (1 == key) // 当在调时状态时就退出调时
            {
                if(1 == key && w == 1)
                {       
                    w = 0;  //退出调时
                    e = 0;  //“下一项”计数器清0   
                }

                if(1 == key && w == 0)
                {   
                    Lcd_Init();    
                    hanzi_Disp(0,1,"电子万年历钟");
                    Delay1ms(2000);
                }
                W_Menu1();           
            }
//----------------------------------------------------------------------------------------------------
            if (2 == key && w == 1)
            { // 加减调整{
                if(2 == key && w == 1)
                {
                    Set_time(e);//调整
                }
            }

//-----------------------------------------------------------------------------------------------------
            if (5 == key && w == 1)
            { // 加减调整      
           
                if(5 == key && w == 1)
                {       
                    Set_time(e);//调整
                }
            }
//---------------------------------------------------------------------------------------------------
            
        }        
        
        if(w == 0)
        {   //正常走时
          W_Menu1();
        }
        else 
        {      //启动调时

        }
  }
}


