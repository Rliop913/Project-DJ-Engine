
.. _program_listing_file_include_tests_dbTest.cpp:

Program Listing for File dbTest.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_dbTest.cpp>` (``include/tests/dbTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "CapnpBinary.hpp"
   #include "dbRoot.hpp"
   #include <iostream>
   #include <string>
   int
   main()
   {
       litedb dbr = litedb();
       if (!dbr.openDB("./tempdb.db")) {
           return 1;
       }
       auto td          = trackdata("first");
       auto md          = musdata("WTC", "TEST", "./WTC.wav", 175);
       md.firstBeat     = "1056";
       auto musicBinary = CapWriter<MusicBinaryCapnpData>();
       musicBinary.makeNew();
       auto aubuilder = musicBinary.Wp->initDatas(2);
       aubuilder[0].setBeat(0);
       aubuilder[0].setSubBeat(0);
       aubuilder[0].setSeparate(4);
       aubuilder[0].setBpm("175.0");
       aubuilder[1].setBeat(32);
       aubuilder[1].setBeat(0);
       aubuilder[1].setSeparate(4);
       aubuilder[1].setBpm("88.0");
       md.bpmBinary = musicBinary.out();
   
       dbr <= md;
       auto mdret = dbr << md;
       // auto tdret = dbr << td;
       if (!mdret.has_value()) {
   
           return 1;
       }
       // if(!tdret.has_value()){
       //     return 1;
       // }
       for (auto i : mdret.value()) {
           std::cout << std::string(i.title.begin(), i.title.end()) << ", "
                     << std::string(i.musicPath.begin(), i.musicPath.end()) << ", "
                     << std::string(i.composer.begin(), i.composer.end()) << ", "
                     << i.bpm << std::endl;
           for (auto j : i.bpmBinary) {
               std::cout << j << std::endl;
           }
       }
       // for(auto i : tdret.value()){
       //     std::cout << i.trackTitle << std::endl;
       // }
   
       return 0;
   }
