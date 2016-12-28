//Ǯ�Ҳ�
#pragma once
#include "RandomTime.h"
#include "cocos2d.h"

USING_NS_CC;

struct XY{
	float x;
	float y;
};

class CoinsNode:public Node{
public:
	//Vector<Sprite*> v_coins;
	//x-X���꣬coinsCount-Ǯ�Ҹ���
	//static CoinsNode* create(float x, int coinsCount);
	CREATE_FUNC(CoinsNode);
	bool init() override;
private:
	//void move();
	//Ǯ�����ʱ��
	RandomTime rtCoins;
	//Sequence* moveSeq;
	void getLineTrack(std::vector<XY>& v);
	void getSinTrack(std::vector<XY>& v);
	//void getBezierTrack();
	void createAndMove();
	void initCoinAnim();
	void onMoveDone(Node* sender);
protected:
	CoinsNode();
	~CoinsNode();
	CoinsNode&operator=(CoinsNode&){}

	void update(float delta) override;

	//CC_SYNTHESIZE(float, m_x, X);
	//CC_SYNTHESIZE(int, m_coinsCount, CoinsCount);
	//CC_SYNTHESIZE(float, m_coinSpeed, CoinSpeed);

};

