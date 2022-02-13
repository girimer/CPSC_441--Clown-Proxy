
//CPSC 441, Winter 2022
//Assignment 1
//Submitted by: Girimer Singh
//Date submitted: Jan 28, 2022

#ifndef PROXY_H
#define PROXY_H

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/types.h.>
#include<winsock2.h>	//inet_addr
#include<winsock.h>	//inet_addr
#include<unistd.h>	//write

/*
Method checks for word "Happy" in input string, if word exists in string
then replaces all occurrances of "Happy" to "Silly"
*/
void check_happy(char *input);

/*
Method switched the links of clown images, everytime method is called
different clown link is returned
*/
char *switch_images();

/*
Method checks header of the client request for request of JPG image
Modifies header to substitute original request's link with clown image
*/
void check_header(char *input);

/*
Method checks for JPG images and changes them to clown images 
and return original input with modified image links
*/
void check_message (char *input);

/*
Methods created a socket to connect to remote server and process client requests
once the request is completed, closes the sockets.
return 0 for failure and 1 for success.
*/
int run_connection (int client_sock);

#endif