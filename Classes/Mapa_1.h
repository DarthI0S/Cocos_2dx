#pragma once

#include <vector>
#include <string>
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "Sofy.h"


#define FILAS 15
#define COLUMNAS 16
#define UP_ARROW_KEY 0
#define DOWN_ARROW_KEY 1
#define LEFT_ARROW_KEY 2
#define RIGHT_ARROW_KEY 3
#define Q_KEY 4
#define E_KEY 5
class Mapa_1 : public cocos2d::Scene
{
private:
    int enemy_health = 7;
    
    bool isMoving = false;
    bool key_states[6];
    bool paused = false;
    bool movingLeft = false;
    bool movingRight = false;
    bool jumping = false;
    cocos2d::Sprite* enemy; 

    std::vector<cocos2d::Sprite*> enemy_bullets;
    std::vector<cocos2d::Sprite*> bullets;
    bool is_onGround=false;
    cocos2d::PhysicsWorld* sceneWorld;
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    Sofy* player;

    bool facingRight = true;

    //cocos2d::PhysicsWorld* sceneWorld;
    //void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };




public:
    static cocos2d::Scene* createScene();
       
    virtual bool init();

    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    //void handle_paused();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
    void spawn_bullet_enemy(float delta);
    void shooting();
    void createHealthBar();
    void bullet_enemy();
    void update(float delta);
    void YouLose();
    void mapa_2();


    //void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event*);
    //    
    // implement the "static create()" method manually
    CREATE_FUNC(Mapa_1);
};     
       
       