#ifndef _MM_MODEL_H
#define _MM_MODEL_H

typedef struct
{
	float MEAN;
	int FLUC;				//�仯�ľ���ֵ���ܺ�
	int INTERVAL;			//��ֵ���µļ�Сֵ��֮���ƽ�����
}STAT_SET;

typedef struct
{
	int a;
	int b;
	int c;
}VECTOR;
//����任
void SpaceCreate( float A[3][3]);
void SpaceTrans(VECTOR *n, float A[3][3]);
//���ò���
void UpdateDatabase( VECTOR*, float A[3][3] );
void AnalyzeDatabase(VECTOR*, STAT_SET*, int *);
unsigned int isStill(STAT_SET*);
//״̬���
unsigned int StillStatus(STAT_SET*);
unsigned int MovingStatus( int * );
//�Ʋ�
int Count( int * ); 

#endif