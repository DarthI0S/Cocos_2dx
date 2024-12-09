#include "cocos2d.h"
#include <vector>
#include "ui\CocosGUI.h"
class mini_game : public cocos2d::Scene
{
private:
    std::string previousScene;
    cocos2d::Sprite* nave = nullptr;
    std::vector<cocos2d::Sprite*> bullets;
    std::vector<cocos2d::Sprite*> asteroids;
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;
    cocos2d::PhysicsWorld* sceneWorld;
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // a selector callback
    void generateBullet();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
    void spawn_enemy(float time);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuPlayBack(Ref* pSender);
    void YouLose();
    void bullet_asteroid();
    void setPreviousScene(const std::string& origin);


    // implement the "static create()" method manually
    CREATE_FUNC(mini_game);
};


