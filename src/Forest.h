#pragma once
#include "Location.h"

struct Forest : public Location {
	Forest();
	std::string stat = "strength";
	void setTrainingOptions() override;
	void setTrainingDialogue() override;

	void chooseOption() override;
	void getOptions() override;
};