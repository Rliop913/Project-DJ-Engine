
.. _program_listing_file_include_tests_audioTest.cpp:

Program Listing for File audioTest.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_audioTest.cpp>` (``include\tests\audioTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #include "CapnpBinary.hpp"
   #include "MixMachine.hpp"
   #include "MixTranslator.hpp"
   #include "audioPlayer.hpp"
   #include "miniaudio.h"
   
   #include "dbRoot.hpp"
   #include <algorithm>
   #include <iostream>
   
   void
   fill(capnp::List<MBData, capnp::Kind::STRUCT>::Builder &ret,
        int                                                idx,
        TypeEnum                                           Ty,
        DetailEnum                                         De,
        int                                                beat,
        int                                                ebeat,
        std::string                                        first,
        std::string                                        second = "",
        std::string                                        third  = "")
   {
       ret[idx].setbeat(beat);
       ret[idx].setBeat(0);
       ret[idx].setSeparate(4);
       ret[idx].setEbeat(ebeat);
       ret[idx].setEbeat(0);
       ret[idx].setEseparate(4);
       ret[idx].setType(Ty);
       ret[idx].setDetails(De);
       ret[idx].setFirst(first);
       ret[idx].setSecond(second);
       ret[idx].setThird(third);
   
       ret[idx].setId(0);
   }
   
   void
   fillDatas(capnp::List<MBData, capnp::Kind::STRUCT>::Builder &ret)
   {
       fill(
           ret, 0, TypeEnum::BPM_CONTROL, DetailEnum::TIME_STRETCH, 0, 0, "175.0");
       fill(ret, 1, TypeEnum::LOAD, DetailEnum::CUE, 1, 2, "WTC", "TEST", "175.0");
   
       fill(ret, 2, TypeEnum::VOL, DetailEnum::TRIM, 5, 37, "3", "0.5", "30");
       // "1", "0,1.0,0,1.0,0.5,1.0,0.5,1.0", "20");
   
       fill(ret,
            3,
            TypeEnum::FILTER,
            DetailEnum::HIGH,
            5,
            37,
            "1",
            "5000, 10000, 0, 1000, 5000, 7000, 10000",
            "");
   
       fill(ret,
            4,
            TypeEnum::BPM_CONTROL,
            DetailEnum::TIME_STRETCH,
            34,
            84,
            "88.0");
   
       fill(ret, 5, TypeEnum::CONTROL, DetailEnum::PAUSE, 81, 82, "");
   
       fill(ret, 6, TypeEnum::UNLOAD, DetailEnum::CUE, 90, 90, "");
   }
   
   auto idx = 0;
   void
   idle_callback(ma_device  *pDevice,
                 void       *pOutput,
                 const void *pInput,
                 ma_uint32   frameCount)
   {
       // auto dvec = reinterpret_cast<std::vector<float>*>(pDevice->pUserData);
       auto dP = (float *)(pDevice->pUserData); // dvec->data();
       dP += idx;
       // if (idx + frameCount < (*dvec).size()) {
       memcpy(pOutput, dP, sizeof(float) * frameCount * 2);
       idx += frameCount * 2;
   
       // }
   }
   // #include <termios.h>
   // #include "MiniAudioWrapper.hpp"
   int
   main()
   {
   
       auto mb = CapWriter<MixBinaryCapnpData>();
       mb.makeNew();
       // auto B = reinterpret_cast<MixBinaryCapnpData::Builder*>(mb.D);
       auto ret = mb.Wp->initDatas(7);
       fillDatas(ret);
       auto flat_returned = mb.out();
   
       auto rb = CapReader<MixBinaryCapnpData>();
       rb.open(flat_returned);
       // auto reader = reinterpret_cast<MixBinaryCapnpData::Reader*>(rb.D);
       auto readGet = rb.Rp->getDatas();
       std::cout << readGet[0].getId() << std::endl;
       std::cout << readGet[1].getId() << std::endl;
       std::cout << ret[0].getFirst().cStr() << std::endl;
   
       litedb dbb;
       if (dbb.openDB("./tempdb.db")) {
           std::cout << "opened" << std::endl;
       } else {
           std::cout << "not opened " << std::endl;
       }
       trackdata td;
       td.trackTitle = "testmix111";
   
       auto restd = dbb << td;
       std::cout << restd->front().mixBinary.size() << std::endl;
       std::cout << rb.out().size() << std::endl;
   
       if (restd.value().front().mixBinary.size() == rb.out().size()) {
           std::cout << "Completele same" << std::endl;
       }
       auto AP     = new audioPlayer(dbb, restd.value().front(), 48, true);
       auto pannel = AP->GetFXControlPannel();
       AP->Activate();
       getchar();
       pannel->FX_ON_OFF(FXList::FILTER, true);
       auto filter_handle = pannel->GetArgSetter(FXList::FILTER);
       for (auto i : filter_handle) {
           std::cout << i.first << std::endl;
       }
   
       char input = 'a';
       int  freq  = 5000;
       int  udVal = 100;
       filter_handle["Filterfreq"](5000);
       filter_handle["HLswitch"](1);
       while (input != 'q') {
           std::cin >> input;
           if (input == 'u') {
               freq += udVal;
               filter_handle["Filterfreq"](freq);
           }
           if (input == 'd') {
               freq -= udVal;
               filter_handle["Filterfreq"](freq);
           }
           if (input == 'h') {
               filter_handle["HLswitch"](0);
           }
           if (input == 'l') {
               filter_handle["HLswitch"](1);
           }
       }
   
       delete AP;
       // if(dbb<<td){
       //     std::cout << "DONE!" << std::endl;
       // }
       // else{
       //     std::cout << SQLITE_LAST_ERRNO << std::endl;
       // }
   
       return 0;
       struct vectest {
           unsigned int one;
           unsigned int two;
       };
   
       MixTranslator mt = MixTranslator();
       if (!(mt.Read(rb))) {
           return 1;
       }
       for (auto i : mt.mixs->mixVec) {
           std::cout << "Frame In: " << i.frame_in << "Frame Out: " << i.frame_out
                     << std::endl;
       }
   
       auto mm = new MixMachine();
       auto db = litedb();
       db.openDB("./tempdb.db");
       mm->IDsort(mt);
       if (!mt.bpms.has_value()) {
           return -1;
       }
   
       auto      res = mm->mix(db, mt.bpms.value());
       ma_device dev;
       auto      Dres = Decoder();
       Dres.init("./WTC.wav", "./");
       // auto arrD = Dres.getRange(48000*10);
       if (res) {
           ma_device_config deconf =
               ma_device_config_init(ma_device_type_playback);
           deconf.playback.format    = ma_format_f32;
           deconf.playback.channels  = 2;
           deconf.sampleRate         = 48000;
           deconf.periodSizeInFrames = 480;
           deconf.dataCallback       = idle_callback;
           deconf.performanceProfile = ma_performance_profile_low_latency;
           deconf.pUserData          = (mm->rendered_out.data());
           ma_device_init(NULL, &deconf, &dev);
           ma_device_start(&dev);
       }
       getchar();
   
       return 0;
   }
