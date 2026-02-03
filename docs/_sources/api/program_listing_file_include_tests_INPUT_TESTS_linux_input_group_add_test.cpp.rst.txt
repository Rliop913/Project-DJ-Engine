
.. _program_listing_file_include_tests_INPUT_TESTS_linux_input_group_add_test.cpp:

Program Listing for File linux_input_group_add_test.cpp
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_linux_input_group_add_test.cpp>` (``include/tests/INPUT_TESTS/linux_input_group_add_test.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "make_group.hpp"
   int
   main()
   {
       try {
           std::string dev = "/dev/input/event0";
   
           std::string user = get_username();
           std::string grp  = device_group(dev);
   
           if (user_in_group(user, grp)) {
               std::cout << "[OK] user already in group '" << grp << "'\n";
               return 0;
           }
   
           std::cout << "[NO] user NOT in group '" << grp << "'\n";
   
           // 여기서부터는 "변경"이므로 root 필요
           if (geteuid() != 0) {
               std::cerr << "Need root to modify groups. Re-run with sudo.\n";
               return 2;
           }
   
           if (!group_exists(grp)) {
               std::cout << "Group '" << grp << "' doesn't exist. Creating...\n";
               // 배포판마다 경로가 다를 수 있음: /usr/sbin/groupadd 흔함
               int st = run_cmd("/usr/sbin/groupadd", { "-r", grp });
               if (st != 0) {
                   std::cerr << "groupadd failed, status=" << st << "\n";
                   return 3;
               }
           }
   
           std::cout << "Adding user to group...\n";
           // /usr/sbin/usermod (대부분)
           int st = run_cmd("/usr/sbin/usermod", { "-aG", grp, user });
           if (st != 0) {
               std::cerr << "usermod failed, status=" << st << "\n";
               return 4;
           }
   
           std::cout
               << "[DONE] Added. Re-login is typically required to take effect.\n";
   
           // (옵션) 현재 프로세스만 그룹 갱신 (root일 때 가능). 전체 세션은
           // 재로그인이 안전. initgroups(user.c_str(), /*primary gid*/ ??? ) 같은
           // 처리도 가능하지만 보통 재로그인 안내가 정석.
   
           return 0;
       } catch (const std::exception &e) {
           std::cerr << "Error: " << e.what() << "\n";
           return 1;
       }
   }
