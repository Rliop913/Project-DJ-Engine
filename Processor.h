#pragma once
#include <concurrent_queue.h>
#include <mutex>
#include "GlobalStructs.h"
#include "ALBUM.h"
#include "beat_compiler_extension.h"
#include "MiniAudioWrapper.h"
#include "miniaudio.h"

class Processor
{
private:
	std::unordered_map<int, ALBUM*> deck;
	ma_device idle_mode;//engine data stored
	ma_device dj_mode;//device for dj playback
	ma_device daw_mode;//device for daw playback


	//ma_uint32 playback_count;//data for count every data_callback

	std::vector<std::thread> t_p;
	std::string metronome_sound;//metronome's sound path
	float met_vol = 1.0f;//metronome's volume

	int MAX_DECK_USE = 4;
	ma_uint64 processed_time = 0;

	bool met_activate = false;//check metronome is active
	bool ok_to_count = false;
	daw_inner_data daw_d;
	//std::unordered_map<int, std::vector<engine_order>> reservation_storage;//the compiled data array for dj
	std::unordered_map<int, bool> stop_queue;
	inline void init_time_count();

	void dj_data_read(const std::string& new_mix_path);//dj data read init
	void worker_maker();//make worker threads
	void worker();//worker's thread function
	void worker_uninit();

	void init_first_album();
	
	
	//void daw_worker();
	//void daw_worker_maker();
	

public:
	Processor();
	~Processor();

	bool BREAK_CALL = false;//event trigger, break all worker threads
	bool TRIG_CALL = false;//event trigger, trig all worker threads

	concurrency::concurrent_queue<work> cq;	
	beat_compiler_extension* pBCE=nullptr;

	inline ALBUM* acc_album(const int& ID);
	inline Faust_engine* acc_faust(const int& ID);
	inline void START_DJ();//ignite first album
	inline int deck_size();
	inline std::unordered_map<int, ALBUM*>::iterator get_deck_p();
	inline bool ID_is_in_stopQ(const int& ID);
	inline ma_uint64 get_processed_time();
	inline void add_processed_time(const ma_uint32& frame_use);
	inline void load_album(const std::string& song_path, const std::string& meta_data_path, const int& albumID);
	inline void unload_album(const int& albumID);
	std::mutex buf_mutex;//access buffer mutex, use in album class


	ma_uint64 raw_to_processed(double raw);
	//ma_uint64 make_it_not_odd(int num);


	//bool processing = false;
	inline void delete_album(const int& ID);
	inline void clear_stopQ();
	inline void set_MAX_DECK_USE(const int& deck_num);

	inline void go_dj(const std::string& dj_data);
	inline void go_manual();
	bool is_on_manual = false;

};

