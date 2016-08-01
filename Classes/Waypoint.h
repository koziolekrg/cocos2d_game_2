#ifndef __HelloCpp__Waypoint__
#define __HelloCpp__Waypoint__

#include "cocos2d.h"
#include "HelloWorldScene.h"
USING_NS_CC;

class Waypoint:public Node {

public:
    Waypoint();
    ~Waypoint();

    static Waypoint* nodeWithTheGame(HelloWorld* game, Point location);
    bool initWithTheGame(HelloWorld* game, Point location);

    CC_SYNTHESIZE(Point, _myPosition, MyPosition)
    CC_SYNTHESIZE(Waypoint*, _nextWaypoint, NextWaypoint)

private:
        HelloWorld* theGame;

};

#endif /* defined(__HelloCpp__Waypoint__) */
