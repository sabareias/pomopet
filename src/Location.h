#pragma once
#include <string>
#include <vector>

struct Location {
	Location(std::string stat = "") {};

	std::string stat;
	std::vector<std::string> trainingOptions;
	std::vector<std::string> trainingDialogue;

	virtual void setTrainingOptions() = 0;
	virtual void setTrainingDialogue() = 0;
	virtual void chooseOption() = 0;
	virtual void getOptions() = 0;

	int training2_cost = 5;
	int training3_cost = 10;
};