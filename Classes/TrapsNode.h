//�����
#pragma once

#include "cocos2d.h"
//#include "RandomTime.h"

USING_NS_CC;

class TrapsNode:public Node
{
public:
	 static TrapsNode* create();
	 
	 
	 //static const float _airCoinOffsetY;//���ڿ��������µĽ��������ľ���
	 //static const float _sinFromHat2Trap;//���Ҳ�Ģ��ͷ�ײ�������ľ���
	 //static const float _sinXscale;//���Ҳ�����������ȵı���
	 //static const float _sinYscale;//���Ҳ��߶�������߶ȵı���
protected:
	bool init() override;
	void update(float delta) override;
private:
	enum class E_NODE_TYPE:int{
		COIN,
		TRAP,
	};
	float SPACE_FROM_SIN_HAT_2_TRAP;//���Ҳ�Ģ��ͷ�ײ�������ľ���(ƫ��k)
	float TRAP_X_SPACE_BEFORE;//�������ҵľ���(ǰ)
	float TRAP_X_SPACE_AFTER;//�������ҵľ���(��)
	float TrapsNode_newAirTrap_oil_down_duration;

	Size DEFAULT_COIN_NODE_SIZE;//Ĭ�ϵĽ��NODE��С

	int _minCoinNodes;//��С���Node����
	int _maxRandCoinNodes;//���������Node����

	int _remainCoinNodes;//ʣ�´����ĵĽ��Node����
	void resetRemainCoinNodes();

	cocos2d::Vector<Node*> _Nodes;

	TrapsNode();
	Sprite* getAirTrapSpriteById(int sprIdx,int for_i, float baseY);
	TrapsNode&operator=(TrapsNode&){}
	//TrapsNode(TrapsNode&){}
	void newAirTrap();
	void newTrap();
	//ɾ������
	//void delTrap(Node* sender);
	//�������ʱ��
	//RandomTime rtTrap;

	Sprite* createCoin();
	Node* packTrapWithCoin(Sprite* spr);
	Node* packAirTrapWithCoin(Sprite* spr);
	Node* newCoinNode(Size& size,float sinXscale=0.0f,bool isIgnoreFirstCoin=false, float spaceX=0);//����һ��װ�ؽ�ҵ�Node�ڵ�
	void addCoinNode(Node* coinNode);
	//CC_SYNTHESIZE(float, m_roadSpeed, RoadSpeed);
};
