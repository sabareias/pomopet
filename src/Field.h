#pragma once
#include "Location.h"

struct Field : public Location {
	Field();
	std::string stat = "speed";
	void setTrainingOptions() override;
	void setTrainingDialogue() override;

	void chooseOption() override;
	void getOptions() override;
};