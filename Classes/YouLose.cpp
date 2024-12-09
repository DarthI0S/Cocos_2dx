#include "MainMenu.h"
#include "YouLose.h"
#include "space_mini.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* YouLose::createScene()
{
    return YouLose::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool YouLose::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //  fondo
    auto fondo = Sprite::create("YouLose.png");
    if (fondo == nullptr)
    {
        CCLOG("Error: No se pudo cargar la imagen 'main_menu.png'.");
    }
    else
    {
        fondo->setScale(
            visibleSize.width / fondo->getContentSize().width,
            visibleSize.height / fondo->getContentSize().height
        );
        fondo->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
        this->addChild(fondo, 0);
    }
    //auto colorLayer = LayerColor::create(Color4B(50, 50, 150, 255)); // Azul oscuro
    //this->addChild(colorLayer, -1);
    //botones

    auto exit = MenuItemImage::create("exit_.png","exit__.png", CC_CALLBACK_1(YouLose::menuCloseCallback, this));
    exit->setPosition(visibleSize.width/2 , visibleSize.height/5);
    exit->setScale(1.1f);
    auto menu = MenuItemImage::create("menu_button.png", "menu_button_2.png", CC_CALLBACK_1(YouLose::menuPlayBack, this));
    menu->setPosition(visibleSize.width/2, visibleSize.height/3 );
    menu->setScale(1.1f);
    

    auto menu_ = Menu::create(menu,exit, NULL);
    menu_->setPosition(Vec2::ZERO);
    this->addChild(menu_, 1);




    // sonido
    auto audioID = AudioEngine::play2d("audio/you_lose.mp3");
    if (audioID == AudioEngine::INVALID_AUDIO_ID) {
        CCLOG("Error: No se pudo cargar el archivo de audio 'audio/you_lose.mp3'.");
    }

    return true;
}

void YouLose::menuCloseCallback(Ref* pSender)
{
    
    Director::getInstance()->end();
}

void YouLose::menuPlayBack(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(TransitionFade::create(0.2, MainMenu::create()));
    //Director::getInstance()->replaceScene(TransitionFade::create(0.2, mini_game::create()));
    //AudioEngine::play2d("audio/item/item.mp3");
}
