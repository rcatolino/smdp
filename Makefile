CC=gcc #Compiler
EDL=gcc #Linker
CCFLAGS=-Wall -g #Compiler options
EDLFLAGS=-Wall -g #Linker options
SERVEREXE=smdp-test #Binary name
CLIENTEXE=smdp-client
DEFINES=NODEBUG #Preprocessor definitions
ECHO=@echo

LOBJ=smdp.o
SOBJ=smdp-test.o
COBJ=smdp-client.o

all: $(SERVEREXE) $(CLIENTEXE)


$(SERVEREXE): $(SOBJ) $(LOBJ)
	@echo building $<
	$(EDL) -o $(SERVEREXE) $(EDLFLAGS) $(SOBJ) $(LOBJ)
	@echo done

$(CLIENTEXE): $(COBJ) $(LOBJ)
	@echo building $<
	$(EDL) -o $(CLIENTEXE) $(EDLFLAGS) $(COBJ) $(LOBJ)
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
