CXX = g++
CXXFLAGS = -W -Wall -O2
DFLAGS = -W -Wall -g -O0
TARGET = ../../bin/chatClient
DTARGET = ../../bin/chatClient-test
OBJECTS = ../chatProgram.o chatClient.o ip.o main.o
DOBJECTS = ../chatProgram-test.o chatClient-test.o ip-test.o main-test.o
LIBS = -lpthread

all: $(TARGET)
debug: $(DTARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(DTARGET): $(DOBJECTS)
	$(CXX) $(DFLAGS) -o $(DTARGET) $(DOBJECTS) $(LIBS)

main.o: main.cpp
chatClient.o: chatClient.cpp 
clientParam.o: clientParam.cpp
ip.o: ip.cpp

main-test.o: main.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)
chatClient-test.o: chatClient.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)
clientParam-test.o: clientParam.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)
ip-test.o: ip.cpp
	$(CXX) -DDEBUG -c -o $(@) $(^)

clean:
	rm -f $(TARGET)
	rm -f $(DTARGET)
	rm -f *.o

.PHONY: all debug clean
