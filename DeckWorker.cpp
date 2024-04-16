#include "DeckWorker.h"
#include "Processor.h"


DeckWorker::DeckWorker(Processor* master, const int& number_got)
{
	my_number = number_got;
	employer_ptr = master;
	make_worker_loop();
}

DeckWorker::~DeckWorker()
{
	thr_ptr.join();
}



void
DeckWorker::make_worker_loop()
{
	thr_ptr = std::thread([this]() {
		looper();
		});
}


void
DeckWorker::get_fired()
{
	employer_ptr->delete_album(reserved_id);
	employer_ptr->delete_in_stopQ(reserved_id);
	employed = false;
}

bool
DeckWorker::go_find_a_job()
{
	int temp_memory = 0;
	employer_ptr->job_seeker_mutex.lock();
	if (employer_ptr->find_remained_job(temp_memory)) {
		employer_ptr->job_seeker_mutex.unlock();
		reserved_id = temp_memory;
		employed = true;

		return true;//got work
	}
	else {
		employer_ptr->job_seeker_mutex.unlock();
		return false;//no work remained
	}
}

void
DeckWorker::time_to_work()
{
	employer_ptr->acc_album(reserved_id)->
		dj_process(
			employer_ptr->get_audio_buffer_size(),
			(float*)employer_ptr->public_bufferout
		);

}

#include <time.h>
#include <iostream>
void
DeckWorker::looper()
{
	clock_t start, end;
	while (true)//worker life loop
	{
		std::unique_lock<std::mutex> lock(employer_ptr->work_mutex);
		employer_ptr->work_call.wait(lock, [this]() {return employer_ptr->LOCK_SAFE<employer_ptr->MAX_DECK_USE; });
		employer_ptr->LOCK_SAFE++;
		//work wait
		lock.unlock();

		//start = clock();

		if (employer_ptr->MASS_LAYOFFS) {
			return;//Break Call from processor
		}

		if (employed) {//go get work
			
			if (employer_ptr->ID_is_in_stopQ(reserved_id)) {
				get_fired();
				goto no_work_dive_point;
			}
			else {
			hired_dive_point:
				//start = clock();
				time_to_work();
				////end = clock() - start;
				////std::cout << end << std::endl;
				//employer_ptr->work_counter_locker.lock();
				////employer_ptr->work_counter += 1;
				//employer_ptr->work_counter_locker.unlock();
			}
		}
		else {//fired  -->  go find a job
		no_work_dive_point:
			if (go_find_a_job()) {
				goto hired_dive_point;
			}
			else {
				//employer_ptr->work_counter_locker.lock();
				////employer_ptr->work_counter += 1;
				//employer_ptr->work_counter_locker.unlock();
			}
		}

		std::unique_lock<std::mutex> endlock(employer_ptr->end_mutex);
		//employer_ptr->work_counter_locker.lock();
		employer_ptr->work_counter += 1;
		//employer_ptr->work_counter_locker.unlock();
		employer_ptr->work_call.wait(endlock);//work wait
		endlock.unlock();
		//end = clock() - start;
		//std::cout << end << std::endl;
	}
}