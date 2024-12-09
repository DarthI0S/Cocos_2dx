#include "space_mini.h"
#include "Mapa_1.h"
#include "Mapa_2.h"
#include "mini_game_pt2.h"
#include "MainMenu.h"
#include "YouLose.h"
#include "audio/include/AudioEngine.h"
#include <vector>
#include <random>
#include <time.h>
#include <Windows.h>
#include "Mapa_3.h"
USING_NS_CC;

using namespace ui;

Scene* mini_game_p2::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = mini_game_p2::create();



    scene->addChild(layer);
    return scene;
}



static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool mini_game_p2::init()
{
    if (!Scene::initWithPhysics()) { return false; }


    srand(unsigned(time(NULL)));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    nave = Sprite::create("navecita.png");

    //  fondo
    auto fondo = Sprite::create("fondo_mini.png");
    if (fondo == nullptr)
    {
        CCLOG("Error: No se pudo cargar la imagen 'fondo_mini.png'.");
    }
    else
    {
        fondo->setScale(
            visibleSize.width / fondo->getContentSize().width,
            visibleSize.height / fondo->getContentSize().height
        );
        fondo->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
        this->addChild(fondo, -1);
    }




    auto bordes = PhysicsBody::createEdgeBox(Size(visibleSize.width, visibleSize.height), PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(bordes);
    this->addChild(edgeNode);
    //Spaceship
    nave->setPosition(20, visibleSize.height / 2);
    auto col_nave = cocos2d::PhysicsBody::createBox(nave->getContentSize(), cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f)); // Material físico
    col_nave->setDynamic(true);          // Permitir movimiento
    col_nave->setGravityEnable(false);   // Desactivar gravedad
    col_nave->setContactTestBitmask(0xFFFFFFFF); // Permitir colisiones
    nave->setPhysicsBody(col_nave);
    this->addChild(nave, 1);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(mini_game_p2::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(mini_game_p2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->schedule(CC_SCHEDULE_SELECTOR(mini_game_p2::spawn_enemy), 1.0f); // Generar asteroides cada 1 segundo



    // sonido
    auto audioID = AudioEngine::play2d("audio/minigame.mp3");
    if (audioID == AudioEngine::INVALID_AUDIO_ID) {
        CCLOG("Error: No se pudo cargar el archivo de audio 'audio/dead_space.mp3'.");
    }

    this->schedule([this](float delta) {
        static float remainingTime = 20.0f; // Declarar la variable como estática o manejarla fuera del lambda

        remainingTime -= delta; // Restar el tiempo transcurrido
        if (remainingTime <= 0) {
            remainingTime = 0;
            this->unschedule("GameTimer"); // Detener el temporizador usando el nombre
            CC_CALLBACK_1(mini_game_p2::menuPlayBack, this); // Callback. // Llamar a la función de fin de juego
        }

        // Actualizar etiqueta con el tiempo restante
        CCLOG("Tiempo restante: %.1f segundos", remainingTime); // Mensaje de depuración
        }, 1.0f, "GameTimer");


    return true;
}

void mini_game_p2::spawn_enemy(float time) {
    auto screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Crear el sprite del asteroide
    auto asteroid = Sprite::create("asteroid.png");
    asteroids.push_back(asteroid);
    asteroid->setTag(10);

    // Generar posición aleatoria dentro del rango visible
    int randomX = origin.x + static_cast<int>(screenSize.width)+asteroid->getContentSize().width;
    int randomY = origin.y + rand() % static_cast<int>(screenSize.height); // Justo encima de la pantalla

    CCLOG("Asteroide generado en posición: (%d, %d)", randomX, randomY);
    //asteroid->setPosition(screenSize.width/2, screenSize.height/2);
    asteroid->setPosition(randomX, randomY);

    // Crear un cuerpo físico para el asteroide
    auto col_asteroid = PhysicsBody::createBox(asteroid->getContentSize(), PhysicsMaterial(0.0f, 0.8f, 0.0f));
    col_asteroid->setDynamic(false);          // Permitir movimiento
    col_asteroid->setGravityEnable(false);   // Desactivar gravedad
    col_asteroid->setContactTestBitmask(0xFFFFFFFF); // Permitir colisiones
    asteroid->setPhysicsBody(col_asteroid);
    this->addChild(asteroid, 1);

    // movement
    auto moveAction = MoveTo::create(1.2f, Vec2(origin.x - asteroid->getContentSize().width, randomY));
    auto removeAction = RemoveSelf::create(); // Eliminar el asteroide al salir de pantalla
    asteroid->runAction(Sequence::create(moveAction, removeAction, nullptr));

    // Crear y registrar el listener de colisión
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this, asteroid](PhysicsContact& contact) -> bool {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        // Verificar si la colisión es entre la nave y este asteroide
        if ((bodyA->getNode() == nave && bodyB->getNode() == asteroid) ||
            (bodyB->getNode() == nave && bodyA->getNode() == asteroid)) {
            // Eliminar la nave
            nave->runAction(RemoveSelf::create());
            CCLOG("Colisión detectada con el asteroide");

            // Llamar a la función de pérdida
            this->YouLose();
        }

        return true; // Retornar true para continuar con la detección de colisiones
        };


    // Asociar el listener al sistema de eventos de la escena
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, asteroid);
}

void mini_game_p2::generateBullet() {
    CCLOG("Generando una bala...");
    auto bullet = cocos2d::Sprite::create("bullet_spaces.png"); // Reemplaza con tu imagen de bala
    if (!bullet) return;



    // Posicionar la bala en el centro de la nave
    bullet->setPosition(nave->getPosition());
    auto pistol = AudioEngine::play2d("audio/space-gun.mp3");
    
    bullet->setTag(20);

    auto bulletBody = cocos2d::PhysicsBody::createBox(bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f)); // Cuerpo circular
    bulletBody->setDynamic(true); // Es dinámico para permitir colisiones
    bulletBody->setGravityEnable(false); // Desactivar la gravedad para que no caiga
    bulletBody->setCategoryBitmask(0x01); // Categoría de la bala
    bulletBody->setCollisionBitmask(0x02); // Colisiona con enemigos (por ejemplo)
    bulletBody->setContactTestBitmask(0x02); // Detecta colisiones con enemigos

    // Asignar el cuerpo físico al sprite de la bala
    bullet->setPhysicsBody(bulletBody);
    // Añadir la bala a la escena
    this->addChild(bullet, 1);

    //movimiento
    auto movimiento = RepeatForever::create(MoveBy::create(1.0f, Vec2(-40, 0)));

    // Añadir la bala al vector de balas
    bullets.push_back(bullet);

    // mover la bala
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto moveAction = MoveBy::create(1.0f, Vec2(screenSize.width, 0)); // Mover hacia arriba

    // Crear una acción para remover la bala
    auto removeAction = CallFunc::create([this, bullet]() {
        bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());//se elimina
        bullet->removeFromParentAndCleanup(true); // Remover de la escena
        });

    // Ejecutar las acciones en secuencia
    bullet->runAction(Sequence::create(moveAction, removeAction, nullptr));
    bullet_asteroid();
}

void mini_game_p2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    CCLOG("Tecla presionada: %d", keyCode);
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:

        if (!movingLeft) { // Evitar reiniciar la acción si ya se está moviendo
            movingLeft = true;
            auto moveLeft = RepeatForever::create(
                MoveBy::create(0.1f, Vec2(-20, 0)) // Movimiento continuo
            );
            moveLeft->setTag(1); // Asignar un tag para identificar la acción

            nave->runAction(moveLeft);
            // Asignar un tag para identificar la acción

        }
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (!movingRight) { // Evitar reiniciar la acción si ya se está moviendo
            movingRight = true;
            auto moveRight = RepeatForever::create(
                MoveBy::create(0.1f, Vec2(20, 0)) // Movimiento continuo
            );

            nave->runAction(moveRight);


        }
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        if (!movingDown) { // Evitar reiniciar la acción si ya se está moviendo
            movingDown = true;
            auto moveDown = RepeatForever::create(
                MoveBy::create(0.1f, Vec2(0, -20)) // Movimiento continuo
            );
            moveDown->setTag(3); // Asignar un tag para identificar la acción

            nave->runAction(moveDown);


        }
        break;

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        if (!movingUp) { // Evitar reiniciar la acción si ya se está moviendo
            movingUp = true;
            auto moveUp = RepeatForever::create(
                MoveBy::create(0.1f, Vec2(0, 20)) // Movimiento continuo
            );
            moveUp->setTag(4); // Asignar un tag para identificar la acción

            nave->runAction(moveUp);
        }
        break;

    case EventKeyboard::KeyCode::KEY_SPACE:
        generateBullet();


    }
}

void mini_game_p2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    CCLOG("Tecla liberada: %d", keyCode);

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        if (movingLeft) {
            movingLeft = false;
            nave->stopActionByTag(1); // Detener la acción con el tag 1

        }
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (movingRight) {
            movingRight = false;
            nave->stopActionByTag(2); // Detener la acción con el tag 2
        }
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        if (movingDown) {
            movingDown = false;
            nave->stopActionByTag(3); // Detener la acción con el tag 2
        }
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        if (movingUp) {
            movingUp = false;
            nave->stopActionByTag(4); // Detener la acción con el tag 2
        }
        break;

    default:
        break;
    }
}

void mini_game_p2::bullet_asteroid() {
    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = [this](PhysicsContact& contact) -> bool {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        auto nodeA = bodyA->getNode();
        auto nodeB = bodyB->getNode();

        if (!nodeA || !nodeB) return false;

        // colisiones entre nave y asteroide
        if ((nodeA == nave && nodeB->getTag() == 10) ||
            (nodeB == nave && nodeA->getTag() == 10)) {
            CCLOG("Colisión: Nave-Asteroide");
            nave->runAction(RemoveSelf::create());
            this->YouLose();
            return true;
        }

        // colisiones entre bala y asteroide
        if ((nodeA->getTag() == 20 && nodeB->getTag() == 10) ||
            (nodeB->getTag() == 20 && nodeA->getTag() == 10)) {
            CCLOG("Colisión: Bala-Asteroide");

            // Eliminar ambos objetos
            if (nodeA->getTag() == 20 || nodeA->getTag() == 10)
                nodeA->runAction(RemoveSelf::create());
            if (nodeB->getTag() == 20 || nodeB->getTag() == 10)
                nodeB->runAction(RemoveSelf::create());
        }

        return true;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}


void mini_game_p2::menuCloseCallback(Ref* pSender)
{

    Director::getInstance()->end();
}
void mini_game_p2::menuPlayBack(Ref* pSender)
{
    AudioEngine::stopAll();


        Director::getInstance()->replaceScene(TransitionFade::create(0.2, Mapa_3::createScene()));
}

void mini_game_p2::YouLose() {
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(TransitionFade::create(0.1, YouLose::create()));

}