#include "HelloWorldScene.h"

USING_NS_CC;

#define BLOCK_TAG	1

#define DRAG_NONE	0
#define DRAG_HORIZ	1
#define DRAG_VERT	2

#define SNAP_DIRECTION	20

HelloWorld::HelloWorld() :
	_maxDisplacementX(0.0f),
	_maxDisplacementY(0.0f),
	_xOffset(0.0f),
	_yOffset(0.0f),
	_dragDirection(DRAG_NONE),
	_startTouchPos(cocos2d::Vec2(0.0f, 0.0f)),
	_startPos(cocos2d::Vec2(0.0f, 0.0f)),
	_startDiff(cocos2d::Vec2(0.0f, 0.0f)),
	_startBlock(nullptr),
	_numBlocks(0)
{
    _xOffset = 30.0f;
    _yOffset = 30.0f;
}

HelloWorld::~HelloWorld()
{
    
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    _numBlocks = 9;

    initBlocks();
    
    SetMaxDisplacmenet(_maxDisplacementX, _maxDisplacementY);
    
    
    /*
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheet.plist", "SpriteSheet.png");
    
    cocos2d::SpriteFrame* outerFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_outer.png");
    cocos2d::SpriteFrame* innerFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("pxlr_inner.png");

    cocos2d::Vec2 pos = cocos2d::Vec2(100.0f, 100.0f);
    cocos2d::Vec2 anchor = cocos2d::Vec2(0.5f, 0.5f);
    
    cocos2d::Sprite* sprt = cocos2d::Sprite::createWithSpriteFrame(outerFrame);
    
    sprt->setAnchorPoint(anchor);
    sprt->setPosition(pos);
    this->addChild(sprt);
    
    cocos2d::Sprite* sprtInner = cocos2d::Sprite::createWithSpriteFrame(innerFrame);
        
    sprtInner->setAnchorPoint(anchor);
    sprtInner->setPosition(pos);
    this->addChild(sprtInner);
     */

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    //label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::SetMaxDisplacmenet(float& maxDisplacementX, float& maxDisplacementY)
{
    maxDisplacementX = getXShift();
    maxDisplacementY = getYShift();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

int HelloWorld::getNumBlocks()
{
    return _numBlocks;
}

float HelloWorld::getSpacingCoeff()
{
    float spacingCoeff = 9;
    return spacingCoeff;
}

cocos2d::Vec2 HelloWorld::getBlockPosition(int col, int row)
{
    float xShift = getXShift();
    float yShift = getYShift();

    return cocos2d::Vec2(col * xShift + _xOffset, row * yShift + _yOffset);
}

void HelloWorld::getBlockIndices(cocos2d::Vec2 pos, int& col, int& row)
{
    float xShift = getXShift();
    float yShift = getYShift();

    col = round((pos.x - _xOffset)/xShift);
    row = round((pos.y - _yOffset)/yShift);
}

float HelloWorld::getXShift()
{
    return getSpacingCoeff() * PxlrBlock::getXSpacing() + PxlrBlock::getBlockSize().width;
}

float HelloWorld::getYShift()
{
    return getSpacingCoeff() * PxlrBlock::getYSpacing() + PxlrBlock::getBlockSize().height;
}

void HelloWorld::initBlocks()
{
    for (int i = 0; i < _numBlocks; i++)
    {
        int x = i % int ( sqrt( getNumBlocks() ) );
        int y = floor ( i / sqrt( getNumBlocks() ) ) ;

        PxlrBlock* block = PxlrBlock::create();
        //cocos2d::Vec2 pos = getBlockPosition(x, y);
        //log("%4.2f, %4.2f", pos.x, pos.y);
        block->setPosition(getBlockPosition(x, y));
        block->setTag(BLOCK_TAG);
        this->addChild(block);
    }
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    //    auto director = Director::getInstance();
    //    director->getTouchDispatcher()->removeDelegate(this);
    Layer::onExit();
}

bool HelloWorld::containsTouchLocation(Touch* touch)
{
    //return cocos2d::SpriteFrame::getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
    return true;
}

void HelloWorld::getBlockPosition(PxlrBlock* block, int& column, int& row)
{
    cocos2d::Vec2 pos = block->getPosition();
    getBlockIndices(pos, column, row);
}

bool HelloWorld::areBlocksAdjacent(PxlrBlock* block1, PxlrBlock* block2)
{
    int b1Col;
    int b1Row;
    int b2Col;
    int b2Row;
    getBlockPosition(block1, b1Col, b1Row);
    getBlockPosition(block2, b2Col, b2Row);
    
    if (((b1Col == b2Col + 1) or (b1Col == b2Col - 1)) and ((b1Row == b2Row + 1) or (b1Row = b2Row - 1))) {
        return true;
    } else {
        return false;
    }
}

PxlrBlock* HelloWorld::getBlock(int column, int row)
{
    auto children = getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        if ((*it)->getTag() == BLOCK_TAG) {
            PxlrBlock* block = (PxlrBlock*)(*it);
            
            int bCol;
            int bRow;
            
            getBlockPosition(block, bCol, bRow);
            if ((bCol == column) && (bRow == row)) {
                return block;
            }
        }
    }
    return nullptr;
}

cocos2d::Vector<PxlrBlock*> HelloWorld::getAdjacentBlocks(PxlrBlock* originalBlock)
{
    cocos2d::Vector<PxlrBlock*> adjacentBlocks;
    
    int bOrigCol;
    int bOrigRow;
    getBlockPosition(originalBlock, bOrigCol, bOrigRow);
    
    // TODO: Implement
    
    return adjacentBlocks;
}

PxlrBlock* HelloWorld::getTocuhingBlock(Touch* touch)
{
    auto children = getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        if ((*it)->getTag() == BLOCK_TAG) {
            PxlrBlock* block = (PxlrBlock*)(*it);
            if (block->containsTouchLocation(touch)) {
                return block;
            }
        }
    }
    return nullptr;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    /*
    CCLOG("Paddle::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    if (_state != kPaddleStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    _state = kPaddleStateGrabbed;
    CCLOG("return true");
     */
    _startBlock = getTocuhingBlock(touch);
    if (_startBlock)
    {
    	_startPos = _startBlock->getPosition();
        _startDiff = _startPos - touch->getLocation();
        //log("StartPos:  %4.2f, %4.2f", _startPos.x, _startPos.y);
        //log("TouchPos:  %4.2f, %4.2f", touch->getLocation().x, touch->getLocation().y);
        //log("Startdiff: %4.2f, %4.2f", _startDiff.x, _startDiff.y);
        _startTouchPos = touch->getLocation();
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    
    // TODO: Implement moving the block while swiping
    
    
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get Sets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    /*
    CCLOG("Paddle::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    CCASSERT(_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");
    
    auto touchPoint = touch->getLocation();
    
    setPosition( Vec2(touchPoint.x, getPosition().y) );
     */
    if (_startBlock)
    {
        auto touchPoint = touch->getLocation();
        
        auto diff = _startTouchPos - touchPoint;
        auto distance = touchPoint.getDistance(_startTouchPos);
        //if (distance < SNAP_DIRECTION)
        //{
            if (pow(diff.x, 2) > pow(diff.y, 2))
            {
                _dragDirection = DRAG_HORIZ;
            } else {
                _dragDirection = DRAG_VERT;
            }
        //}
        
        log("%4.2f, %4.2f", touchPoint.x + _startDiff.x, _maxDisplacementX);
        log("%4.2f, %4.2f", touchPoint.y + _startDiff.y, _maxDisplacementY);
        
        cocos2d::Vec2 displacement;
        if ((touchPoint.x + _startDiff.x) > (_startPos.x + _maxDisplacementX))
        {
            displacement.x = _startPos.x + _maxDisplacementX;
        } else if ((touchPoint.x + _startDiff.x) < (_startPos.x - _maxDisplacementX))
        {
            displacement.x = _startPos.x - _maxDisplacementX;
        } else {
            displacement.x = touchPoint.x + _startDiff.x;
        }
        
        if ((touchPoint.y + _startDiff.y) > (_startPos.y + _maxDisplacementY))
        {
            displacement.y = _startPos.y + _maxDisplacementY;
        } else if ((touchPoint.y + _startDiff.y) < (_startPos.y - _maxDisplacementY))
        {
            displacement.y = _startPos.y - _maxDisplacementY;
        } else {
            displacement.y = touchPoint.y + _startDiff.y;
        }
        
        if (_dragDirection == DRAG_HORIZ)
        {
            _startBlock->setPosition( Vec2(displacement.x, _startPos.y) );
        } else {
            _startBlock->setPosition( Vec2(_startPos.x, displacement.y) );
        }
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    /*
    CCASSERT(_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");
    
    _state = kPaddleStateUngrabbed;
     */
    
    if (_startBlock) {
        _startBlock->setPosition(_startPos);
        PxlrBlock* endBlock = getTocuhingBlock(touch);
        if (endBlock)
        {
            if (areBlocksAdjacent(_startBlock, endBlock)) {
                // TODO: Swap!
            }
            _startBlock = nullptr;
            _dragDirection = DRAG_NONE;
        }
    }
}
