#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

void validIn(int &val, int min, int max, string prompt);
int rng(int min, int max);

void readToArrays(int stat_array[][3], string name_array[], int &amount, string FileName);
void setPlayer(string &name, int stat_array[3]);
void encounter(string player_name, int player_stats[3], string monster_names[], int monster_stats[][3], int monster_amount, int &level, int &experience);
void levelup(int &level, int &experience);
void loadFile(string &name, int stat_array[3], int &level, int &experience);

void save(string player_name, int player_stats[3], int level, int experience);

int main()
{
    int monster_amount, monster_stats[32][3]; // {attack, defense, max health}
    string monster_names[32];
    string MonsterFileName = "Monsters.txt";
    readToArrays(monster_stats, monster_names, monster_amount, MonsterFileName);

    string player_name;
    int player_stats[3] = {10, 10, 100}; // {attack, defense, max health}
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
        levelup(level, experience);
        validIn(playing, 0, 1, "Keep Playing? (1 for Yes, 0 for No): ");
        cout << endl
             << "----------------------------------------------------------------" << endl
             << endl;
    } while (playing);

    save(player_name, player_stats, level, experience);

    cout << "Goodbye!";

    return 0;
}

void validIn(int &val, int min, int max, string prompt)
{
    bool valid = true;
    do
    {
        if (not valid)
            cout << "INVALID INPUT... RE-ENTER..." << endl;
        cout << prompt;
        cin >> val;
        valid = val < min || val > max;
    } while (valid);
}

int rng(int min, int max)
{
    std::srand(std::time(nullptr));

    return int(min + std::rand() % max);
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

void setPlayer(string &name, int stat_array[3])
{
    cout << "Greetings brave warri'r! what is thy nameth? ";
    cin >> name;
    cout << "Well hath met " << name << "! This is our land of V'ria, once peaceful and thriving, but now overrun with foul monsters. Though it seems a daunting task, would you be willing to try to rid the land of these beasts? (Y/N)?";
    char play;
    cin >> play;
    if (play == 'N')
    {
        exit(0);
    }
    cout << "Most wondrous! Thee continueth with mine own full faith in thee." << endl
         << "----------------------------------------------------------------" << endl;

    int weapons_amount, weapon_stats[32][3];
    string weapon_names[32];
    string WeaponFileName = "WeaponStats.txt";
    readToArrays(weapon_stats, weapon_names, weapons_amount, WeaponFileName);

    cout << "H're art thy base stats:" << endl
         << "Attack = " << stat_array[0] << endl
         << "Defense = " << stat_array[1] << endl
         << "Max HP = " << stat_array[2] << endl
         << endl
         << "Thee might not but eke chooseth a weapon; i has't a collection f'r thee:" << endl
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
        stat_array[i] = stat_array[i] + weapon_stats[choice][i];
    }

    cout << endl
         << "Final Stats:" << endl
         << "Attack = " << stat_array[0] << endl
         << "Defense = " << stat_array[1] << endl
         << "Max HP = " << stat_array[2] << endl;

    cout << "----------------------------------------------------------------" << endl;
}

void encounter(string player_name, int player_stats[3], string monster_names[], int monster_stats[][3], int monster_amount, int &level, int &experience)
{
    int monster_number = rng(0, monster_amount - 1);

    int player_attack = level * player_stats[0];
    int player_defense = level * player_stats[1];
    int player_hp = level * player_stats[2];
    bool player_alive = true;

    string monster_name = monster_names[monster_number];
    int monster_attack = monster_stats[monster_number][0];
    int monster_defense = monster_stats[monster_number][1];
    int monster_hp = monster_stats[monster_number][2];
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
        cout << "Available Actions:" << endl
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
            player_damage = 1.11 * player_attack * player_attack / monster_defense * (100 + rng(-25, 15)) / 100;
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
             << "Moster hp is now " << monster_hp << "..." << endl;

        monster_damage = monster_attack * monster_attack / player_defense * (100 + rng(-10, 10)) / 100;
        player_hp = player_hp - monster_damage;

        cout << monster_name << " did " << monster_damage << " damage..." << endl
             << "Your hp is now " << player_hp << "..." << endl
             << endl;

        player_alive = player_hp > 0;
        monster_alive = monster_hp > 0;

    } while (player_alive && monster_alive);

    int exp_gain = (100 + rng(-10, 10));
    experience = experience + exp_gain;
    if (not player_alive)
    {
        cout << "You died! You get 0 experience points" << endl;
    }
    else if (not monster_alive)
    {
        cout << "You won! You get<<" << exp_gain << "experience points for a total of " << experience << "/500..." << endl;
    }

    cout << endl
         << "----------------------------------------------------------------" << endl
         << endl;
}

void levelup(int &level, int &experience)
{
    if (experience > 499)
    {
        level = experience / 500;
        experience = experience % 500;
    }

    cout << "Level: " << level << " | Experience: " << experience << "/500" << endl;
    cout << endl
         << "----------------------------------------------------------------" << endl
         << endl;
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
        cin >> FileName;
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
    cin >> FileName;

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
             << "Stats:" << endl
             << "Name = " << name << endl
             << "Defense = " << stat_array[0] << endl
             << "Attack = " << stat_array[1] << endl
             << "Defense = " << stat_array[2] << endl
             << "Level = " << level << endl
             << "Experience = " << experience <<"/500"<< endl
             << "----------------------------------------------------------------" << endl
             << endl;
    }
    else
    {
        cout << "FILE NOT FOUND" << endl
             << "Starting Basic Setup..." << endl
             << "----------------------------------------------------------------" << endl
             << endl;

        setPlayer(name, stat_array);
    }

    File.close();
}


