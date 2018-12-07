//������ģ����ʾ����
#include <reg52.h>     //����һ��52��׼�ں˵�ͷ�ļ�
#include<intrins.h>  //����_nop_()���������ͷ�ļ�
#define uchar unsigned char //����һ�·���ʹ��
#define uint  unsigned int
#define ulong unsigned long
sbit Tx  = P3^7; //������������
sbit Rx  = P3^2; //�ز�����
sbit KEY =P3^3;//����
sbit BEEP =P1^0;//����
sbit RS=P2^5;    //�Ĵ���ѡ��λ����RSλ����ΪP2.0����
sbit RW=P2^6;    //��дѡ��λ����RWλ����ΪP2.1����
sbit E=P2^7;     //ʹ���ź�λ����Eλ����ΪP2.2����
sbit BF=P0^7;    //æµ��־λ������BFλ����ΪP0.7����
//unsigned char code string[ ]= {"CHAO SHENG BO"}; 
unsigned char code digit[ ]={"0123456789"}; //�����ַ�������ʾ����
uint distance[4];  //�����ջ�����
uchar ge,shi,bai,qian,temp,flag,outcomeH,outcomeL,i,j;  //�Զ���Ĵ���
bit succeed_flag;  //�����ɹ���־
//********��������
void conversion(uint temp_data);
void delay_20us();
void pai_xu();


/*****************************************************
�������ܣ���ʱ1ms
(3j+2)*i=(3��33+2)��10=1010(΢��)��������Ϊ��1����
***************************************************/
void delay1ms()
{
   unsigned char i,j;	
	 for(i=0;i<10;i++)
	  for(j=0;j<33;j++)
	   ;		 
 }
/*****************************************************
�������ܣ���ʱ���ɺ���
��ڲ�����n
***************************************************/
 void delay(unsigned char n)
 {
   unsigned char i;
	for(i=0;i<n;i++)
	   delay1ms();
 }
  
/*****************************************************
�������ܣ��ж�Һ��ģ���æµ״̬
����ֵ��result��result=1��æµ;result=0����æ
***************************************************/
 unsigned char BusyTest(void)
  {
    bit result;
	RS=0;       //���ݹ涨��RSΪ�͵�ƽ��RWΪ�ߵ�ƽʱ�����Զ�״̬
    RW=1;
    E=1;        //E=1���������д
    _nop_();   //�ղ���
    _nop_();
    _nop_(); 
    _nop_();   //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��	
    result=BF;  //��æµ��־��ƽ����result
   E=0;         //��E�ָ��͵�ƽ
    return result;
  }
/*****************************************************
�������ܣ���ģʽ����ָ�����ʾ��ַд��Һ��ģ��
��ڲ�����dictate
***************************************************/
void WriteInstruction (unsigned char dictate)
{   
    while(BusyTest()==1); //���æ�͵ȴ�
	 RS=0;                  //���ݹ涨��RS��R/WͬʱΪ�͵�ƽʱ������д��ָ��
	 RW=0;   
	 E=0;                   //E�õ͵�ƽ(���ݱ�8-6��дָ��ʱ��EΪ�����壬
                             // ������E��0��1���������䣬����Ӧ����"0"
	 _nop_();
	 _nop_();             //�ղ��������������ڣ���Ӳ����Ӧʱ��
	 P0=dictate;            //����������P0�ڣ���д��ָ����ַ
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	 E=1;                   //E�øߵ�ƽ
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	  E=0;                  //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
 }
/*****************************************************
�������ܣ�ָ���ַ���ʾ��ʵ�ʵ�ַ
��ڲ�����x
***************************************************/
 void WriteAddress(unsigned char x)
 {
     WriteInstruction(x|0x80); //��ʾλ�õ�ȷ�������涨Ϊ"80H+��ַ��x"
 }
/*****************************************************
�������ܣ�������(�ַ��ı�׼ASCII��)д��Һ��ģ��
��ڲ�����y(Ϊ�ַ�����)
***************************************************/
 void WriteData(unsigned char y)
 {
    while(BusyTest()==1);  
	  RS=1;           //RSΪ�ߵ�ƽ��RWΪ�͵�ƽʱ������д������
	  RW=0;
	  E=0;            //E�õ͵�ƽ(���ݱ�8-6��дָ��ʱ��EΪ�����壬
                       // ������E��0��1���������䣬����Ӧ����"0"
	  P0=y;           //����������P0�ڣ���������д��Һ��ģ��
	  _nop_();
	  _nop_();
 	  _nop_();
     _nop_();       //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	  E=1;          //E�øߵ�ƽ
	  _nop_();
	  _nop_();
	  _nop_();
	 _nop_();        //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	 E=0;            //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
 }
/*****************************************************
�������ܣ���LCD����ʾģʽ���г�ʼ������
***************************************************/
void LcdInitiate(void)
{
   delay(15);             //��ʱ15ms���״�дָ��ʱӦ��LCDһ�νϳ��ķ�Ӧʱ��
   WriteInstruction(0x38);  //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay(5);               //��ʱ5ms������Ӳ��һ�㷴Ӧʱ��
   WriteInstruction(0x38);
	delay(5);
	WriteInstruction(0x38); //�������Σ�ȷ����ʼ���ɹ�
	delay(5);
	WriteInstruction(0x0c);  //��ʾģʽ���ã���ʾ�����޹�꣬��겻��˸
	delay(5);
        WriteInstruction(0x06);  //��ʾģʽ���ã�������ƣ��ַ�����
	delay(5);
	WriteInstruction(0x01);  //����Ļָ�����ǰ����ʾ�������
	delay(5);
 }
 


void main(void)   // ������
{  uint distance_data,a,b;
   uchar CONT_1;  
//  uchar k;   //�������iָ���ַ�������Ԫ��
    LcdInitiate();         //����LCD��ʼ������  
   delay(10);             //��ʱ10ms����Ӳ��һ�㷴Ӧʱ��
	 WriteAddress(0x0b); 
	 WriteData('2');  
		  WriteData('0'); 
		  WriteData('3'); 
	 WriteData('1');  
		  WriteData('9'); 
		
	
//   WriteAddress(0x00);   // �ӵ�1�е�3�п�ʼ��ʾ
//   k = 0;               //ָ���ַ�����ĵ�1��Ԫ��
 //	while(string[k] != '\0')
//		{					
//			WriteData(string[k]);
//			k++;             //ָ�����ַ�����һ��Ԫ��				
//		}	 
   i=0;

   flag=0;
	Tx=0;       //��������������������
	TMOD=0x10;    //��ʱ��0����ʱ��1��16λ������ʽ
//	TR0=1;	     //������ʱ��0
   IT0=0;        //�ɸߵ�ƽ��͵�ƽ�������ⲿ�ж�
	//ET0=1;        //�򿪶�ʱ��0�ж�
	EX0=0;        //�ر��ⲿ�ж�
	EA=1;         //�����ж�0	
  
	
while(1)         //����ѭ��
	{
	 WriteAddress(0x00);    // �ӵ�2�е�6�п�ʼ��ʾ
// 	        WriteData('J');  
// 			 WriteData('U');  
// 			  WriteData('L');  
// 			   WriteData('I');  
// 			    WriteData(':'); 


		  WriteData(digit[qian]); 
		  WriteData(digit[bai]);  
			WriteData(digit[shi]);  
		  WriteData('.');  
		  WriteData(digit[ge]);  
		  WriteData('c'); 
		  WriteData('m'); 
		
       EA=0;
	     Tx=1;
        delay_20us();
        Tx=0;         //����һ��20us�����壬��Tx����  
        while(Rx==0); //�ȴ�Rx�ز����ű�ߵ�ƽ
	     succeed_flag=0; //������ɹ���־
	     EX0=1;          //���ⲿ�ж�
			 EX1=1;
	 	  TH1=0;          //��ʱ��1����
        TL1=0;          //��ʱ��1����
	     TF1=0;          //
        TR1=1;          //������ʱ��1
   EA=1;

      while(TH1 < 30);//�ȴ������Ľ��������65.535���루�����ж�ʵ�֣�  
		  TR1=0;          //�رն�ʱ��1
        EX0=0;          //�ر��ⲿ�ж�

    if(succeed_flag==1)
	     { 	
		   distance_data=outcomeH;                //��������ĸ�8λ
           distance_data<<=8;                   //����16λ�ĸ�8λ
		     distance_data=distance_data|outcomeL;//���8λ�ϲ���Ϊ16λ�������
            distance_data*=12;                  //��Ϊ��ʱ��Ĭ��Ϊ12��Ƶ
           distance_data/=58;                   //΢��ĵ�λ����58��������
         }                                      //Ϊʲô����58�������ף�  Y��=��X��*344��/2
			                                       // X��=�� 2*Y�ף�/344 ==��X��=0.0058*Y�� ==������=΢��/58 
    if(succeed_flag==0)
		   {
            distance_data=0;                    //û�лز�������

           }

           distance[i]=distance_data; //��������������ݷ��뻺����
            i++;
  	  	 if(i==3)
	  	     {
	  	       distance_data=(distance[0]+distance[1]+distance[2]+distance[3])/4;
              
            pai_xu();
             distance_data=distance[1];

      
	   a=distance_data;
       if(b==a) CONT_1=0;
       if(b!=a) CONT_1++;
       if(CONT_1>=3)
		   { CONT_1=0;
			  b=a;
			  conversion(b);
			}       
	  		 i=0;
 	  		}	     
	 }
}
//***************************************************************
//�ⲿ�ж�0�������жϻز���ƽ
INTO_()  interrupt 0   // �ⲿ�ж���0��
 {    
     outcomeH =TH1;    //ȡ����ʱ����ֵ
     outcomeL =TL1;    //ȡ����ʱ����ֵ
     succeed_flag=1;   //���ɹ������ı�־
     EX0=0;            //�ر��ⲿ�ж�
  }
	
//***************************************************************
//�ⲿ�ж�1��������¼����
INT1_()  interrupt 2   // �ⲿ�ж���2��
 {    
     delay(15);
	 if(KEY==0)
	 {
			while(!KEY);
		  
	
	if(j==2) j=0;
	if(j==0)	
		{ 
			WriteAddress(0x40); 
			WriteData(' ');
			WriteData(' ');
			WriteData(' ');
			WriteData(' ');
			WriteData(' ');		
			WriteData(' ');
			WriteData('c');
			WriteData('m');
						
			WriteAddress(0x40);
			WriteData(' ');	
			WriteData(digit[qian]);			
			WriteData(digit[bai]);  
		  WriteData(digit[shi]);  
		  WriteData('.');  
		  WriteData(digit[ge]); 
		}
	 
		if(j==1)	
		{ 
			
			WriteAddress(0x48); 		  
			WriteData(' ');
			WriteData(' ');
			WriteData(' ');
			WriteData(' ');		
			WriteData(' ');
			WriteData(' ');
			WriteData('c');
			WriteData('m');
			
			WriteAddress(0x49); 
			WriteData(digit[qian]); 
			WriteData(digit[bai]);  
		  WriteData(digit[shi]);  
		  WriteData('.');  
		  WriteData(digit[ge]); 
	
		}
	 
			j++;
	}
 }	
	
	
	
//****************************************************************
//��ʱ��0�ж�,������ʾ
timer0() interrupt 1  // ��ʱ��0�ж���1��
   {
 	// TH0=0xfd; //д�붨ʱ��0��ʼֵ
	// TL0=0x77;	 	
	
   }

//��ʾ����ת������
void conversion(uint temp_data)  
 {  
    uchar ge_data,shi_data,bai_data,qian_data ;
		qian_data=temp_data/1000;
	  temp_data=temp_data%1000; 
    bai_data=temp_data/100 ;
    temp_data=temp_data%100;   //ȡ������
    shi_data=temp_data/10 ;
    temp_data=temp_data%10;   //ȡ������
    ge_data=temp_data;

   

    EA=0;
	  qian=qian_data;
    bai = bai_data;
    shi = shi_data;
    ge  = ge_data ; 
	 
	 
	 
	 if(qian||bai||shi||ge!=0)
	 	BEEP=qian||bai;
	 EA=1;
 }
//******************************************************************


void delay_20us()
 {  uchar bt ;
    for(bt=0;bt<60;bt++);
 }
   void pai_xu()
  {  uint t;
  if (distance[0]>distance[1])
    {t=distance[0];distance[0]=distance[1];distance[1]=t;} 
  if(distance[0]>distance[2])
    {t=distance[2];distance[2]=distance[0];distance[0]=t;} 
  if(distance[1]>distance[2])
    {t=distance[1];distance[1]=distance[2];distance[2]=t;}  
    }


