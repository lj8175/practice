PROJECT (tcpSslLibevent)

ADD_EXECUTABLE(server server.cpp)

TARGET_LINK_LIBRARIES(server event)
