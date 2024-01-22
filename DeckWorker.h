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
public:
	bool work_complete = false;
	DeckWorker(Processor* master);
	~DeckWorker();

};

