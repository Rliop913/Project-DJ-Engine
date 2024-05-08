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
sound_module::toggle(DDTG& data) {
	switch (SI(data.tags["type"])) {
	case dj_type::EQ:
		eq_tog(data);
		break;
	case dj_type::DISTORTION:
		distortion_tog(data);
		break;
	case dj_type::FILTER:
		filter_tog(data);
		break;
	case dj_type::CONTROL:
		control_tog(data);
		break;
	case dj_type::VOL:
		vol_tog(data);
		break;
	case dj_type::LOAD:
		load_tog(data);
		break;
	case dj_type::UNLOAD:
		unload_tog(data);
		break;
	case dj_type::DJ_COMP:
		dj_comp_tog(data);
		break;
	case dj_type::BEAT_MATCH:
		beat_match_tog(data);
		break;
	case dj_type::SOLA:
		sola_tog(data);
		break;
	case dj_type::ECHO:
		echo_tog(data);
		break;
	case dj_type::LFS:
		LFS_tog(data);
		break;
	case dj_type::FLANGER:
		flanger_tog(data);
		break;
	case dj_type::PHASER:
		phaser_tog(data);
		break;
	case dj_type::TRANCE:
		trance_tog(data);
		break;
	case dj_type::PANNER:
		panner_tog(data);
		break;
	case dj_type::BATTLE_DJ:
		battle_tog(data);
		break;
	case dj_type::ROLL:
		roll_tog(data);
		break;
	case dj_type::ALIVE:
		break;
	default:
		break;
	}
}

void
sound_module::eq_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));

	//Faust_engine* PF= processor->acc_faust(SI(data.tags["target"]));//for self or other
	switch (SI(data.tags["what"]))
	{
	case dj_what::HIGH:
		SF(data.tags["first"]) < -60.0 ?
			([PF](double val) {
			PF->EQ_high_sw(false); 
			PF->set_eq_high_value(float(val)); 
		}(SF(data.tags["first"])))
			:
			([PF](double val) {
			PF->EQ_high_sw(true); 
			PF->set_eq_high_value(float(val));
		}(SF(data.tags["first"])));
		break;
	case dj_what::MID:
		SF(data.tags["first"]) < -60.0 ?
		([PF](double val) {
		PF->EQ_mid_sw(false);
		PF->set_eq_mid_value(float(val));
		}(SF(data.tags["first"])))
		:
		([PF](double val) {
		PF->EQ_mid_sw(true);
		PF->set_eq_mid_value(float(val));
		}(SF(data.tags["first"])));
		break;
	case dj_what::LOW:
		SF(data.tags["first"]) < -60.0 ?
		([PF](double val) {
		PF->EQ_low_sw(false);
		PF->set_eq_low_value(float(val));
		}(SF(data.tags["first"])))
		:
		([PF](double val) {
		PF->EQ_low_sw(true);
		PF->set_eq_low_value(float(val));
		}(SF(data.tags["first"])));
		break;
	default:
		break;
	}
}

void
sound_module::distortion_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	
	SF(data.tags["first"]) < 0 ?
		([PF](double val) {
		PF->_distortion_sw(false);
		PF->set_distortion_gain(float(val));
		}(SF(data.tags["first"])))
		:
		([PF](double val) {
		PF->_distortion_sw(true);
		PF->set_distortion_gain(float(val));
		}(SF(data.tags["first"])));
}

void
sound_module::filter_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	switch (SI(data.tags["what"]))
	{
	case dj_what::HIGH:
		SF(data.tags["first"]) < 0 ?
			([PF](double val) {
			PF->filter_high_sw(false);
			}(SF(data.tags["first"])))
			:
			([PF](double val) {
			PF->filter_high_sw(true);
			PF->set_filter_high_freq_value(int(val));
			}(SF(data.tags["first"])));
			break;
	case dj_what::LOW:
		SF(data.tags["first"]) < 0 ?
			([PF](double val) {
			PF->filter_low_sw(false);
			}(SF(data.tags["first"])))
			:
			([PF](double val){
			PF->filter_low_sw(true);
			PF->set_filter_low_freq_value(int(val));
			}(SF(data.tags["first"])));
			break;
	default:
		break;
	}
}
void
sound_module::control_tog(DDTG& data) {
	ALBUM* PA;//for self or other
	SI(data.tags["from"]) != SI(data.tags["target"]) ? PA= processor->acc_album(SI(data.tags["target"])): PA = palbum;
	switch (SI(data.tags["what"]))
	{
	case dj_what::CUE:
		PA->get_cursor()->set_point(
			processor->raw_to_processed(
				processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]))
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
sound_module::vol_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	switch (SI(data.tags["what"]))
	{
	case dj_what::TRIM:
		PF->set_trim(float(SF(data.tags["first"])));
		break;
	case dj_what::FADER:
		PF->set_fader(float(SF(data.tags["first"])));
		break;
	default:
		break;
	}
}
void
sound_module::load_tog(DDTG& data) {
	processor->load_album(data.tags["first_str"], SI(data.tags["target"]));
}

void
sound_module::unload_tog(DDTG& data) {
	processor->unload_album(SI(data.tags["target"]));
}

void
sound_module::dj_comp_tog(DDTG& data) {
	processor->unload_album(SI(data.tags["from"]));
	
}

void
sound_module::sola_tog(DDTG& data) {
	ALBUM* AP;//for self or other
	SI(data.tags["from"]) != SI(data.tags["target"]) ? AP = processor->acc_album(SI(data.tags["target"])) : AP = palbum;
	switch (SI(data.tags["what"]))
	{
	case dj_what::MASTER:
		AP->get_cursor()->set_speed(1.0, (100.0 + SF(data.tags["first"])) / 100.0);
		break;
	case dj_what::RAW:
		AP->get_cursor()->set_speed((100.0 + SF(data.tags["first"])) / 100.0, 1.0);
		break;
	default:
		break;
	}
}

void
sound_module::beat_match_tog(DDTG& data) {//WARP MATCHING
	ALBUM* MA;//Master album
	CURSOR* MA_C;//Master cursor
	MA = processor->acc_album(SI(data.tags["target"]));
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
	double temp_DDD = processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]));
	ma_uint64 temp = processor->raw_to_processed(processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"])));
	double temp_calced = (master_speed * (processor->raw_to_processed(processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]))) - double(master_cursor))) / my_speed;
	ma_uint64 debuggger = ma_uint64(temp_calced);
	int match_time = int(my_cursor) - int(data.frame_in) + int((
		master_speed * (
			processor->raw_to_processed(processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]))
			) - double(master_cursor))) / my_speed);
	my_cursor -= match_time;
	ME_C->set_point(my_cursor);
}

void
sound_module::echo_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ? ([PF](float dur, float feedback) {
			PF->_echo_sw(false);
			
		}(float(SF(data.tags["first"])), SF(data.tags["second"])))
		:
		([PF](float dur, float feedback, float power) {
			PF->_echo_sw(true);
			PF->set_echo_dur_value(dur);
			PF->set_echo_feedback_value(feedback);
			PF->set_echo_power(power);
		}(float(SF(data.tags["first"])), SF(data.tags["second"]), SF(data.tags["third"])));
}

void
sound_module::LFS_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bps) {
			PF->low_siren_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bps,int min_freq, float power) {
			PF->low_siren_sw(true);
			PF->set_l_f_s_gain_min_freq(min_freq);
			PF->set_l_f_s_bps_value(bps);
			PF->set_l_f_s_power(power);
		}(float(SF(data.tags["first"])),SI(data.tags["second"]),SF(data.tags["third"])));
}

void
sound_module::flanger_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bps) {
			PF->_flanger_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bps,float gain, float power) {
			PF->_flanger_sw(true);
			PF->set_flanger_bps_value(bps);
			PF->set_flanger_gain_value(gain);
			PF->set_flanger_power(power);
		}(float(SF(data.tags["first"])),SF(data.tags["second"]),SF(data.tags["third"])));
}

void
sound_module::phaser_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bps) {
			PF->_phaser_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bps, float gain, float power) {
			PF->_phaser_sw(true);
			PF->set_phaser_bps_value(bps);
			PF->set_phaser_gain_value(gain);
			PF->set_phaser_power(power);
		}(float(SF(data.tags["first"])), SF(data.tags["second"]), SF(data.tags["third"])));
}

void
sound_module::trance_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bps) {
			PF->_trance_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bps,float gain, float power) {
			PF->_trance_sw(true);
			PF->set_trance_gain(gain);
			PF->set_trance_bps(bps);
			PF->set_trance_power(power);
		}(float(SF(data.tags["first"])),SF(data.tags["second"]), SF(data.tags["third"])));
}


void
sound_module::panner_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bps) {
			PF->_panner_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bps,float gain, float power) {
			PF->_panner_sw(true);
			PF->set_panner_gain(gain);
			PF->set_panner_bps(bps);
			PF->set_panner_power(power);
		}(float(SF(data.tags["first"])),SF(data.tags["second"]), SF(data.tags["third"])));
}

void
sound_module::battle_tog(DDTG& data) {
	CURSOR *CR = processor->acc_album(SI(data.tags["target"]))->get_cursor();
	double SCR_entry_pos;
	ma_uint64 ent_pos;
	ma_uint64 out_pos;
	switch (SI(data.tags["what"]))
	{
	case dj_what::SCRATCH:
		SCR_entry_pos = processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]));
		ent_pos = processor->raw_to_processed(SCR_entry_pos);
		CR->set_point(ent_pos);
		out_pos = processor->raw_to_processed(processor->pBCE->calc_in_real_time(SD(data.tags["third"]), SI(data.tags["target"])));
		CR->temp_mv(true, SD(data.tags["second"]), 1.0, out_pos - ent_pos);
		break;
	case dj_what::BSCRATCH:
		SCR_entry_pos = processor->pBCE->calc_in_real_time(SF(data.tags["first"]), SI(data.tags["target"]));
		ent_pos = processor->raw_to_processed(SCR_entry_pos);
		CR->set_point(ent_pos);
		out_pos = processor->raw_to_processed(processor->pBCE->calc_in_real_time(SD(data.tags["third"]), SI(data.tags["target"])));
		CR->temp_mv(false, SD(data.tags["second"]), 1.0, ent_pos-out_pos);
		break;
	default:
		break;
	}
}

void
sound_module::roll_tog(DDTG& data) {
	Faust_engine* PF = processor->acc_faust(SI(data.tags["target"]));//for self or other
	SF(data.tags["first"]) < 0 ?
		([PF](float bpm) {
			PF->_roll_sw(false);
		}(float(SF(data.tags["first"]))))
		:
		([PF](float bpm, float power) {
			PF->_roll_sw(true);
			PF->set_roller_bpm(bpm);
			PF->set_roller_power(power);
		}(float(SF(data.tags["first"])), SF(data.tags["second"])));
}




//--------------------------------------------------------------------------------//





void
sound_module::interpolate(DDTG& data) {
	inter_body body;
	body.start_frame = data.frame_in;
	body.end_frame = data.frame_out;
	body.album_pointer = palbum;
	body.for_who_album_p = processor->acc_album(SI(data.tags["target"]));
	switch (SI(data.tags["type"]))
	{
	case dj_type::EQ:
		eq_inter(data, body);
		break;
	case dj_type::FILTER:
		filter_inter(data, body);
		break;
	case dj_type::DISTORTION:
		distortion_inter(data, body);
		break;
	case dj_type::VOL:
		vol_inter(data, body);
		break;
	case dj_type::SOLA:
		sola_inter(data, body);
		break;
	case dj_type::BATTLE_DJ:
		battle_inter(data, body);
		break;
	default:
		break;
	}
}


void
sound_module::eq_inter(DDTG& data,inter_body body) {
	body.start_value = SF(data.tags["first"]);
	body.end_value = SD(data.tags["second"]);
	
	Faust_engine* PF = body.for_who_album_p->get_faust();
	ALBUM* AP = body.album_pointer;
	body.death_method = []() {
		;
	};
	switch (SI(data.tags["what"]))
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
sound_module::distortion_inter(DDTG& data,inter_body body) {
	body.start_value = SF(data.tags["first"]);
	body.end_value = SD(data.tags["second"]);
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
sound_module::filter_inter(DDTG& data,inter_body body) {
	Faust_engine* PF = body.for_who_album_p->get_faust();//for self or other
	
	body.start_value = SF(data.tags["first"]);
	body.end_value = SD(data.tags["second"]);
	body.death_method = []() {
		;
	};
	switch (SI(data.tags["what"]))
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
sound_module::vol_inter(DDTG& data,inter_body body) {
	Faust_engine* PF = body.for_who_album_p->get_faust();//for self or other
	body.start_value = SF(data.tags["first"]);
	body.end_value = SD(data.tags["second"]);
	body.sw_method = [](bool nothing) {
		
	};
	body.death_method = []() {
		;
	};
	switch (SI(data.tags["what"]))
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
sound_module::sola_inter(DDTG& data,inter_body body) {
	CURSOR *CR = body.for_who_album_p->get_cursor();//for self or other
	body.start_value = 1.0 + SF(data.tags["first"]) / 100.0;
	body.end_value = 1.0 + SD(data.tags["second"]) / 100.0;
	body.sw_method = [](bool nothing) {
		
	};
	body.death_method = []() {
		;
	};
	switch (SI(data.tags["what"]))
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
sound_module::battle_inter(DDTG& data,inter_body body) {
	CURSOR* CR = body.for_who_album_p->get_cursor();
	switch (SI(data.tags["what"]))
	{
	case dj_what::SPIN:
		CR->temp_mv(true, SF(data.tags["first"]), 1.0, body.end_frame - body.start_frame);
		break;
	case dj_what::BSPIN:
		CR->temp_mv(false, SF(data.tags["first"]), 1.0, body.start_frame- body.end_frame);
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