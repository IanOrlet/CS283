#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "remote - invalid command results in error" {
    IP=$(hostname -I | awk '{print $1}')
    run ./dsh -s &
    SERVER_PID=$!
    sleep 1
    run ./dsh -c -i "$IP" <<EOF
invalidcommand
EOF

    echo "Output: $output"
    echo "Exit Status: $status"
	 expected_output="cmdloopreturned-50"
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    [[ "$stripped_output" =~ "$expected_output" ]]
    [ "$status" -eq -0 ]
}

@test "remote - trying to connect to server that isnt open" {
	 pkill -9 dsh || true
    run ./dsh -c <<EOF
exit
EOF
	
	echo "Output: $output"
    echo "Exit Status: $status"
	expected_output="cmdloopreturned-52"
	 stripped_output=$(echo "$output" | tr -d '[:space:]')
	 [[ "$stripped_output" =~ "$expected_output" ]]
    # Assertions
    [ "$status" -eq 0 ]
}
@test "remote - opening a server on a port thats in use " {

	 run ./dsh -s & SERVER_PID=$!
	 sleep 1 
	 run ./dsh -s 
	 sleep 1	  
    echo "Output: $output"
    echo "Exit Status: $second_output"
	 stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cmdloopreturned-50"
	 [[ "$stripped_output" =~ "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "remote - piping on server works correctly" {
	 IP=$(hostname -I | awk '{print $1}')
    run ./dsh -s & SERVER_PID=$!
	 sleep 1
	 run ./dsh -c -i "$IP" <<EOF
ls  | grep dshlib.c
EOF

	
	 # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dshlib.c"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
	 echo "Processed Output: ${stripped_output}"
	 echo "Expected Output: ${expected_output}"
    # Check exact match
    [[ "$stripped_output" =~ "$expected_output" ]]

    # Assertions
    [ "$status" -eq 0 ]
}
@test "remote - built in command works correctly" {
    IP=$(hostname -I | awk '{print $1}')
    run ./dsh -s & SERVER_PID=$!
    sleep 1
    run ./dsh -c -i "$IP"<<EOF

cd bats	 
pwd
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output=$(pwd)

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Processed Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"


    # Check exact match
    [[ "$stripped_output" =~ "$expected_output" ]]

    # Assertions
    [ "$status" -eq 0 ]
}


@test "remote - built in command not working" {
    IP=$(hostname -I | awk '{print $1}')
    run ./dsh -s & SERVER_PID=$!
    sleep 1
    run ./dsh -c -i "$IP" <<EOF

cd fakefolder
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="error:failedtochangedirectory"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Processed Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"


    # Check exact match
    [[ "$stripped_output" =~ "$expected_output" ]]

    # Assertions
    [ "$status" -eq 0 ]
}
#@test "remote - stop server exits with OK_EXIT" {
#    IP=$(hostname -I | awk '{print $1}')
#    ./dsh -s > server_log.txt 2>&1 &
#    SERVER_PID=$!
#    sleep 1
#    run ./dsh -c -i "$IP" <<EOF
#stop-server
#EOF
#    OUTPUT=$(cat server_log.txt)
#    echo "Server Output: $OUTPUT"
	 #echo "Output: $output"
    #echo "Exit Status: $status"
    #expected_output_server="cmdloopreturned-7"
    #stripped_output_server=$(echo "$OUTPUT" | tr -d '[:space:]')
    #expected_output="cmdloopreturned0"
    #jstripped_output=$(echo "$output" | tr -d '[:space:]')

    #[[ "$stripped_output_server" =~ "$expected_output_server" ]]
    #[[ "$stripped_output" =~ "$expected_output" ]]
	 #j[ "$status" -eq 0 ]
#}
@test "remote - exit does not close the server" {
		 pkill -9 dsh || true
    IP=$(hostname -I | awk '{print $1}')
    PORT=9835
    ./dsh -s & 
    SERVER_PID=$!
    sleep 1  
    run ./dsh -c -i "$IP" <<EOF
exit
EOF
    sleep 1 
    run nc -z "$IP" "$PORT"
    [ "$status" -eq 0 ]

}
@test "local mode - more than 8 args results in an error " {

run ./dsh<<EOF
ls -a -a -a -a -a -a -a -a -a -a -a
EOF

    echo "Output: $output"
    echo "Exit Status: $status"
    expected_output="cmdloopreturned-2"
    stripped_output=$(echo "$output" | tr -d '[:space:]')

	 [[ "$stripped_output" =~ "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "local mode - no command" {

run ./dsh<<EOF
EOF

    echo "Output: $output"
    echo "Exit Status: $status"
    expected_output="cmdloopreturned0"
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    [[ "$stripped_output" =~ "$expected_output" ]]

}
@test "local mode - ok exit" {
pkill -9 dsh || true
run ./dsh<<EOF
exit
EOF

    echo "Output: $output"
    echo "Exit Status: $status"
    expected_output="cmdloopreturned-7"
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    [ "$status" -eq 249 ]
}


