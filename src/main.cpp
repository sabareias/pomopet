// This game combines pomodoro timers and pet raising sims. 
    // Every time you start the pomodoro timer and begin a focus session, your pet trains in a stat of your choosing.
    // After 4 focus sessions in one single gameplay session, you earn a longer break where your pet can shop for stat boosting items/equipment as well as battle NPCs

// include statements
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "PlayerPet.h"
#include "Enemy.h"
#include "Location.h"
#include "Forest.h"
#include "Field.h"
#include "Kitchen.h"
#include "WishingWell.h"
#include "Shop.h"
#include "Arena.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Enable ANSI escape sequences on Windows consoles so color macros and the CSI clear sequence work
#ifdef _WIN32
static void enableAnsiEscapeSequences() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

// using directives:
using namespace std::chrono;
using std::cout;
using std::cin;
using std::string;
using std::ofstream;
using std::ifstream;

// define colors
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_GREEN "\x1b[1;32m"
#define COLOR_BLUE "\x1b[1;34m"
#define COLOR_MAGENTA "\x1b[1;35m"
#define COLOR_CYAN "\x1b[1;36m"

// function declarations:
void game_intro(PlayerPet& pet);
void post_training_updates(PlayerPet& pet);
void new_game_start(PlayerPet& pet);

string get_location_choice(string& location);
void go_train(string& location, PlayerPet& pet);
void travel_forest(string& location, PlayerPet& pet);
void travel_field(string& location, PlayerPet& pet);
void travel_kitchen(string& location, PlayerPet& pet);
void travel_wishingWell(string& location, PlayerPet& pet);

void get_long_break_choice(PlayerPet& pet);

void go_shop(PlayerPet& pet);
void availableEquipment(PlayerPet& pet, Shop& shop, string& item);
void updateAvailableEquipment(PlayerPet& pet, Shop& shop, int& itemChoice);

void go_battle(PlayerPet& pet, bool& arena_done);
void attack(PlayerPet& pet, Enemy& enemy, Arena& arena);
void displayHealth(PlayerPet& pet, Enemy& enemy, int petHealth, int enemyHealth);
void race(PlayerPet& pet, Enemy& enemy, Arena& arena);
void displayDistance(PlayerPet& pet, Enemy& enemy, int petDistance, int enemyDistance);
void giveReward(Enemy& enemy, PlayerPet& pet);
void getReward(Enemy& enemy, PlayerPet& pet, string type);

bool is_valid_training_choice(int& training_choice);
bool can_afford_training(PlayerPet& pet, int trainingChoice);
bool four_sessions_done(PlayerPet& pet);
bool is_valid_item_choice(int& itemChoice);
bool is_number();
bool has_training_bonus(PlayerPet& pet);

void load_ascii(string fileName);
void load_ascii_red(string fileName);
void tolower(string& s);
void clear();
void div_line();
void output_in_yellow(string name);

void pomo_focus(PlayerPet& pet);

int main() 
{
    #ifdef _WIN32
        enableAnsiEscapeSequences();
    #endif
    string location;
    PlayerPet pet;

    game_intro(pet);

// game loop
    while(true) {
        load_ascii("bird.txt");
        pet.checkInfo();
        get_location_choice(location);
        go_train(location, pet);
        pomo_focus(pet);
        post_training_updates(pet);
    // save data + refresh screen for next focus loop
        pet.saveData();
        div_line();
        clear();
        cout << "\n[ Game Data Saved ]\n";
    }
    return 0;
}

/* ----------------------------------------------------------------
 ------------------------- GAME STUFF -----------------------------
 ----------------------------------------------------------------*/

// game intro
void game_intro(PlayerPet& pet) {

// ascii game title:
    load_ascii("title.txt");

// intro text:
    cout << "\nPomoPet combines virtual pet raising with the Pomodoro method to motivate you to stay focused!\n"
        << "While you focus on your work, your pet will train and get stronger.\n"
        << "Can you work together to defeat enemies, work, and chores ?!\n\n"
        << "[ Press enter to start ]";
    cin.clear();
    cin.ignore();

// check for new game or load save
    int loadType;
    cout << "\nWould you like to start a new game (1) or load your existing file? (2): \n> ";
    cin >> loadType;

    // validates user entered an int
    while (!is_number()) {
        cout << "\nInvalid input.\nWould you like to start a new game (1) or load your existing file? (2): \n> ";
        cin >> loadType;
    }
    // validates user entered 1 or 2
    while (loadType != 1 && loadType != 2) {
        cout << "\nInvalid input.\nWould you like to start a new game (1) or load your existing file? (2): \n> ";
        cin >> loadType;
    } 
    
    if (loadType == 1) { new_game_start(pet); } // new game
    else if (loadType == 2) { pet.loadData(); } // load game

    div_line();
    clear();

    return;
}

// new game start
void new_game_start(PlayerPet& pet) {
    clear();
    cout << "\nA mysterious egg appears in front of you... \n\n";

    load_ascii("egg.txt");
    cout << "\nGet to work to see what will hatch...\n";

    pomo_focus(pet);

    cout << "\n";

    load_ascii("bird.txt");
    cout << "\nA small bird-like creature hatched out of the egg!\n\n"
        << "What would you like to name your new pet?\n";
    cin >> pet.name;

    post_training_updates(pet);
}

// get gold, age up, check if 4 sessions done / pause until ready for break
void post_training_updates(PlayerPet& pet) {
    pet.getGold();
    pet.age++;
    pet.focusCt++;
    if (four_sessions_done(pet)) {
           cout << "\n[ Press enter to start your 10 minute break ]";
        cin.get();
        get_long_break_choice(pet);
    }
    else {
        cout << "\n[ Press enter to start your 5 minute break ]";
        cin.get();
    }
    
}

/* ----------------------------------------------------------------
 ------------------------- LOCATIONS ------------------------------
 ----------------------------------------------------------------*/

// gets choice of location from user
string get_location_choice(string& location) {
    while (true) {
        cout << "\nWhere would you like to go? ( forest, field, kitchen, well )\n\n> ";
        
        cin >> location;
        tolower(location);

        div_line();
        clear();

        if (location != "forest" && location != "field" && location != "kitchen" && location != "well") {
            cout << "\nSorry, that location is invalid! Please enter an option from the given list.\n";
            continue;
        }
        break;
    }
    return location;
}

// direct travel destination based off location entered
void go_train(string& location, PlayerPet& pet) {
    if (location == "forest") {
        travel_forest(location, pet);
    }
    else if (location == "field") {
        travel_field(location, pet);
    }
    else if (location == "kitchen") {
        travel_kitchen(location, pet);
    }
    else if (location == "well") {
        travel_wishingWell(location, pet);
    }
}

// display forest options
void travel_forest(string& location, PlayerPet& pet) {
    Forest forest;
    int trainingChoice = 9;

    load_ascii("forest.txt");

    forest.chooseOption();
    forest.getOptions();

    cout << "\n> ";
    cin >> trainingChoice;

// verify training choice is 0-3:
    while (!is_valid_training_choice(trainingChoice)) {
        cin >> trainingChoice;
    }
// verify pet's gold >= training cost
    while (!can_afford_training(pet, trainingChoice)) {
        cin.ignore();
        cin >> trainingChoice;
    }

    if (trainingChoice == 0) {
        div_line();
        clear();
        get_location_choice(location);
        go_train(location, pet);
    }

    // changes message depending on # of focus sessions completed this game session
    if (pet.focusCt == 0) {
        cout << "\n[ Press enter to start focusing ]";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "\n[ Press enter to end your break and start focusing ]";
        cin.ignore();
        cin.get();
    }

    div_line();
    clear();

    if (trainingChoice == 1) {
        div_line();
        clear();
        cout << "\n" << pet.name << forest.trainingDialogue[0];
        pet.addStrength(1);
    } 
    else if (trainingChoice == 2) {
        cout << "\n" << pet.name << forest.trainingDialogue[1];
        pet.spendGold(5);
        pet.addStrength(2);
    } 
    else if (trainingChoice == 3) {
        cout << "\n" << pet.name << forest.trainingDialogue[2];
        pet.spendGold(10);
        pet.addStrength(5);
    } 
    
    // check for training bonus + add to stat increase
    if (has_training_bonus(pet)) {
        pet.addStrength(pet.trainingBonus);
    }

}

// display field options
void travel_field(string& location, PlayerPet& pet) {
    Field field;
    int trainingChoice = 9;

    load_ascii("field.txt");

    field.chooseOption();
    field.getOptions();

    cout << "\n> ";
    cin >> trainingChoice;

// verify training choice is 0-3:
    while (!is_valid_training_choice(trainingChoice)) {
        cin >> trainingChoice;
    }
// verify pet's gold >= training cost
    while (!can_afford_training(pet, trainingChoice)) {
        cin.ignore();
        cin >> trainingChoice;
    }

    if (trainingChoice == 0) {
        div_line();
        clear();
        get_location_choice(location);
        go_train(location, pet);
    }

    // changes message depending on # of focus sessions completed this game session
    if (pet.focusCt == 0) {
        cout << "\n[ Press enter to start focusing ]";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "\n[ Press enter to end your break and start focusing ]";
        cin.ignore();
        cin.get();
    }

    div_line();
    clear();

    if (trainingChoice == 1) {
        cout << "\n" << pet.name << field.trainingDialogue[0];
        pet.addSpeed(1);
    }
    else if (trainingChoice == 2) {
        cout << "\n" << pet.name << field.trainingDialogue[1];
        pet.spendGold(5);
        pet.addSpeed(2);
    }
    else if (trainingChoice == 3) {
        cout << "\n" << pet.name << field.trainingDialogue[2];
        pet.spendGold(10);
        pet.addSpeed(5);
    }

    // check for training bonus + add to stat increase
    if (has_training_bonus(pet)) {
        pet.addSpeed(pet.trainingBonus);
    }
}

// display kitchen options
void travel_kitchen(string& location, PlayerPet& pet) {
    Kitchen kitchen;
    int trainingChoice = 9;

    load_ascii("kitchen.txt");

    kitchen.chooseOption();
    kitchen.getOptions();

    cout << "\n> ";
    cin >> trainingChoice;

// verify training choice is 0-3:
    while (!is_valid_training_choice(trainingChoice)) {
        cin >> trainingChoice;
    }
// verify pet's gold >= training cost
    while (!can_afford_training(pet, trainingChoice)) {
        cin.ignore();
        cin >> trainingChoice;
    }

    if (trainingChoice == 0) {
        div_line();
        clear();
        get_location_choice(location);
        go_train(location, pet);
    }

    // changes message depending on # of focus sessions completed this game session
    if (pet.focusCt == 0) {
        cout << "\n[ Press enter to start focusing ]";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "\n[ Press enter to end your break and start focusing ]";
        cin.ignore();
        cin.get();
    }

    div_line();
    clear();

    if (trainingChoice == 1) {
        cout << "\n" << pet.name << kitchen.trainingDialogue[0];
        pet.addHealth(1);
    }
    else if (trainingChoice == 2) {
        cout << "\n" << pet.name << kitchen.trainingDialogue[1];
        pet.spendGold(5);
        pet.addHealth(2);
    }
    else if (trainingChoice == 3) {
        cout << "\n" << pet.name << kitchen.trainingDialogue[2];
        pet.spendGold(10);
        pet.addHealth(5);
    }

    // check for training bonus + add to stat increase
    if (has_training_bonus(pet)) {
        pet.addHealth(pet.trainingBonus);
    }
}

// display wishing well options
void travel_wishingWell(string& location, PlayerPet& pet) {
    WishingWell wishingWell;
    int trainingChoice = 9;

    load_ascii("well.txt");

    wishingWell.chooseOption();
    wishingWell.getOptions();

    cout << "\n> ";
    cin >> trainingChoice;

// verify training choice is 0-3:
    while (!is_valid_training_choice(trainingChoice)) {
        cin >> trainingChoice;
    }
// verify pet's gold >= training cost
    while (!can_afford_training(pet, trainingChoice)) {
        cin.ignore();
        cin >> trainingChoice;
    }

    if (trainingChoice == 0) {
        div_line();
        clear();
        get_location_choice(location);
        go_train(location, pet);
    }

    // changes message depending on # of focus sessions completed this game session
    if (pet.focusCt == 0) {
        cout << "\n[ Press enter to start focusing ]";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "\n[ Press enter to end your break and start focusing ]";
        cin.ignore();
        cin.get();
    }

    div_line();
    clear();

    if (trainingChoice == 1) {
        cout << "\n" << pet.name << wishingWell.trainingDialogue[0];
        pet.addLuck(1);
    }
    else if (trainingChoice == 2) {
        cout << "\n" << pet.name << wishingWell.trainingDialogue[1];
        pet.spendGold(5);
        pet.addLuck(2);
    }
    else if (trainingChoice == 3) {
        cout << "\n" << pet.name << wishingWell.trainingDialogue[2];
        pet.spendGold(10);
        pet.addLuck(5);
    }
    // check for training bonus + add to stat increase
    if (has_training_bonus(pet)) {
        pet.addLuck(pet.trainingBonus);
    }
}

/* ----------------------------------------------------------------
 ---------------------------- SHOP --------------------------------
 ----------------------------------------------------------------*/

 // gets choice of long break location from user
void get_long_break_choice(PlayerPet& pet) {
    bool arena_done = false;
    string location;
    
    div_line();
    clear();

    while (true) {
        load_ascii("breaktime.txt");
        cout << "\nYou and " << pet.name << " have been working really hard, time for a longer break!\n"
            << "\nWhere would you like to go? ( shop, arena, training )\n\n> ";
        cin >> location;
        tolower(location);

        if (location != "shop" && location != "arena" && location != "training") {
            cout << "\nSorry, that location is invalid! Please enter an option from the given list.\n";
            continue;
        }
        if (location == "shop") {
            go_shop(pet);
            div_line();
        }
        if (location == "arena" && arena_done == false) {
            go_battle(pet, arena_done);
            div_line();
        }
        else if (location == "arena" && arena_done == true) {
            div_line();
            clear();
            cout << "\nSorry, the arena is now closed! You can come back on your next long break.\n";
            
            continue;
        }
        if (location == "training") {
            char confirm;
            cout << "\nAre you sure you want to go back to training? (Y/N)"
                << "\n[ You won't be able to revisit the shop/arena until you complete another 4 sessions in a row! ]\n\n> ";

            cin >> confirm;
            if (tolower(confirm) == 'y') {
                div_line();
                clear();
                break;
            }
            else {
                div_line();
                clear();
                continue;
            }
        }
    }
}

// displays shop options
void go_shop(PlayerPet& pet) {
    Shop shop;
    int shopChoice = 0;
    int itemChoice = 0;
    clear();
    load_ascii("store.txt");
    shop.introMsg();

// look for equipment owned by pet + remove any already owned from shop
    for (string item : pet.inventory) {
        availableEquipment(pet, shop, item);
    }

    while (true) {
    // display & request shop menu options:
        shop.askShopChoice();
        cin >> shopChoice;
    // if user enters anything but a number, set shopChoice to 9 to prevent errors
        if (!is_number()) {
            shopChoice = 9; 
        }
    // leave shop:
        if (shopChoice == 0) { 
            char confirm;
            cout << "\nAre you sure you want to leave the shop? (Y/N)\n\n"
                << "> ";
            cin >> confirm;
            if (tolower(confirm) == 'y') {
                clear();
                load_ascii("store.txt");
                shop.leaveShop();
                break;
            }
            // return to shop:
            else {
                clear();
                load_ascii("store.txt");
            }
        }
    // consumables menu:
        else if (shopChoice == 1) { 
            clear();
            load_ascii("store.txt");
            
            shop.getConsumables();  
            cout << "( Gold on hand: " << pet.gold << " )\n\n> ";
            cin >> itemChoice;

        // verify item choice is 0-4:
            while (!is_valid_item_choice(itemChoice)) {
                cin >> itemChoice;
            }
        // back out of menu option:
            if (itemChoice == 0) {
                clear();
                load_ascii("store.txt");
                continue;
            }
        // confirm purchase:
            char confirm;
        // cut out stat info from item name stored in vector:
            string itemName = (shop.consumables[itemChoice - 1]);
            itemName = itemName.erase((itemName.find("(")-1));
            cout << "\nPurchase the " << itemName << "? ( Y/N )\n\n"
                << "> ";
            cin >> confirm;
        // purchase confirmed:
            if (tolower(confirm) == 'y') {
            // break out if pet cannot afford item
                if (!shop.canAfford(pet.gold, shop.consumable_price)) {
                    clear();
                    load_ascii("store.txt");
                    shop.brokeMsg();
                    continue;
                }

            // purchase + use item
                clear();
                load_ascii("store.txt");
            // update pet's stats
                shop.purchaseMsg();
                pet.spendGold(shop.consumable_price);
                switch (itemChoice) {
                case 1:
                    pet.addStrength(shop.consumable_stat);
                    pet.statIncreaseMsg("strength", shop.consumable_stat);
                    break;
                case 2:
                    pet.addSpeed(shop.consumable_stat);
                    pet.statIncreaseMsg("speed", shop.consumable_stat);
                    break;
                case 3:
                    pet.addHealth(shop.consumable_stat);
                    pet.statIncreaseMsg("health", shop.consumable_stat);
                    break;
                case 4:
                    pet.addLuck(shop.consumable_stat);
                    pet.statIncreaseMsg("luck", shop.consumable_stat);
                    break;
                default:
                    break;
                }
            }
        // purchase denied:
            else {
                clear();
                load_ascii("store.txt");
            }
            continue;
        }
    // equipment menu:
        else if (shopChoice == 2) {
            clear();
            load_ascii("store.txt");

            shop.getEquipment();
            cout << "( Gold on hand: " << pet.gold << " )\n\n> ";
            cin >> itemChoice;

        // verify item choice is 0-4:
            while (!is_valid_item_choice(itemChoice)) {
                cin >> itemChoice;
            }
        // back out of menu option:
            if (itemChoice == 0) {
                clear();
                load_ascii("store.txt");
                continue;
            }
        // confirm purchase:
            char confirm;
        // cut out stat info from item name stored in vector:
            string itemName = (shop.equipment[itemChoice - 1]);
            itemName = itemName.erase((itemName.find("(") - 1));
            cout << "\nPurchase the " << itemName << "? ( Y/N )\n\n"
                << "> ";
            cin >> confirm;
        // purchase confirmed:
            if (tolower(confirm) == 'y') {
            // break out if pet cannot afford item
                if (!shop.canAfford(pet.gold, shop.equipment_price)) {
                    clear();
                    load_ascii("store.txt");
                    shop.brokeMsg();
                    continue;
                }

            // purchase item
                clear();
                load_ascii("store.txt");
                shop.purchaseMsg();

            // update pet's stats
                pet.spendGold(shop.equipment_price);
                pet.addItem(shop.equipment[itemChoice - 1]);
                switch (itemChoice) {
                case 1:
                    pet.addStrength(shop.equipment_stat);
                    pet.statIncreaseMsg("strength", shop.equipment_stat);
                    break;
                case 2:
                    pet.addSpeed(shop.equipment_stat);
                    pet.statIncreaseMsg("speed", shop.equipment_stat);
                    break;
                case 3:
                    pet.addHealth(shop.equipment_stat);
                    pet.statIncreaseMsg("health", shop.equipment_stat);
                    break;
                case 4:
                    pet.addLuck(shop.equipment_stat);
                    pet.statIncreaseMsg("luck", shop.equipment_stat);
                    break;
                default:
                    break;
                }
                pet.trainingBonus++;

            // look for equipment owned by pet + remove any owned from shop
                updateAvailableEquipment(pet, shop, itemChoice);
            }
        // purchase denied:
            else {
                clear();
                load_ascii("store.txt");
            }
        }
    // invalid shop menu option input:
        else {
            clear();
            load_ascii("store.txt");
            output_in_yellow("Meowy");
            cout << ":  Hmm... meow can't understand you.";
        }
    }
}

// checks pet inventory + removes owned items from shop menu
void availableEquipment(PlayerPet& pet, Shop& shop, string& item) {
    auto it = find(pet.inventory.begin(), pet.inventory.end(), item);
    if (it != pet.inventory.end()) {
        shop.equipment.erase(std::remove(shop.equipment.begin(), shop.equipment.end(), item), shop.equipment.end());
    }
}

// updates / removes recently purchased item from shop menu
void updateAvailableEquipment(PlayerPet& pet, Shop& shop, int& itemChoice) {
    auto it = find(pet.inventory.begin(), pet.inventory.end(), (shop.equipment[itemChoice - 1]));
    if (it != pet.inventory.end()) {
        shop.equipment.erase(std::remove(shop.equipment.begin(), shop.equipment.end(), (shop.equipment[itemChoice - 1])), shop.equipment.end());
    }
}

/* ----------------------------------------------------------------
 -------------------------- BATTLE --------------------------------
 ----------------------------------------------------------------*/

 // displays battle options
void go_battle(PlayerPet& pet, bool& arena_done) {
    Arena arena;
    Enemy enemy(pet.age, pet.battlesWon, pet.racesWon, pet.trainingBonus);

    div_line();
    clear();
    load_ascii("arena.txt");
    arena.introMsg();

    while (true) {
    // default arena choice set to 0 (leave by default)
        int arenaChoice = 0;
    // only ask for arena choice if battle isn't yet done
        if (!arena_done) {
            // display & request arena menu options:
            arena.askArenaChoice();
            cin >> arenaChoice;
        }
    
    // if user enters anything but a number, set arenaChoice to 9 to prevent errors
        if (!is_number()) {
            arenaChoice = 9;
        }
    // leave arena: 
        if (arenaChoice == 0) {
        // default confirm set to 'y' (yes by default)
            char confirm = 'y';
        // only confirm if battle isn't yet done
            if (!arena_done) {
                cout << "\nAre you sure you want to leave the arena? (Y/N)\n\n"
                    << "> ";
                cin >> confirm;
            }
            if (tolower(confirm) == 'y') {
                clear();
                load_ascii("arena.txt");
                arena.leaveArena();
                break;
            }
            // return to arena:
            else {
                clear();
                load_ascii("arena.txt");
            }
        }
    // strength battle:
        else if (arenaChoice == 1) {
            div_line();
            clear();

            load_ascii_red("enemy.txt");
            arena.goFight(enemy.name);

            attack(pet, enemy, arena);
            arena_done = true;

            continue;
        }
    // speed battle:
        else if (arenaChoice == 2) {
            div_line();
            clear();

            load_ascii_red("enemy.txt");
            arena.goRace(enemy.name);

            race(pet, enemy, arena);
            arena_done = true;

            continue;
        }
    // show stats:
        else if (arenaChoice == 3) {
            div_line();
            clear();

            pet.checkInfo();

            continue;
        }
    // invalid arena menu option input:
        else {
            clear();
            load_ascii("arena.txt");
            output_in_yellow("Announcer");
            cout << ":  Thou must speak up! I did not understand that.";
        }
    }
}

// strength battle
void attack(PlayerPet& pet, Enemy& enemy, Arena& arena) {
    int petHealth = 100 + pet.health;
    int enemyHealth = 100 + enemy.health;
    int petDmg;
    int enemyDmg;

    while (true) {
        div_line();
        clear();

        petDmg = pet.fight("strength", pet.strength, enemy.name);
        enemyDmg = enemy.fight("strength", enemy.strength, pet.name);

        cout << COLOR_YELLOW << "\t   ___" << COLOR_RED << "        |_|    ,\n";
        cout << COLOR_YELLOW << "\t (' v ')" << COLOR_RED << "     ('.') ///\n";
        cout << COLOR_YELLOW << "\t((_____))" << COLOR_RED << "    <(_)`-/'\n";
        cout << COLOR_YELLOW << "\t   ^ ^" << COLOR_RED << "   <-._/J L /\n" << COLOR_RESET;

        petHealth -= enemyDmg;
        enemyHealth -= petDmg;

    // tie (tie-breaker based on speed)
        if (petHealth <= 0 && enemyHealth <= 0) {
            if (pet.speed > enemy.speed) {
                arena.closeMsg(enemy.name, pet.name, "win", "battle");
                arena.winnerMsg(enemy.name);
                enemy.getReward();
            }
            else if (pet.speed < enemy.speed) {
                arena.closeMsg(enemy.name, pet.name, "lose", "battle");
                arena.loserMsg(enemy.name);
            }
            else {
                arena.tieMsg(enemy.name);
            }
            cout << "\n[ Press enter to leave ]";
            std::cin.get();
            break;
        }
    // enemy win
        if (petHealth <= 0) {
            arena.loserMsg(enemy.name);
            giveReward(enemy, pet);
            
            cout << "\n[ Press enter to leave ]";
            std::cin.get();
            break;
        }
    // player win
        if (enemyHealth <= 0) {
            arena.winnerMsg(enemy.name);
            getReward(enemy, pet, "battle");
            pet.battlesWon++;

            cout << "\n[ Press enter to leave the arena ]";
            std::cin.get();
            break;
        }

        displayHealth(pet, enemy, petHealth, enemyHealth);

        cout << "\n[ Press enter to continue ]";
        std::cin.get();
    }

}

// speed battle
void race(PlayerPet& pet, Enemy& enemy, Arena& arena) {
    // 200m sprint
    int raceDistance = 200;

    // increase race distance every win 
    if (pet.racesWon > 0) {
        raceDistance = 200 * pet.racesWon;
    }

    int petDistanceLeft = raceDistance;
    int enemyDistanceLeft = raceDistance;
    int petRoundDistance = 0;
    int enemyRoundDistance = 0;
    
    while (true) {
        div_line();
        clear();

        int petRoundDistance = pet.fight("speed", pet.speed, enemy.name);
        int enemyRoundDistance = enemy.fight("speed", enemy.speed, pet.name);

        cout << COLOR_YELLOW << "\t   ___" << COLOR_RED << "        |_|    ,\n";
        cout << COLOR_YELLOW << "\t (' v ')" << COLOR_RED << "     ('.') ///\n";
        cout << COLOR_YELLOW << "\t((_____))" << COLOR_RED << "    <(_)`-/'\n";
        cout << COLOR_YELLOW << "\t   ^ ^" << COLOR_RED << "   <-._/J L /\n" << COLOR_RESET;

        petDistanceLeft -= petRoundDistance;
        enemyDistanceLeft -= enemyRoundDistance;

        // tie (tie-breaker based on strength)
        if (petDistanceLeft <= 0 && enemyDistanceLeft <= 0) {
            if (pet.strength > enemy.strength) {
                arena.closeMsg(enemy.name, pet.name, "win", "race");
                arena.winnerMsg(enemy.name);
                enemy.getReward();
            }
            else if (pet.strength < enemy.strength) {
                arena.closeMsg(enemy.name, pet.name, "lose", "race");
                arena.loserMsg(enemy.name);
            }
            else {
                arena.tieMsg(enemy.name);
            }
            cout << "\n[ Press enter to leave ]";
            std::cin.get();
            break;
        }
        // enemy win
        if (enemyDistanceLeft <= 0) {
            arena.loserMsg(enemy.name);
            
            giveReward(enemy, pet);

            cout << "\n[ Press enter to leave ]";
            std::cin.get();
            break;
        }
        // player win
        if (petDistanceLeft <= 0) {
            arena.winnerMsg(enemy.name);
            getReward(enemy, pet, "race");
            pet.racesWon++;

            cout << "\n[ Press enter to leave the arena ]";
            std::cin.get();
            break;
        }

        displayDistance(pet, enemy, petDistanceLeft, enemyDistanceLeft);

        cout << "\n[ Press enter to continue ]";
        std::cin.get();
    }

}

// update & display health
void displayHealth(PlayerPet& pet, Enemy& enemy, int petHealth, int enemyHealth) {
    cout << COLOR_YELLOW << "    " << pet.name << COLOR_RESET << "'s HP: " << petHealth << "    " << COLOR_RED << enemy.name << COLOR_RESET << "'s HP: " << enemyHealth << "\n";
}

// update & display remaining distance
void displayDistance(PlayerPet& pet, Enemy& enemy, int petDistance, int enemyDistance) {
    cout << COLOR_YELLOW << "    " << pet.name << COLOR_RESET << ": " << petDistance << " meters left    " << COLOR_RED << enemy.name << COLOR_RESET << ": " << enemyDistance << " meters left\n";
}

// lose = give reward to enemy
void giveReward(Enemy& enemy, PlayerPet& pet) {
    // lose money:
    if (pet.gold >= 5) {
        cout << "You give 5 gold to " << enemy.name << " as a reward.\n";
        pet.spendGold(5);
    }
    else if (pet.gold > 0) {
        cout << "You don't have enough gold to give " << enemy.name << " a full reward...\nYou hand over the last " << pet.gold << " gold coins you have.\n";
        pet.spendGold(pet.gold);
    }
    else {
        cout << "You don't have any gold to give " << enemy.name << " a reward... how embarrassing...\n";
    }
}

// win = get reward from enemy
void getReward(Enemy& enemy, PlayerPet& pet, string type) {
    string reward = enemy.getReward();
    load_ascii("trophy.txt");
    cout << "For winning the " << type << ", " << COLOR_RED << enemy.name << COLOR_RESET << " has given you a " << reward << " and 5 gold as a reward!\n";

    pet.gold += 5;

    // increase stats based on random reward earned
    if (reward == "Protein Bar") {
        pet.addStrength(enemy.rewardVal);
        pet.statIncreaseMsg("strength", enemy.rewardVal);
    }
    else if (reward == "Energy Drink") {
        pet.addSpeed(enemy.rewardVal);
        pet.statIncreaseMsg("speed", enemy.rewardVal);
    }
    else if (reward == "Daily Vitamin") {
        pet.addHealth(enemy.rewardVal);
        pet.statIncreaseMsg("health", enemy.rewardVal);
    }
    else if (reward == "Four-Leaf Clover") {
        pet.addLuck(enemy.rewardVal);
        pet.statIncreaseMsg("luck", enemy.rewardVal);
    }
}

/* ----------------------------------------------------------------
 ------------------------ VALIDATION ------------------------------ 
 ----------------------------------------------------------------*/

// checks if training_choice = 0-3
bool is_valid_training_choice(int& trainingChoice) {
    if (trainingChoice == 0) {
        return true;
    }
    else if (trainingChoice != 1 && trainingChoice != 2 && trainingChoice != 3) {
        
        cout << "\nOops, that isn't an option! Please only enter 1 - 3 or 0 to go back.\n"
            << "> ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// loops while user selects a training option they can't afford
bool can_afford_training(PlayerPet& pet, int trainingChoice) {
    int price = 0;
    
    switch (trainingChoice) {
    case 2:
        price = 5;
        break;
    case 3:
        price = 10;
        break;
    default:
        break;
    }
    
    if (pet.gold < price) {
        cout << "\nYou don't have the gold for that training option... ( Gold on hand: " << pet.gold << " )\n"
            << "> ";
        return false;
    }

    return true;
}

// sends user to the shop & allows battle every 4 sessions (aka pet age)
bool four_sessions_done(PlayerPet& pet) {
    if (pet.focusCt % 4 == 0) {
        return true;
    }
    return false;
}

// checks if item_choice = 0 - 4
bool is_valid_item_choice(int& itemChoice) {
    if (itemChoice == 0) {
        return true;
    }
    else if (itemChoice != 1 && itemChoice != 2 && itemChoice != 3 && itemChoice != 4) {

        cout << "\nOops, that isn't an option! Please only enter 1 - 4 or 0 to go back.\n"
            << "> ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// checks for failed input when expecting an int
bool is_number() {
    if (cin.good()) {
        return true;
    }
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

bool has_training_bonus(PlayerPet& pet) {
    if (pet.trainingBonus > 0) {
        return true;
    }
    return false;
}

/* ----------------------------------------------------------------
 ------------------ DISPLAY / HELPER FUNCTIONS --------------------
 ----------------------------------------------------------------*/

// reads a file to output ASCII art in the color yellow
void load_ascii(string fileName) {
    ifstream location_file("ascii/"+fileName);
    string line;
    while (getline(location_file, line)) {
        cout << COLOR_YELLOW << "\t" << line << COLOR_RESET << "\n";
    }
    location_file.close();
}

// reads a file to output ASCII art in the color red
void load_ascii_red(string fileName) {
    ifstream location_file("ascii/" + fileName);
    string line;
    while (getline(location_file, line)) {
        cout << COLOR_RED << "\t" << line << COLOR_RESET << "\n";
    }
    location_file.close();
}

// converts string to lowercase
void tolower(string& s) {
    for (char& c : s) c = tolower(c);
}

// clears the console
void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

// outputs a divider line
void div_line() {
    cout << "\n------------------------------------------------------------\n";
}

void output_in_yellow(string output) {
    cout << COLOR_YELLOW << "\n" << output << COLOR_RESET;
}

/* ----------------------------------------------------------------
 ------------------------ POMODORO TIMERS -------------------------
 ----------------------------------------------------------------*/

 // run pomodoro timer
void pomo_focus(PlayerPet& pet) {
    const auto timer_duration = 0.05min;
    const auto start = steady_clock::now();
    duration<double> time_left = timer_duration - (steady_clock::now() - start);
    load_ascii("focustime.txt");
    cout << "\n";

    while (time_left > 0s)
    {
        const auto hrs = duration_cast<hours>(time_left);
        const auto mins = duration_cast<minutes>(time_left - hrs);
        const auto secs = duration_cast<seconds>(time_left - hrs - mins);
        cout << "Time Left: " << COLOR_YELLOW
            << std::setfill('0') << std::setw(2) << hrs.count() << ":"
            << std::setfill('0') << std::setw(2) << mins.count() << ":"
            << std::setfill('0') << std::setw(2) << secs.count() << "\r"
            << COLOR_RESET;

        std::this_thread::sleep_for(1s);

        time_left = timer_duration - (steady_clock::now() - start);
    }

    cout << "\n";
}