#include "BaseLayer.h"
#include "DialogLayer.h"

bool BaseLayer::init(){
	if ( !Layer::init() )
    {
        return false;
    }

	//×¢²á°´¼ü¼àÌý
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);


	return true;
}

//²¶×½°´¼ü¼àÌý
void BaseLayer::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	CCLOGERROR("KeyCode:%d", (int)keycode);

    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  //·µ»Ø
    {
        //Director::getInstance()->popScene();

		DialogLayer *dialogLayer = DialogLayer::create();
		this->addChild(dialogLayer,999);
    }
    else if (keycode == EventKeyboard::KeyCode::KEY_MENU)
    {
         DialogLayer *dialogLayer = DialogLayer::create();
		this->addChild(dialogLayer,999);
    }
}