#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>

#include "Pet.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[1;31m"

class Enemy : public Pet {
public:
	Enemy(int petAge, int battlesWon, int racesWon, int trainingBonus);

	std::string getReward();
	std::string chooseName();

	int fight(std::string statType, int stat, std::string& enemyName);

	void output_in_red(std::string output);

	int difficulty = 1;
	int rewardVal = 3; // amount that reward increases stats
	std::vector<std::string> rewards{ "Protein Bar", "Energy Drink", "Daily Vitamin", "Four-Leaf Clover" };

};