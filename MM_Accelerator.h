#ifndef _MM_ACCELERATOR_H
#define _MM_ACCELERATOR_H


void Init_ADXL345(void);             	//初始化ADXL345
void Write2ADXL345(unsigned char REG_Address,unsigned char REG_data);  //向ADXL345输入内容
unsigned char ReadADXL345(unsigned char REG_Address);                   //从ADXL345输出数据
void delay(unsigned int k);				//延迟kms
void ADXL345_Start();					//传输起始条件
void ADXL345_Stop();					//传输终止条件
void ADXL345_SendACK(bit ack);			//发送应答信号
bit  ADXL345_RecvACK();					//接受应答信号
void ADXL345_SendByte(char dat);		//ADXL345发送一个字节的数据
char ADXL345_RecvByte();				//ADXL345接收一个字节的数据
void GetAll(int* x_axis, int* y_axis, int* z_axis); //获得3轴数据


#endif