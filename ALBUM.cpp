#include "Engine_Main.h"
#include <iostream>
#include "temp_json_reader.h"










void
ALBUM::idle_process(ma_uint32 frameCount, float* main_buffer) {
	this_data.got_frames = false;
	MAW::silence_memory(process_memory.get(), frameCount);

	cursor->mvcur(process_memory.get(), frameCount);
	this_data.got_frames = true;
	before_faust_caster(process_memory.get(), faust_before1.get(), faust_before2.get(), frameCount);
	//---------------------------FAUST HERE-------------------------------//
	album_engine->compute(frameCount, faust_before1.get(),faust_before2.get(), faust_after1.get(),faust_after2.get());
	//---------------------------FAUST HERE-------------------------------//
	pproc->buf_mutex.lock();
	after_faust_caster(faust_after1.get(), faust_after2.get(), main_buffer, frameCount);
	pproc->buf_mutex.unlock();
}















void
ALBUM::dj_process(ma_uint32 frameCount, float* main_buffer) {
	this_data.got_frames = false;
	ma_silence_pcm_frames(process_memory.get(), frameCount, ma_format_f32, 2);//clean garbage datas
	ma_uint64 frame_now = 0;
	cursor->get_now_frame(frame_now);
	//ma_decoder_get_cursor_in_pcm_frames(&music, &frame_now);
	//pengine->pcompiler->calc_now_bpm(frame_now, this_data.ID, &this_data.bpm);
	reserve_read(frame_now);
	interpolate_function(frame_now);
	if (this_data.playback_ordered) {
		cursor->mvcur(process_memory.get(), frameCount);
		this_data.got_frames = true;
	}
	before_faust_caster(process_memory.get(), faust_before1.get(), faust_before2.get(), frameCount);
	//---------------------------FAUST HERE-------------------------------//
	album_engine->compute(frameCount, faust_before1.get(), faust_before2.get(), faust_after1.get(),faust_after2.get());
	//---------------------------FAUST HERE-------------------------------//
	pproc->buf_mutex.lock();
	after_faust_caster(faust_after1.get(), faust_after2.get(), main_buffer, frameCount);
	pproc->buf_mutex.unlock();
}
//
//void
//ALBUM::daw_process(ma_uint32 frameCount, float* main_buffer) {
//	/*************************************************************************************************/
//	/******************************ready datas********************************************************/
//	/*************************************************************************************************/
//	this_data.got_frames = false;
//	//is_processing = true;//for safe stop
//	//float* temp_void_p = (float*)malloc(VOID_BUFFER_SIZE(frameCount, music_conf.channels));//album's final data
//	ma_silence_pcm_frames(process_memory, frameCount, ma_format_f32, music_conf.channels);//clean garbage datas
//	
//	ma_uint64 frame_now = 0;
//	ma_decoder_get_cursor_in_pcm_frames(&music, &frame_now);
//	pengine->pcompiler->calc_now_bpm(frame_now, this_data.ID, &this_data.bpm);
//	reserve_read(frame_now);
//
//	double remember_origin_sola = 1.0 / music_flow->getInputOutputSampleRatio();
//	music_flow->setTempoChange(pengine->daw_d.master_sola);
//	met_p->compute_metronome_state(frame_now, this_data, music_flow->getInputOutputSampleRatio());
//	interpolate_function(frame_now);
//	/*************************************************************************************************/
//	/******************************check scratch order************************************************/
//	/*************************************************************************************************/
//	if (is_SCRATCH) {
//		if (scratch_data_holding.remained_cycle == 0) {//END SCRATCH CYCLE
//			is_SCRATCH = false;
//			is_rev = false;
//			music_flow->setRate(1.0);
//			this_data.playback_ordered = false;
//		}
//		else {
//			scratch_data_holding.remained_cycle -= frameCount;
//		}
//	}
//	/*************************************************************************************************/
//	/******************************play or pause, get frames******************************************/
//	/*************************************************************************************************/
//	if (this_data.playback_ordered) {
//		this_data.got_frames = true;
//		if (music_flow->getInputOutputSampleRatio() != 1.0) {//time_stretch
//			ma_uint32 final_frame = ceil(double(frameCount) / music_flow->getInputOutputSampleRatio());
//			//float* before_sola = (float*)malloc(VOID_BUFFER_SIZE(final_frame, music_conf.channels));
//			float* sola_temp = new float[final_frame * music_conf.channels];
//			ma_silence_pcm_frames(sola_temp, final_frame, ma_format_f32, 2);
//			if (is_rev) {//REV
//				ma_uint64 Back_cursor;
//				ma_decoder_get_cursor_in_pcm_frames(&music, &Back_cursor);
//				ma_decoder_seek_to_pcm_frame(&music, Back_cursor - final_frame);
//				ma_decoder_read_pcm_frames(&music, sola_temp, final_frame, NULL);
//				rev(sola_temp, final_frame, music_conf.channels);
//				ma_decoder_seek_to_pcm_frame(&music, Back_cursor - final_frame);
//			}
//			else {//NORMAL
//				ma_decoder_read_pcm_frames(&music, sola_temp, final_frame, NULL);
//			}
//			music_flow->putSamples(sola_temp, final_frame);
//			music_flow->receiveSamples(process_memory, frameCount);
//			delete[] sola_temp;
//		}
//		else {//normal play
//			if (is_rev) {//REV
//				ma_uint64 Back_cursor;
//				ma_decoder_get_cursor_in_pcm_frames(&music, &Back_cursor);
//				ma_decoder_seek_to_pcm_frame(&music, Back_cursor - frameCount);
//				ma_decoder_read_pcm_frames(&music, process_memory, frameCount, NULL);
//				rev(process_memory, frameCount, music_conf.channels);
//				ma_decoder_seek_to_pcm_frame(&music, Back_cursor - frameCount);
//			}
//			else {//NORMAL
//				ma_decoder_read_pcm_frames(&music, process_memory, frameCount, NULL);
//			}
//		}
//	}
//	/*************************************************************************************************/
//	/******************************faust start********************************************************/
//	/*************************************************************************************************/
//	ma_silence_pcm_frames(faust_before[0], frameCount, ma_format_f32, 1);
//	ma_silence_pcm_frames(faust_before[1], frameCount, ma_format_f32, 1);
//	ma_silence_pcm_frames(faust_after[0], frameCount, ma_format_f32, 1);
//	ma_silence_pcm_frames(faust_after[1], frameCount, ma_format_f32, 1);
//	before_faust_caster(process_memory, faust_before[0], faust_before[1], frameCount, music_conf.channels);
//	//---------------------------FAUST HERE-------------------------------//
//	album_engine->compute(frameCount, faust_before, faust_after);
//	//---------------------------FAUST HERE-------------------------------//
//	if (pengine->daw_d.is_soloing){
//		if (pengine->daw_d.target_ID == this_data.ID) {
//			pengine->buf_mutex.lock();
//			after_faust_caster(faust_after[0], faust_after[1], main_buffer, frameCount, music_conf.channels);
//			met_p->inject_metronome_in_buffer(main_buffer);
//			pengine->buf_mutex.unlock();
//		}
//		else {//soloing and not target
//			//do nothing
//		}
//	}
//	else {
//	pengine->buf_mutex.lock();
//	after_faust_caster(faust_after[0], faust_after[1], main_buffer, frameCount, music_conf.channels);
//	met_p->inject_metronome_in_buffer(main_buffer);
//	pengine->buf_mutex.unlock();
//	}
//	music_flow->setTempo(remember_origin_sola);
//}



void
ALBUM::before_faust_caster(void* input, void* out_left, void* out_right,const ma_uint32& frameCount) {
	int whole_size = VOID_BUFFER_SIZE(frameCount, 2)/8;
	for (int i = 0; i < whole_size; i++) {//odd is right
		((float*)out_left)[i] = (((float*)input)[i * 2]);
		((float*)out_right)[i] = ((float*)input)[i * 2 + 1];
	}
}

void
ALBUM::after_faust_caster(void* in_left, void* in_right, void* output,const ma_uint32& frameCount) {
	int whole_size = VOID_BUFFER_SIZE(frameCount, 2) / 8;
	for (int i = 0; i < whole_size; i++) {//odd is right
		((float*)output)[i * 2] += ((float*)in_left)[i];
		((float*)output)[i * 2 + 1] += ((float*)in_right)[i];
	}//add to buffer
}


void
ALBUM::album_init(const std::string& song_path) {
	cursor = new CURSOR(song_path, *pproc);
	this_data.data_path = song_path;
	album_engine = new Faust_engine();
	album_engine->init(48000);
	module = new sound_module(pproc,this,album_engine);
}




ALBUM::~ALBUM() {
	delete cursor;
	delete album_engine;
	delete module;
}

ALBUM::ALBUM(const std::string& song_path, const int& channel, const int& albumID, Processor* p, const std::string& meta_data_path) {
	pproc = p;
	album_init(song_path);
	TJR_RETURN file_inside = TJR::temp_json_reader(meta_data_path);
	this->this_data.ID = albumID;
	this->this_data.bpm = std::stod(file_inside.at(0)["bpm"]);
	this->this_data.start_time = std::stod(file_inside.at(0)["start_time"]);
}

void
ALBUM::reserve_read(const ma_uint64& frame_now) {
	std::unordered_map<int,std::vector<engine_order>>* RS = &pproc->pBCE->reservation_storage;
go_to_function_for_simple_check://check until vector until reservation is available
	if ((*RS)[this_data.ID].size() == 0) {
		return;
	}
	if (pproc->raw_to_processed((*RS)[this_data.ID].at(0).frame_in) < frame_now) {
		//new code
		tagables tags;
		tags.type = (*RS)[this_data.ID].at(0).tag["type"];
		tags.what_ = (*RS)[this_data.ID].at(0).tag["what"];
		tags.where_ =std::stoi((*RS)[this_data.ID].at(0).tag["where"]);
		tags.for_who = std::stoi((*RS)[this_data.ID].at(0).tag["for_who"]);
		tags.first = std::stod((*RS)[this_data.ID].at(0).tag["first"]);
		tags.second = std::stod((*RS)[this_data.ID].at(0).tag["second"]);
		tags.str_first= (*RS)[this_data.ID].at(0).tag["str_first"];
		tags.str_second = (*RS)[this_data.ID].at(0).tag["str_second"];
		tags.frame_in = pproc->raw_to_processed((*RS)[this_data.ID].at(0).frame_in);
		tags.frame_out = pproc->raw_to_processed((*RS)[this_data.ID].at(0).frame_out);
		if (tags.frame_out == 0) {//toggle mode
			module->toggle(tags);
		}
		else {//interpolate mode
			module->interpolate(tags,tags.frame_in, tags.frame_out);
		}
		((*RS)[this_data.ID]).erase((*RS)[this_data.ID].begin());

		goto go_to_function_for_simple_check;//back to if
	}

}

void
ALBUM::interpolate_function(const ma_uint64& now_frame) {
		for (auto i = 0; i < inter_queue.size(); i++) {
			inter_body body = inter_queue[i];
			if (now_frame > body.end_frame) {
				body.setter_method(body.end_value);
				body.death_method();
				inter_queue.erase(inter_queue.begin() + i);
				i--;
				continue;
			}
			body.sw_method(true);
			
			body.setter_method((float)(((1 - (double)(now_frame - body.start_frame) / (double)(body.end_frame - body.start_frame)) * body.start_value) + (double)(now_frame - body.start_frame) / (double)(body.end_frame - body.start_frame) * body.end_value));
		}

		
}

//
//
//ALBUM::METRONOME::METRONOME(Engine_Main* pe){
//	pengine = pe;
//	ma_decoder_config de_co;
//	ma_decoder met_dec;
//	de_co = ma_decoder_config_init(ma_format_f32, 2, 44100);
//	pe->loader_function(pe->metronome_sound, &met_dec, &de_co);
//	//ma_decoder_init_file(pe->metronome_sound.c_str(), &de_co, &met_dec);
//	
//	ma_decoder_get_length_in_pcm_frames(&met_dec, &MET.len);
//	MET.met_buffer = (float*)malloc(VOID_BUFFER_SIZE(MET.len, 2));
//	ma_decoder_read_pcm_frames(&met_dec, MET.met_buffer, MET.len, NULL);
//	ma_decoder_uninit(&met_dec);
//}
//
//
//ALBUM::METRONOME::~METRONOME() {
//	free(MET.met_buffer);
//}
//
//void
//ALBUM::METRONOME::compute_metronome_state(ma_uint64 now_frame,song_data SD, double flow_ratio) {
//	if (!pengine->met_activate) {//MET.state != SLEEP_m) {
//		return;
//	}
//	else {
//		MET.temp_buffer = (float*)malloc(VOID_BUFFER_SIZE(441, 2));
//		ma_uint64 frame_per_beat = (ma_uint64)ceil(44100.0 * CONST_BPM / (SD.bpm));
//		ma_uint64 pure_frame = now_frame - (ma_uint64)ceil(SD.start_time * 44100.0);
//		int main_itr = 0;
//		pure_frame > 0 ? main_itr = pure_frame % frame_per_beat : main_itr = -1;
//		for (int i = 0; i < 882; i++) {
//			if (main_itr >= MET.len * 2 || main_itr == -1) {
//				((float*)MET.temp_buffer)[i] = 0.0f;
//				((float*)MET.temp_buffer)[i + 1] = 0.0f;
//			}
//			else {
//				((float*)MET.temp_buffer)[i] = MET.met_buffer[main_itr] * pengine->met_vol;
//				((float*)MET.temp_buffer)[i + 1] = MET.met_buffer[main_itr + 1] * pengine->met_vol;
//			}
//			main_itr += 2;
//		}
//	}
//	
//}
//
//
//void
//ALBUM::METRONOME::inject_metronome_in_buffer(void* main_buffer) {
//	if (!MET.data_ready) {
//		return;
//	}
//	else {
//		for (int i = 0; i < 882; i += 2) {
//			((float*)main_buffer)[i] += ((float*)MET.temp_buffer)[i];
//			((float*)main_buffer)[i+1] += ((float*)MET.temp_buffer)[i+1];
//		}
//		free(MET.temp_buffer);
//	}
//}


mix_data_set
ALBUM::get_mixing_data() {
	mix_data_set set;
	set.path = this_data.data_path;
	set.trim = album_engine->trim;
	set.fader = album_engine->fader;
	set.eq_mid_v = album_engine->eq_mid_v;
	set.eq_low_v = album_engine->eq_low_v;
	set.eq_hi_v = album_engine->eq_hi_v;
	album_engine->lo_fi_sw == 0 ? set.lpf_sw = true : set.lpf_sw = false;
	album_engine->hi_fi_sw == 0 ? set.hpf_sw = true : set.hpf_sw = false;
	album_engine->echo_sw == 0 ? set.echo_sw_ = true : set.echo_sw_= false;
	album_engine->l_f_s_sw== 0 ? set.l_f_s_sw_= true : set.l_f_s_sw_= false;
	album_engine->phaser_sw == 0 ? set.phaser_sw_= true : set.phaser_sw_= false;
	album_engine->flanger_sw == 0 ? set.flanger_sw_= true : set.flanger_sw_= false;
	album_engine->trance_sw == 0 ? set.trance_sw_= true : set.trance_sw_= false;
	album_engine->panner_sw == 0 ? set.panner_sw_= true : set.panner_sw_= false;
	album_engine->distortion_sw== 0 ? set.distortion_sw_= true : set.distortion_sw_= false;
	//set.scratch_sw_ = is_SCRATCH;
	//set.rev_sw_ = is_rev;
	set.now_bpm = this_data.bpm / cursor->get_IOSR();
	return set;
}

void
ALBUM::PLAY()
{
	this_data.playback_ordered = true;
}

void
ALBUM::STOP()
{
	this_data.playback_ordered = false;
}