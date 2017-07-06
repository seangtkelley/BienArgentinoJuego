#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);

    void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

    void update(float) override;

    void updateTimer(float);

  private:
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;
    cocos2d::Vec2 origin;

    cocos2d::Sprite* bg1;
    cocos2d::Sprite* bg2;
    cocos2d::Sprite* bg3;
    cocos2d::Sprite* bg4;
    int bgspeed;

    cocos2d::Label* prelimInst;
    cocos2d::Label* drunkInst;
    cocos2d::Sprite* pauseOverlay;
    bool isPaused;

    cocos2d::Sprite* player;

    cocos2d::Sprite* carObstacleRight;
    cocos2d::Sprite* carObstacleLeft;

    cocos2d::Vector<cocos2d::Sprite*> treeObstacles;

    cocos2d::Vector<cocos2d::Sprite*> rockObstacles;

    cocos2d::ActionInterval* lens;
    cocos2d::ActionInterval* waves;

    int carObstacleRightSpeed;
    int carObstacleLeftSpeed;

    float timer;

    void scrollBackground(float);

    void updateObstacles(float);

    void resetObstacles();
};

#endif // __GAME_SCENE_H__
