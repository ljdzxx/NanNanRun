#include "ScoreNode.h"
#include "GlobalVar.h"
#include "ui\CocosGUI.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

USING_NS_CC;


bool ScoreNode::init(){
	if(!Node::init()){
		return false;
	}
	this->setContentSize(winSize);

	_healthPercent=100;//*1000000;
	_coins=0;
	_distance=0;

	//记分牌背景
	auto sprBgScore=Sprite::create("images/bgScore.png");
	sprBgScore->setTag((int)E_NODE_TAGS::SPR_LABEL_SCORE);
	sprBgScore->setAnchorPoint(Point(1,1));
	sprBgScore->setPosition(Point(winSize.width,winSize.height));
	this->addChild(sprBgScore);
	//记分牌Label
	Label* lbScore=Label::createWithTTF("0","fonts/Marker Felt.ttf",30);
	lbScore->setTag((int)E_NODE_TAGS::SPR_LABEL_SCORE);
	lbScore->setColor(Color3B::WHITE);
	lbScore->setAnchorPoint(Vec2(0,0.5));
	lbScore->setPosition(Vec2(86,sprBgScore->getBoundingBox().size.height/2));
	//label->setZOrder(4);
	sprBgScore->addChild(lbScore);

	//跑动距离背景
	auto sprBgDistance=Sprite::create("images/bgDistance.png");
	sprBgDistance->setTag((int)E_NODE_TAGS::SPR_LABEL_DISTANCE);
	sprBgDistance->setAnchorPoint(Point(0,1));
	sprBgDistance->setPosition(Point(0,winSize.height));//winSize.width-(sprBgScore->getBoundingBox().size.width+20)
	this->addChild(sprBgDistance);
	//跑动距离Label
	Label* lbDist=Label::createWithTTF("MI: 0","fonts/Marker Felt.ttf",30);
	lbDist->setTag((int)E_NODE_TAGS::SPR_LABEL_DISTANCE);
	lbDist->setColor(Color3B::WHITE);
	lbDist->setAnchorPoint(Vec2(0,0.5));
	lbDist->setPosition(Vec2(50,131-55));//sprBgDistance->getBoundingBox().size.height/2
	//label->setZOrder(4);
	sprBgDistance->addChild(lbDist);

	//血条背景
	auto healthBarBg=Sprite::create("images/progressBG.png");
	healthBarBg->setAnchorPoint(Vec2(0.5,0.5));
	healthBarBg->setPosition(winSize.width/2, winSize.height-40);
	healthBarBg->setZOrder(98);
	this->addChild(healthBarBg);
	//血条
	auto healthBar=ui::LoadingBar::create("images/progress.png");
	healthBar->setAnchorPoint(Vec2(0.5,0.5));
	healthBar->setPosition(healthBarBg->getPosition());
	healthBar->setDirection(ui::LoadingBar::Direction::LEFT);
	healthBar->setTag((int)E_NODE_TAGS::SPR_ProgressBar);
	healthBar->setZOrder(99);
	healthBar->setPercent(100);
	this->addChild(healthBar);

	//scheduleUpdate();

	return true;
}

void ScoreNode::update(float delta){
	//updateDistance(1);//刷新跑动距离
}

int ScoreNode::deductHealth(int var){
	//扣血条
	_healthPercent-=var;
	auto progressBar=(ui::LoadingBar*)getChildByTag((int)E_NODE_TAGS::SPR_ProgressBar);
	progressBar->setPercent(_healthPercent);
	return _healthPercent;
}

//刷新距离
void ScoreNode::updateDistance(int dis){
	Sprite* spr=(Sprite*)this->getChildByTag((int)E_NODE_TAGS::SPR_LABEL_DISTANCE);
	Label* lbDist=(Label*)spr->getChildByTag((int)E_NODE_TAGS::SPR_LABEL_DISTANCE);
	CCASSERT(lbDist!=NULL, "the lbDist can't be null.");

	_distance+=dis;
	if(_distance%10==0){
		/*char* pStr=new char[20];
		strcpy(pStr, "MI:");


		char* pStr2=new char[10];
		int mi=distance/60*5;	//假设每秒5米
		itoa(mi, pStr2, 10);

		strcat(pStr, pStr2);

		//pStr[sizeof(pStr)-1]='\0';
		lbDist->setString(pStr);*/

		//int mi=_distance/60*5;	//假设每秒5米
		std::string str=StringUtils::format("MI: %u",_distance);
		lbDist->setString(str);
	}
}

//刷新分数
void ScoreNode::updateScore(int sco){
	Sprite* spr=(Sprite*)this->getChildByTag((int)E_NODE_TAGS::SPR_LABEL_SCORE);
	Label* lbScore=(Label*)spr->getChildByTag((int)E_NODE_TAGS::SPR_LABEL_SCORE);
	CCASSERT(lbScore!=NULL, "the lbScore can't be null.");

	_coins+=sco;
	std::string str=StringUtils::format("%u",_coins);
	lbScore->setString(str);
}

//void ScoreNode::resetHealthPercent(){
//	_healthPercent=100;
//}