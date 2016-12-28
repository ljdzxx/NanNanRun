#include "CoinsNode.h"
#include <math.h>
//#include "MyBodyParser.h"
#include "GlobalVar.h"

USING_NS_CC;

//Ǯ�Ҽ���
//#define COIN_FRAME_SPAN 20.0f
//#define COINS_MAX_Y 60.0f
//X��������ϵ�����˲�������Ǯ��Y���
//#define X_INCREMENT_RATIO 0.5f

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize


CoinsNode::CoinsNode():Node(),rtCoins(5,10){
	//SpriteFrameCache* sprFraCache=SpriteFrameCache::getInstance();
	//sprFraCache->addSpriteFramesWithFile("images/coinAnmi.plist");//Ǯ�Ҷ���
}


CoinsNode::~CoinsNode(){
	//SpriteFrameCache* sprFraCache=SpriteFrameCache::getInstance();
	//sprFraCache->removeSpriteFramesFromFile("images/coinAnmi.plist");

	//moveSeq->release();
}

//CoinsNode * CoinsNode::create(float x, int coinsCount)
//{
//	CoinsNode * ret = new CoinsNode();
//	if (ret)
//	{
//		ret->setX(x);
//		ret->setCoinsCount(coinsCount);
//		//ret->setCoinSpeed(coinSpeed);
//
//		if(ret->init()){
//			ret->autorelease();
//		}
//		else
//		{
//			CC_SAFE_DELETE(ret);
//		}
//	}
//	return ret;
//}

void CoinsNode::update(float delta){
	if(rtCoins.isTimeOut()){
		createAndMove();
	}
}

//void CoinsNode::move(){
//	//��������Ǯ�ҿɼ�����������ײ���
//	for(int i=0;i<m_coinsCount;i++){
//		auto coin=getChildByTag(i);
//		if(coin!=nullptr){
//			coin->setVisible(true);
//			coin->getPhysicsBody()->setContactTestBitmask(2);//0x10
//		}
//	}
//
//	//�����˶��켣
//	float moveLength=winSize.width + this->getBoundingBox().size.width;//(28*1.5+COIN_FRAME_OFFSET)*COINS_COUNT;//������ʱ�趨Ǯ��ͼ����Ϊ28����COINS_COUNT��Ǯ��
//	float coinsRunTime=moveLength/(GlobalVar::_coinSpeed);
//	CCLOGERROR("CoinsNode:: moveLength=%.2f,coinsRunTime=%.2f",moveLength,coinsRunTime);
//	auto place=Place::create(Vec2(winSize.width,0));
//	auto mov=MoveBy::create(coinsRunTime,Point(-moveLength, 0));
//	auto moveSeq=Sequence::create(place, 
//		mov ,
//		//CallFuncN::create(CC_CALLBACK_1(GameScene::onCoinsLayerMoveDone, this)),
//		nullptr);
//	//moveSeq->retain();
//	this->runAction(moveSeq);
//}

//����ֱ�߹켣��Ǯ�����꼯
//������ѧ��ʽy=kx+b��ֱ��������һ�������
void CoinsNode::getLineTrack(std::vector<XY>& v){
	//���������(x1,y1)��(x2,y2)����
	//int rangeX=winSize.width-10;
	int x1=0;

	int minY=95+150;
	int maxY=winSize.height-100;
	int rangeY=maxY-minY;
	srand(GetTime());
	int y1=minY+rand()%rangeY;

	int rangeX=winSize.width-100;
	int x2=100+rand()%rangeX;
	srand(GetTime()+y1);
	int y2=minY+rand()%rangeY;

	//���ù�ʽ���k=(y2-y1)/(x2-x1)
	float k=(float)(y2-y1)/(x2-x1);
	float b=y1-k*x1;

	//����n�������
	int xCount=15;//����һ����������15����
	int coinsCount=(x2-x1)*xCount/winSize.width;
	float space_between=(x2-x1)/coinsCount;//���
	for(int i=0;i<coinsCount;++i){
		float xi=space_between*i;
		float yi=k*xi+b;
		XY xy={xi,yi};
		v.push_back(xy);//������xy��COPY֮��ĸ���
	}

}

//�����������߹켣��Ǯ�����꼯
void CoinsNode::getSinTrack(std::vector<XY>& v){
	int x1=0;
	int rangeX=winSize.width-100;
	srand(GetTime()+rand());
	int x2=100+rand()%rangeX;

	const float RADIAN_RATIO=3.14f/180;//�Ƕ�ת���ȵ�ϵ��
	const float ANGLE_INCREMENT=30.0f;//�Զ��ٶȽ�Ϊ����
	const float COINS_MAX_Y=60.0f;//Y�Შ����Χ (+ -)

	for(int i=0;i<100;++i){
		float xi=ANGLE_INCREMENT*i;//�ԽǶ�����ֱ����ΪX������
		if(xi>x2)break;

		float sinX=RADIAN_RATIO*xi;//ת����
		float yi=95+144+72+COINS_MAX_Y*sin(sinX);
		XY xy={xi,yi};
		v.push_back(xy);//������xy��COPY֮��ĸ���
	}
}

bool CoinsNode::init(){
	if(!Node::init()){
		return false;
	}
	initCoinAnim();
	setContentSize(winSize);
	scheduleUpdate();//����update()
	return true;
}

//����Ǯ��֡����
void CoinsNode::initCoinAnim()
{
	auto animation=Animation::create();
	animation->setDelayPerUnit(1.0/12);
	for(int j=0;j<12;++j){
		auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("coin_%d.png",j+1));
		animation->addSpriteFrame(frame);
	}

	AnimationCache::getInstance()->addAnimation(animation, "coinsAnim");
}

void CoinsNode::createAndMove(){
	SpriteFrameCache* sprFraCache=SpriteFrameCache::getInstance();

	//�����ƶ���
	auto movableNode=Node::create();
	movableNode->setContentSize(winSize);
	movableNode->setAnchorPoint(Vec2::ZERO);
	movableNode->setPosition(Vec2(winSize.width,0));
	movableNode->setTag(911);
	addChild(movableNode);

	//���ɴ�ѵ�Ǯ�Ҿ���
	std::vector<XY> coinsXY;
	srand(GetTime()+rand());
	if(rand()%2==0){//���������·
		getLineTrack(coinsXY);
	}else{
		getSinTrack(coinsXY);
	}
	

	for(std::vector<XY>::iterator it=coinsXY.begin();it!=coinsXY.end();++it){
		//��һ֡
		auto spr=Sprite::createWithSpriteFrameName("coin_1.png");//lz/gold.png
		//spr->setTag(i);
		spr->setName(MACRO_ENUM2STR(E_NODE_NAMES::COIN));//��ʶΪǮ��

		//�󶨸���
		//PhysicsBody* body= PhysicsBody::createCircle(spr->getBoundingBox().size.width/2);
		auto body=PhysicsBody::createBox(spr->getBoundingBox().size);//�������ο��Լ���OPGL ES�����������Ч��
		//�������岻������ϵ��Ӱ��  
		body->setGravityEnable(false);
		//������ײ����(����ײ��ֻ����ײ���)
		body->setCategoryBitmask(1);
		body->setContactTestBitmask(2);
		body->setCollisionBitmask(0);
		spr->setPhysicsBody(body);

		spr->setScale(0.72);//��С
		//spr->setAnchorPoint(Vec2(0,0.5));
		spr->setPosition(Vec2((*it).x,(*it).y));
		movableNode->addChild(spr);

		//�����֡����
		auto animation=AnimationCache::getInstance()->getAnimation("coinsAnim");
		auto animate=Animate::create(animation);
		auto repeatAct=RepeatForever::create(animate);

		spr->runAction(repeatAct);
	}
	
	//�����˶��켣
	float moveLength=winSize.width*2 ;//(28*1.5+COIN_FRAME_OFFSET)*COINS_COUNT;//������ʱ�趨Ǯ��ͼ����Ϊ28����COINS_COUNT��Ǯ��
	float coinsRunTime=moveLength/(GlobalVar::_coinSpeed);
	
	//auto place=Place::create(Vec2(winSize.width,0));
	auto mov=MoveBy::create(coinsRunTime,Point(-moveLength, 0));
	auto moveSeq=Sequence::create(//place, 
		mov ,
		CallFuncN::create(CC_CALLBACK_1(CoinsNode::onMoveDone, this)),
		nullptr);
	
	movableNode->runAction(moveSeq);
}

void CoinsNode::onMoveDone(Node* sender){
	//removeChild(sender);
	CCASSERT(sender==this->getChildByTag(911),"");
	sender->removeFromParentAndCleanup(true);
}