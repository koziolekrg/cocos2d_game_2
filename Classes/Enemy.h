
#ifndef __ENEMY_SCENE_H__
#define __ENEMY_SCENE_H__

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Waypoint.h"
#include "SimpleAudioEngine.h"

class Tower;

class Enemy : public Node
{
public:
    Enemy();
    ~Enemy();

    static Enemy* nodeWithTheGame(HelloWorld* game);
    bool initWithTheGame(HelloWorld *game);
    void doActivate(float dt);
    void getRemoved();
    void update(float delta);

    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    void onDraw();

    CC_SYNTHESIZE(HelloWorld*, _theGame, TheGame)
    CC_SYNTHESIZE(Sprite*, _mySprite, MySprite)

    void getAttacked(Tower* attacker);
    void gotLostSight(Tower* attacker);
    void getDamaged(int damage);
private:
    Point myPosition;
    int maxHP;
    int currentHP;
    float walkingSpeed;
    Waypoint* destinationWaypoint;
    bool active;

    cocos2d::Array *attackedBy;
    CustomCommand _customCommand;

    int id_effect;
};

#endif // __ENEMY_SCENE_H__


