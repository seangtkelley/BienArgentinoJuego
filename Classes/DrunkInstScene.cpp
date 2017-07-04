#include "DrunkInstScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* DrunkInst::createScene()
{
  auto scene = Scene::create();
  auto layer = DrunkInst::create();
  scene->addChild(layer);
  return scene;
}

// on "init" you need to initialize your instance
bool DrunkInst::init()
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
    drunkInst = Label::createWithTTF("¿Ahora, podés evitar los obstaculos mientras estás alcoholizado?", "fonts/Marker Felt.ttf", 72);
    drunkInst->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(drunkInst, 4);

    pauseOverlay = Sprite::createWithSpriteFrameName("pause-overlay.png");
    float pauseScale = MAX(visibleSize.width / pauseOverlay->getContentSize().width, visibleSize.height / pauseOverlay->getContentSize().height);
    pauseOverlay->setScale(pauseScale);
    this->addChild(pauseOverlay, 3);

    // schedule popping of scene in 5 seconds
    this->scheduleOnce(schedule_selector(DrunkInst::popSelf), 5.0f);

    return true;
}

void DrunkInst::popSelf(float dt){
  // pop scene
  Director::getInstance()->popScene();
}
