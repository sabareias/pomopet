#pragma once
#include "Pet.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class PlayerPet : public Pet {
public:
	PlayerPet() {};
	
	void checkInfo();

	void saveData();
	void loadData();

	void addItem(std::string item);

	void getGold();
	void spendGold(int gold);

	int fight(std::string statType, int stat, std::string enemyName);

	void statIncreaseMsg(std::string stat, int stat_increase);

	int age = 0;
	int gold = 0;
	int focusCt = 0;
	int battlesWon = 0;
	int racesWon = 0;
	int trainingBonus = 0;
	std::vector<std::string> inventory;
};