#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

void validIn(int &val, int min, int max, string prompt);
int rng(int min, int max);
void pause();

void readToArrays(int stat_array[][3], string name_array[], int &amount, string FileName);
void setPlayer(string &name, int stat_array[3]);
void encounter(string player_name, int player_stats[3], string monster_names[], int monster_stats[][3], int monster_amount, int &level, int &experience);
void levelup(int &level, int &experience, int player_stats[3]);
void displayStats(string title, int player_stats[3]);

void loadFile(string &name, int stat_array[3], int &level, int &experience);
void save(string player_name, int player_stats[3], int level, int experience);

const int maxExp = 400;

int main()
{
    int monster_amount, monster_stats[32][3]; // {attack, defense, max health}
    string monster_names[32];
    string MonsterFileName = "Monsters.txt";
    readToArrays(monster_stats, monster_names, monster_amount, MonsterFileName);

    string player_name;
    int player_stats[3] = {100, 100, 1000}; // {attack, defense, max health}
    int level = 1, experience = 0, load;

    validIn(load, 0, 1, "Do you want to load a savefile (1 for Yes, 0 for NO)? ");
    if (load)
        loadFile(player_name, player_stats, level, experience);
    else
        setPlayer(player_name, player_stats);

    int playing, alive;
    do
    {
        encounter(player_name, player_stats, monster_names, monster_stats, monster_amount, level, experience);
        levelup(level, experience, player_stats);
        validIn(playing, 0, 1, "Keep Playing? (1 for Yes, 0 for No): ");
        cout << endl
             << "========================================================================" << endl
             << endl;
    } while (playing);

    save(player_name, player_stats, level, experience);

    pause();

    return 0;
}

void validIn(int &val, int min, int max, string prompt) // only works with values 0 - 9
{
    bool valid;

    cout << prompt;
    cin >> val;

    valid = not(val < min || val > max);
    if (not valid)
    {
        cout << "INVALID INPUT... RE-ENTER..." << endl;
        validIn(val, min, max, prompt);
    }
    else
    {
        return;
    }
}

int rng(int min, int max)
{
    std::srand(std::time(nullptr));

    return int(min + (std::rand() % (max - min)));
}

void readToArrays(int stat_array[][3], string name_array[], int &amount, string FileName)
{
    fstream File;
    File.open(FileName, ios::in);

    amount = 0;
    string line;
    File.seekp(31);
    while (not File.eof())
    {
        getline(File, line);
        name_array[amount] = line;
        getline(File, line);
        stat_array[amount][0] = stoi(line);
        getline(File, line);
        stat_array[amount][1] = stoi(line);
        getline(File, line);
        stat_array[amount][2] = stoi(line);
        ++amount;
    }

    File.close();
}

void setPlayer(string &name, int player_stats[3])
{
    cout << "Greetings brave warri'r! what is thy nameth? ";
    cin.ignore();
    getline(cin, name);
    cout << "Well hath met " << name << "! This is our land of V'ria, once peaceful and thriving, but now overrun with foul monsters. Though it seems a daunting task, would you be willing to try to rid the land of these beasts? (Y/N)?";
    char play;
    cin >> play;
    if (play == 'N' or play == 'n')
    {
        cout << "P- pwease? :_( (Y/N):  ";
        cin >> play;
        if (play == 'N' or play == 'n')
        {
            exit(0);
        }
    }
    cout << "Most wondrous! Thee continueth with mine own full faith in thee." << endl
         << "========================================================================" << endl;

    int weapons_amount, weapon_stats[32][3];
    string weapon_names[32];
    string WeaponFileName = "WeaponStats.txt";
    readToArrays(weapon_stats, weapon_names, weapons_amount, WeaponFileName);

    displayStats("H're are thy stats:", player_stats);

    cout << "Thee might not but eke chooseth a weapon; i has't a collection f'r thee:" << endl
         << endl
         << "Weapons (with bonus):" << endl;

    int choice;
    for (int i = 0; i < weapons_amount; ++i)
    {
        cout << "Name: " << weapon_names[i] << " (Press " << i << " to choose)" << endl
             << "Attack = " << weapon_stats[i][0] << endl
             << "Defense = " << weapon_stats[i][1] << endl
             << "Max HP = " << weapon_stats[i][2] << endl
             << endl;
    }

    validIn(choice, 1, 6, "Ent'r choice: ");
    for (int i = 0; i < 3; ++i)
    {
        player_stats[i] = player_stats[i] + weapon_stats[choice][i];
    }

    displayStats("Final Stats:", player_stats);

    pause();

    cout << endl
         << "========================================================================" << endl
         << endl;
}

void encounter(string player_name, int player_stats[3], string monster_names[], int monster_stats[][3], int monster_amount, int &level, int &experience)
{
    int monster_number = rng(0, monster_amount - 1);

    int player_attack = level * player_stats[0];
    int player_defense = level * player_stats[1];
    int player_hp = level * player_stats[2];
    bool player_alive = true;

    string monster_name = monster_names[monster_number];
    int monster_attack = level * monster_stats[monster_number][0] * (100 + rng(0, 10)) / 100;
    int monster_defense = level * monster_stats[monster_number][1] * (100 + rng(0, 10)) / 100;
    int monster_hp = level * monster_stats[monster_number][2] * (100 + rng(0, 10)) / 100;
    bool monster_alive = true;

    cout << "Look out, " << player_name << ", thee've encount'r'd a " << monster_name << "..." << endl
         << "It hast the following stats:" << endl;
    cout << "Attack = " << monster_attack << endl
         << "Defense = " << monster_defense << endl
         << "Max HP = " << monster_hp << endl
         << endl;

    cout << "At thy leveleth (" << level << "), thee has't the following stats: " << endl;
    cout << "Attack = " << player_attack << endl
         << "Defense = " << player_defense << endl
         << "Max HP = " << player_hp << endl
         << endl;
    do
    {
        int action;
        cout << "Player hp: " << player_hp << endl
             << monster_name << " hp: " << monster_hp << endl
             << endl
             << "Available Actions:" << endl
             << "Light Attack - less damage but more consistent (Press " << 0 << " to choose)" << endl
             << "Heavy Attack - more damage but less consistent (Press " << 1 << " to choose)" << endl
             << "Rest - attempt to regain lost hp (Press " << 2 << " to choose)" << endl
             << endl;

        validIn(action, 0, 2, "Enter Action: ");
        cout << endl;

        int player_damage, monster_damage;
        switch (action)
        {
        case 0:
        {
            player_damage = player_attack * player_attack / monster_defense * (100 + rng(-10, 10)) / 100;
            cout << "Light Attack Selected..." << endl;
            break;
        }
        case 1:
        {
            player_damage = 1.2 * player_attack * player_attack / monster_defense * (100 + rng(-25, 15)) / 100;
            cout << "Heavy Attack Selected..." << endl;
            break;
        }
        case 2:
        {
            player_damage = 0;
            int heal = player_defense * (100 + rng(-8, 16)) / 100;
            player_hp = player_hp + heal;
            cout << "Rest Selected..." << endl;
            cout << heal << " hp restored; new hp: " << player_hp << endl;
            break;
        }
        default:
            break;
        }

        monster_hp = monster_hp - player_damage;
        cout << "You did " << player_damage << " damage..." << endl
             << monster_name << " hp is now " << monster_hp << "..." << endl;

        monster_damage = monster_attack * monster_attack / player_defense * (100 + rng(-10, 10)) / 100;
        player_hp = player_hp - monster_damage;

        cout << monster_name << " did " << monster_damage << " damage..." << endl
             << "Your hp is now " << player_hp << "..." << endl
             << endl;

        player_alive = player_hp > 0;
        monster_alive = monster_hp > 0;

        cout << endl
             << "========================================================================" << endl
             << endl;

    } while (player_alive && monster_alive);

    if (not player_alive)
    {
        cout << "You died! You get 0 experience points" << endl;
    }
    else if (not monster_alive)
    {
        int exp_gain = (100 + rng(-10, 10));
        experience = experience + exp_gain;
        cout << "You won! You get " << exp_gain << " experience points for a total of " << experience << "/" << maxExp << "..." << endl;
    }

    cout << endl;
    pause;

    cout << endl
         << "========================================================================" << endl
         << endl;
}

void levelup(int &level, int &experience, int player_stats[3])
{
    if (experience > maxExp - 1)
    {
        ++level;
        experience = experience % maxExp;

        if (level % 3 == 0)
        {
            int choice;

            cout << "You get an upgrade point! Press..." << endl
                 << "0 to upgrade attack... " << endl
                 << "1 to upgrade defense... " << endl
                 << "2 to upgrade health... " << endl
                 << "Enter Choice: " << endl;

            cin >> choice;

            switch (choice)
            {
            case 0:
            {
                cout << "Attack upgraded!" << endl;
                player_stats[choice] = player_stats[choice] + 10;
                break;
            }

            case 1:
            {
                cout << "Attack upgraded!" << endl;
                player_stats[choice] = player_stats[choice] + 10;
                break;
            }

            case 2:
            {
                cout << "Attack upgraded!" << endl;
                player_stats[choice] = player_stats[choice] + 100;
                break;
            }
            default:
            {
                break;
            }
            }

            displayStats("Final Stats:", player_stats);
        }
    }

    cout << "Level: " << level << " | Experience: " << experience << "/" << maxExp << endl;
    cout << endl
         << "========================================================================" << endl
         << endl;
}

void displayStats(string title, int player_stats[3])
{
    cout << endl
         << title << endl
         << "Attack = " << player_stats[0] << endl
         << "Defense = " << player_stats[1] << endl
         << "Max HP = " << player_stats[2] << endl
         << endl;
}

void pause()
{
    string a;
    cout << "Enter any value to continue....";
    cin >> a;
}

void save(string player_name, int player_stats[3], int level, int experience)
{
    int saving;
    validIn(saving, 0, 1, "Do you wish to save (1 for Yes 0 for No)? ");
    if (saving)
    {
        string FileName;
        fstream saveFile;
        cout << "Enter Name for Save File: ";
        cin.ignore();
        getline(cin, FileName);
        FileName = FileName + ".txt";
        saveFile.open(FileName, ios::out);
        saveFile << player_name << endl
                 << player_stats[0] << endl
                 << player_stats[1] << endl
                 << player_stats[2] << endl
                 << level << endl
                 << experience;
        saveFile.close();
    }
}

void loadFile(string &name, int stat_array[3], int &level, int &experience)
{
    fstream File;
    string FileName;

    cout << "Enter Name of Save File: ";
    cin.ignore();
    getline(cin, FileName);

    FileName = FileName + ".txt";

    File.open(FileName, ios::in);

    if (File)
    {
        string line;
        getline(File, line);
        name = line;
        getline(File, line);
        stat_array[0] = stoi(line);
        getline(File, line);
        stat_array[1] = stoi(line);
        getline(File, line);
        stat_array[2] = stoi(line);
        getline(File, line);
        level = stoi(line);
        getline(File, line);
        experience = stoi(line);

        cout << endl
             << "Name = " << name << endl;
        displayStats("Player Stats:", stat_array);
        cout << "Level: " << level << endl
             << "Experience: " << experience << "/" << maxExp << endl
             << endl
             << "========================================================================" << endl
             << endl;
    }
    else
    {
        cout << "FILE NOT FOUND" << endl
             << "Starting Basic Setup..." << endl
             << "========================================================================" << endl
             << endl;

        setPlayer(name, stat_array);
    }

    File.close();
}
