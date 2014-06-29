#include <reg52.h>
#include <MM_Bluetooth.h>
#include <MM_Accelerator.h>



//���͵��ַ�
void SendByte(unsigned char dat)
{
	SBUF = dat;
	ES = 0;
 	while(!TI);
    TI = 0;				 //TIΪ0ʱ��׼�����ͣ�Ϊ1ʱ���������
	ES = 1;
}


//���ڳ�ʼ��
void Init_UART()
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ��ֹ����
	PCON  = 0x00;   			//���ӱ�
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
	TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz
	TL1   = 0xFD;  
    TR1   = 1;                  // TR1:  timer 1 ��                           
    EA    = 1;                  //�����ж�
   	ES    = 1;                  //�򿪴����ж�
}  			 

//���ͺ���
void Send2PhoneMotion(int now, int orig)
{
	if( now==orig )
		return;
	else
	{
		SendByte(0x30+now);
	}
}
void Send2PhoneSteps(int a)
{
	int idata i;
	if(0==a)
		return;
	else
	{
		for(i=0; i<(a%10); i++)
		{
			SendByte(0x30+a/10);
		}
	}
}



/*********���ڲ���*********/
void SENDDATA( int flag, int temp_data )
{
	unsigned char wan, qian, bai, shi, ge;
	SendByte(0x77+flag);//x
	SendByte(0x3D);//=
	if(temp_data>0||temp_data==0)
		SendByte(0x2B);//+��
	else
	{
		SendByte(0x2D);//-�� 
		temp_data=-temp_data;
	}
	wan=temp_data/10000+0x30;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 
	SendByte(qian); 
	SendByte(bai); 
	SendByte(shi); 
	SendByte(ge); 
	SendByte(0x20);//����\n
}