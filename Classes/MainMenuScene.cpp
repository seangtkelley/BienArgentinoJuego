#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
  auto scene = Scene::create();
  auto layer = MainMenu::create();
  scene->addChild(layer);
  return scene;
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

    visibleSize = Director::getInstance()->getVisibleSize();
    windowSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // open sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game-assets.plist");

    // create menu items
    auto play_sprite = Sprite::createWithSpriteFrameName("buttons/jugar.png");
    auto play_selected_sprite = Sprite::createWithSpriteFrameName("buttons/jugar-selected.png");
    auto instrucciones_sprite = Sprite::createWithSpriteFrameName("buttons/instrucciones.png");
    auto instrucciones_selected_sprite = Sprite::createWithSpriteFrameName("buttons/instrucciones-selected.png");
    auto salir_sprite = Sprite::createWithSpriteFrameName("buttons/salir.png");
    auto salir_selected_sprite = Sprite::createWithSpriteFrameName("buttons/salir-selected.png");

    auto play_menu_item = MenuItemSprite::create(play_sprite, play_selected_sprite, CC_CALLBACK_1(MainMenu::play, this));
    //auto highscores_menu_item = MenuItemFont::create("Puntajes", CC_CALLBACK_1(MainMenu::highscores, this));
    auto instructions_menu_item = MenuItemSprite::create(instrucciones_sprite, instrucciones_selected_sprite, CC_CALLBACK_1(MainMenu::instructions, this));
    auto quit_menu_item = MenuItemSprite::create(salir_sprite, salir_selected_sprite, CC_CALLBACK_1(MainMenu::quit, this));

    play_menu_item->setScale(2.0);
    instructions_menu_item->setScale(2.0);
    quit_menu_item->setScale(2.0);

    play_menu_item->setPosition(visibleSize.width/2, (visibleSize.height/6) * 4);
    //highscores_menu_item->setPosition(visibleSize.width/2, (visibleSize.height/5) * 3);
    instructions_menu_item->setPosition(visibleSize.width/2, (visibleSize.height/6) * 3);
    quit_menu_item->setPosition(visibleSize.width/2, (visibleSize.height/6) * 2);

    // create menu, it's an autorelease object
    menu = Menu::create(play_menu_item, /*highscores_menu_item,*/ instructions_menu_item, quit_menu_item, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);

    // create and initialize logo
    auto logo = Sprite::createWithSpriteFrameName("square-logo.png");

    // position the logo on the center of the screen
    logo->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - visibleSize.height/4.5));

    // add the label as a child to this layer
    this->addChild(logo, 1);

    auto background = Sprite::createWithSpriteFrameName("main-menu-background.jpg");
    float scale = MAX(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
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
