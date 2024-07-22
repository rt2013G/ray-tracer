run: build
	@./bin/out
build:
	g++ -std=c++11 -O3 -Wall -Werror src/main.cpp -o bin/out
t:
	g++ -std=c++11 -O3 -Wall -Werror test/test.cpp -o bin/tout && ./bin/tout