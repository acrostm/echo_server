#!/bin/bash

for i in {1..100}
do
   echo "This is message $i" | ./echo_client test & 
done