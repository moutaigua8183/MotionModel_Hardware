#include"1602.h"
#include <reg52.h>	
#include <intrins.h>
#include <MM_Bluetooth.h>


sbit LCD_RS = P2^6;  //复位端            
sbit LCD_RW = P2^5;	 //写数据端
sbit LCD_EP = P2^7;	 //使能端


/****************延时函数************************/	
void delay1602(int ms)
{                   
	int i;
	while(ms--)
	{
	  for(i = 0; i< 250; i++)
	  {
	   _nop_();	  //空执行
	   _nop_();
	   _nop_();
	   _nop_();
	  }
	}
}

/****************侧忙函数************************/		
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
	result = (BOOL)(P0 & 0x80);	 //检测P0最高位是否为1
	LCD_EP = 0;
	return result;//返回侧忙结果
}

/****************写命令函数************************/
void lcd_wcmd_8bit(int cmd)
{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //将高4位位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
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
{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //将高4位位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
	P0 = (cmd&0x0f)<<4; //将低4位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
}
	
/***********位置****************/
void lcd_pos(BYTE pos)
{                          //设定显示位置
	lcd_wcmd(pos | 0x80);
}

/****************写数据函数************************/	
void lcd_wdat(BYTE dat) 
{                   //写入字符显示数据到LCD
	while(lcd_bz());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EP = 0;
	P0 = dat ;  //将高4位数据通过P0口传给1602
	_nop_();
	_nop_();   //用于产生一个脉冲宽度
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
	P0 = (dat&0x0f)<<4;  //将低4位位数据通过P0口传给1602
	_nop_();
	_nop_();   //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0; 
}
	
void lcd_init()					//LCD初始化设定
{ 
    lcd_wcmd_8bit(0x38);                       //16*2显示，5*7点阵，8位数据
	delay1602(1);  
	lcd_wcmd_8bit(0x38);                       //16*2显示，5*7点阵，8位数据
	delay1602(1);  
	lcd_wcmd_8bit(0x28);     //16*2显示，5*7点阵，8位数据
	delay1602(1);  
	lcd_wcmd(0x28);
	delay1602(1);
  	lcd_wcmd(0x0c);     //显示开，关光标
	delay1602(1);
   	lcd_wcmd(0x02);     //显示开，关光标
	delay1602(1);
   	lcd_wcmd(0x01);     //清除LCD的显示内容
	delay1602(1);

/*	lcd_wcmd(0x38);     //16*2显示，5*7点阵，8位数据
	delay(1);
	lcd_wcmd(0x08);     //显示开，关光标
	delay(1);
//	lcd_wcmd(0x04);     //移动光标
	delay(1);
	lcd_wcmd(0x02);     // 4位选择
	delay(1);
	lcd_wcmd(0x01);     //清除LCD的显示内容
	delay(1); */
}


/***********显示各轴参数**************/
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


/***********显示数据**************/
void lcd_show_int(int a)
{
	unsigned char wan,qian,bai,shi,ge;
	int temp_data;
	unsigned char X[8];
	
	temp_data = a;
	if(a<0)
	{
		temp_data=-temp_data;
		X[0]='-';//显示负符号位
	}
	else 
		X[0]='+';  //显示空格
	wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
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
