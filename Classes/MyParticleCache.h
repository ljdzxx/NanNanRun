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

	//��������Ч��
	void play(Node* node, Vec2& position, int particleIdx);
private:
	static const char* _namePlist[16-3];
	cocos2d::Vector<ParticleSystemQuad*> _Particles;
	cocos2d::Vector<ParticleSystemQuad*> _Particle_gold1;//gold1Ч����Ҫ��ʱ����ö�Σ�����ר�ſ�������һ������
	cocos2d::Vector<ParticleSystemQuad*> _Particle_gold2;//gold2Ч����Ҫ��ʱ����ö�Σ�����ר�ſ�������һ������
	void playPackedParticles(Node* node, Vec2& position,int idx0,int idx1=-1,int idx2=-1,int idx3=-1);
	
	void init();
	ParticleSystemQuad* getCachedParticleById(int idx);

//��ֹ������ʽʵ����-------------------------------------
private:
	MyParticleCache();
	~MyParticleCache();
	MyParticleCache(MyParticleCache& cache);
	MyParticleCache&operator=(MyParticleCache&);
};

