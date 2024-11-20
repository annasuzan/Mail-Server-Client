#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ctype.h>
#include <time.h>

#define SIZE 4096

int check_usrn(int sockfd,int j, char data[j]){
	//printf("Entered\n");
	FILE *fp = fopen("../logincred.txt","r");
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	//printf("FILE OPENED\n");
	char c = fgetc(fp);
	char line[100];
	char username[100];
	int lineNo = 0, i,q,d;
	data[strlen(data)-1] = '\0';
	while(c!=EOF){
		memset(&username,0,sizeof(username));
		if(lineNo == 0){
				fclose(fp);
				FILE *fp = fopen("../logincred.txt","r");
		}
		//printf("------NEWLINE-------\n");
		fscanf(fp,"%[^\n]",line);
		i = 0;
		q = 0;
		while(i<strlen(line)){
				//printf("line[%d]=%c\n",i,line[i]);
				if(line[i] != ','){
					username[q] = line[i];
					q++;
				}
				else{
					break;
				}
				i++;	
		}
		
		//printf("username = %s, strlen(username) = %d ",username,strlen(username));
	
		//printf("data=%s, line = %s, strlen(data)=%d\n",data,line,strlen(data));
		lineNo++;
		if(strlen(username) == strlen(data)){
			
			d = 0;
			while(d<strlen(username)){
				//printf("username[%d]=%c, data[%d]=%c\n",d,username[d],d,data[d]);
				//if(strcmp(username[d],data[d]) != 0)
				if(username[d] != data[d]){
					break;
				}
				d++;
				
			}
			//printf("d = %d\n",d);
			if(d == strlen(username)){
				fclose(fp);
				return lineNo;
			}
		}
			
		/*if(strncmp(data,username,strlen(username)) == 0){
			printf("Returning and exiting this\n");
			printf("data = %s, username=%s\n",data,username);
			fclose(fp);
			return lineNo;
		}*/
		
		memset(&line,0,sizeof(line));
		memset(&username,0,sizeof(username));
		c = fgetc(fp);
	}
	fclose(fp);
	return -1;
	//printf("ok\n");
	//exit(1);
	
}

check_passwd(int sockfd,int r,int j,char data[j]){
	//printf("Entered pass\n");
	FILE *fp = fopen("../logincred.txt","r");
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	//printf("FILE OPENED\n");
	char c = fgetc(fp);
	char line[100];
	char password[100];
	int lineNo = 1;
	while(c!=EOF){
		
		
		//fseek(fp, -1, SEEK_CUR);
		//printf("------NEWLINE-------\n");
		fscanf(fp,"%[^\n]",line);
		if(lineNo == r){
		int i = 0;
		int j = 0;
		while(i<strlen(line)){
				//printf("line[%d]=%c\n",i,line[i]);
				if(line[i] == ','){
					i++;
					while(i<strlen(line)){
						password[j] = line[i];
						j++;
						i++;
					}
					
				}
				else{
					i++;
				}
	
		}
		//printf("password = %s\n",password);
	
		//printf("data=%s , line = %s, strlen(data)=%d\n",data,line,strlen(data));
		lineNo++;
		if(strncmp(data,password,strlen(password)) == 0){
			fclose(fp);
			return lineNo;
		}
		}
		else{
		lineNo++;
		
		memset(&line,0,sizeof(line));
		memset(&password,0,sizeof(password));
		c = fgetc(fp);
		}
	
		
	}
	fclose(fp);
	return -1;
	//printf("ok\n");
	//exit(1);


}

int howManyLines(char message[SIZE]){
	//printf("Entered howmanylines\n");
	int i = 0;
	int lineNo = 0;
	while(i<strlen(message)){
		while(message[i] != '\n')
			i++;
		i++;
		lineNo++;
	}
	return lineNo-1;

}

void store_email(int noBytes, char message[SIZE]){
	//printf("Entered storeMailccv\n");
	int i = 0,j = 0, lineNo = 1;
	char username[SIZE];
	char buffer[SIZE];
	memset(username,'\0',sizeof(username));

	int l = howManyLines(message);
	//printf("No lines=%d\n",l);
	while(lineNo<l){
		memset(buffer,'\0',sizeof(buffer));
		j = 0;
		if(lineNo == 1)
			i++;
		while(message[i] != '\n'){
			buffer[j] = message[i];
			i++;
			j++;
		}
		i++;
		//printf("line = %s\n",buffer);
		if(strncmp(buffer,"To:",strlen("To:"))==0)
			break;
		
		lineNo++;
	}
	i = 0;
	//printf("line outside = %s\n",buffer);
	while(i< strlen(buffer)){
		//printf("i = %d\n",i);
		if(strncmp(buffer,"To:",strlen("To:")) == 0){
			while(buffer[i] != ':' && i<strlen(buffer)){
				//printf("buffer[%d]=%c ",i,buffer[i]);
				i++;
			}
			//printf("\n");
			i++;
			j = 0;
			while(buffer[i] != '@' && i<strlen(buffer)){
				//printf("buffer[%d]=%c ",i,buffer[i]);
				if(buffer[i] != ' '){
					username[j] = buffer[i];
					i++; 
					j++;
				}
				else
					i++; 
			}
			//printf("\n");
		}
	}

	//printf("Username: %s\n",username);
	username[0] = toupper(username[0]);
	//printf("Username after: %s\n",username);

	
	//Modify the message to add RECIEVED
	
	char modMssg[2048];
	char currtime[2048];
	memset(modMssg,'\0',sizeof(modMssg));
	memset(currtime,'\0',sizeof(currtime));
	time_t t;
	time(&t);
	sprintf(currtime,"Recieved: %s",ctime(&t));
	i = 0;
	j = 0;
	while(i<strlen(message)){
		memset(buffer,'\0',sizeof(buffer));
		j = 0;
		while(message[i] != '\n'){
			buffer[j] = message[i];
			j++;
			i++;
		}
		buffer[j] = message[i];
		i++;
		j++;
		strcat(modMssg,buffer);
		if(strncmp(buffer,"Subject:",strlen("Subject:")) == 0){
			strcat(modMssg,currtime);
			
		}
		
	}

	FILE *fp;
	char filename[250];
	sprintf(filename,"%s/mymailbox.mail",username);
	//printf("filename =%s\n",filename);
	fp = fopen(filename,"a");
	if(fp == NULL) {
		printf("Error in opening the file.\n");	
		exit(1);
	}
	fprintf(fp,"%s",modMssg);
	fclose(fp);
	

}
int main(int argc, char **argv)
{
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	//char *ip = "127.0.0.1";
	int my_port = atoi(argv[1]);
	//int my_port = argv[1];
	//printf("my_port = %d\n",my_port);

	int sockfd,connSockfd, clntLen;
	struct sockaddr_in srvAddr,clntAddr;

	//CREATING SOCKET
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("Socket creation failed\n");
		exit(0);
	}
	else{
		printf("Socket created successfully\n");
	}

	//INITIALIZING SERVER ADDRESS
	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htons(INADDR_ANY);
	srvAddr.sin_port = htons(my_port);

	//BINDING THE SOCKET TO SERVER
	if(bind(sockfd,(struct sockaddr*) &srvAddr, sizeof(srvAddr)) != 0){
		printf("Socket binding failed\n");
		exit(0);
	}
	else{
		printf("Socket binding successful\n");
	}

	//SERVER IS LISTENING
	if((listen(sockfd,5)) != 0){
		printf("Listening failed\n");
		exit(0);
	}
	else{
		printf("SMTP Server is listening...\n");
	}

	//ACCEPTING CONNECTION
	/*clntLen = sizeof(clntAddr);
	connSockfd = accept(sockfd,(struct sockAddr*) &clntAddr, &clntLen);
	if(connSockfd < 0){
		printf("Connection could not be accepted\n");
		exit(0);
	}
	else{
		printf("Connection to client made successfully\n");
	}*/

	/*fp = fopen(filename, "rb");
	if(fp == NULL) {
		printf("Error in reading the file.\n");	
		exit(1);
	}*/

	printf("\n\n");
	//printf("\n			COMMUNICATION BEGINS \n");
	//printf("		       ______________________\n\n");

	
	//CREATING BUFFER FOR COMMUNICATION
	char buffer[SIZE];
	char data[SIZE];
	int n,r = -1,i,j,w = -1;
	int noBytes = 0;
	while(1){
		//ACCEPTING CONNECTION
		clntLen = sizeof(clntAddr);
		connSockfd = accept(sockfd,(struct sockAddr*) &clntAddr, &clntLen);
		if(connSockfd < 0){
			
			printf("Connection could not be accepted\n");
			exit(0);
		}
		else{
			printf("\n*****************************************\n");
			printf("Connection to client made successfully\n");
		}

		while(1){
		memset(&buffer,0, sizeof(buffer));
		memset(&data,0, sizeof(data));
		//RECEIVING DATA FROM CLIENT
		read(connSockfd,buffer,sizeof(buffer));
		//printf("CLIENT		  : %s\n", buffer);

		if(strncmp("QUIT",buffer,4) == 0){
			printf("\nS: 221 Bye\n");
			break;
		}
		else if(strncmp("HELO",buffer,strlen("HELO")) == 0){
			printf("C: %s\n",buffer);
			memset(&buffer,0, sizeof(buffer));
			strcpy(buffer,"250 Hello client.\n");
			printf("S: %s",buffer);
			write(connSockfd,buffer,sizeof(buffer));
		}
		else if(strncmp("DATA",buffer,strlen("DATA")) == 0){
			printf("C: %s\n",buffer);
			memset(&buffer,0, sizeof(buffer));
			strcpy(buffer,"354 Send message content.\n");
			printf("S: %s",buffer);
			write(connSockfd,buffer,sizeof(buffer));
		}
		else if(strncmp("USERN",buffer,strlen("USERN")) == 0){
			memset(&data,0, sizeof(data));
			//printf("Entered usrn\n");
			i = 0;
			while(i<strlen(buffer)){
				//printf("buffer[%d]=%c\n",i,buffer[i]);
				if(isspace(buffer[i])){
					i++;
					j = 0;
					while(i<strlen(buffer)){
						
						data[j] = buffer[i];
						j++;
						i++;
					}
					
				}
				else
					i++;
			}
			//printf("data = %s\n",data);
			r = check_usrn(sockfd,j,data);
			if(r != -1){
				memset(&buffer,0, sizeof(buffer));
			
				n = 0;
			
				strcpy(buffer,"Correct Username; Need Password.\n");
			
				write(connSockfd,buffer,sizeof(buffer));
				printf("S: Correct Username; Need Password.\n");
				//printf("***************************\n");
			}
			else{
				memset(&buffer,0, sizeof(buffer));
			
				n = 0;
			
				strcpy(buffer,"Incorrect Username.\n");
			
				write(connSockfd,buffer,sizeof(buffer));
				
				printf("S: Incorrect Username.\n");
				//printf("***************************\n");
				//exit(1);
				break;
			}
			//exit(1);
			
		}
		else if(strncmp("PASSWD",buffer,strlen("PASSWD")) == 0){
			if(r != -1){
				i = 0;
		
				memset(&data,0, sizeof(data));
				while(i<strlen(buffer)){
					//printf("buffer[%d]=%c\n",i,buffer[i]);
					if(isspace(buffer[i])){
						i++;
						j = 0;
						while(i<strlen(buffer)){
						
							data[j] = buffer[i];
							j++;
							i++;
						}
					
					}
					else
						i++;
				}
				//printf("data = %s\n",data);

				w = check_passwd(sockfd,r,j,data);
				if(w != -1){
					memset(&buffer,0, sizeof(buffer));
			
					n = 0;
			
					strcpy(buffer,"User Authenticated with password.\n");
			
					write(connSockfd,buffer,sizeof(buffer));
					
					printf("S: User Authenticated with password.\n");
					//printf("***************************\n");
				}
				else{
					memset(&buffer,0, sizeof(buffer));
			
					n = 0;
			
					strcpy(buffer,"Incorrect password.\n");
			
					write(connSockfd,buffer,sizeof(buffer));
					//printf("***************************\n");
					
					
					printf("S: Incorrect password.\n");
					//exit(1);
					break;
				}
				//printf("Data written into new file.\n");
			}
			else{
				printf("S: Username not input\n");
				exit(1);
			}
		}
		else{

			printf("\nC:\n%s",buffer);
			store_email(strlen(buffer),buffer);
			memset(&buffer,0, sizeof(buffer));
			strcpy(buffer,"250 OK, message accepted for delivery\n");
			printf("S: %s\n",buffer);
			write(connSockfd,buffer,sizeof(buffer));
		
		}
		}
		
	
		
	}


	//CLOSING THE SOCKET
	close(sockfd);
	//shutdown(connSockfd, SHUT_RDWR);
}
