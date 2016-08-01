#include "Splash.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Splash::createScene()
{
    auto scene = Scene::create();
    auto layer = Splash::create();

    scene->addChild(layer);

    return scene;
}

bool Splash::init()
{

    if ( !Layer::init() )
    {
        return false;
    }


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    this->scheduleOnce(schedule_selector(Splash::GoToMenuScene), 2);

    auto backgroundSprite = Sprite::create("splash.png");
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    this->addChild(backgroundSprite);


    return true;

}

void Splash::GoToMenuScene(float dt){
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}
