CC=gcc #Compiler
EDL=gcc #Linker
CCFLAGS=-Wall -g #Compiler options
EDLFLAGS=-Wall -g #Linker options
EXE=smdp-test #Binary name
DEFINES=NODEBUG #Preprocessor definitions
ECHO=@echo

EOBJ=smdp.o smdp-test.o

$(EXE): $(EOBJ)
	@echo building $<
	$(EDL) -o $(EXE) $(EDLFLAGS) $(EOBJ)
	@echo done

%.o : %.c *.h
	@echo building $< ...
	$(CC) $(CCFLAGS) -c -D $(DEFINES) $<
	@echo done

clean:
	@echo -n cleaning repository...
	@rm -rf *.o
	@rm -rf *.so*
	@echo cleaned.
