1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > fgets() reads until the end of the line or a null byte. it also stops to many bytes from being read. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > it can handle larger buffers. if you use a fixed size array, you can spend 30 minutes trying to figure out why your output
	 > cuts off a single character. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > if you trim all of the spaces it becomes easier to parse them out later.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > redirection is either inputing or outputing the stdin and stdout to another file. it could be used to provide
	 > commands to the shell, export error logs from the shell, or append all of the outputs to the shell into a text
	 > file. _

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > piping sends the inputs and outputs to other commands, redirecting sends the inputs and outputs to files. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > every program returns something when its done, so i needes to be kept seperate so you are not overwriting data put
	 > into stdout by your program

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > redirect them to a text file to make an error log. 
