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

#include "ScoreNode.h"//�������������

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

	//����Debugģʽ����ῴ������ı��汻������Χ����ҪΪ���ڵ����и����׵ع۲�  
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); 

	//��Ϸ����
	auto mainLayer=GameScene::create();//Color4B::BLACK
	mainLayer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(mainLayer,0,(int)E_NODE_TAGS::ROOT_LAYER);

	//����Ч�������ֲ�
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
	std::string levelTitle=DictionaryLoader::getStrByName("gamescene.level.title");//��N��
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

	//��ʱ��
	//this->schedule(schedule_selector(GameScene::scheduleCallBack), 0.1);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/bgm_super.ogg",true);//�ܲ���Ч

	initMap();//z-order:1
	initScore();//z-order:3
	initCoinAnim();//�������ͽ�Ҳ㶼���õ���Ҷ���
	
	return true;
}

void GameScene::init2(){
	initPersonSpr();

	//�����¼����-------------------------------------------------------------------------
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(GameScene::onTouchBegan,this);
	//listener->onTouchMoved=CC_CALLBACK_2(GameScene::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(GameScene::onTouchEnded, this);
	//��һ��ǳ���Ҫ��FixedPriority listener������֮����Ҫ�ֶ�remove����SceneGraphPriority listener�Ǹ�node�󶨵ģ���node�����������лᱻ�Ƴ���
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,1);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	//������ײ���-----------------------------------------------------------------
	//��Ӽ�����  
	auto contactListener=EventListenerPhysicsContact::create();  
	//���ü���������ײ��ʼ����  
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactTrapBegin, this);  //������false��Chipmunk����ȫ������ײ��preSolve()��postSolve()�ص�����Զ���ᱻִ�У�������Ȼ������״ֹͣ�ص���ʱ����յ�һ���������¼���
	//��ӵ��¼��ַ�����  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//��ʱ���ؿ�
	this->schedule(schedule_selector(GameScene::shiftLevel), GlobalVar::_timePerLevel);

	//ÿ֡����update����
	this->scheduleUpdate();
	
	//initCoin();
	initTrap();
	
	if(GlobalVar::_level%9==0){//ÿ9������һ�����ף�ϵͳ����
		Director::getInstance()->getScheduler()->setTimeScale(1.0f+(GlobalVar::_level-9)*0.1);
	}

	auto lbLevel=getLevelLabel(20.0f);
	lbLevel->setLocalZOrder((int)E_ZORDERS::SCORE);
	lbLevel->setPosition(winSize.width/2, winSize.height-70);
	addChild(lbLevel);

	setLeaving(false);
	CCLOGERROR("GameScene is initialized.");
}

//����Ǯ�Ҳ�
//void GameScene::initCoin(){	
//	auto coinsNode=CoinsNode::create();
//	//coinsNode->setTag((int)E_NODE_TAGS::NODE_COINS);
//	coinsNode->setAnchorPoint(Vec2(0,0));
//	coinsNode->setPosition(Vec2::ZERO);
//	coinsNode->setLocalZOrder((int)E_ZORDERS::COIN);	//z-order:4
//	addChild(coinsNode);
//}

//���������
void GameScene::initTrap(){
	auto trapsNode=TrapsNode::create();
	trapsNode->setPosition(Vec2::ZERO);
	trapsNode->setAnchorPoint(Vec2::ZERO);
	trapsNode->setLocalZOrder((int)E_ZORDERS::TRAP);	//z-order:2
	addChild(trapsNode);
}

void GameScene::setLeaving(bool flag){
	GlobalVar::_isLeaving=flag;
	//����Actor����ײ����
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(flag){
		//actorSpr->getPhysicsBody()->setCategoryBitmask(0);
		//actorSpr->getPhysicsBody()->setContactTestBitmask(0);//���л��������ȵĽ׶β�������ײ���
		
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
		//_eventDispatcher->removeEventListenersForTarget(this, true);

		_eventDispatcher->removeAllEventListeners();//�Ƴ����м����¼�

	}else{
		//actorSpr->getPhysicsBody()->setCategoryBitmask(2);
		//actorSpr->getPhysicsBody()->setContactTestBitmask(1);
	}
}

//�л��ؿ�
void GameScene::shiftLevel(float delta){
	setLeaving(true);//�����뿪��ǰ����

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
	nextLevel();//��һ��
	//auto scene=GameScene::createScene();
	//auto layer=dynamic_cast<GameScene*>(scene->getChildByTag((int)E_NODE_TAGS::ROOT_LAYER));
	removeChild(GlobalVar::_scoreNode);
	CCLOGERROR("shiftLevel2.GlobalVar::_scoreNode->getReferenceCount()=%d",GlobalVar::_scoreNode->getReferenceCount());
	Director::getInstance()->replaceScene(GameScene::createScene());
}

//������ײ���
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
	catch(...){//�������������������Ԥ�ϵ�ָ����󣬹�����cocos2d-x��bug������
		CCLOGERROR("cocos2d-x bug! now return it for avoid abort.");
		return false;
	}
	
	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();

	//A,B��һ���ǵ������壨��һ����Ϊ��ɫ��
	if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){
		
		Node* sprTrap=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::TRAP))==0){
			sprTrap=sprA;
		}else{
			sprTrap=sprB;
		}

		sprTrap->getPhysicsBody()->setContactTestBitmask(0);//���ٴ�����ײ�ص�
		//��Ѫ
		int healthValue=(int)sprTrap->getUserData();
		if(healthValue==6 || healthValue==8){//6,8�������ر��Ѫ
			healthValue=30;
		}else{
			healthValue=20;
		}
		
		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		
		int healthPercent=GlobalVar::_scoreNode->deductHealth(healthValue);//��Ѫ��
		if(healthPercent<=0){//ûѪ�ˣ���Ϸ����
			gameOver();
		}else{
			//������ײ����
			trapCollideEvent(sprTrap);
		}
	
	}//A,B��һ����Ǯ�ң���һ����Ϊ��ɫ��
	else if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){

		audioEng->playEffect("sounds/jinbi.mp3");//������Ч

		//���¼Ƿ���
		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		GlobalVar::_scoreNode->updateScore(10);
		//Ǯ�Ҿ���
		Node* sprCoin=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::COIN))==0){
			sprCoin=sprA;
		}else{
			sprCoin=sprB;
		}
		sprCoin->getPhysicsBody()->setContactTestBitmask(0);//���ٴ�����ײ�ص�

		Vec2 pos=sprCoin->convertToWorldSpaceAR(Point::ZERO);
		MyParticleCache::getInstance()->play(this,pos,EFFECT_EAT_GOLD);

		sprCoin->setVisible(false);//getParent()->removeChild(sprCoin);//�Ƴ�����
	}
	//A,B��һ���ǿ������壨��һ����Ϊ��ɫ��
	else if( /*(*/nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0 || /*nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0) && */
		/*(*/nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0 /*|| nameB.compare(MACRO_ENUM2STR(E_NODE_NAMES::ROLE))==0)*/ ){

		Node* sprTrap=nullptr;
		if(nameA.compare(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP))==0){
			sprTrap=sprA;
		}else{
			sprTrap=sprB;
		}

		sprTrap->getPhysicsBody()->setContactTestBitmask(0);//���ٴ�����ײ�ص�
		//��Ѫ
		int healthValue=(int)sprTrap->getUserData();
		healthValue=20;

		//auto scoreNode=(ScoreNode*)(this->getChildByTag((int)E_NODE_TAGS::SCORE));
		int healthPercent=GlobalVar::_scoreNode->deductHealth(healthValue);//��Ѫ��
		if(healthPercent<=0){//ûѪ�ˣ���Ϸ����
			gameOver();
		}else{
			//����������ײ����
			auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
			actorSpr->updateStatus(E_ACTOR_STATUS::FALL_BACK);
		}
	}
	return false;  
}


//�ڶ�ʱ���е����Զ���ĺ���
//Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(GameScene::updateActor,this, E_ACTOR_STATUS::FALL_AHEAD), 
//			this,/*interval*/0.0f,/*repeat*/0,/*delay*/0.0f,/*pause*/false,/*key*/StringUtils::format("GameScene::updateActor.%d",++_callback_idx));
/*void GameScene::updateActor(float delta,E_ACTOR_STATUS status){
	//�л�ǰ��ˤ������
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

//����������ײ����
void GameScene::trapCollideEvent(Node* sender){

	int tag=(int)sender->getUserData();//������
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(tag==8){//����
		//�л����˶���
		actorSpr->updateStatus(E_ACTOR_STATUS::INJURE);
	}
	else if(tag==4){//��Ͱ
		Vec2 pos=Vec2(80,60);
		MyParticleCache::getInstance()->play(sender,pos,shiftTrapId2ParticleId(tag));//����Ч��
		actorSpr->updateStatus(E_ACTOR_STATUS::FALL_AHEAD);
	}
	else{
		Vec2 pos=sender->getPosition();
		MyParticleCache::getInstance()->play(this,pos,shiftTrapId2ParticleId(tag));
		sender->setVisible(false);//�����ϰ�

		//�л�ǰ��ˤ������
		actorSpr->updateStatus(E_ACTOR_STATUS::FALL_AHEAD);
	}
}

//��ײ����Ѹ�Ϊ����ϵͳ��ײ���
void GameScene::update(float delta){
	if(GlobalVar::_scoreNode!=nullptr){
		GlobalVar::_scoreNode->updateDistance(1);
	}

	/*auto sprPerson=(RoleSprite*)this->getChildByTag((int)E_NODE_TAGS::SPR_PERSON);

	Rect rectPerson=sprPerson->getBoundingBox();
	Rect rectPerson2=Rect(rectPerson.getMinX(),rectPerson.getMinY(),rectPerson.size.width/3,rectPerson.size.height);//���� ͸����������
	SimpleAudioEngine* audioEng=SimpleAudioEngine::getInstance();

	//auto tempSpr=Sprite::create();
	for(Sprite* item : v_traps){
	Rect itemRect=item->getBoundingBox();
	Rect itemRect2=Rect(itemRect.getMinX()+30,itemRect.getMinY(),itemRect.size.width-50,itemRect.size.height-10);//�ϰ� ͸����������
	if(rectPerson2.intersectsRect(itemRect2)){//���������巢����ײ
	CCLOGERROR("the person collided a trap! and the traps size is:%d", v_traps.size());

	audioEng->pauseBackgroundMusic();//ֹͣ��������
	int soundID_0 = audioEng->playEffect("sounds/siwang.mp3");//������Ч

	//��Ѫ
	int healthValue=(int)item->getUserData() * 5;
	healthPercent-=healthValue;
	auto progressBar=dynamic_cast<ui::LoadingBar*>(this->getChildByTag((int)E_NODE_TAGS::SPR_ProgressBar));
	CCASSERT(progressBar!=NULL,"GameScene::update.progressBar is null!");
	if(healthPercent<=0){//ûѪ�ˣ���Ϸ����
	progressBar->setPercent(0);
	//�е�GameOver
	auto scene=TransitionFade::create(0.5, OverScene::createScene());
	Director::getInstance()->replaceScene(scene);
	}else{//��Ѫ��
	progressBar->setPercent(healthPercent);
	trapCollideEvent();
	}

	v_traps.eraseObject(item);//��vectorɾ��

	CCLOGERROR("the trap is erased from its vector! and the traps size is:%d", v_traps.size());

	break;
	}
	}*/

	//������Ǯ����ײ���
	/*Node* coinsLayer=(Node*)this->getChildByTag((int)E_NODE_TAGS::SPR_LAYER_COINS);
	CCASSERT(coinsLayer!=NULL, "the coins layer can't be null.");


	for(Sprite* item : v_coins){

	Rect itemRect=item->getBoundingBox();//�����ת���任�ȴ���֮���ʵ�ʴ�С
	Vec2 layerPoint=coinsLayer->getPosition();
	Rect itemRect2=Rect(layerPoint.x+itemRect.getMinX(),layerPoint.y+itemRect.getMinY(),itemRect.size.width,itemRect.size.height);//����ϵת��
	//auto coin_wp=item->convertToWorldSpaceAR(Vec2(0,-itemRect.size.height/2));//Ǯ�ҵ�(0,0)���(ê��(0,0.5))����������
	//auto itemRect2=Rect(coin_wp.x, coin_wp.y, itemRect.size.width,itemRect.size.height);

	if(rectPerson.intersectsRect(itemRect2)){//������Ǯ�ҷ�����ײ

	CCLOGERROR("the person collided a coin!");

	audioEng->playEffect("sounds/jinbi.mp3");//������Ч

	//���¼Ƿ���
	updateScore(10);

	//�����ӷֶ���
	Label* label=Label::createWithTTF("+10","fonts/Marker Felt.ttf",60);
	label->setColor(Color3B::ORANGE);//��ɫ
	label->setAnchorPoint(Vec2(0.5,0));
	label->setPosition(Vec2(rectPerson.getMidX(),rectPerson.getMaxY()+20));
	label->setZOrder(4);
	this->addChild(label);
	float anmiTime=0.1;//����ʱ��
	//auto fadeOut=FadeOut::create(anmiTime);
	auto moveBy=MoveBy::create(anmiTime, Vec2(0,100));
	//auto spa=Spawn::create(fadeOut, moveBy, NULL);//���ж���
	auto seq=Sequence::create(//spa,
	moveBy,
	CallFuncN::create(CC_CALLBACK_1(GameScene::onScoreAnmiDone, this)),
	NULL);//���ж���
	label->runAction(seq);


	//Ǯ����ײ������Ч��
	auto coins_collided = ParticleSystemQuad::create("images/lz/coins_collided.plist");  
	coins_collided->setAnchorPoint(Vec2(0,0.5));
	coins_collided->setPosition(Vec2(rectPerson.getMaxX(),rectPerson.getMidY()));	//item->getPosition()
	coins_collided->setPositionType(kCCPositionTypeRelative);//��������������ϵ�������λ��
	this->addChild(coins_collided);
	coins_collided->setAutoRemoveOnFinish(true);//���������������ڽ������Զ�����

	item->setVisible(false);//����

	break;
	}
	}
	*/


}

//�ָ����ű�������
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
	//removeChildByTag(int(E_NODE_TAGS::SCORE));//���ټƷְ����
	GlobalVar::_scoreNode->release();
	CCLOGERROR("GlobalVar::_scoreNode->getReferenceCount()=%d",GlobalVar::_scoreNode->getReferenceCount());
	//GlobalVar::_scoreNode->removeFromParentAndCleanup(true);
	GlobalVar::_scoreNode=nullptr;
	GlobalVar::_level=1;//���ùؿ����ӵ�һ�ؿ�ʼ
	auto scene=TransitionFade::create(0.5, OverScene::createScene(dist, coins));
	Director::getInstance()->replaceScene(scene);
}

//��ʼ������
void GameScene::initPersonSpr(){
	auto actorSpr=ActorSprite::createWithSpriteFrameName("jump_14.png");//runlast_1
	actorSpr->setTag((int)E_NODE_TAGS::ACTOR);
	actorSpr->setName(MACRO_ENUM2STR(E_NODE_NAMES::ROLE));//��ʶΪ�����ɫ
	actorSpr->setPosition(Point(winSize.width/4,95+actorSpr->getBoundingBox().size.height/2));
	actorSpr->setLocalZOrder((int)E_ZORDERS::ACTOR);//
	this->addChild(actorSpr);

	actorSpr->updateStatus(E_ACTOR_STATUS::RUN);
}

//������ʼʱ����
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
	posOfTouch=touch->getLocation(); 

	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	posOfPersonOnTouch=actorSpr->getPosition();

	return true;
}

//��ָ����������Ļ���ƶ�ʱ����
void GameScene::onTouchMoved(Touch *touch, Event *unused_event){

}

//���������������ָ�뿪��Ļʱ����
void GameScene::onTouchEnded(Touch *touch, Event *unused_event){
	Point location=touch->getLocation();
	float jumpHeight=location.y-posOfTouch.y;//�����߶�
	auto actorSpr=(ActorSprite*)getChildByTag((int)E_NODE_TAGS::ACTOR);
	if(jumpHeight>0){//����
		actorSpr->updateStatus(E_ACTOR_STATUS::JUMP,jumpHeight);
		//Director::getInstance()->resume();
	}else if(jumpHeight<0){//������
		actorSpr->updateStatus(E_ACTOR_STATUS::SLIDE,jumpHeight);
		//Director::getInstance()->pause();
	}
}

//����Ǯ��֡����
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