#include "sound_module.h"
#include "Processor.h"
#include "ALBUM.h"
#include "CURSOR.h"


sound_module::sound_module(Processor* first, ALBUM* second, Faust_engine* third) {
	processor = first;
	palbum = second;
	pfaust = third;
}

void
sound_module::toggle(const DDTG& tag) {
	switch (tag.type) {
	case dj_type::EQ:
		eq_tog(tag);
		break;
	case dj_type::DISTORTION:
		distortion_tog(tag);
		break;
	case dj_type::FILTER:
		filter_tog(tag);
		break;
	case dj_type::CONTROL:
		control_tog(tag);
		break;
	case dj_type::VOL:
		vol_tog(tag);
		break;
	case dj_type::LOAD:
		load_tog(tag);
		break;
	case dj_type::UNLOAD:
		unload_tog(tag);
		break;
	case dj_type::DJ_COMP:
		dj_comp_tog(tag);
		break;
	case dj_type::BEAT_MATCH:
		beat_match_tog(tag);
		break;
	case dj_type::SOLA:
		sola_tog(tag);
		break;
	case dj_type::ECHO:
		echo_tog(tag);
		break;
	case dj_type::LFS:
		LFS_tog(tag);
		break;
	case dj_type::FLANGER:
		flanger_tog(tag);
		break;
	case dj_type::PHASER:
		phaser_tog(tag);
		break;
	case dj_type::TRANCE:
		trance_tog(tag);
		break;
	case dj_type::PANNER:
		panner_tog(tag);
		break;
	case dj_type::BATTLE_DJ:
		battle_tog(tag);
		break;
	case dj_type::ROLL:
		roll_tog(tag);
		break;
	case dj_type::ALIVE:
		break;
	default:
		break;
	}
}

void
sound_module::eq_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);

	//Faust_engine* PF= processor->acc_faust(tag.target);//for self or other
	switch (tag.what)
	{
	case dj_what::HIGH:
		tag.first_value < -60.0 ?
			([PF](double val) {
			PF->EQ_high_sw(false); 
			PF->set_eq_high_value(float(val)); 
		}(tag.first_value))
			:
			([PF](double val) {
			PF->EQ_high_sw(true); 
			PF->set_eq_high_value(float(val));
		}(tag.first_value));
		break;
	case dj_what::MID:
		tag.first_value < -60.0 ?
		([PF](double val) {
		PF->EQ_mid_sw(false);
		PF->set_eq_mid_value(float(val));
		}(tag.first_value))
		:
		([PF](double val) {
		PF->EQ_mid_sw(true);
		PF->set_eq_mid_value(float(val));
		}(tag.first_value));
		break;
	case dj_what::LOW:
		tag.first_value < -60.0 ?
		([PF](double val) {
		PF->EQ_low_sw(false);
		PF->set_eq_low_value(float(val));
		}(tag.first_value))
		:
		([PF](double val) {
		PF->EQ_low_sw(true);
		PF->set_eq_low_value(float(val));
		}(tag.first_value));
		break;
	default:
		break;
	}
}

void
sound_module::distortion_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	
	tag.first_value < 0 ?
		([PF](double val) {
		PF->_distortion_sw(false);
		PF->set_distortion_gain(float(val));
		}(tag.first_value))
		:
		([PF](double val) {
		PF->_distortion_sw(true);
		PF->set_distortion_gain(float(val));
		}(tag.first_value));
}

void
sound_module::filter_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	switch (tag.what)
	{
	case dj_what::HIGH:
		tag.first_value < 0 ?
			([PF](double val) {
			PF->filter_high_sw(false);
			}(tag.first_value))
			:
			([PF](double val) {
			PF->filter_high_sw(true);
			PF->set_filter_high_freq_value(int(val));
			}(tag.first_value));
			break;
	case dj_what::LOW:
		tag.first_value < 0 ?
			([PF](double val) {
			PF->filter_low_sw(false);
			}(tag.first_value))
			:
			([PF](double val){
			PF->filter_low_sw(true);
			PF->set_filter_low_freq_value(int(val));
			}(tag.first_value));
			break;
	default:
		break;
	}
}
void
sound_module::control_tog(const DDTG& tag) {
	ALBUM* PA;//for self or other
	tag.from != tag.target ? PA= processor->acc_album(tag.target): PA = palbum;
	switch (tag.what)
	{
	case dj_what::CUE:
		PA->get_cursor()->set_point(
			processor->raw_to_processed(
				processor->pBCE->calc_in_real_time(tag.first_value, tag.target)
			)
		);
		break;
	case dj_what::PLAY:
		PA->PLAY();
		break;
	case dj_what::PAUSE:
		PA->STOP();
		break;
	default:
		break;
	}
	
}
void
sound_module::vol_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	switch (tag.what)
	{
	case dj_what::TRIM:
		PF->set_trim(float(tag.first_value));
		break;
	case dj_what::FADER:
		PF->set_fader(float(tag.first_value));
		break;
	default:
		break;
	}
}
void
sound_module::load_tog(const DDTG& tag) {
	processor->load_album(tag.first_str, tag.target);
}

void
sound_module::unload_tog(const DDTG& tag) {
	processor->unload_album(tag.target);
}

void
sound_module::dj_comp_tog(const DDTG& tag) {
	processor->unload_album(tag.from);
	
}

void
sound_module::sola_tog(const DDTG& tag) {
	ALBUM* AP;//for self or other
	tag.from != tag.target ? AP = processor->acc_album(tag.target) : AP = palbum;
	switch (tag.what)
	{
	case dj_what::MASTER:
		AP->get_cursor()->set_speed(1.0, (100.0 + tag.first_value) / 100.0);
		break;
	case dj_what::RAW:
		AP->get_cursor()->set_speed((100.0 + tag.first_value) / 100.0, 1.0);
		break;
	default:
		break;
	}
}

void
sound_module::beat_match_tog(const DDTG& tag) {//WARP MATCHING
	ALBUM* MA;//Master album
	CURSOR* MA_C;//Master cursor
	MA = processor->acc_album(tag.target);
	MA_C = MA->get_cursor();
	ALBUM* ME;
	CURSOR* ME_C;
	ME = palbum;
	ME_C = ME->get_cursor();
	ma_uint64 master_cursor=0;
	ma_uint64 my_cursor = 0;
	
	double my_speed = ME_C->get_IOSR();
	double master_speed = MA_C->get_IOSR();
	ME_C->get_now_frame(my_cursor);
	MA_C->get_now_frame(master_cursor);
	MA->got_frames() ? master_cursor -= ma_uint64(ceil(double(processor->get_audio_buffer_size()) / master_speed)) : true;
	double temp_DDD = processor->pBCE->calc_in_real_time(tag.first_value, tag.target);
	ma_uint64 temp = processor->raw_to_processed(processor->pBCE->calc_in_real_time(tag.first_value, tag.target));
	double temp_calced = (master_speed * (processor->raw_to_processed(processor->pBCE->calc_in_real_time(tag.first_value, tag.target)) - double(master_cursor))) / my_speed;
	ma_uint64 debuggger = ma_uint64(temp_calced);
	int match_time = int(my_cursor) - int(tag.frame_in) + int((
		master_speed * (
			processor->raw_to_processed(processor->pBCE->calc_in_real_time(tag.first_value, tag.target)
			) - double(master_cursor))) / my_speed);
	my_cursor -= match_time;
	ME_C->set_point(my_cursor);
}

void
sound_module::echo_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ? ([PF](float dur, float feedback) {
			PF->_echo_sw(false);
			
		}(float(tag.first_value), float(tag.second_value)))
		:
		([PF](float dur, float feedback, float power) {
			PF->_echo_sw(true);
			PF->set_echo_dur_value(dur);
			PF->set_echo_feedback_value(feedback);
			PF->set_echo_power(power);
		}(float(tag.first_value), float(tag.second_value), float(tag.third_value)));
}

void
sound_module::LFS_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bps) {
			PF->low_siren_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bps,int min_freq, float power) {
			PF->low_siren_sw(true);
			PF->set_l_f_s_gain_min_freq(min_freq);
			PF->set_l_f_s_bps_value(bps);
			PF->set_l_f_s_power(power);
		}(float(tag.first_value),int(tag.second_value),float(tag.third_value)));
}

void
sound_module::flanger_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bps) {
			PF->_flanger_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bps,float gain, float power) {
			PF->_flanger_sw(true);
			PF->set_flanger_bps_value(bps);
			PF->set_flanger_gain_value(gain);
			PF->set_flanger_power(power);
		}(float(tag.first_value),float(tag.second_value),float(tag.third_value)));
}

void
sound_module::phaser_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bps) {
			PF->_phaser_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bps, float gain, float power) {
			PF->_phaser_sw(true);
			PF->set_phaser_bps_value(bps);
			PF->set_phaser_gain_value(gain);
			PF->set_phaser_power(power);
		}(float(tag.first_value), float(tag.second_value), float(tag.third_value)));
}

void
sound_module::trance_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bps) {
			PF->_trance_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bps,float gain, float power) {
			PF->_trance_sw(true);
			PF->set_trance_gain(gain);
			PF->set_trance_bps(bps);
			PF->set_trance_power(power);
		}(float(tag.first_value),float(tag.second_value), float(tag.third_value)));
}


void
sound_module::panner_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bps) {
			PF->_panner_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bps,float gain, float power) {
			PF->_panner_sw(true);
			PF->set_panner_gain(gain);
			PF->set_panner_bps(bps);
			PF->set_panner_power(power);
		}(float(tag.first_value),float(tag.second_value), float(tag.third_value)));
}

void
sound_module::battle_tog(const DDTG& tag) {
	CURSOR *CR = processor->acc_album(tag.target)->get_cursor();
	double SCR_entry_pos;
	ma_uint64 ent_pos;
	ma_uint64 out_pos;
	switch (tag.what)
	{
	case dj_what::SCRATCH:
		SCR_entry_pos = processor->pBCE->calc_in_real_time(tag.first_value, tag.target);
		ent_pos = processor->raw_to_processed(SCR_entry_pos);
		CR->set_point(ent_pos);
		out_pos = processor->raw_to_processed(processor->pBCE->calc_in_real_time(tag.third_value, tag.target));
		CR->temp_mv(true, tag.second_value, 1.0, out_pos - ent_pos);
		break;
	case dj_what::BSCRATCH:
		SCR_entry_pos = processor->pBCE->calc_in_real_time(tag.first_value, tag.target);
		ent_pos = processor->raw_to_processed(SCR_entry_pos);
		CR->set_point(ent_pos);
		out_pos = processor->raw_to_processed(processor->pBCE->calc_in_real_time(tag.third_value, tag.target));
		CR->temp_mv(false, tag.second_value, 1.0, ent_pos-out_pos);
		break;
	default:
		break;
	}
}

void
sound_module::roll_tog(const DDTG& tag) {
	Faust_engine* PF = processor->acc_faust(tag.target);//for self or other
	tag.first_value < 0 ?
		([PF](float bpm) {
			PF->_roll_sw(false);
		}(float(tag.first_value)))
		:
		([PF](float bpm, float power) {
			PF->_roll_sw(true);
			PF->set_roller_bpm(bpm);
			PF->set_roller_power(power);
		}(float(tag.first_value), float(tag.second_value)));
}




//--------------------------------------------------------------------------------//





void
sound_module::interpolate(const DDTG& tag, const ma_uint64& start_frame_, const ma_uint64& end_frame_) {
	inter_body body;
	body.start_frame = start_frame_;
	body.end_frame = end_frame_;
	body.album_pointer = palbum;
	body.for_who_album_p = processor->acc_album(tag.target);
	switch (tag.type)
	{
	case dj_type::EQ:
		eq_inter(tag, body);
		break;
	case dj_type::FILTER:
		filter_inter(tag, body);
		break;
	case dj_type::DISTORTION:
		distortion_inter(tag, body);
		break;
	case dj_type::VOL:
		vol_inter(tag, body);
		break;
	case dj_type::SOLA:
		sola_inter(tag, body);
		break;
	case dj_type::BATTLE_DJ:
		battle_inter(tag, body);
		break;
	default:
		break;
	}
}


void
sound_module::eq_inter(const DDTG& tag,inter_body body) {
	body.start_value = tag.first_value;
	body.end_value = tag.second_value;
	
	Faust_engine* PF = body.for_who_album_p->get_faust();
	ALBUM* AP = body.album_pointer;
	body.death_method = []() {
		;
	};
	switch (tag.what)
	{
	case dj_what::HIGH:
		body.sw_method =[PF](bool turn_on) {
			PF->EQ_high_sw(turn_on);

		};
		body.setter_method = [PF](float val) {
			PF->set_eq_high_value(val);
		};
		AP->inter_queue.push_back(body);
		break;
	case dj_what::MID:
		body.sw_method = [PF](bool turn_on) {
			PF->EQ_mid_sw(turn_on);

		};
		body.setter_method = [PF](float val) {
			PF->set_eq_mid_value(val);
		};
		AP->inter_queue.push_back(body);
		break;
	case dj_what::LOW:
		body.sw_method = [PF](bool turn_on) {
			PF->EQ_low_sw(turn_on);

		};
		body.setter_method = [PF](float val) {
			PF->set_eq_low_value(val);
		};
		AP->inter_queue.push_back(body);
		break;
	default:
		break;
	}
	
}

void
sound_module::distortion_inter(const DDTG& tag,inter_body body) {
	body.start_value = tag.first_value;
	body.end_value = tag.second_value;
	Faust_engine* PF = body.for_who_album_p->get_faust();
	body.sw_method = [PF](bool turn_on) {
		PF->_distortion_sw(turn_on);
	};
	body.setter_method = [PF](float val) {
		PF->set_distortion_gain(val);
	};
	body.death_method = []() {
		;
	};
	ALBUM* AP = body.album_pointer;
	AP->inter_queue.push_back(body);
}



void
sound_module::filter_inter(const DDTG& tag,inter_body body) {
	Faust_engine* PF = body.for_who_album_p->get_faust();//for self or other
	
	body.start_value = tag.first_value;
	body.end_value = tag.second_value;
	body.death_method = []() {
		;
	};
	switch (tag.what)
	{
	case dj_what::HIGH:
		body.sw_method = [PF](bool turn_on) {
			PF->filter_high_sw(turn_on);
		};
		body.setter_method = [PF](float val) {
			PF->set_filter_high_freq_value(int(val));
		};
		break;
	case dj_what::LOW:
		body.sw_method = [PF](bool turn_on) {
			PF->filter_low_sw(turn_on);
		};
		body.setter_method = [PF](float val) {
			PF->set_filter_low_freq_value(int(val));
		};
		break;
	default:
		break;
	}
	ALBUM* AP = body.album_pointer;
	AP->inter_queue.push_back(body);

}


void
sound_module::vol_inter(const DDTG& tag,inter_body body) {
	Faust_engine* PF = body.for_who_album_p->get_faust();//for self or other
	body.start_value = tag.first_value;
	body.end_value = tag.second_value;
	body.sw_method = [](bool nothing) {
		
	};
	body.death_method = []() {
		;
	};
	switch (tag.what)
	{
	case dj_what::TRIM:
		body.setter_method = [PF](float val) {
			PF->set_trim(val);
		};
		break;
	case dj_what::FADER:
		body.setter_method = [PF](float val) {
			PF->set_fader(val);
		};
		break;
	default:
		break;
	}
	ALBUM* AP = body.album_pointer;
	AP->inter_queue.push_back(body);
}


void
sound_module::sola_inter(const DDTG& tag,inter_body body) {
	CURSOR *CR = body.for_who_album_p->get_cursor();//for self or other
	body.start_value = 1.0 + tag.first_value / 100.0;
	body.end_value = 1.0 + tag.second_value / 100.0;
	body.sw_method = [](bool nothing) {
		
	};
	body.death_method = []() {
		;
	};
	switch (tag.what)
	{
	case dj_what::MASTER:
		body.setter_method = [CR](double val) {
			CR->set_speed(1.0, val);
		};
		break;
	case dj_what::RAW:
		body.setter_method = [CR](double val) {
			CR->set_speed(val, 1.0);
		};
		break;
	default:
		break;
	}
	body.for_who_album_p->inter_queue.push_back(body);
}

void
sound_module::battle_inter(const DDTG& tag,inter_body body) {
	CURSOR* CR = body.for_who_album_p->get_cursor();
	switch (tag.what)
	{
	case dj_what::SPIN:
		CR->temp_mv(true, tag.first_value, 1.0, body.end_frame - body.start_frame);
		break;
	case dj_what::BSPIN:
		CR->temp_mv(false, tag.first_value, 1.0, body.start_frame- body.end_frame);
		break;
	case dj_what::REV:
		CR->set_dir(false);
		body.setter_method = [](double nothing) {
			
		};
		body.sw_method = [](bool nothing) {
			
		};
		body.death_method = [CR]() {
			CR->set_dir(true);
		};
		body.for_who_album_p->inter_queue.push_back(body);
		break;
	default:
		break;
	}
}