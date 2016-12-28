#include "AppDelegate.h"
#include "LoadingScene.h"
//#include "GlobalVar.h"
//#include "cocos2d.h"
//#include "DictionaryLoader.h"

//#define COCOS2D_DEBUG 1		//调试模式，仅输出CCLOGERROR调试信息

USING_NS_CC;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLView::create("My Game");
		glview=GLView::createWithRect("NanNanRun", Rect(0,0,800,480));

        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(800,480,ResolutionPolicy::FIXED_HEIGHT);//固定高度 用于横版游戏

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	srand(time(NULL));//随机数种子

	

    // create a scene. it's an autorelease object
    auto scene = LoadingScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
