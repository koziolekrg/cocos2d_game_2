#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <fstream>
class Wave;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual bool deinit();

    void loadTowerPositions();
    CREATE_FUNC(HelloWorld)
    CC_SYNTHESIZE_RETAIN(cocos2d::Array*, _towers, Towers)
    CC_SYNTHESIZE_RETAIN(cocos2d::Array* , _waypoints, Waypoints)
    CC_SYNTHESIZE_RETAIN(cocos2d::Array*, _enemies, Enemies)

    void addWaypoints();
    bool collisonWithCircle(cocos2d::Point circlePoint, float radius, cocos2d::Point circlePointTwo, float radiusTwo);

    bool canBuyTower();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    bool loadWave();
    void enemyGotKilled();
    void getHpDamage();
    void doGameOver();
    void awardGold(int gold);
    void playSound(int sound);
private:
    cocos2d::Array* towerBases;
    int wave;

    cocos2d::LabelBMFont *ui_wave_lbl;

    int playerHP;
    cocos2d::LabelBMFont *ui_hp_lbl;
    bool gameOver;

    int playerGold;
    cocos2d::LabelBMFont *ui_gold_lbl;
    std::list <int> sound;


};

#endif // __HELLOWORLD_SCENE_H__
