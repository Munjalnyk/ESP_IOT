# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/munjal/esp/esp-idf/components/bootloader/subproject"
  "/home/munjal/project/ESP_IOT/build/bootloader"
  "/home/munjal/project/ESP_IOT/build/bootloader-prefix"
  "/home/munjal/project/ESP_IOT/build/bootloader-prefix/tmp"
  "/home/munjal/project/ESP_IOT/build/bootloader-prefix/src/bootloader-stamp"
  "/home/munjal/project/ESP_IOT/build/bootloader-prefix/src"
  "/home/munjal/project/ESP_IOT/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/munjal/project/ESP_IOT/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/munjal/project/ESP_IOT/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
