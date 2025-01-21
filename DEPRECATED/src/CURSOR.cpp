#include "CURSOR.h"
#include "Processor.h"
#include "ALBUM.h"
CURSOR::CURSOR(const std::string& song_path,Processor& pproc,ALBUM* aalbum)
{
	album = aalbum;
	MAW::init_decoder(song_path, dec, pproc);
	ST = new SoundTouch();
	ST->setChannels(2);
	ST->setSampleRate(48000);
	ST->setSetting(0, 1);
	ST->setSetting(3, 1);
	ST->setSetting(2, 0);
	ST->setTempo(1.0);
}


CURSOR::~CURSOR()
{
	delete ST;
	MAW::uninit_decoder(dec, *(album->pproc));
}


void
CURSOR::mvcur(float* wbuf,const ma_uint32& frameCount)
{
	temp_mv_enabled ?//IF(temp_mv_enabled)
		(//true
			remained_frames <= 0 ?//IF(remained_frames<=0)
			back_to_origin()
			:
			(
				go_front ?//IF(go_front)
				MAW::touch_sound(*ST, frameCount, wbuf, dec, album->sola_buffer, ffixer)
				:
				MAW::touch_sound_back(*ST, frameCount, wbuf, dec, album->sola_buffer, ffixer)
				)
			)
		:
		(//false
			go_front ?//IF(go_front)
			MAW::touch_sound(*ST, frameCount, wbuf, dec, album->sola_buffer, ffixer)
			:
			MAW::touch_sound_back(*ST, frameCount, wbuf, dec, album->sola_buffer, ffixer)
		);
	remained_frames <= 0 ? remained_frames = 0 : remained_frames -= frameCount;
}

void
CURSOR::back_to_origin()
{
	temp_mv_enabled = false;
	remained_frames = 0;
	go_front = origin_mv_dir;
	ST->setPitch(origin_pitch);
	ST->setTempo(origin_tempo);
}

void
CURSOR::set_point(const ma_uint64& position)
{
	MAW::decoder_seek_frame(dec, position);
}

double
CURSOR::get_IOSR()
{
	return ST->getInputOutputSampleRatio();
}

void
CURSOR::set_dir(const bool& is_front)
{
	go_front = is_front;
	return;
}

void
CURSOR::set_speed(const double& pitch, const double& tempo)
{
	double pitch_limit = pitch > 2.0 ? 2.0 : pitch;
	double tempo_limit = tempo > 2.0 ? 2.0 : tempo;
	ST->setTempo(tempo_limit);
	ST->setPitch(pitch_limit);
	origin_pitch = pitch_limit;
	origin_tempo = tempo_limit;
	return;
}

void
CURSOR::get_now_frame(ma_uint64& now)
{
	MAW::decoder_get_cursor(dec, now);
	return;
}


void
CURSOR::temp_mv(const bool& direction, const double& pitch, const double& tempo, const ma_uint64& play_frame)
{
	origin_mv_dir = go_front;
	go_front = direction;
	ST->setPitch(pitch);
	ST->setTempo(tempo);
	remained_frames = play_frame;
}