#include "Kitchen.h"
#include <iostream>

Kitchen::Kitchen() {
	setTrainingOptions();
	setTrainingDialogue();
}

void Kitchen::setTrainingOptions() {
	trainingOptions.push_back("   1. Eggs and toast (+1 HP, free!)");
	trainingOptions.push_back("   2. Curry rice (+2 HP, 5 gold)");
	trainingOptions.push_back("   3. Hearty beef stew (+5 HP, 10 gold)");
	trainingOptions.push_back("   0. Leave and choose another area");
}
void Kitchen::setTrainingDialogue() {
	trainingDialogue.push_back(" toasts some sourdough and fries up an over-easy egg...\n");
	trainingDialogue.push_back(" sets up the rice cooker and starts a pot of curry with vegetables...\n");
	trainingDialogue.push_back(" throws together a delicious smelling beef stew...\n");
}
// output option choices intro
void Kitchen::chooseOption() {
	std::cout << "\nYou have arrived at the kitchen.\n"
		<< "Your pet can increase their health here!\n\n"
		<< "Pick a health training option (1, 2, 3, or 0 to go back): \n";
}
// output option choices from trainingOptions
void Kitchen::getOptions() {
	for (std::string option : trainingOptions) {
		std::cout << option << "\n";
	}
}