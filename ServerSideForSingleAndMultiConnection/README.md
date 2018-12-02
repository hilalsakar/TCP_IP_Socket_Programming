# TCP/IP Socket Programming (Server Side)

This repo contains many simple examples of server side applications in TCP/IP programming. 

WARN: While these servers were being developed, there wasn't any client side code. Tests were completed using Telnet.

##### server.cpp
- This server allows only one connection. It sends a message to the client and waits until any response is received from the client. (The response should be one byte.)
##### serverAllowsMultiLetterResponse
- This server allows only one connection. It sends a message to the client and waits until any response is received from the client. (The response can be more than one byte.)
##### serverAllowsMultiClientConnection
- This server allows multiple connection. It sends a message to the clients and waits until any response is received from the client.