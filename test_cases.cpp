#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "eniesLobby.h"
#include <sstream>
#include <fstream>

#include <sstream>

string str(BattleContext &context)
{
    stringstream ss;

    ss << "resultCode=" << context.resultCode << " "
       << "turnCount=" << context.turnCount << " "
       << "morale=" << context.morale << " "
       << "alarmLevel=" << context.alarmLevel << " "
       << "rescueProgress=" << context.rescueProgress << " "
       << "escapeProgress=" << context.escapeProgress << " "
       << "busterCallTimer=" << context.busterCallTimer;

    return ss.str();
}

void printBeforeAfter(Character *actor, Character *target, string action, BattleContext &context)
{
    cout << "- " << actor->getName() << " " << action << " -> " << target->getName() << endl;
    cout << "actor(before):  " << actor->str() << " | alive=" << (actor->isAlive() ? "true" : "false") << endl;
    cout << "target(before): " << target->str() << " | alive=" << (target->isAlive() ? "true" : "false") << endl;
    int damage = 0;
    if (action == "special")
    {
        damage = actor->specialSkill(target, context);
    }
    else
    {
        damage = actor->attack(target, context);
    }
    cout << "damage: " << damage << endl;
    cout << "actor(after):   " << actor->str() << " | alive=" << (actor->isAlive() ? "true" : "false") << endl;
    cout << "target(after):  " << target->str() << " | alive=" << (target->isAlive() ? "true" : "false") << endl;
}

TEST_CASE("test_222")
{
    ofstream fout("test.txt");
    fout << "BUILDING BusterCallShip 164\n";
    fout << "CP9 Kalifa 106 21 26 54 63 3798\n";
    fout << "STRAW_HAT Chopper 111 96 15 27 70 212315640\n";
    fout << "STRAW_HAT Sanji 120 66 16 33 65 236776335\n";
    fout << "BUILDING TowerOfJustice 206\n";
    fout << "CP9 Blueno 128 26 10 28 71 3291\n";
    fout << "CP9 Jabra 99 53 12 48 77 3286\n";
    fout << "CP9 Fukurou 107 46 27 33 75 2917\n";
    fout << "CONTEXT 53 52 19 24 34 100\n";
    fout << "BUILDING BridgeOfHesitation 173\n";
    fout << "BUILDING MainGate 134\n";
    fout << "STRAW_HAT Franky 143 55 34 14 76 348252451\n";
    fout << "CP9 Lucci 95 31 13 22 76 3701\n";
    fout << "STRAW_HAT Zoro 171 38 27 35 74 584957070\n";
    fout << "BUILDING Courthouse 121\n";
    fout << "CP9 Kumadori 155 35 34 18 79 4941\n";
    fout << "STRAW_HAT Nami 199 18 9 10 52 724680404\n";
    fout << "CP9 Kaku 82 41 12 58 54 2490\n";
    fout << "STRAW_HAT Usopp 129 77 20 55 72 919282253\n";
    fout << "STRAW_HAT Luffy 132 37 14 49 63 532380038\n";
    fout.close();

    EniesLobbyBattle battle("test.txt");
    battle.runBattle();
    CHECK(battle.getResult() == "CP9_WIN 41 52 62 100 78 5");
    remove("test.txt");
}