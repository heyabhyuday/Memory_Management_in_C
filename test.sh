#!/bin/bash

# can change this depending on the accuracy you require
TESTCASES=1000


SUM=0
for (( c=0; c<$TESTCASES; c++))
do
    RESULT=$(./memMan -f "random/input1.txt" 65536 | tail -1)
    TIME=${RESULT:(-8)}
    SUM=$(awk '{print $1+$2}' <<< "${TIME} ${SUM}")
done
echo "Average allocation time (in milliseconds) for 256 processes using FCFS algorithm:"
echo $(awk '{print $1/$2}' <<< "${SUM} ${TESTCASES}")


SUM=0
for (( c=0; c<$TESTCASES; c++))
do
    RESULT=$(./memMan -s "random/input2.txt" 65536 | tail -1)
    TIME=${RESULT:(-8)}
    SUM=$(awk '{print $1+$2}' <<< "${TIME} ${SUM}")
done
echo "Average allocation time (in milliseconds) for 256 processes using SJSF algorithm:"
echo $(awk '{print $1/$2}' <<< "${SUM} ${TESTCASES}")


SUM=0
for (( c=0; c<$TESTCASES; c++))
do
    RESULT=$(./memMan -p "random/input3.txt" 65536 | tail -1)
    TIME=${RESULT:(-8)}
    SUM=$(awk '{print $1+$2}' <<< "${TIME} ${SUM}")
done
echo "Average allocation time (in milliseconds) for 256 processes using priority algorithm:"
echo $(awk '{print $1/$2}' <<< "${SUM} ${TESTCASES}")


SUM=0
for (( c=0; c<$TESTCASES; c++))
do
    RESULT=$(./memMan -S "random/input4.txt" 65536 | tail -1)
    TIME=${RESULT:(-8)}
    SUM=$(awk '{print $1+$2}' <<< "${TIME} ${SUM}")
done
echo "Average allocation time (in milliseconds) for 256 processes using segmentation algorithm:"
echo $(awk '{print $1/$2}' <<< "${SUM} ${TESTCASES}")


echo ;


echo "Edge Test Cases:";
echo ;
sleep 1

echo "TestCase 1:"
sleep 1.5
./memMan -f "test/small_input1.txt" 1024
echo ;

echo "TestCase 2:"
sleep 1.5
./memMan -s "test/small_input2.txt" 1024
echo ;

echo "TestCase 3:"
sleep 1.5
./memMan -p "test/small_input3.txt" 1024
echo ;

echo "TestCase 4:"
sleep 1.5
./memMan -Sv "test/small_input4a.txt" 1024
sleep 0.5
./memMan -Sv "test/small_input4b.txt" 1024
echo ;

# echo "TestCase 5:"
# sleep 1.5
# ./memMan -Sv "test/small_input5.txt" 1024
# echo ;

# echo "TestCase 6:"
# sleep 1.5
# ./memMan -Sv "test/small_input6.txt" 1024
# echo ;