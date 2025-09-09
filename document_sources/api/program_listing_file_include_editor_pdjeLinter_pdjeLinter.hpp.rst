
.. _program_listing_file_include_editor_pdjeLinter_pdjeLinter.hpp:

Program Listing for File pdjeLinter.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_pdjeLinter_pdjeLinter.hpp>` (``include/editor/pdjeLinter/pdjeLinter.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "dbRoot.hpp"
   
   template <typename DataType> class PDJE_Linter {
     private:
     public:
       static bool
       Lint(const DataType &target, UNSANITIZED &lint_msg);
   };
