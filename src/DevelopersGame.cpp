#include <iostream>
#include <cstdio>
#include <cctype>
#include <sstream>
#include "../include/conio.h"
#include <cstdlib>
#include <cstdio>
#include <vector>
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
    GOLD_SWORD = 25,
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

enum class Armor
{
	NOTHING = 0,
	LEATHER = 10,
	GOLDEN = 20,
	IRON = 30,
	DIAMOND = 40,
	STEEL = 50
}; 

string i2s(int i)
{
	if (i == 0) return "0";
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
    Armor wearing;
    int maxhp; // not implemented yet 
    Enemy()
    {
        name = "Unknown";
        holding = (Weapon)((rand() % 4 + 1) * 10);
        wearing = (Armor)((rand() % 6) * 10);
        hp = rand() % (150 - 100 + 1) + 100;
    }
    Enemy(string sb, int sn, Weapon sthh, Armor sthw) : name(sb), hp(sn), holding(sthh), wearing(sthw), maxhp(100) {}
    virtual ~Enemy() {}
    bool check() { return hp <= 0; }
    string describe() { return name + string(" HP:") + i2s(hp) + string(" Attack Stength:") + i2s((int)holding) + string(" Defense percent:") + i2s((int)wearing); }
    int hurt(Enemy &target)
    {
        int val = rand() % (int)((int)holding * 1.5) * (1 - (int)target.wearing * 0.01);
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

void red_screen()
{
	#ifdef _WIN32
	system("color 47");
	#else
	cout << "\033[1;41m";
	#endif
}

void green_screen()
{
	#ifdef _WIN32
	system("color 27");
	#else
	cout << "\033[1;42m";
	#endif
}

void restore_screen()
{
	#ifdef _WIN32
	system("color 07");
	#else
	cout << "\033[0m";
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

int input_level()
{
lchoose:
	say("Select level(0 for alternative level, other for main level):");
	char c = getch();
	cout << c;
	cout << endl;
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	say(make_sayable("Incorrect level, please choose again."));
	goto lchoose;
}

// true for normal, false for PvP
int decide(string prompt, bool mode)
{
dochoose:
	say(prompt);
	char c = getch();
	cout << c;
	if (mode && c == '3')
	{
		cout << endl;
		return 3;
	}
	if (c == '1')
	{
		cout << endl;
		return 1;
	}
	if (c == '2')
	{
		cout << 2;
		cout << endl;
		return 2;
	}
	cout << endl;
	say(make_sayable("Invalid choice, please choose again."));
	goto dochoose;
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
    cout << "Warning: This game will change you terminal's color to black background and white foreground." << endl;
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

void output_level(int n)
{
	say(make_sayable(string("Seed:") + i2s(n)));
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
            player.hp = 100;
            player.holding = Weapon::FIST;
            player.wearing = Armor::NOTHING;
            mysleep(2000);
            return false;
        }
        say(make_sayable(player.describe()));
select:
        int c;
        c = decide("Select your move(1 for attack, 2 for heal, 3 for run):", true);
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
                mysleep(2000);
                break;
            case 2:
                say(make_sayable("Type you.heal(); to heal!"));
                typecode("you.heal();");
                cout << endl;
                healval = player.heal();
                green_screen();
                say(make_sayable(string("You healed ") + i2s(healval) + string(" point of HP!")));
                mysleep(2000);
                restore_screen();
                break;
            case 3:
            	say(make_sayable("Type you.run(); to run!"));
            	typecode("you.run();");
            	cout << endl;
                say(make_sayable("You runned away."));
                mysleep(2000);
                return true;
            default:
            	say(make_sayable("Invalid choice, please choose again."));
            	goto select;
        }
        say(make_sayable("It's your enemy's round!"));
        say(make_sayable(e.describe()));
        if (e.check())
        {
            say(make_sayable("Your enemy died!"));
            say(make_sayable(string("It dropped weapon that deals ") + i2s((int)e.holding) + string(" damage!")));
            if ((int)player.holding < (int)e.holding)
            {
                player.holding = e.holding;
                say(make_sayable("You equipped it because it deals more damage."));
            }
            say(make_sayable(string("It dropped armor that defenses ") + i2s((int)e.wearing) + string(" percent of damage!")));
            if ((int)player.wearing < (int)e.wearing)
            {
            	player.wearing = e.wearing;
            	say(make_sayable("You equipped it because it defenses more damage."));
			}
            mysleep(2000);
            return true;
        }
        int val = e.hurt(player);
        red_screen();
        say(make_sayable(string("Your enemy deals ") + i2s(val) + string(" points of damage!")));
        mysleep(2000);
        restore_screen();
    }
}

void pmove(Enemy &me, Enemy &target)
{
select:
	int c;
	c = decide("Select your move(1 for attack, 2 for heal):", false);
	int hurtval;
	int healval;
	switch (c)
	{
		case 1:
			hurtval = me.hurt(target);
            say(make_sayable("Type you.weapon.attack(target); to attack!"));
            typecode("you.weapon.attack(target);");
            cout << endl;
            red_screen();
            say(make_sayable(string("You deal ") + i2s(hurtval) + string(" point of damage!")));
            mysleep(2000);
            restore_screen();
            break;
        case 2:
        	say(make_sayable("Type you.heal(); to heal!"));
            typecode("you.heal();");
            cout << endl;
            healval = me.heal();
            green_screen();
            say(make_sayable(string("You healed ") + i2s(healval) + string(" point of HP!")));
            mysleep(2000);
            restore_screen();
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
			mysleep(2000);
			return false;
		}
		say(make_sayable(p1.describe()));
		pmove(p1, p2);
		if (p2.check())
		{
			say(make_sayable("Player 2 died!"));
			say(make_sayable("Player 1 win!"));
			mysleep(2000);
			return true;
		}
		say(make_sayable(p2.describe()));
		pmove(p2, p1);
	}
}

bool main_level(Enemy &player, int n)
{
	// 2D map coming soon
}

bool alternative_level(Enemy &player)
{
    Enemy e;
    say(make_sayable(e.describe()));
    return combat(player, e);
}

bool choose_level(Enemy &player)
{
    int n = input_level();
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
}

bool weapon_exist(int n)
{
	vector<int> weapons{5, 10, 20, 30, 40};
	for (int i = 0; i < weapons.size(); i++)
	{
		if (weapons[i] == n) return true;
	}
	return false;
}

bool armor_exist(int n)
{
	vector<int> armors{0, 10, 20, 30, 40, 50};
	for (int i = 0; i < armors.size(); i++)
	{
		if (armors[i] == n) return true;
	}
	return false;
}

int main(int argc, char **argv)
{
	restore_screen();
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
		Enemy p1(p1name, 100, Weapon::FIST, Armor::NOTHING);
		Enemy p2(p2name, 100, Weapon::FIST, Armor::NOTHING);
wchoose:
		say("Enter damage that players' weapon deal:");
		int n;
		cin >> n;
		if (!weapon_exist(n))
		{
			say(make_sayable("Incorrect weapon, please choose again."));
			goto wchoose;
		}
		say("Enter percent that players' armor defense:");
		int m;
		cin >> m;
		if (!armor_exist(m))
		{
			say(make_sayable("Incorrect armor, please choose again."));
			goto wchoose;
		}
		p1.holding = (Weapon)n;
		p2.holding = (Weapon)n;
		p1.wearing = (Armor)m;
		p2.wearing = (Armor)m;
		clear_screen();
		pvp(p1, p2);
		goto wchoose;
		return 0;
	}
	// The biggest enemy that you are going to beat is yourself.
    Enemy player("", 100, Weapon::FIST, Armor::NOTHING);
    start(false);
    mysleep(1000);
    clear_screen();
    string name;
    say("Enter your name:");
    getline(cin, name);
    player.name = name;
    int n;
    say("Enter a seed(-1 for random):");
    cin >> n;
    if (n == -1) n = (int)time(0);
	srand(n);
game:
    clear_screen();
    output_level(n);
    choose_level(player);
    goto game;
    return 0;
}
