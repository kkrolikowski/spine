bin_PROGRAMS = spine-agent
spine_agent_SUBDIRS = configdata systemsetup
spine_agent_CFLAGS = -Wall $(MYSQL_CONFIG_CFLAGS) $(GLIBTOP_CFLAGS)
spine_agent_CPPFLAGS=$(MYSQL_CONFIG_CPPFLAGS)
spine_agent_LDADD=$(MYSQL_CONFIG_LIBS) $(GLIBTOP_LIBS)
spine_agent_SOURCES = scopes/apache.c core.c database.c main.c monitoring.c network.c sysconfigdata.c scopes/sysusers.c configdata/buildpkg.c configdata/dbdata.c configdata/extractpkg.c toolkit.c
