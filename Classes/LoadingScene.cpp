#include "LoadingScene.h"
#include "GlobalVar.h"
#include "DictionaryLoader.h"
#include "MyParticleCache.h"
#include "ui\CocosGUI.h"
#include "MenuScene.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool LoadingScene::init(){
	if ( !Layer::init() )
    {
        return false;
    }
	winSize=Director::getInstance()->getVisibleSize();
	counter=0.0f;
	isLoadDone=false;

	auto bgSpr=Sprite::create("images/loading.png");
	bgSpr->setPosition(winSize.width/2,winSize.height/2);
	addChild(bgSpr);

	//������
	auto loading_bar=ui::LoadingBar::create("images/loading_bar.png");
	loading_bar->setAnchorPoint(Vec2(0.5,0.5));
	loading_bar->setPosition(Vec2(winSize.width/2+2,46+33/2));
	loading_bar->setDirection(ui::LoadingBar::Direction::LEFT);
	loading_bar->setTag(0);
	//loading_bar->setZOrder(99);
	loading_bar->setPercent(0);
	this->addChild(loading_bar);

	//ˢ�½�����
	scheduleUpdate();

	//��ʼ������
	schedule(schedule_selector(LoadingScene::initGameData),0);

	return true;
}

void LoadingScene::update(float delta){
	counter+=delta;
	float percent=counter*20;

	auto loading_bar=(ui::LoadingBar*)getChildByTag(0);
	loading_bar->setPercent(percent);

	if(percent>=100 && isLoadDone){//�л�����
		auto scene=TransitionFade::create(0.2, MenuScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}

void LoadingScene::initGameData(float delta){
	SpriteFrameCache* sprFraCache=SpriteFrameCache::getInstance();
	sprFraCache->addSpriteFramesWithFile("images/runlast.plist");//���ܵ�����
	sprFraCache->addSpriteFramesWithFile("images/trap.plist");//��������
	sprFraCache->addSpriteFramesWithFile("images/air_trap.plist");//��������
	sprFraCache->addSpriteFramesWithFile("images/coinAnmi.plist");//Ǯ�Ҷ���
	sprFraCache->addSpriteFramesWithFile("images/jump.plist");//��Ծ����
	sprFraCache->addSpriteFramesWithFile("images/injured.plist");//���˶���
	sprFraCache->addSpriteFramesWithFile("images/slide.plist");//���ж���
	sprFraCache->addSpriteFramesWithFile("images/fall.plist");//ǰ��ˤ��
	

	//Ԥ������Ч
	auto audioEng=CocosDenshion::SimpleAudioEngine::getInstance();
	audioEng->setBackgroundMusicVolume(1.0);//�������������
	audioEng->setEffectsVolume(1.0);//��Ч���������
	//audioEng->preloadEffect("sounds/bgm_super.ogg");
	audioEng->preloadEffect("sounds/siwang.mp3");
	audioEng->preloadEffect("sounds/jump.wav");
	audioEng->preloadEffect("sounds/jinbi.mp3");
	audioEng->preloadEffect("sounds/count.ogg");
	audioEng->preloadEffect("sounds/jump_down.mp3");//��������
	//audioEng->preloadEffect("sounds/mainmenu.ogg");
	audioEng->preloadEffect("sounds/ya.wav");//ѽ
	audioEng->preloadEffect("sounds/en.wav");//��
	audioEng->preloadEffect("sounds/injured.wav");//
	audioEng->preloadEffect("sounds/slide.ogg");//
	audioEng->preloadEffect("sounds/guzhang.wav");//�Ƽ�¼��Ч

	GlobalVar::_level=DictionaryLoader::getIntByName("globalvar.level");

	auto initParticle=MyParticleCache::getInstance();//������ϵͳ����һ��ʵ������������ʱʵ������ɿ���

	isLoadDone=true;
}