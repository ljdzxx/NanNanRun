#ifndef ModalDialogDemo_DialogLayer_h
#define ModalDialogDemo_DialogLayer_h

#include "cocos2d.h"

USING_NS_CC;

class DialogLayer: public LayerColor
{
    // 模态对话框菜单
    //Menu *m_pMenu;
    // 记录菜单点击
    //bool m_bTouchedMenu;
    
public:
    DialogLayer();
    ~DialogLayer();
    
    virtual bool init();
    // 初始化对话框内容
    void initDialog();
    
    //LAYER_NODE_FUNC(DialogLayer);
	CREATE_FUNC(DialogLayer);

    void onEnter();
    void onExit();
    

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override; 
    /*virtual void onTouchMoved(Touch *touch, Event *unused_event) override; 
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override; 
    virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;*/
    
    void okMenuItemCallback(Ref *pSender);
    void cancelMenuItemCallback(Ref *pSender);

private:
	void initMenu();
	void onContinueClick(Ref *sender);
	void onExitClick(Ref *sender);
	void delayExecute(float delta);
	void onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event);
};

#endif
