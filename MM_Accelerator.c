#include <MM_Accelerator.h>
#include <reg52.h>
#include "intrins.h"

#define	SlaveAddress   0xA6	 	//����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                            	//ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A
sbit SCL=P1^0;					//I2Cʱ�����Ŷ���
sbit SDA=P1^1;					//I2C�������Ŷ���
int OFFSETX = 15;
int OFFSETY = -5;
int OFFSETZ = 5;



/***********�ӳ�k����****************/
void delay(unsigned int k)	
{						
	unsigned int idata i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<112;j++);
	}						
}
				

/*************��ʼ�ź�***************/
void ADXL345_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    SDA = 0;                    //�����½���
    SCL = 0;                    //����ʱ����
}

/*************ֹͣ�ź�***************/
void ADXL345_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    SDA = 1;                    //����������
}

/****����Ӧ���ź�ack (0:ACK 1:NAK)****/
void ADXL345_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    SCL = 0;                    //����ʱ����
}

/***********����Ӧ���ź�*************/
bit ADXL345_RecvACK()
{
    SCL = 1;                    //����ʱ����
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����

    return CY;
}

/*****��I2C���߷���һ���ֽ�����*****/
void ADXL345_SendByte(char dat)
{
    unsigned int idata i;
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        SCL = 0;                //����ʱ����
    }
    ADXL345_RecvACK();
}

/*****��I2C���߽���һ���ֽ�����*****/
char ADXL345_RecvByte()
{
    unsigned int idata i;
    char dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
    }
    return dat;
}

/***********��ADXL345��������*************/
void Write2ADXL345(unsigned char REG_Address,unsigned char REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ�������
    ADXL345_Stop();                   //����ֹͣ�ź�
}

/************��ADXL345�������**************/
unsigned char ReadADXL345(unsigned char REG_Address)
{  
	unsigned char REG_data;

    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}

/***********��ʼ��ADXL345****************/
void Init_ADXL345()
{  
   Write2ADXL345(0x1E,0x00);   //X��ƫ��������
   Write2ADXL345(0x1F,0x00);   //Y��ƫ��������
   Write2ADXL345(0x20,0x00);   //Z��ƫ��������
   //Write2ADXL345(0x24,0x64);   //Z���жϴ�������100
   //Write2ADXL345(0x25,0x2D);   //��ֹ�жϴ�������45
   //Write2ADXL345(0x27,0x9A);   //ʹ��Z���жϺ�xy�ᾲֹ�ж϶��ɷ��ȴ���
   Write2ADXL345(0x2C,0x09);   //�����趨Ϊ50Hz �ο�pdf13ҳ	   [100Hz is 0x0a]	 [50Hz is 0x09]
   Write2ADXL345(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
   Write2ADXL345(0x2E,0x80);   //ʹ�� DATA_READY�жϡ�           ���� Activity�жϺ�Inactivity�ж�
   //Write2ADXL345(0x2F,0x08);   //��жϣ�ˤ������INT1���ţ���ֹ�жϣ�վ������INT2����
   //Write2ADXL345(0x30,0x18);   //����Activity��Inactivity���ж��¼�
   Write2ADXL345(0x31,0x2B);   //�ⲿ�жϣ��͵�ƽ��Ч��������Χ������16g��13λģʽ
}

/***********************���ݽ���****************************/
void GetAll(int* x_axis, int* y_axis, int* z_axis)
{
	unsigned char idata Low,High;           //��ʾ����
	int  int_data;                      				 //����
	//Data_X
    Low = ReadADXL345(0x32);
	High = ReadADXL345(0x33);
	int_data=High;
	int_data=(int_data<<8)|Low;    //�ϳ�����   
	*x_axis=int_data + OFFSETX;
	//Data_Y
    Low = ReadADXL345(0x34);
	High = ReadADXL345(0x35);
	int_data=(High<<8)|Low;    //�ϳ�����   
	*y_axis=int_data + OFFSETY;
	//Data_Z
    Low = ReadADXL345(0x36);
	High = ReadADXL345(0x37);
	int_data=(High<<8)|Low;		//�ϳ�����
	*z_axis=int_data + OFFSETZ;   
}

