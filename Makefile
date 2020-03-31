build: 
	mkdir -p build
	cd build; cmake3 ..
	cd build; make

run: build
	build/train_game

valgrind: build
	valgrind build/library_tests

test: build
	build/library_tests

clean: 
	rm -rf build
