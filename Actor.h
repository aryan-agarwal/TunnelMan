#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
#include <queue>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//================================================GlobalConstants================================================

const int STABLE_BOULDER_STATE = 0;
const int WAITING_BOULDER_STATE = 1;
const int FALLING_BOULDER_STATE = 2;

const int PICKUP_ABLE_TUNNELMAN = 0;
const int PICKUP_ABLE_PROTESTOR = 1;

const bool TEMP_GOLD = true;
const bool PERMANENT_GOLD = false;

const int PROTESTOR_LEAVE_STATE = 0;
const int PROTESTOR_STAY_STATE = 1;
const int PROTESTOR_REST_STATE = 2;

const int REG_PROT_ID = 0;
const int HARD_PROT_ID = 1;
const int GOODIE_ID = 2;
const int BOULDER_ID = 3;
const int OIL_ID = 4;
const int SQUIRT_ID = 5;
const int TM_ID = 6;

//================================================Actor================================================

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    
    void setDroppedGoldToTrue();
    StudentWorld* getWorld();
    bool isAlive();
    void setAlive(bool curStatus);
    bool isTunnelManWithinDistance(int x, int y, int dist);
    virtual void doSomething() = 0;
    virtual ~Actor();
    bool getVisibility();
    void setVisibility(bool v);
    int getIdentity();
    void setIdentity(int ID);

private:
    Direction m_dir;
    StudentWorld* m_StudentWorld;
    bool status;
    bool gold;
    bool visible;
    int actorIdentity;
};

//================================================TunnelMan================================================

class TunnelMan: public Actor
{
public:
    TunnelMan(StudentWorld* world, int imageID = TID_PLAYER, int startX = 30, int startY = 60, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    void incrementGoldNuggets();
    void incrementSonarKits();
    int getHitPoints();
    void decrementHitPoints();
    void incrementWaterBy5();
    int getUnitsOfWater();
    int getSonarCharge();
    int getGoldNuggets();
    void removeEarth(int x, int y);
    virtual void doSomething();
    virtual ~TunnelMan();
    
private:
    int hitPoints;
    int unitsOfWater;
    int sonarCharge;
    int goldNuggets;
};

//================================================Earth================================================

class Earth: public GraphObject
{
public:
    Earth(StudentWorld* world, int imageID = TID_EARTH, int startX = 0, int startY = 0, Direction dir = right, double size = 0.25, unsigned int depth = 3);
    virtual void doSomething();
    bool getVisibility();
    void setVisibility(bool v);
    virtual ~Earth();

private:
    bool visible;
};

//================================================SquirtsOfWater================================================

class SquirtsOfWater: public Actor
{
public:
    SquirtsOfWater(StudentWorld* world, int startX, int startY, Direction dir, int imageID = TID_WATER_SPURT, double size = 1.0, unsigned int depth = 1);
    virtual void doSomething();
    virtual ~SquirtsOfWater();
private:
    int distanceTravelled;
};

//================================================Boulders================================================

class Boulders: public Actor
{
public:
    Boulders(StudentWorld* world, int startX, int startY, Direction dir = down, double size = 1.0, unsigned int depth = 1);
    int getBoulderState();
    void setBoulderState(int state);
    bool isEarthBelow();
    virtual void doSomething();
    virtual ~Boulders();
    
private:
    int boulderState;
    int ticks;
};

//================================================Goodies================================================

class Goodies: public Actor
{
public:
    Goodies(TunnelMan* player, StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2);
    void decrementTicksLeft();
    void setTicksLeft(int ticks);
    int getTicksLeft();
    virtual void doSomething();
    virtual ~Goodies();
    
private:
    int ticksLeft;
};

//================================================BarrelsOfOil================================================

class BarrelsOfOil: public Goodies
{
public:
    BarrelsOfOil(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID = TID_BARREL, Direction dir = right, double size = 1.0, unsigned int depth = 2);
    virtual void doSomething();
    virtual ~BarrelsOfOil();
    
private:
};

//================================================GoldNuggets================================================

class GoldNuggets: public Goodies
{
public:
    GoldNuggets(TunnelMan* player, StudentWorld* world, int startX, int startY, bool goldState, int imageID = TID_GOLD, Direction dir = right, double size = 1.0, unsigned int depth = 2);
    void setPickupAbleByProtestor(bool pickup);
    bool getPickupAbleByProtestor();
    void setPickupAbleByTM(bool pickup);
    bool getPickupAbleByTM();
    virtual void doSomething();
    virtual ~GoldNuggets();
    
private:
    bool state;
    bool pickupAbleByTM;
    bool pickupAbleByProtestor;
};

//================================================SonarKits================================================

class SonarKits: public Goodies
{
public:
    SonarKits(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID = TID_SONAR, Direction dir = right, double size = 1.0, unsigned int depth = 2);
    virtual void doSomething();
    virtual ~SonarKits();
    
private:
};

//================================================WaterRefills================================================

class WaterRefills: public Goodies
{
public:
    WaterRefills(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID = TID_WATER_POOL, Direction dir = right, double size = 1.0, unsigned int depth = 2);
    virtual void doSomething();
    virtual ~WaterRefills();
    
private:
};

//================================================Protesters================================================

class Protesters: public Actor
{
public:
    Protesters(StudentWorld* world, bool isHardcore  = false, int imageID = TID_PROTESTER, int startX = 60, int startY = 60, Direction dir = left, double size = 1.0, unsigned int depth = 0);
    
    int getNumSquaresToMoveInCurrentDirection();
    void resetNumSquaresToMoveInCurrentDirection();
    void decrementNumSquaresToMoveInCurrentDirection();
    void setNumSquaresToMoveInCurrentDirection();
        
    void setLeaveTheOilField();
    bool getLeaveTheOilField();
    
    virtual void doSomething();
    
    int getTicksToWaitBetweenMoves();
    void resetTicksToWaitBetweenMoves();
    void decrementTicksToWaitBetweenMoves();
    
    void increaseTicksToWaitBetweenMoves();
    
    bool isFacingTunnelMan();
    
    int getTicksSinceLastShout();
    void resetTicksSinceLastShout();
    void incrementTicksSinceLastShout();
    
    bool checkHorizontalLineOfSight();
    bool checkVerticalLineOfSight();
    
    int getTicksPerp();
    void resetTicksPerp();
    void decrementTicksPerp();
    
    bool isBribed();
    void setBribed(bool b);
    bool isProtHardcore();
    void setProtHardcore(bool hc);
    int getHitPointsForProt();
    void setHitPointsForProt(int p = 5);
    void decrementHitPointsForProt();
    bool getStunned();
    void setStunned(bool s);
    virtual ~Protesters();

private:
    int numSquaresToMoveInCurrentDirection;
    int protesterHitPoints;
    bool leaveTheOilField;
    int ticksToWaitBetweenMoves;
    int ticksSinceLastShout;
    int ticksPerp;
    bool bribe;
    bool hardcore;
    bool stunned;
};

//================================================RegularProtesters================================================

class RegularProtester: public Protesters
{
public:
    RegularProtester(StudentWorld* world);
    virtual ~RegularProtester();
private:
};

//================================================HardcoreProtesters================================================

class HardcoreProtester: public Protesters
{
public:
    HardcoreProtester(StudentWorld* world);
    virtual ~HardcoreProtester();
private:
};

//====================================================================================================================

#endif // ACTOR_H_
