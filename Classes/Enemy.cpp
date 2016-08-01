#include "Enemy.h"
#include "Tower.h"
#define HEALTH_BAR_WIDTH 40
#define HEALTH_BAR_ORIGIN -20

Enemy::Enemy(){
}

Enemy::~Enemy(){

}

Enemy *Enemy::nodeWithTheGame(HelloWorld *game){
    Enemy *pRet = new Enemy();
    if(pRet && pRet->initWithTheGame(game)){

    }else{
        CC_SAFE_RELEASE_NULL(pRet);
    }
    return pRet;
}

bool Enemy::initWithTheGame(HelloWorld *game){
    bool pRet = false;
    attackedBy = Array::create();
    attackedBy->retain();

    do{
        maxHP = 49;
        currentHP = maxHP;
        active = false;
        walkingSpeed = 1.0f;
        _theGame = game;
        _mySprite = Sprite::create("enemy.png");
        this->addChild(_mySprite);

        Waypoint *waypoint = (Waypoint*)_theGame->getWaypoints()->getObjectAtIndex(_theGame->getWaypoints()->count() -1);
        destinationWaypoint = waypoint->getNextWaypoint();
        Point pos = waypoint->getMyPosition();
        myPosition = pos;
        _mySprite->setPosition(pos);
        _theGame->addChild(this);
        this->scheduleUpdate();
        pRet = true;
    }while(0);
    return pRet;

}


void Enemy::doActivate(float dt){
    active = true;
}

void Enemy::getRemoved(){
    this->getParent()->removeChild(this);
    _theGame->getEnemies()->removeObject(this);
    _theGame->enemyGotKilled();

    Object* pObject = NULL;
    CCARRAY_FOREACH(attackedBy, pObject)
    {
        Tower* attacker = (Tower*)pObject;
        attacker->targetKilled();
    }

}

void Enemy::update(float delta){
    if (!active){
        return ;
    }

    if(_theGame->collisonWithCircle(myPosition, 1, destinationWaypoint->getMyPosition(),1)){
        if(destinationWaypoint->getNextWaypoint()){
            destinationWaypoint = destinationWaypoint->getNextWaypoint();
        }else{
            _theGame->getHpDamage();
            this->getRemoved();
        }
    }

    Point targetPoint = destinationWaypoint->getMyPosition();
    float movementSpeed = walkingSpeed;

    Point normalized = Point(targetPoint.x - myPosition.x, targetPoint.y - myPosition.y);
    normalized.normalize();

    _mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)));
    myPosition = Point(myPosition.x + normalized.x * movementSpeed, myPosition.y+normalized.y * movementSpeed);

    _mySprite->setPosition(myPosition);
}

void Enemy::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    Node::draw(renderer,transform,flags);

     _customCommand.init(_globalZOrder);
     _customCommand.func = CC_CALLBACK_0(Enemy::onDraw, this);
      renderer->addCommand(&_customCommand);
}

void Enemy::onDraw(){
    Point healthBarBack [] = {
        Point(_mySprite->getPosition().x - 20, _mySprite->getPosition().y + 32),
        Point(_mySprite->getPosition().x + 20, _mySprite->getPosition().y + 32),
        Point(_mySprite->getPosition().x + 20, _mySprite->getPosition().y + 28),
        Point(_mySprite->getPosition().x - 20, _mySprite->getPosition().y + 28)
    };


    DrawPrimitives::drawSolidPoly(healthBarBack, 4, Color4F(255, 0, 0, 255));


    Point healthBar[] = {
        Point(_mySprite->getPosition().x + HEALTH_BAR_ORIGIN, _mySprite->getPosition().y + 32),
        Point(_mySprite->getPosition().x + HEALTH_BAR_ORIGIN + (float)(currentHP *HEALTH_BAR_WIDTH ) /maxHP, _mySprite->getPosition().y + 32),
        Point(_mySprite->getPosition().x + HEALTH_BAR_ORIGIN + (float)(currentHP * HEALTH_BAR_WIDTH) / maxHP, _mySprite->getPosition().y + 28),
        Point(_mySprite->getPosition().x + HEALTH_BAR_ORIGIN, _mySprite->getPosition().y + 28),
    };

    DrawPrimitives::drawSolidPoly(healthBar, 4, Color4F(0,255,0,255));

}


void Enemy::getAttacked(Tower* attacker)
{
    attackedBy->addObject(attacker);

}

void Enemy::gotLostSight(Tower* attacker)
{
    attackedBy->removeObject(attacker);
}

void Enemy::getDamaged(int damage)
{
    _theGame->playSound(2);

    currentHP -= damage;

    if (currentHP <= 0) {
        this->getRemoved();
        _theGame->awardGold(200);
    }

}




