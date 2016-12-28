#pragma once
//µØÍ¼²ã
#include "cocos2d.h"
USING_NS_CC;

class MapNode:public Node{
public:
	bool init() override;
	CREATE_FUNC(MapNode);
private:
	enum class E_NODE_TAGS:int{
		BACKGROUND,
		ROAD,
	};

	void update(float delta) override;
};