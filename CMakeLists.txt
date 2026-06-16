cmake_minimum_required(VERSION 4.1)
project(MusicalKeyboardTrainer LANGUAGES C CXX)

#set(CURL_LIBRARY D:/libs/curl-8.6.0-gcc/bin/libcurl-x64.dll)
#set(SFML_DIR D:/libs/SFML-2.5.1-gcc)
#set(CURL_INCLUDE_DIR D:/libs/curl-8.6.0-gcc/include)
set(CMAKE_CXX_FLAGS -g)

find_package(CURL REQUIRED)
#include_directories(${CURL_INCLUDE_DIR})

set(CMAKE_BUILD_TYPE Release)

add_executable(MKT
	dialogbase.h dialogbase.cpp
	requestsongdialog.h requestsongdialog.cpp
	loadlyricsdialog.h loadlyricsdialog.cpp
	textinput.h textinput.cpp
	parser.h parser.cpp
	button.h button.cpp
	radiobutton.h radiobutton.cpp
	engine.h engine.cpp
	drawfuncs.h drawfuncs.cpp
	plotwidget.h plotwidget.cpp
	main.cpp
)

find_package(SFML COMPONENTS System Window Graphics REQUIRED)

target_link_libraries(MKT ${CURL_LIBRARIES})
target_link_libraries(MKT SFML::System SFML::Window SFML::Graphics)
