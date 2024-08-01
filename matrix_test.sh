############## Important ##############
# Make sure to run this script after giving it
# execution rights in your terminal. This is done by
# `chmod +x matrix_test.sh`
# The script can be run using the command
# `./matrix_test.sh [num_thread]`
############## Important ############## 
# 
# I recommend num_thread = 32,,, seems to be the 
# sweet spot. There seems to be overhead in creating
# threads and the fact that there will be cases for mutex 
# locks to occur, and more the threads, higher the chance.
# However, when num_threads > num_rows in resultant matrix,
# the number of threads created equates that of number 
# of rows.


#!/bin/bash

# Define the number of test cases to generate
num_tests=10

# Define the init size of the square matrices to use
matrix_size=2

# Read the number of threads
if (($# < 1 ))
then 
    echo "There can only be one argument: [num_thread]"
    exit
elif (($# > 1 ))
then 
    echo "There can only be one argument: [num_thread]"
    exit
else
    num_threads=$1
fi

####### Run this for the verifier to work #######
     ### `sudo apt-get install libgsl-dev` ###
##### Run the above for the verifier to work #####

gcc -Wall -o matrix_multiply_verify matmul_ver.c -lgsl -lgslcblas
gcc -Wall -o matrix_multiply matmul.c -lpthread

# Loop through the number of test cases to generate
for (( i=1; i<=num_tests; i++ ))
do
    # Generate a random file name for the input matrix
    input_file_A="matrixA_$i.txt"
    input_file_B="matrixB_$i.txt"

    # Generate a random file name for the output matrix
    output_file1="matC_thread$i.txt"
    output_file2="matC_ver$i.txt"

    # Generate a random input matrix in a file
    for ((idx=0;idx<matrix_size;idx++))
    do
        for ((jdx=1;jdx<=matrix_size;jdx++))
        do
            if [ $jdx == $matrix_size ]
            then
                echo "$((RANDOM%8192-4096))"
            else
                echo -n "$((RANDOM%8192-4096)) "
            fi
        done
        # echo ""
    done >> $input_file_A

    for ((idx=0;idx<matrix_size;idx++))
    do
        for ((jdx=1;jdx<=matrix_size;jdx++))
        do
            if [ $jdx == $matrix_size ]
            then
                echo "$((RANDOM%8192-4096))"
            else
                echo -n "$((RANDOM%8192-4096)) "
            fi
        done
        # echo ""
    done >> $input_file_B

    # Run programs to multiply the input matrices
    ./matrix_multiply $input_file_A $input_file_B $output_file1 $num_threads
    ./matrix_multiply_verify $input_file_A $input_file_B $output_file2

    # Verify that the output of my program is the same as the gsl implementation
    # diff -q $output_file1 $output_file2 >> /dev/null
    if cmp -s "$output_file1" "$output_file2"
    then
        echo "Test case $i passed"
    else
        echo "Test case $i failed"
    fi

    # increase size of matrix
    matrix_size=$((matrix_size * 2))
    
done

# Delete the test files
rm matrixA_* matrixB_* matC_thread* matC_ver*