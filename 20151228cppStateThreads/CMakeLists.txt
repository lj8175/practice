PROJECT (cppStateThreads)

include_directories(../../st-1.9/DARWIN_14.5.0_DBG/)
link_directories(../../st-1.9/DARWIN_14.5.0_DBG/)

ADD_EXECUTABLE(demo demo.cpp)

TARGET_LINK_LIBRARIES(demo st)
