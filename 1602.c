#include"1602.h"
#include <reg52.h>	
#include <intrins.h>
#include <MM_Bluetooth.h>


sbit LCD_RS = P2^6;  //��λ��            
sbit LCD_RW = P2^5;	 //д���ݶ�
sbit LCD_EP = P2^7;	 //ʹ�ܶ�


/****************��ʱ����************************/	
void delay1602(int ms)
{                   
	int i;
	while(ms--)
	{
	  for(i = 0; i< 250; i++)
	  {
	   _nop_();	  //��ִ��
	   _nop_();
	   _nop_();
	   _nop_();
	  }
	}
}

/****************��æ����************************/		
BOOL lcd_bz()
{                         
	BOOL result;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result = (BOOL)(P0 & 0x80);	 //���P0���λ�Ƿ�Ϊ1
	LCD_EP = 0;
	return result;//���ز�æ���
}

/****************д�����************************/
void lcd_wcmd_8bit(int cmd)
{                          // д��ָ�����ݵ�LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //����4λλָ��ͨ��P0�ڴ���1602
	_nop_();
	_nop_(); //���ڲ���һ��������
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
}	
		
void lcd_wcmd(int cmd)
{                          // д��ָ�����ݵ�LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //����4λλָ��ͨ��P0�ڴ���1602
	_nop_();
	_nop_(); //���ڲ���һ��������
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
	P0 = (cmd&0x0f)<<4; //����4λָ��ͨ��P0�ڴ���1602
	_nop_();
	_nop_(); //���ڲ���һ��������
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
}
	
/***********λ��****************/
void lcd_pos(BYTE pos)
{                          //�趨��ʾλ��
	lcd_wcmd(pos | 0x80);
}

/****************д���ݺ���************************/	
void lcd_wdat(BYTE dat) 
{                   //д���ַ���ʾ���ݵ�LCD
	while(lcd_bz());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EP = 0;
	P0 = dat ;  //����4λ����ͨ��P0�ڴ���1602
	_nop_();
	_nop_();   //���ڲ���һ��������
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0; 

	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EP = 0;
		_nop_();
	_nop_();
	_nop_();
	_nop_();
	P0 = (dat&0x0f)<<4;  //����4λλ����ͨ��P0�ڴ���1602
	_nop_();
	_nop_();   //���ڲ���һ��������
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0; 
}
	
void lcd_init()					//LCD��ʼ���趨
{ 
    lcd_wcmd_8bit(0x38);                       //16*2��ʾ��5*7����8λ����
	delay1602(1);  
	lcd_wcmd_8bit(0x38);                       //16*2��ʾ��5*7����8λ����
	delay1602(1);  
	lcd_wcmd_8bit(0x28);     //16*2��ʾ��5*7����8λ����
	delay1602(1);  
	lcd_wcmd(0x28);
	delay1602(1);
  	lcd_wcmd(0x0c);     //��ʾ�����ع��
	delay1602(1);
   	lcd_wcmd(0x02);     //��ʾ�����ع��
	delay1602(1);
   	lcd_wcmd(0x01);     //���LCD����ʾ����
	delay1602(1);

/*	lcd_wcmd(0x38);     //16*2��ʾ��5*7����8λ����
	delay(1);
	lcd_wcmd(0x08);     //��ʾ�����ع��
	delay(1);
//	lcd_wcmd(0x04);     //�ƶ����
	delay(1);
	lcd_wcmd(0x02);     // 4λѡ��
	delay(1);
	lcd_wcmd(0x01);     //���LCD����ʾ����
	delay(1); */
}


/***********��ʾ�������**************/
void lcd_show_data(BYTE a[8], BYTE b[8], BYTE c[8])
{
	lcd_init();
	delay1602(10);
	lcd_wcmd( 0 | 0x80 );
	lcd_wdat('x');
	lcd_wdat(':');
	lcd_wdat(a[0]);
	lcd_wdat(a[1]);
	lcd_wdat(a[2]);
	lcd_wdat(a[3]);
	lcd_wdat(a[4]);
	lcd_wdat(a[5]);
	lcd_wcmd( 0x40 | 0x80 );
	lcd_wdat('y');
	lcd_wdat(':');
	lcd_wdat(b[0]);
	lcd_wdat(b[1]);
	lcd_wdat(b[2]);
	lcd_wdat(b[3]);
	lcd_wdat(b[4]);
	lcd_wdat(b[5]);
	lcd_wcmd( 14 | 0x80 );
	lcd_wdat('z');
	lcd_wdat(':');
	lcd_wcmd( 0x4a | 0x80 );
	lcd_wdat(c[0]);
	lcd_wdat(c[1]);
	lcd_wdat(c[2]);
	lcd_wdat(c[3]);
	lcd_wdat(c[4]);
	lcd_wdat(c[5]);
}


/***********��ʾ����**************/
void lcd_show_int(int a)
{
	unsigned char wan,qian,bai,shi,ge;
	int temp_data;
	unsigned char X[8];
	
	temp_data = a;
	if(a<0)
	{
		temp_data=-temp_data;
		X[0]='-';//��ʾ������λ
	}
	else 
		X[0]='+';  //��ʾ�ո�
	wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 
	X[1]=wan; X[2]=qian; X[3]=bai; X[4]=shi;X[5]=ge; X[6]='\0';	
	lcd_init();
	delay1602(10);
	lcd_wcmd( 0 | 0x80 );
	lcd_wdat(X[0]);
	lcd_wdat(X[1]);
	lcd_wdat(X[2]);
	lcd_wdat(X[3]);
	lcd_wdat(X[4]);
	lcd_wdat(X[5]);
//	SendByte(':');
//	SendByte(X[0]);
//	SendByte(X[3]);
//	SendByte(X[4]);
//	SendByte(X[5]);
//	SendByte(' ');
}
