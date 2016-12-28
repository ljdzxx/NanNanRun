#ifndef __RANDOM_TIME_H__
#define __RANDOM_TIME_H__
#include "stdlib.h"
#include "time.h"
//#include "crtdefs.h"

class RandomTime{
private:
	float min_time;//��С�ȴ�ʱ��
	int range_time;//�����Χ
	float rand_time;//���ʱ��
	time_t init_time;//��ʼʱ��
	float escape_time;//���ŵ�ʱ�䣬�ӵ�һ�ε���isRandOK()��ʼ��ʱ



	bool start_rand;

public:
	RandomTime(float minTime,float rangTime);

	//�Ƿ����趨��ʱ��
	bool isTimeOut();


};

#endif // __RANDOM_TIME_H__