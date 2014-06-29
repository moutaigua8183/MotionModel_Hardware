#include <reg52.h>
#include <MM_Bluetooth.h>
#include <MM_Accelerator.h>



//发送单字符
void SendByte(unsigned char dat)
{
	SBUF = dat;
	ES = 0;
 	while(!TI);
    TI = 0;				 //TI为0时，准备发送；为1时，发送完毕
	ES = 1;
}


//串口初始化
void Init_UART()
{
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 禁止接受
	PCON  = 0x00;   			//不加倍
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
	TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz
	TL1   = 0xFD;  
    TR1   = 1;                  // TR1:  timer 1 打开                           
    EA    = 1;                  //打开总中断
   	ES    = 1;                  //打开串口中断
}  			 

//发送函数
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



/*********用于测试*********/
void SENDDATA( int flag, int temp_data )
{
	unsigned char wan, qian, bai, shi, ge;
	SendByte(0x77+flag);//x
	SendByte(0x3D);//=
	if(temp_data>0||temp_data==0)
		SendByte(0x2B);//+号
	else
	{
		SendByte(0x2D);//-号 
		temp_data=-temp_data;
	}
	wan=temp_data/10000+0x30;
    temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 
	SendByte(qian); 
	SendByte(bai); 
	SendByte(shi); 
	SendByte(ge); 
	SendByte(0x20);//换行\n
}