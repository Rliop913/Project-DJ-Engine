#pragma once
#include "MiniAudioWrapper.hpp"
#include "Processor.h"

void
idle_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	Processor* pproc = (Processor*)pDevice->pUserData;
	pproc->add_processed_time(frameCount);
	if (pproc->deck_size() > 0) {
		pproc->get_deck_p()->second->idle_process(frameCount, (float*)pOutput);
	}
}


void
dj_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	Processor* pproc = (Processor*)pDevice->pUserData;
	pproc->add_processed_time(frameCount);
	pproc->public_bufferout = pOutput;
	pproc->startline->arrive_and_wait();
	pproc->work_end_buzzer->arrive_and_wait();
}
