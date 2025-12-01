
.. _program_listing_file_include_input_windows_process_Input_Process_Main.cpp:

Program Listing for File Input_Process_Main.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_windows_process_Input_Process_Main.cpp>` (``include/input/windows/process/Input_Process_Main.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ChildProcess.hpp"
   #include "PDJE_Crypto.hpp"
   #include "PSKPipe.hpp"
   #include <sstream>
   int
   main()
   {
       try {
           startlog();
           auto tokenstr = PDJE_CRYPTO::PSKPipe::GetTokenFromSTDPipe();
           std::istringstream spstrm(tokenstr);
           std::string        pskhex;
           std::string        portstr;
           spstrm >> pskhex;
           spstrm >> portstr;
           auto psk = PDJE_CRYPTO::PSK();
           psk.Decode(pskhex);
   
           PDJE_IPC::ChildProcess serv(psk);
           int                    port = std::stoi(portstr);
           serv.RunServer(port);
           if (serv.KillCheck) {
               return 0;
           }
           serv.LoopTrig();
       } catch (const std::exception &e) {
           critlog(e.what());
       }
       return 0;
   }
