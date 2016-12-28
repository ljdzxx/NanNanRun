#include "ActorSprite.h"
#include "GlobalVar.h"
#include "MyBodyParser.h"

USING_NS_CC;
using namespace CocosDenshion;

bool ActorSprite::init2(){
	initRunAnim();//��
	initJumpAnim();//��
	initInjuredAnim();//����
	initSlideAnim();//����
	initFallBackAnim();//����
	initFallAheadAnim();//ǰ��

	return true;
}

//��ʼ���ܲ�����
void ActorSprite::initRunAnim(){
	//createLastRunAction();

	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/48);//1.0/48
	

	for(int i=0;i<24;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("runlast_%d.png",i+1));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::RUN));
}

//��ʼ����Ծ����
void ActorSprite::initJumpAnim(){
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/14);//1.0f/14

	for(int i=0;i<14;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("jump_%d.png",i+1));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::JUMP));
}

//ǰ��ˤ�Ӷ���
void ActorSprite::initFallAheadAnim()
{
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/12);//1.0f/90

	for(int i=1;i<=6;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fall_%d.png",i));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::FALL_AHEAD));
}

//����ˤ�Ӷ���
void ActorSprite::initFallBackAnim()
{
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/12);//1.0f/90

	for(int i=1;i<=3;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("slide_%d.png",i));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::FALL_BACK));
}

//��ʼ�����ж���
void ActorSprite::initSlideAnim(){
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/11);//

	for(int i=1;i<=3;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("slide_%d.png",i));
		animation->addSpriteFrame(frame);
	}

	for(int i=0;i<4;++i){
		for(int j=4;j<=5;++j){
			auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("slide_%d.png",j));
			animation->addSpriteFrame(frame);
		}
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::SLIDE));
}

//���˶���
void ActorSprite::initInjuredAnim(){
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/9);//1.0f/90

	for(int i=0;i<9;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("injured_%d.png",i+1));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::INJURE));
}


//���½�ɫ״̬
//float0:��JUMPʱΪ�����߶ȣ���FALL_AHEADʱΪ��ͼ·���ƶ��ٶ�
//
void ActorSprite::updateStatus(E_ACTOR_STATUS status,float float0/*=0.0f*/){
	if(_oldStatus==status)return;//״̬�޸���
	if(status==E_ACTOR_STATUS::JUMP){
		if(_oldStatus!=E_ACTOR_STATUS::RUN)return;//���ܲ�״̬��������
	}
	if(status==E_ACTOR_STATUS::SLIDE){
		if(_oldStatus!=E_ACTOR_STATUS::RUN)return;//���ܲ�״̬�����»�
	}

	
	++_isUpdating;//������±����ô���
	stopActionByTag((int)_oldStatus);//ͣ����ǰ��֡����(����еĻ�)
	_oldStatus=status;

	//SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	auto animation=AnimationCache::getInstance()->getAnimation(MACRO_ENUM2STR(status));
	CCASSERT(animation,"ActorSprite::updateStatus.animation=null.");
	auto animate=Animate::create(animation);
	animate->setTag((int)status);
	
	auto callfunc=CallFunc::create(CC_CALLBACK_0(ActorSprite::update2Default,this));

	switch (status)
	{
	case E_ACTOR_STATUS::RUN:{
		update2Default();
		break;
		}
	case E_ACTOR_STATUS::JUMP:{
		SimpleAudioEngine::getInstance()->playEffect("sounds/jump.wav");//������Ч
		
		float anim_duration=animation->getDuration();
		auto jumpAct=EaseInOut::create(JumpBy::create(anim_duration, Point(0,0), float0,1),0.618);	//1���ӣ�λ��0(ԭ������)���߶�jumpHeight������1��)
		auto sequAct=Sequence::create(
			jumpAct, 
			CallFunc::create(CC_CALLBACK_0(ActorSprite::playAudioJumpDown,this)),
			callfunc, nullptr);

		runAction(animate);//����֡����
		runAction(sequAct);//����������Ϊ

		break;
		}
	case E_ACTOR_STATUS::SLIDE:{
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//���°��������

		SimpleAudioEngine::getInstance()->playEffect("sounds/slide.ogg");//������Ч

		float anim_duration=animation->getDuration();
		auto delay=DelayTime::create(anim_duration);
		auto sequAct=Sequence::create(delay, callfunc, nullptr);

		runAction(animate);//����֡����
		runAction(sequAct);

		break;
		}
	case E_ACTOR_STATUS::INJURE:{
		SimpleAudioEngine::getInstance()->playEffect("sounds/injured.wav");//������Ч
		runAction(animate);
		runAction(Sequence::create(
				//animate,
				DelayTime::create(animation->getDuration()),
				callfunc,
				nullptr
				)
			);
		
		break;
		}
	case E_ACTOR_STATUS::FALL_AHEAD:{
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//���°��������
		SimpleAudioEngine::getInstance()->playEffect("sounds/en.wav");//������Ч

		float back_distance=getPositionX();
		//CCAssert(float0>0,"float0 can't be zero.");0
		float back_duration=back_distance/GlobalVar::_roadSpeed;	//
		//��϶���:ˤſ�º���·��󻬣���ʱһ�ᣬ�ٸ�λ

		runAction(animate);
		runAction(
			Sequence::create(
				//animate,
				//DelayTime::create(back_duration),//��ʱ
				MoveBy::create(back_duration, Vec2(-back_distance,0)),//����
				CallFunc::create(CC_CALLBACK_0(ActorSprite::update2DefaultWithoutPhysics, this, 1.0f/96)),
				EaseIn::create( MoveBy::create(back_duration, Vec2(back_distance,0)),1.1f),//�����¿�ļ���ǰ��
				callfunc,
				nullptr
			)
		);
		
		break;
		}
	case E_ACTOR_STATUS::FALL_BACK:{
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//���°��������
		SimpleAudioEngine::getInstance()->playEffect("sounds/ya.wav");//������Ч

		float back_distance=getPositionX();
		//CCAssert(float0>0,"float0 can't be zero.");
		float back_duration=back_distance/GlobalVar::_roadSpeed;	//

		runAction(animate);
		//��϶���:ˤſ�º���·��󻬣���ʱһ�ᣬ�ٸ�λ
		runAction(
			Sequence::create(
			//animate,
			MoveBy::create(back_duration/2, Vec2(-back_distance,0)),//����
			RotateBy::create(back_duration/2,-360),//��ת
			//DelayTime::create(back_duration*5),//������ʱ
			CallFunc::create(CC_CALLBACK_0(ActorSprite::update2DefaultWithoutPhysics, this, 1.0f/96)),
			EaseIn::create( MoveBy::create(back_duration, Vec2(back_distance,0)),1.1f),//�����¿�ļ���ǰ��
			callfunc,
			nullptr
			)
		);
		
		break;
		}
	default:
		break;
	}
}

void ActorSprite::playAudioJumpDown(){
	SimpleAudioEngine::getInstance()->playEffect("sounds/jump_down.mp3");//������Ч
}

//���ý�ɫΪĬ���ܲ�״̬
void ActorSprite::update2Default(){
	if(--_isUpdating>0){
		CCLOGERROR("_isUpdating>0 ==============================>%d",_isUpdating);
		return;//������2�����϶�����ִ�У���������β���
	}

	//if(_isUpdating){
	//	return;//�������ִ�и��¾�ɶҲ����
	//}else{

		//���°�������ײ�߽�
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::JUMP));

		//��ԭ�ܲ�״̬
		update2DefaultWithoutPhysics();

		
		_isUpdating=false;
	//}
}

void ActorSprite::update2DefaultWithoutPhysics(float delayPerUnit){
	//��ԭ�ܲ�״̬
	stopActionByTag((int)_oldStatus);//ͣ���ոձ�ִ�����֡����(����еĻ�)
	auto animation=AnimationCache::getInstance()->getAnimation(MACRO_ENUM2STR(E_ACTOR_STATUS::RUN));
	animation->setDelayPerUnit(delayPerUnit);
	auto animate=Animate::create(animation);
	auto repeat=RepeatForever::create(animate);
	repeat->setTag((int)E_ACTOR_STATUS::RUN);
	runAction(repeat);
	_oldStatus=E_ACTOR_STATUS::RUN;

	//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

PhysicsBody* ActorSprite::createPhysicsBody(E_PHYSICS_BODYS e){
	switch (e)
	{
	case E_PHYSICS_BODYS::JUMP:{
		//��sprŪ�ɸ��������������ԭͼ
		auto fra=SpriteFrameCache::getInstance()->getSpriteFrameByName("jump_14.png");
		setSpriteFrame(fra);

		//�󶨸���
		MyBodyParser::getInstance()->parseJsonFile("shapes/jump_14.json");
		auto jumpBody = MyBodyParser::getInstance()->bodyFormJson(this, "jump_14");//����ֻ��һ�����������巢����ײʱ��֡����״
		CCASSERT(jumpBody!=nullptr,"ActorSprite::createPhysicsBody.jumpBody is null!");
		//�������岻������ϵ��Ӱ��  
		jumpBody->setGravityEnable(false);
		//������ײ����(����ײ��ֻ����ײ���)
		jumpBody->setCategoryBitmask(2);//10b
		jumpBody->setContactTestBitmask(1);//01b
		jumpBody->setCollisionBitmask(0);
		//���˸���ľ���ê��ֻ����0.5,0.5)
		CCLOGERROR("jumpBody0:%d,jumpBody1:%d",jumpBody->getCategoryBitmask(), jumpBody->getContactTestBitmask());

		return jumpBody;
		break;}
	case E_PHYSICS_BODYS::SLIDE:{
		//��sprŪ�ɸ��������������ԭͼ
		auto fra=SpriteFrameCache::getInstance()->getSpriteFrameByName("slide_5.png");
		setSpriteFrame(fra);

		//���廬��ʱӦ�󶨵ĸ���
		MyBodyParser::getInstance()->parseJsonFile("shapes/slide_5.json");
		auto slideBody = MyBodyParser::getInstance()->bodyFormJson(this, "slide_5");
		CCASSERT(slideBody!=nullptr,"ActorSprite::createPhysicsBody.slideBody is null!");
		//�������岻������ϵ��Ӱ��  
		slideBody->setGravityEnable(false);
		//������ײ����(����ײ��ֻ����ײ���)
		slideBody->setCategoryBitmask(2);//10b
		slideBody->setContactTestBitmask(1);//01b
		slideBody->setCollisionBitmask(0);
		CCLOGERROR("slideBody0:%d,slideBody1:%d",slideBody->getCategoryBitmask(), slideBody->getContactTestBitmask());

		return slideBody;
		break;}
	default:
		break;
	}
}

ActorSprite* ActorSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    ActorSprite *sprite = new (std::nothrow) ActorSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
		sprite->init2();//initWithSpriteFrame()�Ѿ������˶�Sprite�ĳ�ʼ����������Ϊ�˴���һ���Ը���init2����
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ActorSprite* ActorSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

//void ActorSprite::setDisplayFrameWithAnimationName(const std::string& animationName, ssize_t frameIndex){
//	Sprite::setDisplayFrameWithAnimationName(animationName,frameIndex);
//}

ActorSprite::ActorSprite():_oldStatus(E_ACTOR_STATUS::NULL_STATUS),_isUpdating(0)
{
	//int i=0;
}