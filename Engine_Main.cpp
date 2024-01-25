#pragma once
#include "Engine_Main.h"
#include "default_functions.h"
function_pointers GFP;

Engine_Main::~Engine_Main() {
	delete(PDJE_processor);
}

Engine_Main::Engine_Main(const int& init_audio_buffer_size) {
	GFP.init_fileitr = default_file_init;
	GFP.line_getter = default_line_getter;
	GFP.loader_function = default_decoder_init;
	GFP.uninit_fileitr = default_file_uninit;
	PDJE_processor = new Processor(init_audio_buffer_size);
}


Engine_Main::Engine_Main(const function_pointers& ppp, const int& init_audio_buffer_size) {
	GFP.init_fileitr = ppp.init_fileitr;
	GFP.line_getter = ppp.line_getter;
	GFP.loader_function = ppp.loader_function;
	GFP.uninit_fileitr = ppp.uninit_fileitr;
	PDJE_processor = new Processor(init_audio_buffer_size);
}
//---------------------------------engine_callback--------------------------------------//
int clip_count=0;
//
//ma_uint64
//Engine_Main::get_processed_time() {
//	return PDJE_processor->get_processed_time();
//}








//
//void
//Engine_Main::daw_worker() {
//	while (true) {
//		if (BREAK_CALL) {//breaker event, trig by Engine_Main class
//			return;
//		}
//		else if (!TRIG_CALL) {//TRIG
//			continue;
//		}
//		else {
//			work job_pointer;
//			if (cq.try_pop(job_pointer)) {
//				deck[job_pointer.ID]->daw_process(job_pointer.frameCount, (float*)job_pointer.buf);//process data
//			}
//			else {
//				TRIG_CALL = false;
//			}
//		}
//	}
//}
//
//void
//Engine_Main::daw_worker_maker() {
//	for (int i = 0; i < MAX_DECK_USE; i++) {
//		t_p.emplace_back([](Engine_Main* em) {em->worker(); }, this);
//	}
//}

//void
//Engine_Main::sw_to_daw(std::string planning_data_path) {
//	clear_deck_dj_area();
//	clear_deck_manual_area();
//	ma_device_stop(&dj_mode);
//	ma_device_stop(&idle_mode);
//	ma_device_stop(&daw_mode);
//	daw_d.editing_data_path = planning_data_path;
//	daw_update_reserve();
//}


void
Engine_Main::sw_to_manual(const std::string& song_path, const std::string& song_meta_path) {
	clear_deck();
	PDJE_processor->go_manual();
	manual_music_change(song_path, song_meta_path);
}

void
Engine_Main::manual_music_change(const std::string& song_path, const std::string& song_meta_path) {
	if (PDJE_processor->is_on_manual) {
		if (PDJE_processor->deck_size()==0) {
			PDJE_processor->load_album(song_path, song_meta_path,-1);
			manual_playback(-1);
		}
		else {
			clear_deck();
			PDJE_processor->load_album(song_path, song_meta_path, -1);
			manual_playback(-1);
		}
	
	}
	else {
		return;
	}
}

void
Engine_Main::manual_playback(const int& albumID) {
	if (!PDJE_processor->is_on_manual) {
		return;
	}
	if(PDJE_processor->deck_size()==0) {
		return;
	}
	else {
		PDJE_processor->acc_album(-1)->PLAY();
	}
}

void
Engine_Main::manual_stop(const int& albumID) {
	if (!PDJE_processor->is_on_manual) {
		return;
	}
	if (PDJE_processor->deck_size() == 0) {
		return;
	}
	else {
		PDJE_processor->acc_album(-1)->STOP();
	}
	
}

void
Engine_Main::manual_effect(const int& effect_type,const float& first, const float& second, const float& third) {
	Faust_engine* FE;
	if (!PDJE_processor->is_on_manual) {
		return;
	}
	if (PDJE_processor->deck_size() == 0) {
		return;
	}
	FE = PDJE_processor->acc_faust(-1);
	switch (effect_type)
	{
	case E_EQ_hi:
		first < -60.0f ? 
			FE->EQ_high_sw(false)
			:
			FE->EQ_high_sw(true);

		FE->set_eq_high_value(first);
		break;
	case E_EQ_mid:
		first < -60.0f ?
			FE->EQ_mid_sw(false)
			:
			FE->EQ_mid_sw(true);

		FE->set_eq_mid_value(first);
		break;
	case E_EQ_low:
		first < -60.0f ?
			FE->EQ_low_sw(false)
			:
			FE->EQ_low_sw(true);

		FE->set_eq_low_value(first);
		break;
	case E_lpf:
		first < 0.0f ?
			FE->filter_low_sw(false)
			:
			FE->filter_low_sw(true);

		FE->set_filter_low_freq_value(int(first));
		break;
	case E_hpf:
		first < 0.0f ?
			FE->filter_high_sw(false)
			:
			FE->filter_high_sw(true);

		FE->set_filter_high_freq_value(int(first));
		break;
	case E_echo:

		first < 0.0f ?
			FE->_echo_sw(false)
			:
			FE->_echo_sw(true);

		FE->set_echo_dur_value(first);
		FE->set_echo_feedback_value(second);
		FE->set_echo_power(third);
		break;
	case E_l_f_s:

		first < 0.0f ?
			FE->low_siren_sw(false)
			:
			FE->low_siren_sw(true);

		FE->set_l_f_s_bps_value(first);
		FE->set_l_f_s_gain_min_freq(int(second));
		FE->set_l_f_s_power(third);
		break;
	case E_phaser:

		first < 0.0f ?
			FE->_phaser_sw(false)
			:
			FE->_phaser_sw(true);

		FE->set_phaser_bps_value(first);
		FE->set_phaser_gain_value(second);
		FE->set_phaser_power(third);
		break;
	case E_flanger:

		first < 0.0f ?
			FE->_flanger_sw(false)
			:
			FE->_flanger_sw(true);

		FE->set_flanger_bps_value(first);
		FE->set_flanger_gain_value(second);
		FE->set_flanger_power(third);
		break;
	case E_trance:

		first < 0.0f ?
			FE->_trance_sw(false)
			:
			FE->_trance_sw(true);

		FE->set_trance_bps(first);
		FE->set_trance_gain(second);
		FE->set_trance_power(third);
		break;
	case E_panner:

		first < 0.0f ?
			FE->_panner_sw(false)
			:
			FE->_panner_sw(true);

		FE->set_panner_bps(first);
		FE->set_panner_gain(second);
		FE->set_phaser_power(third);
		break;
	case E_distortion:

		first < 0.0f ?
			FE->_distortion_sw(false)
			:
			FE->_distortion_sw(true);

		FE->set_distortion_gain(first);
		break;
	case E_roll:
		first < 0.0f ?
			FE->_roll_sw(false)
			:
			FE->_roll_sw(true);
		FE->set_roller_bpm(first);
		FE->set_roller_power(second);
		break;

	default:
		break;
	}
}

void
Engine_Main::sw_to_dj(const std::string& dj_data_path) {
	clear_deck();
	PDJE_processor->go_dj(dj_data_path);
}


void
Engine_Main::dj_start() {
	PDJE_processor->START_DJ();
}


void
Engine_Main::clear_deck()
{
	auto Dp = PDJE_processor->get_deck_p();
	for (int i = 0; i < PDJE_processor->deck_size(); ++i) {
		PDJE_processor->delete_album(Dp->first);
		++Dp;
	}
	PDJE_processor->clear_stopQ();
}





//------------------------------------DAW------------------------------//
//void
//Engine_Main::metronome_activate(std::string sound_path,float sound_volume) {
//	metronome_sound = sound_path;
//	met_vol = sound_volume;
//	met_activate = true;
//}
//
//
//std::unordered_map<int,mix_data_set>
//Engine_Main::update_mixing_status_in_deck() {
//	
//	std::unordered_map<int,mix_data_set> temp_set;
//	if (PDJE_processor->deck_size()==0) {
//		return temp_set;//nothing in deck return
//	}
//	else {
//		
//		for (auto i = deck.begin(); i != deck.end(); i++) {
//			
//			temp_set[i->first] = i->second->get_mixing_data();
//		}
//		return temp_set;//update complete return
//	}
//
//}

//
//void
//Engine_Main::daw_solo(int ID) {
//	daw_d.is_soloing = true;
//	daw_d.target_ID = ID;
//}
//
//void
//Engine_Main::daw_unsolo() {
//	daw_d.is_soloing = false;
//}
//
//void
//Engine_Main::daw_set_master_sola(float value) {
//	daw_d.master_sola = value;
//}
//
//void
//Engine_Main::daw_test_play(std::vector<daw_status_frame> data_pointer) {
//	//make threads
//	BREAK_CALL = false;
//	daw_worker_maker();
//	//check reserve data is changed
//	daw_update_reserve();
//	reservation_storage.clear();
//	reservation_storage = daw_d.reserve_buffer;
//
//
//	//fill deck
//
//	if (data_pointer.empty()) {//means start in first
//		init_first_album();
//	}
//	else {//starts in middle
//		for (int i = 0; i < data_pointer.size(); i++) {
//			daw_status_frame* ds = &data_pointer[i];
//			load_album(data_pointer[i].song_path, data_pointer[i].ID);
//			Faust_engine* fe = deck[data_pointer[i].ID]->album_engine;
//
//			fe->trim = ds->trim;
//			fe->fader = ds->fader;
//			//switches
//			fe->lo_fi_sw = ds->lo_fi_sw;
//			fe->hi_fi_sw = ds->hi_fi_sw;
//			fe->eq_mid_sw = ds->eq_mid_sw;
//			fe->eq_low_sw = ds->eq_low_sw;
//			fe->eq_high_sw = ds->eq_high_sw;
//			fe->echo_sw = ds->echo_sw;
//			fe->l_f_s_sw = ds->l_f_s_sw;
//			fe->phaser_sw = ds->phaser_sw;
//			fe->trance_sw = ds->trance_sw;
//			fe->panner_sw = ds->panner_sw;
//			fe->distortion_sw = ds->distortion_sw;
//			//values
//			fe->low_freq_value = ds->low_freq_value;
//			fe->high_freq_value = ds->high_freq_value;
//			fe->eq_mid_v = ds->eq_mid_v;
//			fe->eq_low_v = ds->eq_low_v;
//			fe->eq_hi_v = ds->eq_hi_v;
//			fe->echo_dur = ds->echo_dur;
//			fe->echo_feedback_var = ds->echo_feedback_var;
//			fe->l_f_s_bps = ds->l_f_s_bps;
//			fe->flanger_bps = ds->flanger_bps;
//			fe->flanger_gain = ds->flanger_gain;
//			fe->phaser_bps = ds->phaser_bps;
//			fe->phaser_gain = ds->phaser_gain;
//			fe->trance_bps = ds->trance_bps;
//			fe->panner_bps = ds->panner_bps;
//			fe->distortion_gain = ds->distortion_gain;
//			fe->trance_gain = ds->trance_gain;
//			fe->l_f_s_gain_min_freq = ds->l_f_s_gain_min_freq;
//			fe->panner_gain = ds->panner_gain;
//			deck[ds->ID]->this_data.playback_ordered = ds->playback_ordered;
//			deck[ds->ID]->inter_queue = ds->interpolate_queue;//deep copy
//			ma_decoder_seek_to_pcm_frame(&deck[ds->ID]->music, ds->cursor);
//			
//			int temp = reservation_storage[ds->ID].size();
//			for (int j = 0; j < temp; j++) {//check reservation and delete unusing datas
//				if (reservation_storage[ds->ID].size() == 0) {
//					break;
//				}
//				if (raw_to_processed(reservation_storage[ds->ID].at(0).frame_in) < ds->cursor) {
//					(reservation_storage[ds->ID]).erase(reservation_storage[ds->ID].begin());
//				}
//				else {
//					break;
//				}
//			}
//		}
//	}
//	// start play
//	ma_device_start(&daw_mode);
//
//}

//
//void
//Engine_Main::daw_update_reserve() {
//	if (daw_d.data_written||!comp_alive) {
//		if (comp_alive) {
//			delete pcompiler;
//		}
//		beat_compiler_extension::dj_init_group dig;
//		dig.dj_data_path = daw_d.editing_data_path;
//		dig.engine_pointer = this;
//		dig.init_fileitr = cfp.init_fileitr;
//		dig.line_getter = cfp.line_getter;
//		dig.uninit_fileitr = cfp.uninit_fileitr;
//		pcompiler = new beat_compiler_extension(dig);
//		daw_d.reserve_buffer = reservation_storage;
//	}
//	else {
//		reservation_storage.clear();
//		reservation_storage = daw_d.reserve_buffer;
//	}
//}
//
//
//void
//Engine_Main::daw_stop() {
//	worker_uninit();
//	ma_device_stop(&daw_mode);//stop play
//	deck.clear();
//	
//}
//
//void
//Engine_Main::daw_set_max_deck_use(int value) {
//	MAX_DECK_USE = value;
//}
//
//void
//Engine_Main::view_album(ma_uint8*& buffer, ma_uint64* len, int rev_scale,std::string song_path) {
//	ma_decoder_config cf;
//	cf = ma_decoder_config_init(ma_format_u8, 1, 48000/rev_scale);
//	ma_decoder de;
//	loader_function(song_path, &de, &cf);
//	//ma_decoder_init_file(song_path.c_str(), &cf, &de);
//	ma_decoder_get_length_in_pcm_frames(&de, len);
//	ma_uint64 returned = 0;
//	buffer = new ma_uint8[*len];
//	ma_decoder_read_pcm_frames(&de, buffer, *len, &returned);	
//	*len = returned;
//	ma_decoder_uninit(&de);
//	return;
//}

//
//
//void logCallback(void* pUserData, ma_uint32 logLevel, const char* message) {
//	// 로그 레벨에 따라 적절한 처리를 수행
//	switch (logLevel) {
//	case MA_LOG_LEVEL_ERROR:
//		fprintf(stderr, "Error: %s\n", message);
//		break;
//	case MA_LOG_LEVEL_WARNING:
//		fprintf(stderr, "Warning: %s\n", message);
//		break;
//	case MA_LOG_LEVEL_INFO:
//		printf("Info: %s\n", message);
//		break;
//	case MA_LOG_LEVEL_DEBUG:
//		printf("Debug: %s\n", message);
//		break;
//	default:
//		// 기타 로그 레벨에 대한 처리
//		break;
//	}
//}
//void mfre(void* p, void* userdata) {
//	free(p);
//}
//void* mmlac(size_t sz, void* udat) {
//	return malloc(sz);
//}
//void* mrel(void*p ,size_t sz, void* udat) {
//	return realloc(p, sz);
//}
//#include "include/SoundTouch.h"
//soundtouch::SoundTouch *sstt;
//
//
//void
//temp_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
//
//	ma_decoder* dec = (ma_decoder*)(pDevice->pUserData);
//	MAW::touch_sound(*sstt, frameCount, (float*)pOutput, *dec);
//}
int main()
{
	/*ma_log logger;
	ma_log_callback mlc = ma_log_callback_init(logCallback, NULL);
	ma_allocation_callbacks mac;
	mac.onFree = mfre;
	mac.onMalloc = mmlac;
	mac.onRealloc = mrel;
	mac.pUserData = NULL;
	*//*
	ma_log_init(&mac, &logger);
	ma_log_register_callback(&logger, mlc);*/
	/*sstt = new soundtouch::SoundTouch();
	sstt->setChannels(2);
	sstt->setSampleRate(48000);
	ma_decoder dec;
	ma_decoder_config decconf = ma_decoder_config_init(ma_format_f32, 2, 48000);
	ma_decoder_init_file("E://first_town.wav", &decconf, &dec);
	ma_device_config devconf = ma_device_config_init(ma_device_type_playback);
	devconf.playback.format = ma_format_f32;
	devconf.playback.channels = 2;
	devconf.sampleRate = 48000;
	devconf.dataCallback = temp_callback;
	devconf.pUserData = &dec;
	devconf.performanceProfile = ma_performance_profile_low_latency;
	ma_device dummy;
	ma_device dev;
	ma_device_init(NULL, &devconf, &dev);
	ma_device_start(&dev);*/
	//getchar();
	Engine_Main *EM = new Engine_Main(48);
	EM->sw_to_manual("E://Over Time.wav", "E://Over Time.json");
	//EM->sw_to_dj("./test_jsons/dj_set_2.json");
	//EM->dj_start();
	//EM->manual_effect(10, 170.0/60.0, 1.0, 0.0);
	while (true) {

		getchar();

		EM->manual_effect(12, 170.0*2.0, 1.0 ,0.5);

		getchar();
		EM->manual_effect(12, -1.0, 1.0, 0.5);

	}
	getchar();
	return 1;
}