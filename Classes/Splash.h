#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__
#include "SimpleAudioEngine.h"

#include "cocos2d.h"

class Splash : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Splash);

private:
    void GoToMenuScene(float dt);

};

#endif // __SPLASH_SCENE_H__
