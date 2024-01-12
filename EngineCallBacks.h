#pragma once
#include "MiniAudioWrapper.h"
#include "Processor.h"



void
idle_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	Processor* pproc = (Processor*)pDevice->pUserData;
	if (pproc->deck.size() > 0) {
		bool this_is_true = true;
		pproc->deck[-1]->idle_process(frameCount, (float*)pOutput);
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
	auto Dp = pproc->get_deck_p();
	for (int i = 0; i < pproc->deck_size(); ++i) {
		pproc->add_processed_time(frameCount);

		if (pproc->ID_is_in_stopQ(Dp->first)) {
			pproc->delete_album(Dp->first);
		}
		else {
			work temp_work;
			temp_work.ID = Dp->first;
			temp_work.frameCount = frameCount;
			temp_work.buf = pOutput;
			pproc->cq.push(temp_work);


		}
		++Dp;
	}
	pproc->TRIG_CALL = true;
	pproc->clear_stopQ();
	//not done here
	while (pproc->TRIG_CALL) {//wait until worker thread end

	}
}


//
//void
//idle_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
//	Processor* pproc = (Processor*)pDevice->pUserData;
//	auto Dp = pproc->get_deck_p();
//	for (int i = 0; i < pproc->deck_size(); ++i){
//			pproc->ID_is_in_stopQ(Dp->first) ?
//			pproc->delete_album(Dp->first)
//			:
//			Dp->second->idle_process(frameCount, (float*)pOutput);
//			++Dp;
//	}
//	pproc->clear_stopQ();
//}
//
//
//void
//dj_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
//	Processor* pproc = (Processor*)pDevice->pUserData;
//	auto Dp = pproc->get_deck_p();
//	for (int i = 0; i < pproc->deck_size(); ++i) {
//		pproc->add_processed_time(frameCount);
//
//		if (pproc->ID_is_in_stopQ(Dp->first)) {
//			pproc->delete_album(Dp->first);
//		}
//		else {
//			work temp_work;
//			temp_work.ID = Dp->first;
//			temp_work.frameCount = frameCount;
//			temp_work.buf = pOutput;
//			pproc->cq.push(temp_work);
//
//
//		}
//		++Dp;
//	}
//	pproc->TRIG_CALL = true;
//	pproc->clear_stopQ();
//	//not done here
//	while (pproc->TRIG_CALL) {//wait until worker thread end
//
//	}
//}
////
//void
//daw_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
//	Engine_Main* pengine = (Engine_Main*)pDevice->pUserData;
//	pengine->processing = true;
//	//void* buffer = malloc(VOID_BUFFER_SIZE(frameCount, pengine->custom.channel));
//	//ma_silence_pcm_frames(buffer, frameCount, ma_format_f32, pengine->custom.channel);
//	if (!pengine->deck.empty()) {
//
//		for (auto i = pengine->deck.begin(); i != pengine->deck.end(); i++) {
//			if (pengine->stop_queue.count(i->first) == (size_t)1) {
//
//			}
//			else {
//				Engine_Main::work temp_work;
//				temp_work.ID = i->first;
//				temp_work.frameCount = frameCount;
//				temp_work.buf = pOutput;
//				pengine->cq.push(temp_work);
//			}
//		}
//		pengine->TRIG_CALL = true;
//		for (auto i = pengine->stop_queue.begin(); i != pengine->stop_queue.end(); i++) {
//			delete pengine->deck[i->first];
//			pengine->deck.erase(i->first);
//			pengine->stop_queue.erase(i->first);
//		}//delete album, threads are working
//		while (pengine->TRIG_CALL) {//wait until worker thread end
//		}
//	}
//
//	//for (int j = 0; j < VOID_BUFFER_SIZE(frameCount, pengine->custom.channel) / 4; j++) {
//	//	((float*)pOutput)[j] = ((float*)buffer)[j];//copy buffer to output
//	//}
//	//free(buffer);
//	pengine->processing = false;
//}
