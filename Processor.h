#pragma once
#include <mutex>
#include "GlobalStructs.h"
#include "ALBUM.h"
#include "beat_compiler_extension.h"
#include "MiniAudioWrapper.h"
#include "DeckWorker.h"
class Processor
{
private:
	std::unordered_map<int, ALBUM*> deck;
	ma_device idle_mode;//engine data stored
	ma_device dj_mode;//device for dj playback
	int AB_size = 0;//audio buffer size
	//ma_device daw_mode;//device for daw playback


	//ma_uint32 playback_count;//data for count every data_callback

	std::unordered_map<int, bool> stop_queue;

	void dj_data_read(BINRAW*& mix_bin, const RAWSIZE& binsize);//dj data read init
	void worker_hire();//make worker threads
	void worker_layoff();

	void init_first_album();
public:
	inline void init_time_count();
	ma_uint64 processed_time = 0;
	clock_t updated_clock;
	bool ok_to_count = false;
	Processor(const int& audio_buffer_size);
	~Processor();

	std::vector<DeckWorker*> company;

	std::mutex work_mutex;
	std::condition_variable work_call;
	std::mutex end_mutex;
	std::condition_variable end_sync;

	bool MASS_LAYOFFS = false;//event trigger, fire all worker threads
	int MAX_DECK_USE = 4;

	int LOCK_SAFE = 0;

	int work_counter = 0; std::mutex work_counter_locker;

	beat_compiler_extension* pBCE=nullptr;
	void* public_bufferout=nullptr;

	ALBUM* acc_album(const int& ID);
	bool find_remained_job(int& if_got_id);
	Faust_engine* acc_faust(const int& ID);
	void START_DJ();//ignite first album
	int deck_size();
	std::unordered_map<int, ALBUM*>::iterator get_deck_p();
	std::unordered_map<int, ALBUM*>::iterator get_end_p();
	bool ID_is_in_stopQ(const int& ID);
	double get_processed_time_ms();
	void add_processed_time(const ma_uint32& frame_use);
	void load_album(const std::string& meta_data_path, const int& albumID);
	void unload_album(const int& albumID);
	std::mutex buf_mutex;//access buffer mutex, use in album class
	std::mutex job_seeker_mutex;

	ma_uint64 raw_to_processed(double raw);
	
	//bool processing = false;
	void delete_album(const int& ID);
	void clear_stopQ();
	void delete_in_stopQ(const int& ID);
	void set_MAX_DECK_USE(const int& deck_num);
	int get_audio_buffer_size();
	void go_dj(BINRAW*& dj_data, const RAWSIZE& binsize);
	void go_manual();
	bool is_on_manual = false;

};

