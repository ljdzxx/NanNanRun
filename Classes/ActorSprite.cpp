#include "ActorSprite.h"
#include "GlobalVar.h"
#include "MyBodyParser.h"

USING_NS_CC;
using namespace CocosDenshion;

bool ActorSprite::init2(){
	initRunAnim();//跑
	initJumpAnim();//跳
	initInjuredAnim();//受伤
	initSlideAnim();//滑行
	initFallBackAnim();//后仰
	initFallAheadAnim();//前扑

	return true;
}

//初始化跑步动画
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

//初始化跳跃动作
void ActorSprite::initJumpAnim(){
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/14);//1.0f/14

	for(int i=0;i<14;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("jump_%d.png",i+1));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::JUMP));
}

//前扑摔跤动画
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

//后仰摔跤动画
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

//初始化滑行动作
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

//受伤动画
void ActorSprite::initInjuredAnim(){
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0f/9);//1.0f/90

	for(int i=0;i<9;++i){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("injured_%d.png",i+1));
		animation->addSpriteFrame(frame);
	}
	AnimationCache::getInstance()->addAnimation(animation, MACRO_ENUM2STR(E_ACTOR_STATUS::INJURE));
}


//更新角色状态
//float0:在JUMP时为起跳高度，在FALL_AHEAD时为地图路面移动速度
//
void ActorSprite::updateStatus(E_ACTOR_STATUS status,float float0/*=0.0f*/){
	if(_oldStatus==status)return;//状态无更改
	if(status==E_ACTOR_STATUS::JUMP){
		if(_oldStatus!=E_ACTOR_STATUS::RUN)return;//非跑步状态不能跳起
	}
	if(status==E_ACTOR_STATUS::SLIDE){
		if(_oldStatus!=E_ACTOR_STATUS::RUN)return;//非跑步状态不能下滑
	}

	
	++_isUpdating;//代表更新被调用次数
	stopActionByTag((int)_oldStatus);//停掉先前的帧动画(如果有的话)
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
		SimpleAudioEngine::getInstance()->playEffect("sounds/jump.wav");//播放音效
		
		float anim_duration=animation->getDuration();
		auto jumpAct=EaseInOut::create(JumpBy::create(anim_duration, Point(0,0), float0,1),0.618);	//1秒钟，位移0(原地起跳)，高度jumpHeight，起跳1次)
		auto sequAct=Sequence::create(
			jumpAct, 
			CallFunc::create(CC_CALLBACK_0(ActorSprite::playAudioJumpDown,this)),
			callfunc, nullptr);

		runAction(animate);//播放帧动画
		runAction(sequAct);//其他动作行为

		break;
		}
	case E_ACTOR_STATUS::SLIDE:{
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//重新绑定物理刚体

		SimpleAudioEngine::getInstance()->playEffect("sounds/slide.ogg");//播放音效

		float anim_duration=animation->getDuration();
		auto delay=DelayTime::create(anim_duration);
		auto sequAct=Sequence::create(delay, callfunc, nullptr);

		runAction(animate);//播放帧动画
		runAction(sequAct);

		break;
		}
	case E_ACTOR_STATUS::INJURE:{
		SimpleAudioEngine::getInstance()->playEffect("sounds/injured.wav");//播放音效
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
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//重新绑定物理刚体
		SimpleAudioEngine::getInstance()->playEffect("sounds/en.wav");//播放音效

		float back_distance=getPositionX();
		//CCAssert(float0>0,"float0 can't be zero.");0
		float back_duration=back_distance/GlobalVar::_roadSpeed;	//
		//组合动作:摔趴下后随路面后滑，延时一会，再复位

		runAction(animate);
		runAction(
			Sequence::create(
				//animate,
				//DelayTime::create(back_duration),//延时
				MoveBy::create(back_duration, Vec2(-back_distance,0)),//后移
				CallFunc::create(CC_CALLBACK_0(ActorSprite::update2DefaultWithoutPhysics, this, 1.0f/96)),
				EaseIn::create( MoveBy::create(back_duration, Vec2(back_distance,0)),1.1f),//由慢致快的加速前移
				callfunc,
				nullptr
			)
		);
		
		break;
		}
	case E_ACTOR_STATUS::FALL_BACK:{
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::SLIDE));//重新绑定物理刚体
		SimpleAudioEngine::getInstance()->playEffect("sounds/ya.wav");//播放音效

		float back_distance=getPositionX();
		//CCAssert(float0>0,"float0 can't be zero.");
		float back_duration=back_distance/GlobalVar::_roadSpeed;	//

		runAction(animate);
		//组合动作:摔趴下后随路面后滑，延时一会，再复位
		runAction(
			Sequence::create(
			//animate,
			MoveBy::create(back_duration/2, Vec2(-back_distance,0)),//后移
			RotateBy::create(back_duration/2,-360),//旋转
			//DelayTime::create(back_duration*5),//短暂延时
			CallFunc::create(CC_CALLBACK_0(ActorSprite::update2DefaultWithoutPhysics, this, 1.0f/96)),
			EaseIn::create( MoveBy::create(back_duration, Vec2(back_distance,0)),1.1f),//由慢致快的加速前移
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
	SimpleAudioEngine::getInstance()->playEffect("sounds/jump_down.mp3");//播放音效
}

//设置角色为默认跑步状态
void ActorSprite::update2Default(){
	if(--_isUpdating>0){
		CCLOGERROR("_isUpdating>0 ==============================>%d",_isUpdating);
		return;//至少有2个以上动作在执行，则放弃本次操作
	}

	//if(_isUpdating){
	//	return;//如果正在执行更新就啥也不做
	//}else{

		//重新绑定物理碰撞边界
		setPhysicsBody(createPhysicsBody(E_PHYSICS_BODYS::JUMP));

		//还原跑步状态
		update2DefaultWithoutPhysics();

		
		_isUpdating=false;
	//}
}

void ActorSprite::update2DefaultWithoutPhysics(float delayPerUnit){
	//还原跑步状态
	stopActionByTag((int)_oldStatus);//停掉刚刚被执行完的帧动画(如果有的话)
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
		//把spr弄成刚体绘制所依赖的原图
		auto fra=SpriteFrameCache::getInstance()->getSpriteFrameByName("jump_14.png");
		setSpriteFrame(fra);

		//绑定刚体
		MyBodyParser::getInstance()->parseJsonFile("shapes/jump_14.json");
		auto jumpBody = MyBodyParser::getInstance()->bodyFormJson(this, "jump_14");//这里只绑定一个即将与陷阱发生碰撞时的帧的形状
		CCASSERT(jumpBody!=nullptr,"ActorSprite::createPhysicsBody.jumpBody is null!");
		//设置物体不受重力系数影响  
		jumpBody->setGravityEnable(false);
		//设置碰撞过滤(不碰撞，只做碰撞检测)
		jumpBody->setCategoryBitmask(2);//10b
		jumpBody->setContactTestBitmask(1);//01b
		jumpBody->setCollisionBitmask(0);
		//绑定了刚体的精灵锚点只能是0.5,0.5)
		CCLOGERROR("jumpBody0:%d,jumpBody1:%d",jumpBody->getCategoryBitmask(), jumpBody->getContactTestBitmask());

		return jumpBody;
		break;}
	case E_PHYSICS_BODYS::SLIDE:{
		//把spr弄成刚体绘制所依赖的原图
		auto fra=SpriteFrameCache::getInstance()->getSpriteFrameByName("slide_5.png");
		setSpriteFrame(fra);

		//定义滑行时应绑定的刚体
		MyBodyParser::getInstance()->parseJsonFile("shapes/slide_5.json");
		auto slideBody = MyBodyParser::getInstance()->bodyFormJson(this, "slide_5");
		CCASSERT(slideBody!=nullptr,"ActorSprite::createPhysicsBody.slideBody is null!");
		//设置物体不受重力系数影响  
		slideBody->setGravityEnable(false);
		//设置碰撞过滤(不碰撞，只做碰撞检测)
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
		sprite->init2();//initWithSpriteFrame()已经包含了对Sprite的初始化处理，这里为了代码一致性改用init2命名
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