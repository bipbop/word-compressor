PHP_ARG_ENABLE(wcompression, whether to enable wcompression support,
[  --enable-wcompression          Enable wcompression support])

if test "$PHP_WCOMPRESSION" != "no"; then
  AC_DEFINE(HAVE_WCOMPRESSION, 1, [ Have wcompression support ])
  PHP_NEW_EXTENSION(wcompression, wcompression.c, $ext_shared)
fi
