#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();

    scene->addChild(layer);

    return scene;
}

bool MenuScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundSprite = Sprite::create("Background.png");
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2+origin.y));

    this->addChild(backgroundSprite);

    auto titleSprite = Sprite::create("Title.png");
    titleSprite->setScale(3);
    titleSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));
    this->addChild(titleSprite);

    auto playItem = MenuItemImage::create("play.png", "play_clicked.png", CC_CALLBACK_1(MenuScene::GoToGameScene, this));
    playItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    playItem->setScale(3);
    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);

    this->addChild(menu);

    return true;

}

void MenuScene::GoToGameScene(cocos2d::Ref *sender){

    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));

}
