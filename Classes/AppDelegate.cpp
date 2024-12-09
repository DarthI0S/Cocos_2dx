#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
//#include "level_1"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(1000, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::stopAll();
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0; // flag for package manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview) {
        glview = GLViewImpl::create("Juego_lol");
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(mediumResolutionSize.width, 760, ResolutionPolicy::NO_BORDER);

    auto scene = MainMenu::createScene();
    if (!scene) {
        CCLOG("Error: La escena principal no pudo ser creada.");
        return false;
    }
    

    director->runWithScene(scene);
    return true;
}



void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
