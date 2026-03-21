
.. _program_listing_file_include_util_db_BackendConcept.hpp:

Program Listing for File BackendConcept.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_BackendConcept.hpp>` (``include/util/db/BackendConcept.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/keyvalue/BackendConcept.hpp"
   
   namespace PDJE_UTIL::db {
   
   template <class Backend>
   concept BackendConcept = keyvalue::KeyValueBackendConcept<Backend>;
   
   } // namespace PDJE_UTIL::db
