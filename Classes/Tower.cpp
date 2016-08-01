#include "Tower.h"
#include "SimpleAudioEngine.h"
#include "Enemy.h"
#include <vector>

Tower::Tower(){
    max = 4;
    id_effect_1=0;
    id_effect_2 =0;
}

Tower::~Tower(){

}

Tower* Tower::nodeWithTheGame(HelloWorld *game, Point location){
    Tower* pRet = new Tower();
    if(pRet && pRet->initWithTheGame(game, location)){
        return pRet;
    }
    else{
        CC_SAFE_RELEASE_NULL(pRet);
        return pRet;
    }
}

bool Tower::initWithTheGame(HelloWorld *game, Point location){
    bool bRet = false;
    chosenEnemy = NULL;
    do{
        attackRange = 70;
        damage = 10;
        fireRate =0.9;

        _mySprite = Sprite::create("tower.png");
        _mySprite->setScale(1);
        this->addChild(_mySprite);

        _mySprite->setPosition(location);
        _theGame = game;
        _theGame->addChild(this);

        this->scheduleUpdate();

        bRet = true;
    }while(0);

    return bRet;
}

void Tower::update(float delta){

    if(chosenEnemy){
        Point normalized = Point(chosenEnemy->getMySprite()->getPosition().x - _mySprite->getPosition().x, chosenEnemy->getMySprite()->getPosition().y - _mySprite->getPosition().y);
        normalized.normalize();
        _mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x))-270);

        if(!_theGame->collisonWithCircle(_mySprite->getPosition(), attackRange, chosenEnemy->getMySprite()->getPosition(), 1)){
            this->lostSightOfEnemy();
        }
    }else{
        Object *pObject = NULL;
        CCARRAY_FOREACH(_theGame->getEnemies(), pObject){
            Enemy *enemy = (Enemy*)pObject;
            if(_theGame->collisonWithCircle(_mySprite->getPosition(),attackRange, enemy->getMySprite()->getPosition(), 1)){
                this->chosenEnemyForAttack(enemy);
                break;
            }
        }
    }


}

void Tower::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    Node::draw(renderer,transform,flags);

    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(Tower::onDraw, this);
    renderer->addCommand(&_customCommand);
}

void Tower::onDraw(){
#ifdef COCOS2D_DEBUG

    DrawPrimitives::setDrawColor4F(255.0f, 255.0f, 255.0f, 255.0f);
    DrawPrimitives::drawCircle(_mySprite->getPosition(), attackRange, 360, 30, false);

#endif
}

void Tower::attackEnemy(){
    this->schedule(schedule_selector(Tower::shootWeapon), fireRate);
}

void Tower::chosenEnemyForAttack(Enemy *enemy){
    chosenEnemy = NULL;
    chosenEnemy = enemy;
    this->attackEnemy();
    enemy->getAttacked(this);
}

void Tower::shootWeapon(float delta){
    Sprite* bullet = Sprite::create("bullet.png");
    _theGame->addChild(bullet);
    bullet->setPosition(_mySprite->getPosition());


    bullet->runAction(Sequence::create(MoveTo::create(0.1f, chosenEnemy->getMySprite()->getPosition()), CallFunc::create(std::bind(&Tower::damageEnemy, this)), CallFuncN::create(std::bind(&Tower::removeBullet, this, bullet)), NULL));


    _theGame->playSound(1);


}

void Tower::removeBullet(Sprite* bullet){
    bullet->getParent()->removeChild(bullet);
}

void Tower::damageEnemy(){
    if(chosenEnemy){
        chosenEnemy->getDamaged(damage);
    }
}

void Tower::targetKilled(){
    if(chosenEnemy){
        _theGame->playSound(4);

        chosenEnemy = NULL;
    }
    this->unschedule(schedule_selector(Tower::shootWeapon));
}

void Tower::lostSightOfEnemy(){
    chosenEnemy->gotLostSight(this);
    if(chosenEnemy){
        chosenEnemy = NULL;
    }
    this->unschedule(schedule_selector(Tower::shootWeapon));
}

