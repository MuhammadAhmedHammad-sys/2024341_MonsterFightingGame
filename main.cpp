#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void validIn(int &val, int min, int max, string prompt);
void readToArrays(int stat_array[][3], string name_array[], int &amount, string FileName);
void setPlayer(string &name, int stat_array[3]);

int main()
{
    int weapon_amount, mon_stats[32][3]; // attack, defense, max health
    string mon_names[32];
    string MonsterFileName = "Monsters.txt";
    readToArrays(mon_stats, mon_names, weapon_amount, MonsterFileName);

    string player_name;
    int player_stats[3] = {10, 10, 100}; // attack, defense, max health
    setPlayer(player_name, player_stats);

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

void readToArrays(int stat_array[][3], string name_array[], int &amount, string FileName)
{
    fstream MonsterFile;
    MonsterFile.open(FileName, ios::in);

    amount = 0;
    string line;
    MonsterFile.seekp(31);
    while (not MonsterFile.eof())
    {
        getline(MonsterFile, line);
        name_array[amount] = line;
        getline(MonsterFile, line);
        stat_array[amount][0] = stoi(line);
        getline(MonsterFile, line);
        stat_array[amount][1] = stoi(line);
        getline(MonsterFile, line);
        stat_array[amount][2] = stoi(line);
        ++amount;
    }

    MonsterFile.close();
}

void setPlayer(string &name, int stat_array[3])
{
    cout << "Greetings brave warri'r! what is thy nameth? ";
    cin >> name;
    cout << "----------------------------------------------------------------" << endl;

    int weapons_amount, weapon_stats[32][3];
    string weapon_names[32];
    string WeaponFileName = "WeaponStats.txt";
    readToArrays(weapon_stats, weapon_names, weapons_amount, WeaponFileName);

    cout << "Well hath met, " << name << "! h're art thy base stats:" << endl
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
