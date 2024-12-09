/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "AudioEngine.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    ////el enemigo se va a la ultima posicion del jugador

    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    Director::getInstance()->getOpenGLView()->setFrameSize(1020, 700);
    auto arcane = AudioEngine::play2d("audio/apt.mp3");
    //auto audio = AudioEngine::getDuration(30);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto dinosaur = Sprite::create("Dino1.png");

    auto sprite = Sprite::create("CloseNormal.png");

    auto newSprite = Sprite::create("CloseNormal.png");

    //auto mySprite = Sprite::create("CloseNormal.png");//se crea un sprite con el sprite que se descarga
    sprite->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
    //auto accion = ScaleBy::create(3, 3, 0.25);
    //
    //sprite->runAction(accion);
    this->addChild(sprite);

    //auto action = RotateBy::create(3, 90);
    //sprite->runAction(action);

    //auto new_action = Repeat::create(RotateTo::create(5, 90), 5);//it repeats the action 

    //auto new_action = RepeatForever::create(RotateBy::create(5, 90));
    //the seqeuence can be made by n actions

    auto new_action = Sequence::create(Repeat::create(RotateBy::create(1, 90), 2), MoveBy::create(1, Point(100, -90)), MoveTo::create(1, Point(120, 20)),
        JumpTo::create(10.2, Point(100, 90), 100, 20),
        JumpBy::create(3,
            Point(150, 100), 20, 3), CallFunc::create([=]() {
                AudioEngine::stop(arcane);
                }),
        NULL);//hace una serie de acciones con una sola accion, y termina cuando se llama al null porque ahí no hay nada 

    sprite->runAction(new_action);
    //if (new_action == NULL) {
    //    AudioEngine::stop(arcane);
    //}
    //
    /*en resources se ponen todos los objetos y texturas
    dinosaur->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
    dinosaur->setScale(4);
    this->addChild(dinosaur);

    mySprite->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
    this->addChild(mySprite);//se agrega el sprite a la escena, el this es la escena
    auto action = MoveBy::create(2, Point(100, 0));//se mueve hasta el punto con una velocidad determinada
    mySprite->runAction(action);

    action = MoveTo::create(2, Point(100, 0));
    mySprite->runAction(action);

    auto action2 = JumpBy::create(3, Point(0, 100), 20, 3);
    mySprite->runAction(action2);

    action2 = JumpTo::create(3, Point(0, 100), 20, 3);
    mySprite->runAction(action2);
    sprite->setAnchorPoint(Vec2(0.3, 0.5));
    this->addChild(sprite);
    */


    //al tener un solo sprite se hace la ultima accion nada más                                                                               

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();//el director debe de estar acompañado de get_instance   
    AudioEngine::play2d("audio/apt.mp3");
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}
