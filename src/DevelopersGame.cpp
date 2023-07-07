#include <iostream>
#include <sstream>
#include "../include/conio.h"
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

enum class Weapon
{
    FIST = 5, 
    WOODEN_SWORD = 10,
    STONE_SWORD = 20,
    IRON_SWORD = 30,
    DIAMOND_SWORD = 40,
    HANDGUN = 50,
    SHOTGUN = 60,
    ASSUALT_RIFLE = 75,
    SNIPER_RIFLE = 80,
    MACHINEGUN = 90,
    LASER_GUN = 1000,
    FAT_MAN = 10000,
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
        hp = rand() % (150 - 50 + 1) + 50;
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
    int heal()
    {
    	int val = rand() % 25 + 1;
    	if (hp + val > 100) val = 100 - hp;
    	hp += val;
    	return val;
	}
};

void clear_screen()
{
	#ifdef _WIN32
	system("cls");
	#else
	cout << "\33[2J";
	#endif
}

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

void start(bool pvp_mode)
{
    cout << ">";
    mysleep(1000);
    if (pvp_mode) say(make_sayable("DevelopersGame PvP"));
	else say(make_sayable("DevelopersGame")); 
    mysleep(2000);
    cout << "Welcome to Developers' Game" << endl;
    mysleep(1000);
    if (pvp_mode) cout << "Mode:PvP" << endl;
	else cout << "Mode:Normal" << endl;
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
                healval = player.heal();
                say(make_sayable(string("You healed ") + i2s(healval) + string(" point of HP!")));
                break;
            case 3:
                say(make_sayable("You runned away."));
                return true;
            default:
            	say(make_sayable("Invalid choice, please choose again."));
            	goto select;
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

void pmove(Enemy &me, Enemy &target)
{
	say("Select your move(1 for attack, 2 for heal):");
	int c;
	cin >> c;
	int hurtval;
	int healval;
select:
	switch (c)
	{
		case 1:
			hurtval = me.hurt(target);
            say(make_sayable("Type you.weapon.attack(target); to attack!"));
            typecode("you.weapon.attack(target);");
            cout << endl;
            say(make_sayable(string("You deal ") + i2s(hurtval) + string(" point of damage!")));
            break;
        case 2:
        	say(make_sayable("Type you.heal(); to heal!"));
            typecode("you.heal();");
            cout << endl;
            healval = me.heal();
            say(make_sayable(string("You healed ") + i2s(healval) + string(" point of HP!")));
            break;
        default:
        	say(make_sayable("Invalid choice, please choose again."));
            goto select;
	}
}


bool pvp(Enemy &p1, Enemy &p2)
{
	while (true)
	{
		if (p1.check())
		{
			say(make_sayable("Player 1 died!"));
			say(make_sayable("Player 2 win!"));
		}
		say(make_sayable(string("Player 1's HP:") + i2s(p1.hp)));
		pmove(p1, p2);
		if (p2.check())
		{
			say(make_sayable("Player 2 died!"));
			say(make_sayable("Player 1 win!"));
		}
		say(make_sayable(string("Player 2's HP:") + i2s(p2.hp)));
		pmove(p2, p1);
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
        clear_screen();
        return alternative_level(player);
    }
    else if (n >= 1 && n <= 9)
    {
        clear_screen();
        return main_level(player, n);
    }
    else
    {
        say(make_sayable("Incorrect level, please choose again."));
        goto choose;
    }
}

bool weapon_exist(int n)
{
	if (n >= 100) return false;
	if (n != 5 && n != 75 && n / 10 != 0) return false;
	return true;
}

int main(int argc, char **argv)
{
	srand(time(0));
	if (argc != 1)
	{
		start(true);
		mysleep(1000);
		clear_screen();
		string p1name;
		say("Enter Player 1's name:");
		getline(cin, p1name);
		string p2name;
		say("Enter Player 2's name:");
		getline(cin, p2name);
		Enemy p1(p1name, 100, (Weapon)1);
		Enemy p2(p2name, 100, (Weapon)1);
wchoose:
		say("Enter damage that players' weapon deal:");
		int n;
		cin >> n;
		if (!weapon_exist(n))
		{
			say(make_sayable("Incorrect weapon, please choose again."));
			goto wchoose;
		}
		p1.holding = (Weapon)n;
		p2.holding = (Weapon)n;
		clear_screen();
		pvp(p1, p2);
		return 0;
	}
game:
    // The biggest enemy that you are going to beat is yourself.
    Enemy player("", 100, (Weapon)10);
    start(false);
    mysleep(1000);
    clear_screen();
    string name;
    say("Enter your name:");
    getline(cin, name);
    player.name = name;
    clear_screen();
    output_level();
    if (!choose_level(player)) goto game;
    return 0;
}
