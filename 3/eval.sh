#!/bin/bash

# Specify the executable name
EXECUTABLE="./a.out"

# Specify the input for your program
INPUT=13

# Specify the output file to store execution times
OUTPUT_FILE="execution_times.txt"

# Loop through process sizes from 2 to 20
for PROC_SIZE in {2..20}; do
    # Run the program using mpirun
    mpirun -np $PROC_SIZE --use-hwthread-cpus --oversubscribe $EXECUTABLE <<<$INPUT

    # Capture the execution time and append it to the output file
    echo "Process Size $PROC_SIZE: $(mpirun -np $PROC_SIZE --use-hwthread-cpus --oversubscribe $EXECUTABLE <<<$INPUT)" >> $OUTPUT_FILE
done

echo "Execution times have been stored in $OUTPUT_FILE"
