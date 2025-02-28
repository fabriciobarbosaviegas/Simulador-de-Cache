CC = g++
CFLAGS = -std=c++17 -Iinclude

all: cache_simulator

cache_simulator: src/main.cpp src/cache.cpp src/utils.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f cache_simulator
