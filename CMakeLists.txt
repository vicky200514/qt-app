cmake_minimum_required(VERSION 3.16)
project(QuestionPaperViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Pdf PdfWidgets)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

add_executable(question_paper_viewer
    main.cpp
    loginwindow.cpp
    loginwindow.h
    pdfviewerwindow.cpp
    pdfviewerwindow.h
    networkworker.cpp
    networkworker.h
)

target_link_libraries(question_paper_viewer PRIVATE
    Qt6::Widgets
    Qt6::Pdf
    Qt6::PdfWidgets
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)
