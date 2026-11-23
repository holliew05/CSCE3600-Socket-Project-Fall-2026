# CSCE3600-Socket-Project

This is a project I was given in my Principles of System Programming class. The goal of this project was to make a server and client program, that could be run on different cells/machines and send messages to each other. This project used a UDP socket on the Internet domain. The client will send a PING signal to the server, which will then respond with PONG. This project reflects ~30% packet loss due to UDP. The client will wait for responses, and time-out after 1 second. 
