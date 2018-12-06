#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char

#define lcd12864_dataport P0	//P0为12864数据口
#define keyboard_dataport P2	//P2为键盘数据口
#define dac0832_dataport P1			//P1为dac0832数据口

sbit RS = P3^0 ;	//12864 数据命令选择端
sbit RW = P3^1 ;	//12864 读写选择端
sbit EN = P3^4 ;	//12864	12864使能信号

sbit CLK=P3^5;
sbit DAT=P3^6;
sbit CS=P3^7;

uchar   h=0;

unsigned int t=0;



#define SET_DATA  RS = 1 ;		//写命令
#define SET_INC  RS = 0 ;		//写指令
#define SET_READ  RW = 1 ;		//读
#define SET_WRITE RW = 0 ;		//写
#define SET_EN    EN = 1 ;		//使能开
#define CLR_EN    EN = 0 ;		//使能关

bit keyboard_flag=0;			//键盘处理标志	0为空闲		1为请求处理
uchar dac0832=100;						//二进制da数据
uchar tlc549_data;						//二进制ad数据
uchar e=0;						//保存预设电压的整数部分
uchar f=0;						//保存预设电压的小数部分


uchar code figure[]="0123456789";

//所有函数声明
void delaynms(uchar n);
uchar v_process_1(uchar data_16);
uchar v_process_2(uchar data_10);
uchar keyboard_scan();
void keyboard_process(uchar x);
bit yushe_process_1(void);
bit yushe_process_2(void);
void lcd12864checkbusy( void ) ;
void lcd12864sendcmd( unsigned char byCmd );
void lcd12864senddata( unsigned char byData )  ; 
void lcd12864init( void ) ;
void lcd12864setaddress( unsigned char x, y ) ;
void lcd12864putstring( unsigned char x, unsigned char y, unsigned char *pData );
void lcd12864putchar( unsigned char x, unsigned char y, unsigned char asic );
void lcd12864display_1(void);
void lcd12864display_2(uchar x,y);
uchar TLC549();



//*************************************
//		n毫秒延时程序 入口参数:n
//*************************************
void delaynms(uchar n)
{
	uchar i;
	while(n--)
		for(i=120;i>0;i--);	
}


//******************************************************************************************************
//		电压计算函数		入口参数:data_16	 	 出口参数:data_10
//******************************************************************************************************
uchar v_process_1(uchar data_16)
{
	uchar data_10;
	data_10=data_16*100/200;
	return data_10;
}

//******************************************************************************************************
//		预设电压转二进制函数	入口参数:data_10	  出口参数:data_16
//******************************************************************************************************
uchar v_process_2(uchar data_10)
{
	uchar data_16;
	data_16=data_10*200/100;
	return data_16;
}

/*
//**********************************************************
//		矩阵键盘扫描程序	出口参数：键值num 对应16个按键
//**********************************************************
uchar keyboard_scan()
{			
			uchar num,temp;
			//检测第一行
			keyboard_dataport=0xfe;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)		//当有按键按下
				{
					delaynms(15);		//延时消抖
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)	//确认有按键按下
					{
						temp=keyboard_dataport;
					switch(temp)
						{
							case 0xee:num=1;		//第一行第一列
								break;
							case 0xde:num=2;	//第一行第二列
								break;
							case 0xbe:num=3;		//第一行第三列
								break;
							case 0x7e:num=4;		//第一行第四列
								break;
						}
					while(temp!=0xf0)				//等待按键释放
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}
			//检测第二行
			P3=0xfd;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)				//有按键按下
				{
					delaynms(15);
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=keyboard_dataport;
					switch(temp)
						{
							case 0xed:num=5;	//第二行第一列
								break;
							case 0xdd:num=6;	//第二行第二列
								break;
							case 0xbd:num=7;	//第二行第三列
								break;
							case 0x7d:num=8;	//第二行第四列
								break;
						}
					while(temp!=0xf0)			//等待按键释放
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}

			//检测第三行
			keyboard_dataport=0xfb;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delaynms(15);
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=P3;
					switch(temp)
						{
							case 0xeb:num=9;
								break;
							case 0xdb:num=10;
								break;
							case 0xbb:num=11;
								break;
							case 0x7b:num=12;
								break;
						}
					while(temp!=0xf0)		//等待按键释放
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}

			//检测第四行
			keyboard_dataport=0xf7;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delaynms(5);
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=keyboard_dataport;
					switch(temp)
						{
							case 0xe7:num=13;
								break;
							case 0xd7:num=14;
								break;
							case 0xb7:num=15;
								break;
							case 0x77:num=16;
								break;
						}
					while(temp!=0xf0)			//等待按键释放
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}
			lcd12864putchar(0,2,num+48);
			return num;
				

}

*/

//*******************************************************
//		矩阵键盘扫描程序	出口参数：键值x 对应16个按键
//*******************************************************
uchar keyboard_scan(void)
{
	uchar x=0;			//键值x
	uchar temp=0;		//寄存键盘数据状态
	
//	lcd12864putstring(0,2,"键盘扫描");
	
//	keyboard_dataport=0xff;		 	//键盘数据口进入读状态
	
	keyboard_dataport=0Xf0;		
	temp=keyboard_dataport; 
	
	
	if(temp!=0xf0) 		//有按键按下
	{ 
		delaynms(5);  		//消抖延时
		if(temp!=0xf0)
		{ 
			keyboard_dataport=0xfe;		//检查第四列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xee):{x=16;break; }	//第四列第四个键
				case(0xde):{x=12;break; 	}//第四列第三个键
				case(0xbe):{x=8;break; 	}//第四列第二个键
				case(0x7e):{x=4;break; 	}//第四列第一个键

			}

			keyboard_dataport=0xfd;	//检查第三列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xed):{x=15;break; }
				case(0xdd):{x=11;break; }
				case(0xbd):{x=7;break; }
				case(0x7d):{x=3;break; }
			}

			keyboard_dataport=0xfb;	//检查第二列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xeb):{x=14;break; }
				case(0xdb):{x=10;break; }
				case(0xbb):{x=6;break; }
				case(0x7b):{x=2;
//				while(temp!=0xf0)				//等待按键释放
//		{
//					temp=keyboard_dataport;
//					temp=temp&0xf0;
//			}

				break; }
			}

			keyboard_dataport=0xf7;	//检查第一列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xe7):{x=13;break; }
				case(0xd7):{x=9;break; }
				case(0xb7):{x=5;;break; }
				case(0x77):{x=1;
		/*	while(temp!=0xf0)				//等待按键释放
			{
					temp=keyboard_dataport;
					temp=temp&0xf0;
			}
		*/
			break; }
			}
		} 
		while(temp!=0xf0)				//等待按键释放
			{
					temp=keyboard_dataport;
					temp=temp&0xf0;
			} 
	
   } 
   
 /*  if(temp!=0xf0) 		//有按键按下
	{ 
		delaynms(3);  		//消抖延时
		if(temp!=0xf0)
		{ 
		

			keyboard_dataport=0xfb;	//检查第二列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				
				case(0x7b):{x=2;break; }
			}

			keyboard_dataport=0xf7;	//检查第一列
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0x77):{x=1;break; }
			}
		} 
		while(temp!=0xf0)				//等待按键释放
			{
					temp=keyboard_dataport;
					temp=temp&0xf0;
			}
	
   } 
   
 */  
 //  lcd12864putchar(4,2,x+48);
   return x;	
}


//*******************************************************
//		矩阵键盘处理程序	入口参数：按键键值x
//*******************************************************
void keyboard_process(uchar x)
{
//	lcd12864putstring(0,3,"键盘处理");
	switch(x) 
	{ 
		case(1):
		{
			if(dac0832>=198)
			break;
			dac0832=dac0832+20;
			break; 
		}			//启动定时器0 进行自加 或者关闭定时器0 停止自加
		case(2):
			{
			if(dac0832<=0x01)
			break;
			dac0832=dac0832-20;
			break; 
			}			   //启动定时器1 进行自减 或者关闭定时器1 停止自减
		case(3):
			{if(dac0832>=198)
			break;
			dac0832=dac0832+2;
			break; 
			}				//	电压值+0.1
		case(4):
			{
			if(dac0832<=01)
			break;
			dac0832=dac0832-2;
			break; 
			} 				//	电压值-0.1
	
		case(5):{TR0=~TR0 ;if(dac0832>=198) dac0832=198;
			break; 	}
		case(6): {TR1=~TR1;if(dac0832<=01) dac0832=01;break; 	}
	//	case(7): ;break; 	
//		case(8):							//进入设置状态
//			while (! yushe_process_1()){;}
//			while (! yushe_process_2()){;}
//			break; 	
	//	case(9): ;break; 	
	//	case(10): ;break; 	
	//	case(11): ;break; 	
//		case(12):dac0832=v_process_2(e*10+f);break; 					//确定按钮
	//	case(13): ;break; 	
	//	case(14): ;break; 	
	//	case(15):x=8;break; 	
	//	case(16):x=4;break; 	

	}
	

}

//*********************************************************
//		预设电压整数位处理程序	出口参数:flag 处理完成标志
//*********************************************************
bit yushe_process_1(void)
{
	bit	flag=0;
	uchar   j;
	j=keyboard_scan();
	switch(j)
	{
		
		case(5):e=1;flag=1;break;
		case(6):e=2;flag=1;break;
		case(7):e=3;flag=1;break;
		case(9):e=4;flag=1;break;
		case(10):e=5;flag=1;break;
		case(11):e=6;flag=1;break;
		case(13):e=7;flag=1;break;
		case(14):e=8;flag=1;break;
		case(15):e=9;flag=1;break;
		case(16):e=0;flag=1;break;
	}
	
	return flag;
}

//*********************************************************
//		预设电压小数位位处理程序	出口参数:flag 处理完成标志
//*********************************************************
bit yushe_process_2(void)
{
	bit	flag=0;
	uchar   j;
	j=keyboard_scan();
	switch(j)
	{
		
		case(5):f=1;flag=1;break;
		case(6):f=2;flag=1;break;
		case(7):f=3;flag=1;break;
		case(9):f=4;flag=1;break;
		case(10):f=5;flag=1;break;
		case(11):f=6;flag=1;break;
		case(13):f=7;flag=1;break;
		case(14):f=8;flag=1;break;
		case(15):f=9;flag=1;break;
		case(16):f=0;flag=1;break;
	}
	return flag;

}




//*******************************************************
//		12864检测盲信号函数
//*******************************************************

void lcd12864checkbusy( void )      
{
    unsigned int nTimeOut = 0 ;
    SET_INC
    SET_READ
    CLR_EN
    SET_EN
    while( ( lcd12864_dataport & 0x80 ) && ( ++nTimeOut != 0 ) ) ; //最高位为0时表允许
    CLR_EN
    SET_INC
    SET_READ
}

//*******************************************************
//		12864发送命令函数	入口参数：byCmd （命令代码） 
//*******************************************************
void lcd12864sendcmd( unsigned char byCmd )      
{
    lcd12864checkbusy() ;
    SET_INC
    SET_WRITE
    CLR_EN
    lcd12864_dataport = byCmd ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}


//*******************************************************
//		12864发送信号函数	入口参数：byData（数据代码） 
//*******************************************************
void lcd12864senddata( unsigned char byData )    
{
    lcd12864checkbusy() ;
    SET_DATA
    SET_WRITE
    CLR_EN
    lcd12864_dataport= byData ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}

//*******************************************************
//		12864初始化函数	
//*******************************************************
void lcd12864init( void )                  //初始化
{
    lcd12864sendcmd( 0x30 ) ;              //基本指令集  功能设定 8位数据 无扩充指令
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x01 ) ;              //清屏
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x06 ) ;              //光标右移
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x0c ) ;              //开显示 关游标
}

//*******************************************************************
//		12864地址设置函数	入口参数：x,y	x是列数，y是行数 共8列4行
//*******************************************************************
void lcd12864setaddress( unsigned char x, y ) 
{
    unsigned char byAddress ;
    switch( y )
    {
        case 0 :    byAddress = 0x80 + x ;
            break;
        case 1 :    byAddress = 0x90 + x ;
            break ;
        case 2 :    byAddress = 0x88 + x ;
            break ;
        case 3 :    byAddress = 0x98 + x ;
            break ;
        default :
            break ;
    }
    lcd12864sendcmd( byAddress ) ;
}


//***********************************************************************************************
//		12864字符串显示函数	入口参数：x,y，*pData	x是列数，y是行数 共8列4行 *pData指向字符串
//***********************************************************************************************
void lcd12864putstring( unsigned char x, unsigned char y, unsigned char *pData )
{
    lcd12864setaddress( x, y ) ;
    while( *pData != '\0' )
    {
        lcd12864senddata( *pData++ ) ;
    }            

}

//***********************************************************************************************
//		12864字符显示函数	入口参数：x,y，asiic	x是列数，y是行数 共8列4行 asiic是要写的字符的asiic码
//***********************************************************************************************
void lcd12864putchar( unsigned char x, unsigned char y, unsigned char asic )
{
    lcd12864setaddress( x, y ) ;
    
    lcd12864senddata( asic ) ;
}


//***********************************************************************************************
//		12864常量显示函数	用于显示汉字说明等内容
//***********************************************************************************************
void lcd12864display_1(void)
{
		lcd12864putstring(0,0,"理想电压:");
		 delaynms( 300 ) ;
		lcd12864putstring(0,1,"实际电压:");
		 delaynms( 300 ) ;
}

//******************************************************************************************************
//		12864变量显示函数	用于显示理想电压和实际电压的数值 	入口参数:x,y	x为理想输出电压 y为实际测量电压
//******************************************************************************************************
void lcd12864display_2(uchar x,y)
{
	uchar a,b,c,d;
	a=x/10;
	b=x%10;
	lcd12864putchar(6,0,figure[a]);
	lcd12864senddata( '.' ) ;
	lcd12864senddata( figure[b]) ;
	lcd12864senddata( 'V' ) ;
	
	c=y/10;
	d=y%10;
	lcd12864putchar(6,1,figure[c]);
	lcd12864senddata( '.' ) ;
	lcd12864senddata( figure[d]) ;
	lcd12864senddata( 'V' ) ;
	
	
	
	
}




//*******************************************
//读ad函数		出口参数：16进制ad输出值
//*******************************************
uchar TLC549()
{
	unsigned i,tmp;
	CS=1;
	CLK=0;
	CS=0;
	_nop_();
 	_nop_();
	for(i=0;i<8;i++)
	{
		tmp<<=1; 		//左移一位
		tmp|=DAT;
		CLK=1;
		_nop_();
		CLK=0;
	    
	}
	CS=1;
	for(i=17;i>0;i--)
	_nop_();
	return(tmp);
}



//*******************************************
//			函数功能：主函数
//*******************************************
void main(void)
{
	dac0832_dataport=dac0832;
	
	
	lcd12864init();				//12864初始化
	lcd12864display_1();		//12864显示静态内容
	
	
	EX0=1;     					//允许使用外中断0
//	IT0=1;    					//选择负跳变来触发外中断0
	
	EX1=1;     					//允许使用外中断1
	IT1=1;    					//选择负跳变来触发外中断1
	
	
	//定时器0、1设置		用于控制输出电压自动加或者自动减
	TMOD=0x11;            //使用定时器T0 T1的模式1	16位

	TH0=(65536-50)/256;  //定时器T0的高8位赋初值
	TL0=(65536-50)%256;  //定时器T0的低8位赋初值	计时50us
	ET0=1;						//允许定时器0中断
	TR0=0;					//关闭定时器0
	
	TH1=(65536-50)/256;  //定时器T1的高8位赋初值
	TL1=(65536-50)%256;  //定时器T1的低8位赋初值	计时50us
	ET1=1;						//允许定时器1中断
	TR1=0;					//关闭定时器1
	
/*	
	//定时器2设置	用于控制ad的采样频率	
	RCAP2H=(65536-50000)/256;;				//定时器2初值  计时50000us		16位自动重装
	RCAP2L=(65536-50000)%256;				//定时器2初值  计时50000us
	ET2=1;     					// 允许T2定时器中断
	TR2=1;     					// 启动T2定时器
//	lcd12864putstring(0,2,"定时器中断已开");
	TH2=RCAP2H;					//定时器2赋初值  计时50us
	TL2=RCAP2L;					//定时器2赋初值  计时50us
	
*/

	EA=1;      					//开放总中断
	
	keyboard_dataport=0x0f;
	
	

	while (1)
	{
		if(keyboard_flag==1)		//请求处理键盘操作
		{
	
			keyboard_flag=0;		//取消键盘请求标志
			keyboard_process(keyboard_scan());	//进行键盘按键操作
			keyboard_dataport=0x0f;
		}
			
		dac0832_dataport=dac0832;
		
		
		tlc549_data=TLC549() ;
		
		h++;
		if(h==10)
		{
			h=0;
		lcd12864putstring(0,2,"                ");
		lcd12864putstring(0,3,"                ");
		}
		
		delaynms(10);
		
		
		lcd12864display_2(v_process_1(dac0832),v_process_1(tlc549_data));
	}
			
}
/*
//***********************************************************************
//			函数功能：外中断T0的中断服务程序	用于进行过流保护
//***********************************************************************
void int0(void) interrupt 0 //外中断0的中断编号为0
{
	EX0=0;     			//关闭外中断0
	
	dac0832=0;
	lcd12864putstring(0,2,"电流过大");
	delaynms( 1000 ) ;
	lcd12864putstring(0,3,"已进行自动降压");
	delaynms( 1000 ) ;
	lcd12864putstring(0,2,"                                 ");
	lcd12864putstring(0,3,"                                 ");
	
	EX0=1;     			//打开外中断0
}
*/

//***********************************************************************
//			函数功能：外中断T1的中断服务程序	用于检测键盘按键是否按下
//***********************************************************************
void int1(void) interrupt 2 //外中断1的中断编号为2
{
	EX1=0;     			//关闭外中断0
	keyboard_flag=1;	//键盘处理标志置1
	EX1=1;     			//打开外中断0
	

}


//***************************************************************
//			函数功能：定时器0的中断服务程序	用于输出电压值自动加	
//***************************************************************
void timer0(void) interrupt 1 
{ 
	static uchar    m=0;
//	static uchar	n=0;	
	TR0=0;					//关闭定时器0
	m++;
	if (m==2)				//50us*20=1ms
	{

	  dac0832=dac0832+2; //输出电压自加
		
		m=0;
//	  lcd12864display_2(dac0832,v_process_1(tlc549_data));
		
	}

//	if (n==1000)				//100*1ms=0.1s
//	{
		
//	    dac0832++; //输出电压自加
//		lcd12864display_2(dac0832,v_process_1(tlc549_data));
//		n=0;
		
//	}

	
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	TR0=1;					//打开定时器0
	
}

//***************************************************************
//			函数功能：定时器1的中断服务程序	用于输出电压值自动减
//***************************************************************
void timer1(void) interrupt 3
{ 
	static uchar    p=0;
//	static uchar	q=0;	
	TR1=0;					//关闭定时器1
	p++;
	if (p==2)				//50us*20=1ms
	{
	 
	 dac0832=dac0832-2;			//电压值自动
		
		p=0;
//	 lcd12864display_2(dac0832,v_process_1(tlc549_data));
		
	
	}

//	if (q==1000)				//100*1ms=0.1s
//	{
		
//		dac0832--;			//电压值自动减
//		lcd12864display_2(dac0832,v_process_1(tlc549_data));
//		q=0;
//		
//	}

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	TR1=1;					//打开定时器1
	
}

/*
//***************************************************************
//			函数功能：定时器2的中断服务程序	用于控制ad的采样频率	
//***************************************************************
void timer2() interrupt 5
{
	static uchar  d1=0;

//	static uchar  d2=0;;
//	lcd12864putstring(0,3,"已进入定时器2");
	
	TF2=0;					//!!!注意!!! 定时器2必须由软件对溢出标志位清零，硬件不能清零，这里与定时器0和定时器1不同!!!
	TR2=0;					//关闭定时器2
	d1++;
	if (d1==10)				//50000us*10=0.5s
	{
	 d1=0;
	 tlc549_data=TLC549() ;	
	 lcd12864display_2(v_process_1(dac0832),v_process_1(tlc549_data));
	}
//	if (d2==500)				//500*1ms=0.5s
//	{
//		lcd12864putstring(0,3,"已进入定时器2中断");
//		tlc549_data=TLC549() ;		//采样		采样频率  1秒2次
//		d2=0;
//	}
	//定时器2能自动重装		不需再赋初值
	TR2=1;					//打开定时器2
	
}
*/
