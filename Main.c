#include <MM_Accelerator.h>
#include <MM_Bluetooth.h>
#include <MM_Model.h>
#include <reg52.h>
#include <math.h>

#define DATABASE_SIZE 70

unsigned int idata OPTION;
float idata A[3][3];				//转换矩阵
VECTOR DATABASE[DATABASE_SIZE];		//实时数据
STAT_SET idata SET[3];				//3轴数据的统计信息
int ZArray[DATABASE_SIZE];			//Z轴数据
int idata STATUS_PRE=-1, STATUS_NOW=-1,STATUS_HISTORY[3]={0};		//MotionDetection's
int temp=0, result=0, prestatus=0;//pedometer's

/*********主函数**********/
void main (void)
{
	delay(100);						//上电延时
	Init_ADXL345();                 //初始化ADXL345
	Init_UART();					//初始化串口通信
   	SpaceCreate(A);					//坐标系建立
	delay(200);
	OPTION=9;
	while( 1 )            			//循环
	{ 
		
		switch(OPTION)
		{
		case 1:
			STATUS_PRE=-1;
			STATUS_NOW=-1;
			STATUS_HISTORY[0]=0;
			STATUS_HISTORY[1]=0;
			STATUS_HISTORY[2]=0;
			OPTION=2;
		case 2:					  
			UpdateDatabase( DATABASE, A );
			AnalyzeDatabase( DATABASE, SET, ZArray);
			STATUS_HISTORY[0]=STATUS_HISTORY[1];
			STATUS_HISTORY[1]=STATUS_HISTORY[2];
			if ( isStill(SET) )
			{	
				STATUS_HISTORY[2]=StillStatus(SET);	
			}
			else
			{
				STATUS_HISTORY[2]=MovingStatus(ZArray);
			}
			if(STATUS_HISTORY[0]==STATUS_HISTORY[1]&&STATUS_HISTORY[2]==STATUS_HISTORY[1])
				STATUS_NOW=STATUS_HISTORY[2];
			else
				STATUS_NOW=STATUS_PRE;
			Send2PhoneMotion(STATUS_NOW, STATUS_PRE);
			STATUS_PRE=STATUS_NOW;
			break;
		case 3:
			temp=0;
			result=0;
			prestatus=0;
			OPTION=4;
		case 4:
			UpdateDatabase( DATABASE, A );
			AnalyzeDatabase( DATABASE, SET, ZArray);
			if ( isStill(SET) )
			{
				break;
			}
			result=Count( ZArray );
			if( prestatus==0 )
				prestatus=result/10;
			else if( prestatus!=result/10 )
			{
				temp=prestatus;
				prestatus=result/10;
				result=temp*10+result%10;
			}
			Send2PhoneSteps(result);
			break;
		case 9:
			break;
		}		
	}


}

/****串口中断程序****/
void UART_SER (void) interrupt 4 using 0//串行中断服务程序
{
   if(RI==1)                        
	{
		RI=0;                    
		OPTION=SBUF;
		if( OPTION=='1' )
			OPTION=1;
		else if (OPTION=='3')
			OPTION=3;
		else if (OPTION=='9')
			OPTION=9;
	}
	else
		TI=0;
} 


