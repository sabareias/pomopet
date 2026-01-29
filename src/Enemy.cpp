#include "Enemy.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;

Enemy::Enemy(int petAge, int battlesWon, int racesWon, int trainingBonus) {
	// random num generator
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(2, 4);

	// random num per skill
	int rand1 = dist(rng);
	int rand2 = dist(rng);
	int rand3 = dist(rng);
	int rand4 = dist(rng);

	difficulty = petAge;

	// each skill = petAge / random number from 2 - 4
	strength = difficulty / rand1;
	speed = difficulty / rand2;
	health = difficulty / rand3;
	luck = difficulty / rand4;

	if (battlesWon > 0) {
		strength += (3 * battlesWon);
		health += (2 * battlesWon);
	}

	if (racesWon > 0) {
		speed += (3 * racesWon);
		luck += (2 * racesWon);
	}

	name = chooseName();
}

// randomizes enemy name
string Enemy::chooseName() {
	// vector of name options
	vector<string> names = { "Twitch", "Ace", "Punchy", "Champ", "Stinky" };
	// random num generator
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, (names.size()-1));
	int rand = dist(rng);

	return names[rand];
}

// rolls/returns enemy dmg value & outputs result
int Enemy::fight(string statType, int stat, string& petName) {

	// roll a dice (d20)
	int dice_val = dice_roll();

	cout << COLOR_RED << Enemy::name << COLOR_RESET;

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
		cout << " struck " << petName << "!\n";
		dice_val += (strength);
	}
	else if (statType == "speed") {
		cout << " ran as hard as they could!\n";
		dice_val += (speed);
	}

	return dice_val;
}

// selects a random item from the rewards vector
std::string Enemy::getReward() {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, (rewards.size() - 1));
	
		int rand = dist(rng);
		return rewards[rand];
}

// outputs string in red
void Enemy::output_in_red(string output) {
	cout << COLOR_RED << "\n" << output << COLOR_RESET;
}
