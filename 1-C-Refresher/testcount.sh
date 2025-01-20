#!/bin/bash

# Run the stringfun program with the -c option and a test string
output=$(./stringfun -r "Reversed sentences look very weird")

status=$?

# Check if the program exited successfully
if [ "$status" -ne 0 ]; then
    echo "Test failed: stringfun exited with status $status"
	     exit 1
		  fi

		  # Expected output
		  expected_output="Word Count: 8
		  Buffer:  [There should be eight words in this sentence......]"

		  # Verify the program output matches the expected output
		  if [ "$output" != "$expected_output" ]; then
		      echo "Test failed: output does not match expected output"
				    echo "Expected:"
					     echo "$expected_output"
						      echo "Got:"
								    echo "$output"
									     exit 1
										  fi

										  # Extract the word count from the output
										  word_count=$(echo "$output" | grep "Word Count" | awk '{print $3}')

										  # Calculate the length of the word count
										  word_count_length=${#word_count}

										  # Print the word count length for verification
										  echo "Test passed"
										  echo "Word Count Length: $word_count_length"
