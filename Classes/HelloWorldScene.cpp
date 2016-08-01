#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Tower.h"
#include "Waypoint.h"
#include "Enemy.h"

#include <vector>
USING_NS_CC;

struct Coordinate {
    Coordinate(int _x, int _y){x=_x; y= _y;}
    int x;
    int y;
};

struct Wave{
    Wave(int s){spawnTime = s;}
    float spawnTime;
};

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    Sprite * background = Sprite::create("bg.png");
    background->setPosition(Point(winSize.width/2, winSize.height/2));
    this->addChild(background);
    this->loadTowerPositions();
    _towers = Array::create();
    _towers->retain();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->addWaypoints();

    wave =0;
    ui_wave_lbl = cocos2d::LabelBMFont::create(String::createWithFormat("WAVE:%d",wave)->getCString(), "fonts/font_red_14.fnt");
    this->addChild(ui_wave_lbl, 10);
    ui_wave_lbl->setPosition(Point(400, winSize.height-12));
    ui_wave_lbl->setAnchorPoint(Point(0, 0.5));

    _enemies = Array::create();
    _enemies->retain();
    this->loadWave();

    gameOver = false;
    playerHP=5;
    ui_hp_lbl = cocos2d::LabelBMFont::create(String::createWithFormat("HP: %d", playerHP)->getCString(),"fonts/font_red_14.fnt");
    this->addChild(ui_hp_lbl,10);
    ui_hp_lbl->setPosition(Point(35, winSize.height-12));


    playerGold = 1000;
    ui_gold_lbl = cocos2d::LabelBMFont::create(String::createWithFormat("GOLD: %d", playerGold)->getCString(),"fonts/font_red_14.fnt");
    this->addChild(ui_gold_lbl);
    ui_gold_lbl->setPosition(Point(135, winSize.height-12));
    ui_gold_lbl->setAnchorPoint(Point(0,0.5f));


    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("shoot.wav");//0
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("life_lose.wav");//1
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("tower_place.wav");//2
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("enemy_destroy.wav");//3
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("8bitDungeonLevel.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("8bitDungeonLevel.mp3");


    return true;
}

bool HelloWorld::deinit(){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("8bitDungeonLevel.mp3");

    towerBases->release();
    _towers->release();
    _waypoints->release();
    _enemies->release();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();

}


void HelloWorld::loadTowerPositions(){


    std::vector <Coordinate*> towerPositions;
    towerPositions.push_back(new Coordinate(80,266));
    towerPositions.push_back(new Coordinate(170,266));
    towerPositions.push_back(new Coordinate(260,266));
    towerPositions.push_back(new Coordinate(350,266));

    towerPositions.push_back(new Coordinate(125,174));
    towerPositions.push_back(new Coordinate(215,174));
    towerPositions.push_back(new Coordinate(305,174));
    towerPositions.push_back(new Coordinate(395,174));

    towerPositions.push_back(new Coordinate(80,80));
    towerPositions.push_back(new Coordinate(170,80));
    towerPositions.push_back(new Coordinate(260,80));
    towerPositions.push_back(new Coordinate(350,80));

    towerBases = Array::create();
    towerBases->retain();

    for(auto towerPos : towerPositions)
    {
        Sprite* towerBase = Sprite::create("open_spot.png");
        towerBase->setScale(1);
        this->cocos2d::Node::addChild(towerBase);
        towerBase->setPosition(Point(towerPos->x, towerPos->y));
        towerBases->addObject(towerBase);
    }

}

bool HelloWorld::canBuyTower(){
    if(playerGold - kTOWER_COST >=0 )
        return true;
    else
        return false;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    Point location = touch->getLocation();
    Object* pObject = NULL;
    CCARRAY_FOREACH(towerBases, pObject){
        Sprite* tb = (Sprite*)pObject;
        if (this->canBuyTower() && tb->getBoundingBox().containsPoint(location) && !tb->getUserData()){
            Tower* tower = Tower::nodeWithTheGame(this, tb->getPosition());
            _towers->addObject(tower);
            tb->setUserData(tower);
            playerGold-= kTOWER_COST;
            ui_gold_lbl->setString(String::createWithFormat("GOLD: %d",playerGold)->getCString());

            HelloWorld::playSound(3);

        }
    }
}

void HelloWorld::addWaypoints(){
    _waypoints = Array::create();
    _waypoints->retain();

    Waypoint* waypoint1 = Waypoint::nodeWithTheGame(this, Point(430,32));
    _waypoints->addObject(waypoint1);

    Waypoint* waypoint2 = Waypoint::nodeWithTheGame(this, Point(33,30));
    _waypoints->addObject(waypoint2);
    waypoint2->setNextWaypoint(waypoint1);

    Waypoint* waypoint3 = Waypoint::nodeWithTheGame(this, Point(33, 125));
    _waypoints->addObject(waypoint3);
    waypoint3->setNextWaypoint(waypoint2);

    Waypoint* waypoint4 = Waypoint::nodeWithTheGame(this, Point(445, 125));
    _waypoints->addObject(waypoint4);
    waypoint4->setNextWaypoint(waypoint3);

    Waypoint* waypoint5 = Waypoint::nodeWithTheGame(this, Point(445, 220));
    _waypoints->addObject(waypoint5);
    waypoint5->setNextWaypoint(waypoint4);

    Waypoint* waypoint6 = Waypoint::nodeWithTheGame(this, Point(-50, 220));
    _waypoints->addObject(waypoint6);
    waypoint6->setNextWaypoint(waypoint5);

}

bool HelloWorld::collisonWithCircle(Point circlePoint, float radius, Point circlePointTwo, float radiusTwo){
    float xdif = circlePoint.x - circlePointTwo.x;
    float ydif = circlePoint.y - circlePointTwo.y;

    float distance = sqrt(xdif*xdif + ydif*ydif);

    if(distance <= radius + radiusTwo){
        return true;
    }else{
        return false;
    }
}

bool HelloWorld::loadWave(){

    std::vector <Wave*> waveData;

    waveData.push_back(new Wave(0));
    waveData.push_back(new Wave(1));
    waveData.push_back(new Wave(2));
    if(wave>0){
        waveData.push_back(new Wave(3));
        waveData.push_back(new Wave(4));
    }
    if(wave>1){
        waveData.push_back(new Wave(5));
        waveData.push_back(new Wave(5));
    }
    if(wave>2){
        waveData.push_back(new Wave(7));
        waveData.push_back(new Wave(8));
    }
    if(wave>3){
        waveData.push_back(new Wave(9));
        waveData.push_back(new Wave(10));
    }

    if(wave>=4){
        return false;
    }

    for(auto pWave : waveData){
        Enemy *enemy = Enemy::nodeWithTheGame(this);
        _enemies->addObject(enemy);
        enemy->schedule(schedule_selector(Enemy::doActivate), pWave->spawnTime);
    }
    wave++;
    ui_wave_lbl->setString(String::createWithFormat("WAVE:%d",wave)->getCString());


    return true;
}

void HelloWorld::enemyGotKilled(){
    if(_enemies->count()<=0){
        if(!this->loadWave()){
            Director::getInstance()->replaceScene(TransitionSplitCols::create(1, HelloWorld::createScene()));
        }
    }
}

void HelloWorld::getHpDamage(){
    playerHP--;
    ui_hp_lbl->setString(String::createWithFormat("HP: %d",playerHP)->getCString());
    if(playerHP<=0){
        this->doGameOver();
    }
}

void HelloWorld::doGameOver(){
    if(!gameOver){
        Director::getInstance()->replaceScene(TransitionRotoZoom::create(1, HelloWorld::createScene()));
    }
}

void HelloWorld::awardGold( int gold){
    playerGold+=gold;
    ui_gold_lbl->setString(String::createWithFormat("GOLD: %d", playerGold)->getCString());
}

void HelloWorld::playSound(int id){

    switch (id){
    case 1:  sound.push_back(CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("life_lose.wav"));
        break;
    case 2:  sound.push_back(CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("shoot.wav"));
        break;
    case 3:  sound.push_back( CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("enemy_destroy.wav"));
        break;
    case 4:  sound.push_back(    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tower_place.wav"));
        break;
    }

    while(sound.size()>4)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(sound.front());
        sound.pop_front();
    }


}
