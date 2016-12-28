#include "TrapsNode.h"
#include "MyBodyParser.h"
#include "GlobalVar.h"
#include "DictionaryLoader.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

USING_NS_CC;


TrapsNode* TrapsNode::create(){
    TrapsNode *ret = new TrapsNode();
    if (ret && ret->init())
    {
		//ret->setRoadSpeed(roadSpeed);
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool TrapsNode::init(){
	if(!Node::init()){
		return false;
	}

	setContentSize(winSize);

	scheduleUpdate();
	return true;
}

TrapsNode::TrapsNode():rtTrap(1.0f,2){

}

Sprite* TrapsNode::getAirTrapSpriteById(int sprIdx,int for_i, float baseY){
	auto sprTrap=Sprite::createWithSpriteFrameName(StringUtils::format("air_trap_%d.png",sprIdx));//随机取一个陷阱
	//绑定刚体
	MyBodyParser::getInstance()->parseJsonFile(StringUtils::format("shapes/air_trap_%d.json",sprIdx));
	auto airTrapBody = MyBodyParser::getInstance()->bodyFormJson(sprTrap, StringUtils::format("air_trap_%d",sprIdx));
	CCASSERT(airTrapBody!=nullptr,"GameScene::addTrap.airTrapBody is null!");
	//设置物体不受重力系数影响  
	airTrapBody->setGravityEnable(false);
	//设置碰撞过滤(不碰撞，只做碰撞检测)
	airTrapBody->setCategoryBitmask(1);//01b
	airTrapBody->setContactTestBitmask(2);//10b
	airTrapBody->setCollisionBitmask(0);
	//把刚体与精灵绑定  
	sprTrap->setPhysicsBody(airTrapBody); 
	CCLOGERROR("airTrapBody0:%d,airTrapBody1:%d",airTrapBody->getCategoryBitmask(), airTrapBody->getContactTestBitmask());

	//sprTrap->setAnchorPoint(Point::ZERO);
	sprTrap->setPosition(sprTrap->getBoundingBox().size.width*(2*for_i+1)/2,baseY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP));//标识为空中陷阱
	sprTrap->setUserData((void *)sprIdx);//作为扣血量设置进去
	
	return sprTrap;
}

//添加一个空中陷阱
void TrapsNode::newAirTrap(){
	//123关1排，456关2排，其他以上关卡3排
	int num=GlobalVar::_level/3;
	if(GlobalVar::_level%3 > 0)++num;
	if(num>3)num=3;

	int sprIdx;//陷阱序号
	if(GlobalVar::_level==1){
		sprIdx=1;
	}
	else if(GlobalVar::_level==2){
		srand(GetTime()+rand());
		sprIdx=rand()%2+1;
	}
	else{
		srand(GetTime()+rand());
		sprIdx=rand()%3+1;
	}

	float roadY=128+2;//路面高度
	float actorRunY=95+144;//跑步时人头顶离地高度
	float actorSlideY=190;//下滑时人头顶离地高度
	Node* groupNode=Node::create();
	groupNode->setContentSize(Size(1,1));
	groupNode->setAnchorPoint(Vec2::ZERO);
	groupNode->setPosition(Vec2(winSize.width,0));
	addChild(groupNode);

	//num=3;//test
	for(int i=0;i<num;++i){
		//sprIdx=2;//test
		if(sprIdx==1){//1号，机械臂
			groupNode->addChild(getAirTrapSpriteById(sprIdx,i,actorSlideY));
		}
		else if(sprIdx==2){//2号，油桶
			auto spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			groupNode->addChild(spr);

			std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//运动周期
			float dura=atof(dictStr.c_str());
			spr->runAction(RepeatForever::create(JumpBy::create(dura,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
		else{//3号，滑轮
			auto spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			groupNode->addChild(spr);

			std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//运动周期
			float dura=atof(dictStr.c_str());
			spr->runAction(RepeatForever::create(JumpBy::create(dura,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
	}

	//Action定义

	//从右到左
	float trapRunTime=winSize.width*2/GlobalVar::_roadSpeed;
	auto mov=MoveBy::create(trapRunTime,Point(-(winSize.width*2), 0));
	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	groupNode->runAction(sequAction);

}

//添加一个地面陷阱
void TrapsNode::newTrap(){
	const int trapY=80;//起点Y坐标
	srand(GetTime()+rand());
	int rnd=rand()%8+1;
	//rnd=4;	//测试
	CCLOGERROR("the NO. of trap is:%d",rnd);
	auto sprTrap=Sprite::createWithSpriteFrameName(StringUtils::format("trap_%d.png",rnd));//随机取一个陷阱
	//绑定刚体
	MyBodyParser::getInstance()->parseJsonFile(StringUtils::format("shapes/trap_%d.json",rnd));
	auto trapBody = MyBodyParser::getInstance()->bodyFormJson(sprTrap, StringUtils::format("trap_%d",rnd));
	CCASSERT(trapBody!=nullptr,"GameScene::addTrap.trapBody is null!");
	//设置物体不受重力系数影响  
	trapBody->setGravityEnable(false);
	//设置碰撞过滤(不碰撞，只做碰撞检测)
	trapBody->setCategoryBitmask(1);
	trapBody->setContactTestBitmask(2);
	trapBody->setCollisionBitmask(0);

	//Vec2 offset=bodyTrap->getPositionOffset();
	//CCLOGERROR("bodyTrap->getPositionOffset()=(%d,%d)",offset.x,offset.y);

	//把刚体与精灵绑定  
	sprTrap->setPhysicsBody(trapBody); 

	//sprTrap->setAnchorPoint(Point::ZERO);
	sprTrap->setPosition(winSize.width+sprTrap->getBoundingBox().size.width/2,trapY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::TRAP));//标识为陷阱
	sprTrap->setUserData((void *)rnd);//作为扣血量设置进去
	this->addChild(sprTrap);

	//v_traps.pushBack(sprTrap);//压入向量Vecotr

	//移动时间、速度计算
	//float groundSpeed=winSize.width*4/bgCycleTime * 2;	//地面滑行速度(*2是草皮的速度) pix/s
	float trapRunTime=(winSize.width+sprTrap->boundingBox().size.width)/(GlobalVar::_roadSpeed);

	auto mov=MoveBy::create(trapRunTime,
		Point(-(winSize.width + sprTrap->boundingBox().size.width), 0));

	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);

	sprTrap->runAction(sequAction);

}

//删除陷阱
void TrapsNode::delTrap(Node* sender){
	removeChild(sender);
	CCLOGERROR("the trap(%d) is removed from scene.",(int)sender->getUserData());
}

void TrapsNode::update(float delta){
	if(rtTrap.isTimeOut()){
		srand(GetTime()+rand());
		if(rand()%2==0){//随机
			newTrap();
		}else{
			newAirTrap();
		}
	}
}