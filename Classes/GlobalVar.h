#ifndef __GLOBAL_VAL__
#define __GLOBAL_VAL__
#include <time.h>
#include "cocos2d.h"
//#include "ScoreNode.h"

//ENUMתstring
#define MACRO_ENUM2STR(var) int2str((int)var)

time_t GetTime();
std::string GetDateTime(time_t t);
std::string int2str(int int_temp);

enum class E_NODE_NAMES:char{
	ROLE,		//��ɫ
	TRAP,		//��������
	AIR_TRAP,	//��������
	COIN,		//Ǯ��
};

class ScoreNode;
class MyParticleCache;

class GlobalVar
{
public:
	static cocos2d::Size winSize;	
	static ScoreNode* _scoreNode;//�Ʒְ����һֱ���浽��Ϸ����
	static int _level;//��ǰ�ؿ�
	static bool _isLeaving;//�Ƿ������뿪����
	static const float _bgSpeed;//�����ƶ��ٶ�
	static const float _roadSpeed;//·���ƶ��ٶ�
	static const float _coinSpeed;//����ƶ��ٶ�
	static const float _timePerLevel;//ÿ����Ϸʱ�䣨�룩
	static const float _coinSpace;//��Ҽ������(����)
	static const char* xmlDistanceKey;
	static const char* xmlCoinsKey;
	static const char* xmlDateTimeKey;
};



#endif