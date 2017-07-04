#include "PrelimInstScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* PrelimInst::createScene()
{
  auto scene = Scene::create();
  auto layer = PrelimInst::create();
  scene->addChild(layer);
  return scene;
}

// on "init" you need to initialize your instance
bool PrelimInst::init()
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

    // create instructions and pause screen
    prelimInst = Label::createWithTTF("¡Evitá los obstaculos por 15 segundos!", "fonts/Marker Felt.ttf", 72);
    prelimInst->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(prelimInst, 4);

    pauseOverlay = Sprite::createWithSpriteFrameName("pause-overlay.png");
    float pauseScale = MAX(visibleSize.width / pauseOverlay->getContentSize().width, visibleSize.height / pauseOverlay->getContentSize().height);
    pauseOverlay->setScale(pauseScale);
    pauseOverlay->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(pauseOverlay, 3);

    // schedule popping of scene in 3 seconds
    this->scheduleOnce(schedule_selector(PrelimInst::startGame), 3.0f);

    this->scheduleUpdate();
    return true;
}

void PrelimInst::update(float delta)
{
  // increment on-screen timer
}

void PrelimInst::startGame(float dt){
  // start game scene
  auto gameScene = Game::createScene();
  Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, gameScene));
}
