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
    _tileMap->initWithTMXFile("iso-test-objectgroup.tmx");
    CCSize ms = _tileMap->getMapSize();
    CCSize ts = _tileMap->getTileSize();
    _tileMap->setPosition(ccp( -ms.width * ts.width/2, -ms.height * ts.height/2 ));
    _background = _tileMap->layerNamed("Background");
    //_tree = _tileMap->layerNamed("Tree");
    this->addChild(_tileMap);
    
    
    CCMenuItemSprite *btn = CCMenuItemSprite::create(CCSprite::create("button.png"),NULL,this,menu_selector(HelloWorld::buttonhandler));

    CCMenu *pMenu = CCMenu::create(btn, NULL);
    
    this->addChild(pMenu, 1);
    make_building = false;
    player =  CCSprite::create("troll_1.png");
    ball = CCSprite::create("tinyball.png");
    
    
    player->setPosition(ccp(200,200));
    player->setVisible(false);
    this->addChild(player);
    this->addChild(ball);
   //this->setViewPointCenter(player->getPosition());
    _tileMap->setPosition(ccp(0,0));
    
    bldng = CCSprite::create("building.png");
    //bldng->setPosition(ccp(200,200));
    //this->addChild(background);
    //this->addChild(player);
    this->addChild(bldng);
    
    

    this->setTouchEnabled(true);
    this->scheduleUpdate();
    Ctile tile;
    tile.sprite = _background->tileAt(ccp(0,0));
    tile.blocked = false;
   // std::vector< std::vector<Ctile> > tiles_vec (_background->getLayerSize().width, std::vector<Ctile>(_background->getLayerSize().height,tile)) ;
    //std::vector<Ctile> custom_vec;
    for (int i = 0 ; i<_background->getLayerSize().width ;i++)
    {
        tiles_vec.push_back(std::vector<Ctile>());
        
        for (int j = 0 ; j<_background->getLayerSize().height; j++)
        {
            
            tile.sprite = _background->tileAt(ccp(i,j));
            tile.blocked = false;
            
            CCLog("Tile cords are  %d , %d", i ,j);
            CCPoint temp_pt = isoTo2D(tile.sprite->getPosition());
            CCLog("Position in cartesian of tile is %f and %f",temp_pt.x,temp_pt.y);
            
          //  temp_pt = isoToTiled(tile.sprite->getPosition());
          //  CCLog("Position in tiled of tile is %f and %f",temp_pt.x,temp_pt.y);
            
            temp_pt = tile.sprite->getPosition();
            CCLog("Position in iso of tile is %f and %f",temp_pt.x,temp_pt.y);

            
            temp_pt = convert2Tilecord(isoTo2D(tile.sprite->getPosition()));
            CCLog("Position in real cords of tile is %f and %f",temp_pt.x,temp_pt.y);

            
            //custom_vec.push_back(tile);
            //tiles_vec.push_back(custom_vec);
            tiles_vec[i].push_back(tile);
            
        }
        
    }
    
    CCPoint p1 = isoTo2D(_background->tileAt(ccp(0,0))->getPosition());
    CCPoint p2 = isoTo2D(_background->tileAt(ccp(0,1))->getPosition());
    CCPoint p3 = isoTo2D(_background->tileAt(ccp(1,0))->getPosition());
    width_tile = abs(p1.x -p2.x);
    
    height_tile = abs(p1.y - p3.y);
    
   
    ball->setPosition(convertTile2iso(ccp(0,9)));
    //convert2Tilecord(ccp(0,0));
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
                if(tap.x - bldng->getContentSize().width/2 > 0 && tap.y-bldng->getContentSize().height/2)
                bldng->setPosition(ccp(tap.x,tap.y));
                
            }
        }

    }
    
///Make tiles under building Collidable
    /*
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
                GameSprite*tempsprite = (GameSprite*) _background->tileAt(tile);
                tempsprite->setCollidable(true);
                auto clogtilepos = tileCoordForPosition(tempsprite->getPosition());
                CCLog("Collidable tile position x and y are %f and %f", clogtilepos.x , clogtilepos.y);
                
            }
            
            
            bottom_left.x  = bottom_left.x + j*(_tileMap->getTileSize().width);
            bottom_left.y = bottom_left.y+ i*(_tileMap->getTileSize().height);
        }
    }
    */
     /*
    
    CCPoint clogtilepos = isoToTiled(bldng->getPosition());
    CCLog("Position of building is %f and %f", clogtilepos.x , clogtilepos.y);
    CCLog("Width and height of building is %f and %f", bldng->getContentSize().width,bldng->getContentSize().height);
    CCLog("Anchor point is %f and %f",bldng->getAnchorPoint().x,bldng->getAnchorPoint().y);
    int bldng_width_tiles = bldng->getContentSize().width/width_tile;
    int bldng_height_tiles = bldng->getContentSize().height/height_tile;
    auto bottom_left = ccp(clogtilepos.x+(bldng_height_tiles/2) ,clogtilepos.y+(bldng_width_tiles/2));
    CCLog("Collidable building middle position x and y are %f and %f", clogtilepos.x , clogtilepos.y);

    clogtilepos = isoToTiled(bottom_left);
    //CCLog("Bottom left position in iso is %f and %f" , bottom_left.x , bottom_left.y);
    CCLog("Collidable building bottom left position in tiles, x and y are %f and %f", bottom_left.x , bottom_left.y);
    CCLog("Width of building is %d and height is %d",bldng_width_tiles,bldng_height_tiles);
    int x = bottom_left.x;
    int y = clogtilepos.y;
    bldng_height_tiles = bldng_height_tiles;
    bldng_width_tiles = bldng_width_tiles;
    
    float f = _tileMap->getMapSize().height;
    CCLog("Total tiles are %f", f);
    for (int i = 0 ; i<bldng_width_tiles; i++) {
        
        y = bottom_left.y;
        
        for (int j = 0; j<bldng_height_tiles;j++) {
            //CCLog("u cant walk on these tiles %d and %d",x,y);
            //CCPoint point = convertTiletoPosition(ccp(x,y));
            //CCLog("u cant walk on this position %f and %f",point.x,point.y);
            //CCLog("The y position of the building is %d",y);
            
            CCLog("x and y cordinates in tiles are  tiles %d and %d",x,y);

            if((x >= 0 && x < _background->getLayerSize().width) && y >= 0 && y < _background->getLayerSize().height) {
            //GameSprite* temp = (GameSprite*)_background->tileAt(ccp(x,y));
            CCLog("u cant walk on these tiles %d and %d",x,y);
                tiles_vec[x][y].blocked = true;
            //temp->setCollidable(true);
            }
            y = y - 1;
            
        }
        x = x-1;
    }
  */  
    
    
}


void HelloWorld::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent){
    make_building = false;
    CCLog("MAke building is false");
    CCSetIterator j;
	CCTouch* touch;
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize size_gl = pEGLView->getFrameSize();
	float ab = size_gl.height;
	
    
    
    CCPoint clogtilepos = isoToTiled(bldng->getPosition());
    CCLog("Position of building is %f and %f", clogtilepos.x , clogtilepos.y);
    CCLog("Width and height of building is %f and %f", bldng->getContentSize().width,bldng->getContentSize().height);
    CCLog("Anchor point is %f and %f",bldng->getAnchorPoint().x,bldng->getAnchorPoint().y);
    int bldng_width_tiles = bldng->getContentSize().width/width_tile;
    int bldng_height_tiles = bldng->getContentSize().height/height_tile;
    auto bottom_left = ccp(clogtilepos.x+(bldng_height_tiles/2) ,clogtilepos.y+(bldng_width_tiles/2));
    CCLog("Collidable building middle position x and y are %f and %f", clogtilepos.x , clogtilepos.y);
    
    clogtilepos = isoToTiled(bottom_left);
    //CCLog("Bottom left position in iso is %f and %f" , bottom_left.x , bottom_left.y);
    CCLog("Collidable building bottom left position in tiles, x and y are %f and %f", bottom_left.x , bottom_left.y);
    CCLog("Width of building is %d and height is %d",bldng_width_tiles,bldng_height_tiles);
    int x = bottom_left.x;
    int y = clogtilepos.y;
    bldng_height_tiles = bldng_height_tiles;
    bldng_width_tiles = bldng_width_tiles;
    
    float f = _tileMap->getMapSize().height;
    CCLog("Total tiles are %f", f);
    for (int i = 0 ; i<bldng_width_tiles; i++) {
        
        y = bottom_left.y;
        
        for (int j = 0; j<bldng_height_tiles;j++) {
            //CCLog("u cant walk on these tiles %d and %d",x,y);
            //CCPoint point = convertTiletoPosition(ccp(x,y));
            //CCLog("u cant walk on this position %f and %f",point.x,point.y);
            //CCLog("The y position of the building is %d",y);
            
            CCLog("x and y cordinates in tiles are  tiles %d and %d",x,y);
            
            if((x >= 0 && x < _background->getLayerSize().width) && y >= 0 && y < _background->getLayerSize().height) {
                //GameSprite* temp = (GameSprite*)_background->tileAt(ccp(x,y));
                CCLog("u cant walk on these tiles %d and %d",x,y);
                tiles_vec[x][y].blocked = true;
                //temp->setCollidable(true);
            }
            y = y - 1;
            
        }
        x = x-1;
    }
    

    
    
    
    
    
    
    
	for( j = pTouches->begin(); j != pTouches->end(); j++)
	{
        touch = (CCTouch*) (*j);
        /*
        if (touch) {
            CCPoint touchLocation = touch->getLocation();
            CCPoint playerPos = player->getPosition();
            CCPoint diff = ccpSub(touchLocation, playerPos);
            if ( abs(diff.x) > abs(diff.y) ) {
                if (diff.x > 0) {
                    playerPos.x += width_tile;
                } else {
                    playerPos.x -= width_tile;
                }
            } else {
                if (diff.y > 0) {
                    playerPos.y += height_tile;
                } else {
                    playerPos.y -= height_tile;
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
        */
        if (touch) {
            CCPoint playerPos = ball->getPosition();
            //CCPoint touchLocation = touch->getLocation();
            auto check_pos = _background->tileAt(ccp(0,0))->getPosition();
            auto temp = isoToTiled(playerPos);
            //temp.x = temp.x + 1;
            //temp.y = 0;
            //temp = ccp(0,0);
            CCPoint newpos = convertTile2iso(temp);
            CCPoint tilepos = isoToTiled(newpos);
            CCLog("Player position in tiles is %f , %f", tilepos.x,tilepos.y);
            CCLog("Player position in iso is %f , %f", newpos.x,newpos.y);
            //bfs(ccp(0,9),ccp(9,0));
            path_tiles = bfs(tilepos,ccp(9,0));
            
            
            /*
            for (std::list<node_block>::iterator it=path.begin(); it != path.end(); ++it){
                CCPoint tilecord = ccp(it->Tile.x,it->Tile.y);
                this->setPlayerPosition(convertTile2iso(tilecord));
                CCDelayTime* delayAction = CCDelayTime::create(2.0f);
                this->runAction(CCSequence::create(delayAction,NULL));
                CCLog("Moving to this position %f and %f", it->Tile.x,it->Tile.y);
            }
            
             */
            
            /*
            for (std::list<node_block>::iterator it=path.end(); it != path.begin(); it--){
                CCPoint tilecord = ccp(it->Tile.x,it->Tile.y);
                CCLog("Moving to this position %f and %f", it->Tile.x,it->Tile.y);
                CCDelayTime* delayAction = CCDelayTime::create(50.0f);
                CCSprite* sprite = CCSprite::create("tinyball.png");
                sprite->setVisible(false);
                sprite->setPosition(convertTile2iso(tilecord));
                this->runAction(CCSequence::create(delayAction, CCCallFuncO::create(this, callfuncO_selector(HelloWorld::setlateposition),sprite ),NULL));
                //this->setPlayerPosition(convertTile2iso(tilecord))zz;
            }
            
            auto it = path.begin();
            CCLog("Moving to this position %f and %f", it->Tile.x,it->Tile.y);
            //this->setPlayerPosition(convertTile2iso(it->Tile));
             */
            if(!path_tiles.empty()) {
                this->schedule( schedule_selector(HelloWorld::setlateposition), 1.0f );
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
            isoTo2D(tap);
            int collision = check_collision(tap, bldng);
            CCLog("I am clicking here %f and %f ",tap.x,tap.y);
            tap = isoTo2D(tap);
            CCLog("I am clicking here in 2d %f and %f ",tap.x,tap.y);
            tap = convert2Tilecord(tap);//ToTiled(tap);
            CCLog("I am clicking here in tiled %f and %f ",tap.x,tap.y);
            
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


bool HelloWorld::check_collision_sprites (CCSprite*sprite , CCSprite*sprite2) {
    
    
    CCRect SpriteRect2 = CCRectMake(
                                   sprite2->getPosition().x - (sprite2->getContentSize().width/2),
                                   sprite2->getPosition().y - (sprite2->getContentSize().height/2),
                                   sprite2->getContentSize().width,
                                   sprite2->getContentSize().height);
    
    CCRect SpriteRect = CCRectMake(
                                   sprite->getPosition().x - (sprite->getContentSize().width/2),
                                   sprite->getPosition().y - (sprite->getContentSize().height/2),
                                   sprite->getContentSize().width,
                                   sprite->getContentSize().height);
    
    
    if (SpriteRect2.intersectsRect(SpriteRect))
        return true;
return false;
}
/*
CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = (((_tileMap->getMapSize().height) * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}

CCPoint HelloWorld::convertTiletoPosition(CCPoint tile) {
    int x = tile.x * _tileMap->getTileSize().width;
    int y = (_tileMap->getMapSize().height) * _tileMap->getTileSize().height - tile.y * _tileMap->getTileSize().height;
    return ccp(x,y);
}

*/

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
    CCPoint tileCoord = this->isoToTiled(position);
    tileCoord.x = (int)tileCoord.x;
    tileCoord.y = (int)tileCoord.y;
    //CCLog("Tile cord in x is %f",tileCoord.x);
    //CCLog("Tile cord in y is %f",tileCoord.y);
    
    /*
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
    */
    /*
    GameSprite*tempsprite = dynamic_cast<GameSprite*>(_background->tileAt(tileCoord));
    CCSprite * temp = _background->tileAt(tileCoord);
    
    if (tempsprite && tempsprite->getCollidable() == true) {
        CCLog("Collidable is true");
        return;
    }
    
    if(check_collision_sprites(temp, player))// && check_collision_sprites(temp, bldng)) {
        CCLog("yeh cheeez");
    
    
    int a = check_collision(position, bldng);
    /*
    auto temp_pos = isoTo2D(position);
    temp_pos.x  = temp_pos.x - player ->getContentSize().width/2;
    temp_pos.y = temp_pos.y - player->getContentSize().height/2;
    */
    if(tileCoord.x >= 0 && tileCoord.y >=0 && tileCoord.x < _background->getLayerSize().height && tileCoord.y < _background->getLayerSize().width ) {
        int x = tileCoord.x;
        int y = tileCoord.y;
        if(!tiles_vec[x][y].blocked) {
    player->setPosition(position);
    ball->setPosition(position);
    //CCLog("Ball is set at this isometric position %f and %f",position.x,position.y);
    }
    }
    //player->setPosition(ccp(position.x - player->getContentSize().width/2, position.y + player->getContentSize().height/1 ));
    
    
}


CCPoint HelloWorld::isoTo2D(CCPoint pt){
    CCPoint tempPt = ccp(0,0);
    tempPt.x = (2 * pt.y + pt.x) / 2;
    tempPt.y = (2 * pt.y - pt.x) / 2;
    return(tempPt);
}


CCPoint HelloWorld::isoToTiled(CCPoint pt) {
    CCPoint tempPt = isoTo2D(pt);
    return convert2Tilecord(tempPt);
}

CCPoint HelloWorld::convert2Tilecord(CCPoint position) {
    
    //Dont use tilemap to find width of the tile or the height.
    CCPoint top_right = _background->tileAt(ccp(0,0))->getPosition();
    CCPoint top_left = _background->tileAt(ccp(0,_background->getLayerSize().width - 1)) ->getPosition();
    top_left = isoTo2D(top_left);
    top_right =  isoTo2D(top_right);
   
    
    int diff_x = abs(top_right.x-top_left.x);
   // CCLog("Diff between x is %d", diff_x);
    
    CCPoint bottom_right = _background->tileAt(ccp(_background->getLayerSize().width - 1,0)) ->getPosition();
    bottom_right = isoTo2D(bottom_right);
    //CCLog("Diff between right is %f" , top_right.y - bottom_right.y);
    
    int tilex = (position.y - (bottom_right.y - height_tile/2))/height_tile;
    tilex = _background->getLayerSize().height - 1 - tilex;

    int tiley = (position.x - (top_left.x - width_tile/2))/width_tile;
    //CCLog("Dividing this %f by this %d ",position.x - (top_left.x - width_tile/2),width_tile);

    
    tiley = _background->getLayerSize().width -1 - tiley;
    
   // CCLog("Tile position in x and y is %d and %d",tilex,tiley);
    tiley = tiley + 1;
    return ccp(tilex,tiley);
}

CCPoint HelloWorld::convertTile2iso(CCPoint pos) {
    int x = pos.x;
    int y = pos.y;
    Ctile newtile = tiles_vec[x][y];
    auto temp_pos = isoTo2D(newtile.sprite->getPosition());
    temp_pos.x = temp_pos.x + width_tile;
    return twoDToIso(temp_pos);
}


CCPoint HelloWorld::twoDToIso(CCPoint pt){
    CCPoint tempPt = ccp(0,0);
    tempPt.x = pt.x - pt.y;
    tempPt.y = (pt.x + pt.y) / 2;
    return(tempPt);
}

std::stack<node_block> HelloWorld::bfs(CCPoint start , CCPoint goal) {
    std::stack<node_block> path_temp, path;
    std::list<node_block> open_check_list;
    node_block node,goal_node;
    node.Parent = start;
    node.Tile =  start;
    //path_temp.push(node);
    openlist.push(node);
    
    if (start.x == goal.x && start.y == goal.y) {
        
        return path_temp;
    }
    
    while (!openlist.empty()) {
        
        node_block curnode = openlist.front();
        closedlist.push_back(curnode);
        openlist.pop();
        CCLog("Removing this tile %f and %f",curnode.Tile.x , curnode.Tile.y );
        CCPoint leftpos = ccp(curnode.Tile.x,curnode.Tile.y + 1);
        CCPoint rightpos = ccp(curnode.Tile.x,curnode.Tile.y - 1);
        CCPoint downpos = ccp(curnode.Tile.x + 1, curnode.Tile.y);
        CCPoint uppos = ccp(curnode.Tile.x - 1, curnode.Tile.y);
        
        if(curnode.Tile.x == goal.x && curnode.Tile.y == goal.y) {
            goal_node = curnode;
            CCLog("Goal Found");
            break;
        }
        
        
        if(valid_tile_move(leftpos)) {
            node.Tile = leftpos;
            node.Parent = curnode.Tile;
            
            if((!searchClosedList(node, closedlist)) &&(!searchClosedList(node, open_check_list))) {
            
            openlist.push(node);
            open_check_list.push_back(node);
            CCLog(" Adding this tile %f and %f",node.Tile.x , node.Tile.y );
            }
        }
        
        
        if(valid_tile_move(rightpos)) {
            node.Tile = rightpos;
            node.Parent = curnode.Tile;
            if((!searchClosedList(node, closedlist)) &&(!searchClosedList(node, open_check_list))) {
                
                openlist.push(node);
                open_check_list.push_back(node);
                CCLog(" Adding this tile %f and %f",node.Tile.x , node.Tile.y );
            }        }
        
        
        if(valid_tile_move(downpos)) {
            node.Tile = downpos;
            node.Parent = curnode.Tile;
            
            if((!searchClosedList(node, closedlist)) &&(!searchClosedList(node, open_check_list))) {
                
                openlist.push(node);
                open_check_list.push_back(node);
                CCLog(" Adding this tile %f and %f",node.Tile.x , node.Tile.y );
            }
        }
        
        
        if(valid_tile_move(uppos)) {
            node.Tile = uppos;
            node.Parent = curnode.Tile;
            
            if((!searchClosedList(node, closedlist)) &&(!searchClosedList(node, open_check_list))) {
                
                openlist.push(node);
                open_check_list.push_back(node);
                CCLog(" Adding this tile %f and %f",node.Tile.x , node.Tile.y );
            }
        }
        
        
        
    }
    
    int x_start,y_start;
    x_start = start.x;
    y_start = start.y;
    CCLog("The start position of the bfs is %d and %d ",x_start , y_start);
    ///Found the goal

    if(goal_node.Tile.x == goal.x && goal_node.Tile.y == goal.y) {
        int x, y;
        x = goal_node.Parent.x;
        y = goal_node.Parent.y;
        x_start = start.x;
        y_start =  start.y;
        node_block newnode,node;
        while (x != x_start || y!=y_start) {
            
            newnode.Tile.x = x;
            newnode.Tile.y = y;
            newnode.Parent.x = x;
            newnode.Parent.y = y;
            path.push(newnode);
            node = findtileClosedList(newnode, closedlist);
            x = node.Parent.x;
            y = node.Parent.y;
            
            CCLog("Path contains these tiles %f and %f ", newnode.Tile.x , newnode.Tile.y);
        
            
        }
        
        node_block node_start;
        node_start.Tile.x =  start.x;
        node_start.Tile.y = start.y;
        node_start.Parent.x =  start.x;
        node_start.Parent.y = start.y;
        path.push(node_start);
        return path;
}
    
    
    return path;
    
}



bool HelloWorld::valid_tile_move(CCPoint tile) {
    
    if(tile.x >= 0 && tile.y >= 0  && tile.y < _background->getLayerSize().width && tile.x < _background->getLayerSize().height ) {
        int x = tile.x;
        int y = tile.y;
        if(!tiles_vec[x][y].blocked)
        return true;
    }
    
    return false;
    
}


bool HelloWorld::searchClosedList (node_block blk, std::list<node_block> mylist) {
    
    for (std::list<node_block>::iterator it=mylist.begin(); it != mylist.end(); ++it) {
        if (it->Tile.x == blk.Tile.x && it->Tile.y == blk.Tile.y) {
            return true;
        }
        
        
    }
    
    return false;
    
    
}



node_block HelloWorld::findtileClosedList (node_block blk, std::list<node_block> mylist) {
    
    
    for (std::list<node_block>::iterator it=mylist.begin(); it != mylist.end(); ++it) {
        if (it->Tile.x == blk.Tile.x && it->Tile.y == blk.Tile.y) {
            node_block node = *it;
            return node;
        }
        
        
    }

    
    
}


void HelloWorld::setlateposition(cocos2d::CCObject* pSender) {
    
    if (path_tiles.empty()) {
        
        this->unschedule( schedule_selector(HelloWorld::setlateposition));
        return;
    }
    
    if(valid_tile_move(path_tiles.top().Tile)) {
        CCPoint tilecord = path_tiles.top().Tile;
        CCFiniteTimeAction* actionMove =
        CCMoveTo::create( (float)0.45,convertTile2iso(path_tiles.top().Tile) );
        path_tiles.pop();
        ball->runAction(actionMove);
        auto sprite = _background->tileAt(tilecord);
        sprite->setColor(ccc3(0,0,255));
        
    }
    
                //this->setPlayerPosition(convertTile2iso(path_tiles.top().Tile));
    
  
  
}

