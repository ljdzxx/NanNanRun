#pragma once

#include "cocos2d.h"

USING_NS_CC;

//�����״̬
enum class E_PHYSICS_BODYS:int{
	JUMP,
	SLIDE,
};

enum class E_ACTOR_STATUS:int{
	NULL_STATUS,//��״̬
	RUN,//��
	JUMP,//��
	INJURE,//����
	SLIDE,//����
	FALL_AHEAD,//ǰ��
	FALL_BACK,//����
};


class ActorSprite:public Sprite{
public:
	static ActorSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
	static ActorSprite* createWithSpriteFrameName(const std::string& spriteFrameName);

	bool init2();

	void updateStatus(E_ACTOR_STATUS, float=0.0f);
	//void setDisplayFrameWithAnimationName(const std::string& animationName, ssize_t frameIndex);
private:
	int _isUpdating;//״̬������
	E_ACTOR_STATUS _oldStatus;
	//ActionInterval* mCurrentAction;

	void update2Default();//����״̬������ϣ���ΪĬ��״̬
	void update2DefaultWithoutPhysics(float delayPerUnit=1.0f/48);
	PhysicsBody* createPhysicsBody(E_PHYSICS_BODYS e);
	void initRunAnim();//��
	void initJumpAnim();
	void initSlideAnim();
	void initFallAheadAnim();//ǰ��ˤ�Ӷ���
	void initFallBackAnim();//����ˤ�Ӷ���
	void initInjuredAnim();//���˶���
	void playAudioJumpDown();
private:
	ActorSprite();
	~ActorSprite(){}
	ActorSprite&operator=(ActorSprite&);
	ActorSprite(ActorSprite& a);
};
