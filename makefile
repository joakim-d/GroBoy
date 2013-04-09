all:
	make -C src groboy; \
	mv src/groboy release; 
debug:
	make -C src debug; \
	mv src/groboy dbg;
fac: 
	make -C src fac; \
	mv src/groboy release; 
fac-debug: 
	make -C src fac-debug; \
	mv src/groboy dbg; 
clean-all:
	echo "Deleting release/groboy and dbg/groboy"
	rm release/groboy 2> /dev/null
	rm dbg/groboy 2> /dev/null
