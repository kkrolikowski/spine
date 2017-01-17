bin_PROGRAMS = spine-agent
spine_agent_CFLAGS = -Wall $(MYSQL_CONFIG_CFLAGS) $(GLIBTOP_CFLAGS)
spine_agent_CPPFLAGS=$(MYSQL_CONFIG_CPPFLAGS)
spine_agent_LDADD=$(MYSQL_CONFIG_LIBS) $(GLIBTOP_LIBS)
spine_agent_SOURCES = apache.c core.c database.c main.c monitoring.c network.c sysconfigdata.c sysusers.c
