#ifndef __DRUNKINST_SCENE_H__
#define __DRUNKINST_SCENE_H__

#include "cocos2d.h"

class DrunkInst : public cocos2d::Scene
{
  public:
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(DrunkInst);

    void popSelf(float);

  private:
    cocos2d::Label* drunkInst;
    cocos2d::Sprite* pauseOverlay;

};

#endif // __DRUNKINST_SCENE_H__
