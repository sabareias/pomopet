#pragma once
#include "Location.h"

struct WishingWell : public Location {
	WishingWell();
	std::string stat = "luck";
	void setTrainingOptions() override;
	void setTrainingDialogue() override;

	void chooseOption() override;
	void getOptions() override;
};