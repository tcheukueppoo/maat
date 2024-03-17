#!/bin/sh

tmpa=`mktemp /tmp/tmp.XXXXXXXXX`
tmpb=`mktemp /tmp/tmp.XXXXXXXXX`

ls *.h | sed -e 's/\.h$//' >$tmpa
ls *.c | sed -e 's/\.c$//' >$tmpb

exclude=`diff $tmpa $tmpb | sed -ne 's/[><] //p' | while read f ; do test -e "$f.h" && printf '%s.h\n' "$f" || printf '%s\n' "$f.c" ; done`

{
   for hf in *.h ; do printf '%s\n' "$exclude" | grep -qF "$hf" || printf '%s\n%s\n' "$hf" "${hf%.h}.c" ; done 
   printf '%s\n' "$exclude"
} | parallel 'cat {}' \
  | enscript -2BrG -Ec -U2 -p - \
  | ps2pdf - out2.pdf

rm $tmpa $tmpb
