## -*- Makefile -*-
##
## User: phobos
## Time: 2016-11-11 02:22:13
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -O -m64
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_spine-agent=GNU-amd64-Linux


all: $(TARGETDIR_spine-agent)/spine-agent

## Target: spine-agent
CPPFLAGS_spine-agent = \
	-I/usr/include/mysql \
	-I/usr/include/libgtop-2.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/x86_64-linux-gnu/glib-2.0/include
OBJS_spine-agent =  \
	$(TARGETDIR_spine-agent)/apache.o \
	$(TARGETDIR_spine-agent)/core.o \
	$(TARGETDIR_spine-agent)/database.o \
	$(TARGETDIR_spine-agent)/main.o \
	$(TARGETDIR_spine-agent)/monitoring.o \
	$(TARGETDIR_spine-agent)/network.o \
	$(TARGETDIR_spine-agent)/sysconfigdata.o \
	$(TARGETDIR_spine-agent)/sysusers.o
USERLIBS_spine-agent = /usr/lib/x86_64-linux-gnu/libmysqlclient.so /usr/lib/x86_64-linux-gnu/libgtop-2.0.so $(SYSLIBS_spine)  
DEPLIBS_spine-agent =  
LDLIBS_spine-agent = $(USERLIBS_spine-agent)


# Link or archive
$(TARGETDIR_spine-agent)/spine-agent: $(TARGETDIR_spine-agent) $(OBJS_spine-agent) $(DEPLIBS_spine-agent)
	$(LINK.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ $(OBJS_spine-agent) $(LDLIBS_spine-agent)


# Compile source files into .o files
$(TARGETDIR_spine-agent)/apache.o: $(TARGETDIR_spine-agent) apache.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ apache.c

$(TARGETDIR_spine-agent)/core.o: $(TARGETDIR_spine-agent) core.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ core.c

$(TARGETDIR_spine-agent)/database.o: $(TARGETDIR_spine-agent) database.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ database.c

$(TARGETDIR_spine-agent)/main.o: $(TARGETDIR_spine-agent) main.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ main.c

$(TARGETDIR_spine-agent)/monitoring.o: $(TARGETDIR_spine-agent) monitoring.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ monitoring.c

$(TARGETDIR_spine-agent)/network.o: $(TARGETDIR_spine-agent) network.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ network.c

$(TARGETDIR_spine-agent)/sysconfigdata.o: $(TARGETDIR_spine-agent) sysconfigdata.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ sysconfigdata.c

$(TARGETDIR_spine-agent)/sysusers.o: $(TARGETDIR_spine-agent) sysusers.c
	$(COMPILE.c) $(CFLAGS_spine-agent) $(CPPFLAGS_spine-agent) -o $@ sysusers.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_spine-agent)/spine-agent \
		$(TARGETDIR_spine-agent)/apache.o \
		$(TARGETDIR_spine-agent)/core.o \
		$(TARGETDIR_spine-agent)/database.o \
		$(TARGETDIR_spine-agent)/main.o \
		$(TARGETDIR_spine-agent)/monitoring.o \
		$(TARGETDIR_spine-agent)/network.o \
		$(TARGETDIR_spine-agent)/sysconfigdata.o \
		$(TARGETDIR_spine-agent)/sysusers.o
	rm -f -r $(TARGETDIR_spine-agent)


# Create the target directory (if needed)
$(TARGETDIR_spine-agent):
	mkdir -p $(TARGETDIR_spine-agent)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

