#include "Forest.h"
#include <iostream>

Forest::Forest() {
	setTrainingOptions();
	setTrainingDialogue();
}

void Forest::setTrainingOptions() {
	trainingOptions.push_back("   1. Sword fight with a stick (+1 str, free!)");
	trainingOptions.push_back("   2. Kickbox treetrunks (+2 str, 5 gold)");
	trainingOptions.push_back("   3. Lift and throw heavy rocks (+5 str, 10 gold)");
	trainingOptions.push_back("   0. Leave and choose another area");
}
void Forest::setTrainingDialogue() {
	trainingDialogue.push_back(" hunts down the perfect sword-sized stick and starts slashing it about...\n");
	trainingDialogue.push_back(" begins rhythmically punching and kicking tree trunks as hard as they can...\n");
	trainingDialogue.push_back(" locates a nice sized rock, hefting it up and throwing it at trees over and over...\n");
}
// output option choices intro
void Forest::chooseOption() {
	std::cout
		<< "\nYou have arrived at the forest.\n"
		<< "Your pet can increase their strength here!\n\n"
		<< "Pick a strength training option (1, 2, 3, or 0 to go back): \n";
}
// output option choices from trainingOptions
void Forest::getOptions() {
	for (std::string option : trainingOptions) {
		std::cout << option << "\n";
	}
}