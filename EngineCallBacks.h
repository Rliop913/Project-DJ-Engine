#pragma once
#include "MiniAudioWrapper.h"
#include "Processor.h"

void
idle_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	Processor* pproc = (Processor*)pDevice->pUserData;
	if (pproc->deck_size() > 0) {
		pproc->get_deck_p()->second->idle_process(frameCount, (float*)pOutput);
	}
	/*std::unordered_map<int, ALBUM*>::iterator Dp = pproc->get_deck_p();
	int temp = pproc->deck_size();*/

	//for (int i = 0; i < pproc->deck_size(); ++i) {
	//	auto mdp = Dp;
	//	
	//	pproc->ID_is_in_stopQ(mdp->first) ?
	//		pproc->delete_album(Dp->first)
	//		:
	//		Dp->second->idle_process(frameCount, (float*)pOutput);
	//	//++Dp;
	//}
	//pproc->clear_stopQ();
}


void
dj_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	Processor* pproc = (Processor*)pDevice->pUserData;
	pproc->public_bufferout = pOutput;
	pproc->END_SYNC = false;
	pproc->work_mutex.lock();
	pproc->WORK_CALL = true;
	pproc->work_call.notify_all();
	pproc->work_mutex.unlock();
	for (int i = 0; i < pproc->company.size(); ++i) {//flag waiter
		if (!pproc->company[i]->work_complete) {
			--i;
		}
	}
	pproc->WORK_CALL = false;
	pproc->END_SYNC = true;
}
