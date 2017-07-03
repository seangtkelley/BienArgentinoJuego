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

    // initialize game variables
    visibleSize = Director::getInstance()->getVisibleSize();
    windowSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // create scrolling background
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

    // create player car
    player = Sprite::create("RedCar.png");
    player->setPosition(Vec2(origin.x + player->getContentSize().width, visibleSize.height/2 + origin.y));
    this->addChild(player, 2);
    /* Implementation of effects for future reference
    lens = Lens3D::create(10, Size(32,24), Vec2(100,180), 150);
    waves = Waves3D::create(10, Size(15,10), 18, 15);
    auto nodeGrid = NodeGrid::create();
    nodeGrid->addChild(player);
    nodeGrid->runAction(RepeatForever::create((Sequence*)Sequence::create(waves, lens, NULL)));
    this->addChild(nodeGrid);
    */

    // add accelerometer listener for player car
    Device::setAccelerometerEnabled(true);
    auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(Game::OnAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);

    // create car obstacle
    carObstacle = Sprite::create("RedCar.png");
    carObstacleSpeedX = 100;
    carObstacle->setPosition(bg2->getPosition());
    this->addChild(carObstacle, 2);

    // rotate car obstacle
    auto initRotate = RotateBy::create(0.1, 180);
    carObstacle->runAction(initRotate);

    // create tree obstacles

    // create rock obstacles

    // start timer
    this->schedule(schedule_selector(Game::UpdateTimer),1.0f);

    this->scheduleUpdate();
    return true;
}

void Game::update(float delta)
{
  // check game variables
  if(timer == 5){
    //player->runAction(waves);
  }

  // scroll background images
  auto bgspeed = 400;

  auto position = bg1->getPosition();
  position.x -= bgspeed * delta;
  if (position.x  < 0 - (bg1->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg1->getBoundingBox().size.width/2;
  bg1->setPosition(position);

  position = bg2->getPosition();
  position.x -= bgspeed * delta;
  if (position.x  < 0 - (bg2->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg2->getBoundingBox().size.width/2;
  bg2->setPosition(position);

  position = bg3->getPosition();
  position.x -= bgspeed * delta;
  if (position.x  < 0 - (bg3->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg3->getBoundingBox().size.width/2;
  bg3->setPosition(position);

  position = bg4->getPosition();
  position.x -= bgspeed * delta;
  if (position.x  < 0 - (bg4->getBoundingBox().size.width / 2))
    position.x = this->getBoundingBox().getMaxX() + bg4->getBoundingBox().size.width/2;
  bg4->setPosition(position);

  // update car obstacles
  position = carObstacle->getPosition();
  position.x -=  carObstacleSpeedX * delta;
  if (position.x  < 0 - (carObstacle->getBoundingBox().size.width / 2)){
    carObstacleSpeedX = cocos2d::RandomHelper::random_int(100, 500);
    carObstacleSpeedY = cocos2d::RandomHelper::random_int(-100, 100);
    position.x = this->getBoundingBox().getMaxX() + carObstacle->getBoundingBox().size.width/2;
    position.y = visibleSize.height/2;
  }
  carObstacle->setPosition(position);

  // update tree obstacles

  // update car obstacles
}

void Game::UpdateTimer(float dt)
{
  timer++;
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
