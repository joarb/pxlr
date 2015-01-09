//
//  PxlrBlock.h
//  PXLR
//
//  Created by Joar Bølstad on 24.09.14.
//
//

#ifndef __PXLR__Block__
#define __PXLR__Block__

#include "cocos2d.h"

class PxlrBlock : public cocos2d::Layer
{
private:
    
    int _blockSize;
    std::vector<bool> _pxlStates;
    
    void initBlockState();
    
    void removePxls();
    void setPxls(float& xSize, float& ySize);
    
public:
    
    PxlrBlock();
    ~PxlrBlock();
    
    bool containsTouchLocation(cocos2d::Touch* touch);
    
    static cocos2d::Size getBlockSize();
    static cocos2d::Size getPxlSize();
    static float getXShift();
    static float getYShift();
    static float getXSpacing();
    static float getYSpacing();
    
    void mergeIn(const PxlrBlock* block);
    
    int overlap(const PxlrBlock* block);
    
    void setBlockColor(const cocos2d::Color3B &color);
    void setNoBlockColor();
    
    virtual bool init();
    CREATE_FUNC(PxlrBlock);
};

#endif /* defined(__PXLR__Block__) */
