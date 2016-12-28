#include "CoinsNode.h"
#include <math.h>
//#include "MyBodyParser.h"
#include "GlobalVar.h"

USING_NS_CC;

//钱币间间隔
//#define COIN_FRAME_SPAN 20.0f
//#define COINS_MAX_Y 60.0f
//X坐标增量系数，此参数调节钱币Y间距
//#define X_INCREMENT_RATIO 0.5f

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize


CoinsNode::CoinsNode():Node(),rtCoins(5,10){
	//SpriteFrameCache* sprFraCache=SpriteFrameCache::getInstance();
	//sprFraCache->addSpriteFramesWithFile("images/coinAnmi.plist");//钱币动画
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
//	//设置所有钱币可见，并允许碰撞检测
//	for(int i=0;i<m_coinsCount;i++){
//		auto coin=getChildByTag(i);
//		if(coin!=nullptr){
//			coin->setVisible(true);
//			coin->getPhysicsBody()->setContactTestBitmask(2);//0x10
//		}
//	}
//
//	//定义运动轨迹
//	float moveLength=winSize.width + this->getBoundingBox().size.width;//(28*1.5+COIN_FRAME_OFFSET)*COINS_COUNT;//这里暂时设定钱币图像宽度为28，共COINS_COUNT个钱币
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

//生成直线轨迹的钱币坐标集
//根据数学公式y=kx+b求直线上任意一点的坐标
void CoinsNode::getLineTrack(std::vector<XY>& v){
	//定义随机的(x1,y1)和(x2,y2)两点
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

	//利用公式求解k=(y2-y1)/(x2-x1)
	float k=(float)(y2-y1)/(x2-x1);
	float b=y1-k*x1;

	//生成n个坐标点
	int xCount=15;//假设一个横屏布满15个点
	int coinsCount=(x2-x1)*xCount/winSize.width;
	float space_between=(x2-x1)/coinsCount;//间距
	for(int i=0;i<coinsCount;++i){
		float xi=space_between*i;
		float yi=k*xi+b;
		XY xy={xi,yi};
		v.push_back(xy);//这里存的xy是COPY之后的副本
	}

}

//生成正弦曲线轨迹的钱币坐标集
void CoinsNode::getSinTrack(std::vector<XY>& v){
	int x1=0;
	int rangeX=winSize.width-100;
	srand(GetTime()+rand());
	int x2=100+rand()%rangeX;

	const float RADIAN_RATIO=3.14f/180;//角度转弧度的系数
	const float ANGLE_INCREMENT=30.0f;//以多少度角为增量
	const float COINS_MAX_Y=60.0f;//Y轴波动范围 (+ -)

	for(int i=0;i<100;++i){
		float xi=ANGLE_INCREMENT*i;//以角度增量直接作为X轴增量
		if(xi>x2)break;

		float sinX=RADIAN_RATIO*xi;//转弧度
		float yi=95+144+72+COINS_MAX_Y*sin(sinX);
		XY xy={xi,yi};
		v.push_back(xy);//这里存的xy是COPY之后的副本
	}
}

bool CoinsNode::init(){
	if(!Node::init()){
		return false;
	}
	initCoinAnim();
	setContentSize(winSize);
	scheduleUpdate();//激活update()
	return true;
}

//缓存钱币帧动画
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

	//生成移动层
	auto movableNode=Node::create();
	movableNode->setContentSize(winSize);
	movableNode->setAnchorPoint(Vec2::ZERO);
	movableNode->setPosition(Vec2(winSize.width,0));
	movableNode->setTag(911);
	addChild(movableNode);

	//生成大把的钱币精灵
	std::vector<XY> coinsXY;
	srand(GetTime()+rand());
	if(rand()%2==0){//随机生成线路
		getLineTrack(coinsXY);
	}else{
		getSinTrack(coinsXY);
	}
	

	for(std::vector<XY>::iterator it=coinsXY.begin();it!=coinsXY.end();++it){
		//第一帧
		auto spr=Sprite::createWithSpriteFrameName("coin_1.png");//lz/gold.png
		//spr->setTag(i);
		spr->setName(MACRO_ENUM2STR(E_NODE_NAMES::COIN));//标识为钱币

		//绑定刚体
		//PhysicsBody* body= PhysicsBody::createCircle(spr->getBoundingBox().size.width/2);
		auto body=PhysicsBody::createBox(spr->getBoundingBox().size);//绑定正方形可以减少OPGL ES顶点数，提高效率
		//设置物体不受重力系数影响  
		body->setGravityEnable(false);
		//设置碰撞过滤(不碰撞，只做碰撞检测)
		body->setCategoryBitmask(1);
		body->setContactTestBitmask(2);
		body->setCollisionBitmask(0);
		spr->setPhysicsBody(body);

		spr->setScale(0.72);//缩小
		//spr->setAnchorPoint(Vec2(0,0.5));
		spr->setPosition(Vec2((*it).x,(*it).y));
		movableNode->addChild(spr);

		//后面的帧动画
		auto animation=AnimationCache::getInstance()->getAnimation("coinsAnim");
		auto animate=Animate::create(animation);
		auto repeatAct=RepeatForever::create(animate);

		spr->runAction(repeatAct);
	}
	
	//定义运动轨迹
	float moveLength=winSize.width*2 ;//(28*1.5+COIN_FRAME_OFFSET)*COINS_COUNT;//这里暂时设定钱币图像宽度为28，共COINS_COUNT个钱币
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