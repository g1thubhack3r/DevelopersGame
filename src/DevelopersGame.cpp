#include <iostream>
#include <sstream>
#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

enum class Weapon
{
    FIST = 1, 
    WOODEN_SWORD = 10,
    STONE_SWORD = 20,
    IRON_SWORD = 30,
    DIAMOND_SWORD = 40,
    HANDGUN = 50,
    SHOTGUN = 75,
    ASSUALT_RIFLE = 100,
    SNIPER_RIFLE = 175,
    MACHINEGUN = 200,
    LASER_GUN = 1000,
    MASS_POINT = 0x3f3f3f
};

string i2s(int i)
{
    string res = "";
    while (i > 0)
    {
        res += i % 10 + '0';
        i /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

class Enemy
{
public:
    string name;
    int hp;
    Weapon holding;
    Enemy()
    {
        name = "Unknown";
        holding = (Weapon)((rand() % 4 + 1) * 10);
        hp = rand() % 200 + 100;
    }
    Enemy(string sb, int sn, Weapon sth) : name(sb), hp(sn), holding(sth) {}
    virtual ~Enemy() {}
    bool check() { return hp <= 0; }
    string describe() { return name + string(" HP:") + i2s(hp) + string(" Attack Stength:") + i2s((int)holding); }
    int hurt(Enemy &target)
    {
        int val = rand() % (int)holding + 1;
        target.hp -= val;
        return val;
    }
};

void mysleep(int ms)
{
    chrono::milliseconds t(ms);
    this_thread::sleep_for(t);
}

void say(string sentence)
{
    for (int i = 0; i < sentence.size(); i++)
    {
        cout << sentence[i] << flush;
        mysleep(1);
    }
}

string make_sayable(string s)
{
    stringstream tmp;
    tmp << s << endl;
    return tmp.str();
}

void typecode(string code)
{
    for (int i = 0; i < code.size(); i++)
    {
        char c;
        do
        {
            c = getch();
        }
        while (c != code[i]);
        cout << c;
    }
}

void start()
{
    cout << ">";
    mysleep(1000);
    say(make_sayable("DevelopersGame"));
    mysleep(2000);
    cout << "Welcome to Developers' Game" << endl;
    mysleep(1000);
    cout << "Loading";
    for (int i = 1; i <= 3; i++)
    {
        mysleep(1000);
        cout << ".";
    } 
    cout << endl;
}

void output_level()
{
    for (int i = 1; i <= 9; i++)
    {
        say(string("[") + i2s(i) + string("]") + string("-"));
    }
    cout << endl;
}

bool combat(Enemy &player, Enemy &e)
{
    while (true)
    {
        if (player.check())
        {
            say(make_sayable("You died!"));
            return false;
        }
        say(make_sayable("It's your round!"));
        say(make_sayable(string("Your HP:") + i2s(player.hp)));
select:
        say("Select your move(1 for attack, 2 for heal, 3 for run):");
        int c;
        cin >> c;
        int hurtval;
        int healval;
        switch (c)
        {
            case 1:
                hurtval = player.hurt(e);
                say(make_sayable("Type you.weapon.attack(target); to attack!"));
                typecode("you.weapon.attack(target);");
                cout << endl;
                say(make_sayable(string("You deal ") + i2s(hurtval) + string(" point of damage!")));
                break;
            case 2:
                say(make_sayable("Type you.heal(); to heal!"));
                typecode("you.heal();");
                cout << endl;
                healval = rand() % 25 + 1;
                player.hp += healval;
                say(make_sayable(string("You healed ") + i2s(healval) + string(" point of HP!")));
                break;
            case 3:
                cout << "You runned away." << endl;
                return true;
        }
        say(make_sayable("It's your enemy's round!"));
        say(make_sayable(string("Enemy's HP:") + i2s(e.hp)));
        if (e.check())
        {
            say(make_sayable("Your enemy died!"));
            say(make_sayable(string("It dropped weapon that deals ") + i2s((int)e.holding) + string(" damage!")));
            if ((int)player.holding < (int)e.holding)
            {
                player.holding = e.holding;
                say(make_sayable("You equipped it because it deals more damage."));
            }
            return true;
        }
        int val = e.hurt(player);
        say(make_sayable(string("Your enemy deals ") + i2s(val) + string(" points of damage!")));
    }
}

bool main_level(Enemy &player, int n)
{

}

bool alternative_level(Enemy &player)
{
    Enemy e;
    say(make_sayable(e.describe()));
    return combat(player, e);
}

bool choose_level(Enemy &player)
{
choose:
    int n;
    say("Select level(0 for alternative level, other for main level):");
    cin >> n;
    if (n == 0)
    {
        system("cls");
        return alternative_level(player);
    }
    else if (n >= 1 && n <= 9)
    {
        system("cls");
        return main_level(player, n);
    }
    else
    {
        say(make_sayable("Incorrect level, please choose again."));
        goto choose;
    }
}

int main()
{
game:
    srand(time(0));
    // The biggest enemy that you are going to beat is yourself.
    Enemy player("", 100, (Weapon)10);
    start();
    mysleep(1000);
    system("cls");
    string name;
    say("Enter your name:");
    cin >> name;
    player.name = name;
    system("cls");
    output_level();
    if (!choose_level(player)) goto game;
    return 0;
}
