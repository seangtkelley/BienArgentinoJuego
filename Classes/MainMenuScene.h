#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    cocos2d::Size visibleSize;
    cocos2d::Size windowSize;
    cocos2d::Menu *menu;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // menu handlers
    void play(Ref *pSender);
    void highscores(Ref *pSender);
    void instructions(Ref *pSender);
    void quit(Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);

};

#endif // __MAINMENU_SCENE_H__
