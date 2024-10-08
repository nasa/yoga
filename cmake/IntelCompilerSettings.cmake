if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" AND NOT APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd161,383,869,304,1,823")
  set(magic "")
  set(passed -1)
  execute_process(COMMAND gcc --version
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      OUTPUT_VARIABLE magic
      RESULT_VARIABLE passed
      )
  if(passed EQUAL 0)
    set(word_list ${magic})
    separate_arguments(word_list)
    list(GET word_list 2 gcc_version)
    set(intel_version ${CMAKE_CXX_COMPILER_VERSION})
    set(error_msg "  Intel does not ship with its own c++ stdlib,\n")
    string(APPEND error_msg "  instead, it relies on the gcc implementation.\n")
    string(APPEND error_msg "  Certain versions of Intel are only compatible\n")
    string(APPEND error_msg "  with certain versions of gcc.\n")
    string(APPEND error_msg "  -current Intel version: ${intel_version}\n")
    string(APPEND error_msg "  -current gcc version: ${gcc_version}\n")

    if(intel_version VERSION_LESS "17.0.0")
        message(FATAL_ERROR "Intel versions prior to 17 not supported")
    elseif(intel_version VERSION_LESS "19.0.0")
        if(gcc_version VERSION_LESS "6.0.0" OR gcc_version VERSION_GREATER "6.3.0")
            string(APPEND error_msg "Intel 17-18 only supports gcc 6-6.3")
            message(FATAL_ERROR ${error_msg})
        endif()
    else()
        if(gcc_version VERSION_LESS "6.0.0" OR gcc_version VERSION_GREATER "9.2.0")
            string(APPEND error_msg "Intel 19+ only supports gcc 6-9.2")
            message(FATAL_ERROR ${error_msg})
        endif()
    endif()

  else()
      message(FATAL_ERROR "Could not get gcc c++ stdlib version")
  endif()
endif()
