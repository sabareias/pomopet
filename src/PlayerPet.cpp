#include "PlayerPet.h"

using std::cout;
using std::cin;
using std::string;
using std::ofstream;
using std::ifstream;
using std::endl;

#define COLOR_RESET "\x1b[0m"
#define COLOR_YELLOW "\x1b[1;33m"

// output stats
void PlayerPet::checkInfo() {
	cout << "\n------------------------------\n"
		<< " " << COLOR_YELLOW << name << "'s Stats:\n" << COLOR_RESET
		<< "------------------------------\n"
		<< std::setfill(' ')
		<< "   Age: " << age << "\n"
		<< "   Gold: " << gold << "\n"
		<< "   Battles Won: " << battlesWon << "\n"
		<< "   Races Won: " << racesWon << "\n"
		<< "\n"
		<< "   Strength: " << strength << "\n"
		<< "   Speed: " << speed << "\n"
		<< "   Health: " << health << "\n"
		<< "   Luck: " << luck << "\n"
		<< "\n"
		<< "   Inventory\n";
		
	if (inventory.empty()) {
		cout << "      [ no items ]";
	}
	else {
		for (string item : inventory) {
			cout << "      - " << item << "\n";
		}
	}
	cout << "\n------------------------------\n";
}

// add item to inventory
void PlayerPet::addItem(std::string item) {
	inventory.push_back(item);
}

// save pet data
void PlayerPet::saveData() {
// save pet stats
	ofstream output_pet_file("data/pet.txt", std::ios::out);
		if (output_pet_file.is_open() == false) {
			cout << "Couldn't open file.\n";
			return;
		}
		output_pet_file << name << endl << strength << endl << speed << endl << health << endl << luck << endl << gold << endl << age << endl << battlesWon << endl << racesWon << endl << trainingBonus;
	output_pet_file.close();

// save inventory
	ofstream output_inventory_file("data/petInventory.txt", std::ios::out);
		if (output_inventory_file.is_open() == false) {
			cout << "Couldn't open file.\n";
			return;
		}
		for (string item : inventory) {
			output_inventory_file << item << endl;
		}
	output_inventory_file.close();
}

// load saved pet data
void PlayerPet::loadData() {
// load pet stats
	ifstream pet_file("data/pet.txt", std::ios::in);
		if (pet_file.is_open() == false) {
			cout << "Couldn't open file.\n";
			return;
		}
		pet_file >> name >> strength >> speed >> health >> luck >> gold >> age >> battlesWon >> racesWon >> trainingBonus;
	pet_file.close();

// load inventory
	ifstream pet_inventory_file("data/petInventory.txt", std::ios::in);
		if (pet_inventory_file.is_open() == false) {
			cout << "Couldn't open file.\n";
			return;
		}
		for (string item : inventory) {
			inventory.push_back(item);
		}
	pet_inventory_file.close();
}

// roll a dice (+luck/2) to determine amt of gold found while training
void PlayerPet::getGold() {
	int dice_val = dice_roll();

	dice_val += (luck/2);

	if (dice_val == 20) {
		cout << "\nWOAH!!! " << COLOR_YELLOW << PlayerPet::name << COLOR_RESET << " found 5 gold while you both were working!!\n";
		gold += 5;
	}
	else if (dice_val > 15) {
		cout << "\nLooks like " << COLOR_YELLOW << PlayerPet::name << COLOR_RESET << " found 2 gold while you both were working!\n";
		gold += 2;
	}
	else if (dice_val >= 5) {
		cout << "\n" << COLOR_YELLOW << PlayerPet::name << COLOR_RESET << " found 1 gold while you both were working!\n";
		gold += 1;
	}
}

// subtract amount spent from gold total
void PlayerPet::spendGold(int gold) {
	this->gold -= gold;
}

void PlayerPet::statIncreaseMsg(std::string stat, int stat_increase) {
	cout << COLOR_YELLOW << "\n*" << PlayerPet::name << "'s " << stat << " increased by " << stat_increase << " points!*\n" << COLOR_RESET;
}


int PlayerPet::fight(std::string statType, int stat, std::string enemyName) {

	// roll a dice (d20)
	int dice_val = dice_roll();

	cout << "\n" << COLOR_YELLOW << PlayerPet::name << COLOR_RESET;

// natural 1 = failure (return dice_val = 0 right away)
	if (dice_val == 1) {
		if (statType == "strength") {
			cout << " got distracted by a butterfly... no damage done this round.\n";
			dice_val = 0;
			return dice_val;
		}
		else if (statType == "speed") {
			cout << " tripped.... no progress made this round.\n";
			dice_val = 0;
			return dice_val;
		}
	}

// natural 20 = critical (+5 to dice_val)
	if (dice_val == 20) {
		if (statType == "strength") {
			cout << " got a critical hit!!\n";
			dice_val += 5;
		}
		else if (statType == "speed") {
			cout << " got a HUGE burst of speed!!\n";
			dice_val += 5;
		}
	}
// add str/sp modifier
	else if (statType == "strength") {
		cout << " struck " << enemyName << "!\n";
		dice_val += (strength);
	}
	else if (statType == "speed") {
		cout << " ran as hard as they could!\n";
		dice_val += (speed);
	}

	return dice_val;
}

