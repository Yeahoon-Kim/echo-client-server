TARGETS = chatServer chatClient

all: 
	cd src; make; cd ..

clean:
	cd src; make clean; cd ..

.PHONY: all clean