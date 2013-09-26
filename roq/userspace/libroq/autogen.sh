#! /bin/sh

set -x
# run libtoolize first, autoreconf version 2.63
# requires that
libtoolize --force --copy
autoreconf -i -f

