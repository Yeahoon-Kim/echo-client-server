CXX = g++
CXXFLAGS = -W -Wall -O2
DFLAGS = -W -Wall -g -O0
TAEGET = ../bin/chatServer
DTARGET = ../bin/chatServer-test
OBJECTS = ../chatProgram.o chatServer.o main.o
DOBJECTS = ../chatProgram-test.o chatServer-test.o main-test.o
LIBS = -lpthread

all: $(TARGET)
debug: $(DTARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(DTARGET): $(DOBJECTS)
	$(CXX) $(DFLAGS) -o $(DTARGET) $(DOBJECTS) $(LIBS)

main.o: main.cpp
chatServer.o: chatServer.cpp

main-test.o: main.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)
chatServer-test.o: chatServer.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)

clean:
	rm -f $(TARGET)
	rm -f $(DTARGET)
	rm -f *.o

.PHONY: all debug clean
