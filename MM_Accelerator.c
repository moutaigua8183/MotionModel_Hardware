#include <MM_Accelerator.h>
#include <reg52.h>
#include "intrins.h"

#define	SlaveAddress   0xA6	 	//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                            	//ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A
sbit SCL=P1^0;					//I2C时钟引脚定义
sbit SDA=P1^1;					//I2C数据引脚定义
int OFFSETX = 15;
int OFFSETY = -5;
int OFFSETZ = 5;



/***********延迟k毫秒****************/
void delay(unsigned int k)	
{						
	unsigned int idata i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<112;j++);
	}						
}
				

/*************起始信号***************/
void ADXL345_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    SDA = 0;                    //产生下降沿
    SCL = 0;                    //拉低时钟线
}

/*************停止信号***************/
void ADXL345_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    SDA = 1;                    //产生上升沿
}

/****发送应答信号ack (0:ACK 1:NAK)****/
void ADXL345_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    SCL = 0;                    //拉低时钟线
}

/***********接收应答信号*************/
bit ADXL345_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线

    return CY;
}

/*****向I2C总线发送一个字节数据*****/
void ADXL345_SendByte(char dat)
{
    unsigned int idata i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        SCL = 0;                //拉低时钟线
    }
    ADXL345_RecvACK();
}

/*****从I2C总线接收一个字节数据*****/
char ADXL345_RecvByte()
{
    unsigned int idata i;
    char dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
    }
    return dat;
}

/***********向ADXL345输入内容*************/
void Write2ADXL345(unsigned char REG_Address,unsigned char REG_data)
{
    ADXL345_Start();                  //起始信号
    ADXL345_SendByte(SlaveAddress);   //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);    //内部寄存器地址
    ADXL345_SendByte(REG_data);       //内部寄存器数据
    ADXL345_Stop();                   //发送停止信号
}

/************从ADXL345输出数据**************/
unsigned char ReadADXL345(unsigned char REG_Address)
{  
	unsigned char REG_data;

    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);            //发送存储单元地址，从0开始	
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=ADXL345_RecvByte();              //读出寄存器数据
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //停止信号
    return REG_data; 
}

/***********初始化ADXL345****************/
void Init_ADXL345()
{  
   Write2ADXL345(0x1E,0x00);   //X轴偏移量清零
   Write2ADXL345(0x1F,0x00);   //Y轴偏移量清零
   Write2ADXL345(0x20,0x00);   //Z轴偏移量清零
   //Write2ADXL345(0x24,0x64);   //Z轴活动中断触发幅度100
   //Write2ADXL345(0x25,0x2D);   //静止中断触发幅度45
   //Write2ADXL345(0x27,0x9A);   //使能Z轴活动中断和xy轴静止中断都由幅度触发
   Write2ADXL345(0x2C,0x09);   //速率设定为50Hz 参考pdf13页	   [100Hz is 0x0a]	 [50Hz is 0x09]
   Write2ADXL345(0x2D,0x08);   //选择电源模式   参考pdf24页
   Write2ADXL345(0x2E,0x80);   //使能 DATA_READY中断、           不开 Activity中断和Inactivity中断
   //Write2ADXL345(0x2F,0x08);   //活动中断（摔倒）到INT1引脚，静止中断（站立）到INT2引脚
   //Write2ADXL345(0x30,0x18);   //设置Activity和Inactivity有中断事件
   Write2ADXL345(0x31,0x2B);   //外部中断：低电平有效。测量范围：正负16g，13位模式
}

/***********************数据接收****************************/
void GetAll(int* x_axis, int* y_axis, int* z_axis)
{
	unsigned char idata Low,High;           //显示变量
	int  int_data;                      				 //变量
	//Data_X
    Low = ReadADXL345(0x32);
	High = ReadADXL345(0x33);
	int_data=High;
	int_data=(int_data<<8)|Low;    //合成数据   
	*x_axis=int_data + OFFSETX;
	//Data_Y
    Low = ReadADXL345(0x34);
	High = ReadADXL345(0x35);
	int_data=(High<<8)|Low;    //合成数据   
	*y_axis=int_data + OFFSETY;
	//Data_Z
    Low = ReadADXL345(0x36);
	High = ReadADXL345(0x37);
	int_data=(High<<8)|Low;		//合成数据
	*z_axis=int_data + OFFSETZ;   
}

