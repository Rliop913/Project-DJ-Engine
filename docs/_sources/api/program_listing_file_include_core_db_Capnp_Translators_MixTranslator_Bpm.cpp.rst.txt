
.. _program_listing_file_include_core_db_Capnp_Translators_MixTranslator_Bpm.cpp:

Program Listing for File Bpm.cpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_MixTranslator_Bpm.cpp>` (``include/core/db/Capnp/Translators/MixTranslator/Bpm.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Bpm.hpp"
   
   BPM::BPM()
   {
       usable_threads = std::thread::hardware_concurrency();
       if (usable_threads == 0) {
           usable_threads = 1;
       }
   }
   
   BPM::~BPM()
   {
   }
   
   void
   bpm_thread(MixStruct    *M,
              std::mutex   *bpm_locker,
              BpmStruct    *bpms,
              unsigned long range)
   {
       MixStruct *mp = M;
       for (unsigned long i = 0; i < range; ++i) {
           if (mp->RP.getType() == TypeEnum::BPM_CONTROL) {
               auto        bpmStr = std::string(mp->RP.getFirst().cStr());
               BpmFragment tempbpm;
               tempbpm.beat     = mp->RP.getBeat();
               tempbpm.subBeat  = mp->RP.getSubBeat();
               tempbpm.separate = mp->RP.getSeparate();
               try {
                   tempbpm.bpm = std::stod(bpmStr);
               } catch (...) {
                   critlog("failed to convert string to double. From Bpm.cpp "
                           "bpm_thread. ExceptionLog: ");
                   critlog(bpmStr);
                   tempbpm.bpm = -404;
               }
               {
                   std::lock_guard<std::mutex> lock(*bpm_locker);
                   bpms->fragments.push_back(tempbpm);
               }
           }
           ++mp;
       }
   }
   
   bool
   BPM::getBpms(MIX &mixx)
   {
       unsigned long jobs_per_thread = mixx.mixVec.size() / usable_threads;
       if (jobs_per_thread == 0) {
           std::mutex bpm_locker;
           bpm_thread(
               &(mixx.mixVec[0]), &bpm_locker, &(bpmVec), mixx.mixVec.size());
       } else {
           unsigned long remained_job =
               mixx.mixVec.size() - (jobs_per_thread * usable_threads);
           std::vector<std::thread> thread_pool;
           unsigned long            idx = 0;
           std::mutex               bpm_locker;
           for (unsigned int i = 0; i < (usable_threads - 1); ++i) {
               thread_pool.emplace_back(bpm_thread,
                                        &(mixx.mixVec[idx]),
                                        &bpm_locker,
                                        &(bpmVec),
                                        jobs_per_thread);
               idx += jobs_per_thread;
           }
           thread_pool.emplace_back(bpm_thread,
                                    &(mixx.mixVec[idx]),
                                    &bpm_locker,
                                    &(bpmVec),
                                    jobs_per_thread + remained_job);
   
           for (int i = 0; i < thread_pool.size(); ++i) {
               thread_pool[i].join();
           }
       }
       bpmVec.sortFragment();
       if (bpmVec.fragments.empty() || bpmVec.fragments[0].beat != 0 ||
           bpmVec.fragments[0].subBeat != 0) {
           critlog("failed to sort bpmFragments. from BPM getBpms.");
           return false;
       }
       return bpmVec.calcFrame();
   }
