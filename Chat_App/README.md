# Chat_App



## Program Overview

This is a chat program with the final objective of sending and receiving messages in a ring (network).
To do this, there are 3 main layors: Topologica, Encaminhamento e Chat. 
Topologica - Ensures the ring structure and communication between the nodes, as well as with the nodes server.
Encaminhamento - has the paths between the nodes.
Chat - Sends the messages.

## Files

Main.c - Runs the program. Receives the commands from the interface, and calls the functions to execute them.

Interface.c - With the help of the select function, it is allways ready to receive a command and send it to main.

protocolo_top.c - Has all the functions to secure the structure of the ring: Regists to the Server of nodes and can enter and leave a ring, as well as handleling the entry or leave of other nodes.

UDP_Client.c - Acts as a UDP Client to communicate with the nodes server. Sends messages and receives the response from the server.

TCP_Client.c - Acts as a TCP Client and sends TCP messages to other nodes to communicate with them.

TCP_Server.c - Acts as TCP Server. With the help of the select function, it is allways ready to receive messages from other nodes.

response_handler.c - analizes the UDP and TCP resopnses received and returns the neccessary information for the program to act accordingly to this responses content.

