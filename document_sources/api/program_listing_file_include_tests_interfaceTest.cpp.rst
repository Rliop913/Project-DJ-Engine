
.. _program_listing_file_include_tests_interfaceTest.cpp:

Program Listing for File interfaceTest.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_interfaceTest.cpp>` (``include/tests/interfaceTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_interface.hpp"
   
   #include <iostream>
   int
   main()
   {
       auto testpdje     = new PDJE("./tempdb.db");
       auto searchResult = testpdje->SearchMusic("WTC", "");
       if (searchResult.empty()) {
           std::cout << "can't search" << std::endl;
           delete testpdje;
           return 1;
       }
       for (auto i : searchResult) {
           std::cout << "title: " << std::string(i.title.begin(), i.title.end())
                     << "path: "
                     << std::string(i.musicPath.begin(), i.musicPath.end())
                     << std::endl;
       }
   
       auto trackSearch = testpdje->SearchTrack("testmix111");
       if (trackSearch.empty()) {
           std::cout << "can't search track" << std::endl;
           delete testpdje;
           return 1;
       }
       for (auto i : trackSearch) {
           std::cout << " track title: " << TO_STR(i.trackTitle)
                     << " note binary size: " << i.noteBinary.size()
                     << " mix binary size: " << i.mixBinary.size() << std::endl;
       }
       testpdje->InitPlayer(PLAY_MODE::HYBRID_RENDER, trackSearch[0], 48);
       if (!testpdje->player.has_value()) {
           std::cout << "can't search track" << std::endl;
           delete testpdje;
           return 1;
       }
       testpdje->player->Activate();
       getchar();
       testpdje->player->GetFXControlPanel()->FX_ON_OFF(FXList::DISTORTION, true);
       auto panel =
           testpdje->player->GetFXControlPanel()->GetArgSetter(FXList::DISTORTION);
       panel["distortionValue"](2);
   
       getchar();
       auto mus = testpdje->player->GetMusicControlPanel();
       mus->LoadMusic(testpdje->DBROOT.value(), searchResult[0]);
       std::cout << TO_STR(mus->GetLoadedMusicList()[0]);
       mus->SetMusic("WTC", true);
       getchar();
       for (auto i : (panel)) {
           std::cout << i.first << " " << std::endl;
       }
       testpdje->player->Deactivate();
       getchar();
       delete testpdje;
       return 0;
   }
