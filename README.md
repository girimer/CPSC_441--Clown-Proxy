# CPSC_441--Clown-Proxy
Proxy handles http 1.0 and 1.1 requests to switch clown jpg images on requested resource with clown image. Also changes all occurrences of word "Happy" to "Silly"

CPSC 441: Assignment 1, Winter 2022
Submitted by Girimer Singh.

Program is written in c language on a windows machine and test at home using chrome browser. 
There are two files, proxy.h and proxy.c

To run the program:	
	Compile using command gcc proxy.c -lwsock32
	run with command ./a.exe

allow access if firewall warning shows.

Open the proxy setting for the machine. Enter proxy IP address - 127.0.0.1, Port 9000
Save the settings. 

Use the chrome browser to open the links. 

Testing:
Program can handle HTTP GET requests. All the below links were successful opened with modified information as listed in assignment 1.
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/assignment1.html
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test1.html
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test2.html
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test3.html
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test4.html
	http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test5.html
	http://pages.cpsc.ucalgary.ca/~carey/index.htm
There are 2 clown images that program will alternate on every request.

