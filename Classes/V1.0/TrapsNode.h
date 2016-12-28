//ÏÝÚå²ã
#pragma once

#include "cocos2d.h"
#include "RandomTime.h"

USING_NS_CC;

class TrapsNode:public Node
{
public:
	 static TrapsNode* create();

protected:
	bool init() override;
	void update(float delta) override;
private:
	TrapsNode();
	Sprite* getAirTrapSpriteById(int sprIdx,int for_i, float baseY);
	TrapsNode&operator=(TrapsNode&){}
	//TrapsNode(TrapsNode&){}
	void newAirTrap();
	void newTrap();
	//É¾³ýÏÝÚå
	void delTrap(Node* sender);
	//ÏÝÚåËæ»úÊ±¼ä
	RandomTime rtTrap;

	//CC_SYNTHESIZE(float, m_roadSpeed, RoadSpeed);
};
