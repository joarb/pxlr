//
//  HUDIndicator.h
//  PXLR
//
//  Created by Joar Bølstad on 29.12.14.
//
//

#ifndef __PXLR__HUDIndicator__
#define __PXLR__HUDIndicator__

#include "cocos2d.h"

class HUDIndicator : public cocos2d::Layer
{
    
    int _remainingDots;
    int _score;
    
    void removeDotIndicator();
    void updateDotIndicator();
    
public:
    
    HUDIndicator();
    ~HUDIndicator();
    
    virtual bool init();
    
    CREATE_FUNC(HUDIndicator);
    
    int getRemainingDots();
    void setRemainingDots(int newRemainingDots);
    
    int getScore();
    void setScore(int newScore);

};

#endif /* defined(__PXLR__HUDIndicator__) */
