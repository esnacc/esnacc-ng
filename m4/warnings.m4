# -*- autoconf -*-

dnl Tries to enable -Wall -Wextra on systems which support it.
AC_DEFUN([ESNACC_CHECK_WARNINGS],
        [AC_LANG_PUSH([C])
        AC_MSG_CHECKING(for C-compiler all-warnings support with -Wall)
        old_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS -Wall"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
          AC_MSG_RESULT(yes),
          AC_MSG_RESULT(no)
          CFLAGS="$old_CFLAGS")

        AC_MSG_CHECKING(for C-compiler extra-warnings support with -Wextra)
        old_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS -Wextra"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
          AC_MSG_RESULT(yes),
          AC_MSG_RESULT(no)
          CFLAGS="$old_CFLAGS")
        AC_LANG_POP()

        AC_LANG_PUSH([C++])
        AC_MSG_CHECKING(for C++-compiler all-warnings support with -Wall)
        old_CXXFLAGS="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS -Wall"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
          AC_MSG_RESULT(yes),
          AC_MSG_RESULT(no)
        CXXFLAGS="$old_CXXFLAGS")

        AC_MSG_CHECKING(for C++-compiler extra-warnings support with -Wextra)
        old_CXXFLAGS="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS -Wextra"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
          AC_MSG_RESULT(yes),
          AC_MSG_RESULT(no)
          CXXFLAGS="$old_CXXFLAGS")
        AC_LANG_POP()
])
