#ifndef _1602_H
#define _1602_H

typedef unsigned char BYTE;	//��BYTE����unsigned char 
typedef bit BOOL;  // ��BOOL����bit 
	


void delay1602(int ms);
BOOL lcd_bz();
void lcd_wcmd(int cmd);
void lcd_pos(BYTE pos);
void lcd_wdat(BYTE dat);
void lcd_init();
void lcd_show_data(BYTE* a, BYTE* b, BYTE* c);
void lcd_show_int(int );


#endif