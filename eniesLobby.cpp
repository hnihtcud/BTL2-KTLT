#include "eniesLobby.h"
#include <iostream>
/*
 * BattleContext
 */
BattleContext::BattleContext() {
    turnCount = 0;
    morale = 0;
    alarmLevel = 0;
    rescueProgress = 0;
    escapeProgress = 0;
    busterCallTimer = 0;
    mainGateDestroyed = false;
    robinRescued = false;
    bridgeOpened = false;
    battleEnded = false;
    istargetkilled = false;
    resultCode = "";
}

void BattleContext::nextTurn() {
    this -> turnCount++;
}

/*
 * Character
 */
Character::Character() {
    name = "";
    hp = 0;
    maxHp = 0;
    atk = 0;
    def = 0;
    speed = 0;
    energy = 0;
    alive = false;
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy) {
    this -> name = name;
    this -> hp = hp;
    this -> maxHp = hp;
    this -> atk = atk;
    this -> def = def;
    this -> speed = speed;
    this -> energy = energy;
    if(hp <= 0)
        alive = false;
    else
        alive = true;
}

Character::~Character() {
    this -> name = "";
    this -> hp = 0;
    this -> maxHp = 0;
    this -> atk = 0;
    this -> def = 0;
    this -> speed = 0;
    this -> energy = 0;
}

int Character::attack(Building* target, BattleContext& context) {
    return 0;
}

int Character::specialSkill(Building* target, BattleContext& context) {
    return 0;
}

void Character::endTurn(BattleContext& context) {
    return ;
}

void Character::receiveDamage(int damage)
{
    int r_damage = damage - this -> def;
    if(r_damage < 0) r_damage = 0;
    this -> hp-= r_damage;
    if(this -> hp <= 0){
        this -> hp  = 0;
        this -> alive = false;
    } 
}
void Character::receiveDef(int def){
    this -> def-= def;
    if(this -> def < 0)   this -> def = 0;
}
void Character::receiveSpeed(int speed){
    this -> speed-= speed;
    if(this -> speed < 0)   this -> speed = 0;
}
void Character::receiveHP(int hp){
    this -> hp+= hp;
    if(this -> hp > this -> maxHp)
        this -> hp = this -> maxHp;
}
bool Character::isAlive() const
{
    // TODO: implement
    return this -> alive;
}

string Character::getName() const
{
    // TODO: implement
    return this -> name;
}

int Character::getHP() const
{
    // TODO: implement
    return this -> hp;
}
int Character::getSpeed() const{
    return this -> speed;
}
int Character::getEnergy() const
{
    // TODO: implement
    return this -> energy;
}
int Character::getMaxHp() const{
    return this -> maxHp;
}
int Character::getdef() const{
    return this -> def;
}

bool Character::isStrawHat() const {
    if(this -> name == "Luffy" || this -> name == "Zoro" || this -> name == "Sanji"
    || this -> name == "Usopp" || this -> name == "Nami" || this -> name == "Franky"
    || this -> name == "Chopper")
        return true;
    return false;
}

bool Character::isCP9() const {
    if(this -> name == "Lucci" || this -> name == "Kaku" || this -> name == "Jabra"
    || this -> name == "Blueno" || this -> name == "Kalifa" || this -> name == "Kumadori"
    || this -> name == "Fukurou")
        return true;
    return false;
}

/*
 * StrawHat
 */
StrawHat::StrawHat() : Character()
{
    bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
: Character(name, hp, atk, def, speed, energy)
{
    this -> bounty = bounty;
}


bool StrawHat::isStrawHat() const
{
    return true;
}

string StrawHat::str() const
{
    return "StrawHat[name=" + this -> name + ", hp=" + to_string(this -> hp) + ", atk=" + to_string(this -> atk)
    + ", def=" + to_string(this -> def) + ", speed=" + to_string(this -> speed) + ", energy=" + to_string(this -> energy)
    + ", bounty=" + to_string(this -> bounty) + "]";
}


/*
 * Luffy
 */
Luffy::Luffy(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Luffy::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = 0;
    if(this -> hp * 10 > 5 * this -> maxHp){
        damage = this -> atk;
    }
    else if(this -> hp * 10 > 3 * this -> maxHp && this -> hp * 10 <= 5 * this -> maxHp){
        damage = ceil((this -> atk * 115) / 100.0);
    }
    else{
         damage = ceil((this -> atk * 130) / 100.0);
    }
    target -> receiveDamage(damage);
    if(alive && target->isAlive() == false){
        context.istargetkilled = true;
        context.morale+= 5;
    }
    if(context.morale <0) context.morale = 0;
    if(context.morale > 100) context.morale = 100;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Luffy::specialSkill(Character *target, BattleContext &context)
{
    int damage = 0;
    if(this -> energy >= 20 && this -> hp * 100 >= 15 * this -> maxHp){
        bool alive = target -> isAlive();
        this -> energy-=20;
        damage = this -> atk * 2;
        this -> speed+= 15;
        this -> atk+= 15;
        this->hp-= ceil((8 * this -> maxHp)/100.0);
        if(this -> hp <= 0){
            this -> hp  = 0;
            this -> alive = false;
        }
        target -> receiveDamage(damage);
        if(alive && target->isAlive() == false)
            context.istargetkilled = true;
        context.alarmLevel+= 10;
        if(context.alarmLevel <0) context.alarmLevel = 0;
        if(context.alarmLevel > 100) context.alarmLevel = 100;
        
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    else return 0;
}

int Luffy::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = 0;
    if(this -> hp * 10 > 5 * this -> maxHp){
        damage = this -> atk;
    }
    else if(this -> hp * 10 > 3 * this -> maxHp && this -> hp * 10 <= 5 * this -> maxHp){
        damage = ceil((this -> atk * 115) / 100.0);
    }
    else{
         damage = ceil((this -> atk * 130) / 100.0);
    }
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
    return damage;
}

int Luffy::specialSkill(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = 0;
    if(this -> energy >= 20 && this -> hp * 100 >= 15 * this -> maxHp){
        this -> energy-=20;
        damage = this -> atk * 2;
        this -> speed+= 15;
        this -> atk+= 15;
        this -> hp-= ceil((8 * this -> maxHp)/100.0);
        if(this -> hp <= 0){
            this -> hp  = 0;
            this -> alive = false;
        }
        target -> receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
        context.alarmLevel+= 10;
        if(context.alarmLevel <0) context.alarmLevel = 0;
        if(context.alarmLevel > 100) context.alarmLevel = 100;
        return damage;
    }
    else return 0;
}

void Luffy::endTurn(BattleContext &context)
{
    if(this -> hp * 10 <= 3 * this -> maxHp){
        context.morale+= 3;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
    }
    if(context.istargetkilled){
        this -> energy+= 5;
        if(this -> energy <0) this -> energy = 0;
        if(this -> energy > 100) this -> energy = 100;
        context.istargetkilled = false;
    }
}

/*
 * Zoro
 */
Zoro::Zoro(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Zoro::attack(Character *target, BattleContext &context)
{
    int damage = ceil((20 * this -> def)/100.0) + this -> atk;
    bool alive = target -> isAlive();
    if(target -> getHP() * 10 < 4 * target -> getMaxHp())
        damage = ceil((damage * 115)/100.0);
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Zoro::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 15){
    bool alive = target -> isAlive();
    this -> energy-= 15;
    int damage = ceil((220 * this -> atk)/100.0);
    if(target -> getHP() * 10 < 5 * target->getMaxHp()){
        damage = ceil((150 * double(damage))/100.0);
    }
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        this -> energy+= 8;
        context.morale+= 4;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        if(this -> energy <0) this -> energy = 0;
        if(this -> energy > 100) this -> energy = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}
    return 0;
}

int Zoro::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = ceil((20 * this -> def)/100.0) + this -> atk;
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
    return damage;
}

int Zoro::specialSkill(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    if(this -> energy >= 15){
    this -> energy-= 15;
    int damage = ceil((220 * this -> atk)/100.0);
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
    return damage;
}
    return 0;
}

void Zoro::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.morale+= 6;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        this -> atk = ceil((105 * this -> atk)/100.0);
        context.istargetkilled = false;
    }
}

/*
 * Sanji
 */
Sanji::Sanji(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Sanji::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = this -> atk + ceil((50 * double(this -> speed))/100.0);
    if(target -> getdef() < this -> def && target -> isCP9())
        damage = ceil((110 * double(damage))/100.0);
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Sanji::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 18){
        this -> energy-= 18;
        bool alive = target -> isAlive();
        int firstdef = target->getdef();
        int damage = ceil((210 * double(this -> atk)) / 100.0);
        Jabra* jabra = dynamic_cast<Jabra*>(target);
        target -> receiveDamage(damage);
        if(target -> isCP9() && jabra != nullptr) 
            target -> receiveDef(12);
        else if(target -> isCP9())
            target -> receiveDef(8);
        if(alive && target -> isAlive() == false)
            context.istargetkilled = true;
        return (damage - firstdef > 0) ? damage - firstdef : 0;
    }
    return 0;
}

int Sanji::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = this -> atk + ceil((50 * double(this -> speed))/100.0);
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
    return damage;
}

int Sanji::specialSkill(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    if(this -> energy >= 18){
        this -> energy-= 18;
        int damage = ceil((210 * double(this -> atk)) / 100.0);
        target -> receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) {
            target->onDestroyed(context);
        }
        return damage;
    }
    return 0;
}

void Sanji::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.morale+= 8;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        this -> atk += ceil((10 * this -> atk)/100.0);
        context.istargetkilled = false;
    }
}

/*
 * Nami
 */
Nami::Nami(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Nami::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = 0;
    if(target -> isCP9()){
        damage = this -> atk + floor((30 * double(target -> getdef()))/100.0);
    }
    else
        damage = ceil((50 * double(this -> atk))/100.0);
    target->receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        context.morale+= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Nami::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 20){
        bool alive = target -> isAlive();
        this -> energy-= 20;
        int damage = this -> atk + 40;
        target -> receiveSpeed(10);
        if(!(target -> isCP9()))
            damage+= ceil((50 * double(damage))/100.0);
        context.busterCallTimer+= 1;
        context.alarmLevel-= 5;
        if(context.alarmLevel < 0) context.alarmLevel = 0;
        target->receiveDamage(damage);
        if(alive && target -> isAlive() == false){
            context.istargetkilled = true;
            context.morale+= 5;
            if(context.morale <0) context.morale = 0;
            if(context.morale > 100) context.morale = 100;
        }
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
}

int Nami::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = 0;
    damage = ceil((50 * double(this -> atk))/100.0);
    target->receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
    return damage;
}

int Nami::specialSkill(Building *target, BattleContext &context)
{
    if(this -> energy >= 20){
        bool wasDestroyed = target->isDestroyed();
        this -> energy-= 20;
        int damage = this -> atk + 40;
        damage+= ceil((50 * double(damage))/100.0);
        context.busterCallTimer+= 1;
        if(context.busterCallTimer < 0) context.busterCallTimer = 0;
        context.alarmLevel-= 5;
        if(context.alarmLevel <0) context.alarmLevel = 0;
        if(context.alarmLevel > 100) context.alarmLevel = 100;
        target->receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
        return damage;
    }
    return 0;
}

void Nami::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
        this -> energy+= 6;
        if(this -> energy <0) this -> energy = 0;
        if(this -> energy > 100) this -> energy = 100;
    }
}

/*
 * Chopper
 */
Chopper::Chopper(string name, int hp, int atk, int def,
                 int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Chopper::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = this -> atk;
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
            context.istargetkilled = true;
    return (damage - target->getdef() > 0) ? damage - target->getdef() : 0;
}

int Chopper::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 15 && target -> isStrawHat()){
        this -> energy-= 15;
        int heal = 35 + ceil((50 * double(this -> atk))/100.0);
        target->receiveHP(heal);
        Luffy* luffy = dynamic_cast<Luffy*>(target);
        if(luffy != nullptr){
            context.morale+= 5;
            if(context.morale <0) context.morale = 0;
            if(context.morale > 100) context.morale = 100;
        }
        return 0;
    }
    return 0;
}

int Chopper::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = this -> atk;
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
    return damage;
}

void Chopper::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
    }
}

/*
 * Usopp
 */
Usopp::Usopp(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Usopp::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = this -> atk;
    if(!(target->isCP9()))
        damage = ceil((50 * double(this -> atk))/100.0);
    else if(target -> getSpeed() < 50)
        damage = ceil((120 * double(damage))/100.0);
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Usopp::specialSkill(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = 0;
    if(this-> energy >= 16){
        this -> energy -= 16;
        damage = ceil((80 * double(this -> atk))/100.0);
        target -> receiveSpeed(12);
        context.escapeProgress+= 8;
        if(context.escapeProgress <0) context.escapeProgress = 0;
        if(context.escapeProgress > 100) context.escapeProgress = 100;
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false)
            context.istargetkilled = true;
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
    
}

int Usopp::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = this -> atk;
    damage = ceil((50 * double(this -> atk))/100.0);
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
    return damage;
}

int Usopp::specialSkill(Building *target, BattleContext &context)
{
    int damage = 0;
    if(this-> energy >= 16){
        bool wasDestroyed = target->isDestroyed();
        this -> energy -= 16;
        damage = ceil((80 * double(this -> atk))/100.0);
        context.escapeProgress+= 8;
        if(context.escapeProgress <0) context.escapeProgress = 0;
        if(context.escapeProgress > 100) context.escapeProgress = 100;
        target -> receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
        return damage;
    }
    
    return 0;
}

void Usopp::endTurn(BattleContext &context)
{
    if(context.istargetkilled)
        context.istargetkilled = false;
    context.morale+= 10;
    if(context.morale <0) context.morale = 0;
    if(context.morale > 100) context.morale = 100;
}

/*
 * Franky
 */
Franky::Franky(string name, int hp, int atk, int def,
               int speed, int energy, long long bounty)
: StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    // TODO: implement
}

int Franky::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = this -> atk + ceil((30 * double(this -> def))/100.0);
    damage = ceil((110 * double(damage))/100.0);
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        context.morale+= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Franky::specialSkill(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = 0;
    if(this -> energy >= 30){
        this -> energy-= 30;
        damage = ceil((120 * double(this -> atk))/100.0);
        target -> receiveDamage(damage);
    }
    else if(this -> energy >= 20){
        this -> energy-= 20;
        damage = ceil((180 * double(this -> atk))/100.0);
        target -> receiveSpeed(8);
        Lucci* lucci = dynamic_cast<Lucci*>(target);
        if(lucci != nullptr)
            damage = ceil((120 * double(damage))/100.0);
        target -> receiveDamage(damage);
    }
    if(alive && target -> isAlive() == false){
        context.istargetkilled = true;
        context.morale+= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Franky::attack(Building *target, BattleContext &context)
{
    bool wasDestroyed = target->isDestroyed();
    int damage = this -> atk + ceil((30 * double(this -> def))/100.0);
    target -> receiveDamage(damage);
    if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
    return damage;
}

int Franky::specialSkill(Building *target, BattleContext &context)
{
    int damage = 0;
    bool wasDestroyed = target->isDestroyed();
    if(this -> energy >= 30){
        this -> energy-= 30;
        damage = target -> getHP();
        target -> receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
        return damage;
    }
    else if(this -> energy >= 20){
        this -> energy-= 20;
        damage = ceil((180 * double(this -> atk))/100.0);
        target -> receiveDamage(damage);
        if (!wasDestroyed && target->isDestroyed()) 
            target->onDestroyed(context);
        return damage;
    }
    return 0;
}

void Franky::endTurn(BattleContext &context)
{
    if(this -> hp * 10 > 7 * this -> maxHp)
        this -> def+= 5;
    if(this -> hp * 10 < 3 * this -> maxHp)
        this -> atk= ceil((110 * double(this -> atk))/100.0);
    if(context.istargetkilled){
        context.istargetkilled = false;
    }
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character() {
    doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki)
: Character(name, hp, atk, def, speed, energy)
{
    this -> doriki = doriki;
}

bool CP9Agent::isCP9() const
{
    return true;
}

string CP9Agent::str() const
{
    return "CP9[name=" + this -> name + ", hp=" + to_string(this -> hp) + ", atk=" + to_string(this -> atk)
    + ", def=" + to_string(this -> def) + ", speed=" + to_string(this -> speed) + ", energy=" + 
    to_string(this -> energy) + ", doriki=" + to_string(this -> doriki) + "]";
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def,
             int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Lucci::attack(Character *target, BattleContext &context)
{
    bool alive = target -> isAlive();
    int damage = this -> atk + ceil((5 * this -> doriki)/100.0);
    if(target -> getHP() * 10 < 5 * target -> getMaxHp())
        damage = ceil((120 * double(damage))/100.0);
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;

}

int Lucci::specialSkill(Character *target, BattleContext &context)
{
    int damage = 0;
    bool alive = target -> isAlive();
    if(this -> energy >= 25){
        this -> energy-= 25;
        damage = ceil((280 * double(this -> atk))/100.0) + floor((50 * double(target -> getdef()))/100.0);
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false)
            context.istargetkilled = true;
        if(context.istargetkilled == true){
            context.morale-= 10;
            if(context.morale <0) context.morale = 0;
            if(context.morale > 100) context.morale = 100;
        }
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;    
}


void Lucci::endTurn(BattleContext &context)
{
    if(this -> hp * 10 < 4 * this -> maxHp)
        this -> atk = ceil((105 * this -> atk)/100.0);
    if(context.istargetkilled)
        context.istargetkilled = false;
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def,
           int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Kaku::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        context.morale-= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Kaku::specialSkill(Character *target, BattleContext &context)
{
    int damage = 0, totaldmg = 0;
    
    if(this -> energy >= 20){
        bool alive = target -> isAlive();
        this -> energy-= 20;
        damage = ceil((120 * double(this -> atk))/100.0);
        target -> receiveDamage(damage);
        totaldmg = (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
        if(alive && target -> isAlive() == false){
             context.morale-= 5;
                if(context.morale <0) context.morale = 0;
                if(context.morale > 100) context.morale = 100;
            context.istargetkilled = true;
            return totaldmg;
        }
        else{
            damage = this -> atk;
            target -> receiveDamage(damage);
            totaldmg+= (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
            if(alive && target -> isAlive() == false){
                context.istargetkilled = true;
                context.morale-= 5;
                if(context.morale <0) context.morale = 0;
                if(context.morale > 100) context.morale = 100;
                return totaldmg;
            }
            else{
            damage = ceil((80 * double(this -> atk))/100.0);
            target -> receiveDamage(damage);
            totaldmg+= (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
            if(alive && target -> isAlive() == false){
                context.morale-= 5;
                if(context.morale <0) context.morale = 0;
                if(context.morale > 100) context.morale = 100;
                context.istargetkilled = true;
            }
        }
    }
    return totaldmg;
    }
    return 0;
}


void Kaku::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        
        context.istargetkilled = false;
    }
}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def,
             int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Jabra::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Jabra::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 18){
        this -> energy-= 18;
        bool alive = target -> isAlive();
        int damage = ceil((150 * double(this -> atk))/100.0);
        if(this -> hp * 10 < this -> maxHp * 3)
            damage = ceil((125 * double(damage))/100.0);
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false){
            context.istargetkilled = true;
            context.morale-= 5;
            if(context.morale <0) context.morale = 0;
            if(context.morale > 100) context.morale = 100;
        }
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
}

void Jabra::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
    }
}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def,
               int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Blueno::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        context.morale-= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Blueno::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 15){
        bool alive = target -> isAlive();
        this -> energy-= 15;
        int damage = ceil((130 * double(this -> atk))/100.0);
        if(this -> hp * 10 <= this -> maxHp * 5)
            damage+= 40;
        else
            damage+= 20;
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false){
            context.morale-= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
            context.istargetkilled = true;
        }
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}
    return 0;
}

void Blueno::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
        
    }
}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def,
               int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Kalifa::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Kalifa::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 18){
        bool alive = target -> isAlive();
        this -> energy-= 18;
        int damage = ceil((140 * double(this -> atk))/100.0);
        Nami* nami = dynamic_cast<Nami*>(target);
        if(nami != nullptr)
            context.morale-= 12;
        else
            context.morale-= 8;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        target -> receiveSpeed(6);
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false)
            context.istargetkilled = true;
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
}

void Kalifa::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
    }
}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    // TODO: implement
}

int Kumadori::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false){
        context.morale-= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        context.istargetkilled = true;
    }
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Kumadori::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 16){
        bool alive = target -> isAlive();
        this -> energy-= 16;
        int damage = 30 + ceil((10 * double(this -> doriki))/100.0);
        if(this -> hp * 10 < this -> maxHp * 4)
            damage+= 25;
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false){
            context.morale-= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
            context.istargetkilled = true;
        }
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
}

void Kumadori::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
        
    }
}

/*
 * Fukurou
 */

Fukurou::Fukurou(string name, int hp, int atk, int def,
                 int speed, int energy, int doriki)
: CP9Agent(name, hp, atk, def, speed, energy, doriki)
{

}

int Fukurou::attack(Character *target, BattleContext &context)
{
    int damage = this -> atk;
    bool alive = target -> isAlive();
    target -> receiveDamage(damage);
    if(alive && target -> isAlive() == false)
        context.istargetkilled = true;
    return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
}

int Fukurou::specialSkill(Character *target, BattleContext &context)
{
    if(this -> energy >= 14){
        bool alive = target -> isAlive();
        this -> energy-= 14;
        int damage = ceil((130 * double(this -> atk))/100.0);
        if(this -> istargetminhp){
            damage+= 20;
            this -> istargetminhp = false;
        }
        target -> receiveDamage(damage);
        if(alive && target -> isAlive() == false){
            context.morale-= 6;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
            context.istargetkilled = true;
        }
        return (damage - target -> getdef() > 0) ? damage - target -> getdef() : 0;
    }
    return 0;
}

void Fukurou::endTurn(BattleContext &context)
{
    if(context.istargetkilled){
        context.istargetkilled = false;
    }
}

/*
 * Building
 */
Building::Building(string name, int hp)
{
    this -> name = name;
    this -> hp = hp;
    this -> maxHP = hp;
    if(this -> hp <= 0)
        this -> destroyed = true;
    else
        this -> destroyed = false;

}

Building::~Building()
{
    this -> name = "";
    this -> hp = 0;
    this -> maxHP = 0;
    this -> destroyed = false;
}

void Building::receiveDamage(int damage)
{
    this -> hp-= damage;
    if(this -> hp <= 0){
        this -> hp = 0;
        this -> destroyed = true;
    }
}

bool Building::isDestroyed() const
{
    return destroyed;
}

void Building::onDestroyed(BattleContext &context)
{
    return;
}

string Building::str() const
{
    return "Building[name=" + this -> name + ", hp=" + to_string(this -> hp)
    + ", maxHP=" + to_string(this -> maxHP) + ", destroyed=" + (this -> destroyed ? "true" : "false") + "]";
}
int Building::getHP() const{
    return this -> hp;
}
int Building::getMaxHP() const{
    return this -> maxHP;
}
/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext &context)
{
         
}

void MainGate::onDestroyed(BattleContext &context)
{
    if(this -> destroyed){
        context.mainGateDestroyed = true;
        context.rescueProgress+= 20;
        if(context.rescueProgress <0) context.rescueProgress = 0;
        if(context.rescueProgress > 100) context.rescueProgress = 100;
        context.morale+= 5;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
    }
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext &context)
{
    if(!this -> destroyed){
        context.alarmLevel+= 5;
        if(context.alarmLevel > 100) context.alarmLevel = 100;
    }
}

void Courthouse::onDestroyed(BattleContext &context)
{
    if(this -> destroyed){
        context.alarmLevel-=20;
        if(context.alarmLevel <0) context.alarmLevel = 0;
    }
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext &context){
    if(!this -> destroyed){
    if(context.mainGateDestroyed && !context.robinRescued){
        context.rescueProgress+= 5;
        if(context.rescueProgress <0) context.rescueProgress = 0;
        if(context.rescueProgress > 100) context.rescueProgress = 100;
        if(context.rescueProgress >= 100)
            context.robinRescued = true;
        if(context.rescueProgress >= 100){
        context.morale+= 10;
        if(context.morale <0) context.morale = 0;
        if(context.morale > 100) context.morale = 100;
        }
    }
}
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext &context)
{
    if(!this -> destroyed){
    if(context.robinRescued){
        context.bridgeOpened = true;
        context.escapeProgress+= 5;
        if(context.escapeProgress <0) context.escapeProgress = 0;
        if(context.escapeProgress > 100) context.escapeProgress = 100;
        if(context.escapeProgress >= 100){
            context.battleEnded = true;
            context.resultCode = "STRAW_HAT_WIN";
        }
    }
    }
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext &context)
{
    if(!this -> destroyed){
        context.busterCallTimer-= 1;
        if(context.busterCallTimer < 0) context.busterCallTimer = 0;
        if(context.busterCallTimer <= 0){
            context.battleEnded = true;
            context.resultCode = "BUSTER_CALL";
        }        
    }
}

void BusterCallShip::onDestroyed(BattleContext &context){
    if(this -> destroyed)  
        context.busterCallTimer+= 3;
}

/*
 * EniesLobbyBattle
 */
EniesLobbyBattle::EniesLobbyBattle(const string& filename) {
    maxTurns = 0, strawHatCount = 0, cp9Count = 0, buildingCount = 0;
    strawHats = new Character* [7];
    cp9Agents = new Character* [7];
    buildings = new Building* [5];
    turnOrder = nullptr;
    loadFromFile(filename);
}

EniesLobbyBattle::~EniesLobbyBattle() {
    for(int i = 0; i < strawHatCount; i++){
        delete strawHats[i];
        strawHats[i] = nullptr;
    }
    delete[] strawHats;
    for(int i = 0; i < cp9Count; i++){
        delete cp9Agents[i];
        cp9Agents[i] = nullptr;
    }
    delete[] cp9Agents;
    for(int i = 0; i < buildingCount; i++){
        delete buildings[i];
        buildings[i] = nullptr;
    }
    delete[] buildings;
    while(turnOrder != nullptr){
        TurnNode* temp = turnOrder;
        turnOrder = turnOrder->next;
        delete temp;
    }
}

void EniesLobbyBattle::loadFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
        return;
    string line;
    while(file >> line){
        if(line == "CONTEXT"){
            file >> context.morale >> context.alarmLevel >> context.rescueProgress >> context.escapeProgress >> context.busterCallTimer >> maxTurns;
        }
        else if(line == "STRAW_HAT"){
            int hp, atk, def, speed, energy;
            long long bounty;
            string name;
            file >> name >> hp >> atk >> def >> speed >> energy >> bounty;
            Character *NewMember = nullptr;
            if (name == "Luffy") NewMember = new Luffy(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Zoro") NewMember = new Zoro(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Sanji") NewMember = new Sanji(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Nami") NewMember = new Nami(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Usopp") NewMember = new Usopp(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Chopper") NewMember = new Chopper(name, hp, atk, def, speed, energy, bounty);
            else if (name == "Franky") NewMember = new Franky(name, hp, atk, def, speed, energy, bounty);
            if (NewMember != nullptr) 
                addStrawHat(NewMember);
        }
        else if(line == "CP9"){
            string name;
            int hp, atk, def, speed, energy;
            long long doriki;
            file >> name >> hp >> atk >> def >> speed >> energy >> doriki;
            Character *NewCP9 = nullptr;
            if (name == "Lucci") NewCP9 = new Lucci(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Kaku") NewCP9 = new Kaku(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Jabra") NewCP9 = new Jabra(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Blueno") NewCP9 = new Blueno(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Kalifa") NewCP9 = new Kalifa(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Kumadori") NewCP9 = new Kumadori(name, hp, atk, def, speed, energy, doriki);
            else if (name == "Fukurou") NewCP9 = new Fukurou(name, hp, atk, def, speed, energy, doriki);
            if (NewCP9 != nullptr)
                addCP9Agent(NewCP9);
        }
        else if(line == "BUILDING"){
            int hp;
            string name;
            file >> name >> hp;
            Building *NewBuilding = nullptr;
            if (name == "MainGate") NewBuilding = new MainGate(name, hp);
            else if (name == "Courthouse") NewBuilding = new Courthouse(name, hp);
            else if (name == "TowerOfJustice") NewBuilding = new TowerOfJustice(name, hp);
            else if (name == "BridgeOfHesitation") NewBuilding = new BridgeOfHesitation(name, hp);
            else if (name == "BusterCallShip") NewBuilding = new BusterCallShip(name, hp);
            
            if (NewBuilding != nullptr) addBuilding(NewBuilding);
        }
        
    }
    buildTurnOrder();

    file.close();
}

void EniesLobbyBattle::addStrawHat(Character *character)
{
    if(strawHatCount < 7)
        strawHats[strawHatCount++] = character;
}

void EniesLobbyBattle::addCP9Agent(Character *character)
{
    if(cp9Count < 7)
        cp9Agents[cp9Count++] = character;

}

void EniesLobbyBattle::addBuilding(Building *building)
{
    if(buildingCount < 5)
        buildings[buildingCount++] = building;

}

void EniesLobbyBattle::buildTurnOrder()
{
    TurnNode* last = nullptr;
    for(int i  = 0; i < strawHatCount; i++){
        TurnNode* tmp = new TurnNode();
        tmp -> data = strawHats[i];
        tmp -> next = nullptr;
        if(turnOrder == nullptr){
            turnOrder = tmp;
            last = tmp;
        }
        else{
            last -> next = tmp;
            last = tmp;
        }
    }
    for(int i = 0; i < cp9Count; i++){
        TurnNode* tmp1 = new TurnNode();
        tmp1 -> data = cp9Agents[i];
        tmp1 -> next = nullptr;
        if(turnOrder == nullptr){
            turnOrder = tmp1;
            last = tmp1;
        }
        else{
            last -> next = tmp1;
            last = tmp1;
        }
    }

}

void EniesLobbyBattle::runBattle() {
    checkEndCondition();
   while(context.battleEnded == false && context.turnCount < maxTurns){
        if(turnOrder == nullptr)
            break;
        Character* crt = turnOrder -> data;
        if(crt -> isAlive()){
                processTurn(crt);
        }
        if(turnOrder != nullptr && turnOrder -> next != nullptr){
            TurnNode* head = turnOrder;
            turnOrder = turnOrder -> next;
            head -> next = nullptr;
            TurnNode* a = turnOrder;
            while(a -> next != nullptr)
                a = a -> next;
            a -> next = head;
        }
        processBuildings();
        context.turnCount++;
        checkEndCondition();
        if(context.turnCount >= maxTurns && !context.battleEnded){
            context.battleEnded = true;
            context.resultCode = "TIME_OUT";
        }
    }
}

void EniesLobbyBattle::processTurn(Character* character) {
    if(character -> isCP9()){
        Character* target = nullptr;
        for(int i = 0; i < strawHatCount; i++)
            if(strawHats[i] -> isAlive()){
                target = strawHats[i];
                break;
            }
        if(target != nullptr){
            Fukurou* fukurou = dynamic_cast<Fukurou*>(character);
            if(fukurou != nullptr){
                int minhp = INT_MAX;
                Character* minhp_target= nullptr;
                for(int i = 0; i < strawHatCount; i++){
                    if(strawHats[i] -> isAlive() && strawHats[i] -> getHP() < minhp){
                        minhp_target = strawHats[i];
                        minhp = strawHats[i] -> getHP();
                    }
                }
                if(minhp_target == target)
                    fukurou -> istargetminhp = true;
                else 
                    fukurou -> istargetminhp = false;
            }
            int energy = character -> getEnergy();
            character -> specialSkill(target, context);
            if(energy == character -> getEnergy())
                character -> attack(target, context);
            character -> endTurn(context);
        }
    }
    else{
        Character* targetCP9 = nullptr;
        Building* targetBuilding = nullptr;
        Chopper* chopper = dynamic_cast<Chopper*>(character);
        if(chopper != nullptr){
            Character* target = nullptr;
            if(character -> getEnergy() >= 15){
                int minHP = INT_MAX;
                 for(int i = 0; i < strawHatCount; i++)
                    if(strawHats[i] -> isAlive() && strawHats[i] -> getHP() < minHP){
                        target = strawHats[i];
                        minHP = strawHats[i] -> getHP();
                    }
                if(target != nullptr){
                    character -> specialSkill(target, context);
                    character -> endTurn(context);          
                    return;
                }
            }
        }
        if(!context.mainGateDestroyed){
            for(int i = 0; i < buildingCount; i++)
                if(dynamic_cast<MainGate*>(buildings[i]) != nullptr){
                    targetBuilding = buildings[i];
                    break;
                }
        }
        else{
            for(int i = 0; i < buildingCount; i++)
                if(dynamic_cast<Courthouse*>(buildings[i]) != nullptr  && !buildings[i] -> isDestroyed() && context.alarmLevel >= 50){
                    targetBuilding = buildings[i];
                    break;
                }
        }
        if(targetBuilding == nullptr){
            for(int i = 0; i < buildingCount; i++)
                if(dynamic_cast<BusterCallShip*>(buildings[i]) != nullptr  && !buildings[i] -> isDestroyed() && context.busterCallTimer <= 5){
                    targetBuilding = buildings[i];
                    break;
                }
        }
        if(targetBuilding == nullptr)
            if(context.robinRescued){
                for(int i = 0; i < buildingCount; i++)
                    if(dynamic_cast<BridgeOfHesitation*>(buildings[i]) != nullptr && !buildings[i] -> isDestroyed()){
                        targetBuilding = buildings[i];
                        break;
                    }
                if(targetBuilding == nullptr){
                    for(int i = 0; i < cp9Count; i++)
                        if(cp9Agents[i] -> isAlive()){
                            targetCP9 = cp9Agents[i];
                            break;
                    }
                }
            }
            else{
                for(int i = 0; i < cp9Count; i++)
                    if(cp9Agents[i] -> isAlive()){
                        targetCP9 = cp9Agents[i];
                        break;
                    }
            }
        if (targetBuilding != nullptr) {
            int energy = character->getEnergy();
            character->specialSkill(targetBuilding, context);
            if (energy == character->getEnergy()) 
                character->attack(targetBuilding, context);
            character->endTurn(context);
            return;
        }
        else if (targetCP9 != nullptr) {
            int energy = character->getEnergy();
            character->specialSkill(targetCP9, context);
            if (energy == character->getEnergy()) 
                character->attack(targetCP9, context);
            character->endTurn(context);
            return;
        }
    }
}

void EniesLobbyBattle::processBuildings() {
    for(int i = 0; i < buildingCount; i++)
        buildings[i] -> applyEffect(context);
}

void EniesLobbyBattle::checkEndCondition() {
    if(context.robinRescued && context.escapeProgress >= 100){
        context.battleEnded = true;
        context.resultCode = "STRAW_HAT_WIN";
    }
    else if(context.busterCallTimer <= 0){
        context.battleEnded = true;
        context.resultCode = "BUSTER_CALL";
    }
    else{
        bool cp9 = false, strawhat = false;
        for(int i = 0; i < strawHatCount; i++)
            if(strawHats[i] -> isAlive()){
                strawhat = true;
                break;
            }
        for(int i = 0; i < cp9Count; i++)
            if(cp9Agents[i] -> isAlive()){
                cp9 = true;
                break;
            }
        if(!strawhat){
            context.battleEnded = true;
            context.resultCode = "CP9_WIN";
        }
        else if(!cp9){
            context.battleEnded = true;
            context.resultCode = "STRAW_HAT_WIN_BY_DEFEAT_CP9";
        }
    }
    if(!context.battleEnded && context.turnCount >= maxTurns){
        context.battleEnded = true;
        context.resultCode = "TIME_OUT";
    }
    if(context.battleEnded)
        getResult();
}

string EniesLobbyBattle::getResult() const {
    return context.resultCode + " " + to_string(context.turnCount) + " " + to_string(context.morale) + " " 
    + to_string(context.alarmLevel) + " " + to_string(context.rescueProgress) + " " + to_string(context.escapeProgress) 
    + " " + to_string(context.busterCallTimer);
}