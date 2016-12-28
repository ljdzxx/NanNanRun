#include "RandomTime.h"
#include "cocos2d.h"
#include "GlobalVar.h"

USING_NS_CC;

//�����ʱ��
RandomTime::RandomTime(float mTime,float rTime):min_time(mTime),range_time(rTime),init_time(0),start_rand(false){
	
}

bool RandomTime::isTimeOut(){
	if(!start_rand){//��δ��ʼ��ʱ
		//srand(GetTime()+rand());
		rand_time=rand()%range_time+1;//���һ�����ʱ��
		init_time=GetTime();
		start_rand=true;
		return false;
	}else{
		bool r = GetTime()-init_time >= min_time+rand_time-(float)GlobalVar::_level/10;
		if(r){
			start_rand=false;
		}
		return r;
	}

}