#!/bin/bash
echo hello
dirs=$(find -L $1 -type l -mmin +7)
    for element in $dirs; do
        if $(test -h $element && !(test -e $element));
            then echo $element; fi
    done
echo ok
