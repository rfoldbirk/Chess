compiler	= cc
files		= *.c
flags		= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
include 	= -I include
wsserver	= lib/libws.a
out			= -o bin/app

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
