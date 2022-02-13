
//CPSC 441, Winter 2022
//Assignment 1
//Submitted by: Girimer Singh
//Date submitted: Jan 28, 2022

#include "proxy.h"
# define Max_Size 70000

/* some of the code is from the CPSC 441 tutorials, modification and additions
were made to complete assignment 1 */

int image =1;		//global variable to switch between clown images

void check_happy(char *input){
		char* check_header = strstr(input, "Happy");	//search for word "Happy"
		if((check_header != NULL)){						//If pointer is not NULL, implies word Happy found in string
		while(check_header != NULL){
			strncpy(check_header, "Silly",5);			//Changing to Silly
			check_header = check_header+5;				//incrementing pointer to check if there are more words "Happy" in string
			check_header = strstr(input, "Happy");
		}
	}
}

char *switch_images(){
	if(image == 1){
		image =2;										//changing so that different images shows everytime
		return "/~carey/CPSC441/ass1/clown1.png";		//1st clown image link
	}

	if(image == 2){
		image =1;										//2nd clown image link
		return "/~carey/CPSC441/ass1/clown2.png";
	}
}

void check_header(char *input){
		char temp[Max_Size];
		char* check_header = strstr(input, ".jpg");		//checking GET request to check if image is requested by client
		if(check_header != NULL){						//if image is requested
			check_header = check_header +4 ;			//incrementing pointer to point at after image extension .jpg
			strcpy(temp, check_header);					// copying rest of data from input to temp to be concatenated later	
			memset(input, '\0', sizeof(input));			//reseting input string
			strcpy(input, "GET ");						//copying new data
			strcat(input, switch_images());
			strcat(input, temp);						//concatenation rest of data from original request
		}
}


void check_message (char *input){
	char store_input[Max_Size];							// copying and setting up temp variable to store and change original response
		memset(&store_input, '\0', sizeof(store_input));
		char temp[Max_Size];
		char temp2[Max_Size];
		strcpy(temp, input);	
		char* token = strtok(temp, "\n");				// creating token at new line

		while (token != NULL) {							//until the end of input string, keep creating tokens
			memset(temp2, '\0', sizeof(temp2));			
			strcpy(temp2, token);			
			char* check_image = strstr(temp2, "<img");	//checking for <img tag in response

			if(check_image != NULL){					
				char* img_begin = strstr(check_image, "src="); //if <img tag exist check where src is located
				char *img_end = strstr(img_begin, ".jpg");		//check the end of jpg image
				if(img_end != NULL){							//if .jpg is not foung then image is not of jpg format, ignore
					img_end = img_end +4;					
					char temp3[Max_Size];						
					strncpy(temp3, temp2, strlen(temp2)-strlen(img_begin)); //copying the imformation that comes before src= tag
					strcat (temp3, "src=");									//adding link to clown images
					strcat(temp3, "\"");
					strcat(temp3, switch_images());
					strcat (temp3,img_end);						// copying rest of the data after .jpg

					memset(temp2, '\0', sizeof(temp2));			//resetting variables
					strcpy(temp2, temp3);
					memset(temp3, '\0', sizeof(temp3));
				}
			}
			strcat(store_input, temp2);							//adding string to final output variable that will be copied later
			token = strtok(NULL, "\n");
			if (token != NULL){									// adding new line again
					strcat(store_input, "\n");
			}
		}
		strncpy (input, store_input, sizeof(store_input));		//copying modified response to original input string
}

int run_connection (int client_sock){						
	int recvStatus, server_sock, serverrecvStatus;				
	char client_message[Max_Size], server_message[Max_Size];

	struct sockaddr_in p_client;
	struct hostent *address;

	//Create server socket
	server_sock = socket(AF_INET , SOCK_STREAM , 0);				//creating a socket to act as server for client requests
	if (server_sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	memset(&client_message, '\0', sizeof(client_message));			//clearing client_message variable

	recvStatus = recv(client_sock, client_message, Max_Size, 0);	//Receive a message from client
	if (recvStatus==-1){
		printf("Error in receiving!");
		return 0;
	}
	
	check_header(client_message);						//method to check if header is a GET request for a image

	char copy_client_msg[Max_Size];							
	strcpy(copy_client_msg, client_message);
	char* hostname = strtok(copy_client_msg, "\n");		//extracting hostname from header, tokenizing line by new line
	hostname = strtok(NULL, " ");						// tokenizing line now by space
	memset(&hostname, '\0', sizeof(hostname));
	hostname = strtok(NULL, "\n");

	char hostname_ip[100];
	strcpy (hostname_ip, hostname);						//copying hostname to char[] to be passed to gethostbyname method
	hostname_ip[strlen(hostname)-1] = '\0';

	if ((address = gethostbyname(hostname_ip))== NULL){
		return 0;
	}

	char *ip;
	ip = inet_ntoa(*((struct in_addr*) address->h_addr_list[0])); //copying ip to char*

	p_client.sin_addr.s_addr = inet_addr(ip);					//setting up variable to connect to remote server
	p_client.sin_family = AF_INET;
	p_client.sin_port = htons(80);

	//Connect to remote server
	if (connect(server_sock , (struct sockaddr *)&p_client , sizeof(p_client)) < 0)
	{
		perror("connect failed. Error");
		return 0;
	}
	
	if(send(server_sock , client_message , sizeof(client_message),0)<0){ 		//Send message to server
		puts("Send message to server failed");
		return 0;
	}
	puts("waiting for response");		

	while(1){
		memset(&server_message, '\0', sizeof(server_message)); 					//Receive a message from server
        serverrecvStatus = recv(server_sock, server_message, Max_Size, 0);
        if (serverrecvStatus==-1){
            printf("Error in receiving!");
            break;
        }

		char* content_type = strstr(server_message, "Content-Type: text");			//checking response content type

		if(content_type != NULL){												// if reponce of text, check for images
		check_message(server_message);
		check_happy(server_message);
		}

		send(client_sock , server_message , serverrecvStatus,0);				//Send server message to client
		// printf("\nreply to client %s \n", server_message);
		if (serverrecvStatus==0){												//keeping reeciving data until all is not received 
            printf("all data received\n");
            break;
        }
	}
    close(client_sock);															//close sockets
    close(server_sock);
	return 1;
}


int main(int argc, char *argv[]){
	
	WSADATA wsa;											//for windows socket implementation
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}

	SOCKET socket_desc, client_sock, server_sock;	
	struct sockaddr_in server, p_client;
	
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);			//Create socket
	if (socket_desc == INVALID_SOCKET)
	{
		printf("Could not create socket\n");
	}
	puts("Socket created\n");
	
	server.sin_family = AF_INET;							//Prepare the sockaddr_in structure
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9000);
	
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server)); //Binding socket 
	if( bindStatus == -1){
		perror("Binding failed");
		return 1;
	}
	printf("Binding done.\n");

	listen(socket_desc, 3);								//Listen for requests

	while(1){
		client_sock = accept(socket_desc, NULL, NULL); 	//accept connection from an incoming client
		
		if (client_sock < 0){
			perror("Connection failed");
			return 1;
		}
		printf("Connection accepted\n");
		
		run_connection (client_sock);					//process the client request
	}
	return 0;
}
