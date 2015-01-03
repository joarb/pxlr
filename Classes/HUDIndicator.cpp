//
//  HUDIndicator.cpp
//  PXLR
//
//  Created by Joar Bølstad on 29.12.14.
//
//

#include "HUDIndicator.h"

#define DOT_INDICATOR_TAG   1


HUDIndicator::HUDIndicator() :
	_remainingDots(6),
	_score(0)
{
    
}

HUDIndicator::~HUDIndicator()
{
    
}

bool HUDIndicator::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    updateDotIndicator();
    return true;
}

void HUDIndicator::removeDotIndicator()
{
    auto children = getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        if ((*it)->getTag() == DOT_INDICATOR_TAG)
        {
            removeChild((*it));
        }
    }
}

void HUDIndicator::updateDotIndicator()
{
    
    float xOffset = 30.0f;
    float xSpacing = 30.0f;
    float ySpacing = 30.0f;
    
    removeDotIndicator();
    
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheet.plist", "SpriteSheet.png");
    cocos2d::SpriteFrame* dotFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_inner_small.png");
    
    for (int i = 0; i < _remainingDots; i++) {
    	cocos2d::Sprite* dotIndicator = cocos2d::Sprite::createWithSpriteFrame(dotFrame);
        dotIndicator->setTag(DOT_INDICATOR_TAG);
        dotIndicator->setPosition(xOffset + xSpacing * i, visibleSize.height - ySpacing);
        addChild(dotIndicator);
    }
    
}

int HUDIndicator::getRemainingDots()
{
    return _remainingDots;
}

void HUDIndicator::setRemainingDots(int newRemainingDots)
{
    _remainingDots = newRemainingDots;
    updateDotIndicator();
}

int HUDIndicator::getScore()
{
    return _score;
}

void HUDIndicator::setScore(int newScore)
{
    
}