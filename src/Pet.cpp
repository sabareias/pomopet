#include "Pet.h"

// add stats
void Pet::addStrength(int strength) {
	this->strength += strength;
}
void Pet::addSpeed(int speed) {
	this->speed += speed;
}
void Pet::addHealth(int health) {
	this->health += health;
}
void Pet::addLuck(int luck) {
	this->luck += luck;
}

// random num generator
int Pet::dice_roll() {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(1, 20);
	int dice_value = dist(rng);

	return dice_value;
}