#pragma once

#include "cocos2d.h"
USING_NS_CC;

class LoadingScene: public Layer{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;  
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);

private:
	bool isLoadDone;//是否加载完成
	float counter;
	void update(float delta) override;
	void initGameData(float delta);
};