#include "TrapsNode.h"
#include "MyBodyParser.h"
#include "GlobalVar.h"
#include "DictionaryLoader.h"

#ifndef winSize
#define winSize GlobalVar::winSize
#endif // !winSize

USING_NS_CC;

//Size TrapsNode::DEFAULT_COIN_NODE_SIZE=Size(150,90);//Ĭ�ϵĽ��NODE��С
//const float TrapsNode::TRAP_X_SPACE=100.0f;//�������ҵľ���
//const float TrapsNode::_airCoinOffsetY=45.0f;//���ڿ��������µĽ��������ľ���
//const float _sinFromHat2Trap=20.0f;//���Ҳ�Ģ��ͷ�ײ�������ľ���
//const float TrapsNode::_sinXscale=1.0f;//���Ҳ�����������ȵı���
//const float TrapsNode::_sinYscale=1.0f;//���Ҳ��߶�������߶ȵı���

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

	//��С���Node����
	_minCoinNodes=GlobalVar::_level/3;
	if(GlobalVar::_level%3==0)--_minCoinNodes;
	_minCoinNodes=3-_minCoinNodes;
	if(_minCoinNodes<1)_minCoinNodes=1;//ÿ3��һ�飺3,2,1
	
	//���������Node����
	_maxRandCoinNodes=_minCoinNodes;//ÿ3��һ�飺3,2,1

	//��ʼ��ʣ����Node����
	resetRemainCoinNodes();

	//����һ����ҽڵ�
	auto coinNode=newCoinNode(DEFAULT_COIN_NODE_SIZE,10);
	coinNode->setTag((int)E_NODE_TYPE::COIN);
	addCoinNode(coinNode);

	scheduleUpdate();
	return true;
}

TrapsNode::TrapsNode():/*rtTrap(1.0f,2),*/_Nodes(20){

}

Sprite* TrapsNode::getAirTrapSpriteById(int sprIdx,int for_i, float baseY){
	auto sprTrap=Sprite::createWithSpriteFrameName(StringUtils::format("air_trap_%d.png",sprIdx));//���ȡһ������
	//�󶨸���
	MyBodyParser::getInstance()->parseJsonFile(StringUtils::format("shapes/air_trap_%d.json",sprIdx));
	auto airTrapBody = MyBodyParser::getInstance()->bodyFormJson(sprTrap, StringUtils::format("air_trap_%d",sprIdx));
	CCASSERT(airTrapBody!=nullptr,"GameScene::addTrap.airTrapBody is null!");
	//�������岻������ϵ��Ӱ��  
	airTrapBody->setGravityEnable(false);
	//������ײ����(����ײ��ֻ����ײ���)
	airTrapBody->setCategoryBitmask(1);//01b
	airTrapBody->setContactTestBitmask(2);//10b
	airTrapBody->setCollisionBitmask(0);
	//�Ѹ����뾫���  
	sprTrap->setPhysicsBody(airTrapBody); 
	CCLOGERROR("airTrapBody0:%d,airTrapBody1:%d",airTrapBody->getCategoryBitmask(), airTrapBody->getContactTestBitmask());

	//sprTrap->setAnchorPoint(Point::ZERO);
	sprTrap->setPosition(sprTrap->getBoundingBox().size.width*(2*for_i+1)/2,baseY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::AIR_TRAP));//��ʶΪ��������
	sprTrap->setUserData((void *)sprIdx);//��Ϊ��Ѫ�����ý�ȥ
	
	return sprTrap;
}

Sprite* TrapsNode::createCoin(){
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
		//spr->setPosition(Vec2((*it).x,(*it).y));
		//movableNode->addChild(spr);

		//�����֡����
		auto animation=AnimationCache::getInstance()->getAnimation("coinsAnim");
		auto animate=Animate::create(animation);
		auto repeatAct=RepeatForever::create(animate);

		spr->runAction(repeatAct);

		return spr;
}




//��װһ���������壬Ϊ����Ͻ��
Node* TrapsNode::packAirTrapWithCoin(Sprite* spr){
	Node* group2=Node::create();//Node������Ĭ�ϵ�ê����(0,0)��
	group2->setAnchorPoint(spr->getAnchorPoint());
	group2->setContentSize(spr->getBoundingBox().size);
	group2->setPosition(spr->getPosition());
	spr->setPosition(group2->getContentSize().width/2,group2->getContentSize().height/2);
	group2->addChild(spr);

	float airCoinOffsetY=45.0f;//���ڿ��������µĽ��������ľ���
	for(int i=0;i<=1;++i){
		auto coin=createCoin();
		coin->setPosition(group2->getContentSize().width*(2*i+1)/4, -airCoinOffsetY);//1/4,3/4
		group2->addChild(coin);
	}
	return group2;
}

//���һ����������
void TrapsNode::newAirTrap(){
	//123��1�ţ�456��2�ţ��������Ϲؿ�3��
	int num=GlobalVar::_level/3;
	if(GlobalVar::_level%3 > 0)++num;
	if(num>3)num=3;

	int sprIdx;//�������
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

	float roadY=128+2;//·��߶�
	float actorRunY=95+144;//�ܲ�ʱ��ͷ����ظ߶�
	float actorSlideY=190;//�»�ʱ��ͷ����ظ߶�
	Node* groupNode=Node::create();
	//groupNode->setContentSize(winSize);
	//groupNode->setAnchorPoint(Vec2::ZERO);
	//groupNode->setPosition(Vec2(winSize.width,0));
	

	//num=3;//test
	float groupWidth=0;
	for(int i=0;i<num;++i){
		//sprIdx=2;//test
		Sprite* spr;
		if(sprIdx==1){//1�ţ���е��
			spr=getAirTrapSpriteById(sprIdx,i,actorSlideY);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);
		}
		else if(sprIdx==2){//2�ţ���Ͱ
			spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);

			//std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//�˶�����
			//float dura=atof(dictStr.c_str());
			group2->runAction(RepeatForever::create(JumpBy::create(TrapsNode_newAirTrap_oil_down_duration,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
		else{//3�ţ�����
			spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			auto group2=packAirTrapWithCoin(spr);
			groupNode->addChild(group2);

			//std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//�˶�����
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

	
	//Action����	//���ҵ���
	/*float trapRunTime=winSize.width*2/GlobalVar::_roadSpeed;
	auto mov=MoveBy::create(trapRunTime,Point(-(winSize.width*2), 0));
	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	groupNode->runAction(sequAction);*/

}


/*
//����һ��ƥ������size��С�ĳ��ؽ�ҵ�Node�ڵ�
sinXscale-���Ҳ�Ģ��ͷ��X�����������ȵĹ�ϵ����ȼ�һ���̶�ֵ��
isIgnoreFirstCoin-�Ƿ�����׽ڵ�
spaceX-��ǰһ���ڵ���X��ļ��

�������߿ɱ�ʾΪy=Asin(��x+��)+k������Ϊ����y=Asin(��x+��)+k��ֱ������ϵ�ϵ�ͼ������sinΪ���ҷ��ţ�x��ֱ������ϵx���ϵ���ֵ��y����ͬһֱ������ϵ�Ϻ�����Ӧ��yֵ��k���غͦ��ǳ�����k���ء��ա�R�Ҧء�0��
�������壺
A������������������켣�����������ߵ�ֱ�������˶�ʱ����ֵΪ�г̵�1/2��
(��x+��)������λ����ӳ����y������״̬��
�ա������࣬x=0ʱ����λ����ӳ������ϵ����Ϊͼ��������ƶ���
k����ƫ�࣬��ӳ������ϵ����Ϊͼ������ƻ����ơ�
�ء������ٶȣ� ������������(��λ�Ƕ����𶯵Ĵ���)��
*/
Node* TrapsNode::newCoinNode(Size& size,float sinXscale,bool isIgnoreFirstCoin, float spaceX){
	const int trapY=80;//���Node���Y����
	Node* group2=Node::create();//Node������Ĭ�ϵ�ê����(0,0)��
	/*auto testBg=Sprite::create("white.png");
	testBg->setAnchorPoint(Vec2::ZERO);
	testBg->setPosition(Vec2::ZERO);
	group2->addChild(testBg);*/
	
	//������Ҳ���·�ϵĽ��
	const float sinYscale=10.0f;//���Ҳ�Ģ��ͷ��Y����������߶ȵı��������A��
	//const float sinFromHat2Trap=1.0f;//���Ҳ�Ģ��ͷ�ײ�������ľ���(ƫ��k)

	const int splitCount=5;//��Ҹ���������180�ȷֳɶ��ٸ��ȷݣ�
	const float RADIAN_RATIO=3.1416f/180;//�Ƕ�ת���ȵ�ϵ��
	const float ANGLE_INCREMENT=180.0f/(splitCount-1);//�Ƕ�����(���ٶȦ�)
	
	const float xIncrement=(sinXscale+size.width)/(splitCount-1);//x��������
	//const float baseY=size.height+SPACE_FROM_SIN_HAT_2_TRAP;//��ƫ��k��
	float x=0;//-sinXscale/2;//��һ�����X����
	for(int i=0;i<splitCount;++i){		
		float y= sinYscale * sin(ANGLE_INCREMENT*RADIAN_RATIO*i+0) + (size.height+SPACE_FROM_SIN_HAT_2_TRAP);//y=Asin(��x+��)+k

		auto coin=createCoin();
		coin->setPosition(x,y);

		x+=xIncrement;
		if(i==0 && isIgnoreFirstCoin)continue;//�Ƿ�����׽ڵ�
		group2->addChild(coin);
	}
	group2->setContentSize(Size(size.width+sinXscale,size.height));//���ð�Χ�д�С

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
//��װһ���������壬Ϊ����Ͻ��
*/
Node* TrapsNode::packTrapWithCoin(Sprite* spr){
	

	Size sprSize=spr->boundingBox().size;
	const float sinXscale=58.0f;//���Ҳ�Ģ��ͷ��X�����������ȵĹ�ϵ����ȼ�һ���̶�ֵ��
	auto group2=newCoinNode(sprSize,sinXscale,false,TRAP_X_SPACE_BEFORE);
	group2->setTag((int)E_NODE_TYPE::TRAP);

	//�������
	//group2->setAnchorPoint(spr->getAnchorPoint());//(0.5,0.5)
	//group2->setContentSize(spr->getBoundingBox().size);
	//group2->setPosition(spr->getPosition());
	
	spr->setPosition(group2->getContentSize().width/2,group2->getContentSize().height/2);
	group2->addChild(spr);

	return group2;
}


//���һ����������
void TrapsNode::newTrap(){
	//const int trapY=80;//���Y����
	//srand(GetTime()+rand());
	int rnd=rand()%8+1;
	//rnd=8;	//����
	CCLOGERROR("the NO. of trap is:%d",rnd);
	auto sprTrap=Sprite::createWithSpriteFrameName(StringUtils::format("trap_%d.png",rnd));//���ȡһ������
	//�󶨸���
	MyBodyParser::getInstance()->parseJsonFile(StringUtils::format("shapes/trap_%d.json",rnd));
	auto trapBody = MyBodyParser::getInstance()->bodyFormJson(sprTrap, StringUtils::format("trap_%d",rnd));
	CCASSERT(trapBody!=nullptr,"GameScene::addTrap.trapBody is null!");
	//�������岻������ϵ��Ӱ��  
	trapBody->setGravityEnable(false);
	//������ײ����(����ײ��ֻ����ײ���)
	trapBody->setCategoryBitmask(1);
	trapBody->setContactTestBitmask(2);
	trapBody->setCollisionBitmask(0);

	//Vec2 offset=bodyTrap->getPositionOffset();
	//CCLOGERROR("bodyTrap->getPositionOffset()=(%d,%d)",offset.x,offset.y);

	//�Ѹ����뾫���  
	sprTrap->setPhysicsBody(trapBody); 

	//sprTrap->setPosition(winSize.width+sprTrap->getBoundingBox().size.width/2,trapY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::TRAP));//��ʶΪ����
	sprTrap->setUserData((void *)rnd);//��Ϊ��Ѫ�����ý�ȥ
	
	auto group2=packTrapWithCoin(sprTrap);
	//group2->setPosition(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()+TRAP_X_SPACE,trapY+sprTrap->getBoundingBox().size.height/2);
	this->addChild(group2);
	_Nodes.pushBack(group2);
	//group2->setContentSize(Size(0,0));

	//v_traps.pushBack(sprTrap);//ѹ������Vecotr

	//�ƶ�ʱ�䡢�ٶȼ���
	/*float trapRunTime=(winSize.width+sprTrap->boundingBox().size.width)/(GlobalVar::_roadSpeed);

	auto mov=MoveBy::create(trapRunTime,
		Point(-(winSize.width + sprTrap->boundingBox().size.width), 0));

	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	
	group2->runAction(sequAction);*/

}

//ɾ������
//void TrapsNode::delTrap(Node* sender){
//	removeChild(sender);
//	CCLOGERROR("the trap(%d) is removed from scene.",(int)sender->getUserData());
//}

//���ô����Ľ��NODE��
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
		if(_Nodes.at(_Nodes.size()-1)->getBoundingBox().getMaxX()<=winSize.width){//Ҫ����µĽڵ�
			if(_remainCoinNodes>0){//��ӽ��
				auto nodeCoin=newCoinNode(DEFAULT_COIN_NODE_SIZE,10,true);
				nodeCoin->setTag((int)E_NODE_TYPE::COIN);
				addCoinNode(nodeCoin);
			}else{//�������
				if(rand()%2==0){//���
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
	//	if(rand()%2==0){//���
	//		newTrap();
	//	}else{
	//		newAirTrap();
	//	}
	//}
}