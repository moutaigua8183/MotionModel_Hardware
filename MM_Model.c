#include <MM_Model.h>
#include <MM_Accelerator.h>
#include <math.h>
#include <MM_Bluetooth.h>

#define DATABASE_SIZE 70

int idata THRES_STILL=25;
int idata THRES_SIT_Z=50;
int idata THRES_STAND_X=80;
int idata THRES_STAND_Y=90;
int idata THRES_STAND_Z=-200;
int idata THRES_RUN	=-500;
int idata THRES_WALK=-270;


 /******************坐标系***************/
void SpaceCreate(float A[3][3])
{
	int idata i,a,b,c,ap,bp,cp;
	VECTOR idata test[3];
	float idata temp;
	GetAll(&ap,&bp,&cp);
	for(i=0;i<3;i++)
	{
		GetAll(&a,&b,&c);
		delay(70);
		if( abs(ap-a)<10 && abs(bp-b)<10 && abs(cp-c)<10 )
		{
			 test[i].a=a;
			 test[i].b=b;
			 test[i].c=c;
		}
		else
			i--;
		ap=a;
		bp=b;
		cp=c;
	}
	//x暂时借作为临时变量，算标准化过的z
	A[0][0]=-(test[0].a+test[1].a+test[2].a)/3;
	A[0][1]=-(test[0].b+test[1].b+test[2].b)/3;
	A[0][2]=-(test[0].c+test[1].c+test[2].c)/3;
	temp=sqrt(A[0][0]*A[0][0]+A[0][1]*A[0][1]+A[0][2]*A[0][2]);
	A[2][0]=A[0][0]/temp;
	A[2][1]=A[0][1]/temp;
	A[2][2]=A[0][2]/temp;

	GetAll(&a,&b,&c);
	i=0;
	test[0].a=0;
	test[0].b=0;
	test[0].c=0;
	while( 1 )
	{
		if( i<10
			&& abs(a-ap)<20 && abs(b-bp)<20 && abs(c-cp)<20 
			&& (abs(a+A[0][0])>130 || abs(b+A[0][1])>130 || abs(c+A[0][2])>130)
			)
		{
			test[0].a+=a;
			test[0].b+=b;
			test[0].c+=c;
			i++;
		}
		if(i==10)
			break;	
		ap=a;	bp=b;	cp=c;
		GetAll(&a,&b,&c);
		delay(50);
	}

	//第一下变化方向
	A[1][0]=test[0].a/10+A[0][0];
	A[1][1]=test[0].b/10+A[0][1];
	A[1][2]=test[0].c/10+A[0][2];
	//X轴正向
	A[0][0]=A[1][1]*A[2][2]-A[2][1]*A[1][2];
	A[0][1]=A[1][2]*A[2][0]-A[2][2]*A[1][0];
	A[0][2]=A[1][0]*A[2][1]-A[2][0]*A[1][1];
	//X轴标准化
	temp=sqrt(A[0][0]*A[0][0]+A[0][1]*A[0][1]+A[0][2]*A[0][2]);
	A[0][0]=A[0][0]/temp;
	A[0][1]=A[0][1]/temp;
	A[0][2]=A[0][2]/temp;
	//Y轴标准化
	A[1][0]=A[2][1]*A[0][2]-A[0][1]*A[2][2];
	A[1][1]=A[0][2]*A[2][2]-A[2][0]*A[0][2];
	A[1][2]=A[2][0]*A[0][1]-A[0][0]*A[2][1];
	SendByte('K');
}

void SpaceTrans(VECTOR *n, float A[3][3])
{
	VECTOR idata temp;
	temp.a=(*n).a;
	temp.b=(*n).b;
	temp.c=(*n).c;
	(*n).a=A[0][0]*temp.a+A[0][1]*temp.b+A[0][2]*temp.c;
	(*n).b=A[1][0]*temp.a+A[1][1]*temp.b+A[1][2]*temp.c;
	(*n).c=A[2][0]*temp.a+A[2][1]*temp.b+A[2][2]*temp.c;
}






 /******************Common Functions***************/
void UpdateDatabase( VECTOR n[DATABASE_SIZE],float A[3][3] )
{
	int idata i;
	for (i=0; i<DATABASE_SIZE; i++)
	{	
		GetAll(&n[i].a, &n[i].b, &n[i].c);
		SpaceTrans(n+i,A);
		delay(2);
	}
}


void AnalyzeDatabase(VECTOR x[DATABASE_SIZE], STAT_SET set[3], int array[DATABASE_SIZE])
{
	int idata i;
	int idata count=0,count_sum=0,count_num=0, isFirst=1;
	long idata a=0,b=0,c=0;
	for(i=0;i<DATABASE_SIZE;i++)
	{
		array[i]=x[i].c;
		a+=x[i].a;
		b+=x[i].b;
		c+=x[i].c;
	}
	set[0].MEAN=a/DATABASE_SIZE;
	set[1].MEAN=b/DATABASE_SIZE;
	set[2].MEAN=c/DATABASE_SIZE;
	for(i=0;i<DATABASE_SIZE;i++)
	{
		set[0].FLUC=abs(x[i].a-set[0].MEAN);
		set[1].FLUC=abs(x[i].b-set[1].MEAN);
		set[2].FLUC=abs(x[i].c-set[2].MEAN);
	}
}



unsigned int isStill( STAT_SET *set )
{
	if( set[0].FLUC<THRES_STILL && set[1].FLUC<THRES_STILL && set[2].FLUC<THRES_STILL)
		return 1;
	else
		return 0;
}


/*******************Motion Detection*********************/
unsigned int StillStatus(STAT_SET *set)
{
	if(	set[2].MEAN<THRES_STAND_Z )
		return 1;	//Standing
	if( abs( set[0].MEAN<120 )		//排除侧翻
		&& set[1].MEAN>0 
		&& set[2].MEAN<THRES_SIT_Z && set[2].MEAN>THRES_STAND_Z 
		)
		return 2;	//Sitting
	if( abs(set[0].MEAN)>150		//侧翻
		|| set[1].MEAN<-200 	 	//前扑
		|| set[2].MEAN>100 			//后坐
		)		
		return 8;	//FallDown
	return 0;
}

unsigned int MovingStatus( int array[DATABASE_SIZE] )
{
	int idata i=0,j=0,levelnum_min=0;
	float idata level_min=0;
	double idata levelsum_min=0;
	int temp[3],validvalues[DATABASE_SIZE/4];
	temp[0]=array[0];
	temp[1]=array[1];
	temp[2]=array[2]; 
	for(i=1;i<DATABASE_SIZE;i++)
	{
		if( temp[1]<temp[0] && temp[1]<temp[2] )
		{
			if( temp[1]<-250 )
			{
				validvalues[j]=temp[1];
				levelsum_min=levelsum_min+temp[1];
				levelnum_min++;
				j++;				
			}
		}
		if(i==DATABASE_SIZE-2)
			break;
		else
		{
			temp[0]=temp[1];
			temp[1]=temp[2];
			temp[2]=array[i+2];
		}		
	}
	level_min=levelsum_min/levelnum_min;
	levelsum_min=0;levelnum_min=0;
	for(i=0;i<j;i++)
	{
		if( validvalues[i]<level_min )
		{
			levelsum_min=levelsum_min+validvalues[i];
			levelnum_min++;				
		}
	}
	level_min=levelsum_min/levelnum_min;
	if( levelnum_min==0 )
		return 0;
	if( level_min<THRES_RUN )
		return 5;
	else if(level_min<THRES_WALK ) 
		return 4;
	return 0;


}


/***************************Pedometer********************************/
int Count( int array[DATABASE_SIZE] )
{
	int idata i=0,j=0,m=0,NUM=50,count=0,levelnum_max=0,levelnum_min=0,prestatus=-1,prepos=0;
	float idata level_min=0,level_max=0,THRES_INTERVAL;
	double idata levelsum_max=0,levelsum_min=0,exp;
	int temp[3],validvalues[DATABASE_SIZE/4],validpos[DATABASE_SIZE/4];
	temp[0]=array[0];
	temp[1]=array[1];
	temp[2]=array[2]; 
	for(i=1;i<DATABASE_SIZE;i++)
	{
		if( temp[1]<temp[0] && temp[1]<temp[2] )
		{
			if( temp[1]<-250 )
			{
				validvalues[j]=temp[1];
				validpos[j]=i;
				levelsum_min=levelsum_min+temp[1];
				levelnum_min++;
				j++;				
			}
		}
		else if ( temp[1]>temp[0] && temp[1]>temp[2] )
		{
			if( temp[1]>-250 )
			{
				validvalues[j]=temp[1];
				validpos[j]=i;
				levelsum_max=levelsum_max+temp[1];
				levelnum_max++;
				j++;				
			}
		}
		if(i==DATABASE_SIZE-2)
			break;
		else
		{
			temp[0]=temp[1];
			temp[1]=temp[2];
			temp[2]=array[i+2];
		}		
	}
	level_min=levelsum_min/levelnum_min;
	level_max=levelsum_max/levelnum_max;
	m=0;levelsum_min=0;levelnum_min=0;levelsum_max=0;levelnum_max=0;
	for(i=0;i<j;i++)
	{
		if( validvalues[i]>level_max )
		{
			validvalues[m]=validvalues[i];
			validpos[m]=validpos[i];	
			m++;				
		}
		else if( validvalues[i]<level_min )
		{
			validvalues[m]=validvalues[i];
			validpos[m]=validpos[i];
			levelsum_min=levelsum_min+validvalues[i];
			levelnum_min++;
			m++;				
		}
	}
	level_min=levelsum_min/levelnum_min;
	if( levelnum_min==0 )
		return 0;
	exp=-360/level_min;
	THRES_INTERVAL=pow(10.0,exp);
	if(validvalues[0]<-250)
		prestatus=0;
	else
		prestatus=1;
	prepos=0;
	for(i=1;i<m;i++)
	{
		if( validvalues[i]<-250 && prestatus==1 )
		{
			prestatus=0;
			count++;
			prepos=validpos[i];
			continue;
		}
		if( validvalues[i]<-250 && prestatus==0 )
		{
			if( validpos[i]-prepos>THRES_INTERVAL )
			{
				count++;
				prepos=validpos[i];
				continue;
			}	
			else
				continue;
		}
		if( validvalues[i]>-250 && prestatus==0 )
		{ 
			prestatus=1;
			count++;
			prepos=validpos[i];
			continue;
		}
		if( validvalues[i]>-250 && prestatus==1 )
		{
			if( validpos[i]-prepos>THRES_INTERVAL )
			{
				count++;
				prepos=validpos[i];
				continue;
			}	
			else
				continue;
		}
	}
	if( level_min<THRES_RUN )				//z轴部分小于所有极小值的平均值的极小值的平均值 跟临界线比
		count=count+50;
	else if(level_min<THRES_WALK ) 
		count=count+40;
	return count;
}

