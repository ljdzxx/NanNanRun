 #include <string>
#include <sstream>
#include "GlobalVar.h"

using namespace std;

cocos2d::Size GlobalVar::winSize=cocos2d::Size::ZERO;//静态变量必须在类的外部初始化一次;
int GlobalVar::_level=1;
const float GlobalVar::_bgSpeed=32*2;
const float GlobalVar::_roadSpeed=256*1.2;
const float GlobalVar::_coinSpeed=256*1.2;
ScoreNode* GlobalVar::_scoreNode=nullptr;
bool GlobalVar::_isLeaving=false;
const float GlobalVar::_timePerLevel=45.0f;
const float GlobalVar::_coinSpace=20.0f;

//跨平台的系统时间获取
time_t GetTime()  
{ 
	//struct tm *tm;  
	time_t timep;  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);
#else  
	/*
	//3.0以下版本
	struct cc_timeval now;   
	CCTime::gettimeofdayCocos2d(&now, NULL);   
	t = now.tv_sec; */

	struct timeval tv;  
	gettimeofday(&tv,NULL);  
	timep = tv.tv_sec;
#endif
	return timep;
}

string GetDateTime(time_t timep){
	struct tm *tm;
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;  
	int month = tm->tm_mon + 1;  
	int day = tm->tm_mday;  

	//int hour=tm->tm_hour;  
	//int minute=tm->tm_min;  
	//int second=tm->tm_sec;
	//std::cout << "当前日期:" << year << "-" << month << "-" << day << std::endl;  
	//std::cout << "当前时间:" << hour << ":" << minute << ":" << second << std::endl;  
	char s[11];
	sprintf(s,"%d-%02d-%02d",year,month,day);
	return s;//返回char*的拷贝
}

//int转string
string int2str(int int_temp)
{  
	stringstream stream;  
	stream<<int_temp;  
	string string_temp=stream.str();   //此处也可以用 stream>>string_temp  
	return string_temp;
}