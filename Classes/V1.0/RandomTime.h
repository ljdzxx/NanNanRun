#ifndef __RANDOM_TIME_H__
#define __RANDOM_TIME_H__
#include "stdlib.h"
#include "time.h"
//#include "crtdefs.h"

class RandomTime{
private:
	float min_time;//最小等待时间
	int range_time;//随机范围
	float rand_time;//随机时间
	time_t init_time;//初始时间
	float escape_time;//流逝的时间，从第一次调用isRandOK()开始计时



	bool start_rand;

public:
	RandomTime(float minTime,float rangTime);

	//是否到了设定的时间
	bool isTimeOut();


};

#endif // __RANDOM_TIME_H__