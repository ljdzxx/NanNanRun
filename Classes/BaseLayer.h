#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BaseLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;  
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(BaseLayer);

private:
	void onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event);
};