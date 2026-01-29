#include "Shop.h"
#include "PlayerPet.h"


Shop::Shop() {
	consumables.push_back("Protein Bar (Strength)");
	consumables.push_back("Energy Drink (Speed)");
	consumables.push_back("Daily Vitamin (Health)");
	consumables.push_back("Four-Leaf Clover (Luck)");

	equipment.push_back("BIG Sword (Strength)");
	equipment.push_back("Speedy Sneaks (Speed)");
	equipment.push_back("Chef's Frying Pan (Health)");
	equipment.push_back("Rabbit's Foot (Luck)");
}

// displays consumables menu
void Shop::getConsumables() {
	output_in_yellow("Meowy");
	std::cout << ":  Great choice, meow friend! Here is what we have in stock:\n\n";
	int i = 1;
	for (std::string item : consumables) {
		std::cout << "\t" << i << ". " << item << "\n";
		i++;
	}
	output_in_yellow("Meowy");
	std::cout << ":  Each item costs 5 gold and increases your skill by 3 points!"
		<< "\n\nWhich do you want to buy? (Enter 1-4, or 0 to go back)\n";
}

// displays equpiment menu
void Shop::getEquipment() {
	output_in_yellow("Meowy");
	std::cout << ":  Great choice, meow friend! Here is what we have in stock:\n\n";
	int i = 1;
	for (std::string item : equipment) {
		std::cout << "\t" << i << ". " << item << "\n";
		i++;
	}
	output_in_yellow("Meowy");
	std::cout << ":  Each item costs 50 gold, increases your skill by 20 points,\n\tAND gives a + 1 bonus to ALL stats when training!\n\tThese are very rare... limited stock... only one of each!!!"
		<< "\n\nWhich do you want to buy? (Enter 1-4, or 0 to go back)\n";
}

// display msg when first arriving @ shop
void Shop::introMsg() {
	output_in_yellow("Meowy");
	std::cout << ":  Welcome to meow shop ~"
		<< "\n\tI sell magic items to boost your stats!\n";
}

// display shop menu options
void Shop::askShopChoice() {
	output_in_yellow("Meowy");
	std::cout << ":  Are you interested in browsing meow consumable items (1), equipment (2),\n\tor meowby none at all (0)?\n\n> ";
}

// goodbye msg when leaving shop
void Shop::leaveShop() {
		output_in_yellow("Meowy");
		std::cout << ":  No problem, meow friend, I'm sure I'll see you again soon ~\n";
}

// thank you msg when purchasing
void Shop::purchaseMsg() {
	output_in_yellow("Meowy");
	std::cout << ":  Heh, thank you for your purrchase ~\n";
}

// cannot afford item msg
void Shop::brokeMsg() {
	output_in_yellow("Meowy");
	std::cout << ":  Ohhh, so sorry meow friend but it looks like you don't have the coin...\n";
}

// verify if pet can afford item
bool Shop::canAfford(int gp, int price) {
	if (gp >= price) {
		return true;
	}
	else { return false; }
}

// outputs string in yellow
void Shop::output_in_yellow(std::string output) {
	std::cout << COLOR_YELLOW << "\n" << output << COLOR_RESET;
}