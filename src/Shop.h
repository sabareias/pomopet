#pragma once
#include <string>
#include <vector>
#include <iostream>

#define COLOR_RESET "\x1b[0m"
#define COLOR_YELLOW "\x1b[1;33m"

struct Shop {
	Shop();

	void getConsumables();
	void getEquipment();

	void introMsg();
	void askShopChoice();
	void leaveShop();
	void purchaseMsg();
	void brokeMsg();

	bool canAfford(int gp, int price);

	void output_in_yellow(std::string name);

	std::vector<std::string> consumables;
	std::vector<std::string> equipment;

	int equipment_price = 50;
	int consumable_price = 5;

	int consumable_stat = 3;
	int equipment_stat = 20;
};