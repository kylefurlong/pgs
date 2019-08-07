#!/bin/bash
echo
echo "ent for urandom"
echo
../../ent/ent -b <(head -c $1 /dev/urandom)
echo
echo
echo "ent for pgs"
echo
../../ent/ent -b <(head -c $1 <(./bytes))
