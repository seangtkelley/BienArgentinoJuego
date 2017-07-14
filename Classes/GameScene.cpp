#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Game::createScene()
{
  auto scene = Scene::createWithPhysics();
  auto layer = Game::create();
  layer->SetPhysicsWorld(scene->getPhysicsWorld());
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
    isPaused = true;
    isDrunk = false;
    isCrashed = false;
    bgspeed = 1500;
    lives = 3;

    // create node grid for all sprites
    spriteGrid = NodeGrid::create();

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

    spriteGrid->addChild(bg1);
    spriteGrid->addChild(bg2);
    spriteGrid->addChild(bg3);
    spriteGrid->addChild(bg4);

    // create instructions and pause screen
    prelimInst = Label::create("¡Evitá los obstaculos por 15 segundos! Tenés tres vidas.", "Helvetica", 72);
    prelimInst->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(prelimInst, 10);

    drunkInst = Label::create("¿Ahora, podés evitar los obstaculos \n mientras estás alcoholizado? \n Tenés tres vidas de nuevo.", "Helvetica", 72);
    drunkInst->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(drunkInst, 10);
    drunkInst->setOpacity(0);

    crashInst = Label::create("¡Ah, chocaste tu carro! Durabas por 0 segundos.", "Helvetica", 72);
    crashInst->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(crashInst, 10);
    crashInst->setOpacity(0);

    pauseOverlay = Sprite::createWithSpriteFrameName("pause-overlay.png");
    float pauseScale = MAX(visibleSize.width / pauseOverlay->getContentSize().width, visibleSize.height / pauseOverlay->getContentSize().height);
    pauseOverlay->setScale(pauseScale);
    pauseOverlay->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(pauseOverlay, 9);
    pauseOverlay->setOpacity(0.75*255);

    // create play again menu
    auto jugar_de_nuevo_sprite = Sprite::createWithSpriteFrameName("buttons/jugar-de-nuevo.png");
    auto jugar_de_nuevo_selected_sprite = Sprite::createWithSpriteFrameName("buttons/jugar-de-nuevo-selected.png");
    jugar_de_nuevo_menu_item = MenuItemSprite::create(jugar_de_nuevo_sprite, jugar_de_nuevo_selected_sprite, CC_CALLBACK_1(Game::playAgain, this));
    jugar_de_nuevo_menu_item->setScale(2.0);
    jugar_de_nuevo_menu_item->setPosition(visibleSize.width/2, (visibleSize.height/6) * 3);
    playAgainMenu = Menu::create(jugar_de_nuevo_menu_item, NULL);
    playAgainMenu->setPosition(Vec2::ZERO);
    this->addChild(playAgainMenu, 10);
    playAgainMenu->setOpacity(0);

    // create player car
    player = Sprite::createWithSpriteFrameName("red-car.png");
    player->setPosition(Vec2(origin.x + player->getContentSize().width, visibleSize.height/2 + origin.y));
    auto playerBody = PhysicsBody::createBox(player->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    playerColBitmask = 1;
    playerBody->setCollisionBitmask(playerColBitmask);
    playerBody->setContactTestBitmask(0xFFFFFFFF);
    playerBody->setDynamic(false);
    player->setPhysicsBody(playerBody);
    spriteGrid->addChild(player, 2);

    // add accelerometer listener for player car
    Device::setAccelerometerEnabled(true);
    auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(Game::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);

    // create left car obstacle
    carObstacleRight = Sprite::createWithSpriteFrameName("red-car.png");
    carObstacleRightSpeed = 1500;
    carObstacleRight->setPosition(0 - (carObstacleRight->getBoundingBox().size.width / 2), this->getBoundingBox().getMaxY()/3 + this->getBoundingBox().getMaxY()/12);

    auto carObRBody = PhysicsBody::createBox(carObstacleRight->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    carObRBody->setDynamic(false);
    carObRBody->setContactTestBitmask(0xFFFFFFFF);
    carObstacleRight->setPhysicsBody(carObRBody);

    spriteGrid->addChild(carObstacleRight, 2);

    // create right car obstacle
    carObstacleLeft = Sprite::createWithSpriteFrameName("red-car.png");
    carObstacleLeft->setFlippedX(true);
    carObstacleLeftSpeed = 1500;
    carObstacleLeft->setPosition(0 - (carObstacleRight->getBoundingBox().size.width / 2), (this->getBoundingBox().getMaxY()/3)*2 - this->getBoundingBox().getMaxY()/11);

    auto carObLBody = PhysicsBody::createBox(carObstacleLeft->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    carObLBody->setDynamic(false);
    carObLBody->setContactTestBitmask(0xFFFFFFFF);
    carObstacleLeft->setPhysicsBody(carObLBody);

    spriteGrid->addChild(carObstacleLeft, 2);


    // load tree obstacles
    char str[100];
    for(int i = 0; i < 4; i++){
      // create Sprite
      sprintf(str, "trees/%02d.png", i+1);
      auto tree = Sprite::createWithSpriteFrameName(str);

      // put all trees beyond screen bounding box on left
      tree->setPosition(0 - (tree->getBoundingBox().size.width / 2), visibleSize.height/2);

      // create physics body for tree
      auto treeBody = PhysicsBody::createCircle(tree->getContentSize().width/2.15, PHYSICSBODY_MATERIAL_DEFAULT);
      treeBody->setDynamic(false);
      treeBody->setContactTestBitmask(0xFFFFFFFF);
      tree->setPhysicsBody(treeBody);

      // add tree to scene
      spriteGrid->addChild(tree, 3);

      // add tree to vectors
      treeObstacles.pushBack(tree);
    }

    // load rock obstacles
    for(int i = 0; i < 4; i++){
      // create Sprite
      sprintf(str, "rocks/%02d.png", i+1);
      auto rock = Sprite::createWithSpriteFrameName(str);

      // put all trees beyond screen bounding box on left
      rock->setPosition(0 - (rock->getBoundingBox().size.width / 2), visibleSize.height/2);

      // create physics body for tree
      auto rockBody = PhysicsBody::createCircle(rock->getContentSize().width/1.75, PHYSICSBODY_MATERIAL_DEFAULT);
      rockBody->setDynamic(false);
      rockBody->setContactTestBitmask(0xFFFFFFFF);
      rock->setPhysicsBody(rockBody);

      // add rock to scene
      spriteGrid->addChild(rock, 2);

      // add tree to vectors
      rockObstacles.pushBack(rock);
    }

    // add sprite grid to scene
    this->addChild(spriteGrid);

    // create contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // start timer
    this->schedule(schedule_selector(Game::updateTimer), 0.1f);

    this->scheduleUpdate();
    return true;
}

void Game::update(float delta)
{
  if(!isCrashed){
    if(timer > 1.0 && timer < 4.0){
      // scroll background images
      this->scrollBackground(delta);

    } else if(timer >= 4.0 && timer < 5.0){
      // scroll background images
      this->scrollBackground(delta);

      if(isPaused){
        // unpause game
        auto fadeOut = FadeOut::create(0.75);
        prelimInst->setOpacity(0);
        pauseOverlay->runAction(fadeOut);
        isPaused = false;
      }
    } else if(timer >= 5.0 && timer < 20.0){
      // scroll background images
      this->scrollBackground(delta);

      // update obstacles
      this->updateObstacles(delta);

    } else if(timer >= 20.0 && timer < 24.0) {
      // scroll background images
      this->scrollBackground(delta);

      // display instructions about driving drunk
      if(!isPaused){
        auto fadeIn = FadeTo::create(0.75, 0.75*255);
        drunkInst->setOpacity(255);
        pauseOverlay->runAction(fadeIn);
        this->resetObstacles();
        lives = 3;
        isPaused = true;
      }
    } else if(timer >= 24.0 && timer < 25.0){
      // scroll background images
      this->scrollBackground(delta);

      if(isPaused){
        // unpause game
        auto fadeOut = FadeOut::create(0.75);
        drunkInst->setOpacity(0);
        pauseOverlay->runAction(fadeOut);
        isPaused = false;
      }
    } else if (timer >= 25.0) {

      // scroll background images
      this->scrollBackground(delta);

      // update obstacles
      this->updateObstacles(delta);

      // become drunk
      if(!isDrunk){
        auto ripple = Ripple3D::create(10, Size(32,24), Vec2(windowSize.width/2, windowSize.height/2), windowSize.width*2, 50, 150.0);
        auto waves = Waves3D::create(10, Size(32,24), 30, 150);
        auto shake = Shaky3D::create(10, Size(32,24), 30, true);
        spriteGrid->runAction(RepeatForever::create((Sequence*)Sequence::create(ripple, waves, shake, NULL)));
        isDrunk = true;
      }
    }
  } else {
    if(!isPaused){
      auto fadeIn = FadeTo::create(0.75, 0.75*255);
      pauseOverlay->runAction(fadeIn);
      char str[100];
      sprintf(str, "¡Ah, chocaste tu carro! Durabas por %.1f segundos.", timer);
      crashInst->setString(str);
      crashInst->setOpacity(255);
      playAgainMenu->setOpacity(255);
      isPaused = true;
    }
  }
}

void Game::updateTimer(float dt)
{
  timer += 0.1;
}

void Game::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
  if(!isCrashed){
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
}

bool Game::onContactBegin(cocos2d::PhysicsContact &contact){
  PhysicsBody* a = contact.getShapeA()->getBody();
  PhysicsBody* b = contact.getShapeB()->getBody();

  // check to see if car is part of collision
  if(a->getCollisionBitmask() == playerColBitmask || b->getCollisionBitmask() == playerColBitmask){
    if(!isCrashed && !isPaused){
      if(lives <= 0){
        isCrashed = true;
      } else {
        lives--;
      }
    }
  }

  return false;
}

void Game::scrollBackground(float delta){
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

void Game::updateObstacles(float delta){
  // update car obstacles
  auto position = carObstacleRight->getPosition();
  position.x -=  carObstacleRightSpeed * delta;
  if (position.x < 0 - (carObstacleRight->getBoundingBox().size.width / 2)){
    carObstacleRightSpeed = cocos2d::RandomHelper::random_int(bgspeed - (bgspeed/5)*3, bgspeed - bgspeed/5);
    position.x = this->getBoundingBox().getMaxX() + carObstacleRight->getBoundingBox().size.width/2 + cocos2d::RandomHelper::random_int(0, static_cast<int>(this->getBoundingBox().getMaxX()));
  }
  carObstacleRight->setPosition(position);

  position = carObstacleLeft->getPosition();
  position.x -=  carObstacleLeftSpeed * delta;
  if (position.x < 0 - (carObstacleLeft->getBoundingBox().size.width / 2)){
    carObstacleLeftSpeed = cocos2d::RandomHelper::random_int(bgspeed, bgspeed + bgspeed/3);
    position.x = this->getBoundingBox().getMaxX() + carObstacleLeft->getBoundingBox().size.width/2 + cocos2d::RandomHelper::random_int(0, static_cast<int>(this->getBoundingBox().getMaxX()));
  }
  carObstacleLeft->setPosition(position);

  // update trees
  int i = 0;
  for(auto tree : treeObstacles){
    // get tree position
    position = tree->getPosition();
    position.x -=  bgspeed * delta;
    if (position.x < 0 - (tree->getBoundingBox().size.width / 2)){
      // initialize new position
      position.x = this->getBoundingBox().getMaxX() + (tree->getBoundingBox().size.width / 2) + cocos2d::RandomHelper::random_int(0, static_cast<int>(this->getBoundingBox().getMaxX()));
      if(i < 2){
        position.y = cocos2d::RandomHelper::random_int(static_cast<int>(this->getBoundingBox().getMaxY()/15), static_cast<int>(this->getBoundingBox().getMaxY()/3 - this->getBoundingBox().getMaxY()/12));
      } else {
        position.y = cocos2d::RandomHelper::random_int(static_cast<int>((this->getBoundingBox().getMaxY()/3)*2 + this->getBoundingBox().getMaxY()/12), static_cast<int>(this->getBoundingBox().getMaxY() - this->getBoundingBox().getMaxY()/15));
      }
    }
    tree->setPosition(position);
    i++;
  }

  // update rocks
  i = 0;
  for(auto rock : rockObstacles){
    // get rocks position
    position = rock->getPosition();
    position.x -=  bgspeed * delta;
    if (position.x < 0 - (rock->getBoundingBox().size.width / 2)){
      // initialize new position
      position.x = this->getBoundingBox().getMaxX() + (rock->getBoundingBox().size.width / 2) + cocos2d::RandomHelper::random_int(0, static_cast<int>(this->getBoundingBox().getMaxX()));
      if(i < 2){
        position.y = cocos2d::RandomHelper::random_int(static_cast<int>(this->getBoundingBox().getMaxY()/15), static_cast<int>(this->getBoundingBox().getMaxY()/3 - this->getBoundingBox().getMaxY()/12));
      } else {
        position.y = cocos2d::RandomHelper::random_int(static_cast<int>((this->getBoundingBox().getMaxY()/3)*2 + this->getBoundingBox().getMaxY()/12), static_cast<int>(this->getBoundingBox().getMaxY() - this->getBoundingBox().getMaxY()/15));
      }
    }
    rock->setPosition(position);
    i++;
  }
}

void Game::resetObstacles(){
  // reset sprites to outside of screen bounding box
  for(auto tree : treeObstacles){
    tree->setPosition(0 - (tree->getBoundingBox().size.width / 2), visibleSize.height/2);
  }
  for(auto rock : rockObstacles){
    rock->setPosition(0 - (rock->getBoundingBox().size.width / 2), visibleSize.height/2);
  }
  carObstacleRight->setPosition(0 - (carObstacleRight->getBoundingBox().size.width / 2), this->getBoundingBox().getMaxY()/3 + this->getBoundingBox().getMaxY()/12);
  carObstacleLeft->setPosition(0 - (carObstacleRight->getBoundingBox().size.width / 2), (this->getBoundingBox().getMaxY()/3)*2 - this->getBoundingBox().getMaxY()/11);
}

void Game::playAgain(cocos2d::Ref *pSender){
  Director::getInstance()->popScene();
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
