#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxScrollNode.h"

class Game : public cocos2d::Scene
{
public:
    cocos2d::Sprite* sprite;
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;

    CCParallaxScrollNode *parallax;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);

    void OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

    void update(float delta);
};

#endif // __GAME_SCENE_H__
