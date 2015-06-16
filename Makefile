server: server.cpp commands.cpp commands.hpp
	gcc --std=c++11  -o server server.cpp commands.cpp -lstdc++
