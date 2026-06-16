#include <fstream>
#include "engine.h"

bool updateDailyFilesIfNeeded() {
	time_t t = time(0);
	tm* t1 = localtime(&t);
	int cd = t1->tm_mday; // current day

	std::fstream in(LAST_DAY_TXT_PATH, std::ios_base::in);
	if(!in.is_open()) {
		in.open(LAST_DAY_TXT_PATH, std::ios_base::out);
		in << cd;
		return false;
	}

	int ld; // last day
	in >> ld;
	if(ld != cd) {
		in.close();
		{ // clears files
			in.open(LAST_DAY_TXT_PATH, std::ios_base::out | std::ios_base::ate);
			in << cd;
			in.close();

			in.open(RATING_TXT_PATH, std::ios_base::out | std::ios_base::ate);
			in.close();
		}

		return false;
	}
	return true;
}

int main() {
	updateDailyFilesIfNeeded();
	Engine engine(50);
	engine.start();
	return 0;
}
