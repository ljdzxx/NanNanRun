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
	//static const int COINS_COUNT;//Ǯ�Ҹ���
	//static const float bgCycleTime; //�������һ��ѭ����ʱ��
	//static const float bgSpeedRate;//�����ٶȱ���(��������������ٶȵı���)

	//static const float COIN_FRAME_OFFSET;	//Ǯ�Ҷ���ÿ֮֡��ļ��

	//static const std::string SPR_NAME_TRAP;

	
	//static float m_SpeedScale;
	static const float fadeDuration;
	
	//static const float durationPerFrame;//����֡/��

	bool init() override;
	
	//static Size winSize;
	static Scene* createScene();
	//��ʾ�ؿ���Label
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
	//	ACTION_RUN=10,//�ܲ�
	//	ACTION_JUMP,//����
	//	ACTION_INJURED,//����
	//	ACTION_SLIDE,//����
	//	ACTION_FALL,//ǰ��
	//	ACTION_COUCH,//����
	//};

	enum class E_ZORDERS:int{
		MAP=1,//��ͼ��
		TRAP=2,//�����

		COIN=4,//��Ҳ�

		ACTOR=10,//��ɫ��
		
		SCORE=13,//״̬��ʾ��
	};

	enum class E_NODE_TAGS:int{
		ROOT_LAYER=100,
		MASK_LAYER,
		ACTOR,
		MAP,
		//LABEL_LEVEL,
		//SCORE,
	};

	

	//����������ꡢ��ʱ��������
	Point posOfTouch, posOfPersonOnTouch;
	//bool jumping;

	void setLeaving(bool flag);

	//PhysicsBody* jumpBody;//����ʱҪ�󶨵ĸ���
	//PhysicsBody* slideBody;//����ʱҪ�󶨵ĸ���

	//4�ſ�800���صı���ͼ������Ļ�ܹ���ʱ��


	//�ݾ�
	//Vector<Sprite*> v_traps;
	//Ǯ��
	//Vector<Sprite*> v_coins;

	//CCRenderTexture* _rt;

	void initMap();//��ʼ������
	void initScore();
	//void initCoin();
	void initTrap();

	void initPersonSpr();
	

	//��Ծ�������������
	void onJumpEnded();
	//�»��������������
	void onSlideEnded();

	//void trapAndCoinsSchedule(float delta);
	void shiftLevel(float delta);

	void trapCollideEvent(Node* sender);
	//void randCoinSchedule(float delta);
	//void addCoinsLayer(/*float delta*/);


	void initCoinAnim();

	//�������
	//void addTrap(/*float delta*/);
	
	//�ָ����ű�������
	void resumeBackgroundMusic(Ref* sender);

	void update(float delta) override;

	//void initCoinLayer();

	//�������ʱ��
	//static RandomTime trapRT;

	//Ǯ�����ʱ��
	//static RandomTime coinsRT;


	//void moveBackgroundSlowly(bool isSlow);
	//void roleWillRunAnim(Ref* sender, RoleSprite* role, FiniteTimeAction* act);
	//�ý�ɫ������
	//void runRunAction(Ref* sender);
	//�ָ������ƶ��ٶ�
	//void resumeBackgroundMoveSpeed(Ref* sender, MoveTo* act);

	PhysicsWorld* m_world;  
	void setPhyWorld(PhysicsWorld* world) {m_world=world;};
	//��������
	bool onContactTrapBegin(const PhysicsContact& contact);

	//ActionInterval* getAnimate(E_ACTIONS act);

	//����TAG���ɲ�ͬ������Ч��
	//ParticleSystemQuad* getParticleByTag(int tag);
	int shiftTrapId2ParticleId(int trapId);

	//�����ٶ�ϵ�������Ʊ����ƶ��ٶȣ�
	//CC_SYNTHESIZE(float,m_SpeedScale, SpeedScale);

	void init2();
	void shiftLevel2();

	void gameOver();
};

#endif