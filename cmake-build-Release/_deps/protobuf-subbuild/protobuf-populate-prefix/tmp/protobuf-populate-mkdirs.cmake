# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-src"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-build"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/tmp"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/src"
  "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/rrop/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/cmake-build-Release/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
