ak.PHONY = all

FLAGS = -lX11 -pthread -lsfml-graphics -lsfml-window -lsfml-system -lGL -Wno-pointer-arith
OUTPUT = game.out
CC = /usr/bin/g++ $(FLAGS)

SRCS := $(shell find . -wholename "./src/*.cpp")
BINS := $(SRCS:./src/%.cpp=out/%.o)

PROGRAMS = \ ${BINS} \
	main \


all : ${BINS} main

debug : FLAGS += -DDEBUG -g
debug : OUTPUT = debug_game.out
debug : all

out/%.o : src/%.cpp
	${CC} -c $< -o $@


main : 
	${CC} -o ${OUTPUT} ${BINS}

clean :
	rm ${BINS}