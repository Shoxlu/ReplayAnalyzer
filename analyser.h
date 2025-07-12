#pragma once
#include <vector>
#include <string>

enum Watchers {
	Lives,
	Bombs
};

struct ReplayInfo {
	ReplayInfo() :n_miss(0), n_bombs(0) {};
	size_t n_miss;
	size_t n_bombs;
};

class WatcherInt {
public:

	WatcherInt():
		pEntry(nullptr),
		entry(NULL),
		prev_entry(NULL),
		has_decreased(false),
		has_increased(false) 
	{
	};

	WatcherInt(int* address_entry): 
		pEntry(address_entry),
		entry(NULL),
		prev_entry(NULL),
		has_decreased(false), 
		has_increased(false)
	{
		if (!pEntry) {
			return;
		}
		entry = *pEntry;
		prev_entry = entry;
	};
	void Update() {
		has_increased = false;
		has_decreased = false;
		if (!pEntry) {
			return;
		}
		entry = *pEntry;
		diff = entry - prev_entry;
		if (diff > 0) {
			has_increased = true;
		}
		else if (diff < 0) {
			has_decreased = true;
		}
		prev_entry = entry;
	}
	bool hasChanged() {
		return has_increased || has_decreased;
	}
	bool hasIncreased() {
		return has_increased;
	}
	bool hasDecreased() {
		return has_decreased;
	}
	int diff;
	int entry;
	int* pEntry;
private:
	int prev_entry;
	bool has_increased;
	bool has_decreased;
};

class Analyzer {
public:
	Analyzer();
	~Analyzer();
	void Update();
	void SaveResults(std::string file_name);
	int32_t analyze_state = 0;
private:
	std::vector<WatcherInt> observations;
	ReplayInfo results;
};