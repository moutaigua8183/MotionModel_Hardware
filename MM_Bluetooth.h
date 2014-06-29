#ifndef _MM_BLUETOOTH_H
#define _MM_BLUETOOTH_H

void SendByte(unsigned char dat);	//发送单字符
void Init_UART();				 	//串口初始化
void Send2PhoneMotion(int now, int orig);	//发送给手机
void Send2PhoneSteps(int a);	//发送给手机
void SENDDATA( int flag, int temp_data );

#endif