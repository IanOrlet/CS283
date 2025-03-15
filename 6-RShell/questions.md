1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

It uses an EOF charater to know when to stop looping. It uses fixed length data sending, and waits until it recives the
correct amount of bytes to stop.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

It uses an character to mark the end of a command, such as a newline. If it is not handled correctly, it results in
garbage data and partial reads. 

3. Describe the general differences between stateful and stateless protocols.

stateful protcals are temporary, can be changed easily, and is used to store session data. Stateless protocols are not temporary, remain the same, and
are more faster than stateful protocals 


4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is good when speed is more important than accuracy. Video streaming and videogames use UDP because its faster, and
dont require a lot of accuracy. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

sockets are used by appilcations to us communicate to networks. They let appications send data to eachother using
function calls.
