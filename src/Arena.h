#pragma once
#include <string>
#include <vector>
#include <iostream>

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_YELLOW "\x1b[1;33m"

struct Arena {
	Arena();

	void introMsg();
	void leaveArena();

	void askArenaChoice();

	void goFight(std::string& enemy);
	void goRace(std::string& enemy);

	void winnerMsg(std::string& enemy);
	void loserMsg(std::string& enemy);
	void tieMsg(std::string& enemy);
	void closeMsg(std::string& enemy, std::string& pet, std::string condition, std::string matchType);

	void output_in_yellow(std::string name);

	bool win = false;

};