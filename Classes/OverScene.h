#ifndef __OVER_SCENE_H__
#define __OVER_SCENE_H__
//��Ϸ��������
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class OverScene : public BaseLayer{
public:
	OverScene();
	virtual bool init(int distance, int coins);
	static Scene* createScene(int distance, int coins);
	//CREATE_FUNC(OverScene);
	static OverScene *create(int distance, int coins);
private:
	enum class E_SPR_TAG:int{
		lb_MyDistance=100,
		lb_HisDistance,
		lb_MyCoins,
		lb_HisCoins,
		lb_HisDateTime,

		mi_PlayAgain,
		mi_Back,
	};
	int m_delayCount;//�÷�Ч����ʱ
	int m_MyDistance;//, m_HisDistance;	//���˾�������ʷ��߾���
	int m_MyCoins;//, m_HisCoins;	//���˽������ʷ��߽��

	Label* m_lbMyDistance, *m_lbHisDistance;
	Label* m_lbMyCoins, *m_lbHisCoins;
	Label* m_lbHisDateTime;

	float m_H1,m_H2;//����Label�ĺ�����
	float m_V1,m_V2;//����Label��������

	void initMenu();
	void menuCallback(Ref *sender);
	void update(float delta);
	void readAndSaveHisData();

	void createLable(const std::string& text, const std::string& fontFile, float fontSize,
				 int tag,
				 const Color3B &color,
				 const Vec2& anchorPoint,
				 const Vec2& position
				 );

	CC_SYNTHESIZE(int, _distance, Distance);
	CC_SYNTHESIZE(int, _coins, Coins);
};

#endif
