#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PxlrBlock.h"

class HelloWorld : public cocos2d::Layer
{
private:
    
    float _maxDisplacementX;
    float _maxDisplacementY;
    
    int _dragDirection;
    cocos2d::Vec2 _startTouchPos;
    cocos2d::Vec2 _startPos;
    cocos2d::Vec2 _startDiff;
    PxlrBlock* _startBlock;
    
    int _numBlocks;
    
    int getNumBlocks();
    float getSpacingCoeff();
    cocos2d::Vec2 getBlockPosition(int col, int row);
    void getBlockIndices(cocos2d::Vec2 pos, int& col, int& row);
    float getXShift();
    float getYShift();
    
    PxlrBlock* getTocuhingBlock(cocos2d::Touch* touch);
    
    bool areBlocksAdjacent(PxlrBlock* block1, PxlrBlock* block2);
    PxlrBlock* getBlock(int column, int row);
    cocos2d::Vector<PxlrBlock*> getAdjacentBlocks(PxlrBlock* originalBlock);
    void getBlockPosition(PxlrBlock* block, int& column, int& row);
    
    void initBlocks();
    
    bool containsTouchLocation(cocos2d::Touch* touch);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void SetMaxDisplacmenet(float& maxDisplacementX, float& maxDisplacementY);
    
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