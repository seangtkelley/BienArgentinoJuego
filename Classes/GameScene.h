#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Game : public cocos2d::Scene
{
public:
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;

    cocos2d::Sprite* bg1;
    cocos2d::Sprite* bg2;
    cocos2d::Sprite* bg3;
    cocos2d::Sprite* bg4;

    cocos2d::Sprite* player;
    cocos2d::Sprite* carObstacle;

    cocos2d::ActionInterval* lens;
    cocos2d::ActionInterval* waves;

    int carObstacleSpeedX;
    int carObstacleSpeedY;

    int timer;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);

    void OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

    void update(float) override;

    void UpdateTimer(float);
};

#endif // __GAME_SCENE_H__
