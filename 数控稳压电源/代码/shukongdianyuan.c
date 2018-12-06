#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char

#define lcd12864_dataport P0	//P0Ϊ12864���ݿ�
#define keyboard_dataport P2	//P2Ϊ�������ݿ�
#define dac0832_dataport P1			//P1Ϊdac0832���ݿ�

sbit RS = P3^0 ;	//12864 ��������ѡ���
sbit RW = P3^1 ;	//12864 ��дѡ���
sbit EN = P3^4 ;	//12864	12864ʹ���ź�

sbit CLK=P3^5;
sbit DAT=P3^6;
sbit CS=P3^7;

uchar   h=0;

unsigned int t=0;



#define SET_DATA  RS = 1 ;		//д����
#define SET_INC  RS = 0 ;		//дָ��
#define SET_READ  RW = 1 ;		//��
#define SET_WRITE RW = 0 ;		//д
#define SET_EN    EN = 1 ;		//ʹ�ܿ�
#define CLR_EN    EN = 0 ;		//ʹ�ܹ�

bit keyboard_flag=0;			//���̴����־	0Ϊ����		1Ϊ������
uchar dac0832=100;						//������da����
uchar tlc549_data;						//������ad����
uchar e=0;						//����Ԥ���ѹ����������
uchar f=0;						//����Ԥ���ѹ��С������


uchar code figure[]="0123456789";

//���к�������
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
//		n������ʱ���� ��ڲ���:n
//*************************************
void delaynms(uchar n)
{
	uchar i;
	while(n--)
		for(i=120;i>0;i--);	
}


//******************************************************************************************************
//		��ѹ���㺯��		��ڲ���:data_16	 	 ���ڲ���:data_10
//******************************************************************************************************
uchar v_process_1(uchar data_16)
{
	uchar data_10;
	data_10=data_16*100/200;
	return data_10;
}

//******************************************************************************************************
//		Ԥ���ѹת�����ƺ���	��ڲ���:data_10	  ���ڲ���:data_16
//******************************************************************************************************
uchar v_process_2(uchar data_10)
{
	uchar data_16;
	data_16=data_10*200/100;
	return data_16;
}

/*
//**********************************************************
//		�������ɨ�����	���ڲ�������ֵnum ��Ӧ16������
//**********************************************************
uchar keyboard_scan()
{			
			uchar num,temp;
			//����һ��
			keyboard_dataport=0xfe;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)		//���а�������
				{
					delaynms(15);		//��ʱ����
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)	//ȷ���а�������
					{
						temp=keyboard_dataport;
					switch(temp)
						{
							case 0xee:num=1;		//��һ�е�һ��
								break;
							case 0xde:num=2;	//��һ�еڶ���
								break;
							case 0xbe:num=3;		//��һ�е�����
								break;
							case 0x7e:num=4;		//��һ�е�����
								break;
						}
					while(temp!=0xf0)				//�ȴ������ͷ�
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}
			//���ڶ���
			P3=0xfd;
			temp=keyboard_dataport;
			temp=temp&0xf0;
			while(temp!=0xf0)				//�а�������
				{
					delaynms(15);
					temp=keyboard_dataport;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=keyboard_dataport;
					switch(temp)
						{
							case 0xed:num=5;	//�ڶ��е�һ��
								break;
							case 0xdd:num=6;	//�ڶ��еڶ���
								break;
							case 0xbd:num=7;	//�ڶ��е�����
								break;
							case 0x7d:num=8;	//�ڶ��е�����
								break;
						}
					while(temp!=0xf0)			//�ȴ������ͷ�
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}

			//��������
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
					while(temp!=0xf0)		//�ȴ������ͷ�
						{
							temp=keyboard_dataport;
							temp=temp&0xf0;
						}
					}
				}

			//��������
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
					while(temp!=0xf0)			//�ȴ������ͷ�
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
//		�������ɨ�����	���ڲ�������ֵx ��Ӧ16������
//*******************************************************
uchar keyboard_scan(void)
{
	uchar x=0;			//��ֵx
	uchar temp=0;		//�Ĵ��������״̬
	
//	lcd12864putstring(0,2,"����ɨ��");
	
//	keyboard_dataport=0xff;		 	//�������ݿڽ����״̬
	
	keyboard_dataport=0Xf0;		
	temp=keyboard_dataport; 
	
	
	if(temp!=0xf0) 		//�а�������
	{ 
		delaynms(5);  		//������ʱ
		if(temp!=0xf0)
		{ 
			keyboard_dataport=0xfe;		//��������
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xee):{x=16;break; }	//�����е��ĸ���
				case(0xde):{x=12;break; 	}//�����е�������
				case(0xbe):{x=8;break; 	}//�����еڶ�����
				case(0x7e):{x=4;break; 	}//�����е�һ����

			}

			keyboard_dataport=0xfd;	//��������
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xed):{x=15;break; }
				case(0xdd):{x=11;break; }
				case(0xbd):{x=7;break; }
				case(0x7d):{x=3;break; }
			}

			keyboard_dataport=0xfb;	//���ڶ���
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xeb):{x=14;break; }
				case(0xdb):{x=10;break; }
				case(0xbb):{x=6;break; }
				case(0x7b):{x=2;
//				while(temp!=0xf0)				//�ȴ������ͷ�
//		{
//					temp=keyboard_dataport;
//					temp=temp&0xf0;
//			}

				break; }
			}

			keyboard_dataport=0xf7;	//����һ��
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0xe7):{x=13;break; }
				case(0xd7):{x=9;break; }
				case(0xb7):{x=5;;break; }
				case(0x77):{x=1;
		/*	while(temp!=0xf0)				//�ȴ������ͷ�
			{
					temp=keyboard_dataport;
					temp=temp&0xf0;
			}
		*/
			break; }
			}
		} 
		while(temp!=0xf0)				//�ȴ������ͷ�
			{
					temp=keyboard_dataport;
					temp=temp&0xf0;
			} 
	
   } 
   
 /*  if(temp!=0xf0) 		//�а�������
	{ 
		delaynms(3);  		//������ʱ
		if(temp!=0xf0)
		{ 
		

			keyboard_dataport=0xfb;	//���ڶ���
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				
				case(0x7b):{x=2;break; }
			}

			keyboard_dataport=0xf7;	//����һ��
			temp=keyboard_dataport;
			switch(temp) 
			{ 
				case(0x77):{x=1;break; }
			}
		} 
		while(temp!=0xf0)				//�ȴ������ͷ�
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
//		������̴������	��ڲ�����������ֵx
//*******************************************************
void keyboard_process(uchar x)
{
//	lcd12864putstring(0,3,"���̴���");
	switch(x) 
	{ 
		case(1):
		{
			if(dac0832>=198)
			break;
			dac0832=dac0832+20;
			break; 
		}			//������ʱ��0 �����Լ� ���߹رն�ʱ��0 ֹͣ�Լ�
		case(2):
			{
			if(dac0832<=0x01)
			break;
			dac0832=dac0832-20;
			break; 
			}			   //������ʱ��1 �����Լ� ���߹رն�ʱ��1 ֹͣ�Լ�
		case(3):
			{if(dac0832>=198)
			break;
			dac0832=dac0832+2;
			break; 
			}				//	��ѹֵ+0.1
		case(4):
			{
			if(dac0832<=01)
			break;
			dac0832=dac0832-2;
			break; 
			} 				//	��ѹֵ-0.1
	
		case(5):{TR0=~TR0 ;if(dac0832>=198) dac0832=198;
			break; 	}
		case(6): {TR1=~TR1;if(dac0832<=01) dac0832=01;break; 	}
	//	case(7): ;break; 	
//		case(8):							//��������״̬
//			while (! yushe_process_1()){;}
//			while (! yushe_process_2()){;}
//			break; 	
	//	case(9): ;break; 	
	//	case(10): ;break; 	
	//	case(11): ;break; 	
//		case(12):dac0832=v_process_2(e*10+f);break; 					//ȷ����ť
	//	case(13): ;break; 	
	//	case(14): ;break; 	
	//	case(15):x=8;break; 	
	//	case(16):x=4;break; 	

	}
	

}

//*********************************************************
//		Ԥ���ѹ����λ�������	���ڲ���:flag ������ɱ�־
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
//		Ԥ���ѹС��λλ�������	���ڲ���:flag ������ɱ�־
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
//		12864���ä�źź���
//*******************************************************

void lcd12864checkbusy( void )      
{
    unsigned int nTimeOut = 0 ;
    SET_INC
    SET_READ
    CLR_EN
    SET_EN
    while( ( lcd12864_dataport & 0x80 ) && ( ++nTimeOut != 0 ) ) ; //���λΪ0ʱ������
    CLR_EN
    SET_INC
    SET_READ
}

//*******************************************************
//		12864���������	��ڲ�����byCmd ��������룩 
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
//		12864�����źź���	��ڲ�����byData�����ݴ��룩 
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
//		12864��ʼ������	
//*******************************************************
void lcd12864init( void )                  //��ʼ��
{
    lcd12864sendcmd( 0x30 ) ;              //����ָ�  �����趨 8λ���� ������ָ��
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x01 ) ;              //����
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x06 ) ;              //�������
    delaynms( 50 ) ;
    lcd12864sendcmd( 0x0c ) ;              //����ʾ ���α�
}

//*******************************************************************
//		12864��ַ���ú���	��ڲ�����x,y	x��������y������ ��8��4��
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
//		12864�ַ�����ʾ����	��ڲ�����x,y��*pData	x��������y������ ��8��4�� *pDataָ���ַ���
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
//		12864�ַ���ʾ����	��ڲ�����x,y��asiic	x��������y������ ��8��4�� asiic��Ҫд���ַ���asiic��
//***********************************************************************************************
void lcd12864putchar( unsigned char x, unsigned char y, unsigned char asic )
{
    lcd12864setaddress( x, y ) ;
    
    lcd12864senddata( asic ) ;
}


//***********************************************************************************************
//		12864������ʾ����	������ʾ����˵��������
//***********************************************************************************************
void lcd12864display_1(void)
{
		lcd12864putstring(0,0,"�����ѹ:");
		 delaynms( 300 ) ;
		lcd12864putstring(0,1,"ʵ�ʵ�ѹ:");
		 delaynms( 300 ) ;
}

//******************************************************************************************************
//		12864������ʾ����	������ʾ�����ѹ��ʵ�ʵ�ѹ����ֵ 	��ڲ���:x,y	xΪ���������ѹ yΪʵ�ʲ�����ѹ
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
//��ad����		���ڲ�����16����ad���ֵ
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
		tmp<<=1; 		//����һλ
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
//			�������ܣ�������
//*******************************************
void main(void)
{
	dac0832_dataport=dac0832;
	
	
	lcd12864init();				//12864��ʼ��
	lcd12864display_1();		//12864��ʾ��̬����
	
	
	EX0=1;     					//����ʹ�����ж�0
//	IT0=1;    					//ѡ���������������ж�0
	
	EX1=1;     					//����ʹ�����ж�1
	IT1=1;    					//ѡ���������������ж�1
	
	
	//��ʱ��0��1����		���ڿ��������ѹ�Զ��ӻ����Զ���
	TMOD=0x11;            //ʹ�ö�ʱ��T0 T1��ģʽ1	16λ

	TH0=(65536-50)/256;  //��ʱ��T0�ĸ�8λ����ֵ
	TL0=(65536-50)%256;  //��ʱ��T0�ĵ�8λ����ֵ	��ʱ50us
	ET0=1;						//����ʱ��0�ж�
	TR0=0;					//�رն�ʱ��0
	
	TH1=(65536-50)/256;  //��ʱ��T1�ĸ�8λ����ֵ
	TL1=(65536-50)%256;  //��ʱ��T1�ĵ�8λ����ֵ	��ʱ50us
	ET1=1;						//����ʱ��1�ж�
	TR1=0;					//�رն�ʱ��1
	
/*	
	//��ʱ��2����	���ڿ���ad�Ĳ���Ƶ��	
	RCAP2H=(65536-50000)/256;;				//��ʱ��2��ֵ  ��ʱ50000us		16λ�Զ���װ
	RCAP2L=(65536-50000)%256;				//��ʱ��2��ֵ  ��ʱ50000us
	ET2=1;     					// ����T2��ʱ���ж�
	TR2=1;     					// ����T2��ʱ��
//	lcd12864putstring(0,2,"��ʱ���ж��ѿ�");
	TH2=RCAP2H;					//��ʱ��2����ֵ  ��ʱ50us
	TL2=RCAP2L;					//��ʱ��2����ֵ  ��ʱ50us
	
*/

	EA=1;      					//�������ж�
	
	keyboard_dataport=0x0f;
	
	

	while (1)
	{
		if(keyboard_flag==1)		//��������̲���
		{
	
			keyboard_flag=0;		//ȡ�����������־
			keyboard_process(keyboard_scan());	//���м��̰�������
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
//			�������ܣ����ж�T0���жϷ������	���ڽ��й�������
//***********************************************************************
void int0(void) interrupt 0 //���ж�0���жϱ��Ϊ0
{
	EX0=0;     			//�ر����ж�0
	
	dac0832=0;
	lcd12864putstring(0,2,"��������");
	delaynms( 1000 ) ;
	lcd12864putstring(0,3,"�ѽ����Զ���ѹ");
	delaynms( 1000 ) ;
	lcd12864putstring(0,2,"                                 ");
	lcd12864putstring(0,3,"                                 ");
	
	EX0=1;     			//�����ж�0
}
*/

//***********************************************************************
//			�������ܣ����ж�T1���жϷ������	���ڼ����̰����Ƿ���
//***********************************************************************
void int1(void) interrupt 2 //���ж�1���жϱ��Ϊ2
{
	EX1=0;     			//�ر����ж�0
	keyboard_flag=1;	//���̴����־��1
	EX1=1;     			//�����ж�0
	

}


//***************************************************************
//			�������ܣ���ʱ��0���жϷ������	���������ѹֵ�Զ���	
//***************************************************************
void timer0(void) interrupt 1 
{ 
	static uchar    m=0;
//	static uchar	n=0;	
	TR0=0;					//�رն�ʱ��0
	m++;
	if (m==2)				//50us*20=1ms
	{

	  dac0832=dac0832+2; //�����ѹ�Լ�
		
		m=0;
//	  lcd12864display_2(dac0832,v_process_1(tlc549_data));
		
	}

//	if (n==1000)				//100*1ms=0.1s
//	{
		
//	    dac0832++; //�����ѹ�Լ�
//		lcd12864display_2(dac0832,v_process_1(tlc549_data));
//		n=0;
		
//	}

	
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	TR0=1;					//�򿪶�ʱ��0
	
}

//***************************************************************
//			�������ܣ���ʱ��1���жϷ������	���������ѹֵ�Զ���
//***************************************************************
void timer1(void) interrupt 3
{ 
	static uchar    p=0;
//	static uchar	q=0;	
	TR1=0;					//�رն�ʱ��1
	p++;
	if (p==2)				//50us*20=1ms
	{
	 
	 dac0832=dac0832-2;			//��ѹֵ�Զ�
		
		p=0;
//	 lcd12864display_2(dac0832,v_process_1(tlc549_data));
		
	
	}

//	if (q==1000)				//100*1ms=0.1s
//	{
		
//		dac0832--;			//��ѹֵ�Զ���
//		lcd12864display_2(dac0832,v_process_1(tlc549_data));
//		q=0;
//		
//	}

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	TR1=1;					//�򿪶�ʱ��1
	
}

/*
//***************************************************************
//			�������ܣ���ʱ��2���жϷ������	���ڿ���ad�Ĳ���Ƶ��	
//***************************************************************
void timer2() interrupt 5
{
	static uchar  d1=0;

//	static uchar  d2=0;;
//	lcd12864putstring(0,3,"�ѽ��붨ʱ��2");
	
	TF2=0;					//!!!ע��!!! ��ʱ��2����������������־λ���㣬Ӳ���������㣬�����붨ʱ��0�Ͷ�ʱ��1��ͬ!!!
	TR2=0;					//�رն�ʱ��2
	d1++;
	if (d1==10)				//50000us*10=0.5s
	{
	 d1=0;
	 tlc549_data=TLC549() ;	
	 lcd12864display_2(v_process_1(dac0832),v_process_1(tlc549_data));
	}
//	if (d2==500)				//500*1ms=0.5s
//	{
//		lcd12864putstring(0,3,"�ѽ��붨ʱ��2�ж�");
//		tlc549_data=TLC549() ;		//����		����Ƶ��  1��2��
//		d2=0;
//	}
	//��ʱ��2���Զ���װ		�����ٸ���ֵ
	TR2=1;					//�򿪶�ʱ��2
	
}
*/
