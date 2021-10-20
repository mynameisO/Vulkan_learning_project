COMP = g++
CVER = -std=c++17
LIB = -LC:/Vulkan/lib
INCLUDE = -IC:/Vulkan/Include
CPPFILE = cppdirectory/*.cpp

debug : cppdirectory/main.cpp
	$(COMP) -g -o test -D DBUG $(CVER) $(INCLUDE) $(LIB) $(CPPFILE) -lvulkan-1 -lglfw3 -lgdi32 -luser32 -lkernel32

release : cppdirectory/main.cpp
	$(COMP) -o main -D NDEBUG $(CVER) $(INCLUDE) $(LIB) $(CPPFILE) -lvulkan-1 -lglfw3 -lgdi32 -luser32 -lkernel32