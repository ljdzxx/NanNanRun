#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
//#include "RandomTime.h"
#include "ActorSprite.h"
#include "MyParticleCache.h"
#include "GlobalVar.h"
#include "BaseLayer.h"

USING_NS_CC;


class GameScene : public BaseLayer{
public:
	//static const int COINS_COUNT;//钱币个数
	//static const float bgCycleTime; //背景完成一个循环的时间
	//static const float bgSpeedRate;//背景速度倍率(慢速相对于正常速度的倍率)

	//static const float COIN_FRAME_OFFSET;	//钱币动画每帧之间的间距

	//static const std::string SPR_NAME_TRAP;

	
	//static float m_SpeedScale;
	static const float fadeDuration;
	
	//static const float durationPerFrame;//动画帧/秒

	bool init() override;
	
	//static Size winSize;
	static Scene* createScene();
	//显示关卡的Label
	static Label* getLevelLabel(float fontSize=80.0f);

	CREATE_FUNC(GameScene);
	
	/*static GameScene * create(const Color4B& color){
		GameScene * layer = new GameScene();
		if(layer && layer->initWithColor(color))
		{
			layer->autorelease();
			return layer;
		}
		CC_SAFE_DELETE(layer);
		return nullptr;
	}*/

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	static void nextLevel(){++GlobalVar::_level;/*m_SpeedScale+=0.5;*/}

//protected:
//	
//	~GameScene();
//	
//
//	void onEnter() override;
//	void onExit() override;
private:
	GameScene();
	~GameScene();

	//bool initWithColor(const Color4B& color);
	//bool initWithColor(const Color4B& color, GLfloat width, GLfloat height);
	

	//enum class E_ACTIONS:int
	//{
	//	ACTION_RUN=10,//跑步
	//	ACTION_JUMP,//起跳
	//	ACTION_INJURED,//受伤
	//	ACTION_SLIDE,//滑行
	//	ACTION_FALL,//前扑
	//	ACTION_COUCH,//后仰
	//};

	enum class E_ZORDERS:int{
		MAP=1,//地图层
		TRAP=2,//陷阱层

		COIN=4,//金币层

		ACTOR=10,//角色层
		
		SCORE=13,//状态显示层
	};

	enum class E_NODE_TAGS:int{
		ROOT_LAYER=100,
		MASK_LAYER,
		ACTOR,
		MAP,
		//LABEL_LEVEL,
		//SCORE,
	};

	

	//触摸点的坐标、当时人物坐标
	Point posOfTouch, posOfPersonOnTouch;
	//bool jumping;

	void setLeaving(bool flag);

	//PhysicsBody* jumpBody;//起跳时要绑定的刚体
	//PhysicsBody* slideBody;//滑行时要绑定的刚体

	//4张宽800像素的背景图滑过屏幕总共的时长


	//陷井
	//Vector<Sprite*> v_traps;
	//钱币
	//Vector<Sprite*> v_coins;

	//CCRenderTexture* _rt;

	void initMap();//初始化背景
	void initScore();
	//void initCoin();
	void initTrap();

	void initPersonSpr();
	

	//跳跃动作结束后调用
	void onJumpEnded();
	//下滑动作结束后调用
	void onSlideEnded();

	//void trapAndCoinsSchedule(float delta);
	void shiftLevel(float delta);

	void trapCollideEvent(Node* sender);
	//void randCoinSchedule(float delta);
	//void addCoinsLayer(/*float delta*/);


	void initCoinAnim();

	//添加陷阱
	//void addTrap(/*float delta*/);
	
	//恢复播放背景音乐
	void resumeBackgroundMusic(Ref* sender);

	void update(float delta) override;

	//void initCoinLayer();

	//陷阱随机时间
	//static RandomTime trapRT;

	//钱币随机时间
	//static RandomTime coinsRT;


	//void moveBackgroundSlowly(bool isSlow);
	//void roleWillRunAnim(Ref* sender, RoleSprite* role, FiniteTimeAction* act);
	//让角色跑起来
	//void runRunAction(Ref* sender);
	//恢复背景移动速度
	//void resumeBackgroundMoveSpeed(Ref* sender, MoveTo* act);

	PhysicsWorld* m_world;  
	void setPhyWorld(PhysicsWorld* world) {m_world=world;};
	//碰到陷阱
	bool onContactTrapBegin(const PhysicsContact& contact);

	//ActionInterval* getAnimate(E_ACTIONS act);

	//根据TAG生成不同的粒子效果
	//ParticleSystemQuad* getParticleByTag(int tag);
	int shiftTrapId2ParticleId(int trapId);

	//设置速度系数（控制背景移动速度）
	//CC_SYNTHESIZE(float,m_SpeedScale, SpeedScale);

	void init2();
	void shiftLevel2();

	void gameOver();
};

#endif