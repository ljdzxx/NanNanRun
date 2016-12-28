//陷阱层
#pragma once

#include "cocos2d.h"
//#include "RandomTime.h"

USING_NS_CC;

class TrapsNode:public Node
{
public:
	 static TrapsNode* create();
	 
	 
	 //static const float _airCoinOffsetY;//悬在空中陷阱下的金币离陷阱的距离
	 //static const float _sinFromHat2Trap;//正弦波蘑菇头底部离陷阱的距离
	 //static const float _sinXscale;//正弦波跨度与陷阱宽度的比例
	 //static const float _sinYscale;//正弦波高度与陷阱高度的比例
protected:
	bool init() override;
	void update(float delta) override;
private:
	enum class E_NODE_TYPE:int{
		COIN,
		TRAP,
	};
	float SPACE_FROM_SIN_HAT_2_TRAP;//正弦波蘑菇头底部离陷阱的距离(偏距k)
	float TRAP_X_SPACE_BEFORE;//陷阱离金币的距离(前)
	float TRAP_X_SPACE_AFTER;//陷阱离金币的距离(后)
	float TrapsNode_newAirTrap_oil_down_duration;

	Size DEFAULT_COIN_NODE_SIZE;//默认的金币NODE大小

	int _minCoinNodes;//最小金币Node个数
	int _maxRandCoinNodes;//最大随机金币Node个数

	int _remainCoinNodes;//剩下待消耗的金币Node个数
	void resetRemainCoinNodes();

	cocos2d::Vector<Node*> _Nodes;

	TrapsNode();
	Sprite* getAirTrapSpriteById(int sprIdx,int for_i, float baseY);
	TrapsNode&operator=(TrapsNode&){}
	//TrapsNode(TrapsNode&){}
	void newAirTrap();
	void newTrap();
	//删除陷阱
	//void delTrap(Node* sender);
	//陷阱随机时间
	//RandomTime rtTrap;

	Sprite* createCoin();
	Node* packTrapWithCoin(Sprite* spr);
	Node* packAirTrapWithCoin(Sprite* spr);
	Node* newCoinNode(Size& size,float sinXscale=0.0f,bool isIgnoreFirstCoin=false, float spaceX=0);//生成一个装载金币的Node节点
	void addCoinNode(Node* coinNode);
	//CC_SYNTHESIZE(float, m_roadSpeed, RoadSpeed);
};
