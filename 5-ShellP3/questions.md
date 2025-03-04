1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

it uses waitpid() to wait until all child processes are done. if you didnt use waitpid(), then it could return before
all of its child processes are done, and it wouldnt work right.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

if you leave pipes open then it can cause a descriptor leak, which can prevent new pipes from being opened.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd modifies the current directory, so when called as a child, it modifys the child's directory, not the parents
directory. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

if you use dynamic memory allocation, then you could have as many piped commands as you want, however dynamic memory
allocation is always much harder to do, and if not done correctly introduces memory leaks. 
