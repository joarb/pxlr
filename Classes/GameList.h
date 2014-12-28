//
//  GameList.h
//  PXLR
//
//  Created by Joar Bølstad on 05.12.14.
//
//

#ifndef __PXLR__GameList__
#define __PXLR__GameList__

#include "cocos2d.h"

class GameList : public cocos2d::Layer
{
    
public:
    
    GameList();
    ~GameList();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();

    CREATE_FUNC(GameList);
};

#endif /* defined(__PXLR__GameList__) */
