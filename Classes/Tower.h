#ifndef __TOWER_SCENE_H__
#define __TOWER_SCENE_H__

#include "cocos2d.h"
#include "HelloWorldScene.h"

#define kTOWER_COST 300

USING_NS_CC;

class Enemy;
class Tower : public Node
{
public:
    Tower();
    ~Tower();

    static Tower* nodeWithTheGame(HelloWorld* game, Point location);
    bool initWithTheGame(HelloWorld* game, Point location);

    void update(float delta);
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    void onDraw();

    CC_SYNTHESIZE(HelloWorld*, _theGame, TheGame)
    CC_SYNTHESIZE(Sprite*, _mySprite, MySprite)

    void attackEnemy();
    void chosenEnemyForAttack(Enemy *enemy);
    void shootWeapon(float delta);
    void removeBullet(Sprite* bullet);
    void damageEnemy();
    void targetKilled();
    void lostSightOfEnemy();

private:
    int attackRange;
    int damage;
    float fireRate;
    bool attacking;
    Enemy *chosenEnemy;
    CustomCommand _customCommand;

    int max;
   int id_effect_1;
   int id_effect_2;
};

#endif // __TOWER_SCENE_H__
