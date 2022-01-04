.PHONY: build
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

segment_demo: build
	build/line_segment_demo '(20,20)' '(400,400)' '10'
	build/line_segment_demo '(400,20)' '(20,400)' '10'
	build/line_segment_demo '(400,20)' '(20,400)' '100'

intersection_demo: build
	build/intersection_demo '(20,20)' '(400,400)' '(300,0)'
	build/intersection_demo '(400,20)' '(20,400)' '(300,0)'
	build/intersection_demo '(400,20)' '(20,400)' '(100,400)'
	
loading_test: build
	build/loading_test res/sample_track_data.json

clean: 
	rm -rf build
