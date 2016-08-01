#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MenuScene);

private:
    void GoToGameScene(cocos2d::Ref *sender);

};

#endif // __MAINMENU_SCENE_H__
