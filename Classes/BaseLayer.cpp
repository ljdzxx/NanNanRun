#include "BaseLayer.h"
#include "DialogLayer.h"

bool BaseLayer::init(){
	if ( !Layer::init() )
    {
        return false;
    }

	//ע�ᰴ������
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);


	return true;
}

//��׽��������
void BaseLayer::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	CCLOGERROR("KeyCode:%d", (int)keycode);

    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  //����
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