#include "HelloWorldScene.h"

USING_NS_CC;

#define BLOCK_TAG	    1

#define DRAG_NONE	0
#define DRAG_HORIZ	1
#define DRAG_VERT	2

#define SNAP_DIRECTION	20

cocos2d::Color3B HelloWorld::ACTIVE_BLOCK = cocos2d::Color3B(0, 0, 255);

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
	_hoverBlock(nullptr),
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
    
    _hudIndicator = HUDIndicator::create();
    addChild(_hudIndicator);
    
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
    
    _numBlocks = 16;

    initBlocks();
    
    setMaxDisplacmenet(_minDisplacementX, _maxDisplacementX, _minDisplacementY, _maxDisplacementY);
    
    
    return true;
}

void HelloWorld::setMaxDisplacmenet(float& minDisplacementX, float& maxDisplacementX, float& minDisplacementY, float& maxDisplacementY)
{
    maxDisplacementX = getXShift();
    minDisplacementX = -maxDisplacementX;
    
    maxDisplacementY = getYShift();
    minDisplacementY = -maxDisplacementY;
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

void HelloWorld::getBlockIndices(const cocos2d::Vec2& pos, int& col, int& row)
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

bool HelloWorld::areBlocksAdjacent(const cocos2d::Vec2& pos1, const cocos2d::Vec2& pos2)
{
    int b1Col;
    int b1Row;
    int b2Col;
    int b2Row;
    getBlockIndices(pos1, b1Col, b1Row);
    getBlockIndices(pos2, b2Col, b2Row);
    
    if ((((b1Col == b2Col + 1) or (b1Col == b2Col - 1)) and (b1Row == b2Row))
        or (((b1Row == b2Row + 1) or (b1Row == b2Row - 1)) and (b1Col == b2Col))) {
        //cocos2d::log("Adjacent: (%d, %d), (%d, %d)", b1Col, b1Row, b2Col, b2Row);
        return true;
    } else {
        //cocos2d::log("Not adjacent: (%d, %d), (%d, %d)", b1Col, b1Row, b2Col, b2Row);
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

PxlrBlock* HelloWorld::getTocuhingBlock(Touch* touch, PxlrBlock* exclude)
{
    auto children = getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        if ((*it)->getTag() == BLOCK_TAG) {
            PxlrBlock* block = (PxlrBlock*)(*it);
            if (block != exclude)
            {
                if (block->containsTouchLocation(touch)) {
                	return block;
            	}
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
        _startBlock->setZOrder(1);
        _startBlock->setBlockColor(cocos2d::Color3B::RED);
    	_startPos = _startBlock->getPosition();
        _startDiff = _startPos - touch->getLocation();
        //log("StartPos:  %4.2f, %4.2f", _startPos.x, _startPos.y);
        //log("TouchPos:  %4.2f, %4.2f", touch->getLocation().x, touch->getLocation().y);
        //log("Startdiff: %4.2f, %4.2f", _startDiff.x, _startDiff.y);
        
        int col;
        int row;
        
        setMaxDisplacmenet(_minDisplacementX, _maxDisplacementX, _minDisplacementY, _maxDisplacementY);
        getBlockPosition(_startBlock, col, row);
        
        if (col == 0)
        {
            _minDisplacementX = 0.0f;
        }
        else if (col == (int(sqrt(_numBlocks)) - 1))
        {
            _maxDisplacementX = 0.0f;
        }
        
        if (row == 0)
        {
            _minDisplacementY = 0.0f;
        }
        else if (row == (int(sqrt(_numBlocks)) - 1))
        {
            _maxDisplacementY = 0.0f;
        }
        
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
        
        cocos2d::Vec2 displacement = _startPos;

        auto touchPoint = touch->getLocation();
        auto diff = _startTouchPos - touchPoint;
        
        if (pow(diff.x, 2) > pow(diff.y, 2))
        {
            _dragDirection = DRAG_HORIZ;
        } else {
            _dragDirection = DRAG_VERT;
        }
        
        if ((touchPoint.x + _startDiff.x) > (_startPos.x + _maxDisplacementX))
        {
            displacement.x = _startPos.x + _maxDisplacementX;
        } else if ((touchPoint.x + _startDiff.x) < (_startPos.x + _minDisplacementX))
        {
            displacement.x = _startPos.x + _minDisplacementX;
        } else {
            displacement.x = touchPoint.x + _startDiff.x;
        }
        
        if ((touchPoint.y + _startDiff.y) > (_startPos.y + _maxDisplacementY))
        {
            displacement.y = _startPos.y + _maxDisplacementY;
        } else if ((touchPoint.y + _startDiff.y) < (_startPos.y + _minDisplacementY))
        {
            displacement.y = _startPos.y + _minDisplacementY;
        } else {
            displacement.y = touchPoint.y + _startDiff.y;
        }
        
        PxlrBlock* hoverBlock = getTocuhingBlock(touch, _startBlock);
        if (hoverBlock)
        {
            
            if (areBlocksAdjacent(_startPos, hoverBlock->getPosition()))
            {
                if (_hoverBlock && (_hoverBlock != hoverBlock))
                {
                    _hoverBlock->setNoBlockColor();
                }
                _hoverBlock = hoverBlock;
                _hoverBlock->setBlockColor(cocos2d::Color3B::BLUE);
                
                // Set drag direction
                int hoverCol;
                int hoverRow;
                int startCol;
                int startRow;
                
                getBlockPosition(_hoverBlock, hoverCol, hoverRow);
                getBlockIndices(_startPos, startCol, startRow);
                
                //cocos2d::log("(%d, %d), (%d, %d)", startCol, startRow, hoverCol, hoverRow);
                if (hoverCol == startCol)
                {
                    _dragDirection = DRAG_VERT;
                    //cocos2d::log("_dragDir: VERT, (%d, %d), (%d, %d)", startCol, startRow, hoverCol, hoverRow);
                }
                if (hoverRow == startRow)
                {
                    _dragDirection = DRAG_HORIZ;
                    //cocos2d::log("_dragDir: HORIZ, (%d, %d), (%d, %d)", startCol, startRow, hoverCol, hoverRow);
                }
                
            }
            
        } else {

            if (_hoverBlock)
            {
                _hoverBlock->setNoBlockColor();
            }
            
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
	if (_startBlock) {
        _startBlock->setPosition(_startPos);
        PxlrBlock* endBlock = getTocuhingBlock(touch, _startBlock);
        if (endBlock)
        {
            if (areBlocksAdjacent(_startPos, endBlock->getPosition())) {
                
                // TODO: Animate swap
                
                _startBlock->setPosition(endBlock->getPosition());
                endBlock->setPosition(_startPos);
                
                updateScore(_startBlock, endBlock);
                
            }
        }
        _startBlock->setNoBlockColor();
        _startBlock->setZOrder(0);
        _startBlock = nullptr;
        _dragDirection = DRAG_NONE;
        if (_hoverBlock)
        {
            _hoverBlock->setNoBlockColor();
            _hoverBlock = nullptr;
        }
    }
}

int HelloWorld::calculateScore(PxlrBlock* startBlock, PxlrBlock* endBlock)
{
    return 0;
}

int HelloWorld::calculateOverlappingDots(PxlrBlock* startBlock, PxlrBlock* endBlock)
{
    return startBlock->overlap(endBlock);
}

void HelloWorld::updateScore(PxlrBlock* startBlock, PxlrBlock* endBlock)
{
    int newRemainingDots = _hudIndicator->getRemainingDots() - calculateOverlappingDots(startBlock, endBlock);
        
    // TODO: Calculate new score
    
    _hudIndicator->setRemainingDots(newRemainingDots);
}
