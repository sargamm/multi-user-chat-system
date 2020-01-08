# multi-user-chat-system
Multi User Chat System using the linux IPC mechanism

SERVER:
first, the socket of the server is set up using the socket() function.
The Unix socket IPC address struct sockaddr_un is made and initialised and the path of the file where the messages are stored is also assigned to its attribute sun_path. unlink() is used to delete the socket file if it exists.Then bind() function is used to link the socket to an address in the unix domain. listen() is used to set an upper cap of the maximum no. Of clients that can be connected to the socket being used. Now, for connecting to the client, accept() is used, it returns another socket descriptor and sets its connection to the server to communicate.
Next,threads are created, where the receiving and sending of the messages from the client is done.  First I receive the id of the client I want to communicate with, if the message is to be sent to everyone, -1 is received. Following this,the message recieved from the client and is sent to the mentioned clients.

CLIENT:
Firstly, the socket of the client is setup using which it can communicate.The Unix socket IPC address struct sockaddr_un is made and initialised and the path of the file where the messages are stored is also assigned to its attribute sun_path. connect() sets up the connection with the socket. Messages are sent and received using send() and recv() functions.
