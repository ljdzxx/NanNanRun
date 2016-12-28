#pragma once

#include "cocos2d.h"

USING_NS_CC;

//物理绑定状态
enum class E_PHYSICS_BODYS:int{
	JUMP,
	SLIDE,
};

enum class E_ACTOR_STATUS:int{
	NULL_STATUS,//空状态
	RUN,//跑
	JUMP,//跳
	INJURE,//受伤
	SLIDE,//滑行
	FALL_AHEAD,//前扑
	FALL_BACK,//后仰
};


class ActorSprite:public Sprite{
public:
	static ActorSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
	static ActorSprite* createWithSpriteFrameName(const std::string& spriteFrameName);

	bool init2();

	void updateStatus(E_ACTOR_STATUS, float=0.0f);
	//void setDisplayFrameWithAnimationName(const std::string& animationName, ssize_t frameIndex);
private:
	int _isUpdating;//状态正更新
	E_ACTOR_STATUS _oldStatus;
	//ActionInterval* mCurrentAction;

	void update2Default();//人物状态更新完毕，置为默认状态
	void update2DefaultWithoutPhysics(float delayPerUnit=1.0f/48);
	PhysicsBody* createPhysicsBody(E_PHYSICS_BODYS e);
	void initRunAnim();//跑
	void initJumpAnim();
	void initSlideAnim();
	void initFallAheadAnim();//前倾摔跤动画
	void initFallBackAnim();//后仰摔跤动画
	void initInjuredAnim();//受伤动画
	void playAudioJumpDown();
private:
	ActorSprite();
	~ActorSprite(){}
	ActorSprite&operator=(ActorSprite&);
	ActorSprite(ActorSprite& a);
};
