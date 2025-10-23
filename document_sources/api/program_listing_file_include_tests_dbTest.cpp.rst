
.. _program_listing_file_include_tests_dbTest.cpp:

Program Listing for File dbTest.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_dbTest.cpp>` (``include/tests/dbTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_interface.hpp"
   
   #include <iostream>
   #include <string>
   int
   main()
   {
       auto pdje   = new PDJE("ROOT_DB");
       auto Musics = pdje->SearchMusic("", "");
       auto Tracks = pdje->SearchTrack("");
       std::cout << "============musics==================" << std::endl;
       for (const auto &musd : Musics) {
   
           std::cout << "title: " << PDJE_Name_Sanitizer::getFileName(musd.title)
                     << std::endl;
           std::cout << "composer: "
                     << PDJE_Name_Sanitizer::getFileName(musd.composer)
                     << std::endl;
   
           std::cout << "bpm: " << musd.bpm << std::endl;
           CapReader<MusicBinaryCapnpData> bpmReader;
           bpmReader.open(musd.bpmBinary);
           auto datas = bpmReader.Rp->getDatas();
           std::cout << "getting bpm binaries" << musd.bpm << std::endl;
           for (int i = 0; i < datas.size(); ++i) {
               std::cout << "beat: " << datas[i].getBeat()
                         << " subBeat: " << datas[i].getSubBeat()
                         << " separate: " << datas[i].getSeparate() << std::endl;
           }
   
           std::cout << "first beat: " << musd.firstBeat << std::endl;
           std::cout << "musicPath: "
                     << PDJE_Name_Sanitizer::getFileName(musd.musicPath)
                     << std::endl;
       }
   
       std::cout << "==============tracks====================" << std::endl;
       for (const auto &trackd : Tracks) {
           std::cout << "track title"
                     << PDJE_Name_Sanitizer::getFileName(trackd.trackTitle)
                     << std::endl;
           std::cout << "getting mix binary" << std::endl;
           CapReader<MixBinaryCapnpData> mixReader;
           mixReader.open(trackd.mixBinary);
           auto mixs = mixReader.Rp->getDatas();
           for (int i = 0; i < mixs.size(); ++i) {
   
               std::cout << " Type: " << static_cast<int64_t>(mixs[i].getType())
                         << " Detail: "
                         << static_cast<int64_t>(mixs[i].getDetails())
                         << " ID: " << mixs[i].getId() << std::endl;
               std::cout << " first: " << mixs[i].getFirst().cStr()
                         << " Second: " << mixs[i].getSecond().cStr()
                         << " Third: " << mixs[i].getThird().cStr() << std::endl;
               std::cout << " beat: " << mixs[i].getBeat()
                         << " subBeat: " << mixs[i].getSubBeat()
                         << " separate: " << mixs[i].getSeparate() << std::endl;
               std::cout << " Ebeat: " << mixs[i].getEbeat()
                         << " Esubbeat: " << mixs[i].getEsubBeat()
                         << " Eseparate: " << mixs[i].getEseparate() << std::endl;
           }
           CapReader<NoteBinaryCapnpData> noteReader;
           noteReader.open(trackd.mixBinary);
           auto notes = noteReader.Rp->getDatas();
           for (int i = 0; i < notes.size(); ++i) {
               std::cout << " NoteType: " << notes[i].getNoteType().cStr()
                         << " NoteDetail: " << notes[i].getNoteDetail()
                         << std::endl;
               std::cout << " first: " << notes[i].getFirst().cStr()
                         << " Second: " << notes[i].getSecond().cStr()
                         << " Third: " << notes[i].getThird().cStr() << std::endl;
               std::cout << " beat: " << notes[i].getBeat()
                         << " subBeat: " << notes[i].getSubBeat()
                         << " separate: " << notes[i].getSeparate() << std::endl;
               std::cout << " Ebeat: " << notes[i].getEbeat()
                         << " Esubbeat: " << notes[i].getEsubBeat()
                         << " Eseparate: " << notes[i].getESeparate() << std::endl;
           }
       }
       return 0;
   }
