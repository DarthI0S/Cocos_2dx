#include "MainMenu.h"
#include "Mapa_1.h"
#include "Mapa_2.h"
#include "space_mini.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MainMenu::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //  fondo
    auto fondo = Sprite::create("main_menu.png");
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

    auto exit = MenuItemImage::create("exit_.png","exit__.png", CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    exit->setPosition(visibleSize.width/2 , visibleSize.height/6);
    exit->setScale(1.1f);
    auto play = MenuItemImage::create("jugar.png", "jugar_s.png", CC_CALLBACK_1(MainMenu::menuPlayBack, this));
    play->setPosition(visibleSize.width/2, visibleSize.height/3 );
    play->setScale(1.1f);
    

    auto menu = Menu::create(play,exit, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);




    // sonido
    auto audioID = AudioEngine::play2d("audio/dead_space.mp3");
    if (audioID == AudioEngine::INVALID_AUDIO_ID) {
        CCLOG("Error: No se pudo cargar el archivo de audio 'audio/dead_space.mp3'.");
    }

    return true;
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    
    Director::getInstance()->end();
}

void MainMenu::menuPlayBack(Ref* pSender)
{
    AudioEngine::stopAll();
   // Director::getInstance()->replaceScene(TransitionFade::create(0.2, Mapa_1::create()));
   // Director::getInstance()->replaceScene(TransitionFade::create(0.2, mini_game::create()));
    Director::getInstance()->replaceScene(TransitionFade::create(0.2, Mapa_1::create()));
    //AudioEngine::play2d("audio/item/item.mp3");
}
