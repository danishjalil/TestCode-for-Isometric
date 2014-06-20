#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocos2d;
USING_NS_CC;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("iso-test2.tmx");
    CCSize ms = _tileMap->getMapSize();
    CCSize ts = _tileMap->getTileSize();
    _tileMap->setPosition(ccp( -ms.width * ts.width/2, -ms.height * ts.height/2 ));

    _background = _tileMap->layerNamed("BackGround");
    _tree = _tileMap->layerNamed("Tree");
    this->addChild(_tileMap);
    
    
    CCMenuItemSprite *btn = CCMenuItemSprite::create(CCSprite::create("button.png"),NULL,this,menu_selector(HelloWorld::buttonhandler));

    CCMenu *pMenu = CCMenu::create(btn, NULL);
    
    this->addChild(pMenu, 1);
    make_building = false;
    player =  CCSprite::create("troll_1.png");
    player->setPosition(ccp(200,200));
    this->addChild(player);
   //this->setViewPointCenter(player->getPosition());
    _tileMap->setPosition(ccp(-2000,-1000));
    
    bldng = CCSprite::create("building.png");
    //bldng->setPosition(ccp(200,200));
    //this->addChild(background);
    //this->addChild(player);
    this->addChild(bldng);
    
    

    this->setTouchEnabled(true);
    this->scheduleUpdate();
    return true;
    
}






void HelloWorld::update(float dt) {
   // float x  = player->getPositionX();
   // x = x+1;
    //player->setPositionX(x);
    
    
}


void HelloWorld::menuStartCallback(cocos2d::CCObject* pSender)
{
   
}
    
void HelloWorld::ccTouchesMoved(CCSet * pTouches, CCEvent * pEvent) {
    
    CCSetIterator j;
	CCTouch* touch;
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize size_gl = pEGLView->getFrameSize();
	float ab = size_gl.height;
	
    
	for( j = pTouches->begin(); j != pTouches->end(); j++)
	{
        touch = (CCTouch*) (*j);
        if (touch)
        {
            CCPoint tap = touch->getLocation();
            if(make_building) {
                bldng->setPosition(ccp(tap.x,tap.y));
                int bldng_width_tiles = bldng->getContentSize().width/_tileMap->getTileSize().width;
                int bldng_height_tiles = bldng->getContentSize().height/_tileMap->getTileSize().height;
                CCLog("Building height is %d and width is %d" , bldng_height_tiles,bldng_width_tiles);
                CCPoint bottom_left,tile;
                bottom_left.x = bldng->getPositionX() -  bldng->getContentSize().width/2;
                bottom_left.y = bldng->getPositionY() -  bldng->getContentSize().height/2;
                CCString *str = CCString::create("True");
                //tile = this->tileCoordForPosition(bottom_left);
                //int tileGid = _background->tileGIDAt(bottom_left);
                for (int i = 0 ; i<bldng_height_tiles ; i++) {
                        bottom_left.x = bldng->getPositionX() -  bldng->getContentSize().width/2;
                        bottom_left.y = bldng->getPositionY() -  bldng->getContentSize().height/2;
                        
                        for (int j = 0 ; j<bldng_width_tiles; j++) {
                            
                            tile = this->tileCoordForPosition(bottom_left);
                            int tileGid = _background->tileGIDAt(tile);
                            if (tileGid) {
                                CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
                                if (properties) {
                                    properties->setObject(str, "Collidable");
                                }
                            }

                            bottom_left.x  = bottom_left.x + j*(_tileMap->getTileSize().width);
                            bottom_left.y = bottom_left.y+ i*(_tileMap->getTileSize().height);
                    }
                }
                
            }
        }

    }
    
}


void HelloWorld::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent){
    make_building = false;
    CCLog("MAke building is false");
    CCSetIterator j;
	CCTouch* touch;
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize size_gl = pEGLView->getFrameSize();
	float ab = size_gl.height;
	
    
	for( j = pTouches->begin(); j != pTouches->end(); j++)
	{
        touch = (CCTouch*) (*j);
        if (touch) {
            CCPoint touchLocation = touch->getLocationInView();
            touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
            touchLocation = this->convertToNodeSpace(touchLocation);
            
            CCPoint playerPos = player->getPosition();
            CCPoint diff = ccpSub(touchLocation, playerPos);
            if ( abs(diff.x) > abs(diff.y) ) {
                if (diff.x > 0) {
                    playerPos.x += _tileMap->getTileSize().width;
                } else {
                    playerPos.x -= _tileMap->getTileSize().width;
                }
            } else {
                if (diff.y > 0) {
                    playerPos.y += _tileMap->getTileSize().height;
                } else {
                    playerPos.y -= _tileMap->getTileSize().height;
                }
            }
            
            // safety check on the bounds of the map
            if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
                playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
                playerPos.y >= 0 &&
                playerPos.x >= 0 )
            {
                this->setPlayerPosition(playerPos);
            }
            
        }

    }
    
    
}


void HelloWorld::buttonhandler(cocos2d::CCObject* pSender) {
    
    make_building = true;
    CCLog("MAke building is true");
}



void HelloWorld::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent) {
    
    CCSetIterator j;
	CCTouch* touch;
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize size_gl = pEGLView->getFrameSize();
	float ab = size_gl.height;
	
    
	for( j = pTouches->begin(); j != pTouches->end(); j++)
	{
        touch = (CCTouch*) (*j);
        if (touch)
        {
            CCPoint tap = touch->getLocation();
            int collision = check_collision(tap, bldng);
            if(collision == 1 && !make_building) {
                CCLog("The house was clicked");
                
            }
        }
        
    }
    
}





int HelloWorld::check_collision(CCPoint cord, CCSprite*sprite)
{
    float xcord = cord.x;
    float ycord = cord.y;
    CCPoint cur_pos = sprite->getPosition();
    //printf( "posotion of sprite is  %f,%f\n", cur_pos.x, cur_pos.y );
    float width = sprite->boundingBox().size.width;
    //printf( "width of sprite is  %f\n", width);
    float height = sprite->boundingBox().size.height;
    if (cord.x > (cur_pos.x - width/2) && cord.x < (cur_pos.x + width/2) )
    {
        //printf( "if x is greater than %f and less %f \n",cur_pos.x - width/2,cur_pos.x + width/2);
        if (cord.y > (cur_pos.y - height/2) && cord.y < (cur_pos.y + height/2) )
        {
            return 1;
        }
        //printf("returning false");
        return 0;
    }
    //printf("returning false");
    return 0;
    
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}


/*
void HelloWorld::setViewPointCenter(CCPoint position) {
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

 */


void HelloWorld::setPlayerPosition(CCPoint position)
{
    CCPoint tileCoord = this->tileCoordForPosition(position);
    tileCoord.x = (int)tileCoord.x;
    tileCoord.y = (int)tileCoord.y;
    CCLog("Tile cord in x is %f",tileCoord.x);
    CCLog("Tile cord in y is %f",tileCoord.y);
    
 
    int tileGid = _background->tileGIDAt(tileCoord);
    CCLog("the gid is %d ", tileGid);
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            CCLog("The value of Collision is %s", collision->getCString());
            if (collision && (collision->compare("True") == 0)) {
                return;
            }
        }
    }
    player->setPosition(position);
}
