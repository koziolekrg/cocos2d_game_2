#include "Waypoint.h"
USING_NS_CC;

Waypoint::Waypoint(){
    _nextWaypoint = NULL;
}

Waypoint::~Waypoint(){

}

Waypoint* Waypoint::nodeWithTheGame(HelloWorld *game, Point location){
    Waypoint* pRet = new Waypoint();
    if(pRet && pRet->initWithTheGame(game,location)){

    }else{
        CC_SAFE_RELEASE_NULL(pRet);
    }
    return pRet;
}

bool Waypoint::initWithTheGame(HelloWorld *game, Point location){
    bool bRet = false;
    do{
        theGame = game;
        _myPosition = location;

        this->setPosition(Point::ZERO);
        theGame->addChild(this);

        bRet=true;
    }while(0);
    return bRet;
}
