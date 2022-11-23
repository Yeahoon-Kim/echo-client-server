TARGETS = chatServer chatClient

all: 
	cd src; make; cd ..

debug:
	cd src; make debug; cd ..

clean:
	cd src; make clean; cd ..

.PHONY: all clean