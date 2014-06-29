#ifndef _1602_H
#define _1602_H

typedef unsigned char BYTE;	//用BYTE代替unsigned char 
typedef bit BOOL;  // 用BOOL代替bit 
	


void delay1602(int ms);
BOOL lcd_bz();
void lcd_wcmd(int cmd);
void lcd_pos(BYTE pos);
void lcd_wdat(BYTE dat);
void lcd_init();
void lcd_show_data(BYTE* a, BYTE* b, BYTE* c);
void lcd_show_int(int );


#endif