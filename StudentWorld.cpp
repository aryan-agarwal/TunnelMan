#include "StudentWorld.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
    int B = min(int(getLevel() / 2 + 2), 9);
    int G = max(int(5 - getLevel() / 2), 2);
    int L = min(int(2 + getLevel()), 21);
    
    barrelsOfOil = L;
    protestorTicks = 0;
    ticks = 0;
    nProtestors = 0;
    m_TunnelMan = new TunnelMan(this);
    
    for(int i = 0; i < VIEW_WIDTH; i++)
    {
        for(int j = 0; j < VIEW_HEIGHT; j++)
        {
            if((i < 30 || i > 33 || j < 4) && j < 60)
            {
                m_Earth[i][j] = new Earth(this, TID_EARTH, i, j);
                actorID[i][j] = AID_EARTH;
            }
            else
            {
                m_Earth[i][j] = nullptr;
                actorID[i][j] = AID_EMPTY;
            }
        }
    }
    
    for(int b = 0; b < B; b++)
    {
        bool flag = false;
        int x = 0, y = 0;
        while(flag == false)
        {
            x = rand() % 61;
            y = rand() % 37 + 20;
            if(canBePlaced(x, y) == true && (x < 27 || x > 33) && (y < 57 || y >= 0 ))
                flag = true;
        }
        
        for(int p = x; p < x + 4; p++)
        {
            for(int q = y; q < y + 4; q++)
            {
                if(m_Earth[p][q] != nullptr)
                {
                    if(m_Earth[p][q] -> getVisibility() == true)
                    {
                        
                        m_Earth[p][q] -> setVisible(false);
                        m_Earth[p][q] -> setVisibility(true);
                        delete m_Earth[p][q];
                        m_Earth[p][q] = nullptr;
                    }
                }
            }
        }
        m_actors.push_back(new Boulders(this, x, y));
        setActorID(x, y, AID_BOULDERS);
    }

    for(int l = 0; l < L; l++)
    {
        bool flag = false;
        int x = 0, y = 0;
        while(flag == false)
        {
            x = rand() % 61;
            y = rand() % 57;
            if(canBePlaced(x, y) == true && (x < 27 || x > 33) && (y < 57 || y >= 0 ))
                flag = true;
        }
        m_actors.push_back(new BarrelsOfOil(getTunnelMan(), this, x, y));
        setActorID(x, y, AID_OIL);
    }
    
    for(int g = 0; g < G; g++)
    {
        bool flag = false;
        int x = 0, y = 0;
        while(flag == false)
        {
            x = rand() % 61;
            y = rand() % 57;
            if(canBePlaced(x, y) == true && (x < 27 || x > 33) && (y < 57 || y >= 0 ))
                flag = true;
        }

        m_actors.push_back(new GoldNuggets(getTunnelMan(), this, x, y, PERMANENT_GOLD));
        setActorID(x, y, AID_GOLD);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
    addGoodies();
    addProtestors();
    ticks++;

    for (vector<Actor *>::iterator itr = m_actors.begin(); itr != m_actors.end(); itr++)
    {
        if ((*itr) == nullptr)
            continue;
    
        if ((*itr)->isAlive())
            (*itr)->doSomething();
    }
    
    m_TunnelMan -> doSomething();
    
    if(barrelsOfOil == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if(m_TunnelMan -> getHitPoints() <= 0)
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        m_TunnelMan -> setAlive(false);
    }
    
    if(!m_TunnelMan -> isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    for (vector<Actor *>::iterator itr = m_actors.begin(); itr != m_actors.end(); itr++)
    {
        if((*itr) != nullptr && !(*itr) -> isAlive())
        {
            delete *itr;
            itr = m_actors.erase(itr);
            itr--;
        }
    }
    
    for (vector<Protesters*>::iterator itr = m_protesters.begin(); itr != m_protesters.end(); itr++)
    {
        if((*itr) != nullptr && !(*itr) -> isAlive())
        {
            itr = m_protesters.erase(itr);
            itr--;
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addActor(Actor* act)
{
    m_actors.push_back(act);
}

int StudentWorld::getActorID(int x, int y)
{
    return actorID[x][y];
}

void StudentWorld::setActorID(int x, int y, int ID)
{
    actorID[x][y] = ID;
}

TunnelMan* StudentWorld::getTunnelMan()
{
    return m_TunnelMan;
}

void StudentWorld::decreaseBarrelCount()
{
    barrelsOfOil--;
}

int StudentWorld::getBarrelsOfOil()
{
    return barrelsOfOil;
}

Earth* StudentWorld::getEarth(int x, int y)
{
    return m_Earth[x][y];
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); )
    {
        delete *itr;
        m_actors.erase(m_actors.begin());
    }
    
    for (vector<Protesters*>::iterator itr = m_protesters.begin(); itr != m_protesters.end(); itr++)
    {
        itr = m_protesters.erase(itr);
        itr--;
    }
    
    delete m_TunnelMan;
    m_TunnelMan = nullptr;
    
    for(int i = 0; i < VIEW_WIDTH; i++)
    {
        for(int j = 0; j < VIEW_HEIGHT; j++)
        {
            if((i < 30 || i > 33 || j < 4) && j < 60)
                if(m_Earth[i][j] != nullptr)
                {
                    delete m_Earth[i][j];
                    m_Earth[i][j] = nullptr;
                }
        }
    }
}

StudentWorld::~StudentWorld()
{}

void StudentWorld::setDisplayText()
{
    int level = getLevel();
    int lives = getLives();
    int health = (m_TunnelMan -> getHitPoints()) * 100 / 10;
    int squirts = m_TunnelMan -> getUnitsOfWater();
    int gold = m_TunnelMan -> getGoldNuggets();
    int barrelsLeft = getBarrelsOfOil();
    int sonar = m_TunnelMan -> getSonarCharge();
    int score = getScore();
    
    ostringstream oss;
    
    oss << "Lvl: ";
    oss << setw(2) << level;
    
    oss << "  Lives: ";
    oss << setw(1) << lives;
    
    oss << "  Hlth: ";
    oss << setw(2) << health;
    
    oss << "%";
    
    oss << "  Wtr: ";
    oss << setw(2) << squirts;
    
    oss << "  Gld: ";
    oss << setw(2) << gold;
    
    oss << "  Oil Left: ";
    oss << setw(2) << barrelsLeft;
    
    oss << "  Sonar: ";
    oss << setw(2) << sonar;
    
    oss << "  Scr: ";
    oss << setw(6) << setfill('0') << score;
    
    // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
    string s = oss.str();
    setGameStatText(s);
}

bool StudentWorld::canBePlaced(int x, int y)
{
    if(m_actors.size() == 0)
        return true;
    
    for(int i = 0; i < m_actors.size(); i++)
    {
        int xPosActor = m_actors[i] -> getX();
        int yPosActor = m_actors[i] -> getY();
        int xCoord = x - xPosActor;
        int yCoord = y - yPosActor;
        int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
        
        if(radiusSq < 36)
            return false;
    }
    return true;
}

void StudentWorld::addProtestors()
{
    int T = fmax(25, 200 - getLevel());
    int P = fmin(15, 2 + getLevel() * 1.5);
    int probabilityOfHardcore = fmin(90, getLevel() * 10 + 30);
    int value = 1 + rand() % 100 ;
    Protesters *prot;
    
    if(ticks == 1)
    {
        if(value > probabilityOfHardcore)
            prot = new RegularProtester(this);
        else
            prot = new HardcoreProtester(this);
        nProtestors++;
        m_actors.push_back(prot);
        m_protesters.push_back(prot);
    }
    
    if(nProtestors < P && protestorTicks >= T)
    {
        if(value > probabilityOfHardcore)
            prot = new RegularProtester(this);
        else
            prot = new HardcoreProtester(this);
        m_actors.push_back(prot);
        m_protesters.push_back(prot);
        nProtestors++;
        protestorTicks = 0;
    }
    else
        protestorTicks++;
}

void StudentWorld::addGoodies()
{
    int G = getLevel() * 25 + 300;
    int val = rand() % G;
    if(val == 0)
    {
        int val2 = rand() % 5;
        if(val2 == 0)
        {
            m_actors.push_back(new SonarKits(getTunnelMan(), this, 0, 60, TID_SONAR));
        }
        else
        {
            int x = 0, y = 0;
            bool flag = false;
            while(flag == false)
            {
                x = rand() % 61;
                y = rand() % 61;
                if(checkIfGoodieCanBePlaced(x, y) == true)
                    flag = true;
                else
                    continue;
            }
            m_actors.push_back(new WaterRefills(getTunnelMan(), this, x, y, TID_WATER_POOL));
        }
    }
    
}

bool StudentWorld::checkIfGoodieCanBePlaced(int x, int y)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(getActorID(x + i, y + j) != AID_EMPTY)
                return false;
        }
    }
    return true;
}

void StudentWorld::useSonar(int x, int y)
{
    for(int i = 0; i < m_actors.size(); i++)
    {
        if(m_actors[i] -> getIdentity() == GOODIE_ID || m_actors[i] -> getIdentity() == OIL_ID)
        {
            int xPosActor = m_actors[i] -> getX();
            int yPosActor = m_actors[i] -> getY();
            int xCoord = x - xPosActor;
            int yCoord = y - yPosActor;
            int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
            if(radiusSq <= 144)
            {
                m_actors[i] -> setVisible(true);
                m_actors[i] -> setVisibility(true);
            }
            
        }
    }
}

bool StudentWorld::canMove(int x, int y, int dir)
{
    if(x > 60 || x < 0 || y > 60 || y < 0)
        return false;
    
    if(dir == DIR_RIGHT)
    {
        if(x == 60)
            return false;
        
        if(isBoulderPresent(x + 1, y) == true)
            return false;
        
        for(int i = 0; i < 4; i++)
        {
            if(isEarthPresent(x + 4, y + i) == true)
                return false;
        }
    }
    
    if(dir == DIR_LEFT)
    {
        if(x == 0)
            return false;
        
        if(isBoulderPresent(x - 1, y) == true)
            return false;
        
        for(int i = 0; i < 4; i++)
        {
            if(isEarthPresent(x - 1, y + i) == true)
                return false;
        }
    }
    
    if(dir == DIR_UP)
    {
        if(y == 60)
            return false;
        
        if(isBoulderPresent(x, y + 1) == true)
            return false;
        
        for(int i = 0; i < 4; i++)
        {
            if(isEarthPresent(x + i, y + 4) == true)
                return false;
        }
    }
    
    if(dir == DIR_DOWN)
    {
        if(y == 0)
            return false;
        
        if(isBoulderPresent(x, y - 1) == true)
            return false;
        
        for(int i = 0; i < 4; i++)
        {
            if(isEarthPresent(x + i, y - 1) == true)
                return false;
        }
    }
    
    return true;
}

bool StudentWorld::isBoulderPresent(int x, int y)
{
    for(int i = 0; i < m_actors.size(); i++)
    {
        if(m_actors[i] -> getIdentity() == BOULDER_ID)
        {
            int xPosBoulder = m_actors[i] -> getX();
            int yPosBoulder = m_actors[i] -> getY();
            int xCoord = x - xPosBoulder;
            int yCoord = y - yPosBoulder;
            int radiusSq =(xCoord * xCoord) + (yCoord * yCoord);
                
            if (radiusSq <= 9)
                return true;
            }
        }
    return false;
}

bool StudentWorld::isEarthPresent(int x, int y)
{
    if(m_Earth[x][y] == nullptr || m_Earth[x][y] -> getVisibility() == false)
        return false;
    
    return true;
}

std::vector<Actor*> StudentWorld::getActor()
{
    return m_actors;
}

void StudentWorld::shootProtester(int x, int y)
{
    for(int i = 0; i < m_protesters.size(); i++)
    {
        int xPosProt = m_protesters[i] -> getX();
        int yPosProt = m_protesters[i] -> getY();
        int xCoord = x - xPosProt;
        int yCoord = y - yPosProt;
        int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
        if(radiusSq <= 9)
        {
            m_protesters[i] -> decrementHitPointsForProt();
            m_protesters[i] -> setStunned(true);
        }
    }
}

void StudentWorld::dropBoulderOnProtester(int x, int y)
{
    for(int i = 0; i < m_protesters.size(); i++)
    {
        int xPosProt = m_protesters[i] -> getX();
        int yPosProt = m_protesters[i] -> getY();
        int xCoord = x - xPosProt;
        int yCoord = y - yPosProt;
        int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
        if(radiusSq <= 9)
        {
            playSound(SOUND_PROTESTER_GIVE_UP);
            m_protesters[i] -> setLeaveTheOilField();
            m_protesters[i] -> getWorld() -> increaseScore(500);
            m_protesters[i] -> setAlive(false);
            m_protesters[i] -> setVisible(false);
            m_protesters[i] -> setVisibility(false);
        }
    }
}

void StudentWorld::bribeProtester(int x, int y)
{
    for(int i = 0; i < m_protesters.size(); i++)
    {
        int xPosProt = m_protesters[i] -> getX();
        int yPosProt = m_protesters[i] -> getY();
        int xCoord = x - xPosProt;
        int yCoord = y - yPosProt;
        int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
        
        if(radiusSq <= 9)
        {
            m_protesters[i] -> setBribed(true);
            playSound(SOUND_PROTESTER_FOUND_GOLD);
            if(m_protesters[i] -> isProtHardcore() == false)
            {
                m_protesters[i] -> setLeaveTheOilField();
                increaseScore(25);
                m_protesters[i] -> setAlive(false);
                m_protesters[i] -> setVisibility(false);
                m_protesters[i] -> setVisible(false);
            }
            else
            {
                increaseScore(50);
                m_protesters[i] -> increaseTicksToWaitBetweenMoves();
            }
        }
    }
}

bool StudentWorld::isProtesterWithinDist(int x, int y, int dist)
{
    for(int i = 0; i < m_protesters.size(); i++)
    {
        int xPosProt = m_protesters[i] -> getX();
        int yPosProt = m_protesters[i] -> getY();
        int xCoord = x - xPosProt;
        int yCoord = y - yPosProt;
        int radiusSq = (xCoord * xCoord) + (yCoord * yCoord);
        if(radiusSq <= (dist*dist))
            return true;
    }
    return false;
}

