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
	pproc->work_counter = 0;
	pproc->work_mutex.lock();
	pproc->LOCK_SAFE = 0;
	pproc->work_call.notify_all();
	pproc->work_mutex.unlock();
	while (true) {
		if (pproc->work_counter >= pproc->MAX_DECK_USE) {
			break;
		}
	}
	pproc->end_mutex.lock();
	pproc->end_sync.notify_all();
	pproc->end_mutex.unlock();
}
