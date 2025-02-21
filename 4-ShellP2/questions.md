1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > it lets you run commands more than once and it also lets you continue if there is an error. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > if the fork call fails then returns an error code. a fork call fails when it is less than 0 because that means it
	 > failed to create a child. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp finds the command to execute by using the value given by cmd._cmd_buffer, and it uses that to
	 > search in PATH for a valid command. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > It waits because it needs to run the command it was given. if it doesnt, then there is nothing to return until it
	 > is proccessed

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > WEXITSTATUS() is the exit code of the child. it is important because it tells the program if the child ran
	 > correctly or not. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > My program handles quoted arguments by turning every space outside of the argument into a quote, and using strtok
	 > and " as a delimiter. it was neccesarry because anything inside of quotes is a string, so it needs to keep its
	 > whitespaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > instead of using | as a delimiter, i used ". There were a lot of challanges because i am awful with string parsing
	 > and overcomplicated it way to much. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > signals are used to control processes. They are different from other IPCS because it is strictly async and does
	 > not transfer any data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > SIGKILL kills a process without letting it return. It is useful if a program is unresponsive. SIGTERM lets the
	 > program return and stop itself. SIGINT stops programs while they are running, and lets it return. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > If a program recives SIGSTOP then it will always be stopped. programs cannot ignore SIGSTOP. A typical use case
	 > would be to pause a program without killing the proccess. 
