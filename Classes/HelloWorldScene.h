#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PxlrBlock.h"
#include "HUDIndicator.h"

class HelloWorld : public cocos2d::Layer
{
private:
    
    float _minDisplacementX;
    float _maxDisplacementX;
    float _minDisplacementY;
    float _maxDisplacementY;
    
    float _xOffset;
    float _yOffset;

    int _dragSign;
    int _prevDragSign;
    int _dragDirection;
    int _prevDragDirection;
    cocos2d::Vec2 _startTouchPos;
    cocos2d::Vec2 _startPos;
    cocos2d::Vec2 _startDiff;
    PxlrBlock* _startBlock;
    PxlrBlock* _hoverBlock;
    PxlrBlock* _nextPxlBlock;

    std::map<PxlrBlock*, cocos2d::Vec2> _startPositions;

    HUDIndicator* _hudIndicator;
    
    int _numBlocks;
    
    int getNumBlocks();
    float getSpacingCoeff();
    cocos2d::Vec2 getBlockPosition(int col, int row);
    void getBlockIndices(const cocos2d::Vec2& pos, int& col, int& row);
    float getXShift();
    float getYShift();
    
    void fillRemainingCol(int indexRowStart, int indexRowEnd, int indexCol);
    void fillRemainingRow(int indexColStart, int indexColEnd, int indexRow);
    void fillRemainingColRowPositions(PxlrBlock* draggedElement, int dragDirection);
    
    PxlrBlock* getTocuhingBlock(cocos2d::Touch* touch, PxlrBlock* exclude = nullptr);
    
    bool areBlocksAdjacent(const cocos2d::Vec2& pos1, const cocos2d::Vec2& pos2);
    PxlrBlock* getBlock(int column, int row);
    cocos2d::Vector<PxlrBlock*> getAdjacentBlocks(PxlrBlock* originalBlock);
    void getBlockPosition(PxlrBlock* block, int& column, int& row);
    
    void initBlocks();
    
    void moveRemaining(const cocos2d::Vec2 shift);
    
    bool containsTouchLocation(cocos2d::Touch* touch);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void setMaxDisplacmenet(float& minDisplacementX, float& maxDisplacementX, float& minDisplacementY, float& maxDisplacementY);
    
    int calculateScore(PxlrBlock* startBlock, PxlrBlock* endBlock);
    int calculateOverlappingDots(PxlrBlock* startBlock, PxlrBlock* endBlock);

    void pullGap();
    
    void updateScore(PxlrBlock* startBlock, PxlrBlock* endBlock);
    
    static cocos2d::Color3B ACTIVE_BLOCK;
    //static cocos2d::Color3B NORMAL_BLOCK;
    
public:
    
    HelloWorld();
    ~HelloWorld();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    virtual void onEnter();
    virtual void onExit();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__