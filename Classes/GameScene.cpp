#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
//#include "RandomTime.h"
#include "ActorSprite.h"
#include "GlobalVar.h"
#include "OverScene.h"
//#include "CoinsNode.h"
#include "TrapsNode.h"
#include "DictionaryLoader.h"
#include "MapNode.h"

#include "ScoreNode.h"//保持在最后引用

USING_NS_CC;
using namespace CocosDenshion;

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

const float GameScene::fadeDuration=3.0f;


//bool GameScene::initWithColor(const Color4B& color)
//{
//    Size s = Director::getInstance()->getWinSize();
//    this->initWithColor(color, s.width, s.height);
//    return true;
//}
//
//bool GameScene::initWithColor(const Color4B& color, GLfloat w, GLfloat h)
//{
//    if (GameScene::init())
//    {
//
//        // default blend function
//        _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
//
//        _displayedColor.r = _realColor.r = color.r;
//        _displayedColor.g = _realColor.g = color.g;
//        _displayedColor.b = _realColor.b = color.b;
//        _displayedOpacity = _realOpacity = color.a;
//
//        for (size_t i = 0; i<sizeof(_squareVertices) / sizeof( _squareVertices[0]); i++ )
//        {
//            _squareVertices[i].x = 0.0f;
//            _squareVertices[i].y = 0.0f;
//        }
//
//        updateColor();
//        setContentSize(Size(w, h));
//
//        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_COLOR_NO_MVP));
//        return true;
//    }
//    return false;
//}

Scene* GameScene::createScene(){
	auto scene = Scene::createWithPhysics();

	//设置Debug模式，你会看到物体的表面被线条包围，主要为了在调试中更容易地观察  
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); 

	//游戏主层
	auto mainLayer=GameScene::create();//Color4B::BLACK
	mainLayer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(mainLayer,0,(int)E_NODE_TAGS::ROOT_LAYER);

	//淡出效果的遮罩层
	LayerColor* maskLayer = LayerColor::create(Color4B::BLACK);
	auto labelLevel=getLevelLabel(60.0f);
	maskLayer->addChild(labelLevel);
	/*Spawn::create(ScaleBy::create(fadeDuration/2,0.2),
		MoveTo::create(fadeDuration/2, Vec2(winSize.width/2, winSize.height-80)),nullptr)*/
	labelLevel->runAction(FadeOut::create(fadeDuration/2));

	scene->addChild(maskLayer,1,(int)E_NODE_TAGS::MASK_LAYER);
	auto fadeOut = Sequence::create
        (
            FadeOut::create(fadeDuration/2),
            CallFunc::create(CC_CALLBACK_0(GameScene::init2,mainLayer)),
			nullptr
        );
    maskLayer->runAction(fadeOut);

	return scene;
}

GameScene::GameScene(){
}

GameScene::~GameScene(){
	
}

Label* GameScene::getLevelLabel(float fontSize){
	std::string levelTitle=DictionaryLoader::getStrByName("gamescene.level.title");//第N关
	levelTitle=StringUtils::format(levelTitle.c_str(),GlobalVar::_level);
	Label* lb=Label::createWithSystemFont(levelTitle,"",fontSize);
	//lb->setTag((int)E_NODE_TAGS::LABEL_LEVEL);
	lb->setColor(Color3B::GREEN);
	//lb->setAnchorPoint(anchorPoint);
	lb->setPosition(winSize.width/2,winSize.height/2);
	return lb;
}

bool GameScene::init(){
	if(!BaseLayer::init()){
		return false;
	}
	CCLOGERROR("GameScene is initializing...");

	//定时器
	//this->schedule(schedule_selector(GameScene::scheduleCallBack), 0.1);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/bgm_super.ogg",true);//跑步音效

	initMap();//z-order:1
	initScore();//z-order:3
	initCoinAnim();//在陷阱层和金币层都会用到金币动画
	
	return true;
}

void GameScene::init2(){
	initPersonSpr();

	//触摸事件检测-------------------------------------------------------------------------
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(GameScene::onTouchBegan,this);
	//listener->onTouchMoved=CC_CALLBACK_2(GameScene::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(GameScene::onTouchEnded, this);
	//有一点非常重要，FixedPriority listener添加完成之后需要手动remove，而SceneGraphPriority listener是跟node绑定的，在node的析构函数中会被移除。
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,1);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	//物理碰撞检测-----------------------------------------------------------------
	//添加监听器  
	auto contactListener=EventListenerPhysicsContact::create();  
	//设置监听器的碰撞开始函数  
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactTrapBegin, this);  //若返回false，Chipmunk会完全忽略碰撞，preSolve()和postSolve()回调将永远不会被执行，但你仍然会在形状停止重叠的时候接收到一个单独的事件。
	//添加到事件分发器中  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//定时换关卡
	this->schedule(schedule_selector(GameScene::shiftLevel), GlobalVar::_timePerLevel);

	//每帧调用update方法
	this->scheduleUpdate();
	
	//initCoin();
	initTrap();
	
	if(GlobalVar::_level%9==0){//每9关设置一个进阶，系统加速
		Director::getInstance()->getScheduler()->setTimeScale(1.0f+(GlobalVar::_level-9)*0.1);
	}

	auto lbLevel=getLevelLabel(20.0f);
	lbLevel->setLocalZOrder((int)E_ZORDERS::SCORE);
	lbLevel->setPosition(winSize.width/2, winSize.height-70);
	addChild(lbLevel);

	setLeaving(false);
	CCLOGERROR("GameScene is initialized.");
}

//创建钱币层
//void GameScene::initCoin(){	
//	auto coinsNode=CoinsNode::create();
//	//coinsNode->setTag((int)E_NODE_TAGS::NODE_COINS);
//	coinsNode->setAnchorPoint(Vec2(0,0));
//	coinsNode->setPosition(Vec2::ZERO);
//	coinsNode->setLocalZOrder((int)E_ZORDERS::COIN);	//z-order:4
//	addChild(coinsNode);
//}

//创建陷阱层
void GameScene::initTrap(){
	auto trapsNode=TrapsNode::create();
	trapsNode->setPosition(Vec2::ZERO);
	trapsNode->setAnchorPoint(Vec2::ZERO);
	trapsNode->setLocalZOrder((int)E_ZORDERS::TRAP);	//z-order:2
	addChild(trapsNode);
}

void GameScene::setLeaving(bool flag){
	GlobalVar::_isLeaving=flag;
	//设置Actor的碰撞属性
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(flag){
		//actorSpr->getPhysicsBody()->setCategoryBitmask(0);
		//actorSpr->getPhysicsBody()->setContactTestBitmask(0);//当切换场景过度的阶段不进行碰撞检测
		
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
		//_eventDispatcher->removeEventListenersForTarget(this, true);

		_eventDispatcher->removeAllEventListeners();//移除所有监听事件

	}else{
		//actorSpr->getPhysicsBody()->setCategoryBitmask(2);
		//actorSpr->getPhysicsBody()->setContactTestBitmask(1);
	}
}

//切换关卡
void GameScene::shiftLevel(float delta){
	setLeaving(true);//正在离开当前场景

	auto maskLayer=Director::getInstance()->getRunningScene()->getChildByTag((int)E_NODE_TAGS::MASK_LAYER);
	maskLayer->runAction(
			Sequence::create(
			FadeIn::create(fadeDuration/2),
			CallFunc::create(CC_CALLBACK_0(GameScene::shiftLevel2,this)),
			nullptr
			)
	);
}

void GameScene::shiftLevel2(){
	nextLevel();//下一关
	//auto scene=GameScene::createScene();
	//auto layer=dynamic_cast<GameScene*>(scene->getChildByTag((int)E_NODE_TAGS::ROOT_LAYER));
	removeChild(GlobalVar::_scoreNode);
	CCLOGERROR("shiftLevel2.GlobalVar::_scoreNode->getReferenceCount()=%d",GlobalVar::_scoreNode->getReferenceCount());
	Director::getInstance()->replaceScene(GameScene::createScene());
}

//物理碰撞检测
bool GameScene::onContactTrapBegin(const PhysicsContact& contact)  
{  
	Node* sprA=nullptr;
	Node* sprB=nullptr;
	std::string nameA;
	std::string nameB;
	try{
		auto bodyA=contact.getShapeA()->getBody();
		auto bodyB=contact.getShapeB()->getBody();

		CCLOGERROR("A0:%d,A1:%d;B0:%d,B1:%d",bodyA->getCategoryBitmask(), bodyA->getContactTestBitmask(),bodyB->getCategoryBitmask(), bodyB->getContactTestBitmask());

		sprA=bodyA->getNode();
		sprB=bodyB->getNode();
		if(sprA==nullptr || sprB==nullptr)return false;
		nameA=sprA->getName();
		nameB=sprB->getName();
	}
	catch(...){//上述代码可能引发不可预料的指针错误，估计是cocos2d-x的bug，无奈
		CCLOGERROR("cocos2d-x bug! now return it for avoid abort.");
		return false;
	}
	
	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();

	//A,B有一个是地面陷阱（另一个必为角色）
	if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){
		
		Node* sprTrap=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0){
			sprTrap=sprA;
		}else{
			sprTrap=sprB;
		}

		sprTrap->getPhysicsBody()->setContactTestBitmask(0);//不再触发碰撞回调
		//扣血
		int healthValue=(int)sprTrap->getUserData();
		if(healthValue==6 || healthValue==8){//6,8号陷阱特别扣血
			healthValue=30;
		}else{
			healthValue=20;
		}
		
		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		
		int healthPercent=GlobalVar::_scoreNode->deductHealth(healthValue);//扣血条
		if(healthPercent<=0){//没血了，游戏结束
			gameOver();
		}else{
			//陷阱碰撞处理
			trapCollideEvent(sprTrap);
		}
	
	}//A,B有一个是钱币（另一个必为角色）
	else if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){

		audioEng->playEffect("sounds/jinbi.mp3");//播放音效

		//更新记分牌
		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		GlobalVar::_scoreNode->updateScore(10);
		//钱币精灵
		Node* sprCoin=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0){
			sprCoin=sprA;
		}else{
			sprCoin=sprB;
		}
		sprCoin->getPhysicsBody()->setContactTestBitmask(0);//不再触发碰撞回调

		Vec2 pos=sprCoin->convertToWorldSpaceAR(Point::ZERO);
		MyParticleCache::getInstance()->play(this,pos,EFFECT_EAT_GOLD);

		sprCoin->setVisible(false);//getParent()->removeChild(sprCoin);//移除自身
	}
	//A,B有一个是空中陷阱（另一个必为角色）
	else if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){

		Node* sprTrap=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0){
			sprTrap=sprA;
		}else{
			sprTrap=sprB;
		}

		sprTrap->getPhysicsBody()->setContactTestBitmask(0);//不再触发碰撞回调
		//扣血
		int healthValue=(int)sprTrap->getUserData();
		healthValue=20;

		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		int healthPercent=GlobalVar::_scoreNode->deductHealth(healthValue);//扣血条
		if(healthPercent<=0){//没血了，游戏结束
			gameOver();
		}else{
			//空中陷阱碰撞处理
			auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
			actorSpr->updateStatus(E_ACTOR_STATUS::FALL_BACK);
		}
	}
	return false;  
}


//在定时器中调用自定义的函数
//Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(GameScene::updateActor,this, E_ACTOR_STATUS::FALL_AHEAD), 
//			this,/*interval*/0.0f,/*repeat*/0,/*delay*/0.0f,/*pause*/false,/*key*/StringUtils::format("GameScene::updateActor.%d",++_callback_idx));
/*void GameScene::updateActor(float delta,E_ACTOR_STATUS status){
	//切换前扑摔倒动画
	actorSpr->updateStatus(status,m_roadSpeed);
}*/

int GameScene::shiftTrapId2ParticleId(int trapId){
	switch (trapId)
	{
	case 1:{
		return EFFECT_WOOD_BOMB;  
		break;
	}
	case 2:{
		return EFFECT_WOOD_BOMB;
		//particle = ParticleSystemQuad::create("lz/woodbreak.plist");
		break;
	}
	case 3:{
		return EFFECT_STONE_BOMB;
		//particle = ParticleSystemQuad::create("lz/stonebreak.plist");
		break;
	}
	case 4:{
		return EFFECT_OIL;
		//particle = ParticleSystemQuad::create("lz/bigfire.plist");
		break;
	}
	case 5:{
		return EFFECT_STONE_BOMB;
		//particle = ParticleSystemQuad::create("lz/stonebreak2.plist");
		break;
	}
	case 6:{
		return EFFECT_WOOD_BOMB;
		//particle = ParticleSystemQuad::create("lz/woodbreak3.plist");
		break;
	}
	case 7:{
		return EFFECT_GLASS_BOMB;
		//particle = ParticleSystemQuad::create("lz/glassbreak.plist");
		break;
	}
	case 8:{
		//particle = ParticleSystemQuad::create("lz/woodbreak2.plist");
		break;
	}
	default:
		break;
	}
	return -1;
}

//地面陷阱碰撞处理
void GameScene::trapCollideEvent(Node* sender){

	int tag=(int)sender->getUserData();//陷阱编号
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(tag==8){//陷阱
		//切换受伤动画
		actorSpr->updateStatus(E_ACTOR_STATUS::INJURE);
	}
	else if(tag==4){//油桶
		Vec2 pos=Vec2(80,60);
		MyParticleCache::getInstance()->play(sender,pos,shiftTrapId2ParticleId(tag));//粒子效果
		actorSpr->updateStatus(E_ACTOR_STATUS::FALL_AHEAD);
	}
	else{
		Vec2 pos=sender->getPosition();
		MyParticleCache::getInstance()->play(this,pos,shiftTrapId2ParticleId(tag));
		sender->setVisible(false);//隐藏障碍

		//切换前扑摔倒动画
		actorSpr->updateStatus(E_ACTOR_STATUS::FALL_AHEAD);
	}
}

//碰撞检测已改为物理系统碰撞检测
void GameScene::update(float delta){
	if(GlobalVar::_scoreNode!=nullptr){
		GlobalVar::_scoreNode->updateDistance(1);
	}

	/*auto sprPerson=(RoleSprite*)this->getChildByTag((int)E_NODE_TAGS::SPR_PERSON);

	Rect rectPerson=sprPerson->getBoundingBox();
	Rect rectPerson2=Rect(rectPerson.getMinX(),rectPerson.getMinY(),rectPerson.size.width/3,rectPerson.size.height);//人物 透明区域修正
	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();

	//auto tempSpr=Sprite::create();
	for(Sprite* item : v_traps){
	Rect itemRect=item->getBoundingBox();
	Rect itemRect2=Rect(itemRect.getMinX()+30,itemRect.getMinY(),itemRect.size.width-50,itemRect.size.height-10);//障碍 透明区域修正
	if(rectPerson2.intersectsRect(itemRect2)){//人物与陷阱发生碰撞
	CCLOGERROR("the person collided a trap! and the traps size is:%d", v_traps.size());

	audioEng->pauseBackgroundMusic();//停止背景音乐
	int soundID_0 = audioEng->playEffect("sounds/siwang.mp3");//播放音效

	//扣血
	int healthValue=(int)item->getUserData() * 5;
	healthPercent-=healthValue;
	auto progressBar=dynamic_cast<ui::LoadingBar*>(this->getChildByTag((int)E_NODE_TAGS::SPR_ProgressBar));
	CCASSERT(progressBar!=NULL,"GameScene::update.progressBar is null!");
	if(healthPercent<=0){//没血了，游戏结束
	progressBar->setPercent(0);
	//切到GameOver
	auto scene=TransitionFade::create(0.5, OverScene::createScene());
	Director::getInstance()->replaceScene(scene);
	}else{//扣血条
	progressBar->setPercent(healthPercent);
	trapCollideEvent();
	}

	v_traps.eraseObject(item);//从vector删除

	CCLOGERROR("the trap is erased from its vector! and the traps size is:%d", v_traps.size());

	break;
	}
	}*/

	//人物与钱币碰撞检测
	/*Node* coinsLayer=(Node*)this->getChildByTag((int)E_NODE_TAGS::SPR_LAYER_COINS);
	CCASSERT(coinsLayer!=NULL, "the coins layer can't be null.");


	for(Sprite* item : v_coins){

	Rect itemRect=item->getBoundingBox();//获得旋转、变换等处理之后的实际大小
	Vec2 layerPoint=coinsLayer->getPosition();
	Rect itemRect2=Rect(layerPoint.x+itemRect.getMinX(),layerPoint.y+itemRect.getMinY(),itemRect.size.width,itemRect.size.height);//坐标系转换
	//auto coin_wp=item->convertToWorldSpaceAR(Vec2(0,-itemRect.size.height/2));//钱币的(0,0)点的(锚点(0,0.5))的世界坐标
	//auto itemRect2=Rect(coin_wp.x, coin_wp.y, itemRect.size.width,itemRect.size.height);

	if(rectPerson.intersectsRect(itemRect2)){//人物与钱币发生碰撞

	CCLOGERROR("the person collided a coin!");

	audioEng->playEffect("sounds/jinbi.mp3");//播放音效

	//更新记分牌
	updateScore(10);

	//创建加分动画
	Label* label=Label::createWithTTF("+10","fonts/Marker Felt.ttf",60);
	label->setColor(Color3B::ORANGE);//橙色
	label->setAnchorPoint(Vec2(0.5,0));
	label->setPosition(Vec2(rectPerson.getMidX(),rectPerson.getMaxY()+20));
	label->setZOrder(4);
	this->addChild(label);
	float anmiTime=0.1;//动画时间
	//auto fadeOut=FadeOut::create(anmiTime);
	auto moveBy=MoveBy::create(anmiTime, Vec2(0,100));
	//auto spa=Spawn::create(fadeOut, moveBy, NULL);//并行动作
	auto seq=Sequence::create(//spa,
	moveBy,
	CallFuncN::create(CC_CALLBACK_1(GameScene::onScoreAnmiDone, this)),
	NULL);//串行动作
	label->runAction(seq);


	//钱币碰撞的粒子效果
	auto coins_collided = ParticleSystemQuad::create("images/lz/coins_collided.plist");  
	coins_collided->setAnchorPoint(Vec2(0,0.5));
	coins_collided->setPosition(Vec2(rectPerson.getMaxX(),rectPerson.getMidY()));	//item->getPosition()
	coins_collided->setPositionType(kCCPositionTypeRelative);//所有粒子随坐标系进行相对位移
	this->addChild(coins_collided);
	coins_collided->setAutoRemoveOnFinish(true);//设置粒子生命周期结束后自动销毁

	item->setVisible(false);//隐藏

	break;
	}
	}
	*/


}

//恢复播放背景音乐
void GameScene::resumeBackgroundMusic(Ref* sender){
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void GameScene::initMap(){
	auto mapNode=MapNode::create();
	mapNode->setPosition(Vec2::ZERO);
	mapNode->setAnchorPoint(Vec2(0,0));
	mapNode->setTag((int)E_NODE_TAGS::MAP);
	mapNode->setLocalZOrder((int)E_ZORDERS::MAP);
	this->addChild(mapNode);
}

void GameScene::initScore(){
	if(GlobalVar::_scoreNode==nullptr){
		GlobalVar::_scoreNode=ScoreNode::create();
		GlobalVar::_scoreNode->setPosition(Vec2::ZERO);
		GlobalVar::_scoreNode->setAnchorPoint(Vec2(0,0));
		//GlobalVar::_scoreNode->setTag((int)E_NODE_TAGS::SCORE);
		GlobalVar::_scoreNode->setLocalZOrder((int)E_ZORDERS::SCORE);
		GlobalVar::_scoreNode->retain();
	}
	this->addChild(GlobalVar::_scoreNode);
}

void GameScene::gameOver(){
	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();
	audioEng->stopBackgroundMusic();
	audioEng->playEffect("sounds/siwang.mp3");

	int dist=GlobalVar::_scoreNode->getDistance();
	int coins=GlobalVar::_scoreNode->getCoins();
	//removeChildByTag(int(E_NODE_TAGS::SCORE));//销毁计分板对象
	GlobalVar::_scoreNode->release();
	CCLOGERROR("GlobalVar::_scoreNode->getReferenceCount()=%d",GlobalVar::_scoreNode->getReferenceCount());
	//GlobalVar::_scoreNode->removeFromParentAndCleanup(true);
	GlobalVar::_scoreNode=nullptr;
	GlobalVar::_level=1;//重置关卡，从第一关开始
	auto scene=TransitionFade::create(0.5, OverScene::createScene(dist, coins));
	Director::getInstance()->replaceScene(scene);
}

//初始化人物
void GameScene::initPersonSpr(){
	auto actorSpr=ActorSprite::createWithSpriteFrameName("jump_14.png");//runlast_1
	actorSpr->setTag((int)E_NODE_TAGS::ACTOR);
	actorSpr->setName(MACRO_ENUM2STR(E_NODE_NAMES::ROLE));//标识为人物角色
	actorSpr->setPosition(Point(winSize.width/4,95+actorSpr->getBoundingBox().size.height/2));
	actorSpr->setLocalZOrder((int)E_ZORDERS::ACTOR);//
	this->addChild(actorSpr);

	actorSpr->updateStatus(E_ACTOR_STATUS::RUN);
}

//触屏开始时调用
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
	posOfTouch=touch->getLocation(); 

	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	posOfPersonOnTouch=actorSpr->getPosition();

	return true;
}

//手指触屏并在屏幕上移动时调用
void GameScene::onTouchMoved(Touch *touch, Event *unused_event){

}

//触摸动作完成且手指离开屏幕时调用
void GameScene::onTouchEnded(Touch *touch, Event *unused_event){
	Point location=touch->getLocation();
	float jumpHeight=location.y-posOfTouch.y;//划屏高度
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(jumpHeight>0){//起跳
		actorSpr->updateStatus(E_ACTOR_STATUS::JUMP,jumpHeight);
		//Director::getInstance()->resume();
	}else if(jumpHeight<0){//俯身滑行
		actorSpr->updateStatus(E_ACTOR_STATUS::SLIDE,jumpHeight);
		//Director::getInstance()->pause();
	}
}

//缓存钱币帧动画
void GameScene::initCoinAnim()
{
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/12);
	for(int j=0;j<12;++j){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("coin_%d.png",j+1));
		animation->addSpriteFrame(frame);
	}

	AnimationCache::getInstance()->addAnimation(animation, "coinsAnim");
}