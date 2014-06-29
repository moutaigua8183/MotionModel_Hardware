#ifndef _MM_ACCELERATOR_H
#define _MM_ACCELERATOR_H


void Init_ADXL345(void);             	//��ʼ��ADXL345
void Write2ADXL345(unsigned char REG_Address,unsigned char REG_data);  //��ADXL345��������
unsigned char ReadADXL345(unsigned char REG_Address);                   //��ADXL345�������
void delay(unsigned int k);				//�ӳ�kms
void ADXL345_Start();					//������ʼ����
void ADXL345_Stop();					//������ֹ����
void ADXL345_SendACK(bit ack);			//����Ӧ���ź�
bit  ADXL345_RecvACK();					//����Ӧ���ź�
void ADXL345_SendByte(char dat);		//ADXL345����һ���ֽڵ�����
char ADXL345_RecvByte();				//ADXL345����һ���ֽڵ�����
void GetAll(int* x_axis, int* y_axis, int* z_axis); //���3������


#endif