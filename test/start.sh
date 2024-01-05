g++ main.cpp

num=1

while [ $num -le 20 ]
do
    echo $num
    ./a.out < "$num".in > "$num".out
    num=$(( $num + 1 ))
done