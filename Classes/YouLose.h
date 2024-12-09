#ifndef __YOU_LOSE_H__
#define __YOU_LOSE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
class YouLose : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuPlayBack(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(YouLose);
};

#endif // __HELLOWORLD_SCENE_H__
