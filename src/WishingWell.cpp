#include "WishingWell.h"
#include <iostream>

WishingWell::WishingWell() {
	setTrainingOptions();
	setTrainingDialogue();
}

void WishingWell::setTrainingOptions() {
	trainingOptions.push_back("   1. Look for 4-leaf clovers (+1 luck, free!)");
	trainingOptions.push_back("   2. Small donation (+2 luck, 5 gold)");
	trainingOptions.push_back("   3. Big donation (+5 luck, 10 gold)");
	trainingOptions.push_back("   0. Leave and choose another area");
}
void WishingWell::setTrainingDialogue() {
	trainingDialogue.push_back(" searches around the well for 4-leaf clovers...\n");
	trainingDialogue.push_back(" throws 5 gold into the well...\n");
	trainingDialogue.push_back(" throws 10 gold into the well...\n");
}
// output option choices intro
void WishingWell::chooseOption() {
	std::cout << "\nYou have arrived at the wishing well.\n"
		<< "Your pet can increase their luck here!\n\n"
		<< "Pick a luck training option (1, 2, 3, or 0 to go back): \n";
}
// output option choices from trainingOptions
void WishingWell::getOptions() {
	for (std::string option : trainingOptions) {
		std::cout << option << "\n";
	}
}