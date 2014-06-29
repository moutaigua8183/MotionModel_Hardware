#include <MM_Accelerator.h>
#include <MM_Bluetooth.h>
#include <MM_Model.h>
#include <reg52.h>
#include <math.h>

#define DATABASE_SIZE 70

unsigned int idata OPTION;
float idata A[3][3];				//ת������
VECTOR DATABASE[DATABASE_SIZE];		//ʵʱ����
STAT_SET idata SET[3];				//3�����ݵ�ͳ����Ϣ
int ZArray[DATABASE_SIZE];			//Z������
int idata STATUS_PRE=-1, STATUS_NOW=-1,STATUS_HISTORY[3]={0};		//MotionDetection's
int temp=0, result=0, prestatus=0;//pedometer's

/*********������**********/
void main (void)
{
	delay(100);						//�ϵ���ʱ
	Init_ADXL345();                 //��ʼ��ADXL345
	Init_UART();					//��ʼ������ͨ��
   	SpaceCreate(A);					//����ϵ����
	delay(200);
	OPTION=9;
	while( 1 )            			//ѭ��
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

/****�����жϳ���****/
void UART_SER (void) interrupt 4 using 0//�����жϷ������
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


