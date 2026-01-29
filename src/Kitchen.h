#pragma once
#include "Location.h"

struct Kitchen : public Location {
	Kitchen();
	std::string stat = "health";
	void setTrainingOptions() override;
	void setTrainingDialogue() override;

	void chooseOption() override;
	void getOptions() override;
};