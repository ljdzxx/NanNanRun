#ifndef __GLOBAL_VAL__
#define __GLOBAL_VAL__
#include <time.h>
#include "cocos2d.h"
//#include "ScoreNode.h"

//ENUM转string
#define MACRO_ENUM2STR(var) int2str((int)var)

time_t GetTime();
std::string GetDateTime(time_t t);
std::string int2str(int int_temp);

enum class E_NODE_NAMES:char{
	ROLE,		//角色
	TRAP,		//地面陷阱
	AIR_TRAP,	//空中陷阱
	COIN,		//钱币
};

class ScoreNode;
class MyParticleCache;

class GlobalVar
{
public:
	static cocos2d::Size winSize;	
	static ScoreNode* _scoreNode;//计分板对象一直保存到游戏结束
	static int _level;//当前关卡
	static bool _isLeaving;//是否正在离开场景
	static const float _bgSpeed;//背景移动速度
	static const float _roadSpeed;//路面移动速度
	static const float _coinSpeed;//金币移动速度
	static const float _timePerLevel;//每关游戏时间（秒）
	static const float _coinSpace;//金币间隔距离(像素)
	static const char* xmlDistanceKey;
	static const char* xmlCoinsKey;
	static const char* xmlDateTimeKey;
};



#endif