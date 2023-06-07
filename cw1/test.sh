# ! /usr/bin/bash

touch ans.txt
for ((i = 1902; i <= 2037; i++)) do
     echo ${i} | ./4 >> ans.txt
   # echo ${i}
done
