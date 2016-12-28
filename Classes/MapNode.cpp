#include "MapNode.h"
#include "GlobalVar.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

USING_NS_CC;

bool MapNode::init(){
	if(!Node::init()){
		return false;
	}

	auto mapSpr=Sprite::create("images/bg1.png", Rect(0,0,800,480));
	mapSpr->setPosition(Vec2::ZERO);
	mapSpr->setAnchorPoint(Vec2(0,0));
	mapSpr->setTag((int)E_NODE_TAGS::BACKGROUND);
	//http://www.himigame.com/iphone-cocos2d/507.html?spm=5176.100239.blogcont44931.10.NDcehx
	//http://blog.csdn.net/zhy_cheng/article/details/8481366
	mapSpr->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	this->addChild(mapSpr);


	auto spr_1=Sprite::create("images/bg1.png", Rect(0,480+1,800,480));
	spr_1->setAnchorPoint(Point::ZERO);
	spr_1->setPosition(Point(800,0));
	spr_1->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	mapSpr->addChild(spr_1);//,0,Point(1,0),Point(800,0));

	auto spr_2=Sprite::create("images/bg2.png", Rect(0,0,800,480));
	spr_2->setAnchorPoint(Point::ZERO);
	spr_2->setPosition(Point(1600,0));
	spr_2->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	mapSpr->addChild(spr_2);//,0,Point(1,0),Point(1600,0));

	auto spr_3=Sprite::create("images/bg2.png", Rect(0,480+1,800,480));
	spr_3->setAnchorPoint(Point::ZERO);
	spr_3->setPosition(Point(2400,0));
	spr_3->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	mapSpr->addChild(spr_3);//,0,Point(1,0),Point(2400,0));

	//��һ��ͼ��������
	auto spr_4=Sprite::create("images/bg1.png", Rect(0,0,800,480));
	spr_4->setAnchorPoint(Point::ZERO);
	spr_4->setPosition(Point(3200,0));
	spr_4->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	mapSpr->addChild(spr_4);//,0,Point(1,0),Point(3200,0));

	//�ٽ�һ��ͼ����Ϊ��Щ�ֻ���Ļ�ϳ�
	auto spr_5=Sprite::create("images/bg1.png", Rect(0,480+1,800,480));
	spr_5->setAnchorPoint(Point::ZERO);
	spr_5->setPosition(Point(4000,0));
	spr_5->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	mapSpr->addChild(spr_5);

	/*auto move_1=MoveTo::create(3200/bgSpeed,Point(-3200,0));
	auto place_1=Place::create(Point::ZERO);
	auto seq_1=Sequence::create(move_1,place_1,NULL);
	auto rf_1=RepeatForever::create(seq_1);
	node->runAction(rf_1);*/
	


	//·�����Ч����ͼƬ���� - w:512,h:128,y-offset:6,x-offset:12
	auto roadSpr=Sprite::create("images/grounds.png", Rect(0,11,500,111));
	roadSpr->setPosition(Vec2::ZERO);
	roadSpr->setAnchorPoint(Vec2::ZERO);
	roadSpr->setTag((int)E_NODE_TAGS::ROAD);
	roadSpr->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
	this->addChild(roadSpr);

	Sprite * spr_road;
	for(int i=1;i<4;++i){
		spr_road=Sprite::create("images/grounds.png", Rect(0,11,500,111));
		spr_road->setAnchorPoint(Point::ZERO);
		spr_road->setPosition(Vec2(500*i,0));
		spr_road->getTexture()->setAliasTexParameters();//���ò�����ݣ����1���ط�϶����
		roadSpr->addChild(spr_road);//,1,Point(2,0),Point(500*i,0));
	}
	//ƴ������100���Ժϳ�2������(1600)
	/*spr_road=Sprite::create("images/grounds.png", Rect(0,11,100,111));
	spr_road->setAnchorPoint(Point::ZERO);
	spr_road->setPosition(Vec2(1500,0));
	roadSpr->addChild(spr_road);*/

	scheduleUpdate();
	return true;
}

void MapNode::update(float delta){
		//�ƶ�����
	auto bg=getChildByTag((int)E_NODE_TAGS::BACKGROUND);
	if(bg==nullptr)return;
	float x=bg->getPositionX();
	float distance=GlobalVar::_bgSpeed*delta;//Ӧ���ƶ��ľ���
	float x2=x-distance;
	if(x2 < -winSize.width*4){//Խ��
		x2+=(winSize.width*4);	
	}
	bg->setPositionX(x2);

	//�ƶ�·��
	auto road=getChildByTag((int)E_NODE_TAGS::ROAD);
	if(road==nullptr)return;
	x=road->getPositionX();
	distance=GlobalVar::_roadSpeed*delta;//Ӧ���ƶ��ľ���

	/*if(GlobalVar::_scoreNode!=nullptr){
		GlobalVar::_scoreNode->updateDistance(distance);
	}*/
	x2=x-distance;
	if(x2 < -winSize.width){//Խ��
		x2+=winSize.width;	
	}
	road->setPositionX(x2);
}