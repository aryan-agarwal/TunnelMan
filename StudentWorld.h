#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include <vector>
const int AID_TUNNEL_MAN = 0;
const int AID_EARTH = 1;
const int AID_REGULAR_PROTESTOR = 2;
const int AID_HARDCORE_PROTESTOR = 3;
const int AID_BOULDERS = 4;
const int AID_GOLD = 5;
const int AID_EMPTY = 6;
const int AID_SQUIRT = 7;
const int AID_OIL = 8;
const int DIR_RIGHT = 0;
const int DIR_LEFT = 1;
const int DIR_UP = 2;
const int DIR_DOWN = 3;

class Actor;
class TunnelMan;
class Earth;
class Protesters;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void addActor(Actor* act);
    
    int getActorID(int x, int y); // Only to check if empty
    void setActorID(int x, int y, int ID);
    
    TunnelMan* getTunnelMan();
    std::vector<Actor*> getActor();
    Earth* getEarth(int x, int y);
    
    void addGoodies();
    bool checkIfGoodieCanBePlaced(int x, int y);
    void useSonar(int x, int y);
    bool canBePlaced(int x, int y); // For the barrels and gold and boulders
    void decreaseBarrelCount();
    int getBarrelsOfOil();
    
    void addProtestors();
    void shootProtester(int x, int y);
    void dropBoulderOnProtester(int x, int y);
    void bribeProtester(int x, int y);
    bool isProtesterWithinDist(int x, int y, int dist);
        
    bool isEarthPresent(int x, int y);
    bool isBoulderPresent(int x, int y);
    bool canMove(int x, int y, int dir);
    
    void setDisplayText();
    
    virtual ~StudentWorld();
    
private:
    std::vector<Actor*> m_actors;
    std::vector<Protesters*> m_protesters;
    Earth* m_Earth[VIEW_WIDTH][VIEW_HEIGHT];
    TunnelMan* m_TunnelMan;
    int actorID[VIEW_WIDTH][VIEW_HEIGHT];
    int barrelsOfOil;
    int nProtestors;
    int protestorTicks;
    int ticks;
};

#endif // STUDENTWORLD_H_
