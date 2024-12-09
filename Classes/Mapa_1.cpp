#include <vector>
#include <string>
#include "MainMenu.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "Mapa_1.h"
#include "Sofy.h"
#include "Mapa_2.h"
#include "YouLose.h"
#include "space_mini.h"


const float MAP_WIDTH = 3000.0f;
const float MAP_HEIGHT = 2000.0f;
USING_NS_CC;

Scene* Mapa_1::createScene() {
    auto scene = Scene::createWithPhysics(); // Habilitar físicas
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    //scene->getPhysicsWorld()->setGravity(Vec2(0, -980)); // Configurar la gravedad (opcional)
    auto layer = Mapa_1::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer); 
    


    return scene;
}
// Print useful error message instead of segfaulting when files are not there.


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Mapa_1::init()
{
    Size real_S(2048, 780);
    if (!Scene::initWithPhysics()) { return false; }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Crear al jugador
    player = Sofy::createPlayer();
    player->setPosition(Vec2(10, visibleSize.height / 2));
    player->healthBar->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - 50));
    this->addChild(player->healthBar);
    this->scheduleUpdate();

    enemy = cocos2d::Sprite::create("canion_1.png"); // Imagen del enemigo
    enemy->setPosition(Vec2(1900, 100));
    CCLOG("El enemigo ha sido creado.");

    // Configurar cuerpo físico del enemigo
    auto enemyBody = cocos2d::PhysicsBody::createBox(enemy->getContentSize(), cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
    enemyBody->setDynamic(false); // El enemigo puede moverse
    enemyBody->setCategoryBitmask(0x02);
    enemyBody->setCollisionBitmask(0x01); // Puede colisionar con balas y jugador
    enemyBody->setContactTestBitmask(0x01); // Detectar contactos con balas
    enemy->setPhysicsBody(enemyBody);
    this->addChild(enemy);
    enemy->setTag(15);


    this->schedule(CC_SCHEDULE_SELECTOR(Mapa_1::spawn_bullet_enemy), 1.0f); // Generar asteroides cada 1 segundo

    auto bordes = PhysicsBody::createEdgeBox(Size(MAP_WIDTH, MAP_HEIGHT), PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(MAP_WIDTH / 2, MAP_HEIGHT / 2));
    edgeNode->setPhysicsBody(bordes);
    this->addChild(edgeNode);

    CCLOG("Tamaño de la pantalla: %f x %f", visibleSize.width, visibleSize.height);
    CCLOG("Origen: (%f, %f)", origin.x, origin.y);

    auto fondo = Sprite::create("fondo_1.png");

    fondo->setAnchorPoint(Vec2(0, 0));  // Asegúrate de que el fondo se ajuste desde la esquina inferior izquierda
    fondo->setPosition(origin.x, origin.y);
    //piso
    auto piso = Sprite::create("piso_2.png");
    piso->setPosition(origin.x, origin.y);
    //colision_piso
    auto col_piso = cocos2d::PhysicsBody::createBox(piso->getContentSize(), cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f)); // Material físico
    col_piso->setDynamic(false);
    col_piso->setContactTestBitmask(true);
    piso->setPhysicsBody(col_piso);



    // Crear una acción de seguimiento para la cámara
    auto cameraFollow = Follow::create(player, Rect(0, 0, 2040, visibleSize.height)); // camara
    this->runAction(cameraFollow);

    auto audio_level = AudioEngine::play2d("audio/level_1.mp3");



    this->addChild(fondo, -1);
    this->addChild(player, 1);
    this->addChild(piso, 1);
    Vec2 player_pos = player->getPosition();
    auto scene = this->getScene();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Mapa_1::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Mapa_1::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);             

    auto contactListener = EventListenerPhysicsContact::create();
    auto enemy_contactlis = EventListenerPhysicsContact::create();

    // Asignar el callback para detectar colisiones (tipo funcion)
    contactListener->onContactBegin = [this, piso](PhysicsContact& contact) -> bool {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        if ((bodyA->getNode() == player && bodyB->getNode() == piso) ||
            (bodyB->getNode() == player && bodyA->getNode() == piso)) {
            is_onGround = true; // Marcar que el jugador está en el suelo
            CCLOG("Jugador está en el suelo");
        }

        return true; // Retornar true para continuar con la física
        };



    // Registrar el listener en el dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


    return true;    
}

void Mapa_1::update(float delta) {
    // Asegúrate de que player y healthBar están definidos y accesibles.
    if (player && player->healthBar) {
        // Ajusta la posición de la barra relativa a la posición del jugador.
        Vec2 healthBarOffset(0, 50); // Ajusta el desplazamiento según necesites.
        player->healthBar->setPosition(player->getPosition() + healthBarOffset);
    }
}

//CAMBIAR POR EL SPRITE_SHOOTING
void Mapa_1::shooting() {
    CCLOG("Generando una bala...");
    auto bullet = cocos2d::Sprite::create("bullet_spaces.png"); // Reemplaza con tu imagen de bala
    if (!bullet) return;

    // Posicionar la bala en el centro de la nave
    bullet->setPosition(player->getPosition());
    AudioEngine::play2d("audio/space-gun.mp3");

    // Configuración del cuerpo físico de la bala
    auto bulletBody = cocos2d::PhysicsBody::createBox(bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
    bulletBody->setDynamic(true);
    bulletBody->setGravityEnable(false);
    bulletBody->setCategoryBitmask(0x01);
    bulletBody->setCollisionBitmask(0x02);
    bulletBody->setContactTestBitmask(0x02);

    bullet->setPhysicsBody(bulletBody);
    bullet->setTag(20);
    this->addChild(bullet, 1);

    // Cambiar el sprite del jugador según la dirección
    //if (facingRight) {
    //    player->setTexture("astronaut_shooting_R.png");
    //}
    //else {
    //    player->setTexture("astronaut_shooting_L.png");
    //}

    // Restaurar sprite del jugador tras disparar
    auto restoreSprite = CallFunc::create([this]() {
        if (facingRight) {
            CCLOG("Cambiando a textura: astronaut_shooting_R.png");
            player->setTexture("astronaut_shooting_R.png");
        }
        else {
            CCLOG("Cambiando a textura: astronaut_shooting_L.png");
            player->setTexture("astronaut_shooting_L.png");
        }
        });

    // Añadir la bala al vector
    bullets.push_back(bullet);

    // Movimiento de la bala
    auto screenSize = Director::getInstance()->getVisibleSize();
    Vec2 direction = facingRight ? Vec2(screenSize.width, 0) : Vec2(-screenSize.width, 0);
    auto moveAction = MoveBy::create(1.0f, direction);

    // Remover la bala después de moverse
    auto removeAction = CallFunc::create([this, bullet]() {
        bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
        bullet->removeFromParentAndCleanup(true);
        });

    bullet->runAction(Sequence::create(moveAction, removeAction, nullptr));
    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = [this](PhysicsContact& contact) -> bool {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        // Obtener los nodos asociados a los cuerpos físicos
        auto nodeA = bodyA ? bodyA->getNode() : nullptr;
        auto nodeB = bodyB ? bodyB->getNode() : nullptr;

        // Verificar que ambos nodos son válidos
        if (!nodeA || !nodeB) {
            return false; // Salir si alguno es nullptr
        }

        // Verificar si la colisión es entre una bala (tag 20) y un enemigo (tag 15)
        if ((nodeA->getTag() == 20 && nodeB->getTag() == 15) ||
            (nodeB->getTag() == 20 && nodeA->getTag() == 15)) {
            CCLOG("Colisión detectada: Bala-enemigo");

            // Reducir la vida del enemigo
            enemy_health--;

            // Remover los nodos involucrados (de manera segura)
            auto bulletNode = (nodeA->getTag() == 20) ? nodeA : nodeB;
            auto enemyNode = (nodeA->getTag() == 15) ? nodeA : nodeB;

            if (bulletNode && bulletNode->getParent() != nullptr) {
                bulletNode->removeFromParentAndCleanup(true);
            }
            if (enemy_health <= 0 && enemyNode && enemyNode->getParent() != nullptr) {
                enemyNode->removeFromParentAndCleanup(true);
                enemy = nullptr;
                this->mapa_2();

            }

            return true; // Finalizar el contacto
        }

        return false; // No hay colisión relevante
        };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, bullet);
}
      
void Mapa_1::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    CCLOG("Tecla presionada: %d", keyCode);

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        
        if (movingLeft==false) { // Evitar reiniciar la acción si ya se está moviendo
            movingLeft = true;

             // Asignar un tag para identificar la acción
            player->moveLeft();

        }
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (movingRight==false) { // Evitar reiniciar la acción si ya se está moviendo
            movingRight = true;


            player->moveRight();

        }
        break;

    case EventKeyboard::KeyCode::KEY_Z:
        CCLOG("Saltando");
        if (is_onGround == true) {
            player->jumpL();
            is_onGround = false;
        }

        break;
    case EventKeyboard::KeyCode::KEY_X:
        shooting();
        
        break;


    default:
        break;
    }
}

void Mapa_1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    CCLOG("Tecla liberada: %d", keyCode);

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        if (movingLeft) {
            movingLeft = false;    
            facingRight = false;
            player->stop_animations();
            player->stopActionByTag(1); // Detener la acción con el tag 1
            player->setTexture("astronautL.png");
            //player=Sprite::create("astronautL.png");
        }
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (movingRight) {
            movingRight = false;
            facingRight = true;
            player->stopActionByTag(2); // Detener la acción con el tag 2
            player->stop_animations();
            player->setTexture("astronautR.png");
        }
        break;

    default:
        break;
    }
}
void Mapa_1::spawn_bullet_enemy(float delta) {
    auto enemy_bullet = cocos2d::Sprite::create("bullet_enemy.png");
    if (!enemy) {
        CCLOG("Error: El enemigo no está inicializado.");
        return;
    }
    CCLOG("BALA GENERADA");
    if (!enemy_bullet) {
        CCLOG("Error: No se pudo cargar la imagen de la bala enemiga.");
        return;
    }


    // Posicionar la bala en el enemigo
    enemy_bullet->setPosition(Vec2(enemy->getPosition().x - enemy->getContentSize().width / 2, enemy->getPosition().y + 1));

    AudioEngine::play2d("audio/enemy_shoot.mp3");
    enemy_bullet->setTag(15);

    // Configurar cuerpo físico
    auto bulletBody = cocos2d::PhysicsBody::createBox(enemy_bullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
    bulletBody->setDynamic(true);
    bulletBody->setGravityEnable(false);
    bulletBody->setCategoryBitmask(0x01);
    bulletBody->setCollisionBitmask(0x02);
    bulletBody->setContactTestBitmask(0x02);

    enemy_bullet->setPhysicsBody(bulletBody);
    this->addChild(enemy_bullet, 1);


    // Agregar la bala al vector
    enemy_bullets.push_back(enemy_bullet);

    // Acción de movimiento hacia la izquierda
    auto moveAction = MoveTo::create(2.0f, Vec2(0, enemy->getPosition().y + 1)); // Ajusta la distancia y tiempo según necesites

    // Acción para remover la bala al salir de la pantalla
    auto removeAction = CallFunc::create([this, enemy_bullet]() {
        enemy_bullets.erase(std::remove(enemy_bullets.begin(), enemy_bullets.end(), enemy_bullet), enemy_bullets.end());
        enemy_bullet->removeFromParentAndCleanup(true);
        });


    // Ejecutar acciones
    enemy_bullet->runAction(Sequence::create(moveAction, removeAction, nullptr));

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this, enemy_bullet](PhysicsContact& contact) -> bool {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        // Verificar si la colisión es entre la nave y este asteroide
        if ((bodyA->getNode() == player && bodyB->getNode() == enemy_bullet) ||
            (bodyB->getNode() == player && bodyA->getNode() == enemy_bullet)) {
            // bajar la vida
            player->setHealth(player->getHealth() - 1);
            int vida = player->getHealth();


            if (vida == 0) this->YouLose();
            // Actualizar la barra de salud

            float healthPercentage = (float)vida / 5 * 100;
            player->updateHealthBar(healthPercentage);


            CCLOG("Vida del jugador: %d", vida);
            auto oof = AudioEngine::play2d("audio/golpe.mp3");

            // Remover la bala enemiga de la escena
            auto bulletNode = (bodyA->getNode() == player) ? bodyB->getNode() : bodyA->getNode();//Si bodyA es el nodo del player, entonces bulletNode será bodyB
            bulletNode->removeFromParentAndCleanup(true);

            return true;//termina la col
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, enemy_bullet);

}
void Mapa_1::YouLose() {
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(TransitionFade::create(0.1, YouLose::create()));
}

void Mapa_1::mapa_2() {
    AudioEngine::stopAll();
    //Director::getInstance()->replaceScene(mini_game::createSceneWithPrevious("Mapa_1"));
    Director::getInstance()->replaceScene(TransitionFade::create(0.1, mini_game::create()));

}



