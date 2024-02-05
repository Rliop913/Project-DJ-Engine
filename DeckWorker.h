#pragma once
#include "GlobalStructs.h"
#include <thread>
class Processor;
class DeckWorker
{
private:
	int reserved_id = 0;
	bool employed = false;
	Processor* employer_ptr=nullptr;
	std::thread thr_ptr;
	void time_to_work();
	void get_fired();
	bool go_find_a_job();
	void looper();
	void make_worker_loop();
	int my_number = -1;
public:
	DeckWorker(Processor* master, const int& number_got);
	~DeckWorker();

};

