/* 
 * server FTP program
 * By:
 * Suban Krishnamoorthy
 * Matthew Vaillancourt (solo group)
 *
 * HW2 Computer Networking - Fall 2020
 *
 * Program provides the functionality of the ftp protocol.
 * This functionallity is limited to the following commands:
 * 
 * 	user -	sends login request for user name <argument>
 * 	pass -	send password verification request to server. Used with user command.
 * 	quit -	quits application.
 * 	mkdir -	creates specified directory.
 * 	rmdir - deletes specified directory.
 * 	cd    - change to specified directory.
 * 	dele  - deletes specified file.
 *  	pwd   - returns working directory name.
 * 	ls    -	lists contents of directory.
 * 	stat  - displays file transfer mode and login status.
 *	help  - displays available commands.
 *	send  - transfers specified file from client to server.
 *	recv  - transfers specified file from server to client.
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_FTP_PORT 1994	//listen port for control connection
#define CLIENT_FTP_PORT 1995	//listen port for data connection

/* Error and OK codes */
#define OK 0
#define ER_INVALID_HOST_NAME -1
#define ER_CREATE_SOCKET_FAILED -2
#define ER_BIND_FAILED -3
#define ER_CONNECT_FAILED -4
#define ER_SEND_FAILED -5
#define ER_RECEIVE_FAILED -6
#define ER_ACCEPT_FAILED -7


/* Function prototypes */

int svcInitServer(int *s);
int sendMessage (int s, char *msg, int  msgSize);
int receiveMessage(int s, char *buffer, int  bufferSize, int *msgSize);
int clntConnect(char *serverName, int *s);

/* List of all global variables */

char userCmd[1024];	/* user typed ftp command line received from client */
char cmd[1024];		/* ftp command (without argument) extracted from userCmd */
char argument[1024];	/* argument (without ftp command) extracted from userCmd */
char replyMsg[2048];       /* buffer to send reply message to client */

char *userNames[] = {"user1", "user2", "user3", "user4", "user5"};		//list of user names
char *userPasswords[] = {"user1Pw", "user2Pw", "user3Pw", "user4Pw", "user5Pw"};//list of users passwords
int userLength = 5;	//number of users on system
int matchIndex = -1;	//mathc index for vaild use/pw combo

int transbufferSize = 100; //size of buffer in send/recv file operations

int IS_LOGGED_IN = 1;
int USER_LOG_STATUS = 0;	//user logged flag
/*
 * main
 *
 * Function to listen for connection request from client
 * Receive ftp command one at a time from client
 * Process received command
 * Send a reply message to the client after processing the command with staus of
 * performing (completing) the command
 * On receiving QUIT ftp command, send reply to client and then close all sockets
 *
 * Parameters
 * argc		- Count of number of arguments passed to main (input)
 * argv  	- Array of pointer to input parameters to main (input)
 *		   It is not required to pass any parameter to main
 *		   Can use it if needed.
 *
 * Return status
 *	0			- Successful execution until QUIT command from client 
 *	ER_ACCEPT_FAILED	- Accepting client connection request failed
 *	N			- Failed stauts, value of N depends on the command processed
 */

int main( 
	int argc, 
	char *argv[] 
	)
{
	/* List of local varibale */

	int msgSize;        /* Size of msg received in octets (bytes) */
	int listenSocket;   /* listening server ftp socket for client connect request */
	int ccSocket;		/* Control connection socket - to be used in all client communication */
	int status;

	int dcSocket;		/* Data connection socket*/
	/*
	 * NOTE: without \n at the end of format string in printf,
         * UNIX will buffer (not flush)
	 * output to display and you will not see it on monitor.
	*/
	printf("Started execution of server ftp\n");

	 /*initialize server ftp*/
	printf("Initialize ftp server\n");	/* changed text */

	status=svcInitServer(&listenSocket);
	if(status != 0)
	{
		printf("Exiting server ftp due to svcInitServer returned error\n");
		exit(status);
	}


	printf("ftp server is waiting to accept connection\n");

	/* wait until connection request comes from client ftp */
	ccSocket = accept(listenSocket, NULL, NULL);

	printf("Came out of accept() function \n");

	if(ccSocket < 0)
	{
		perror("cannot accept connection:");
		printf("Server ftp is terminating after closing listen socket.\n");
		close(listenSocket);  /* close listen socket */
		return (ER_ACCEPT_FAILED);  // error exist
	}

	printf("Connected to client, calling receiveMsg to get ftp cmd from client \n");


	/* Receive and process ftp commands from client until quit command.
 	 * On receiving quit command, send reply to client and 
         * then close the control connection socket "ccSocket". 
	 */
	do
	{
	    /* Receive client ftp commands until */
 	    status=receiveMessage(ccSocket, userCmd, sizeof(userCmd), &msgSize);
	    if(status < 0)
	    {
			printf("Receive message failed. Closing control connection \n");
			printf("Server ftp is terminating.\n");
			break;
	    }

/*
 * Starting Homework#2 program to process all ftp commandsmust be added here.
 * See Homework#2 for list of ftp commands to implement.
 */
	    /* Separate command and argument from userCmd */
		memset(replyMsg, '\0', 2048);	//clear replyMsg buffer
		memset(cmd, '\0', 1024);	//clear cmd buffer
		memset(argument, '\0', 1024);	//clear argument buffer

		char temp[1024];
		strcpy(temp, userCmd);			//store originol command

		char *ptr;
		ptr = strtok(temp, " ");		//extract commad from userCmd 

		if(ptr != NULL)
		{
			strcpy(cmd, ptr);		//copy ftp command into cmd buffer
		}
		else
		{
			strcpy(replyMsg, "100 Not vaild ftp command.\n");
		}
		
		ptr = strtok(NULL, " "); //get the argument, NULL gets next token from origional string
		if (ptr != NULL)
		{
			strcpy(argument, ptr); //copy ftp argument into argument array
 		}
		else
		{
			argument[0] = '\0';
		}

		if(strcmp(cmd, "user") == 0)
		{
			if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments\n");
			}
			else if(USER_LOG_STATUS != IS_LOGGED_IN)
			{
				matchIndex = -1;
				int i = 0;
				while(i < userLength)
				{
					if(userNames[i] != NULL && strcmp(userNames[i], argument) == 0)
					{
						matchIndex = i;
						break;
					}
					i++;
				}
				if(matchIndex >= 0)		//if user exists, ask for password
				{
					strcpy(replyMsg, "331 User name ok, need password.\n");
				}
				else
				{
					strcpy(replyMsg, "530-Login incorrect. 530 User does not exist.\n");
				}
			}
			else	// user already logged in
			{
				strcpy(replyMsg, "230 User logged in, proceed.\n");
			}
		}
		else if(strcmp(cmd, "pass") == 0)
		{
			if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments\n");
			}
			else if(matchIndex >= 0)		//if user found through 'user' cmd
			{
				if(strcmp(userPasswords[matchIndex], argument) == 0)	//compre user name to password
				{
					USER_LOG_STATUS = IS_LOGGED_IN;	//set user flag to successful login
					strcpy(replyMsg, "230 User logged in, proceed.\n");
				}
				else
				{
					strcpy(replyMsg, "530 Login incorrect.\n");
				}
			}
			else	// need valid user name
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
		}
		else if(strcmp(cmd, "stat") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) //If not logged in
			{
				strcpy(replyMsg, "Not logged in.\n");
				strcat(replyMsg, "File transfer mode: ASCII\n");
			}
			else
			{
				strcpy(replyMsg, "Logged in.\n");
				strcat(replyMsg, "File transfer mode: ASCII\n");
			}
		}
		else if(strcmp(cmd, "help") == 0)	//display avaiable commands to user
		{
			strcpy(replyMsg,
			"Commands are:\n\
			user    pass    quit    mkdir\n\
			rmdir   cd      dele    pwd\n\
			ls      stat    help    send\n\
			recv\n");
		}
		else if(strcmp(cmd, "quit") == 0)
		{
			strcpy(replyMsg, "130 Closing connection\n");
		}
		else if(strcmp(cmd, "mkdir") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments.\n");
			}
			else
			{
				status = system(userCmd);		//send make directory cmd to system				
				if(status == 0)
				{
					strcpy(replyMsg, "257 ");
					strcat(replyMsg, argument);
					strcat(replyMsg, " directory created.\n");
				}
				else
				{
					strcpy(replyMsg, "550-Requested action not taken\n550 directory not created.\n");
				}
			}
		}
		else if(strcmp(cmd, "rmdir") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments.\n");
			}
			else
			{
				status = system(userCmd);		//send remove directory cmd to system
				if(status == 0)
				{
					strcpy(replyMsg, "258 ");
					strcat(replyMsg, argument);
					strcat(replyMsg, " directory removed\n");
				}
				else
				{
					strcpy(replyMsg, "550-Requested action not taken\n550 directory not removed.\n");
				}
			}
		}
		else if(strcmp(cmd, "cd") == 0)			// change directory
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else
			{
				if(argument[0] == '\0')
				{
					strcpy(argument, "~");
				}
				
				status = chdir(argument);		//send change directory command to system
				if(status == 0)
				{
					strcpy(replyMsg, "250 Change directory completed.\n");
				}
				else
				{
					strcpy(replyMsg, "550-Requested action not taken.\n550 No such file or directory.\n");
				}
			}
		}
		else if(strcmp(cmd, "dele") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments.\n");
			}
			else
			{
				char tempCmd[1024];
				strcpy(tempCmd, "rm ");		//create unix cmd 'rm'
				strcat(tempCmd, argument);	//cat argument to unix cmd
				status = system(tempCmd);	//send remove command to server
				if(status == 0)
				{
					strcpy(replyMsg, "250 ");
					strcat(replyMsg, argument);
					strcat(replyMsg, " deleted\n");
				}
				else
				{
					strcpy(replyMsg, "550-Requested action not taken.\n550 ");
					strcat(replyMsg, argument);
					strcat(replyMsg, " not deleted\n");
				}
			}
		}
		else if(strcmp(cmd, "pwd") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else
			{
				char unixCmd[] = "pwd > tempFile";
				status = system(unixCmd);	//send cmd to system, creates temp file
				if(status == 0)
				{
					FILE *filePtr = fopen("tempFile", "r");	//open file in read mode
					if(filePtr != NULL)
					{
						int bytesRead = fread(replyMsg, sizeof(char), sizeof(replyMsg), filePtr);
						if(bytesRead <= 0)			//temp file read success
						{
							strcpy(replyMsg, "550 Error reading local file.\n");
						}
						fclose(filePtr);			//close temp file
					}
					else							//temp file not opened
					{
						strcpy(replyMsg, "550 Error opening local file.\n");
					}	
				}
				else								//system cmd failed
				{
					strcpy(replyMsg, "550 Requested action not taken. system command failed\n");
				}	
				
				if(system("rm tempFile") != 0)		//remove temp file
				{
					strcat(replyMsg, "552 Error deleting temp file\n");
				}
			}
		}
		else if(strcmp(cmd, "ls") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else 
			{
				
				char unixCmd[] = " > tempFile";
				strcat(userCmd, unixCmd);
				status = system(userCmd);			//send userCmd to system, creates temp file
				if(status == 0)
				{
					FILE *filePtr = fopen("tempFile", "r");	//open file in read mode
					if(filePtr != NULL)
					{
						int bytesRead = fread(replyMsg, sizeof(char), sizeof(replyMsg), filePtr);
						if(bytesRead < 0)
						{
							strcpy(replyMsg, "550 Error reading local file.\n");
						}
						close(filePtr);	//close temp file
					}
					else
					{
						strcpy(replyMsg, "550 Error opening local file.\n");
					}
				}
				else
				{
					strcpy(replyMsg, "550 Requested action not taken. system command failed\n");
				}
				if(system("rm tempFile") < 0)		//remove temp file
				{
					strcat(replyMsg, "552 Error deleting temp file\n");
				}
			}
		}
		else if(strcmp(cmd, "recv") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments.\n");
			}
			else
			{
				status = clntConnect("10.3.200.17", &dcSocket);	//connect to client
				if(status != 0)
				{
					printf("Data connection to client failed.\n");
					strcpy(replyMsg, "426 Connection closed; transfer aborted.\n");
				}
				else
				{
					FILE *fptr = fopen(argument, "r");	//opens local file in read mode
					if(fptr != NULL)
					{					
							while( ! feof(fptr)) //if not end of file read from file
							{
								char temp[transbufferSize];
							
								int bytesRead = fread(temp, sizeof(char), transbufferSize, fptr);	//read from local file
								if(bytesRead == 0)
								{
									strcpy(replyMsg, "550 File is empty.\n");
									break;
								}
								else if(bytesRead < 0)
								{
									strcpy(replyMsg, "550 Error reading local file.\n");
									break;
								}
								else
								{
									sendMessage(dcSocket, temp, bytesRead); // send file content to client
								}
							}
							
							if(feof(fptr) != 0) //if end of file was successfully reached
							{
								strcpy(replyMsg, "226-Closing data connection, transfer successful.\n");
							}
							fclose(fptr); 	//close local file
					}
					else
					{
						strcpy(replyMsg, "550 Requested action not taken. Could not open file.\n");
					}					
				}
				printf("Closing data connection.\n");
				close(dcSocket);//close data socket
			}
		}
		else if(strcmp(cmd, "send") == 0)
		{ //receive file content from server and write to local file
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else if(argument[0] == '\0')
			{
				strcpy(replyMsg, "501 Invlid number of arguments.\n");
			}
			else
			{
				status = clntConnect("10.3.200.17", &dcSocket);	//connect to client
				if(status != 0)
				{
					printf("Data connection to client failed.\n");
					strcpy(replyMsg, "426 Connection closed; transfer aborted.\n");	
				}
				else
				{
					FILE *fptr = fopen(argument, "w"); //open local file in write mode
					if(fptr != NULL)
					{
						int bytesReceived;
						do
						{
							char temp[transbufferSize];
							bytesReceived = -1;		//reset bytesReceived
							receiveMessage(dcSocket, temp, transbufferSize, &bytesReceived);	//receive data from client
							if(bytesReceived > 0)	//receive data success
							{
								int bytesWritten = fwrite(temp, sizeof(char), bytesReceived, fptr);	//write to file
								if(bytesWritten < 0)
								{
									strcpy(replyMsg, "550 Error writing local file.\n");
									break;
								}
							}
						}
						while(bytesReceived > 0);
						
						if(bytesReceived == 0)
						{
							strcpy(replyMsg, "226-Closing data connection, file transfer successful\n");
						}
						fclose(fptr);		//close file	
					}
					else
					{
						strcpy(replyMsg, "550 Requested action not taken. could not open file.\n");
					}
				}
				printf("Closing data connection.\n");
				close(dcSocket);	//close data connection
			}
		}
		else if(strcmp(cmd, "mode") == 0 || strcmp(cmd, "syst") == 0 || strcmp(cmd, "noop") == 0 ||strcmp(cmd, "pasv") == 0)
		{
			if(USER_LOG_STATUS != IS_LOGGED_IN) // check if user has successfully logged in
			{
				strcpy(replyMsg, "530 Not logged in.\n");
			}
			else
			{
				strcpy(replyMsg, "202 Command not implemented.\n");
			}
		}
		else
		{
			strcpy(replyMsg, "500 Unrecognized command.\n");
		}

	    /*
 	     * ftp server sends only one reply message to the client for 
	     * each command received in this implementation.
	     */
	    status=sendMessage(ccSocket,replyMsg,strlen(replyMsg) + 1);	/* Added 1 to include NULL character in */
				/* the reply string strlen does not count NULL character */
	    if(status < 0)
	    {
			break;  /* exit while loop */
	    }
	}
	while(strcmp(cmd, "quit") != 0);

	printf("Closing control connection socket.\n");
	close (ccSocket);  /* Close client control connection socket */

	printf("Closing listen socket.\n");
	close(listenSocket);  /*close listen socket */

	printf("Exiting from server ftp main.\n");

	return (status);
}


/*
 * svcInitServer
 *
 * Function to create a socket and to listen for connection request from client
 *    using the created listen socket.
 *
 * Parameters
 * s		- Socket to listen for connection request (output)
 *
 * Return status
 *	OK			- Successfully created listen socket and listening
 *	ER_CREATE_SOCKET_FAILED	- socket creation failed
 */

int svcInitServer (
	int *s 		/*Listen socket number returned from this function */
	)
{
	int sock;
	struct sockaddr_in svcAddr;
	int qlen;

	/*create a socket endpoint */
	if( (sock=socket(AF_INET, SOCK_STREAM,0)) <0)
	{
		perror("cannot create socket");
		return(ER_CREATE_SOCKET_FAILED);
	}

	/*initialize memory of svcAddr structure to zero. */
	memset((char *)&svcAddr,0, sizeof(svcAddr));

	/* initialize svcAddr to have server IP address and server listen port#. */
	svcAddr.sin_family = AF_INET;
	svcAddr.sin_addr.s_addr=htonl(INADDR_ANY);  /* server IP address */
	svcAddr.sin_port=htons(SERVER_FTP_PORT);    /* server listen port # */

	/* bind (associate) the listen socket number with server IP and port#.
	 * bind is a socket interface function 
	 */
	if(bind(sock,(struct sockaddr *)&svcAddr,sizeof(svcAddr))<0)
	{
		perror("cannot bind");
		close(sock);
		return(ER_BIND_FAILED);	/* bind failed */
	}

	/* 
	 * Set listen queue length to 1 outstanding connection request.
	 * This allows 1 outstanding connect request from client to wait
	 * while processing current connection request, which takes time.
	 * It prevents connection request to fail and client to think server is down
	 * when in fact server is running and busy processing connection request.
	 */
	qlen=1; 


	/* 
	 * Listen for connection request to come from client ftp.
	 * This is a non-blocking socket interface function call, 
	 * meaning, server ftp execution does not block by the 'listen' funcgtion call.
	 * Call returns right away so that server can do whatever it wants.
	 * The TCP transport layer will continuously listen for request and
	 * accept it on behalf of server ftp when the connection requests comes.
	 */

	listen(sock,qlen);  /* socket interface function call */

	/* Store listen socket number to be returned in output parameter 's' */
	*s=sock;

	return(OK); /*successful return */
}


/*
 * sendMessage
 *
 * Function to send specified number of octet (bytes) to client ftp
 *
 * Parameters
 * s		- Socket to be used to send msg to client (input)
 * msg  	- Pointer to character arrary containing msg to be sent (input)
 * msgSize	- Number of bytes, including NULL, in the msg to be sent to client (input)
 *
 * Return status
 *	OK		- Msg successfully sent
 *	ER_SEND_FAILED	- Sending msg failed
 */

int sendMessage(
	int    s,	/* socket to be used to send msg to client */
	char   *msg, 	/* buffer having the message data */
	int    msgSize 	/* size of the message/data in bytes */
	)
{
	int i;


	/* Print the message to be sent byte by byte as character */
	for(i=0; i<msgSize; i++)
	{
		printf("%c",msg[i]);
	}
	printf("\n");

	if((send(s, msg, msgSize, 0)) < 0) /* socket interface call to transmit */
	{
		perror("unable to send ");
		return(ER_SEND_FAILED);
	}

	return(OK); /* successful send */
}


/*
 * receiveMessage
 *
 * Function to receive message from client ftp
 *
 * Parameters
 * s		- Socket to be used to receive msg from client (input)
 * buffer  	- Pointer to character arrary to store received msg (input/output)
 * bufferSize	- Maximum size of the array, "buffer" in octent/byte (input)
 *		    This is the maximum number of bytes that will be stored in buffer
 * msgSize	- Actual # of bytes received and stored in buffer in octet/byes (output)
 *
 * Return status
 *	OK			- Msg successfully received
 *	R_RECEIVE_FAILED	- Receiving msg failed
 */


int receiveMessage (
	int s, 		/* socket */
	char *buffer, 	/* buffer to store received msg */
	int bufferSize, /* how large the buffer is in octet */
	int *msgSize 	/* size of the received msg in octet */
	)
{
	int i;

	*msgSize=recv(s,buffer,bufferSize,0); /* socket interface call to receive msg */

	if(*msgSize<0)
	{
		perror("unable to receive");
		return(ER_RECEIVE_FAILED);
	}

	/* Print the received msg byte by byte */
	for(i=0;i<*msgSize;i++)
	{
		printf("%c", buffer[i]);
	}
	printf("\n");

	return (OK);
}


/*
 * clntConnect
 *
 * Function to create a socket, bind local client IP address and port to the socket
 * and connect to the server
 *
 * Parameters
 * serverName	- IP address of server in dot notation (input)
 * s		- Control connection socket number (output)
 *
 * Return status
 *	OK			- Successfully connected to the server
 *	ER_INVALID_HOST_NAME	- Invalid server name
 *	ER_CREATE_SOCKET_FAILED	- Cannot create socket
 *	ER_BIND_FAILED		- bind failed
 *	ER_CONNECT_FAILED	- connect failed
 */


int clntConnect (
	char *serverName, /* server IP address in dot notation (input) */
	int *s 		  /* control connection socket number (output) */
	)
{
	int sock;	/* local variable to keep socket number */

	struct sockaddr_in clientAddress;  	/* local client IP address */
	struct sockaddr_in serverAddress;	/* server IP address */
	struct hostent	   *serverIPstructure;	/* host entry having server IP address in binary */


	/* Get IP address os server in binary from server name (IP in dot natation) */
	if((serverIPstructure = gethostbyname(serverName)) == NULL)
	{
		printf("%s is unknown server. \n", serverName);
		return (ER_INVALID_HOST_NAME);  /* error return */
	}

	/* Create control connection socket */
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cannot create socket ");
		return (ER_CREATE_SOCKET_FAILED);	/* error return */
	}

	/* initialize client address structure memory to zero */
	memset((char *) &clientAddress, 0, sizeof(clientAddress));

	/* Set local client IP address, and port in the address structure */
	clientAddress.sin_family = AF_INET;	/* Internet protocol family */
	clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY is 0, which means */
						 /* let the system fill client IP address */
	clientAddress.sin_port = 0;  /* With port set to 0, system will allocate a free port */
			  /* from 1024 to (64K -1) */

	/* Associate the socket with local client IP address and port */
	if(bind(sock,(struct sockaddr *)&clientAddress,sizeof(clientAddress))<0)
	{
		perror("cannot bind");
		close(sock);
		return(ER_BIND_FAILED);	/* bind failed */
	}


	/* Initialize serverAddress memory to 0 */
	memset((char *) &serverAddress, 0, sizeof(serverAddress));

	/* Set ftp server ftp address in serverAddress */
	serverAddress.sin_family = AF_INET;
	memcpy((char *) &serverAddress.sin_addr, serverIPstructure->h_addr, 
			serverIPstructure->h_length);
	serverAddress.sin_port = htons(CLIENT_FTP_PORT);

	/* Connect to the client */
	if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("Cannot connect to client ");
		close (sock); 	/* close the control connection socket */
		return(ER_CONNECT_FAILED);  	/* error return */
	}


	/* Store listen socket number to be returned in output parameter 's' */
	*s=sock;

	return(OK); /* successful return */
}  // end of clntConnect() */


