#ifndef _MM_MODEL_H
#define _MM_MODEL_H

typedef struct
{
	float MEAN;
	int FLUC;				//变化的绝对值的总和
	int INTERVAL;			//阈值以下的极小值点之间的平均间隔
}STAT_SET;

typedef struct
{
	int a;
	int b;
	int c;
}VECTOR;
//坐标变换
void SpaceCreate( float A[3][3]);
void SpaceTrans(VECTOR *n, float A[3][3]);
//公用部分
void UpdateDatabase( VECTOR*, float A[3][3] );
void AnalyzeDatabase(VECTOR*, STAT_SET*, int *);
unsigned int isStill(STAT_SET*);
//状态检测
unsigned int StillStatus(STAT_SET*);
unsigned int MovingStatus( int * );
//计步
int Count( int * ); 

#endif