
.. _program_listing_file_include_tests_gittest.cpp:

Program Listing for File gittest.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_gittest.cpp>` (``include/tests/gittest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <iostream>
   #include "gitWrapper.hpp"
   int main()
   {
       git_libgit2_init();
       git_repository* repo = NULL;
       int err = git_repository_init(&repo, "./testgit", false);
       std::cout <<err <<std::endl;
       git_libgit2_shutdown();
       return 0;
   }
