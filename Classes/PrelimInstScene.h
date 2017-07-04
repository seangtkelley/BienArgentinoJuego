#ifndef __PRELIMINST_SCENE_H__
#define __PRELIMINST_SCENE_H__

#include "cocos2d.h"

class PrelimInst : public cocos2d::Scene
{
  public:
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(PrelimInst);

    void update(float) override;

    void startGame(float);

  private:
    cocos2d::Label* prelimInst;
    cocos2d::Sprite* pauseOverlay;

};

#endif // __PRELIMINST_SCENE_H__
