#!/bin/bash
# case only works with regex
echo '======= ;; ========='
# this is the same as c case with break
# when input is 2, it runs case 2 and done. 
# this is like
for i; do
    case $i in
        1) echo 1;;
        2) echo 2;;
        3) echo 3;;
        *) echo '*';;
    esac
done

echo '======= ;& =========='
# this is the same as c without break.
# input 2, run case 2 and run command under case 3. and because ;; after 3, it ends there. 
# output 2 3
for i; do
    case $i in
        1) echo 1;;
        2) echo 2;&
        3) echo 3;;
        *) echo '*';;
    esac
done

echo '======= ;;& =========='
#this is special, typically all cases should use ;;& to always test all the cases.
# input 2, run 2, then continue test all the cases following. doesn't stop at 3) because of ;; there. 
# output 2 * 
for i; do
    case $i in
        1) echo 1;;
        2) echo 2;;&
        3) echo 3;;
        *) echo '*';;
    esac
done

echo '=== going through all case conditions ==='
for i; do
    case $i in
        *1*) echo 1;;&
        *2*) echo 2;;&
        *3*) echo 3;;&
    esac
done
