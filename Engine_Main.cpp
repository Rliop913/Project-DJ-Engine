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


void
Engine_Main::sw_to_manual(const std::string& song_meta_path) {
	clear_deck();
	PDJE_processor->go_manual();
	manual_music_change(song_meta_path);
}

void
Engine_Main::manual_music_change(const std::string& song_meta_path) {
	if (PDJE_processor->is_on_manual) {
		if (PDJE_processor->deck_size()==0) {
			PDJE_processor->load_album(song_meta_path,-1);
			manual_playback(-1);
		}
		else {
			clear_deck();
			PDJE_processor->load_album(song_meta_path, -1);
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





//
//int main()
//{
//	/*ma_log logger;
//	ma_log_callback mlc = ma_log_callback_init(logCallback, NULL);
//	ma_allocation_callbacks mac;
//	mac.onFree = mfre;
//	mac.onMalloc = mmlac;
//	mac.onRealloc = mrel;
//	mac.pUserData = NULL;
//	*//*
//	ma_log_init(&mac, &logger);
//	ma_log_register_callback(&logger, mlc);*/
//	/*sstt = new soundtouch::SoundTouch();
//	sstt->setChannels(2);
//	sstt->setSampleRate(48000);
//	ma_decoder dec;
//	ma_decoder_config decconf = ma_decoder_config_init(ma_format_f32, 2, 48000);
//	ma_decoder_init_file("E://first_town.wav", &decconf, &dec);
//	ma_device_config devconf = ma_device_config_init(ma_device_type_playback);
//	devconf.playback.format = ma_format_f32;
//	devconf.playback.channels = 2;
//	devconf.sampleRate = 48000;
//	devconf.dataCallback = temp_callback;
//	devconf.pUserData = &dec;
//	devconf.performanceProfile = ma_performance_profile_low_latency;
//	ma_device dummy;
//	ma_device dev;
//	ma_device_init(NULL, &devconf, &dev);
//	ma_device_start(&dev);*/
//	//getchar();
//	Engine_Main *EM = new Engine_Main(48);
//	EM->sw_to_manual("E://Over Time.wav", "E://Over Time.json");
//	//EM->sw_to_dj("./test_jsons/dj_set_2.json");
//	//EM->dj_start();
//	//EM->manual_effect(10, 170.0/60.0, 1.0, 0.0);
//	while (true) {
//
//		getchar();
//
//		EM->manual_effect(12, 170.0*2.0, 1.0 ,1.0);
//
//		getchar();
//		EM->manual_effect(12, -1.0, 1.0, 0.0);
//
//	}
//	getchar();
//	return 1;
//}
int main()
{
	Engine_Main* EM = new Engine_Main(48);
	EM->sw_to_dj("./test_jsons/dj_set_2.json");
	EM->dj_start();
	getchar();
}