#pragma once

#include "cocos2d.h"

USING_NS_CC;

enum{
	EFFECT_EAT_GOLD,
	EFFECT_WOOD_BOMB,
	EFFECT_STONE_BOMB,
	EFFECT_DIAMOND_BOMB,
	EFFECT_GLASS_BOMB,
	EFFECT_SMOKE,
	EFFECT_STAR,
	EFFECT_OIL,
};

class MyParticleCache/*:public Ref*/{
public:

	static MyParticleCache* getInstance();

	//播放粒子效果
	void play(Node* node, Vec2& position, int particleIdx);
private:
	static const char* _namePlist[16-3];
	cocos2d::Vector<ParticleSystemQuad*> _Particles;
	cocos2d::Vector<ParticleSystemQuad*> _Particle_gold1;//gold1效果需要短时间调用多次，这里专门开辟另外一个缓存
	cocos2d::Vector<ParticleSystemQuad*> _Particle_gold2;//gold2效果需要短时间调用多次，这里专门开辟另外一个缓存
	void playPackedParticles(Node* node, Vec2& position,int idx0,int idx1=-1,int idx2=-1,int idx3=-1);
	
	void init();
	ParticleSystemQuad* getCachedParticleById(int idx);

//禁止其他方式实例化-------------------------------------
private:
	MyParticleCache();
	~MyParticleCache();
	MyParticleCache(MyParticleCache& cache);
	MyParticleCache&operator=(MyParticleCache&);
};

