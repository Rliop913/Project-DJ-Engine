
.. _program_listing_file_include_core_db_Capnp_Translators_MusicTranslator_MusicTranslator.cpp:

Program Listing for File MusicTranslator.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_MusicTranslator_MusicTranslator.cpp>` (``include\core\db\Capnp\Translators\MusicTranslator\MusicTranslator.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicTranslator.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <string>
   bool
   MusicTranslator::Read(const CapReader<MusicBinaryCapnpData> &binary,
                         unsigned long long                     startFrame)
   {
       auto DVec = binary.Rp->getDatas();
       for (unsigned long i = 0; i < DVec.size(); ++i) {
           if (DVec[i].hasBpm()) {
               BpmFragment frg;
               frg.beat     = DVec[i].getBeat();
               frg.subBeat  = DVec[i].getSubBeat();
               frg.separate = DVec[i].getSeparate();
               try {
                   frg.bpm = std::stod(DVec[i].getBpm().cStr());
   
               } catch (std::exception &e) {
                   critlog("failed to convert string to double. from "
                           "MusicTranslator Read. ErrString: ");
                   critlog(DVec[i].getBpm().cStr());
                   continue;
               }
               bpms.fragments.push_back(frg);
           }
       }
       bpms.sortFragment();
   
       return bpms.calcFrame(startFrame);
   }
