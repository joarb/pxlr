//
//  PxlrBlock.cpp
//  PXLR
//
//  Created by Joar Bølstad on 24.09.14.
//
//

#include "PxlrBlock.h"

#define	PXL_TAG	1

// Fetched from
//
// http://stackoverflow.com/questions/2860673/initializing-a-c-vector-to-random-values-fast
// http://en.cppreference.com/w/cpp/numeric/random/rand
//
struct gen_rand_bool {
    double range;
public:
    gen_rand_bool(double r = 1.0) : range(r) {}
    bool operator()() {
        //return (rand()/(double)RAND_MAX) * range;
        return bool(rand() % 2);
    }
};

PxlrBlock::PxlrBlock()
{
    _blockSize = 9;
}

PxlrBlock::~PxlrBlock()
{
    
}

bool PxlrBlock::init()
{
    if (!cocos2d::Layer::init())
    {
        return false;
    }
    
    initBlockState();
    
    float xSize = 0.0f;
    float ySize = 0.0f;
    
    float xShift = PxlrBlock::getXShift();
    float yShift = PxlrBlock::getYShift();
    
    setPxls(xSize, ySize);
    
    float maxX = (_blockSize - 1) % int ( sqrt(_blockSize) );
    float maxY = floor ( ( _blockSize - 1 ) / sqrt(_blockSize) );

    setContentSize(cocos2d::Size(xSize + xShift * maxX, ySize + yShift * maxY));

    return true;
}

bool PxlrBlock::containsTouchLocation(cocos2d::Touch* touch)
{
    //cocos2d::Vec2 touchPos = convertTouchToNodeSpace(touch);
    
    //cocos2d::log("size: %4.2f, %4.2f", getContentSize().width, getContentSize().height);
    //cocos2d::log("pos:  %4.2f, %4.2f", touch->getLocation().x, touch->getLocation().y);
    //cocos2d::log("bobo: %4.2f, %4.2f", getBoundingBox().getMinX(), getBoundingBox().getMaxX());
    
    if (getBoundingBox().containsPoint(touch->getLocation()))
    {
        return true;
    } else {
        return false;
    }
}

float PxlrBlock::getXSpacing()
{
    cocos2d::Size pxlSize = PxlrBlock::getPxlSize();
    return pxlSize.width / 10.0f;
}

float PxlrBlock::getYSpacing()
{
    cocos2d::Size pxlSize = PxlrBlock::getPxlSize();
    return pxlSize.height / 10.0f;
}

float PxlrBlock::getXShift()
{
    cocos2d::Size pxlSize = PxlrBlock::getPxlSize();
    return pxlSize.width + PxlrBlock::getXSpacing();
}

float PxlrBlock::getYShift()
{
    cocos2d::Size pxlSize = PxlrBlock::getPxlSize();
    return pxlSize.height + PxlrBlock::getYSpacing();
}

cocos2d::Size PxlrBlock::getPxlSize()
{
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheet.plist", "SpriteSheet.png");
    cocos2d::SpriteFrame* outerFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_outer_small.png");
    
    cocos2d::Sprite* sprt = cocos2d::Sprite::createWithSpriteFrame(outerFrame);

    return sprt->getContentSize();
}

cocos2d::Size PxlrBlock::getBlockSize()
{
    int PxlrBlockSize = 9;
    
    cocos2d::Size pxlSize = PxlrBlock::getPxlSize();
    float xSize = pxlSize.width;
    float ySize = pxlSize.height;

    float xShift = PxlrBlock::getXShift();
    float yShift = PxlrBlock::getYShift();
    
    float maxX = (PxlrBlockSize - 1) % int ( sqrt(PxlrBlockSize) );
    float maxY = floor ( ( PxlrBlockSize - 1 ) / sqrt(PxlrBlockSize) );

    cocos2d::Size size = cocos2d::Size(xSize + xShift * maxX, ySize + yShift * maxY);
    
    return size;
}

// http://stackoverflow.com/questions/2860673/initializing-a-c-vector-to-random-values-fast
void PxlrBlock::initBlockState()
{
    _pxlStates.reserve(_blockSize);
    std::generate_n(std::back_inserter(_pxlStates), _blockSize, gen_rand_bool());
}

void PxlrBlock::setPxls(float& xSize, float& ySize)
{
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheet.plist", "SpriteSheet.png");
    
    cocos2d::SpriteFrame* outerFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_outer_small.png");
    cocos2d::SpriteFrame* innerFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_inner_small.png");
    
    int counter = 0;
    
    float xShift = PxlrBlock::getXShift();
    float yShift = PxlrBlock::getYShift();
    
    //float xSize = 0.0f;
    //float ySize = 0.0f;
    
    //float xOffset = outerFrame->getTexture()->getContentSize().width / 2.0;
    ///float yOffset = outerFrame->getTexture()->getContentSize().height / 2.0;
    
    for (auto it = _pxlStates.begin(); it != _pxlStates.end(); ++it)
    {
        int x = counter % int ( sqrt(_blockSize) );
        int y = floor ( counter / sqrt(_blockSize) ) ;
        
        cocos2d::Sprite* sprt = cocos2d::Sprite::createWithSpriteFrame(outerFrame);
        
        float xOffset = sprt->getContentSize().width / 2.0;
        float yOffset = sprt->getContentSize().height / 2.0;
        
        xSize = sprt->getContentSize().width;
        ySize = sprt->getContentSize().height;
        
        cocos2d::Vec2 pos = cocos2d::Vec2(xOffset + xShift * x, yOffset + yShift * y);
        
        sprt->setPosition(pos);
        sprt->setTag(PXL_TAG);
        this->addChild(sprt);
        
        if (*it)
        {
            cocos2d::Sprite* sprtInner = cocos2d::Sprite::createWithSpriteFrame(innerFrame);
            
            sprtInner->setPosition(pos);
            sprtInner->setTag(PXL_TAG);
            this->addChild(sprtInner);
        }
        
        counter++;
    }
    
}

void PxlrBlock::mergeIn(const PxlrBlock* block)
{
    for (auto it = _pxlStates.begin(); it != _pxlStates.end(); ++it)
    {
        long index = it - _pxlStates.begin();
        if (!(*it) && (block->_pxlStates.at(index)))
        {
            (*it) = true;
        }
    }

    removePxls();
    
    float xSize = 0.0f;
    float ySize = 0.0f;
    
    setPxls(xSize, ySize);
}

void PxlrBlock::removePxls()
{
    //std::for_each(getChildren().begin(), getChildren().end(), [this](cocos2d::Node* &node){ if (node->getTag() == PXL_TAG) removeChild(node); });
//    for (auto it = getChildren().begin(); it != getChildren().end(); ++it) {
//        if (*it != nullptr)
//        {
//            if ((*it)->getTag() == PXL_TAG)
//            {
//                removeChild(*it);
//            }
//        }
//    }
    
//    for (auto& child : _children)
//    {
//        if(child && child->getTag() == PXL_TAG)
//            this->removeChild(child);
//    }

    //removeChildByTag(PXL_TAG);
    cocos2d::Node* child = getChildByTag(PXL_TAG);
    while (child)
    {
        removeChild(child);
        child = getChildByTag(PXL_TAG);
    }
}

void PxlrBlock::setBlockColor(const cocos2d::Color3B &color)
{
    for (auto it = getChildren().begin(); it != getChildren().end(); ++it) {
        //cocos2d::Sprite* sprt = dynamic_cast<cocos2d::Sprite*>(*it);
        //if (sprt != nullptr) {
            //sprt->setColor(cocos2d::Color3B(1.0f, 0.0f, 0.0f));
        //}
        if ((*it)->getTag() == PXL_TAG) {
            ((cocos2d::Sprite*)(*it))->setColor(color);
        }
    }
}

void PxlrBlock::setNoBlockColor()
{
    setBlockColor(cocos2d::Color3B::WHITE);
}

int PxlrBlock::overlap(const PxlrBlock* otherBlock)
{
    int overlappingPxls = 0;
    for (auto it = otherBlock->_pxlStates.begin(); it != otherBlock->_pxlStates.end(); ++it)
    {
        long index = it - otherBlock->_pxlStates.begin();
        if ((*it) && (_pxlStates.at(index)))
        {
            overlappingPxls++;
        }
    }
    //cocos2d::log("Calculated overlap: %d", overlappingPxls);
    return overlappingPxls;
}