#include "MyParticleCache.h"
#include "cocos2d.h"

USING_NS_CC;

MyParticleCache* MyParticleCache::getInstance()
{
    static MyParticleCache* sg_ptr = nullptr;//静态局部变量初始化语句只会被执行一次
    if (nullptr == sg_ptr)
    {
        sg_ptr = new MyParticleCache();
		//sg_ptr->retain();
		//sg_ptr->autorelease();
		sg_ptr->init();
    }
    return sg_ptr;
}

const char* MyParticleCache::_namePlist[] = {
	"lz/woodbreak.plist"/*0*/, "lz/woodbreak2.plist"/*1*/,"lz/stonebreak.plist"/*2*/, "lz/stonebreak2.plist"/*3*/,
	//"diamond1.plist"/*4*/,
	//"diamond2.plist"/*5*/,"diamond3.plist"/*6*/,
	"lz/star1.plist"/*7*/, "lz/star2.plist"/*8*/,"lz/star3.plist"/*9*/, "lz/star4.plist"/*10*/,
	"lz/glassbreak.plist"/*11*/,
	"lz/smoke1.plist"/*12*/,
	"lz/gold1.plist"/*13*/, "lz/gold2.plist"/*14*/,
	"lz/bigfire.plist"/*15*/
};
//const char* MyParticleCache::_nameImage[] = {
//	"lz/woodpiece1.png", "lz/woodpiece2.png","lz/stone_1.png", "lz/stone_2.png",
//	 "lz/diamond.png",
//	"lz/star2.png", "lz/stars2.png","lz/star3a.png", "lz/gq.png",
//	"lz/glasspiece1.png", 
//	"lz/cloud.png",
//	"lz/gold.png", "lz/fire.png"
//};
/*enum{
		EFFECT_EAT_GOLD,
		EFFECT_WOOD_BOMB,
		EFFECT_STONE_BOMB,
		EFFECT_DIAMOND_BOMB,
		EFFECT_GLASS_BOMB,
		EFFECT_SMOKE,
		EFFECT_STAR,
	};*/

void MyParticleCache::playPackedParticles(Node* node, Vec2& position,int idx0,int idx1,int idx2,int idx3){
	ParticleSystemQuad* p;
	p=getCachedParticleById(idx0);
	p->setPosition(position);
	if(idx0!=sizeof(_namePlist)/sizeof(_namePlist[0])-1){//火焰粒子不重新设置粒子数量
		p->setEmissionRate(50);
	}else{
		p->setPositionType(kCCPositionTypeRelative);//所有粒子随坐标系进行相对位移
	}
	p->resetSystem();
	p->setAutoRemoveOnFinish(true);
	node->addChild(p);

	if(idx1>=0){
		p=getCachedParticleById(idx1);
		p->setPosition(position);
		p->setEmissionRate(50);
		p->resetSystem();
		p->setAutoRemoveOnFinish(true);
		node->addChild(p);
	}

	if(idx2>=0){
		p=getCachedParticleById(idx2);
		p->setPosition(position);
		p->setEmissionRate(50);
		p->resetSystem();
		p->setAutoRemoveOnFinish(true);
		node->addChild(p);
	}

	if(idx3>=0){
		p=getCachedParticleById(idx3);
		p->setPosition(position);
		p->setEmissionRate(50);
		p->resetSystem();
		p->setAutoRemoveOnFinish(true);
		node->addChild(p);
	}
}

void MyParticleCache::play(Node* node, Vec2& position, int particleIdx){
	switch (particleIdx)
	{
	case EFFECT_EAT_GOLD:{
		playPackedParticles(node,position,13-3,14-3);

		break;
	}
	case EFFECT_WOOD_BOMB:{
		playPackedParticles(node,position,0,1);
		break;
	}
	case EFFECT_STONE_BOMB:{
		playPackedParticles(node,position,2,3);
		break;
	}
	case EFFECT_DIAMOND_BOMB:{
		//playPackedParticles(node,position,/*4,*/5-1,6-1);
		break;
	}
	case EFFECT_GLASS_BOMB:{
		playPackedParticles(node,position,11-3);
		break;
	}
	case EFFECT_SMOKE:{
		playPackedParticles(node,position,12-3);
		break;
	}
	case EFFECT_STAR:{
		playPackedParticles(node,position,7-3,8-3,9-3,10-3);
		break;
	}
	case EFFECT_OIL:{
		playPackedParticles(node,position,15-3);
		break;
	}
	default:
		break;
	}
}

ParticleSystemQuad* MyParticleCache::getCachedParticleById(int idx){
	auto ptc=_Particles.at(idx);
	if(ptc!=nullptr && ptc->getParent()==nullptr){
		CCLOGERROR("%s is from cache!",_namePlist[idx]);
		return ptc;
	}
	else{
		if(idx==13-3){//gold1
			ParticleSystemQuad* idle=nullptr;
			for(ParticleSystemQuad* item: _Particle_gold1){
				if(item!=nullptr && item->getParent()==nullptr){
					idle=item;
					break;
				}
			}
			if(idle!=nullptr){
				CCLOGERROR("gold1 is from cache!");
				return idle;
			}else{
				CCLOGERROR("gold1 is from new()!");
				return ParticleSystemQuad::create(_namePlist[idx]);
			}
		}else if(idx==14-3){//gold2
			ParticleSystemQuad* idle=nullptr;
			for(ParticleSystemQuad* item: _Particle_gold2){
				if(item!=nullptr && item->getParent()==nullptr){
					idle=item;
					break;
				}
			}
			if(idle!=nullptr){
				CCLOGERROR("gold2 is from cache!");
				return idle;
			}else{
				CCLOGERROR("gold2 is from new()!");
				return ParticleSystemQuad::create(_namePlist[idx]);
			}
		}else{	
			CCLOGERROR("%s is from new()!",_namePlist[idx]);
			return ParticleSystemQuad::create(_namePlist[idx]);
		}
	}
}

void MyParticleCache::init(){
	
	int ptcCount=sizeof(_namePlist)/sizeof(_namePlist[0]);
	for(int i=0;i<ptcCount;++i){
		auto ptc=ParticleSystemQuad::create(_namePlist[i]);
		//ptc->retain();	//vector负责维护引用计数
		ptc->stopSystem();
		_Particles.pushBack(ptc);
	}	

	for(int i=0;i<10;i++){
		auto ptc=ParticleSystemQuad::create("lz/gold1.plist");
		ptc->stopSystem();
		_Particle_gold1.pushBack(ptc);
	}

	for(int i=0;i<10;i++){
		auto ptc=ParticleSystemQuad::create("lz/gold2.plist");
		ptc->stopSystem();
		_Particle_gold2.pushBack(ptc);
	}
}

MyParticleCache::MyParticleCache():_Particles(30),_Particle_gold1(10),_Particle_gold2(10){
	
}

MyParticleCache::~MyParticleCache(){
	/*int ptcCount=sizeof(_Particles)/sizeof(_Particles[0]);
	for(int i=0;i<ptcCount;++i){
		_Particles[i]->release();
	}*/
}
