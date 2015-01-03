//
//  GameList.cpp
//  PXLR
//
//  Created by Joar Bølstad on 05.12.14.
//
//

#include "GameList.h"

GameList::GameList()
{
    
}

GameList::~GameList()
{
    
}

cocos2d::Scene* GameList::createScene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameList::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameList::init()
{
    return true;
}