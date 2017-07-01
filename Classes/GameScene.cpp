#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Game::createScene()
{
    return Game::create();
}

// on "init" you need to initialize your instance
bool Game::init()
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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Game::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // create parallax background
    bg1 = Sprite::create("GameBackground.jpg");
    bg2 = Sprite::create("GameBackground.jpg");
    bg3 = Sprite::create("GameBackground.jpg");
    bg4 = Sprite::create("GameBackground.jpg");

    float scale = MAX(visibleSize.width / bg1->getContentSize().width, visibleSize.height / bg1->getContentSize().height);
    bg1->setScale(scale);
    bg2->setScale(scale);
    bg3->setScale(scale);
    bg4->setScale(scale);

    bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    bg2->setPosition(Vec2(bg1->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));
    bg3->setPosition(Vec2(bg2->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));
    bg4->setPosition(Vec2(bg3->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));

    this->addChild(bg1);
    this->addChild(bg2);
    this->addChild(bg3);
    this->addChild(bg4);

    player = Sprite::create("RedCar.png");

    player->setPosition(Vec2(origin.x + player->getContentSize().width, visibleSize.height/2 + origin.y));

    this->addChild(player, 2);

    Device::setAccelerometerEnabled(true);
    auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(Game::OnAcceleration, this));

    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);

    this->scheduleUpdate();
    return true;
}

void Game::update(float delta)
{
  auto speed = 400;

  auto position = bg1->getPosition();
  position.x -= speed * delta;
  if (position.x  < 0 - (bg1->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg1->getBoundingBox().size.width/2;
  bg1->setPosition(position);

  position = bg2->getPosition();
  position.x -= speed * delta;
  if (position.x  < 0 - (bg2->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg2->getBoundingBox().size.width/2;
  bg2->setPosition(position);

  position = bg3->getPosition();
  position.x -= speed * delta;
  if (position.x  < 0 - (bg3->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg3->getBoundingBox().size.width/2;
  bg3->setPosition(position);

  position = bg4->getPosition();
  position.x -= speed * delta;
  if (position.x  < 0 - (bg4->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg4->getBoundingBox().size.width/2;
  bg4->setPosition(position);
}

void Game::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
  auto w = visibleSize.width;
  auto h = visibleSize.height;

  auto x = player->getPosition().x;
  auto y = player->getPosition().y;

  x = x + (acc->x * w * 0.075);
  y = y + (acc->y * h * 0.075);

  if(x > (player->getContentSize().width/2) && x <= w-(player->getContentSize().width/2) && y > (player->getContentSize().height) && y <= h+(player->getContentSize().height/2)){
    player->setPosition(x, y);
  }
}

void Game::menuCloseCallback(Ref* pSender)
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
