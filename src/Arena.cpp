#include "Arena.h"
#include "PlayerPet.h"
#include "Enemy.h"


Arena::Arena() {
	
}

// display msg when first arriving @ shop
void Arena::introMsg() {
	output_in_yellow("Announcer");
	std::cout << ": Hail fellow, welcome to the arena!"
		<< "\n\t   Here thou may compete in a battle of Strength or Speed.\n";
	output_in_yellow("Announcer");
	std::cout << ": Shouldst thou triumph, thou shalt be bestowed with gold and a worthy item!"
		<< "\n\t   Yet, shouldst thou falter, thou must surrender a modicum of gold as a tribute.\n";
}

// display shop menu options
void Arena::askArenaChoice() {
	output_in_yellow("Announcer");
	std:: cout << ": Dost thou wish to compete?"
		<<"\n\t   (Enter 1 for strength, 2 for speed, 3 to check stats, or 0 to go back)\n\n> ";
}

// goodbye msg when leaving shop
void Arena::leaveArena() {
	output_in_yellow("Announcer");
	std::cout << ": Fare thee well! We eagerly await thy next visit.\n";
}

// outputs fight start
void Arena::goFight(std::string& enemy) {
	output_in_yellow("Announcer");
	std::cout << ": So tis a battle of strength thou seek!"
		<< "\n\t   Thy adversary shall be "
		<< COLOR_RED << enemy << COLOR_RESET << ".\n\n"
		<< "[ Press enter to begin battle ]";
		std::cin.ignore();
		std::cin.get();

}

// outputs race start
void Arena::goRace(std::string& enemy) {
	output_in_yellow("Announcer");
	std::cout << ": So tis a race thou seek!"
		<< "\n\t   Your opponent will be "
		<< COLOR_RED << enemy << COLOR_RESET << ".\n\n"
		<< "[ Press enter to begin race ]";
		std::cin.ignore();
		std::cin.get();
}

// outputs message if player wins
void Arena::winnerMsg(std::string& enemy) {
	output_in_yellow("Announcer");
	std::cout << ": Congratulations! You have defeated "
		<< COLOR_RED << enemy << COLOR_RESET << "!!\n\n";
}

// outputs message if player loses
void Arena::loserMsg(std::string& enemy) {
	output_in_yellow("Announcer");
	std::cout << ": The winner is " << COLOR_RED << enemy << COLOR_RESET << "!"
		<< "\n\t   Don't fret, thou shall prevail next time!\n\n";
}

// outputs message if player ties (exact same tie breaker stat)
void Arena::tieMsg(std::string& enemy) {
	output_in_yellow("Announcer");
	std::cout << ": Wow, a true tie! How rare..."
		<< "\n\t   Neither thou nor "
		<< COLOR_RED << enemy << COLOR_RESET << " win this battle.\n\n";
}

// outputs message if player ties (uneven tie breaker stat)
void Arena::closeMsg(std::string& enemy, std::string& pet, std::string condition, std::string matchType) {
	output_in_yellow("Announcer");
	std::cout << ": Wow, what a close match!"
		<< "\n\t   In the end, ";
	if (condition == "win") {
		if (matchType == "battle") {
			std::cout << COLOR_YELLOW << pet << COLOR_RESET << " was faster and able to\n\t    land the final blow first!\n";
		}
		else if (matchType == "race") {
			std::cout << COLOR_YELLOW << pet << COLOR_RESET << " pulled the strength together\n\t    in the final sprint to win it all!\n";
		}
	} 
	if (condition == "lose") {
		if (matchType == "battle") {
			std::cout << COLOR_YELLOW << pet << COLOR_RESET << " was too slow, alowing " << COLOR_RED << enemy << COLOR_RESET << "\n\t   to land the final blow first.\n";
		}
		else if (matchType == "race") {
			std::cout << COLOR_YELLOW << pet << COLOR_RESET << " didn't have the strength to keep up\n\t   with " << COLOR_RED << enemy << COLOR_RESET << " in the final sprint.\n";
		}
	}
}

// outputs string in yellow
void Arena::output_in_yellow(std::string output) {
	std::cout << COLOR_YELLOW << "\n" << output << COLOR_RESET;
}