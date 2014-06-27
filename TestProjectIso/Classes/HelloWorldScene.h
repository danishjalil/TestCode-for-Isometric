#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "GameSprite.h"
#include <vector.h>
#include <queue>
#include <stack>
#include <list.h>

USING_NS_CC;
using namespace extension;

struct node_block {
    CCPoint Parent;
    CCPoint Tile;
};

class HelloWorld : public cocos2d::CCLayer
{
private:
    bool make_building = false;
    CCSprite * bldng;
    CCTMXTiledMap *_tileMap;
    CCTMXLayer *_background;
    CCTMXLayer *_tree;
    CCPoint tileCoordForPosition(CCPoint position);
    CCPoint convert2Tilecord(CCPoint position);
    CCPoint convertTiletoPosition (CCPoint tilepos);
    bool check_collision_sprites (CCSprite*sprite , CCSprite*sprite2);
    struct Ctile{
        CCSprite *sprite;
        bool blocked;
    };
  
    
    std::queue<node_block> openlist;
    std::list<node_block> closedlist;
    std::vector <std::vector<Ctile> >tiles_vec;
    std::stack<node_block> path_tiles;
    CCPoint isoTo2D(CCPoint pt);
    CCPoint isoToTiled(CCPoint pt);
    int width_tile;
    int height_tile;
    std::stack<node_block> bfs(CCPoint start , CCPoint goal);
    bool valid_tile_move( CCPoint tile );
    bool searchClosedList (node_block blk, std::list<node_block> l);
    node_block findtileClosedList (node_block blk, std::list<node_block> mylist);
    CCPoint convertTile2iso(CCPoint pos);
    CCPoint twoDToIso(CCPoint pt);
    cocos2d::CCSpriteBatchNode * _batchNode;
    CCSprite*ball;
    bool searchOpenList (node_block blk , std::queue<node_block> mylist);
    
    
	cocos2d::CCSprite * _ship;
    CCSprite *_spacedust1;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    float content_size_width;
    void update(float dt);
    CCArray* _asteroids;
    int _nextAsteroid;
    float _nextAsteroidSpawn;
    CCSprite * player;
    CCArray* _shipLasers;
    int _nextShipLaser;
    int _lives;
    float u = 0;
    float v = 0;
    float gravity = -50;
    float fire_time = 0.5;
    float destination_x = 0;
    float destination_y = 0;
    typedef enum {
        KENDREASONWIN,
        KENDREASONLOSE
    } EndReason;
    double _gameOverTime;
    bool _gameOver;
    void endScene(EndReason endReason);
    void restartTapped();
    
    
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void menuStartCallback(cocos2d::CCObject* pSender);
    void buttonhandler(cocos2d::CCObject* pSender);
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved	(CCSet *pTouches,CCEvent *pEvent);
    int check_collision(CCPoint cord, CCSprite*sprite);
    void setPlayerPosition(CCPoint position);
    void setlateposition(cocos2d::CCObject* pSender);

  
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
  //  void menuCloseCallback(CCObject* pSender);
  //  void UpButtonClicked(CCObject * sender , CCControlEvent * controlEvent);
   // void DownButtonClicked(CCObject * sender , CCControlEvent * controlEvent);
   // void touchUpButton();
   // float randomValueBetween(float low, float high);
   // void setInvisible(CCNode * node);
   // float getTimeTick();
   // void jumpAction();
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
