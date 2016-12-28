#include "OverScene.h"
#include "GlobalVar.h"
#include "GameScene.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace CocosDenshion;

#define winSize GlobalVar::winSize

/*
unsigned m_MyDistance, m_HisDistance;	//���˾�������ʷ��߾���
unsigned m_MyCoins, m_HisCoins;	//���˽������ʷ��߽��
*/
OverScene::OverScene():m_MyDistance(0),m_MyCoins(0),m_H1(262),m_V1(111),m_H2(335),m_V2(369),m_delayCount(0){

}

OverScene *OverScene::create(int distance, int coins)
{
    OverScene *ret = new OverScene();
    if (ret && ret->init(distance,coins))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* OverScene::createScene(int distance, int coins){
	auto scene = Scene::create();
	auto layer=OverScene::create(distance, coins);
	//layer->setDistance(distance);
	//layer->setCoins(coins);
	scene->addChild(layer);

	return scene;
}

//����Label
void OverScene::createLable(const std::string& text, const std::string& fontFile, float fontSize,
							int tag,
							const Color3B &color,
							const Vec2& anchorPoint,
							const Vec2& position
							){

								Label* lb=Label::createWithTTF(text,fontFile,fontSize);
								lb->setTag(tag);
								lb->setColor(color);
								lb->setAnchorPoint(anchorPoint);
								lb->setPosition(position);
								this->addChild(lb);
}

bool OverScene::init(int distance, int coins){
	if(!BaseLayer::init()){
		return false;
	}
	_distance=distance;
	_coins=coins;

	auto bg=Sprite::create("images/gameover_bg.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg);

	float fontSize=40; //�����С
	//4���Ʒְ�
	createLable("0","fonts/Marker Felt.ttf",fontSize,(int)E_SPR_TAG::lb_MyDistance,Color3B::WHITE,Vec2(0,0.5),Vec2(m_V1, winSize.height-m_H1));
	m_lbMyDistance=dynamic_cast<Label*>(this->getChildByTag((int)E_SPR_TAG::lb_MyDistance));
	createLable("0","fonts/Marker Felt.ttf",fontSize,(int)E_SPR_TAG::lb_MyCoins,Color3B::WHITE,Vec2(0,0.5),Vec2(m_V1, winSize.height-m_H2));
	m_lbMyCoins=dynamic_cast<Label*>(this->getChildByTag((int)E_SPR_TAG::lb_MyCoins));
	createLable("0","fonts/Marker Felt.ttf",fontSize,(int)E_SPR_TAG::lb_HisDistance,Color3B::WHITE,Vec2(0,0.5),Vec2(m_V2, winSize.height-m_H1));
	m_lbHisDistance=dynamic_cast<Label*>(this->getChildByTag((int)E_SPR_TAG::lb_HisDistance));
	createLable("0","fonts/Marker Felt.ttf",fontSize,(int)E_SPR_TAG::lb_HisCoins,Color3B::WHITE,Vec2(0,0.5),Vec2(m_V2, winSize.height-m_H2));
	m_lbHisCoins=dynamic_cast<Label*>(this->getChildByTag((int)E_SPR_TAG::lb_HisCoins));
	//��ʷʱ��
	createLable("2016-12-03","fonts/Marker Felt.ttf",20,(int)E_SPR_TAG::lb_HisDateTime,Color3B::WHITE,Vec2(0,0.5),Vec2(310, winSize.height-390));
	m_lbHisDateTime=dynamic_cast<Label*>(this->getChildByTag((int)E_SPR_TAG::lb_HisDateTime));

	//Label* m_lbMyDistance, *m_lbHisDistance;
	//Label* m_lbMyCoins, *m_lbHisCoins;

	initMenu();

	//������
	readAndSaveHisData();

	this->scheduleUpdate();

	return true;
}

void OverScene::readAndSaveHisData(){
	int hisDistance=UserDefault::getInstance()->getIntegerForKey(GlobalVar::xmlDistanceKey,0);
	m_lbHisDistance->setString(std::to_string(hisDistance));
	int hisCoins=UserDefault::getInstance()->getIntegerForKey(GlobalVar::xmlCoinsKey,0);
	m_lbHisCoins->setString(std::to_string(hisCoins));

	m_lbHisDateTime->setString(UserDefault::getInstance()->getStringForKey(GlobalVar::xmlDateTimeKey,""));

	//����ʷ��¼������Ҫ����
	if(_distance>hisDistance || _coins>hisCoins){
		SimpleAudioEngine::getInstance()->playEffect("sounds/guzhang.wav");//��������
		//�µļ�¼����ͼ��
		auto spr=Sprite::create("images/new_record.png");
		spr->setAnchorPoint(Vec2(0.5,0.5));
		spr->setPosition(Vec2(144,winSize.height-404));
		this->addChild(spr);
		auto s_out=ScaleTo::create(0.2,1.2);
		auto s_in=ScaleTo::create(0.2,0.8);
		auto seq=Sequence::create(s_out,s_in,nullptr);
		auto rf=RepeatForever::create(seq);
		spr->runAction(rf);


		if(_distance>hisDistance){
			UserDefault::getInstance()->setIntegerForKey(GlobalVar::xmlDistanceKey,_distance);
		}
		if(_coins>hisCoins){
			UserDefault::getInstance()->setIntegerForKey(GlobalVar::xmlCoinsKey,_coins);
		}
		//char dt[20];
		//GetDateTime(dt,GetTime());
		UserDefault::getInstance()->setStringForKey(GlobalVar::xmlDateTimeKey,GetDateTime(GetTime()));
		//д��
		UserDefault::getInstance()->flush();
	}else{
		SimpleAudioEngine::getInstance()->playEffect("sounds/count.ogg");//ͳ�Ʒ�����Ч
	}
}

void OverScene::update(float delta){
	//CCLOGERROR("update delta:%.4f",delta);
	//if(m_delayCount<10){
	//	++m_delayCount;
	//}else{

		//SimpleAudioEngine::getInstance()->playEffect("sounds/jinbi.mp3");

	m_delayCount=0;

	do{
		//�÷�Ч��
		if (m_MyDistance<_distance)
		{
			m_lbMyDistance->setString(std::to_string(m_MyDistance++));
		}

		if (m_MyCoins<_coins)
		{
			m_lbMyCoins->setString(std::to_string(m_MyCoins++));
		}

		++m_delayCount;
	}while(m_delayCount<100);
	
	

		//m_delayCount=0;
	//}

}

void OverScene::initMenu(){
	auto item_0=MenuItemImage::create("images/buttons/again_n.png", "images/buttons/again_d.png", CC_CALLBACK_1(OverScene::menuCallback,this));
	item_0->setTag((int)OverScene::E_SPR_TAG::mi_PlayAgain);

	auto item_1=MenuItemImage::create("images/buttons/back_n.png", "images/buttons/back_d.png", CC_CALLBACK_1(OverScene::menuCallback,this));
	item_1->setTag((int)OverScene::E_SPR_TAG::mi_Back);

	auto menu= Menu::create(item_0,item_1, nullptr);
	menu->alignItemsVerticallyWithPadding(40);//��ֱ���У��趨�ļ��
	menu->setAnchorPoint(Point(0,0));
	//menu->setPosition(Point(580,winSize.height-457));//*winSize.width/800	*winSize.height/480
	menu->setPosition(675, winSize.height-348);
	this->addChild(menu);
}

void OverScene::menuCallback(Ref *sender){
	auto item=(MenuItem*)sender;
	switch(item->getTag()){
	case (int)OverScene::E_SPR_TAG::mi_PlayAgain:{
		//GlobalVar::_level=1;//�������ӵ�һ�ؿ�ʼ
		auto scene=TransitionFade::create(0.2, GameScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
		break;
	case (int)OverScene::E_SPR_TAG::mi_Back:{
		//GlobalVar::_level=1;//�������ӵ�һ�ؿ�ʼ
		auto scene=TransitionFade::create(0.2, MenuScene::createScene());
		Director::getInstance()->replaceScene(scene);

		//cocos2d::MessageBox("msg", "title"
		//	,
		//	[this]()
		//	{
		//		//if (GGlobal::GLoginSocket->ConnectLoginServer()) // ������������
		//		//{
		//		//	this->mTimeUsed = 0.0f;
		//		//}
		//	}
		//);

		}
		break;

	/*case TAG_MENUITEM_OPTION:
		break;
		case TAG_MENUITEM_QUIT:{
		Director::getInstance()->end();
		#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		exit(0);
		#endif
		break;
		}*/
	default:
		break;


	}
}