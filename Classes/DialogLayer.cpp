#include "DialogLayer.h"

USING_NS_CC;

DialogLayer::DialogLayer()
{
}

DialogLayer::~DialogLayer()
{
}

bool DialogLayer::init()
{
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!LayerColor::initWithColor(ccc4(0, 0, 0, 125)));
        
        this->initDialog();
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void DialogLayer::initDialog()
{
	initMenu();

	//注册按键监听
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(DialogLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	//注册触摸事件监听-------------------------------------------------------------------------
	auto listener=EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);//事件不再向下分发（需要onTouchBegan返回true）
	listener->onTouchBegan=CC_CALLBACK_2(DialogLayer::onTouchBegan,this);
	/*listener->onTouchMoved=CC_CALLBACK_2(DialogLayer::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(DialogLayer::onTouchEnded, this);
	listener->onTouchCancelled=CC_CALLBACK_2(DialogLayer::onTouchCancelled,this);*/
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
	scheduleOnce(schedule_selector(DialogLayer::delayExecute),0);//不在定时器中执行pause会造成卡顿
}

//捕捉按键监听
void DialogLayer::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	CCLOGERROR("DialogLayer KeyCode:%d", (int)keycode);
	event->stopPropagation();//阻止事件向下传递

    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  //返回
    {
        //Director::getInstance()->popScene();

		onContinueClick(nullptr);
    }
}

void DialogLayer::delayExecute(float delta){
	Director::getInstance()->pause();
}

void DialogLayer::onEnter()
{
    LayerColor::onEnter();
}

void DialogLayer::onExit()
{
    CCLayerColor::onExit();
}

bool DialogLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *unused_event)
{

    return true;
}

//void DialogLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::CCEvent *unused_event)
//{
//}
//
//void DialogLayer::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *unused_event)
//{
//}
//
//void DialogLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *unused_event)
//{
//}

void DialogLayer::initMenu(){
	auto item_0=MenuItemImage::create("images/buttons/continue_n.png", "images/buttons/continue_d.png", CC_CALLBACK_1(DialogLayer::onContinueClick,this));
	auto item_1=MenuItemImage::create("images/buttons/exit_n.png", "images/buttons/exit_d.png", CC_CALLBACK_1(DialogLayer::onExitClick,this));

	auto menu= Menu::create(item_0,item_1, nullptr);
	menu->alignItemsVerticallyWithPadding(20);
	this->addChild(menu);
}

void DialogLayer::onContinueClick(Ref *sender){
	//auto item=(MenuItem*)sender;

	Director::getInstance()->resume();
	this->removeFromParentAndCleanup(true);
}

void DialogLayer::onExitClick(Ref *sender){
	//auto item=(MenuItem*)sender;

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}