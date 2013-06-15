OBJDIR:=.
SYSROOT:=
TCPATH:=
AR:=$(TCPATH)ar
CC:=$(TCPATH)gcc #Compiler
EDL:=$(TCPATH)gcc #Linker
ARFLAGS:=rcs
CCFLAGS:=-Wall -g #Compiler options
EDLFLAGS:=-Wall -g #Linker options
SERVEREXE:=$(OBJDIR)/smdp-test #Binary name
CLIENTEXE:=$(OBJDIR)/smdp-client
LIB:=$(OBJDIR)/libsmdp.a
DEFINES:=NODEBUG #Preprocessor definitions
ECHO:=@echo
LOBJ:=$(OBJDIR)/smdp.o
SOBJ:=$(OBJDIR)/smdp-test.o
COBJ:=$(OBJDIR)/smdp-client.o

all: $(SERVEREXE) $(CLIENTEXE) lib

lib : $(LOBJ)
	@echo archiving $<
	$(AR) $(ARFLAGS) $(LIB) $(LOBJ)

$(OBJDIR) :
	test -d $(OBJDIR) || mkdir $(OBJDIR)

$(SERVEREXE): $(SOBJ) $(LOBJ)
	@echo building $<
	$(EDL) -o $(SERVEREXE) $(EDLFLAGS) $(SYSROOT) $(SOBJ) $(LOBJ)
	@echo done

$(CLIENTEXE): $(COBJ) $(LOBJ)
	@echo building $<
	$(EDL) -o $(CLIENTEXE) $(EDLFLAGS) $(SYSROOT) $(COBJ) $(LOBJ)
	@echo done

$(OBJDIR)/%.o : %.c *.h $(OBJDIR)
	@echo building $< ...
	$(CC) $(CCFLAGS) -c $(SYSROOT) -D $(DEFINES) -o $@ $<
	@echo done

clean:
	@echo -n cleaning repository...
	@rm -rf *.o
	@rm -rf *.so*
	@rm -rf $(OBJDIR)/*.o
	@rm -rf $(OBJDIR)/*.so*
	@rm -f *~
	@echo cleaned.
