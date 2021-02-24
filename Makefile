compiler = cc
files = *.c
OS := $(shell uname -s)
flags = -lraylib
ifeq ($(OS),Darwin)
	flags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -D_FORTIFY_SOURCE=0
endif
ifeq ($(OS),Linux)
	flags += -lGL -lm -lpthread -ldl -lrt -lX11
endif
out = -o bin/app

# Kommer til at blive brugt
#include 	= -I include
#wsserver	= lib/libws.a

# En liste over alt der skal køres
all: game

run:
	@make
	@./bin/app
	@rm ./bin/app

# Selve spillet
game:
	@mkdir -p bin
	$(compiler) $(files) $(flags) $(include) $(out)
	
# Clean
clean:
	@rm -rf bin
