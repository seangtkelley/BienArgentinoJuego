#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->windowSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // create menu items
    auto play_menu_item = MenuItemFont::create("Jugar", CC_CALLBACK_1(MainMenu::play, this));
    auto highscores_menu_item = MenuItemFont::create("Puntajes", CC_CALLBACK_1(MainMenu::highscores, this));
    auto instructions_menu_item = MenuItemFont::create("Instrucciones", CC_CALLBACK_1(MainMenu::instructions, this));
    auto quit_menu_item = MenuItemFont::create("Salir", CC_CALLBACK_1(MainMenu::quit, this));

    play_menu_item->setPosition(this->visibleSize.width/2, (this->visibleSize.height/5) * 4);
    highscores_menu_item->setPosition(this->visibleSize.width/2, (this->visibleSize.height/5) * 3);
    instructions_menu_item->setPosition(this->visibleSize.width/2, (this->visibleSize.height/5) * 2);
    quit_menu_item->setPosition(this->visibleSize.width/2, (this->visibleSize.height/5) * 1);

    play_menu_item->setColor(Color3B(27, 50, 95));
    highscores_menu_item->setColor(Color3B(27, 50, 95));
    instructions_menu_item->setColor(Color3B(27, 50, 95));
    quit_menu_item->setColor(Color3B(27, 50, 95));

    // create menu, it's an autorelease object
    this->menu = Menu::create(play_menu_item, highscores_menu_item, instructions_menu_item, quit_menu_item, NULL);
    this->menu->setPosition(Vec2::ZERO);
    this->addChild(this->menu, 1);

    // add a label shows "Hello World"
    // create and initialize a label
    auto label = Label::createWithTTF("Bien Argentino", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + this->visibleSize.width/2,
                            origin.y + this->visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    auto background = Sprite::create("MainMenuBackground.jpg");
    float scale = MAX(this->visibleSize.width / background->getContentSize().width, this->visibleSize.height / background->getContentSize().height);
    background->setScale(scale);
    background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    this->addChild(background, 0);

    return true;
}

void MainMenu::play(cocos2d::Ref *pSender)
{
  auto gameScene = Game::createScene();
  Director::getInstance()->pushScene(TransitionCrossFade::create(1.5, gameScene));
  return;
}

void MainMenu::highscores(cocos2d::Ref *pSender)
{
  MessageBox("highscores clicked", "Msg");
  return;
}

void MainMenu::instructions(cocos2d::Ref *pSender)
{
  MessageBox("instructions clicked", "Msg");
  return;
}

void MainMenu::quit(cocos2d::Ref *pSender)
{
  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

  #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
  #endif
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
