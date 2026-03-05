
.. _program_listing_file_include_global_DataLines_PDJE_Buffer.hpp:

Program Listing for File PDJE_Buffer.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_PDJE_Buffer.hpp>` (``include\global\DataLines\PDJE_Buffer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Atomic_Double_Buffer.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "ipc_shared_memory.hpp"
   #include <memory_resource>
   #include <random>
   
   namespace fs = std::filesystem;
