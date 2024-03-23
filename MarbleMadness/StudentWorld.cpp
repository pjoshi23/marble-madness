#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {

}

int StudentWorld::init() {
    
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel("level00.txt");
    
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level00.txt data file\n";
    } else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
    } else if (result == Level::load_success) {
        cerr << "Successfully loaded level\n";
        
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry entry = lev.getContentsOf(x,y);
                
                switch(entry) {
                    case Level::empty:
                        break;
                    case Level::player:
                        mAvatar = new Player(this, x, y);
                        break;
                    case Level::wall:
                        actors.push_back(new Wall(this, x, y));
                        break;
                    case Level::pit:
                        actors.push_back(new Pit(this, x, y));
                        break;
                    case Level::marble:
                        actors.push_back(new Marble(this, x, y));
                        break;
                    case Level::crystal:
                        numCrystals++;
                        actors.push_back(new Crystal(this, x, y));
                        break;
                    case Level::ammo:
                        actors.push_back(new AmmoGoodie(this, x, y));
                        break;
                    case Level::restore_health:
                        actors.push_back(new RestoreHealthGoodie(this, x, y));
                        break;
                    case Level::extra_life:
                        actors.push_back(new ExtraLifeGoodie(this, x, y));
                        break;
                    case Level::exit:
                        actors.push_back(new Exit(this, x, y));
                        break;
                    case Level::vert_ragebot:
                        actors.push_back(new RageBot(this, x, y, Actor::down));
                        break;
                    case Level::horiz_ragebot:
                        actors.push_back(new RageBot(this, x, y, Actor::right));
                        break;
                }
            }
        }
        
        mBonus = 1000;
        
        return 0;
        
    }
    
    return 1;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you type q
    
    mAvatar->doSomething();
    for (Actor* actor : actors) {
        actor->doSomething();
    }
    
    for(auto temp = actors.begin(); temp != actors.end();)
      {
             if(!((*temp)->isAlive()))
             {
                 Actor* actor = *temp;
                 delete actor;
                 temp = actors.erase(temp);
             }
             else
             {
                 temp++;
             }
         }

    //setGameStatText(std::to_string(mAvatar->getHitPoints()));
    
    mBonus--;
    if (mBonus < 0) {
        mBonus = 0;
    }
    
    if (mAvatar->isAlive()) {
        return GWSTATUS_CONTINUE_GAME;
    } else {
        cleanUp();
        return GWSTATUS_PLAYER_DIED;
    }

    
}

void StudentWorld::cleanUp() {
    for (int i = 0; i < actors.size(); i++) {
        delete actors[i];
    }
    actors.clear();
    
    std::cout << mAvatar << endl;
    delete mAvatar;
    mAvatar = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

// Can agent move to x,y?  (dx and dy indicate the direction of motion)
bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y) const {
    bool possible = true;
    for (Actor* actor : actors) {
        if (actor->getX() == x && actor->getY() == y) {
            if (!actor->isAlive()) {
                continue;
            }
            if (actor->allowsAgentColocation()) {
                possible = true;
            } else {
                return false;
            }
        }
    }
    
    return true;
}

bool StudentWorld::moveMarbleIfPossible(int xOld, int yOld, int xNew, int yNew) {
    for (Actor* actor : actors) {
        if (actor->getX() == xOld && actor->getY() == yOld) {
            if (!actor->isAlive()) {
                continue;
            }
            if (!actor->isSwallowable()) {
                continue;
            }
            
            if (this->canMarbleMoveTo(xNew, yNew)) {
                actor->moveTo(xNew, yNew);
                return true;
            } else {
                return false;
            }
        }
    }
    
    return false;
}

// Can a marble move to x,y?
bool StudentWorld::canMarbleMoveTo(int xNew, int yNew) {
    for (Actor* actor : actors) {
        if (actor->getX() == xNew && actor->getY() == yNew) {
            if (!actor->isAlive()) {
                continue;
            }
            if (actor->allowsMarble()) {
                return true;
            } else {
                return false;
            }
        }
    }
    
    return true;
}

bool StudentWorld::isMarble(int x, int y) {
    bool found = false;
    for (Actor* actor : actors) {
        if (actor->getX() == x && actor->getY() == y) {
            if (!actor->isAlive()) {
                continue;
            }
            if (actor->isSwallowable()) {
                //setGameStatText("is a marble");
                found = true;
                return found;
            } else {
                //setGameStatText("is not a marble");
                found = false;
            }
        }
    }
    
    return found;
}

// Is the player on the same square as an Actor?
//bool StudentWorld::isPlayerColocatedWith(int x, int y) const;

// Try to cause damage to something at a's location.  (a is only ever
// going to be a pea.)  Return true if something stops a -- something
// at this location prevents a pea from continuing.
//bool StudentWorld::damageSomething(Actor* a, int damageAmt);

// Swallow any swallowable object at a's location.  (a is only ever
// going to be a pit.)
bool StudentWorld::swallowSwallowable(Actor* a) {
    
    int xPos = a->getX();
    int yPos = a->getY();
    
    for (Actor* actor : actors) {
        if (actor->getX() == xPos && actor->getY() == yPos) {
            if (actor->isSwallowable()) {
                actor->setVisible(false);
                actor->setDead();
                return true;
            }
        }
    }
    
    return false;
}

bool StudentWorld::hasClearShot(Actor* a) {
    if (a->getDirection() == Actor::up) {
        if (mAvatar->getX() == a->getX() && mAvatar->getY() > a->getY()) {
            for (Actor* actor : actors) {
                if (actor->getX() == a->getX() && actor->getY() > a->getY() && actor->getY() < mAvatar->getY()) {
                    if (actor->isDamageable() || actor->stopsPea()) {
                        return false;
                    }
                }
                
            }
            
            return true;
        } else {
            return false;
        }
    } else if (a->getDirection() == Actor::down) {
        if (mAvatar->getX() == a->getX() && mAvatar->getY() < a->getY()) {
            for (Actor* actor : actors) {
                if (actor->getX() == a->getX() && actor->getY() < a->getY() && actor->getY() > mAvatar->getY()) {
                    if (actor->isDamageable() || actor->stopsPea()) {
                        return false;
                    }
                }
                
            }
            
            return true;
        } else {
            return false;
        }
    } else if (a->getDirection() == Actor::left) {
        if (mAvatar->getY() == a->getY() && mAvatar->getX() < a->getX()) {
            for (Actor* actor : actors) {
                if (actor->getY() == a->getY() && actor->getX() < a->getX() && actor->getX() > mAvatar->getX()) {
                    if (actor->isDamageable() || actor->stopsPea()) {
                        return false;
                    }
                }
                
            }
            
            return true;
        } else {
            return false;
        }
    } else {
        if (mAvatar->getY() == a->getY() && mAvatar->getX() > a->getX()) {
            for (Actor* actor : actors) {
                if (actor->getY() == a->getY() && actor->getX() > a->getX() && actor->getX() < mAvatar->getX()) {
                    if (actor->isDamageable() || actor->stopsPea()) {
                        return false;
                    }
                }
                
            }
            
            return true;
        } else {
            return false;
        }
    }
}

void StudentWorld::increasePlayerScore(int amt) {
    this->increaseScore(amt);
}

bool StudentWorld::damageIfPossible(Actor *a) {
    if (mAvatar->getX() == a->getX() && mAvatar->getY() == a->getY()) {
        playSound(SOUND_PLAYER_IMPACT);
        mAvatar->tryToBeKilled(2);
        setGameStatText("damaging actor");
        return true;
    }
    
    
    for (Actor* actor: actors) {
        if ((actor->getX() == a->getX()) && (actor->getY() == a->getY())) {
            if (!(actor->isAlive())) {
                continue;
            }
            //setGameStatText("about to check");
            if (actor->isDamageable()) {
                actor->tryToBeKilled(2);
                if (actor->makesSoundWhenHit()) {
                    playSound(actor->shootingSound());
                }
                //setGameStatText("damaging actor");
                return true;
            } else if (actor->stopsPea()) {
                //setGameStatText("actor stopped pea, no damage");
                return true;
            }
        }
    }
    
    return false;
}

bool StudentWorld::isWithPlayer(Actor *a) {
    if (mAvatar->getX() == a->getX() && mAvatar->getY() == a->getY()) {
        return true;
    } else {
        return false;
    }
}

// If a pea were at x,y moving in direction dx,dy, could it hit the
// player without encountering any obstructions?
//bool StudentWorld::existsClearShotToPlayer(int x, int y, int dx, int dy) const;

// If a factory is at x,y, how many items of the type that should be
// counted are in the rectangle bounded by x-distance,y-distance and
// x+distance,y+distance?  Set count to that number and return true,
// unless an item is on the factory itself, in which case return false
// and don't care about count.  (The items counted are only ever going
// ThiefBots.)
//bool StudentWorld::doFactoryCensus(int x, int y, int distance, int& count) const;

// If an item that can be stolen is at x,y, return a pointer to it;
// otherwise, return a null pointer.  (Stealable items are only ever
// going be goodies.)
//Actor* StudentWorld::getColocatedStealable(int x, int y) const;

// Restore player's health to the full amount.
void StudentWorld::restorePlayerHealth() {
    mAvatar->restoreHealth();
}

// Increase the amount of ammunition the player has
void StudentWorld::increaseAmmo() {
    mAvatar->increaseAmmo();
}

// Are there any crystals left on this level?
bool StudentWorld::anyCrystals() const {
    return (numCrystals != 0);
}

// Reduce the count of crystals on this level by 1.
void StudentWorld::decCrystals() {
    numCrystals--;
    
}

void StudentWorld::addBonusPoints() {
    increasePlayerScore(mBonus);
}

// Indicate that the player has finished the level.
//void StudentWorld::setLevelFinished() {
//
//}

// Add an actor to the world
void StudentWorld::addActor(Actor* a) {
    actors.push_back(a);
}

Actor* StudentWorld::claimGoodieIfPossible(Actor *a) {
    for (Actor* actor : actors) {
        if (actor->getX() == a->getX() && actor->getY() == a->getY()) {
            if (actor->isStealable() && !actor->getStolen()) {
                int num = (std::rand() % 10) + 1;
                if (num == 1) {
                    actor->setStolen(true);
                    return actor;
                }
            }
        }
    }
    
    return nullptr;
}
