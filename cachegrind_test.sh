valgrind -q --tool=cachegrind --cachegrind-out-file=temp.cgout ./a.out $1 $2 > /dev/null
cg_annotate temp.cgout | egrep 'Command|seqAccess|function'
./a.out $1 $2 | grep chunk
