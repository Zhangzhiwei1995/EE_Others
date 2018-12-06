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
uint 	bdata sta;   //状态标志
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;

sbit io_LCD12864_RS = P2^0 ;
sbit io_LCD12864_RW = P2^1 ;
sbit io_LCD12864_EN = P2^2 ;

sbit CE=P3^4;
sbit CSN =P3^7;
sbit SCK =P3^5;
sbit MOSI =P3^3;
sbit MISO =P3^6;
sbit IRQ =P3^2;

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
void nRF24L01_RxPacket(uchar * tx_buf); //启动一次发送
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
		SPI_RW(*pBuf++);
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

/****************************************************************************************************/
/*函数：void SetRX_Mode(void)
/*功能：数据接收配置 
/****************************************************************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	CE = 1; 
	 v_DelayMs_f(10);

}

/******************************************************************************************************/
/*函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
/*功能：数据读取后放如rx_buf接收缓冲区中
/******************************************************************************************************/
unsigned char NRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;
	status=SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
	//if(RX_DR)				// 判断是否接收到数据
	{	
		 
	    CE = 0; 			
	  SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//读取数据完成标志
//	CSN = 0;
//	SPI_RW(FLUSH_RX);
 //   CSN = 1;
 	 /**到此为止*/
 	SPI_RW_Reg(WRITE_REG+STATUS,0xff);//清中断标志	 
	}

	
	 return revale;
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

void main()
{
	uchar RxBuf[32];
	uchar Buf0[5]={0,0,0,0,0};
	uchar i=0;

init_NRF24L01();
	IRQ=1;

//	SPI_Read_Buf(RX_ADDR_P0,Buf0,5);
	
v_DelayMs_f(100);
v_Lcd12864Init_f() ;
v_Lcd12864SendCmd_f(0x01) ;
v_DelayMs_f(2);
v_Lcd12864PutString_f(0,0,"环境温度");
v_Lcd12864PutString_f(0,2,"RX_N");//接收次数	
v_DelayMs_f(1000);
SetRX_Mode();

while(1)
	{
		if(IRQ==0)
		{
	
		NRF24L01_RxPacket(RxBuf);
	
		v_Lcd12864PutString_f(0,1,uchartostr(RxBuf[0]));
			
			
	v_Lcd12864PutString_f(1,1,".");	
		v_Lcd12864PutString_f(2,1,uchartostr(RxBuf[1]));
v_Lcd12864PutString_f(3,1,"℃");
			
			v_Lcd12864PutString_f(3,2,uchartostr(RxBuf[2]));
			
//		v_Lcd12864SendCmd_f(0x01) ;
		
	//v_Lcd12864PutString_f(0,3,uchartostr(RxBuf[31]));
			IRQ=1;
			
			CE=0;
		SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
			
			
		CE = 1; 
		v_DelayMs_f(5);
		
		
	}


}
	

}
