#include "Processor.h"
#include "EngineCallBacks.h"


Processor::Processor(const int& audio_buffer_size, function_pointers& fp)
{
	EFP = &fp;
	AB_size = audio_buffer_size;
	MAW::init_device(idle_mode, ma_device_type_playback, AB_size, idle_callback, this);
	MAW::init_device(dj_mode, ma_device_type_playback, AB_size, dj_callback, this);
	//MAW::stop_device(dj_mode);
	//daw_mode = MAW::init_device(ma_device_type_playback, true, daw_callback, this);
	//ma_device_start(&idle_mode);
	is_on_manual = true;
}

Processor::~Processor()
{
	MAW::stop_device(idle_mode);
	MAW::stop_device(dj_mode);
	MAW::uninit_device(idle_mode);
	MAW::uninit_device(dj_mode);
	//ma_device_uninit(&daw_mode);
}



void 
Processor::load_album(const std::string& song_meta_path, const int& albumID)
{
	char* meta_bin;
	long long metasz;
	data_manager dm(*EFP);
	dm[song_meta_path];
	//EFP->read_whole_file(meta_bin, metasz, song_meta_path);
	deck[albumID] = new ALBUM(2, albumID, this, dm);
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
	return int(deck.size());
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

std::unordered_map<int, ALBUM*>::iterator 
Processor::get_end_p()
{
	return deck.end();
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
Processor::dj_data_read(const std::string& dj_data_path) {
	dj_init_group DIG;
	DIG.dj_data = dj_data_path;
	DIG.process_pointer = (void*)this;
	pBCE = new beat_compiler_extension(DIG, *EFP);
	worker_hire();
	init_first_album();
}


void
Processor::worker_hire() {
	MASS_LAYOFFS = false;
	startline = new std::barrier(MAX_DECK_USE+1);
	work_end_buzzer = new std::barrier(MAX_DECK_USE+1);
	for (int i = 0; i < MAX_DECK_USE; i++) {
		DeckWorker* new_employee = new DeckWorker(this, i);
		company.push_back(new_employee);
	}
}

bool
Processor::find_remained_job(int& if_got_job)
{
	for (auto i = deck.begin(); i != deck.end(); ++i) {
		if (!i->second->has_owner) {
			if_got_job = i->first;
			i->second->has_owner = true;
			return true;
		}
	}
	return false;
}


void
Processor::delete_in_stopQ(const int& ID)
{
	stop_queue.erase(ID);
}


void
Processor::worker_layoff() {
	MASS_LAYOFFS = true;
	for (int i = 0; i < company.size(); ++i) {
		delete company[i];
	}
	company.clear();
	delete(startline);
	delete(work_end_buzzer);
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
		if (SI((*RS)[0].at(i).dj_tags["type"]) == dj_type::LOAD) {
			if (SI((*RS)[0].at(i).dj_tags["target"]) == 0) {
				load_album((*RS)[0].at(i).dj_tags["first"], 0);
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

double
Processor::get_processed_time_ms()
{
	int corrected_ms = clock() - updated_clock;
	double ms = processed_time / 48.0;
	return (ms+(double)corrected_ms);
}

inline void
Processor::add_processed_time(const ma_uint32& frame_use)
{
	ok_to_count ? processed_time += frame_use : false;
	updated_clock = clock();
}

//ma_uint64
//Engine_Main::make_it_not_odd(int num) {
//	return num % 2 == 0 ? num : num-1;
//}

void
Processor::go_dj(const std::string& dj_data_path)
{
	MAW::stop_device(idle_mode);
	dj_data_read(dj_data_path);
	MAW::start_device(dj_mode);
	is_on_manual = false;
}


void
Processor::go_manual()
{
	MAW::stop_device(dj_mode);
	MAW::start_device(idle_mode);
	is_on_manual = true;
}


int
Processor::get_audio_buffer_size()
{
	return AB_size;
}
