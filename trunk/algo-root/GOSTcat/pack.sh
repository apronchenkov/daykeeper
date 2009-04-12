#!/bin/sh

MESSAGE=`cat`
ENCODE=`echo ${MESSAGE} | ./gcat | uuencode -m -`
IM=`echo ${ENCODE} | ./gimito`

echo ${IM}
echo ${ENCODE}

