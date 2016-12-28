#include "TrapsNode.h"
#include "MyBodyParser.h"
#include "GlobalVar.h"
#include "DictionaryLoader.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

USING_NS_CC;

//Size TrapsNode::DEFAULT_COIN_NODE_SIZE=Size(150,90);//默认的金币NODE大小
//const float TrapsNode::TRAP_X_SPACE=100.0f;//陷阱离金币的距离
//const float TrapsNode::_airCoinOffsetY=45.0f;//悬在空中陷阱下的金币离陷阱的距离
//const float _sinFromHat2Trap=20.0f;//正弦波蘑菇头底部离陷阱的距离
//const float TrapsNode::_sinXscale=1.0f;//正弦波跨度与陷阱宽度的比例
//const float TrapsNode::_sinYscale=1.0f;//正弦波高度与陷阱高度的比例

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

void TrapsNode::addCoinNode(Node* coinNode){
	if(_remainCoinNodes<=0)return;

	
	_Nodes.pushBack(coinNode);
	addChild(coinNode);
	--_remainCoinNodes;
}

bool TrapsNode::init(){
	if(!Node::init()){
		return false;
	}
	setContentSize(winSize);
	TRAP_X_SPACE_BEFORE=DictionaryLoader::getFloatByName("TrapsNode.TRAP_X_SPACE_BEFORE");
	TRAP_X_SPACE_AFTER=DictionaryLoader::getFloatByName("TrapsNode.TRAP_X_SPACE_AFTER");

	float f_DEFAULT_COIN_NODE_SIZE_X=DictionaryLoader::getFloatByName("TrapsNode.DEFAULT_COIN_NODE_SIZE_X");
	float f_DEFAULT_COIN_NODE_SIZE_Y=DictionaryLoader::getFloatByName("TrapsNode.DEFAULT_COIN_NODE_SIZE_Y");
	DEFAULT_COIN_NODE_SIZE=Size(f_DEFAULT_COIN_NODE_SIZE_X,f_DEFAULT_COIN_NODE_SIZE_Y);

	SPACE_FROM_SIN_HAT_2_TRAP=DictionaryLoader::getFloatByName("TrapsNode.SPACE_FROM_SIN_HAT_2_TRAP");

	TrapsNode_newAirTrap_oil_down_duration=DictionaryLoader::getFloatByName("TrapsNode.newAirTrap.oil.down.duration");

	//最小金币Node个数
	_minCoinNodes=GlobalVar::_level/3;
	if(GlobalVar::_level%3==0)--_minCoinNodes;
	_minCoinNodes=3-_minCoinNodes;
	if(_minCoinNodes<1)_minCoinNodes=1;//每3关一组：3,2,1
	
	//最大随机金币Node个数
	_maxRandCoinNodes=_minCoinNodes;//每3关一组：3,2,1

	//初始化剩余金币Node个数
	resetRemainCoinNodes();

	//创建一个金币节点
	auto coinNode=newCoinNode(DEFAULT_COIN_NODE_SIZE,10);
	coinNode->setTag((int)E_NODE_TYPE::COIN);
	addCoinNode(coinNode);

	scheduleUpdate();
	return true;
}

TrapsNode::TrapsNode():/*rtTrap(1.0f,2),*/_Nodes(20){

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

Sprite* TrapsNode::createCoin(){
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
		//spr->setPosition(Vec2((*it).x,(*it).y));
		//movableNode->addChild(spr);

		//后面的帧动画
		auto animation=AnimationCache::getInstance()->getAnimation("coinsAnim");
		auto animate=Animate::create(animation);
		auto repeatAct=RepeatForever::create(animate);

		spr->runAction(repeatAct);

		return spr;
}




//包装一个空中陷阱，为其带上金币
Node* TrapsNode::packAirTrapWithCoin(Sprite* spr){
	Node* group2=Node::create();//Node创建后默认的锚点是(0,0)！
	group2->setAnchorPoint(spr->getAnchorPoint());
	group2->setContentSize(spr->getBoundingBox().size);
	group2->setPosition(spr->getPosition());
	spr->setPosition(group2->getContentSize().width/2,group2->getContentSize().height/2);
	group2->addChild(spr);

	float airCoinOffsetY=45.0f;//悬在空中陷阱下的金币离陷阱的距离
	for(int i=0;i<=1;++i){
		auto coin=createCoin();
		coin->setPosition(group2->getContentSize().width*(2*i+1)/4, -airCoinOffsetY);//1/4,3/4
		group2->addChild(coin);
	}
	return group2;
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
		//srand(GetTime()+rand());
		sprIdx=rand()%2+1;
	}
	else{
		//srand(GetTime()+rand());
		sprIdx=rand()%3+1;
	}

	float roadY=128+2;//路面高度
	float actorRunY=95+144;//跑步时人头顶离地高度
	float actorSlideY=190;//下滑时人头顶离地高度
	Node* groupNode=Node::create();
	//groupNode->setContentSize(winSize);
	//groupNode->setAnchorPoint(Vec2::ZERO);
	//groupNode->setPosition(Vec2(winSize.width,0));
	

	//num=3;//test
	float groupWidth=0;
	for(int i=0;i<num;++i){
		//sprIdx=2;//test
		Sprite* spr;
		if(sprIdx==1){//1号，机械臂
			spr=getAirTrapSpriteById(sprIdx,i,actorSlideY);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);
		}
		else if(sprIdx==2){//2号，油桶
			spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);

			//std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//运动周期
			//float dura=atof(dictStr.c_str());
			group2->runAction(RepeatForever::create(JumpBy::create(TrapsNode_newAirTrap_oil_down_duration,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
		else{//3号，滑轮
			spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);

			//std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//运动周期
			//float dura=atof(dictStr.c_str());
			group2->runAction(RepeatForever::create(JumpBy::create(TrapsNode_newAirTrap_oil_down_duration,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
		groupWidth+=spr->getBoundingBox().size.width;
	}
	groupNode->setContentSize(Size(groupWidth,winSize.height));
	groupNode->setPosition(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()+TRAP_X_SPACE_BEFORE,0);
	groupNode->setTag((int)E_NODE_TYPE::TRAP);
	addChild(groupNode);
	_Nodes.pushBack(groupNode);

	
	//Action定义	//从右到左
	/*float trapRunTime=winSize.width*2/GlobalVar::_roadSpeed;
	auto mov=MoveBy::create(trapRunTime,Point(-(winSize.width*2), 0));
	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	groupNode->runAction(sequAction);*/

}


/*
//生成一个匹配陷阱size大小的承载金币的Node节点
sinXscale-正弦波蘑菇头的X轴跨度与陷阱宽度的关系（宽度加一个固定值）
isIgnoreFirstCoin-是否忽略首节点
spaceX-与前一个节点在X轴的间距

正弦曲线可表示为y=Asin(ωx+φ)+k，定义为函数y=Asin(ωx+φ)+k在直角坐标系上的图象，其中sin为正弦符号，x是直角坐标系x轴上的数值，y是在同一直角坐标系上函数对应的y值，k、ω和φ是常数（k、ω、φ∈R且ω≠0）
参数定义：
A——振幅，当物体作轨迹符合正弦曲线的直线往复运动时，其值为行程的1/2。
(ωx+φ)——相位，反映变量y所处的状态。
φ——初相，x=0时的相位；反映在坐标系上则为图像的左右移动。
k——偏距，反映在坐标系上则为图像的上移或下移。
ω——角速度， 控制正弦周期(单位角度内震动的次数)。
*/
Node* TrapsNode::newCoinNode(Size& size,float sinXscale,bool isIgnoreFirstCoin, float spaceX){
	const int trapY=80;//金币Node起点Y坐标
	Node* group2=Node::create();//Node创建后默认的锚点是(0,0)！
	/*auto testBg=Sprite::create("white.png");
	testBg->setAnchorPoint(Vec2::ZERO);
	testBg->setPosition(Vec2::ZERO);
	group2->addChild(testBg);*/
	
	//添加正弦波线路上的金币
	const float sinYscale=10.0f;//正弦波蘑菇头的Y轴跨度与陷阱高度的比例（振幅A）
	//const float sinFromHat2Trap=1.0f;//正弦波蘑菇头底部离陷阱的距离(偏距k)

	const int splitCount=5;//金币个数（即把180度分成多少个等份）
	const float RADIAN_RATIO=3.1416f/180;//角度转弧度的系数
	const float ANGLE_INCREMENT=180.0f/(splitCount-1);//角度增量(角速度ω)
	
	const float xIncrement=(sinXscale+size.width)/(splitCount-1);//x坐标增量
	//const float baseY=size.height+SPACE_FROM_SIN_HAT_2_TRAP;//（偏距k）
	float x=0;//-sinXscale/2;//第一个金币X坐标
	for(int i=0;i<splitCount;++i){		
		float y= sinYscale * sin(ANGLE_INCREMENT*RADIAN_RATIO*i+0) + (size.height+SPACE_FROM_SIN_HAT_2_TRAP);//y=Asin(ωx+φ)+k

		auto coin=createCoin();
		coin->setPosition(x,y);

		x+=xIncrement;
		if(i==0 && isIgnoreFirstCoin)continue;//是否忽略首节点
		group2->addChild(coin);
	}
	group2->setContentSize(Size(size.width+sinXscale,size.height));//设置包围盒大小

	float y=trapY/*+size.height/2*/;
	if(_Nodes.size()<=0){
		x=winSize.width;
	}else{
		auto lastNode=_Nodes.at(_Nodes.size()-1);
		x=lastNode->getBoundingBox().getMaxX()+spaceX;
		if(lastNode->getTag()==(int)E_NODE_TYPE::TRAP)x+=TRAP_X_SPACE_AFTER;
	}
	group2->setPosition(x,y);
	//group2->setPosition(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()+TRAP_X_SPACE,trapY+size.height/2);
	return group2;
}

/*
//包装一个地面陷阱，为其带上金币
*/
Node* TrapsNode::packTrapWithCoin(Sprite* spr){
	

	Size sprSize=spr->boundingBox().size;
	const float sinXscale=58.0f;//正弦波蘑菇头的X轴跨度与陷阱宽度的关系（宽度加一个固定值）
	auto group2=newCoinNode(sprSize,sinXscale,false,TRAP_X_SPACE_BEFORE);
	group2->setTag((int)E_NODE_TYPE::TRAP);

	//添加陷阱
	//group2->setAnchorPoint(spr->getAnchorPoint());//(0.5,0.5)
	//group2->setContentSize(spr->getBoundingBox().size);
	//group2->setPosition(spr->getPosition());
	
	spr->setPosition(group2->getContentSize().width/2,group2->getContentSize().height/2);
	group2->addChild(spr);

	return group2;
}


//添加一个地面陷阱
void TrapsNode::newTrap(){
	//const int trapY=80;//起点Y坐标
	//srand(GetTime()+rand());
	int rnd=rand()%8+1;
	//rnd=8;	//测试
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

	//sprTrap->setPosition(winSize.width+sprTrap->getBoundingBox().size.width/2,trapY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::TRAP));//标识为陷阱
	sprTrap->setUserData((void *)rnd);//作为扣血量设置进去
	
	auto group2=packTrapWithCoin(sprTrap);
	//group2->setPosition(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()+TRAP_X_SPACE,trapY+sprTrap->getBoundingBox().size.height/2);
	this->addChild(group2);
	_Nodes.pushBack(group2);
	//group2->setContentSize(Size(0,0));

	//v_traps.pushBack(sprTrap);//压入向量Vecotr

	//移动时间、速度计算
	/*float trapRunTime=(winSize.width+sprTrap->boundingBox().size.width)/(GlobalVar::_roadSpeed);

	auto mov=MoveBy::create(trapRunTime,
		Point(-(winSize.width + sprTrap->boundingBox().size.width), 0));

	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	
	group2->runAction(sequAction);*/

}

//删除陷阱
//void TrapsNode::delTrap(Node* sender){
//	removeChild(sender);
//	CCLOGERROR("the trap(%d) is removed from scene.",(int)sender->getUserData());
//}

//重置待消耗金币NODE数
void TrapsNode::resetRemainCoinNodes(){
	int rnd=rand()%_maxRandCoinNodes;
	CCLOGERROR("resetRemainCoinNodes's random:%d",rnd);
	_remainCoinNodes=_minCoinNodes+rnd;
	CCLOGERROR("resetRemainCoinNodes:%d",_remainCoinNodes);
}

void TrapsNode::update(float delta){
	//CCLOGERROR("_Nodes.size.before:%d",_Nodes.size());

	float moveRangeX=GlobalVar::_roadSpeed*delta;
	for(cocos2d::Vector<Node*>::iterator it=_Nodes.begin();it!=_Nodes.end();){//auto node:_Nodes
		(*it)->setPositionX((*it)->getPositionX()-moveRangeX);
		if((*it)->getBoundingBox().getMaxX()<0){
			(*it)->removeFromParentAndCleanup(true);
			it=_Nodes.erase(it);
			//_Nodes.eraseObject(it);
		}else{
			++it;	
		}
	}

	if(_Nodes.size()>0){
		if(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()<=winSize.width){//要添加新的节点
			if(_remainCoinNodes>0){//添加金币
				auto nodeCoin=newCoinNode(DEFAULT_COIN_NODE_SIZE,10,true);
				nodeCoin->setTag((int)E_NODE_TYPE::COIN);
				addCoinNode(nodeCoin);
			}else{//添加陷阱
				if(rand()%2==0){//随机
					newTrap();
				}else{
					newAirTrap();
				}
				resetRemainCoinNodes();
			}
		}
	}else{
	
	}

	//CCLOGERROR("_Nodes.size.end:%d",_Nodes.size());

	//if(rtTrap.isTimeOut()){
	//	if(rand()%2==0){//随机
	//		newTrap();
	//	}else{
	//		newAirTrap();
	//	}
	//}
}