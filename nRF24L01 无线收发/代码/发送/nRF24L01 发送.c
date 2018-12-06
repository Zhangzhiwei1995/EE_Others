#include <reg52.h>
#include <intrins.h>
#include "nrf2401.h"
typedef unsigned char uchar;
typedef unsigned int uint;  
extern void Lcd_lnit (void);    //LCD初始化
//extern void hanzi_Disp(uchu xJ uchar y,uchar *s);
uchar *uchartostr (uchar num);
uchar *uchartostr(uchar num);
uchar str [4];//作为存储上面uchartostr函数转换的字符
uchar status;
uchar TX_ADDRESS[TX_ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05}; //本地地址 
uchar RX_ADDRESS[RX_ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05}; //接收地址 

unsigned char time;   //设置全局变量，专门用于严格延时
unsigned char flag,tltemp;						//负温度标志 和临时暂存变量

sbit io_LCD12864_RS = P2^0 ;
sbit io_LCD12864_RW = P2^1 ;
sbit io_LCD12864_EN = P2^2 ;

sbit CE=P3^4;
sbit CSN =P3^7;
sbit SCK =P3^5;
sbit MOSI =P3^3;
sbit MISO =P3^6;
sbit IRQ =P3^2;

sbit DQ=P1^0;


#define io_LCD12864_DATAPORT P0

#define SET_DATA  io_LCD12864_RS = 1 ;
#define SET_INC  io_LCD12864_RS = 0 ;
#define SET_READ  io_LCD12864_RW = 1 ;
#define SET_WRITE io_LCD12864_RW = 0 ;
#define SET_EN    io_LCD12864_EN = 1 ;
#define CLR_EN    io_LCD12864_EN = 0 ;

void v_Lcd12864PutString_f( unsigned char x, unsigned char y, unsigned char *pData );
void v_DelayMs_f( unsigned int nDelay );
//*********************NRF24L01函数定义**************************** 
//void delayms(uint t);//毫秒延时
void init_NRF24L01(void);   //模块初始化函数
//uchar SPI_RW(uchar reg);    //基本SPI读写时序
uchar SPI_READ_Reg(uchar reg);  //从寄存器reg读一个字节
//void SetRX_Mode(void);      //设置接收模式
//uchar SPI_RW_Reg(uchar reg, uchar value);  //向寄存器写一个字节
//uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars); // 从缓冲器读出uchars字节的数据
//uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars); //向缓冲器写进uchars字节的数据
void nRF24L01_TxPacket(uchar * tx_buf); //启动一次发送
//uchar nRF24L01_RxPacket(uchar * rx_buf);//读取接收的数据，放入rx_buf数组
uchar *uchartostr(uchar  num);

uchar *uchartostr(uchar  num)
{
    uchar  x00,xx,x0,x,n;
    x00=num/100;
    xx=num%100;
    x0= xx/10 ;
    x= xx%10 ;
    n=0;
    if (x00!=0)
    {str[n] =x00+48;//值加48即为字符
			n++;
    }
    if(!(x00==0&&x0==0))
    {
			str[n]=x0+48;
			n++;
    }
    str[n] =x+48;
    n++;
    str[n]='\0';
    return str;
}
	
/****************************************************************************************************
/*函数：uint SPI_RW(uint uchar1)
/*功能：NRF24L01的SPI写时序
/****************************************************************************************************/
uint SPI_RW(uint uchar1)
{
	  uint bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (uchar1 & 0x80);         // output 'uchar', MSB to MOSI
		uchar1 = (uchar1 << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		uchar1 |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(uchar1);           		  // return read uchar
}
		

/****************************************************************************************************
/*函数：uchar SPI_Read(uchar reg)
/*功能：NRF24L01的SPI读时序
/****************************************************************************************************/
uchar SPI_Read_Reg(uchar reg)
{
	uchar reg_val;
//	v_Lcd12864PutString_f(0,3,"进入read");
	CSN = 0; 	// CSN low, initialize SPI communication...
//	v_DelayMs_f( 1000 );
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
		
	//	v_DelayMs_f(800);
	//	v_Lcd12864PutString_f(0,3,"退出read");
	
	
	return(reg_val);        // return register value
	
}

/****************************************************************************************************/
/*功能：NRF24L01写寄存器函数
/****************************************************************************************************/
uint SPI_RW_Reg(uchar reg, uchar value)
{
	uint status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}


/****************************************************************************************************/
/*函数：uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
/****************************************************************************************************/
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);       		// Select register to write to and read status uchar
	
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
		pBuf[uchar_ctr] = SPI_RW(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status uchar
}
/*********************************************************************************************************
/*函数：uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
//*********************************************************************************************************/
uint SPI_Write_Buf(uchar reg, uchar  *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPI使能       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		{SPI_RW(*pBuf);pBuf++;}
	CSN = 1;           //关闭SPI
	return(status);    // 
}

void init_NRF24L01(void)
{
	v_DelayMs_f(1);
	CE=0;
	CSN=1;
	SCK=0;
	IRQ=1;	
	SPI_Write_Buf(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,RX_ADDRESS,TX_ADR_WIDTH);
	SPI_RW_Reg(WRITE_REG+EN_AA,0X00);
	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0X01);
	SPI_RW_Reg(WRITE_REG+RF_CH,1);
	SPI_RW_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG+RF_SETUP+RF_SETUP,0X07);

}

void nRF24L01_TxPacket(unsigned char*tx_buf)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG+CONFIG,0X0E);
	CE=1;
	v_DelayMs_f( 1 ) ;
}

//********************************************************************************************************************************

void v_Lcd12864CheckBusy_f( void )      //忙检测函数
{
    unsigned int nTimeOut = 0 ;
    SET_INC
    SET_READ
    CLR_EN
    SET_EN
    while( ( io_LCD12864_DATAPORT & 0x80 ) && ( ++nTimeOut != 0 ) ) ; //最高位为0时表允许
    CLR_EN
    SET_INC
    SET_READ
}

void v_Lcd12864SendCmd_f( unsigned char byCmd )      //发送命令
{
    v_Lcd12864CheckBusy_f() ;
    SET_INC
    SET_WRITE
    CLR_EN
    io_LCD12864_DATAPORT = byCmd ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}
void v_Lcd12864SendData_f( unsigned char byData )    //发送数据
{
    v_Lcd12864CheckBusy_f() ;
    SET_DATA
    SET_WRITE
    CLR_EN
    io_LCD12864_DATAPORT = byData ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}

void v_DelayMs_f( unsigned int nDelay )              //n毫秒延时
{
    unsigned int i ;
    for( ; nDelay > 0 ; nDelay-- )
    {
        for( i = 125 ; i > 0 ; i-- ) ;
    }
}

void v_Lcd12864Init_f( void )                  //初始化
{
    v_Lcd12864SendCmd_f( 0x30 ) ;              //基本指令集  功能设定 8位数据 无扩充指令
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x01 ) ;              //清屏
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x06 ) ;              //光标右移
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x0c ) ;              //开显示 关游标
}
void v_Lcd12864SetAddress_f( unsigned char x, y )  //地址转换   x是列数，y是行数 共8列4行
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
    v_Lcd12864SendCmd_f( byAddress ) ;
}
void v_Lcd12864PutString_f( unsigned char x, unsigned char y, unsigned char *pData )
{
    v_Lcd12864SetAddress_f( x, y ) ;
    while( *pData != '\0' )
    {
        v_Lcd12864SendData_f( *pData++ ) ;
    }            

}

//*******************************************************************************************
/*****************************************************
函数功能：将DS18B20传感器初始化，读取应答信号
出口参数：flag 
***************************************************/
bit Init_DS18B20(void)	
{
 bit flag;         //储存DS18B20是否存在的标志，flag=0，表示存在；flag=1，表示不存在
 DQ = 1;           //先将数据线拉高
 for(time=0;time<2;time++) //略微延时约6微秒
     ;
 DQ = 0;           //再将数据线从高拉低，要求保持480~960us
 for(time=0;time<200;time++)  //略微延时约600微秒
     ;         //以向DS18B20发出一持续480~960us的低电平复位脉冲 
 DQ = 1;           //释放数据线（将数据线拉高） 
  for(time=0;time<10;time++)
     ;  //延时约30us（释放总线后需等待15~60us让DS18B20输出存在脉冲）
 flag=DQ;          //让单片机检测是否输出了存在脉冲（DQ=0表示存在）      
 for(time=0;time<200;time++)  //延时足够长时间，等待存在脉冲输出完毕
      ;
 return (flag);    //返回检测成功标志
}
/*****************************************************
函数功能：从DS18B20读取一个字节数据
出口参数：dat
***************************************************/ 
unsigned char ReadOneChar(void)
 {
		unsigned char i=0;	
		unsigned char dat;  //储存读出的一个字节数据
		for (i=0;i<8;i++)
		 {
		   
		   DQ =1;       // 先将数据线拉高
		   _nop_();	    //等待一个机器周期	 
		   DQ = 0;      //单片机从DS18B20读书据时,将数据线从高拉低即启动读时序
		   _nop_();     //等待一个机器周期		   
		   DQ = 1;     //将数据线"人为"拉高,为单片机检测DS18B20的输出电平作准备
		   for(time=0;time<2;time++)
             ;      //延时约6us，使主机在15us内采样
			 	dat>>=1;
		   if(DQ==1)
		      dat|=0x80;  //如果读到的数据是1，则将1存入dat
			else
				dat|=0x00;//如果读到的数据是0，则将0存入dat
		     //将单片机检测到的电平信号DQ存入r[i]	
		   for(time=0;time<8;time++)
		      	;              //延时3us,两个读时序之间必须有大于1us的恢复期	
	    }	                    
	 return(dat);    //返回读出的十六进制数据
}
/*****************************************************
函数功能：向DS18B20写入一个字节数据
入口参数：dat
***************************************************/  
WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=0; i<8; i++)
		 {
		  DQ =1;         // 先将数据线拉高
		  _nop_();	     //等待一个机器周期	 
		  DQ=0;          //将数据线从高拉低时即启动写时序       
		  DQ=dat&0x01;   //利用与运算取出要写的某位二进制数据,
                       //并将其送到数据线上等待DS18B20采样	
		 for(time=0;time<10;time++)	
		     ;//延时约30us，DS18B20在拉低后的约15~60us期间从数据线上采样
		  DQ=1;          //释放数据线		    
		  for(time=0;time<1;time++)
			  ;//延时3us,两个写时序间至少需要1us的恢复期
		  dat>>=1;       //将dat中的各二进制位数据右移1位
		 }
	  for(time=0;time<4;time++)
	              ; //稍作延时,给硬件一点反应时间
}

/*****************************************************
函数功能：做好读温度的准备
***************************************************/ 
void ReadyReadTemp(void)
{
      Init_DS18B20();     //将DS18B20初始化
		WriteOneChar(0xCC); // 跳过读序号列号的操作
		WriteOneChar(0x44); // 启动温度转换	  
	      v_DelayMs_f(200); //转换一次需要延时一段时间 
	
		Init_DS18B20();     //将DS18B20初始化
		WriteOneChar(0xCC); //跳过读序号列号的操作
		WriteOneChar(0xBE); //读取温度寄存器,前两个分别是温度的低位和高位	
}


void main()
{
	unsigned char TL;     //储存暂存器的温度低位
     unsigned char TH;    //储存暂存器的温度高位
     unsigned char TN;      //储存温度的整数部分
	  unsigned char TD;       //储存温度的小数部分
	
	uchar status;
	uchar fifo_s;
uchar i;
	
	uchar TxBuf[32];
	

	
v_DelayMs_f(100);
v_Lcd12864Init_f() ;
v_Lcd12864SendCmd_f(0x01) ;
v_DelayMs_f(2);
v_Lcd12864PutString_f(0,0,"nRF24L01发射测试");
v_Lcd12864PutString_f(0,1,"STA:");
v_Lcd12864PutString_f(0,2,"FIF:");
init_NRF24L01();
i=1;
	

	while(1)
{
	flag=0;
		ReadyReadTemp();     //读温度准备

	    TL=ReadOneChar();    //先读的是温度值低位
		TH=ReadOneChar();    //接着读的是温度值高位
	if((TH&0xf8)!=0x00)//判断高五位 得到温度正负标志
		{
		flag=1;
		TL=~TL;	 //取反
		TH=~TH;	  //取反
		tltemp=TL+1; //低位加1
		TL=tltemp;
		if(tltemp>255) TH++;  //如果低8位大于255，向高8位进1
	   TN=TH*16+TL/16;      //实际温度值=(TH*256+TL)/16,即：TH*16+TL/16
				                  //这样得出的是温度的整数部分,小数部分被丢弃了
	   TD=(TL%16)*10/16;    //计算温度的小数部分,将余数乘以10再除以16取整，
		}
		TN=TH*16+TL/16;      //实际温度值=(TH*256+TL)/16,即：TH*16+TL/16
				                  //这样得出的是温度的整数部分,小数部分被丢弃了
	   TD=(TL%16)*10/16;    //计算温度的小数部分,将余数乘以10再除以16取整，
			                  //这样得到的是温度小数部分的第一位数字(保留1位小数)
	
	TxBuf[0]=TN;
	TxBuf[1]=TD;
  TxBuf[2]=i;		
	v_Lcd12864PutString_f(0,3,uchartostr(TN));	
	v_Lcd12864PutString_f(1,3,".");	
		v_Lcd12864PutString_f(2,3,uchartostr(TD));
v_Lcd12864PutString_f(3,3,"℃");			
		
	SPI_RW_Reg(WRITE_REG+STATUS,0XFF);
	SPI_RW_Reg(FLUSH_TX,0XFF);
		status=SPI_READ_Reg(STATUS);
	fifo_s=SPI_READ_Reg(FIFO_STATUS);
	v_Lcd12864PutString_f(2,1,uchartostr(status));
  v_Lcd12864PutString_f(2,2,uchartostr(fifo_s));
	nRF24L01_TxPacket(TxBuf);
	v_DelayMs_f(200);
	status=SPI_READ_Reg(STATUS);
	fifo_s=SPI_READ_Reg(FIFO_STATUS);
	v_Lcd12864PutString_f(4,1,uchartostr(status));
	v_Lcd12864PutString_f(4,2,uchartostr(fifo_s));
	v_Lcd12864PutString_f(6,1,"TX_N");//发送次数
	v_Lcd12864PutString_f(6,2,uchartostr(i));
	v_DelayMs_f(1000);
	v_Lcd12864PutString_f(2,1,"        ");
	v_Lcd12864PutString_f(2,2,"        ");
	i++;
	if(i==255) i=1;
	v_DelayMs_f(600);
	}
}
