#pragma once
#include <string>
#include <random>

class Pet {
public:
	Pet(int strength = 0, int speed = 0, int health = 0, int luck = 0) {}

	void addStrength(int strength);
	void addSpeed(int speed);
	void addHealth(int health);
	void addLuck(int luck);

	int dice_roll();

	std::string name;
	int strength = 0;
	int speed = 0;
	int health = 0;
	int luck = 0;
};