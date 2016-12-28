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
		srand(GetTime()+rand());
		sprIdx=rand()%2+1;
	}
	else{
		srand(GetTime()+rand());
		sprIdx=rand()%3+1;
	}

	float roadY=128+2;//·��߶�
	float actorRunY=95+144;//�ܲ�ʱ��ͷ����ظ߶�
	float actorSlideY=190;//�»�ʱ��ͷ����ظ߶�
	Node* groupNode=Node::create();
	groupNode->setContentSize(Size(1,1));
	groupNode->setAnchorPoint(Vec2::ZERO);
	groupNode->setPosition(Vec2(winSize.width,0));
	addChild(groupNode);

	//num=3;//test
	for(int i=0;i<num;++i){
		//sprIdx=2;//test
		if(sprIdx==1){//1�ţ���е��
			groupNode->addChild(getAirTrapSpriteById(sprIdx,i,actorSlideY));
		}
		else if(sprIdx==2){//2�ţ���Ͱ
			auto spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			groupNode->addChild(spr);

			std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//�˶�����
			float dura=atof(dictStr.c_str());
			spr->runAction(RepeatForever::create(JumpBy::create(dura,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
		else{//3�ţ�����
			auto spr=getAirTrapSpriteById(sprIdx,i,winSize.height-150);
			groupNode->addChild(spr);

			std::string dictStr=DictionaryLoader::getStrByName("TrapsNode.newAirTrap.oil.down.duration");//�˶�����
			float dura=atof(dictStr.c_str());
			spr->runAction(RepeatForever::create(JumpBy::create(dura,Vec2::ZERO,-(winSize.height-150-actorSlideY),1)));
		}
	}

	//Action����

	//���ҵ���
	float trapRunTime=winSize.width*2/GlobalVar::_roadSpeed;
	auto mov=MoveBy::create(trapRunTime,Point(-(winSize.width*2), 0));
	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);
	groupNode->runAction(sequAction);

}

//���һ����������
void TrapsNode::newTrap(){
	const int trapY=80;//���Y����
	srand(GetTime()+rand());
	int rnd=rand()%8+1;
	//rnd=4;	//����
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

	//sprTrap->setAnchorPoint(Point::ZERO);
	sprTrap->setPosition(winSize.width+sprTrap->getBoundingBox().size.width/2,trapY+sprTrap->getBoundingBox().size.height/2);
	sprTrap->setName(MACRO_ENUM2STR(E_NODE_NAMES::TRAP));//��ʶΪ����
	sprTrap->setUserData((void *)rnd);//��Ϊ��Ѫ�����ý�ȥ
	this->addChild(sprTrap);

	//v_traps.pushBack(sprTrap);//ѹ������Vecotr

	//�ƶ�ʱ�䡢�ٶȼ���
	//float groundSpeed=winSize.width*4/bgCycleTime * 2;	//���滬���ٶ�(*2�ǲ�Ƥ���ٶ�) pix/s
	float trapRunTime=(winSize.width+sprTrap->boundingBox().size.width)/(GlobalVar::_roadSpeed);

	auto mov=MoveBy::create(trapRunTime,
		Point(-(winSize.width + sprTrap->boundingBox().size.width), 0));

	auto sequAction=Sequence::create(mov,
		CallFuncN::create(CC_CALLBACK_1(TrapsNode::delTrap, this)),
		nullptr);

	sprTrap->runAction(sequAction);

}

//ɾ������
void TrapsNode::delTrap(Node* sender){
	removeChild(sender);
	CCLOGERROR("the trap(%d) is removed from scene.",(int)sender->getUserData());
}

void TrapsNode::update(float delta){
	if(rtTrap.isTimeOut()){
		srand(GetTime()+rand());
		if(rand()%2==0){//���
			newTrap();
		}else{
			newAirTrap();
		}
	}
}