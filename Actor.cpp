#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================Actor================================================

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    :GraphObject(imageID, startX, startY, dir), m_StudentWorld(world), status(false), gold(false)
{
    setVisibility(true);
    setVisible(true);
    setAlive(true);
}

void Actor::setDroppedGoldToTrue()
{
    gold = true;
}

StudentWorld* Actor::getWorld()
{
    return m_StudentWorld;
}

bool Actor::isAlive()
{
    return status;
}

void Actor::setAlive(bool curStatus)
{
    status = curStatus;
}

bool Actor::getVisibility()
{
    return visible;
}

void Actor::setVisibility(bool v)
{
    visible = v;
}

int Actor::getIdentity()
{
    return actorIdentity;
}

void Actor::setIdentity(int ID)
{
    actorIdentity = ID;
}

bool Actor::isTunnelManWithinDistance(int x, int y, int dist)
{
    int xPosTM = getWorld() -> getTunnelMan() -> getX();
    int yPosTM = getWorld() -> getTunnelMan() -> getY();
    int xCoord = x - xPosTM;
    int yCoord = y - yPosTM;
    int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
    
    if(radiusSq <= (dist*dist))
        return true;
    
    return false;
}

Actor::~Actor()
{}

//================================================TunnelMan================================================

TunnelMan::TunnelMan(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    :Actor(world, imageID, startX, startY), hitPoints(10), unitsOfWater(5), sonarCharge(1), goldNuggets(0)
{
    setVisibility(true);
    setVisible(true);
    setAlive(true);
    setIdentity(TM_ID);
}

int TunnelMan::getHitPoints()
{
    return hitPoints;
}

void TunnelMan::incrementSonarKits()
{
    sonarCharge += 2;
}

void TunnelMan::incrementWaterBy5()
{
    unitsOfWater += 5;
}

int TunnelMan::getUnitsOfWater()
{
    return unitsOfWater;
}

int TunnelMan::getSonarCharge()
{
    return sonarCharge;
}

int TunnelMan::getGoldNuggets()
{
    return goldNuggets;
}

void TunnelMan::incrementGoldNuggets()
{
    goldNuggets++;
}

void TunnelMan::decrementHitPoints()
{
    hitPoints -= 2;
}

void TunnelMan::doSomething()
{
    int ch;
    if(getWorld() -> getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
            case 'a':
            case 4:
            {
                if(getDirection() != left)
                {
                    setDirection(left);
                    break;
                }

                if((getX() - 1) >= 0)
                {
                    if(getWorld() -> isBoulderPresent(getX() - 1, getY()) == false)
                        moveTo(getX() - 1, getY());
                    
                    bool sound = false;
                    for(int i = 0; i < 4; i++)
                    {
                        if(getWorld() -> getEarth(getX(), i + getY()) != nullptr && getWorld() -> getEarth(getX(), i + getY()) -> getVisibility() == true)
                        {
                            if(sound == false)
                            {
                                getWorld() -> playSound(SOUND_DIG);
                                sound = true;
                            }
                            removeEarth(getX(), i + getY());
                        }
                    }
                }
                break;
            }
                
            case KEY_PRESS_RIGHT:
            case 'd':
            case 6:
            {
                if(getDirection() != right)
                {
                    setDirection(right);
                    break;
                }
                
                if((getX() + 1) <= (VIEW_WIDTH - 4))
                {
                    if(getWorld() -> isBoulderPresent(getX() + 1, getY()) == false)
                        moveTo(getX() + 1, getY());
                    
                    bool sound = false;
                    for(int i = 0; i < 4; i++)
                    {
                        if(getWorld() -> getEarth(getX() + 3, i + getY()) != nullptr && getWorld() -> getEarth(getX() + 3, i + getY()) -> getVisibility() == true)
                        {
                            if(sound == false)
                            {
                                getWorld() -> playSound(SOUND_DIG);
                                sound = true;
                            }
                            removeEarth(getX() + 3, i + getY());
                        }
                    }
                }
                
                break;
            }
                
            case KEY_PRESS_UP:
            case 'w':
            case 8:
            {
                if(getDirection() != up)
                {
                    setDirection(up);
                    break;
                }
                
                if((getY() + 1) <= (VIEW_HEIGHT - 4))
                {
                    if(getWorld() -> isBoulderPresent(getX(), getY() + 1) == false)
                        moveTo(getX(), getY() + 1);
                    
                    bool sound = false;
                    for(int i = 0; i < 4; i++)
                    {
                        if(getWorld() -> getEarth(getX() + i, 3 + getY()) != nullptr && getWorld() -> getEarth(getX() + i, 3 + getY()) -> getVisibility() == true)
                        {
                            if(sound == false)
                            {
                                getWorld() -> playSound(SOUND_DIG);
                                sound = true;
                            }
                            removeEarth(getX() + i, 3 + getY());
                        }
                    }
                }
                
                break;
            }
                
            case KEY_PRESS_DOWN:
            case 's':
            case 2:
            {
                if(getDirection() != down)
                {
                    setDirection(down);
                    break;
                }
                
                if((getY() - 1) >= 0)
                {
                    if(getWorld() -> isBoulderPresent(getX(), getY() - 1) == false)
                        moveTo(getX(), getY() - 1);
                    
                    bool sound = false;
                    for(int i = 0; i < 4; i++)
                    {
                        if(getWorld() -> getEarth(getX() + i, getY()) != nullptr && getWorld() -> getEarth(getX() + i, getY()) -> getVisibility() == true)
                        {
                            if(sound == false)
                            {
                                getWorld() -> playSound(SOUND_DIG);
                                sound = true;
                            }
                            removeEarth(getX() + i, getY());
                        }
                    }
                }
                
                break;
            }
            
            case KEY_PRESS_ESCAPE:
            {
                setAlive(false);
                break;
            }
                
            case 'Z':
            case 'z':
            {
                if(sonarCharge >= 1)
                {
                    getWorld() -> playSound(SOUND_SONAR);
                    getWorld() -> useSonar(getX(), getY());
                    sonarCharge--;
                }
                break;
            }
                
            case KEY_PRESS_TAB:
            {
                if(goldNuggets >= 1)
                {
                    Actor* dropGold = new GoldNuggets(this, getWorld(), getX(), getY(), TEMP_GOLD, TID_GOLD, right, 1.0, 2);
                    getWorld() -> addActor(dropGold);
                    dropGold -> setDroppedGoldToTrue();
                    getWorld() -> setActorID(getX(), getY(), AID_GOLD);
                    goldNuggets--;
                }
                break;
            }

            case KEY_PRESS_SPACE:
            {
                if(unitsOfWater >= 1)
                {
                    getWorld() -> playSound(SOUND_PLAYER_SQUIRT);
                    unitsOfWater--;
                    bool canShoot = true;
                    SquirtsOfWater* squirtPtr;
                    
                    if(getDirection() == right)
                    {
                        squirtPtr = new SquirtsOfWater(getWorld(), getX() + 4, getY(), right, TID_WATER_SPURT);
                        for(int i = 0; i < 4; i++)
                        {
                            if(getWorld() -> canMove(getX() + i, getY(), DIR_RIGHT) == false)
                                canShoot = false;
                        }
                    }
                    else if(getDirection() == left)
                    {
                        squirtPtr = new SquirtsOfWater(getWorld(), getX() - 4, getY(), left, TID_WATER_SPURT);
                        for(int i = 0; i < 4; i++)
                        {
                            if(getWorld() -> canMove(getX() - i, getY(), DIR_LEFT) == false)
                                canShoot = false;
                        }
                    }
                    else if(getDirection() == up)
                    {
                        squirtPtr = new SquirtsOfWater(getWorld(), getX(), getY() + 4, up, TID_WATER_SPURT);
                        for(int i = 0; i < 4; i++)
                        {
                            if(getWorld() -> canMove(getX(), getY() + i, DIR_UP) == false)
                                canShoot = false;
                        }
                    }
                    else
                    {
                        squirtPtr = new SquirtsOfWater(getWorld(), getX(), getY() - 4, down, TID_WATER_SPURT);
                        for(int i = 0; i < 4; i++)
                        {
                            if(getWorld() -> canMove(getX(), getY() - i, DIR_DOWN) == false)
                                canShoot = false;
                        }
                    }
   
                    if(canShoot == false)
                    {
                        squirtPtr -> setAlive(false);
                        squirtPtr -> setVisible(false);
                        squirtPtr -> setVisibility(true);
                        getWorld() -> setActorID(squirtPtr -> getX(), squirtPtr -> getY(), AID_SQUIRT);
                        return;
                    }
                    
                    getWorld() -> addActor(squirtPtr);
                }
                break;
            }
                
            default:
                break;
        }
    }
}

void TunnelMan::removeEarth(int x, int y)
{
    if(getWorld() -> getEarth(x, y) -> getVisibility() == true)
    {
        getWorld() -> getEarth(x, y) -> setVisible(false);
        getWorld() -> getEarth(x, y) -> setVisibility(false);
        getWorld() -> setActorID(x, y, AID_EMPTY);
    }
}

TunnelMan::~TunnelMan()
{}

//================================================Earth================================================

Earth::Earth(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    :GraphObject(imageID, startX, startY, dir, size, depth)
{
    setVisible(true);
    setVisibility(true);
}

void Earth::doSomething()
{}

Earth::~Earth()
{}

bool Earth::getVisibility()
{
    return visible;
}

void Earth::setVisibility(bool v)
{
    visible = v;
}

//================================================SquirtsOfWater================================================

SquirtsOfWater::SquirtsOfWater(StudentWorld* world, int startX, int startY, Direction dir, int imageID, double size, unsigned int depth)
    :Actor(world, imageID, startX, startY, dir, size, depth), distanceTravelled(0)
{
    setVisible(true);
    setVisibility(true);
    setAlive(true);
    setIdentity(SQUIRT_ID);
}

void SquirtsOfWater::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld() -> isProtesterWithinDist(getX(), getY(), 3) == true)
    {
        getWorld() -> shootProtester(getX(), getY());
        getWorld() -> playSound(SOUND_PROTESTER_ANNOYED);
        setAlive(false);
        setVisibility(false);
        setVisible(false);
        return;
    }
    
    distanceTravelled++;
    
    if(distanceTravelled >= 4)
    {
        setAlive(false);
        setVisibility(false);
        setVisible(false);
    }
    
    if(getDirection() == right)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT))
            moveTo(getX() + 1, getY());
    }
    if(getDirection() == left)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_LEFT))
            moveTo(getX() - 1, getY());
    }
    if(getDirection() == up)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_UP))
            moveTo(getX(), getY() + 1);
    }
    if(getDirection() == down)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_DOWN))
            moveTo(getX(), getY() - 1);
    }
}

SquirtsOfWater::~SquirtsOfWater()
{}


//================================================Boulders================================================

Boulders::Boulders(StudentWorld* world, int startX, int startY, Direction dir, double size, unsigned int depth)
    :Actor(world, TID_BOULDER, startX, startY, dir), boulderState(STABLE_BOULDER_STATE), ticks(30)
{
    setVisible(true);
    setVisibility(true);
    setAlive(true);
    setIdentity(BOULDER_ID);
}

void Boulders::doSomething()
{
    if(!isAlive())
        return;
    
    if(getBoulderState() == STABLE_BOULDER_STATE)
    {
        if(isEarthBelow() == false)
            setBoulderState(WAITING_BOULDER_STATE);
    }
    
    else if(getBoulderState() == WAITING_BOULDER_STATE)
    {
        ticks--;
        if(ticks <= 0)
        {
            setBoulderState(FALLING_BOULDER_STATE);
            getWorld() -> playSound(SOUND_FALLING_ROCK);
        }
    }
    
    else if(getBoulderState() == FALLING_BOULDER_STATE)
    {
        if(getWorld() -> getTunnelMan() -> isTunnelManWithinDistance(getX(), getY(), 3) == true)
        {
            while(getWorld() -> getTunnelMan() -> getHitPoints() >= 0)
            {
                getWorld() -> getTunnelMan() -> decrementHitPoints();
            }
            getWorld() -> getTunnelMan() -> setAlive(false);
        }

        if(getWorld() -> isProtesterWithinDist(getX(), getY(), 3) == true)
        {
            getWorld() -> dropBoulderOnProtester(getX(), getY());
            setAlive(false);
            setVisibility(false);
            setVisible(false);
            return;
        }
        
        if(getY() != 0 && getWorld() -> isEarthPresent(getX(), getY() - 1) == false && getWorld() -> isEarthPresent(getX() + 1, getY() - 1) == false && getWorld() -> isEarthPresent(getX() + 2, getY() - 1) == false && getWorld() -> isEarthPresent(getX() + 3, getY() - 1) == false && getWorld() -> isBoulderPresent(getX(), getY() - 4) == false)
            moveTo(getX(), getY() - 1);
        else
            setAlive(false);
    }
}

bool Boulders::isEarthBelow()
{
    for(int i = 0; i < 4; i++)
    {
        if(getWorld() -> getActorID(getX() + i, getY() - 1) == AID_EARTH)
            return true;
    }
    return false;
}

int Boulders::getBoulderState()
{
    return boulderState;
}

void Boulders::setBoulderState(int state)
{
    boulderState = state;
}

Boulders::~Boulders()
{}

//================================================Goodies================================================

Goodies::Goodies(TunnelMan* player, StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    :Actor(world, imageID, startX, startY)
{
    setVisible(true);
    setVisibility(true);
    setIdentity(GOODIE_ID);
}
int Goodies::getTicksLeft()
{
    return ticksLeft;
}

void Goodies::decrementTicksLeft()
{
    ticksLeft--;
}

void Goodies::setTicksLeft(int ticks)
{
    ticksLeft = ticks;
}

void Goodies::doSomething()
{}

Goodies::~Goodies()
{}

//================================================BarrelsOfOil================================================

BarrelsOfOil::BarrelsOfOil(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth)
    :Goodies(player, world, TID_BARREL, startX, startY)
{
    setVisible(false);
    setVisibility(false);
    setAlive(true);
    setIdentity(OIL_ID);
}

void BarrelsOfOil::doSomething()
{
    if(!isAlive())
        return;
    
    if(isTunnelManWithinDistance(getX(), getY(), 4) && getVisibility() == false)
    {
        setVisible(true);
        setVisibility(true);
        return;
    }
    
    else if(isTunnelManWithinDistance(getX(), getY(), 3))
    {
        setAlive(false);
        getWorld() -> playSound(SOUND_FOUND_OIL);
        getWorld() -> increaseScore(1000);
        getWorld() -> decreaseBarrelCount();
    }
}

BarrelsOfOil::~BarrelsOfOil()
{}

//================================================WaterRefills================================================

WaterRefills::WaterRefills(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth)
    :Goodies(player, world, TID_WATER_POOL, startX, startY)
{
    setVisible(true);
    setVisibility(true);
    setAlive(true);
    int ticksLeftWR = fmax(100, 300 - (10 * getWorld() -> getLevel()));
    setTicksLeft(ticksLeftWR);
}

void WaterRefills::doSomething()
{
    decrementTicksLeft();
    
    if(!isAlive())
        return;
    
    if(isTunnelManWithinDistance(getX(), getY(), 3))
    {
        setAlive(false);
        getWorld() -> playSound(SOUND_GOT_GOODIE);
        getWorld() -> getTunnelMan() -> incrementWaterBy5();
        getWorld() -> increaseScore(100);
        return;
    }
    
    if(getTicksLeft() <= 0)
        setAlive(false);
}

WaterRefills::~WaterRefills()
{}

//================================================SonarKits================================================

SonarKits::SonarKits(TunnelMan* player, StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth)
    :Goodies(player, world, TID_SONAR, startX, startY)
{
    setVisible(true);
    setVisibility(true);
    setAlive(true);
    int ticksLeftSK = fmax(100, 300 - (10 * getWorld() -> getLevel()));
    setTicksLeft(ticksLeftSK);
}

void SonarKits::doSomething()
{
    decrementTicksLeft();
    
    if(!isAlive())
    return;

    if(isTunnelManWithinDistance(getX(), getY(), 3))
        {
            setAlive(false);
            getWorld() -> playSound(SOUND_GOT_GOODIE);
            getWorld() -> getTunnelMan() -> incrementSonarKits();
            getWorld() -> increaseScore(75);
            return;
        }
        
        if(getTicksLeft() <= 0)
            setAlive(false);
}

SonarKits::~SonarKits()
{}

//================================================GoldNuggets================================================

GoldNuggets::GoldNuggets(TunnelMan* player, StudentWorld* world, int startX, int startY, bool goldState, int imageID, Direction dir, double size, unsigned int depth)
    :Goodies(player, world, TID_GOLD, startX, startY), state(goldState)
{
    setAlive(true);
    setVisible(goldState);
    setVisibility(goldState);
    setPickupAbleByTM(!goldState);
    setPickupAbleByProtestor(goldState);
    setTicksLeft(100);
}

void GoldNuggets::doSomething()
{
    decrementTicksLeft();
    
    if(!isAlive())
    return;
    
    if(getPickupAbleByTM() == true && state == PERMANENT_GOLD && getVisibility() == false && isTunnelManWithinDistance(getX(), getY(), 4))
    {
        setVisible(true);
        setVisibility(true);
        return;
    }
    
    else if(getPickupAbleByTM() == true && state == PERMANENT_GOLD && isTunnelManWithinDistance(getX(), getY(), 3))
    {
        setAlive(false);
        getWorld() -> playSound(SOUND_GOT_GOODIE);
        getWorld() -> increaseScore(25);
        getWorld() -> getTunnelMan() -> incrementGoldNuggets();
    }
    
    else if(getPickupAbleByProtestor() == true && state == TEMP_GOLD && getWorld() -> isProtesterWithinDist(getX(), getY(), 3))
    {
        setAlive(false);
        setVisible(false);
        setVisibility(false);
        getWorld() -> bribeProtester(getX(), getY());
    }
    
    if(getPickupAbleByProtestor() == true && state == TEMP_GOLD && getTicksLeft() <= 0)
    {
        setAlive(false);
        setVisible(false);
        setVisibility(false);
    }
}

void GoldNuggets::setPickupAbleByProtestor(bool pickup)
{
    pickupAbleByProtestor = pickup;
}

bool GoldNuggets::getPickupAbleByProtestor()
{
    return pickupAbleByProtestor;
}

void GoldNuggets::setPickupAbleByTM(bool pickup)
{
    pickupAbleByTM = pickup;
}

bool GoldNuggets::getPickupAbleByTM()
{
    return pickupAbleByTM;
}

GoldNuggets::~GoldNuggets()
{}

//================================================Protesters================================================

Protesters::Protesters(StudentWorld* world, bool isHardcore, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    :Actor(world, imageID, startX, startY, dir, 1.0, 0)
{
    setVisible(true);
    setVisibility(true);
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
    if(isHardcore == false)
    {
        protesterHitPoints = 5;
        setHitPointsForProt(5);
    }
    else
    {
        protesterHitPoints = 20;
        setHitPointsForProt(20);

    }
    leaveTheOilField = false;
    ticksToWaitBetweenMoves = fmax(0, 3 - (getWorld() -> getLevel())/4);
    ticksSinceLastShout = 0;
    ticksPerp = 200;
    setIdentity(REG_PROT_ID);
    setStunned(false);
    setBribed(false);
    hardcore = isHardcore;
}

int Protesters::getNumSquaresToMoveInCurrentDirection()
{
    return numSquaresToMoveInCurrentDirection;
}

void Protesters::resetNumSquaresToMoveInCurrentDirection()
{
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}

void Protesters::decrementNumSquaresToMoveInCurrentDirection()
{
    numSquaresToMoveInCurrentDirection--;
}

void Protesters::setNumSquaresToMoveInCurrentDirection()
{
    numSquaresToMoveInCurrentDirection = 0;
}

void Protesters::setLeaveTheOilField()
{
    leaveTheOilField = true;
}

bool Protesters::getLeaveTheOilField()
{
    return leaveTheOilField;
}

int Protesters::getTicksToWaitBetweenMoves()
{
    return ticksToWaitBetweenMoves;
}

void Protesters::resetTicksToWaitBetweenMoves()
{
    ticksToWaitBetweenMoves = fmax(0, 3 - (getWorld() -> getLevel())/4);
}

void Protesters::decrementTicksToWaitBetweenMoves()
{
    ticksToWaitBetweenMoves--;
}

void Protesters::increaseTicksToWaitBetweenMoves()
{
    ticksToWaitBetweenMoves += fmax(50, 100 - getWorld() -> getLevel() * 1);
}

bool Protesters::isFacingTunnelMan()
{
    int tunnelmanX = getWorld() -> getTunnelMan() -> getX();
    int tunnelmanY = getWorld() -> getTunnelMan() -> getY();
    int protesterX = getX();
    int protesterY = getY();
    Actor::Direction dir = getDirection();
    
    if(dir == right)//direction of protester is right i.e. tunnel man must be to the right
    {
        if(tunnelmanX > protesterX)
            return true;
    }
    else if(dir == left)
    {
        if(tunnelmanX < protesterX)
            return true;
    }
    else if(dir == up)
    {
        if(tunnelmanY > protesterY)
            return true;
    }
    else if(dir == down)
    {
        if(tunnelmanY < protesterY)
            return true;
    }
    return false;
}

int Protesters::getTicksSinceLastShout()
{
    return ticksSinceLastShout;
}

void Protesters::resetTicksSinceLastShout()
{
    ticksSinceLastShout = 0;
}

void Protesters::incrementTicksSinceLastShout()
{
    ticksSinceLastShout++;
}

bool Protesters::checkHorizontalLineOfSight()
{
    int tunnelmanX = getWorld() -> getTunnelMan() -> getX();
    int tunnelmanY = getWorld() -> getTunnelMan() -> getY();
    int protesterX = getX();
    int protesterY = getY();
    int deltaX = tunnelmanX - protesterX;
    int deltaY = tunnelmanY - protesterY;
    
    if(deltaY != 0)
        return false;
    
    if(deltaX < 0) //tunnelmanX < protesterX
    {
        for(int i = tunnelmanX; i < protesterX; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(getWorld() -> isEarthPresent(i, getY() + j) == true)
                    return false;
            }
            
            if(getWorld() -> isBoulderPresent(i, getY()) == true)
                return false;
        }
    }
    else if(deltaX > 0) //tunnelmanX > protesterX
    {
        for(int i = protesterX; i < tunnelmanX; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(getWorld() -> isEarthPresent(i, getY() + j) == true)
                    return false;
            }
            
            if(getWorld() -> isBoulderPresent(i, getY()) == true)
                return false;
        }
    }
    return true;
}

bool Protesters::checkVerticalLineOfSight()
{
    int tunnelmanX = getWorld() -> getTunnelMan() -> getX();
    int tunnelmanY = getWorld() -> getTunnelMan() -> getY();
    int protesterX = getX();
    int protesterY = getY();
    int deltaX = tunnelmanX - protesterX;
    int deltaY = tunnelmanY - protesterY;
    
    if(deltaX != 0)
        return false;
    
    if(deltaY < 0) //tunnelmanY < protesterY
    {
        for(int i = tunnelmanY; i < protesterY; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(getWorld() -> isEarthPresent(getX() + j, i) == true)
                    return false;
            }
            
            if(getWorld() -> isBoulderPresent(getX(), i) == true)
                return false;
        }
    }
    else if(deltaY > 0) //tunnelmanY > protesterY
    {
        for(int i = protesterY; i < tunnelmanY; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(getWorld() -> isEarthPresent(getX() + j, i) == true)
                    return false;
            }
            
            if(getWorld() -> isBoulderPresent(getX(), i) == true)
                return false;
        }
    }
    return true;
}

int Protesters::getTicksPerp()
{
    return ticksPerp;
}

void Protesters::resetTicksPerp()
{
    ticksPerp = 200;
}
void Protesters::decrementTicksPerp()
{
    ticksPerp--;
}

void Protesters::doSomething()
{
    if(!isAlive())
        return;
    
    if(getStunned() == true)
    {
        increaseTicksToWaitBetweenMoves();
        setStunned(false);
        return;
    }
    
    if(getHitPointsForProt() <= 0)
    {
        if(isProtHardcore() == true)
            getWorld() -> increaseScore(250);
        else
            getWorld() -> increaseScore(100);
        getWorld() -> playSound(SOUND_PROTESTER_GIVE_UP);
        setAlive(false);
    }
    
    if(getTicksToWaitBetweenMoves() > 0)
    {
        decrementTicksToWaitBetweenMoves();
        return;
    }
    
    resetTicksToWaitBetweenMoves();
    incrementTicksSinceLastShout();
    decrementTicksPerp();
    
    if(getLeaveTheOilField() == true && getHitPointsForProt() <= 0)
    {
        //if(getX() == 60 && getY() == 60)
            setAlive(false);
        //else
            //exit algorithm
        return;
    }
    
    if(getWorld() -> getTunnelMan() -> isTunnelManWithinDistance(getX(), getY(), 4) == true && isFacingTunnelMan())
    {
        if(getTicksSinceLastShout() >= 15)
        {
            getWorld() -> playSound(SOUND_PROTESTER_YELL);
            getWorld() -> getTunnelMan() -> decrementHitPoints();
            resetTicksSinceLastShout();
        }
        return;
    }
    
    if((checkVerticalLineOfSight() == true || checkHorizontalLineOfSight() == true) && getWorld() -> getTunnelMan() -> isTunnelManWithinDistance(getX(), getY(), 4) == false)
    {
        int tunnelmanX = getWorld() -> getTunnelMan() -> getX();
        int tunnelmanY = getWorld() -> getTunnelMan() -> getY();
        int protesterX = getX();
        int protesterY = getY();
        
        if(checkVerticalLineOfSight() == true)
        {
            if(tunnelmanY > protesterY)
            {
                setDirection(up);
                if(getWorld() -> canMove(getX(), getY(), DIR_UP))
                    moveTo(getX(), getY() + 1);
            }
            else
            {
                setDirection(down);
                if(getWorld() -> canMove(getX(), getY(), DIR_DOWN))
                    moveTo(getX(), getY() - 1);
            }
        }
        else if(checkHorizontalLineOfSight() == true)
        {
            if(tunnelmanX > protesterX)
            {
                setDirection(right);
                if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT))
                    moveTo(getX() + 1, getY());
            }
            else
            {
                setDirection(left);
                if(getWorld() -> canMove(getX(), getY(), DIR_LEFT))
                    moveTo(getX() - 1, getY());
            }
        }
        setNumSquaresToMoveInCurrentDirection();
        return;
    }
    
    decrementNumSquaresToMoveInCurrentDirection();
    
    if(getNumSquaresToMoveInCurrentDirection() <= 0)
    {
        bool flag = false;
        while(flag == false)
        {
            int randDir = rand() % 4;
            if(randDir == 0 && getWorld() -> canMove(getX(), getY(), randDir) == true)
            {
                setDirection(right);
                resetNumSquaresToMoveInCurrentDirection();
                flag = true;
            }
            else if(randDir == 1 && getWorld() -> canMove(getX(), getY(), randDir) == true)
            {
                setDirection(left);
                resetNumSquaresToMoveInCurrentDirection();
                flag = true;
            }
            else if(randDir == 2 && getWorld() -> canMove(getX(), getY(), randDir) == true)
            {
                setDirection(up);
                resetNumSquaresToMoveInCurrentDirection();
                flag = true;
            }
            if(randDir == 3 && getWorld() -> canMove(getX(), getY(), randDir) == true)
            {
                setDirection(down);
                resetNumSquaresToMoveInCurrentDirection();
                flag = true;
            }
        }
    }
    
    if(getTicksPerp() <= 0)
    {
        Actor::Direction currentDir = getDirection();
        if(currentDir == right || currentDir == left)
        {
            if(getWorld() -> canMove(getX(), getY(), DIR_UP) == true || getWorld() -> canMove(getX(), getY() - 4, DIR_DOWN) == true)
            {
                if(getWorld() -> canMove(getX(), getY(), DIR_UP) == true && getWorld() -> canMove(getX(), getY(), DIR_DOWN) == true)
                {
                    int randDir = rand() % 2 + 2;
                    if(randDir == 2)
                    {
                        setDirection(up);
                        resetTicksPerp();
                    }
                    else
                    {
                        setDirection(down);
                        resetTicksPerp();
                    }
                }
                else if(getWorld() -> canMove(getX(), getY(), DIR_UP) == true)
                {
                    setDirection(up);
                    resetTicksPerp();

                }
                else
                {
                    setDirection(down);
                    resetTicksPerp();
                }
            }
        }
        else if(currentDir == up || currentDir == down)
        {
            if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == true || getWorld() -> canMove(getX(), getY(), DIR_LEFT) == true)
            {
                if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == true && getWorld() -> canMove(getX(), getY(), DIR_LEFT) == true)
                {
                    int randDir = rand() % 2;
                    if(randDir == 0)
                    {
                        setDirection(right);
                        resetTicksPerp();
                    }
                    else
                    {
                        setDirection(left);
                        resetTicksPerp();
                    }
                }
                else if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == true)
                {
                    setDirection(right);
                    resetTicksPerp();
                }
                else
                {
                    setDirection(left);
                    resetTicksPerp();
                }
            }
        }
    }
    
    else if(getDirection() == right && getWorld() -> canMove(getX(), getY(), DIR_UP) == false && getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == false && getWorld() -> canMove(getX(), getY(), DIR_DOWN) == false)
        setDirection(left);
    else if(getDirection() == left && getWorld() -> canMove(getX(), getY(), DIR_UP) == false && getWorld() -> canMove(getX(), getY(), DIR_LEFT) == false && getWorld() -> canMove(getX(), getY(), DIR_DOWN) == false)
        setDirection(right);
    else if(getDirection() == up && getWorld() -> canMove(getX(), getY(), DIR_UP) == false && getWorld() -> canMove(getX(), getY(), DIR_LEFT) == false && getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == false)
        setDirection(down);
    else if(getDirection() == down && getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == false && getWorld() -> canMove(getX(), getY(), DIR_LEFT) == false && getWorld() -> canMove(getX(), getY(), DIR_DOWN) == false)
        setDirection(up);
    
    if(getDirection() == right)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_RIGHT) == true)
            moveTo(getX() + 1, getY());
    }
    else if(getDirection() == left)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_LEFT) == true)
            moveTo(getX() - 1, getY());
    }
    else if(getDirection() == down)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_DOWN) == true)
            moveTo(getX(), getY() - 1);
    }
    else if(getDirection() == up)
    {
        if(getWorld() -> canMove(getX(), getY(), DIR_UP) == true)
            moveTo(getX(), getY() + 1);
    }
    else
    {
        setNumSquaresToMoveInCurrentDirection();
    }
}

bool Protesters::isBribed()
{
    return bribe;
}

void Protesters::setBribed(bool b)
{
    bribe = b;
}
    
bool Protesters::isProtHardcore()
{
    return hardcore;
}

void Protesters::setProtHardcore(bool hc)
{
    hardcore = hc;
}

Protesters::~Protesters(){}

int Protesters::getHitPointsForProt()
{
    return protesterHitPoints;
}

void Protesters::setHitPointsForProt(int p)
{
    protesterHitPoints = p;
}

void Protesters::decrementHitPointsForProt()
{
    protesterHitPoints -= 2;
}

bool Protesters::getStunned()
{
    return stunned;
}

void Protesters::setStunned(bool s)
{
    stunned = s;
}

//================================================RegularProtesters================================================

RegularProtester::RegularProtester(StudentWorld* world)
    :Protesters(world)
{}
    
RegularProtester::~RegularProtester()
{}

//================================================HardcoreProtesters================================================

HardcoreProtester::HardcoreProtester(StudentWorld* world)
    :Protesters(world, true, TID_HARD_CORE_PROTESTER)
{}

HardcoreProtester::~HardcoreProtester()
{}

//====================================================================================================================
