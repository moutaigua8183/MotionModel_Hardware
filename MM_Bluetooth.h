#ifndef _MM_BLUETOOTH_H
#define _MM_BLUETOOTH_H

void SendByte(unsigned char dat);	//���͵��ַ�
void Init_UART();				 	//���ڳ�ʼ��
void Send2PhoneMotion(int now, int orig);	//���͸��ֻ�
void Send2PhoneSteps(int a);	//���͸��ֻ�
void SENDDATA( int flag, int temp_data );

#endif