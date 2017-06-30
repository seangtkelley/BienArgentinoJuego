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

    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->windowSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Game::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + this->visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // create parallax background
    Sprite *bg1 = Sprite::create("GameBackground.jpg");
    Sprite *bg2 = Sprite::create("GameBackground.jpg");
    Sprite *bg3 = Sprite::create("GameBackground.jpg");

    float scale = MAX(this->visibleSize.width / bg1->getContentSize().width, this->visibleSize.height / bg1->getContentSize().height);

    bg1->setScale(scale);
    bg2->setScale(scale);
    bg3->setScale(scale);

    //background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    this->parallax->addInfiniteScrollXWithZ(0, Point(1, 1), Point(0,0), bg1, bg2, bg3, NULL);
    this->addChild(this->parallax);

    // add "HelloWorld" splash screen"
    this->sprite = Sprite::create("RedCar.png");

    // position the sprite on the center of the screen
    this->sprite->setPosition(Vec2(origin.x + sprite->getContentSize().width, this->visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(this->sprite, 2);

    Device::setAccelerometerEnabled(true);
    auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(Game::OnAcceleration, this));

    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);

    return true;
}

void Game::update(float delta)
{
	this->parallax->updateWithVelocity(Point(-0.1, 0), delta);
}

void Game::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
  auto w = this->visibleSize.width;
  auto h = this->visibleSize.height;

  auto x = this->sprite->getPosition().x;
  auto y = this->sprite->getPosition().y;

  x = x + (acc->x * w * 0.075);
  y = y + (acc->y * h * 0.075);

  if(x > (sprite->getContentSize().width/2) && x <= w-(sprite->getContentSize().width/2) && y > (sprite->getContentSize().height) && y <= h+(sprite->getContentSize().height/2)){
    this->sprite->setPosition(x, y);
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
