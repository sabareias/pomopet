#include "Field.h"
#include <iostream>

Field::Field() {
	setTrainingOptions();
	setTrainingDialogue();
}

void Field::setTrainingOptions() {
	trainingOptions.push_back("   1. Hike up hills (+1 speed, free!)");
	trainingOptions.push_back("   2. Jog through the tall grass (+2 speed, 5 gold)");
	trainingOptions.push_back("   3. Sprint towards the river (+5 speed, 10 gold)");
	trainingOptions.push_back("   0. Leave and choose another area");
}
void Field::setTrainingDialogue() {
	trainingDialogue.push_back(" scampers up and down nearby hills using a branch as a walking stick...\n");
	trainingDialogue.push_back(" jogs laps through the tall grass, leaving a track in their wake...\n");
	trainingDialogue.push_back(" repeatedly sprints full speed towards the river and back again...\n");
}
// output option choices intro
void Field::chooseOption() {
	std::cout << "\nYou have arrived at the field.\n"
		<< "Your pet can increase their speed here!\n\n"
		<< "Pick a speed training option (1, 2, 3, or 0 to go back): \n";
}
// output option choices from trainingOptions
void Field::getOptions() {
	for (std::string option : trainingOptions) {
		std::cout << option << "\n";
	}
}