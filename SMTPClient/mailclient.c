#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define SIZE 4096 

int checkFormat(int n, char line[n]){
	int j = 0;
	int charb = 0;
	int chara = 0;
	int sym = 0;
				
	while(line[j] != ':'){
		j++;
	}
	j++;

	while(line[j] != '@' && j<strlen(line)){
		//printf("line[%d]=%c ",j,line[j]);
		if(line[j] != ' ')
			charb += 1;
		j++;
	}
				
	//printf("\n");

	if(line[j] == '@' && j<strlen(line)){
		//printf("line[%d]=%c ",j,line[j]);
		sym = 1;
		j++;
	}

	//printf("\n");
	while(line[j] != '\n' && j<strlen(line)){
		//printf("line[%d]=%c ",j,line[j]);
		if(line[j] != ' ')
			chara += 1;
		j++;
	}
	//printf("charb = %d, sym = %d, chara = %d\n",charb,sym,chara);
				
	if(charb > 0 && sym == 1 && chara > 0){
		//printf("X@Y Format OK\n");
		return 1;
	}
	return -4;

}

int checkEmail(int n, char message[n]){
	char line[1024];
	int lineNo = 1;
	int i = 0, j = 0;
	int checks = 0, sym = 0, chara = 0, charb = 0, retVal = 0;
	//printf("message inside the func:%s",message);
	
	while(lineNo < 4){
		memset(line,'\0',sizeof(line));
		j = 0;
		if(lineNo == 1)
			i++;
		while(message[i] != '\n'){
			line[j] = message[i];
			i++;
			j++;
		}
		i++;
		//printf("line = %s\n",line);
		if(lineNo == 1){
			if(strncmp(line,"From:",5) == 0){
				checks = 1;
				retVal = checkFormat(strlen(line),line);
				if(retVal < 0){
					checks = -4;
					break;
				}
				lineNo++;
				
				
			}
			else{
				checks = -1;
				break;
			}
			
		}
		else if(lineNo == 2){

			if(strncmp(line,"To:",strlen("To:")) == 0){
				checks = 2;
				retVal = checkFormat(strlen(line),line);
				if(retVal < 0){
					checks = -5;
					break;
				}
				lineNo++;
				
			}
			else{
				checks = -2;
				break;
			}
		}
		else if(lineNo == 3){
			if(strncmp(line,"Subject:",strlen("Subject:")) == 0){
				checks = 3;
				lineNo++;
			}
			else{
				checks = -3;
				break;
			}
		}
			
	}
	if(checks < 0)
	{
		//printf("checks = %d : Incorrect format\n", checks);
		return -1;
	}
	return 1;

}


int main(int argc, char **argv)
{
	printf("\n--------------------SMTP CLIENT--------------------\n");
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	//char *ip = "127.0.0.1";
	int my_port = atoi(argv[1]);
	int sockfd,connection;

	struct sockaddr_in srvAddr;

	//CREATING SOCKET
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("Socket creation failed\n");
		exit(0);
	}
	/*else{
		printf("Socket created successfully\n");
	}*/
	
	//SERVER ADDRESS
	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvAddr.sin_port = htons(2022);


	//SENDING CONNECTION TO SERVER
	if(connect(sockfd,(struct sockAddr*) &srvAddr, sizeof(srvAddr)) != 0){
		printf("Could not connect to Server\n");
		exit(0);
	}
	/*else{
		printf("Connected to Server\n");
	}*/
	printf("\n\n");
	//printf("\n			COMMUNICATION BEGINS \n");
	//printf("		       ______________________\n\n");



	//CREATING BUFFER FOR COMMUNICATION
	char buffer[SIZE];
	char data[SIZE];
	int n, j =0;
	
	//INITIATING COMMUNICATION
	n = 0;
	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	//printf("C: HELO\n");
	strcpy(buffer,"HELO");
	write(sockfd,buffer,sizeof(buffer));

	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	read(sockfd,buffer,sizeof(buffer));
	strcpy(data,"250 Hello client.\n");
	if(strcmp(data,buffer) != 0){
		printf("Comunication not initiated\n");
		exit(1);	
	}
	//printf("S: %s",buffer);

	//USERNAME
	n = 0;
	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	printf("Username: ");
	while((data[n++] = getchar()) != '\n');
	sprintf(buffer, "USERN %s", data);
	write(sockfd,buffer,sizeof(buffer));
	//printf("Data sent over is: %s\n",buffer);

	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	read(sockfd,buffer,sizeof(buffer));
	strcpy(data,"Correct Username; Need Password.\n");
	if(strcmp(data,buffer) != 0){
		printf("No username of this kind\n");
		//close(sockfd);
		exit(1);	
	}
	//printf("S: %s",buffer);


	//PASSWORD
	n = 0;
	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	printf("Password: ");
	while((data[n++] = getchar()) != '\n');
	sprintf(buffer, "PASSWD %s", data);
	write(sockfd,buffer,sizeof(buffer));
	//printf("Data sent over is: %s\n",buffer);

	memset(&data,0,sizeof(data));
	memset(&buffer,0,sizeof(buffer));
	read(sockfd,buffer,sizeof(buffer));
	strcpy(data,"User Authenticated with password.\n");
	if(strcmp(data,buffer) != 0){
		printf("Password is incorrect\n");
		//close(sockfd);
		exit(1);	
	}
	//printf("S: %s",buffer);
	


	int ch = 0;
	int con = 1;
	int l;
	char line[1024];
	char mssgEnd[1024];
	char message[2048];
	
	while(con == 1){
		printf("\n---------------MENU--------------\n");
		printf("	1. Send Mail\n");
		printf("	2. Quit\n");
		printf("---------------------------------\n");
		printf("Enter your choice:");
		scanf("%d",&ch);
		if(ch == 1){
			n = 0;
			memset(&data,0,sizeof(data));
			memset(&buffer,0,sizeof(buffer));
			printf("C: DATA\n");
			strcpy(buffer,"DATA");
			write(sockfd,buffer,sizeof(buffer));

			memset(&data,0,sizeof(data));
			memset(&buffer,0,sizeof(buffer));
			read(sockfd,buffer,sizeof(buffer));
			strcpy(data,"354 Send message content.\n");
			if(strcmp(data,buffer) != 0){
				printf("Response from server not recieved\n");
				//close(sockfd);
				exit(1);	
			}
			//printf("S: %s",buffer);

			memset(mssgEnd,'\0',sizeof(mssgEnd));
			memset(message,'\0',sizeof(message));
			strcpy(mssgEnd,".\n");
			printf("\n\nWrite the mail below...\n");
			//ACCEPTING THE EMAIL
			while(1){
				l = 0;
				memset(line,0,sizeof(line));
				while((line[l++] = getchar()) != '\n');
				//printf("line = %s\n",line);
				strcat(message,line);
				//message[strlen(message)] = '\n';
				if(strcmp(line,mssgEnd) == 0){
					break;
				}
			}
			//printf("Message:\n%s\n",message);
			printf("\n\n");
			
			//CHECKING THE EMAIL
			int retVal = checkEmail(strlen(message),message);
			if(retVal < 0)
				printf("Incorrect format\n");
			else{
				write(sockfd,message,sizeof(message));
				memset(&data,0,sizeof(data));
				memset(&buffer,0,sizeof(buffer));
				read(sockfd,buffer,sizeof(buffer));
				strcpy(data,"250 OK, message accepted for delivery\n");
				if(strcmp(data,buffer) != 0){
					printf("Response from server not recieved\n");
					//close(sockfd);
					exit(1);	
				}
				//printf("S: %s",buffer);
			
			}

			
			
			
		}
		else if(ch == 2){
			memset(&buffer,0, sizeof(buffer));
			strcpy(buffer,"QUIT");
			printf("C: Quit\n");
			write(sockfd,buffer,sizeof(buffer));
			//printf("Client Exiting\n");
			con = 0;

		}
		else{
			printf("Wrong input\n");

		}
		
		
	}
	
	//CLOSING THE CONNECTION
	close(sockfd);

	


}
