#include "MainMenu.h"
#include "YouLose.h"
#include "space_mini.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "thanks.h"

USING_NS_CC;
using namespace ui;

Scene* thanks::createScene()
{
    return thanks::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool thanks::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //  fondo
    auto fondo = Sprite::create("thanks.png");
    this->addChild(fondo);
    if (fondo == nullptr)
    {
        CCLOG("Error: No se pudo cargar la imagen 'main_menu.png'.");
    }

    // sonido
    auto audioID = AudioEngine::play2d("audio/THANKS.mp3");
    if (audioID == AudioEngine::INVALID_AUDIO_ID) {
        CCLOG("Error: No se pudo cargar el archivo de audio 'audio/you_lose.mp3'.");
    }

    return true;
}

void thanks::menuCloseCallback(Ref* pSender)
{
    
    Director::getInstance()->end();
}

void thanks::menuPlayBack(Ref* pSender)
{
    Director::getInstance()->end();
    //Director::getInstance()->replaceScene(TransitionFade::create(0.2, mini_game::create()));
    //AudioEngine::play2d("audio/item/item.mp3");
}
