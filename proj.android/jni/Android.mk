LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared
#LOCAL_CFLAGS += -DCOCOS2D_DEBUG=1
LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/GlobalVar.cpp \
                   ../../Classes/MyBodyParser.cpp \
                   ../../Classes/OverScene.cpp \
                   ../../Classes/ActorSprite.cpp \
                   ../../Classes/MyParticleCache.cpp \
                   ../../Classes/DictionaryLoader.cpp \
                   ../../Classes/MapNode.cpp \
                   ../../Classes/ScoreNode.cpp \
                   ../../Classes/TrapsNode.cpp \
                   ../../Classes/BaseLayer.cpp \
                   ../../Classes/DialogLayer.cpp \
                   ../../Classes/LoadingScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_gui_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
 $(call import-module,editor-support/cocostudio)
# $(call import-module,network)

 $(call import-module,ui)
 $(call import-module,extensions)
