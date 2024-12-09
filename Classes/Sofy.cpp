#include "Sofy.h"
#include <vector>
USING_NS_CC;


void Sofy::moveLeft() {

    // Configurar frames
    Vector<SpriteFrame*> walkingL;
    walkingL.pushBack(SpriteFrame::create("Animation_1L.png", Rect(0, 0, 75, 95)));
    walkingL.pushBack(SpriteFrame::create("Animation_2L.png", Rect(0, 0, 75, 95)));
    walkingL.pushBack(SpriteFrame::create("Animation_3L.png", Rect(0, 0, 75, 95)));
    walkingL.pushBack(SpriteFrame::create("Animation_4L.png", Rect(0, 0, 75, 95)));

    // Crear animación
    auto animation = Animation::createWithSpriteFrames(walkingL, 0.3f);
    auto animate = Animate::create(animation);
    auto run_action = RepeatForever::create(animate);
    run_action->setTag(3); // Asignar una etiqueta a la animación

    // Crear movimiento
    auto moveLeft = RepeatForever::create(
        MoveBy::create(0.5f, Vec2(-200, 0))
    );
    moveLeft->setTag(1); // Asignar una etiqueta al movimiento

    // Ejecutar las acciones
    //this->runAction(run_action);
    this->runAction(moveLeft);
}

void Sofy::moveRight() {

    // Configurar frames
    Vector<SpriteFrame*> walkingR;
    walkingR.pushBack(SpriteFrame::create("Animation_1R.png", Rect(0, 0, 75, 95)));
    walkingR.pushBack(SpriteFrame::create("Animation_2R.png", Rect(0, 0, 75, 95)));
    walkingR.pushBack(SpriteFrame::create("Animation_3R.png", Rect(0, 0, 75, 95)));
    //walkingR.pushBack(SpriteFrame::create("Animation_4R.png", Rect(0, 0, 75, 95))); // Tamaño ajustado

    // Crear animación
    auto animation = Animation::createWithSpriteFrames(walkingR, 0.3f);
    auto animate = Animate::create(animation);
    auto run_actions = RepeatForever::create(animate);

    // Combinar movimiento
    auto moveRight = RepeatForever::create(
        MoveBy::create(0.5f, Vec2(200, 0))
    );
    moveRight->setTag(2); // Asignar una etiqueta al movimiento
    //this->runAction(run_actions);
    this->runAction(moveRight);
}


void Sofy::jumpL() {
    SpriteFrameCache* cache1 = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> jumpingL(4); // parameter = number of frames you have for this anim

    jumpingL.pushBack(SpriteFrame::create("astronaut_jump1L.png", Rect(0, 0, 75, 95)));
    jumpingL.pushBack(SpriteFrame::create("astronaut_jump2L.png", Rect(0, 0, 75, 95)));
    jumpingL.pushBack(SpriteFrame::create("astronaut_jump3L.png", Rect(0, 0, 75, 95)));
    jumpingL.pushBack(SpriteFrame::create("astronaut_jump4L.png", Rect(0, 0, 75, 95)));
    auto animation = Animation::createWithSpriteFrames(jumpingL, 0.5f); // 0.1 segundos por frame
    auto animate = Animate::create(animation);
    auto run_action = animate;
   

    //this->runAction(run_action);
    auto salto = MoveBy::create(0.1f, Vec2(0, 300));
    this->runAction(salto);
}

void Sofy::jumpR() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> jumpingR(4); // parameter = number of frames you have for this anim

    jumpingR.pushBack(SpriteFrame::create("astronaut_jump1R.png", Rect(0, 0, 75, 95)));
    jumpingR.pushBack(SpriteFrame::create("astronaut_jump2R.png", Rect(0, 0, 75, 95)));
    jumpingR.pushBack(SpriteFrame::create("astronaut_jump3R.png", Rect(0, 0, 75, 95)));
    jumpingR.pushBack(SpriteFrame::create("astronaut_jump4R.png", Rect(0, 0, 75, 95)));
    auto animation = Animation::createWithSpriteFrames(jumpingR, 0.5f); // 0.1 segundos por frame
    auto animate = Animate::create(animation);
    auto run_action = animate;
    

    this->runAction(run_action);
    auto salto = MoveBy::create(0.1f, Vec2(0, 40));
    this->runAction(salto);

}

void Sofy::startMove(Vec2 direction) {
    if (!isMoving) {
        isMoving = true;
        auto currentAction = RepeatForever::create(MoveBy::create(0.1f, direction));
        
        this->runAction(currentAction);
    }
}

void Sofy::stop_animations() {
    this->stopAllActions();
}


