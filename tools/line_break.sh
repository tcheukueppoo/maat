#!/bin/sh

perl -E 'local $/; $t = <> =~ s/\n+/ /gr; $t =~ s/\s+$//; say ((length((split " ", substr($t, pos($t) - 1, 2))[0]) == 2 ? scalar(pos($t) = pos($t) - (length($1) - length($l = $1 =~ s/\S+$//r)), $l) : $1) =~ s/^\s+//r =~ s/\s+$//r) while $t =~ /\G(.{1,'${1:-80}'})/g'
