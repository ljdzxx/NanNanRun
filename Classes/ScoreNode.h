#pragma once
//记分牌(跑动距离，金币，血量等)
#include "cocos2d.h"
USING_NS_CC;

class ScoreNode:public Node{
public:

	bool init() override;
	CREATE_FUNC(ScoreNode);

	void updateDistance(int dis);
	void updateScore(int sco);
	int deductHealth(int var);
	//void resetHealthPercent();
private:
	float _healthPercent;//血量百分比值
	//unsigned _coins;//金币数
	//unsigned _distance;//跑动距离
	enum class E_NODE_TAGS:int{
		SPR_ProgressBar,
		SPR_LABEL_SCORE,
		SPR_LABEL_DISTANCE,
	};

	void update(float delta) override;

	CC_SYNTHESIZE(int, _coins, Coins);
	CC_SYNTHESIZE(int, _distance, Distance);
};