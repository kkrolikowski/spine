#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([spine-agent], [0.0.1])
AC_CONFIG_SRCDIR([src/sysconfigdata.c])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/statvfs.h sys/time.h unistd.h])


AC_MSG_CHECKING([for mysql_config include])
MYSQL_INCLUDE=`mysql_config --include`
AS_IF([ test "$?" -eq 0 ],[CPPFLAGS="$MYSQL_INCLUDE $CPPFLAGS"],)
AC_MSG_RESULT([$MYSQL_INCLUDE])

AC_MSG_CHECKING([for mysql_config pkglibdir])
MYSQL_LIBDIR=`mysql_config --variable=pkglibdir`
AS_IF([ test "$?" -eq 0 ],[LDFLAGS="-L$MYSQL_LIBDIR $LDFLAGS"],)
AC_MSG_RESULT([$MYSQL_LIBDIR])

AC_CHECKING([for MySQL library])
AC_CHECK_LIB(mysqlclient, mysql_real_connect, [], [AC_MSG_ERROR([MySQL client library not found])])

AC_CHECKING([for MySQL header files])
AC_CHECK_HEADERS([mysql.h mysql/mysql.h mysql.h], [break])
if test "$ac_cv_header_mysql_mysql_h" != "yes" -a "$ac_cv_header_mysql_h" != "yes"; then
    AC_MSG_ERROR([MySQL headers not found])
fi
PKG_CHECK_MODULES(GLIBTOP, libgtop-2.0)
AC_SUBST(GLIBTOP_CFLAGS)
AC_SUBST(GLIBTOP_LIBS)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([acl bzero gethostbyname gethostname inet_ntoa memset mkdir rmdir select socket strchr strstr sysinfo uname])
AC_DEFUN([CHECK_MYSQL_LIB],
[
AC_CHECK_PROGS(
    MYSQL_CONFIG,
    mysql_config
)

  #In case it fails to find pthread then exit configure 
  if test "x${MYSQL_CONFIG}" != xmysql_config; then
    echo "------------------------------------------"
    echo " The mysql library and header file is      "
    echo " required to build this project. Stopping "
    echo " Check 'config.log' for more information. "
    echo "------------------------------------------"
    (exit 1); exit 1;
  else
    MYSQL_CONFIG_CFLAGS=`$MYSQL_CONFIG --cflags`
    MYSQL_CONFIG_CPPFLAGS=`$MYSQL_CONFIG --include`
    MYSQL_CONFIG_LIBS=`$MYSQL_CONFIG --libs`
    AC_SUBST([MYSQL_CONFIG_CFLAGS])
    AC_SUBST([MYSQL_CONFIG_CPPFLAGS])
    AC_SUBST([MYSQL_CONFIG_LIBS])
  fi
])
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 src/Makefile])
AC_OUTPUT
