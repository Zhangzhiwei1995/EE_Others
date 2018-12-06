#include <reg52.h>
#include <intrins.h>
#include "nrf2401.h"
typedef unsigned char uchar;
typedef unsigned int uint;  
extern void Lcd_lnit (void);    //LCD��ʼ��
//extern void hanzi_Disp(uchu xJ uchar y,uchar *s);
uchar *uchartostr (uchar num);
uchar *uchartostr(uchar num);
uchar str [4];//��Ϊ�洢����uchartostr����ת�����ַ�
uchar status;
uchar TX_ADDRESS[TX_ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05}; //���ص�ַ 
uchar RX_ADDRESS[RX_ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05}; //���յ�ַ 

unsigned char time;   //����ȫ�ֱ�����ר�������ϸ���ʱ
unsigned char flag,tltemp;						//���¶ȱ�־ ����ʱ�ݴ����

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
//*********************NRF24L01��������**************************** 
//void delayms(uint t);//������ʱ
void init_NRF24L01(void);   //ģ���ʼ������
//uchar SPI_RW(uchar reg);    //����SPI��дʱ��
uchar SPI_READ_Reg(uchar reg);  //�ӼĴ���reg��һ���ֽ�
//void SetRX_Mode(void);      //���ý���ģʽ
//uchar SPI_RW_Reg(uchar reg, uchar value);  //��Ĵ���дһ���ֽ�
//uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars); // �ӻ���������uchars�ֽڵ�����
//uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars); //�򻺳���д��uchars�ֽڵ�����
void nRF24L01_TxPacket(uchar * tx_buf); //����һ�η���
//uchar nRF24L01_RxPacket(uchar * rx_buf);//��ȡ���յ����ݣ�����rx_buf����
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
    {str[n] =x00+48;//ֵ��48��Ϊ�ַ�
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
/*������uint SPI_RW(uint uchar1)
/*���ܣ�NRF24L01��SPIдʱ��
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
/*������uchar SPI_Read(uchar reg)
/*���ܣ�NRF24L01��SPI��ʱ��
/****************************************************************************************************/
uchar SPI_Read_Reg(uchar reg)
{
	uchar reg_val;
//	v_Lcd12864PutString_f(0,3,"����read");
	CSN = 0; 	// CSN low, initialize SPI communication...
//	v_DelayMs_f( 1000 );
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
		
	//	v_DelayMs_f(800);
	//	v_Lcd12864PutString_f(0,3,"�˳�read");
	
	
	return(reg_val);        // return register value
	
}

/****************************************************************************************************/
/*���ܣ�NRF24L01д�Ĵ�������
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
/*������uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
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
/*������uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
//*********************************************************************************************************/
uint SPI_Write_Buf(uchar reg, uchar  *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPIʹ��       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		{SPI_RW(*pBuf);pBuf++;}
	CSN = 1;           //�ر�SPI
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

void v_Lcd12864CheckBusy_f( void )      //æ��⺯��
{
    unsigned int nTimeOut = 0 ;
    SET_INC
    SET_READ
    CLR_EN
    SET_EN
    while( ( io_LCD12864_DATAPORT & 0x80 ) && ( ++nTimeOut != 0 ) ) ; //���λΪ0ʱ������
    CLR_EN
    SET_INC
    SET_READ
}

void v_Lcd12864SendCmd_f( unsigned char byCmd )      //��������
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
void v_Lcd12864SendData_f( unsigned char byData )    //��������
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

void v_DelayMs_f( unsigned int nDelay )              //n������ʱ
{
    unsigned int i ;
    for( ; nDelay > 0 ; nDelay-- )
    {
        for( i = 125 ; i > 0 ; i-- ) ;
    }
}

void v_Lcd12864Init_f( void )                  //��ʼ��
{
    v_Lcd12864SendCmd_f( 0x30 ) ;              //����ָ�  �����趨 8λ���� ������ָ��
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x01 ) ;              //����
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x06 ) ;              //�������
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x0c ) ;              //����ʾ ���α�
}
void v_Lcd12864SetAddress_f( unsigned char x, y )  //��ַת��   x��������y������ ��8��4��
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
�������ܣ���DS18B20��������ʼ������ȡӦ���ź�
���ڲ�����flag 
***************************************************/
bit Init_DS18B20(void)	
{
 bit flag;         //����DS18B20�Ƿ���ڵı�־��flag=0����ʾ���ڣ�flag=1����ʾ������
 DQ = 1;           //�Ƚ�����������
 for(time=0;time<2;time++) //��΢��ʱԼ6΢��
     ;
 DQ = 0;           //�ٽ������ߴӸ����ͣ�Ҫ�󱣳�480~960us
 for(time=0;time<200;time++)  //��΢��ʱԼ600΢��
     ;         //����DS18B20����һ����480~960us�ĵ͵�ƽ��λ���� 
 DQ = 1;           //�ͷ������ߣ������������ߣ� 
  for(time=0;time<10;time++)
     ;  //��ʱԼ30us���ͷ����ߺ���ȴ�15~60us��DS18B20����������壩
 flag=DQ;          //�õ�Ƭ������Ƿ�����˴������壨DQ=0��ʾ���ڣ�      
 for(time=0;time<200;time++)  //��ʱ�㹻��ʱ�䣬�ȴ���������������
      ;
 return (flag);    //���ؼ��ɹ���־
}
/*****************************************************
�������ܣ���DS18B20��ȡһ���ֽ�����
���ڲ�����dat
***************************************************/ 
unsigned char ReadOneChar(void)
 {
		unsigned char i=0;	
		unsigned char dat;  //���������һ���ֽ�����
		for (i=0;i<8;i++)
		 {
		   
		   DQ =1;       // �Ƚ�����������
		   _nop_();	    //�ȴ�һ����������	 
		   DQ = 0;      //��Ƭ����DS18B20�����ʱ,�������ߴӸ����ͼ�������ʱ��
		   _nop_();     //�ȴ�һ����������		   
		   DQ = 1;     //��������"��Ϊ"����,Ϊ��Ƭ�����DS18B20�������ƽ��׼��
		   for(time=0;time<2;time++)
             ;      //��ʱԼ6us��ʹ������15us�ڲ���
			 	dat>>=1;
		   if(DQ==1)
		      dat|=0x80;  //���������������1����1����dat
			else
				dat|=0x00;//���������������0����0����dat
		     //����Ƭ����⵽�ĵ�ƽ�ź�DQ����r[i]	
		   for(time=0;time<8;time++)
		      	;              //��ʱ3us,������ʱ��֮������д���1us�Ļָ���	
	    }	                    
	 return(dat);    //���ض�����ʮ����������
}
/*****************************************************
�������ܣ���DS18B20д��һ���ֽ�����
��ڲ�����dat
***************************************************/  
WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=0; i<8; i++)
		 {
		  DQ =1;         // �Ƚ�����������
		  _nop_();	     //�ȴ�һ����������	 
		  DQ=0;          //�������ߴӸ�����ʱ������дʱ��       
		  DQ=dat&0x01;   //����������ȡ��Ҫд��ĳλ����������,
                       //�������͵��������ϵȴ�DS18B20����	
		 for(time=0;time<10;time++)	
		     ;//��ʱԼ30us��DS18B20�����ͺ��Լ15~60us�ڼ���������ϲ���
		  DQ=1;          //�ͷ�������		    
		  for(time=0;time<1;time++)
			  ;//��ʱ3us,����дʱ���������Ҫ1us�Ļָ���
		  dat>>=1;       //��dat�еĸ�������λ��������1λ
		 }
	  for(time=0;time<4;time++)
	              ; //������ʱ,��Ӳ��һ�㷴Ӧʱ��
}

/*****************************************************
�������ܣ����ö��¶ȵ�׼��
***************************************************/ 
void ReadyReadTemp(void)
{
      Init_DS18B20();     //��DS18B20��ʼ��
		WriteOneChar(0xCC); // ����������кŵĲ���
		WriteOneChar(0x44); // �����¶�ת��	  
	      v_DelayMs_f(200); //ת��һ����Ҫ��ʱһ��ʱ�� 
	
		Init_DS18B20();     //��DS18B20��ʼ��
		WriteOneChar(0xCC); //����������кŵĲ���
		WriteOneChar(0xBE); //��ȡ�¶ȼĴ���,ǰ�����ֱ����¶ȵĵ�λ�͸�λ	
}


void main()
{
	unsigned char TL;     //�����ݴ������¶ȵ�λ
     unsigned char TH;    //�����ݴ������¶ȸ�λ
     unsigned char TN;      //�����¶ȵ���������
	  unsigned char TD;       //�����¶ȵ�С������
	
	uchar status;
	uchar fifo_s;
uchar i;
	
	uchar TxBuf[32];
	

	
v_DelayMs_f(100);
v_Lcd12864Init_f() ;
v_Lcd12864SendCmd_f(0x01) ;
v_DelayMs_f(2);
v_Lcd12864PutString_f(0,0,"nRF24L01�������");
v_Lcd12864PutString_f(0,1,"STA:");
v_Lcd12864PutString_f(0,2,"FIF:");
init_NRF24L01();
i=1;
	

	while(1)
{
	flag=0;
		ReadyReadTemp();     //���¶�׼��

	    TL=ReadOneChar();    //�ȶ������¶�ֵ��λ
		TH=ReadOneChar();    //���Ŷ������¶�ֵ��λ
	if((TH&0xf8)!=0x00)//�жϸ���λ �õ��¶�������־
		{
		flag=1;
		TL=~TL;	 //ȡ��
		TH=~TH;	  //ȡ��
		tltemp=TL+1; //��λ��1
		TL=tltemp;
		if(tltemp>255) TH++;  //�����8λ����255�����8λ��1
	   TN=TH*16+TL/16;      //ʵ���¶�ֵ=(TH*256+TL)/16,����TH*16+TL/16
				                  //�����ó������¶ȵ���������,С�����ֱ�������
	   TD=(TL%16)*10/16;    //�����¶ȵ�С������,����������10�ٳ���16ȡ����
		}
		TN=TH*16+TL/16;      //ʵ���¶�ֵ=(TH*256+TL)/16,����TH*16+TL/16
				                  //�����ó������¶ȵ���������,С�����ֱ�������
	   TD=(TL%16)*10/16;    //�����¶ȵ�С������,����������10�ٳ���16ȡ����
			                  //�����õ������¶�С�����ֵĵ�һλ����(����1λС��)
	
	TxBuf[0]=TN;
	TxBuf[1]=TD;
  TxBuf[2]=i;		
	v_Lcd12864PutString_f(0,3,uchartostr(TN));	
	v_Lcd12864PutString_f(1,3,".");	
		v_Lcd12864PutString_f(2,3,uchartostr(TD));
v_Lcd12864PutString_f(3,3,"��");			
		
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
	v_Lcd12864PutString_f(6,1,"TX_N");//���ʹ���
	v_Lcd12864PutString_f(6,2,uchartostr(i));
	v_DelayMs_f(1000);
	v_Lcd12864PutString_f(2,1,"        ");
	v_Lcd12864PutString_f(2,2,"        ");
	i++;
	if(i==255) i=1;
	v_DelayMs_f(600);
	}
}
