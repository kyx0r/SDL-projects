#OBJS specifies which files to compile as part of the project 
OBJS = ./*cpp
HEADS = ./include/*.h

#CC specifies which compiler we're using 
CC = g++
#INCLUDE_PATHS specifies the additional include paths we'll need 
INCLUDE_PATHS = -I ./SDL2-2.0.8/i686-w64-mingw32/include/

#LIBRARY_PATHS specifies the additional library paths we'll need 
LIBRARY_PATHS = -L ./SDL2-2.0.8/i686-w64-mingw32/lib

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
COMPILER_FLAGS = -w 

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -static-libstdc++
 
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = Ether.exe 

#This is the target that compiles our executable 
all : 
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)