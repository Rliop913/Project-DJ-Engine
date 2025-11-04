
.. _program_listing_file_include_input_windows_process_Input_Process_Main.cpp:

Program Listing for File Input_Process_Main.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_windows_process_Input_Process_Main.cpp>` (``include/input/windows/process/Input_Process_Main.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ChildProcess.hpp"
   
   int main(int argc, char const *argv[])
   {
       if(argc < 2){
           return -1;
       }
       try{
   
           PDJE_IPC::ChildProcess serv;
           auto port = std::stoi(argv[1]);
           serv.RunServer(port);
           if(serv.KillCheck){
               return 0;
           }
           serv.LoopTrig();
       }
       catch (const std::exception& e){
           critlog(e.what());
       }
       return 0;
   }
