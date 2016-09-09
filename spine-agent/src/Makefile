## -*- Makefile -*-
##
## User: phobos
## Time: 2016-09-09 23:52:56
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -O -m64
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_spine=GNU-amd64-Linux


all: $(TARGETDIR_spine)/spine

## Target: spine
CPPFLAGS_spine = \
	-I/usr/include/mysql
OBJS_spine =  \
	$(TARGETDIR_spine)/apache.o \
	$(TARGETDIR_spine)/core.o \
	$(TARGETDIR_spine)/database.o \
	$(TARGETDIR_spine)/main.o \
	$(TARGETDIR_spine)/monitoring.o \
	$(TARGETDIR_spine)/network.o \
	$(TARGETDIR_spine)/sysconfigdata.o
USERLIBS_spine = /usr/lib/x86_64-linux-gnu/libmysqlclient.so $(SYSLIBS_spine) 
DEPLIBS_spine =   
LDLIBS_spine = $(USERLIBS_spine)


# Link or archive
$(TARGETDIR_spine)/spine: $(TARGETDIR_spine) $(OBJS_spine) $(DEPLIBS_spine)
	$(LINK.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ $(OBJS_spine) $(LDLIBS_spine)


# Compile source files into .o files
$(TARGETDIR_spine)/apache.o: $(TARGETDIR_spine) apache.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ apache.c

$(TARGETDIR_spine)/core.o: $(TARGETDIR_spine) core.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ core.c

$(TARGETDIR_spine)/database.o: $(TARGETDIR_spine) database.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ database.c

$(TARGETDIR_spine)/main.o: $(TARGETDIR_spine) main.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ main.c

$(TARGETDIR_spine)/monitoring.o: $(TARGETDIR_spine) monitoring.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ monitoring.c

$(TARGETDIR_spine)/network.o: $(TARGETDIR_spine) network.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ network.c

$(TARGETDIR_spine)/sysconfigdata.o: $(TARGETDIR_spine) sysconfigdata.c
	$(COMPILE.c) $(CFLAGS_spine) $(CPPFLAGS_spine) -o $@ sysconfigdata.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_spine)/spine \
		$(TARGETDIR_spine)/apache.o \
		$(TARGETDIR_spine)/core.o \
		$(TARGETDIR_spine)/database.o \
		$(TARGETDIR_spine)/main.o \
		$(TARGETDIR_spine)/monitoring.o \
		$(TARGETDIR_spine)/network.o \
		$(TARGETDIR_spine)/sysconfigdata.o
	rm -f -r $(TARGETDIR_spine)


# Create the target directory (if needed)
$(TARGETDIR_spine):
	mkdir -p $(TARGETDIR_spine)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

