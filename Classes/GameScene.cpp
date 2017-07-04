#include "GameScene.h"
#include "PrelimInstScene.h"
#include "DrunkInstScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Game::createScene()
{
  auto scene = Scene::create();
  auto layer = Game::create();
  scene->addChild(layer);
  return scene;
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
    origin = Director::getInstance()->getVisibleOrigin();
    timer = 0.0;

    // open sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game-assets.plist");

    // create scrolling background
    bg1 = Sprite::createWithSpriteFrameName("game-background.jpg");
    bg2 = Sprite::createWithSpriteFrameName("game-background.jpg");
    bg3 = Sprite::createWithSpriteFrameName("game-background.jpg");
    bg4 = Sprite::createWithSpriteFrameName("game-background.jpg");

    float bgscale = MAX(visibleSize.width / bg1->getContentSize().width, visibleSize.height / bg1->getContentSize().height);
    bg1->setScale(bgscale);
    bg2->setScale(bgscale);
    bg3->setScale(bgscale);
    bg4->setScale(bgscale);

    bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    bg2->setPosition(Vec2(bg1->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));
    bg3->setPosition(Vec2(bg2->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));
    bg4->setPosition(Vec2(bg3->getPosition().x + bg1->getContentSize().width, bg1->getPosition().y));

    this->addChild(bg1);
    this->addChild(bg2);
    this->addChild(bg3);
    this->addChild(bg4);

    // create pause var
    isPaused = false;

    // create player car
    player = Sprite::createWithSpriteFrameName("red-car.png");
    player->setPosition(Vec2(origin.x + player->getContentSize().width, visibleSize.height/2 + origin.y));
    this->addChild(player, 2);

    // add accelerometer listener for player car
    Device::setAccelerometerEnabled(true);
    auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(Game::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);

    // create car obstacle
    carObstacle = Sprite::createWithSpriteFrameName("red-car.png");
    carObstacleSpeedX = 100;
    carObstacle->setPosition(bg2->getPosition());
    this->addChild(carObstacle, 2);

    // rotate car obstacle
    auto initRotate = RotateBy::create(0.1, 180);
    carObstacle->runAction(initRotate);

    // create tree obstacles

    // create rock obstacles

    // start timer
    this->schedule(schedule_selector(Game::updateTimer), 0.1f);

    this->scheduleUpdate();
    return true;
}

void Game::update(float delta)
{
  if(timer > 1.0 && timer < 15.0){

    // scroll background images
    this->scrollBackground(1500, delta);

    // update car obstacles
    auto position = carObstacle->getPosition();
    position.x -=  carObstacleSpeedX * delta;
    if (position.x < 0 - (carObstacle->getBoundingBox().size.width / 2)){
      carObstacleSpeedX = cocos2d::RandomHelper::random_int(500, 1000);
      carObstacleSpeedY = cocos2d::RandomHelper::random_int(-100, 100);
      position.x = this->getBoundingBox().getMaxX() + carObstacle->getBoundingBox().size.width/2;
      position.y = visibleSize.height/2;
    }
    carObstacle->setPosition(position);

  } else if(timer >= 15.0 && timer < 16.0) {
    // display instructions about driving drunk
    if(!isPaused){
      auto drunkInstScene = DrunkInst::createScene();
      Director::getInstance()->pushScene(TransitionCrossFade::create(1.0, drunkInstScene));
      isPaused = true;
    }
  } else if (timer > 15) {
    // unpause game
    if(isPaused){
      isPaused = false;
    }

    /* Implementation of effects for future reference
    lens = Lens3D::create(10, Size(32,24), Vec2(100,180), 150);
    waves = Waves3D::create(10, Size(15,10), 18, 15);
    auto nodeGrid = NodeGrid::create();
    nodeGrid->addChild(player);
    nodeGrid->runAction(RepeatForever::create((Sequence*)Sequence::create(waves, lens, NULL)));
    this->addChild(nodeGrid);
    */

    // scroll background images
    this->scrollBackground(1500, delta);
  }
}

void Game::updateTimer(float dt)
{
  timer += 0.1;
}

void Game::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
  auto w = visibleSize.width;
  auto h = visibleSize.height;

  auto oldX = player->getPosition().x;
  auto oldY = player->getPosition().y;

  auto newX = oldX + (acc->x * w * 0.075);
  auto newY = oldY + (acc->y * h * 0.075);

  if(newX > player->getBoundingBox().size.width/2 && newX < this->getBoundingBox().getMaxX()-(player->getContentSize().width/2)){
    if(newY > player->getBoundingBox().size.height * 2 && newY < this->getBoundingBox().getMaxY()-player->getBoundingBox().size.height * 2){
      player->setPosition(newX, newY);
    } else {
      player->setPosition(newX, oldY);
    }
  } else {
    if(newY > player->getBoundingBox().size.height * 2 && newY < this->getBoundingBox().getMaxY()-player->getBoundingBox().size.height * 2){
      player->setPosition(oldX, newY);
    } else {
      player->setPosition(oldX, oldY);
    }
  }
}

void Game::scrollBackground(int bgspeed, float delta){
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
