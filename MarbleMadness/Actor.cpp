#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir) : GraphObject(imageID, startX, startY, startDir) {
    this->world = world;
    this->health = hitPoints;
    this->mAlive = true;
    setVisible(true);
}

// Is this actor alive?
bool Actor::isAlive() const {
    return mAlive;
}

// Mark this actor as dead
void Actor::setDead() {
    mAlive = false;
}

// Reduce this actor's hit points
void Actor::decHitPoints(int amt) {
    this->health -= amt;
    if (this->health <= 0) {
        this->health = 0;
        this->mAlive = false;
    }
}

// Get this actor's world
StudentWorld* Actor::getWorld() const {
    return this->world;
}

// Can an agent occupy the same square as this actor?
bool Actor::allowsAgentColocation() const {
    return false;
}

// Can a marble occupy the same square as this actor?
bool Actor::allowsMarble() const {
    return false;
}

// Does this actor count when a factory counts items near it?
bool Actor::countsInFactoryCensus() const {
    return false;
}

// Does this actor stop peas from continuing?
bool Actor::stopsPea() const {
    return false;
}

// Can this actor be damaged by peas?
bool Actor::isDamageable() const {
    return false;
}

// Cause this Actor to sustain damageAmt hit points of damage.
void Actor::damage(int damageAmt) {
    decHitPoints(damageAmt);
}

// Can this actor be pushed by a to location x,y?
//virtual bool bePushedBy(Agent* a, int x, int y);


// Can this actor be picked up by a ThiefBot?
bool Actor::isStealable() const {
    return false;
}

// How many hit points does this actor have left?
int Actor::getHitPoints() const {
    return this->health;
}

// Set this actor's hit points to amt.
void Actor::setHitPoints(int amt) {
    this->health = amt;
}

// Make the actor sustain damage.  Return true if this kills the
// actor, and false otherwise.
bool Actor::tryToBeKilled(int damageAmt) {
    decHitPoints(damageAmt);
    if (this->mAlive) {
        return false;
    } else {
        return true;
    }
}

bool Actor::getStolen() {
    return mStolen;
}

bool Actor::isSwallowable() {
    return false;
}

bool Actor::makesSoundWhenHit() {
    return false;
}

int Actor::shootingSound() const {
    return 0;
}

void Actor::setStolen(bool stolen) {
    mStolen = stolen;
    if (stolen == true) {
        setVisible(false);
    } else {
        setVisible(true);
    }
}

Wall::Wall(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_WALL, 0, none) {
}

void Wall::doSomething() {
    return;
}

bool Wall::stopsPea() const {
    return true;
}

Agent::Agent(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir) : Actor(world, startX, startY, imageID, hitPoints, startDir) {
}

// Move to the adjacent square in the direction the agent is facing
// if it is not blocked, and return true.  Return false if the agent
// can't move.
bool Agent::moveIfPossible() {
    // TODO: implement later
    return true;
}

// Return true if this agent can push marbles (which means it's the
// player).
bool Agent::canPushMarbles() const {
    return false;
}

// Return true if this agent doesn't shoot unless there's an unobstructed
// path to the player.
bool Agent::needsClearShot() const {
    return true;
}

// Return the sound effect ID for a shot from this agent.
int Agent::shootingSound() const {
    return 0;
}

Player::Player(StudentWorld* world, int startX, int startY) : Agent(world, startX, startY, IID_PLAYER, 20, 0) {
}

void Player::doSomething() {
    int ch;
    
    if (!isAlive()) {
        return;
    }
    
    getWorld()->getKey(ch);
    
    if (ch == KEY_PRESS_LEFT) {
        
        if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
            this->setDirection(left);
            this->moveTo(this->getX()-1, this->getY());
            return;
        }
        
        //getWorld()->setGameStatText("passed check 0, ");
        if (getWorld()->isMarble(this->getX()-1, this->getY())) {
            //getWorld()->setGameStatText("passed check 1, ");
            bool result = getWorld()->moveMarbleIfPossible(this->getX()-1, this->getY(), this->getX()-2, this->getY());
            if (result) {
                this->setDirection(left);
                this->moveTo(this->getX()-1, this->getY());
                return;
            }
        }
        
        return;
    }
    
    if (ch == KEY_PRESS_RIGHT) {
        if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
            this->setDirection(right);
            this->moveTo(this->getX()+1, this->getY());
            return;
        }
        
        if (getWorld()->isMarble(this->getX()+1, this->getY())) {
            bool result = getWorld()->moveMarbleIfPossible(this->getX()+1, this->getY(), this->getX()+2, this->getY());
            if (result) {
                this->setDirection(right);
                this->moveTo(this->getX()+1, this->getY());
                return;
            }
        }
        
        return;
    }
    
    if (ch == KEY_PRESS_UP) {
        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
            this->setDirection(up);
            this->moveTo(this->getX(), this->getY()+1);
            return;
        }
        
        if (getWorld()->isMarble(this->getX(), this->getY()+1)) {
            bool result = getWorld()->moveMarbleIfPossible(this->getX(), this->getY()+1, this->getX(), this->getY()+2);
            if (result) {
                this->setDirection(up);
                this->moveTo(this->getX(), this->getY()+1);
                return;
            }
        }
        
        return;
    }
    
    if (ch == KEY_PRESS_DOWN) {
        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
            this->setDirection(down);
            this->moveTo(this->getX(), this->getY()-1);
            return;
        }
        
        if (getWorld()->isMarble(this->getX(), this->getY()-1)) {
            bool result = getWorld()->moveMarbleIfPossible(this->getX(), this->getY()-1, this->getX(), this->getY()-2);
            if (result) {
                this->setDirection(down);
                this->moveTo(this->getX(), this->getY()-1);
                return;
            }
        }
        
        return;
    }
    
    //getWorld()->setGameStatText("here");
    
    if (ch == KEY_PRESS_SPACE) {
        
        if (mAmmo == 0) {
            return;
        }
        
        mAmmo--;
        if (this->getDirection() == up) {
            getWorld()->setGameStatText("adding Pea with direction up");
            getWorld()->addActor(new Pea(getWorld(), this->getX(), this->getY()+1, up));
            return;
        }
        
        if (getDirection() == down) {
            getWorld()->setGameStatText("adding Pea with direction down");
            getWorld()->addActor(new Pea(getWorld(), this->getX(), this->getY()-1, down));
            return;
        }
        
        if (getDirection() == right) {
            getWorld()->setGameStatText("adding Pea with direction right");
            //getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->addActor(new Pea(getWorld(), this->getX()+1, this->getY(), right));
            return;
        }
        
        if (getDirection() == left) {
            getWorld()->setGameStatText("adding Pea with direction left");
            getWorld()->addActor(new Pea(getWorld(), this->getX()-1, this->getY(), left));
            return;
        }
    }
    
    //getWorld()->setGameStatText("penis");
    
    return;
}

bool Player::isDamageable() const {
    return true;
}

void Player::damage(int damageAmt) {
    bool res = tryToBeKilled(damageAmt);
    if (res) {
        setDead();
    }
}

bool Player::canPushMarbles() const {
    return true;
}

bool Player::needsClearShot() const {
    return false;
}

int Player::shootingSound() const {
    return true;
}

// Get player's health percentage
int Player::getHealthPct() const {
    return getHitPoints()/20.0;
}

// Get player's amount of ammunition
int Player::getAmmo() const {
    return mAmmo;
}

// Restore player's health to the full amount.
void Player::restoreHealth() {
    setHitPoints(20);
}

// Increase player's amount of ammunition.
void Player::increaseAmmo() {
    mAmmo += 20;
}

bool Player::makesSoundWhenHit() {
    return true;
}

Pit::Pit(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_PIT, 0, none) {
}

void Pit::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->swallowSwallowable(this)) {
        setDead();
        setVisible(false);
    }
}

bool Pit::allowsMarble() const {
    return true;
}

Marble::Marble(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_MARBLE, 10, none) {
}

void Marble::doSomething() {
    return;
}

bool Marble::isDamageable() const {
    return true;
}

void Marble::damage(int damageAmt) {
    this->Actor::damage(damageAmt);
}

bool Marble::isSwallowable() {
    return true;
}

bool Marble::bePushedBy(Agent* a, int x, int y) {
    if (getWorld()->canMarbleMoveTo(x, y)) {
        this->moveTo(x, y);
        return true;
    } else {
        return false;
    }
}

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score) : Actor(world, startX, startY, imageID, 0, none) {
    this->mScore = score;
}

void PickupableItem::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->isWithPlayer(this)) {
        getWorld()->increasePlayerScore(mScore);
        setVisible(false);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

bool PickupableItem::allowsAgentColocation() const {
    return true;
}

Crystal::Crystal(StudentWorld* world, int startX, int startY) : PickupableItem(world, startX, startY, IID_CRYSTAL, 50) {
}

void Crystal::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->isWithPlayer(this)) {
        getWorld()->decCrystals();
        PickupableItem::doSomething();
    }
}

Pea::Pea(StudentWorld* world, int startX, int startY, int startDir) : Actor(world, startX, startY, IID_PEA, 0, startDir) {
    
    //getWorld()->setGameStatText("created pea with direction: " + std::to_string(startDir));
    
}

void Pea::doSomething() {
    //getWorld()->setGameStatText("doing pea things");
    
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->damageIfPossible(this)) {
        setVisible(false);
        setDead();
        return;
    }
    
    if (getDirection() == up) {
        this->moveTo(this->getX(), this->getY()+1);
    } else if (getDirection() == down) {
        this->moveTo(this->getX(), this->getY()-1);
    } else if (getDirection() == right) {
        this->moveTo(this->getX()+1, this->getY());
    } else if (getDirection() == left) {
        this->moveTo(this->getX()-1, this->getY());
    }
    
    if (getWorld()->damageIfPossible(this)) {
        setVisible(false);
        setDead();
        return;
    }
    
    return;
    

}

bool Pea::allowsAgentColocation() const {
    return true;
}

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score) : PickupableItem(world, startX, startY, imageID, score) {
    
}

void Goodie::doSomething() {
    PickupableItem::doSomething();
}

bool Goodie::isStealable() const {
    return true;
}

  // Set whether this goodie is currently stolen.
void Goodie::setStolen(bool status) {
    stolen = status;
    if (stolen == true) {
        setVisible(false);
    }
}

bool Goodie::getStatus() {
    return stolen;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000) {
    
}

void ExtraLifeGoodie::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->isWithPlayer(this)) {
        PickupableItem::doSomething();
        getWorld()->incLives();
    }
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500){
    
}
void RestoreHealthGoodie::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->isWithPlayer(this)) {
        PickupableItem::doSomething();
        getWorld()->restorePlayerHealth();
    }
}

AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_AMMO, 100){
    
}

void AmmoGoodie::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->isWithPlayer(this)) {
        PickupableItem::doSomething();
        getWorld()->increaseAmmo();
    }
}

Exit::Exit(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_EXIT, 0, none) {
    setVisible(false);
    done = false;
}

void Exit::doSomething() {
    if (getWorld()->anyCrystals()) {
        return;
    }
    
    if (done) {
        return;
    }
    
    if (!getWorld()->anyCrystals()) {
        setVisible(true);
        active = true;
        
        if (getWorld()->isWithPlayer(this)) {
            getWorld()->setGameStatText("same place as player");
            getWorld()->playSound(SOUND_FINISHED_LEVEL);
            getWorld()->increasePlayerScore(2000);
            getWorld()->addBonusPoints();
            done = true;
//            getWorld()->setLevelFinished();
            
        }
    }
}
bool Exit::allowsAgentColocation() const {
    return true;
}

bool Exit::allowsMarble() const {
    if (active) {
        return false;
    } else {
        return true;
    }
}

Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
             int hitPoints, int score, int startDir) : Agent(world, startX, startY, imageID, hitPoints, startDir) {
    
}

void Robot::doSomething() const {
    
}

bool Robot::isDamageable() const {
    return true;
}

void Robot::damage(int damageAmt) {
    tryToBeKilled(damageAmt);
}

bool Robot::canPushMarbles() const {
    return false;
}

bool Robot::needsClearShot() const {
    return true;
}

bool Robot::makesSoundWhenHit() {
    return true;
}

int Robot::shootingSound() const {
    return SOUND_ROBOT_IMPACT;
}

  // Does this robot shoot?
bool Robot::isShootingRobot() const {
    return true;
}

RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir) : Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir) {
    ticksPerMove = int((28 - getWorld()->getLevel()) / 4);

    if (ticksPerMove < 3) {
        ticks = 3;
    }
}

void RageBot::doSomething() {
    ticks++;
    
    if (ticks % ticksPerMove == 0) {
        if (getWorld()->hasClearShot(this)) {
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            if (getDirection() == up) {
                getWorld()->addActor(new Pea(getWorld(), this->getX(), this->getY()+1, up));
                return;
            }
            
            if (getDirection() == down) {
                getWorld()->addActor(new Pea(getWorld(), this->getX(), this->getY()-1, down));
                return;
            }
            
            if (getDirection() == left) {
                getWorld()->addActor(new Pea(getWorld(), this->getX()-1, this->getY(), left));
                return;
            }
            
            if (getDirection() == right) {
                getWorld()->addActor(new Pea(getWorld(), this->getX()+1, this->getY(), right));
                return;
            }
        } else {
            if (getDirection() == up) {
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                    this->moveTo(this->getX(), this->getY()+1);
                } else {
                    this->setDirection(down);
                    
                }
                
                return;
            }
            
            if (getDirection() == down) {
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                    this->moveTo(this->getX(), this->getY()-1);
                } else {
                    this->setDirection(up);
                    
                }
                
                return;
            }
            
            if (getDirection() == left) {
                if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                    this->moveTo(this->getX()-1, this->getY());
                } else {
                    this->setDirection(right);
                    
                }
                
                return;
            }
            
            if (getDirection() == right) {
                if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                    this->moveTo(this->getX()+1, this->getY());
                } else {
                    this->setDirection(left);
                    
                }
                
                return;
            }
        }
    } else {
        return;
    }
}

ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
                   int hitPoints, int score) : Robot(world, startX, startY, imageID, 5, score, right) {
    ticksPerMove = (28 - getWorld()->getLevel()) / 4;
    if (ticksPerMove < 3) {
        ticksPerMove = 3;
    }
}

void ThiefBot::doSomething() {
}
    
bool ThiefBot::countsInFactoryCensus() const {
    return true;
}
    
void ThiefBot::damage(int damageAmt) {
    tryToBeKilled(damageAmt);
}

RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, IID_THIEFBOT, 5, 10) {
    setVisible(true);
}

void RegularThiefBot::doSomething() {
    ticks++;
    if (!isAlive()) {
        return;
    }
    
    if (ticks % ticksPerMove != 0) {
        return;
    }
    
    bool hadGoodie = false;
    if (goodie != nullptr) {
        hadGoodie = true;
    } else {
        goodie = getWorld()->claimGoodieIfPossible(this);
    }
    
    if (goodie != nullptr && hadGoodie == false) {
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    } else {
        if (distanceBeforeTurning != 0) {
            if (getDirection() == up) {
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1))  {
                    this->moveTo(this->getX(), this->getY()+1);
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()+1);
                    }
                    return;
                } else {
                    distanceBeforeTurning = (std::rand() % 6) + 1;
                    int randomDir = (std::rand() % 4);
                    if (randomDir == 0) {
                        setDirection(up);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            distanceBeforeTurning--;
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(up);
                            return;
                        }
                    } else if (randomDir == 1) {
                        setDirection(right);
                        if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(right);
                            return;
                        }
                    } else if (randomDir == 2) {
                        setDirection(down);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(down);
                            return;
                        }
                    } else {
                        setDirection(left);
                        if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            distanceBeforeTurning--;
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            distanceBeforeTurning--;
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            distanceBeforeTurning--;
                            return;
                        } else {
                            setDirection(left);
                            return;
                        }
                    }
                }
            } else if (getDirection() == down) {
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1))  {
                    this->moveTo(this->getX(), this->getY()-1);
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()-1);
                    }
                    return;
                } else {
                    distanceBeforeTurning = (std::rand() % 6) + 1;
                    int randomDir = (std::rand() % 4);
                    if (randomDir == 0) {
                        setDirection(up);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(up);
                            return;
                        }
                    } else if (randomDir == 1) {
                        setDirection(right);
                        if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(right);
                            return;
                        }
                    } else if (randomDir == 2) {
                        setDirection(down);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(down);
                            return;
                        }
                    } else {
                        setDirection(left);
                        if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else {
                            setDirection(left);
                            return;
                        }
                    }
                }
            } else if (getDirection() == left) {
                if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY()))  {
                    this->moveTo(this->getX()-1, this->getY());
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()-1, this->getY());
                    }
                    return;
                } else {
                    distanceBeforeTurning = (std::rand() % 6) + 1;
                    int randomDir = (std::rand() % 4);
                    if (randomDir == 0) {
                        setDirection(up);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(up);
                            return;
                        }
                    } else if (randomDir == 1) {
                        setDirection(right);
                        if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(right);
                            return;
                        }
                    } else if (randomDir == 2) {
                        setDirection(down);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(down);
                            return;
                        }
                    } else {
                        setDirection(left);
                        if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else {
                            setDirection(left);
                            return;
                        }
                    }
                }
            } else {
                if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY()))  {
                    this->moveTo(this->getX()+1, this->getY());
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else {
                    distanceBeforeTurning = (std::rand() % 6) + 1;
                    int randomDir = (std::rand() % 4);
                    if (randomDir == 0) {
                        setDirection(up);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(up);
                            return;
                        }
                    } else if (randomDir == 1) {
                        setDirection(right);
                        if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(right);
                            return;
                        }
                    } else if (randomDir == 2) {
                        setDirection(down);
                        if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            setDirection(left);
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else {
                            setDirection(down);
                            return;
                        }
                    } else {
                        setDirection(left);
                        if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                            this->moveTo(this->getX()-1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()-1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                            setDirection(right);
                            this->moveTo(this->getX()+1, this->getY());
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX()+1, this->getY());
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                            setDirection(down);
                            this->moveTo(this->getX(), this->getY()-1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()-1);
                            }
                            return;
                        } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                            setDirection(up);
                            this->moveTo(this->getX(), this->getY()+1);
                            distanceBeforeTurning--;
                            if (goodie != nullptr) {
                                goodie->moveTo(this->getX(), this->getY()+1);
                            }
                            return;
                        } else {
                            setDirection(left);
                            return;
                        }
                    }
                }
            }
        } else {
            distanceBeforeTurning = (std::rand() % 6) + 1;
            int randomDir = (std::rand() % 4);
            if (randomDir == 0) {
                setDirection(up);
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                    this->moveTo(this->getX(), this->getY()+1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()+1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                    setDirection(right);
                    this->moveTo(this->getX()+1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                    setDirection(down);
                    this->moveTo(this->getX(), this->getY()-1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()-1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                    setDirection(left);
                    this->moveTo(this->getX()-1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()-1, this->getY());
                    }
                    return;
                } else {
                    setDirection(up);
                    return;
                }
            } else if (randomDir == 1) {
                setDirection(right);
                if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                    this->moveTo(this->getX()+1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                    setDirection(up);
                    this->moveTo(this->getX(), this->getY()+1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()+1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                    setDirection(down);
                    this->moveTo(this->getX(), this->getY()-1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()-1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                    setDirection(left);
                    this->moveTo(this->getX()-1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()-1, this->getY());
                    }
                    return;
                } else {
                    setDirection(right);
                    return;
                }
            } else if (randomDir == 2) {
                setDirection(down);
                if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                    this->moveTo(this->getX(), this->getY()-1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()-1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                    setDirection(right);
                    this->moveTo(this->getX()+1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                    setDirection(up);
                    this->moveTo(this->getX(), this->getY()+1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()+1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                    setDirection(left);
                    this->moveTo(this->getX()-1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()-1, this->getY());
                    }
                    return;
                } else {
                    setDirection(down);
                    return;
                }
            } else {
                setDirection(left);
                if (getWorld()->canAgentMoveTo(this, this->getX()-1, this->getY())) {
                    this->moveTo(this->getX()-1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()-1, this->getY());
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX()+1, this->getY())) {
                    setDirection(right);
                    this->moveTo(this->getX()+1, this->getY());
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()-1)) {
                    setDirection(down);
                    this->moveTo(this->getX(), this->getY()-1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX(), this->getY()-1);
                    }
                    return;
                } else if (getWorld()->canAgentMoveTo(this, this->getX(), this->getY()+1)) {
                    setDirection(up);
                    this->moveTo(this->getX(), this->getY()+1);
                    distanceBeforeTurning--;
                    if (goodie != nullptr) {
                        goodie->moveTo(this->getX()+1, this->getY());
                    }
                    return;
                } else {
                    setDirection(left);
                    return;
                }
            }
        }
        
    }
}

bool RegularThiefBot::isShootingRobot() const {
    return true;
}

