#OBJS specifies which files to compile as part of the project 
OBJS = main.cpp

#CC specifies which compiler we're using 
CC = g++
#INCLUDE_PATHS specifies the additional include paths we'll need 
INCLUDE_PATHS = -I ./SDL2/include

#LIBRARY_PATHS specifies the additional library paths we'll need 
LIBRARY_PATHS = -L ./lib

WINAPI = -lmingw32 -lsdl -lsetupapi -lkernel32 -lm -ldxguid -ldxerr8 -luser32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lgdi32 -lcomdlg32 -lwinspool 
WINAPI+= -lcomctl32 -luuid -lrpcrt4 -ladvapi32 -lwsock32 -lshlwapi -lversion -ldbghelp -lwinpthread -static-libgcc -static-libstdc++

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = Ether.exe 

SHARED_FLAGS = -O3 -s -m32 -Wall -Wextra -fno-align-functions -Wno-unused-parameter -Wno-cast-function-type -Wno-write-strings
export SHARED_FLAGS

_SDL2: 
	$(MAKE) all -C ./SDL2

#This is the target that compiles our executable 
all: _SDL2
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SHARED_FLAGS) $(WINAPI) -o $(OBJ_NAME)
	
clean_o:
	$(MAKE) clean -C ./SDL2	