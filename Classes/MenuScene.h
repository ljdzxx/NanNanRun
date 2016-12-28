#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__
#include "cocos2d.h"
#include "AppDelegate.h"
#include "BaseLayer.h"

USING_NS_CC;

//#define 

class MenuScene: public BaseLayer
{
public:
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

	void initBG();
	void initMenu();
	void menuCallback(Ref *sender);


	static const int TAG_MENUITEM_PLAY;

private:
	void initButton();
	
	~MenuScene();

};

#endif // __MENU_SCENE_H__
