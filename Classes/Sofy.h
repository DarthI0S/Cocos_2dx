#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

class Sofy : public cocos2d::Sprite {
private:
    // Atributos privados
    bool isMoving = false;
    bool jumping = false;
    int _health = 5;    // Vida del jugador
    float _speed = 200.0;   // Velocidad de movimiento
    std::vector<cocos2d::Sprite*> bullets;
    int maxHealth = 100;   // Vida máxima 
    int currentHealth = 100; // Vida actual 


public:
    // Crear un jugador (con sprite)
    cocos2d::ui::LoadingBar* healthBar = nullptr; // Puntero a la barra de vida
    static Sofy* createPlayer() {
        auto player = new (std::nothrow) Sofy();
        if (player && player->initWithFile("astronautR.png")) {
            player->autorelease();
            player->initPhysicsBody();

            // Inicializar la barra de vida
            player->healthBar = cocos2d::ui::LoadingBar::create("health_bar.png");
            player->healthBar->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);
            player->healthBar->setPercent(100); // Vida inicial al 100%

            return player;
        }
        CC_SAFE_DELETE(player);
        return nullptr;
    }
    void updateHealthBar(float percentage) {
        if (healthBar) {
            healthBar->setPercent(percentage);
        }
    }
    void setHealth(int health) {
        _health = std::max(0, std::min(health, maxHealth)); // Asegúrate de que la salud esté entre 0 y maxHealth
        if (healthBar) {
            healthBar->setPercent((static_cast<float>(_health) / maxHealth) * 100.0f); // Actualizar la barra de vida
        }
    }



    // Inicializar cuerpo físico
    void initPhysicsBody() {
        auto body = cocos2d::PhysicsBody::createBox(this->getContentSize(),cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f)); // Material físico
        // 1.0f = densidad/masa, 0.0f = fricción, 0.0f = restitución (sin rebote)
        body->setDynamic(true);
        body->setContactTestBitmask(true);
        body->setRotationEnable(false);
        this->setPhysicsBody(body);
    }

    // Métodos para manipular atributos
    int getHealth() const { return _health; }
    //void setHealth(int health) { _health = health; }

    float getSpeed() const { return _speed; }
    void setSpeed(float speed) { _speed = speed; }

    // Métodos básicos de movimiento

    void startMove(cocos2d::Vec2 direction);
    //void stopMove();
    void moveRight();
    void moveLeft();
    void jumpL();
    void jumpR();
    void updateHealth(float newHealth) {
        healthBar->setPercent(newHealth);
    }
    void stop_animations();



};
