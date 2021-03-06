# 山寨跑酷游戏之———跳跳楠

* 引擎版本：Cocos2d-x 3.2
* 开发环境：Win7、VS2012
* Android NDK： r9d

这是本人在学习cocos2d-x过程中的一个练习作品，模仿了各种移动端横版跑酷游戏的玩法，玩家通过划屏操作控制游戏角色的跑步、飞行、跳跃、滑行、摔倒等各种状态切换以躲避障碍物、或是拾取金币；

游戏分为以下几个场景：
* LoadingScene:负责加载游戏资源（图片、动画、声音、缓存粒子效果等）；
* MenuScene：游戏主菜单；
* GameScene：主游戏场景，控制角色与各种道具的动作，监听用户事件及处理；
* OverScene：游戏结束后显示得分情况；

Layer层次：
由于cocos2d-x是通过设置Node的逻辑Z-Order来控制节点在渲染时的绘制顺序，所以我这里通过给不同的游戏道具给它们分类组别，通过设置不同Z-Order来实现：
* MapNode-最底层（最先绘制），以下按顺序先后绘制；
* TrapsNode-陷阱等道具（含金币）；
* ActorSprite-演员（游戏角色）；
* ScoreNode-得分情况（跑动距离、金币数、血量等）；

碰撞检测：
* 利用cocos2d-x 3.2集成的chipmunk物理引擎来现实；
* 多边形顶点绘制工具：我用的是physics-body-editor，这是一个开源项目，在此获取：[https://code.google.com/p/box2d-editor/][1]


模态窗口：
* 由于cocos2d-x 3.0之后的版本对事件分发机制做了较大的修改，该版本之上实现模态窗口相对简单很多，只需要创建一个遮罩层（项目中的DialogLayer.cpp）盖在所有其他层之上，捕获触摸事件之后设置不再往下传递即可；

关于按键监控：
* 游戏中玩家需要按系统中的”返回“或者”菜单“键来控制游戏暂停或退出，所以我这里定义了一个基类BaseLayer来监控这些按键事件，项目中所有的场景都继承自该基类；


关于Android 4.4+版本：
* 由于Android 4.4之后的版本新增了沉浸模式，可以隐藏底下的4个功能键，这无疑给游戏玩家带来了更好的体验，所以我们发布到Android平台的游戏应该启用这个模式，我通过修改项目目录下的proj.android\src\org\cocos2dx\cpp\AppActivity.java文件来实现，参考了这个帖子中提到的实现方法[http://discuss.cocos2d-x.org/t/how-to-set-full-screen-on-android-4-4/10278][2]，我修改了一下可以完美兼容4.4以前的版本；

关于性能优化：
* 纹理图片：利用TexturePacker工具打包纹理，生成相应的plist和png文件；
* 动画帧：利用SpriteFrameCache实现动画帧缓存；
* 音效：利用SimpleAudioEngine的preloadEffect进行预加载；
* 粒子效果：项目中用ParticleSystemQuad加载粒子效果的plist文件，并把它缓存起来（按单例模式调用）；

关于游戏名称：
* 其实整个开发过程中直至开发完成我并没有想过给这个游戏取名字，只是当初我为了取悦女儿（小名楠楠）用了她的照片做为角色头像剪辑，然后把程序导入到手机上给她试玩，没想到她很是喜欢，以至于后来某天她突然对着我说：”爸爸我要玩……那个……跳跳楠“，呵呵，我想这其实是个好名字，那就这么叫它吧！

版权声明：
* 游戏中的图片、声音等游戏资源大部分来源于网络，仅供学习之用，请勿发布做商业用途。

[1]: https://code.google.com/p/box2d-editor/
[2]: http://discuss.cocos2d-x.org/t/how-to-set-full-screen-on-android-4-4/10278
