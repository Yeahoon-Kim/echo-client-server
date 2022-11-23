COMMANDTARGET = all clean
DIRECTORY = server client

$(COMMANDTARGET): chatProgram.o $(DIRECTORY)
ifeq ($(MAKECMDGOALS), clean)
	@rm -f *.o
endif

debug: chatProgram-test.o $(DIRECTORY)

chatProgram.o: chatProgram.cpp

chatProgram-test.o: chatProgram.cpp
	g++ -DDEBUG -c -o $(@) $(^)

$(DIRECTORY):
	$(MAKE) -C $(@) $(MAKECMDGOALS)

.PHONY: $(COMMANDTARGET) $(DIRECTORY)
