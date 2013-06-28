#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#


GEN_SRC = ../gen-cpp/SharedService.cpp ../gen-cpp/shared_types.cpp ../gen-cpp/tutorial_types.cpp ../gen-cpp/Calculator.cpp

default: server client

server: CppServer.cpp
	g++ -o CppServer -DHAVE_NETINET_IN_H  -I../gen-cpp CppServer.cpp ${GEN_SRC} -lthrift

client: CppClient.cpp
	g++ -o CppClient -DHAVE_NETINET_IN_H  -I../gen-cpp CppClient.cpp ${GEN_SRC} -lthrift

clean:
	$(RM) -r CppClient CppServer
