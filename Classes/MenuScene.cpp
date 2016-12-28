#include "MenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GlobalVar.h"
#include "OverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

#ifndef winSize
#define winSize GlobalVar::winSize
#endif

const int MenuScene::TAG_MENUITEM_PLAY=0;
const char* GlobalVar::xmlDistanceKey="Max_Distance";
const char* GlobalVar::xmlCoinsKey="Max_Coins";
const char* GlobalVar::xmlDateTimeKey="DateTime";

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }

	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();
	//播放音效
	//audioEng->playEffect("sounds/mainmenu.ogg", true);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/mainmenu.ogg", true);//循环播放背景音乐

	initBG();
	//initMenu();
	initButton();

    return true;
}

MenuScene::~MenuScene(){
	//_cachedParticle->release();
}




void MenuScene::initBG(){
	//洞口
	auto hollSpr = Sprite::create("images/gameStart.png", Rect(0,481,224,446));
	hollSpr->setAnchorPoint(Vec2::ZERO);
	hollSpr->setPosition(Vec2(0,86));
	this->addChild(hollSpr);
	//微弱移动洞口
	auto moveLeft=MoveBy::create(2.5f,Vec2(-10,0));
	auto moveRight=MoveBy::create(2.5f,Vec2(10,0));
	auto seq=Sequence::create(moveLeft,moveRight,nullptr);
	hollSpr->runAction(RepeatForever::create(seq));


	//火把
	auto torchSpr = Sprite::create("images/torch.png");
	torchSpr->setAnchorPoint(Vec2::ZERO);
	torchSpr->setPosition(Vec2(50,90));
	this->addChild(torchSpr);

	//大背景
	auto bgSpr = Sprite::create("images/gameStart.png", Rect(0,0,800,480));
	bgSpr->setAnchorPoint(Vec2::ZERO);
	bgSpr->setPosition(Vec2::ZERO);
	this->addChild(bgSpr);

	//路面，图片属性 - w:512,h:128,y-offset:6,x-offset:12
	auto roadSpr=Sprite::create("images/grounds.png", Rect(0,11,500,111));
	roadSpr->setPosition(Vec2::ZERO);
	roadSpr->setAnchorPoint(Vec2::ZERO);
	//roadSpr->setTag((int)E_NODE_TAGS::ROAD);
	this->addChild(roadSpr);

	//拼上最后的300米
	auto spr_road=Sprite::create("images/grounds.png", Rect(0,11,300,111));
	spr_road->setAnchorPoint(Point::ZERO);
	spr_road->setPosition(Vec2(500,0));
	roadSpr->addChild(spr_road);



}

void MenuScene::initButton(){
	auto item_0=MenuItemImage::create("images/buttons/enter_n.png", "images/buttons/enter_d.png", CC_CALLBACK_1(MenuScene::menuCallback,this));
	//CCLOGERROR("item_0 size:w- %.2f, h- %.2f", item_0->getContentSize().width,item_0->getContentSize().height);
	auto menu= Menu::create(item_0, nullptr);
	//menu->setContentSize(item_0->getContentSize());
	
	//menu->alignItemsVerticallyWithPadding(20);
	//menu->setAnchorPoint(Vec2(1,1));
	menu->setPosition(winSize.width-item_0->getContentSize().width/2-10,65);
	this->addChild(menu);
}


void MenuScene::initMenu(){

	//跳跳楠文字
	auto sprT1 = Sprite::create("images/ttn.png", Rect(0,0,151,116));
	sprT1->setScale(0.6);
	//sprT1->setAnchorPoint(Vec2::ZERO);
	//sprT1->setPosition(Vec2(510,80));
	//this->addChild(sprT1);

	auto rt1=RotateTo::create(2,-5);
	auto rt2=RotateTo::create(2,5);
	auto rf=RepeatForever::create(Sequence::create(rt1,rt2,nullptr));
	sprT1->runAction(rf);
	
	//float space=100.0f;
	auto sprT2 = Sprite::create("images/ttn.png", Rect(151,0,151,116));
	sprT2->setScale(0.6);
	//sprT2->setAnchorPoint(Vec2::ZERO);
	//sprT2->setPosition(Vec2(510+space,80));
	//this->addChild(sprT2);
	sprT2->runAction(rf->clone());

	auto sprN = Sprite::create("images/ttn.png", Rect(302,0,151,116));
	sprN->setScale(0.6);
	//sprN->setPosition(Vec2(510+space*2,80));
	sprN->runAction(rf->clone());
	//sprN->setTag(TAG_MENUITEM_PLAY);


	auto mi_1=MenuItemSprite::create(sprT1, sprT1, nullptr,CC_CALLBACK_1(MenuScene::menuCallback,this));
	mi_1->setTag(TAG_MENUITEM_PLAY);
	
	auto mi_2=MenuItemSprite::create(sprT2, sprT2, nullptr,CC_CALLBACK_1(MenuScene::menuCallback,this));
	mi_2->setTag(TAG_MENUITEM_PLAY);

	auto mi_3=MenuItemSprite::create(sprN, sprN, nullptr,CC_CALLBACK_1(MenuScene::menuCallback,this));
	mi_3->setTag(TAG_MENUITEM_PLAY);
	//addChild(mi_3);
	

	/*
	auto item_0=MenuItemImage::create("images/bt1.png", "images/bt1.png", CC_CALLBACK_1(MenuScene::menuCallback,this));
	//item_0->setScale(0.8);//缩小
	//auto item_1=MenuItemImage::create("menu/help_n.png", "menu/help_s.png", "menu/help_d.png", CC_CALLBACK_1(MenuScene::menuCallback,this));
	//auto item_2=MenuItemImage::create("menu/option_n.png", "menu/option_s.png", "menu/option_d.png", CC_CALLBACK_1(MenuScene::menuCallback,this));
	//auto item_3=MenuItemImage::create("menu/quit_n.png", "menu/quit_s.png", CC_CALLBACK_1(MenuScene::menuCallback,this));

	item_0->setTag(TAG_MENUITEM_PLAY);
	//item_1->setTag(TAG_MENUITEM_HELP);
	//item_1->setEnabled(false);//禁用
	//item_2->setTag(TAG_MENUITEM_OPTION);
	//item_2->setEnabled(false);//禁用
	//item_3->setTag(TAG_MENUITEM_QUIT);

	*/

	auto menu= Menu::create(mi_1,mi_2,mi_3, /*item_0,/*item_1,item_2,item_3,*/ nullptr);
	//menu->setScale(0.5);
	menu->alignItemsHorizontallyWithPadding(-50.0f);
	//menu->alignItemsVerticallyWithPadding(20);
	menu->setAnchorPoint(Point(0.5,0.5));
	//(106,320)是800*480分辨率下按钮的中轴点
	menu->setPosition(Point(670,80));//*winSize.width/800	*winSize.height/480
	this->addChild(menu);
}

void MenuScene::menuCallback(Ref *sender){
	//auto item=(MenuItem*)sender;
	//switch(item->getTag()){
	//	case TAG_MENUITEM_PLAY:{
			auto scene=TransitionFade::create(0.2, GameScene::createScene());
			Director::getInstance()->replaceScene(scene);

			//测试得分结果界面
			//auto scene=TransitionFade::create(0.5, OverScene::createScene(rand()%10000, rand()%10000));
			//Director::getInstance()->replaceScene(scene);

		//}
		//	break;


		/*case TAG_MENUITEM_HELP:
			break;
		case TAG_MENUITEM_OPTION:
			break;
		case TAG_MENUITEM_QUIT:{
			Director::getInstance()->end();
			#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
				exit(0);
			#endif
			break;
		}*/
		
		//default:
		//	break;


	//}
}