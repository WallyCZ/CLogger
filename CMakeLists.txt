cmake_minimum_required(VERSION 3.4)
project (CLogger)


IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
ENDIF(MSVC)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("cxx Flags:" ${CMAKE_CXX_FLAGS})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore")
  message("AppType: WinStore")
  set(APP_SYSTEM WIN_STORE)
else()
  message("AppType: WinDesktop")
  set(APP_SYSTEM WIN_DESKTOP)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(DLL_NAME CLogger)
set(SHORT_NAME ${DLL_NAME})

set(SOURCE_FILES
  pch.cpp
  Logger.cpp
  LogTargetmanager.cpp
  Targets/FileOutput.cpp
  )

set(HEADER_FILES
  pch.h
  ILogTarget.h
  ILogTargetRef.h
  Logger.h
  LoggerRef.h
  LogTargetManager.h
  Targets/DebugOutputTarget.h
  Targets/FileOutput.h
  )

if(${APP_SYSTEM} STREQUAL WIN_STORE)
  list(APPEND SOURCE_FILES
    LoggerRef.cpp
    LogTargetManagerRef.cpp
  )

  list(APPEND HEADER_FILES
    LogTargetManagerRef.h
    Targets/DebugOutputTargetRef.h
    Targets/FileOutputRef.h
  )

endif()

add_definitions(-DCompactLogger_EXPORTS)

set_property(SOURCE ${DEBUG_CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT $<CONFIG:Debug>)
set_property(SOURCE ${RELEASE_CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT $<CONFIG:Release>)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

add_library(${DLL_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

if(${APP_SYSTEM} STREQUAL WIN_STORE)
  set_property(TARGET ${DLL_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)
  set_property(TARGET ${DLL_NAME} PROPERTY VS_GLOBAL_ROOTNAMESPACE "CLogger")
endif()

target_link_libraries(${DLL_NAME}  )