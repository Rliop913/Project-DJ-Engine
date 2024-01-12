#include "Processor.h"
#include "EngineCallBacks.h"

Processor::Processor()
{
	idle_mode = MAW::init_device(ma_device_type_playback, true, idle_callback, this);
	dj_mode = MAW::init_device(ma_device_type_playback, true, dj_callback, this);
	//daw_mode = MAW::init_device(ma_device_type_playback, true, daw_callback, this);
	ma_device_start(&idle_mode);
	is_on_manual = true;
}

Processor::~Processor()
{
	ma_device_uninit(&idle_mode);
	ma_device_uninit(&dj_mode);
	ma_device_uninit(&daw_mode);
}

void 
Processor::load_album(const std::string& song_path, const std::string& meta_data_path, const int& albumID)
{
	deck[albumID] = new ALBUM(song_path, 2, albumID, this, meta_data_path);
}

void
Processor::unload_album(const int& albumID)
{
	stop_queue[albumID] = true;
}


Faust_engine*
Processor::acc_faust(const int& ID)
{
	return deck[ID]->get_faust();
}

ALBUM*
Processor::acc_album(const int& ID)
{
	return deck[ID];
}

int
Processor::deck_size()
{
	return deck.size();
}

void
Processor::init_time_count()
{
	ok_to_count = true;
	processed_time = 0;
}


void
Processor::START_DJ()
{
	deck[0]->PLAY();
	init_time_count();
	
}

std::unordered_map<int, ALBUM*>::iterator
Processor::get_deck_p()
{
	return deck.begin();
}

bool
Processor::ID_is_in_stopQ(const int& ID)
{
	return stop_queue.contains(ID);
}

void
Processor::delete_album(const int& ID)
{
	delete deck[ID];
	deck.erase(ID);
	return;
}

void
Processor::clear_stopQ()
{
	stop_queue.clear();
	return;
}


void
Processor::dj_data_read(const std::string& new_mix_path) {
	dj_init_group DIG;
	DIG.dj_data_path = new_mix_path;
	DIG.process_pointer = (void*)this;
	pBCE = new beat_compiler_extension(DIG);
	worker_maker();
	init_first_album();
}


void
Processor::worker_maker() {
	for (int i = 0; i < MAX_DECK_USE; i++) {
		t_p.emplace_back([](Processor* em) {em->worker(); }, this);
	}
}


void
Processor::worker() {
	while (true) {
		if (BREAK_CALL) {//breaker event, trig when off dj mode
			return;
		}
		else if (!TRIG_CALL) {//TRIG
			continue;
		}
		else {
			work job_pointer;
			if (cq.try_pop(job_pointer)) {
				deck[job_pointer.ID]->dj_process(job_pointer.frameCount, (float*)job_pointer.buf);//process data
			}
			else {
				TRIG_CALL = false;
			}
		}
	}
}



void
Processor::worker_uninit() {
	BREAK_CALL = true;
	if (t_p.size() != 0) {
		for (int i = 0; i < t_p.size(); i++) {
			t_p[i].join();
		}
		t_p.clear();
	}
	BREAK_CALL = false;
}

void
Processor::set_MAX_DECK_USE(const int& deck_num)
{
	MAX_DECK_USE = deck_num;
	return;
}


void
Processor::init_first_album() {
	std::unordered_map<int, std::vector<engine_order>>* RS = &pBCE->reservation_storage;
	for (int i = 0; i < (*RS)[0].size(); i++) {
		if ((*RS)[0].at(i).tag.at("type") == "LOAD") {
			if ((*RS)[0].at(i).tag.at("for_who") == "0") {
				load_album((*RS)[0].at(i).tag.at("str_first"), (*RS)[0].at(i).tag.at("str_second"), 0);
				(*RS)[0].erase((*RS)[0].begin() + i);
				return;
			}
		}
	}
	//no starter ERROR
}

ma_uint64
Processor::raw_to_processed(double raw) {//time to frame
	return (ma_uint64)ceil(48000.0 * raw);
}

ma_uint64
Processor::get_processed_time()
{
	return processed_time;
}

void
Processor::add_processed_time(const ma_uint32& frame_use)
{
	ok_to_count ? processed_time += frame_use : false;
}

//ma_uint64
//Engine_Main::make_it_not_odd(int num) {
//	return num % 2 == 0 ? num : num-1;
//}

void
Processor::go_dj(const std::string& dj_data)
{
	ma_device_stop(&idle_mode);
	ma_device_start(&dj_mode);
	dj_data_read(dj_data);
	is_on_manual = false;
}


void
Processor::go_manual()
{
	ma_device_stop(&dj_mode);
	ma_device_start(&idle_mode);
	is_on_manual = true;
}

