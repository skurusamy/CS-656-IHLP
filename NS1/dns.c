/***
 * CS656-00N Group W4
 * AtulAvijeet ,aa2758@njit.edu
 * JinalPatel , jp874@njit.edu
 * JasminPatel , jp878@njit.edu
 * Subhasree Kurusamy , sk2745@njit.edu
 * SauravHarjai ,sh625@njit.edu
 * HimaniPatel , hp437@njit.edu
 * **/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include <sys/timeb.h>
#include <sys/time.h>

struct addrinfo *res;
char host[100],path[100],head[100];
char  port[6],pref[100];

//dns function
int dns(char addr[])
{
	int errcode=0;
	errcode = getaddrinfo(addr, NULL, NULL, &res);
	return errcode;
}
//parsing Http request
void  doParse(char addr[])
{
 	int j,k=0,l=0,z,d=0,t,w=0;
 	char *addr1,hos[100];
	char po[20],*host1,*y;
	
 	addr1= strchr(addr,':');
 	for(j=3;addr1[j]!='/';j++)
 	{
  	hos[k++]=addr1[j];
 	}
 	hos[k++]='\0';
 	for(k=j;addr1[k]!=' ';k++)
 	{
  	path[l++]=addr1[k];
 	}
 	path[l++]='\0';

	y=strstr(hos,":");
	if(y)
	{
	 for(t=0;hos[t]!=':';t++)
	   host[w++]=hos[t];
	 host[w++]='\0';
	 host1 = strchr(hos,':');
	 for(z=1;z<strlen(host1);z++)
	   po[d++]=host1[z]; 
	 po[d++]='\0';
	strcpy(port,po);
	}
	else
	{
	strcpy(host,hos);
	strcpy(port,"80");
	}
	/*w=0;
	for(t=0;addr[t]!='\n';t++)
		head[w++]=addr[t];
	head[w++]='\r';
	head[w++]='\n';
	for(t=w-1;addr[t]!='\n';t++)
		head[w++]=addr[t];
	head[w++]='\r';
	head[w++]='\n';
	head[w++]='\r';
	head[w++]='\n';
	printf("%s\n",head);*/
	strcpy(head,"GET ");
	strcat(head,path);
	strcat(head," HTTP/1.1\r\nHost: 
}
void doHttp(int con,char buf[100])
{
	/*char div_line[30]="<div>";
	char style[30]="<style>div{color:red;}</style>";
	send(con,style,strlen(style),0); 
	send(con,div_line,strlen("next_line"),0);
	send(con,"HOSTIP = ",strlen("HOST IP ="),0); 
	send(con,host,strlen(host),0);
	send(con," ( ",strlen(" ( "),0); 
	send(con,pref,strlen(pref),0);
	send(con," ) ",strlen(" ) "),0); 
	send(con,div_line,strlen("next_line"),0);
	send(con,"PORT =  ",strlen("PORT =  "),0); 
	send(con,port,strlen(port),0); 
	send(con,div_line,strlen("next_line"),0);
	send(con,"PATH = ",strlen("PATH ="),0); 
	send(con,path,strlen(path),0);*/
	send(con,buf,strlen(buf),0);
	

}
void print()
{
	perror("Error: ");
 	exit(0);
	}
 int  main(int argc, char **argv)
 {
	int socket_id,conn,len,sockfd,i,j;
 	int min=9999999;
	struct sockaddr_in servaddr,client_addr,my_addr;
	char line[1024],buf[1024],addrstr[100],local_ip[100];
	socklen_t slen = sizeof(client_addr);
	socklen_t mylen = sizeof(my_addr);
	int errcode=0;
	void *ptr;  
	int total_t;
	char ipaddr[100];
	struct timeval start, end;
//fu request 
	int socket_id1,rep;
	struct sockaddr_in client;
	char webpage[9999];
	char head1[999]="GET / HTTP/1.1\r\nHost: www.njit.edu\r\n\r\n";
//html files
	
/*	char header[80]="<!DOCTYPE html><html><head></head><body>";
	char footer[40]="</body></html>";
	char next_line[10]="<p>";
	char type1[40]="HTTP/1.0 200 OK\n";
	char type2[100]="Content-Type: text/html\n\n";
*/
	strncpy(ipaddr, "", sizeof(ipaddr));
	if(argc<2){
		printf("Insufficient Input\n");
		exit(0);
		}	
	if((socket_id=socket(AF_INET,SOCK_STREAM,0))==-1)//socket creation
		print();
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(atoi(argv[1]));
	if((bind(socket_id,(struct sockaddr *) &servaddr, sizeof(servaddr)))<0)
		print();  
	//printf(" Server is Listening on port %d\n",atoi(argv[1]));
	printf("Stage 2 program by SK2745\n");
	printf("Server is Listening on port %d\n",atoi(argv[1]));
	for(j=0;;j++){
		if((listen(socket_id,1))==-1)
			print();
		if((conn= accept(socket_id,(struct sockaddr *) &client_addr, &slen))<0)
			print();
		//finding server ip
		getsockname(conn, (struct sockaddr *)&my_addr,&mylen);
		inet_ntop (AF_INET,&(my_addr.sin_addr),local_ip,100);
	//	printf("(%d) Incoming client connection from [%s:%d] to me [%s:%d]\n",j+1,inet_ntoa(client_addr.sin_addr),atoi(argv[1]),local_ip,atoi(argv[1]));
		
		if((len = recv(conn,line,1024,0))<=0)
			perror("Receive Failed\n");
		for(i=0;i<len-2;i++)
			buf[i]=line[i];
		buf[i]='\0';
		printf("REQUEST: %s\n",buf);
		printf("Requested\n");
	//	send(conn,type1,strlen(type1),0);
	//	send(conn,type2,strlen(type2),0);
	//	send(conn,header,strlen(header),0);
	//	send(conn,"REQ= ",strlen("REQ= "),0); 
	//	send(conn,buf,strlen(buf),0); 
	//	send(conn,next_line,strlen("next_line"),0);
		doParse(buf);
		errcode=dns(host);

		//lists all the ip obtained from dns function 
		if(errcode!=0){
		//	send(conn,next_line,strlen(next_line),0); 
		//	send(conn,"\tNo IP address found\n",strlen("No IP address found\n"),0);
		//	send(conn,next_line,strlen(next_line),0); 
		//	send(conn,footer,strlen(footer),0);
		 	strcpy(pref,"No IP found");
			}
		else{
			while (res){
				inet_ntop (res->ai_family,res->ai_addr->sa_data,addrstr,100);
				switch(res->ai_family){
					case AF_INET:
						ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
						break;
					case AF_INET6:
						ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
						break;
						}//switch
 				inet_ntop (res->ai_family,ptr,addrstr,100);
				sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
				gettimeofday(&start,NULL);
				connect(sockfd,res->ai_addr,res->ai_addrlen); 
				gettimeofday(&end,NULL);
				total_t = (int)(end.tv_usec-start.tv_usec);
				if(total_t<min){
					min=total_t;

					strcpy(ipaddr,addrstr);
					}	
				res=res->ai_next;
					}//while loop
			strcpy(pref,ipaddr);
			//send(conn,footer,strlen(footer),0);
			//send(conn,"\n",strlen("\n"),0);
			}//else(found ip)
		min=9999999;
		if((socket_id1=socket(AF_INET,SOCK_STREAM,0))==-1)//socket creation
			print();
		bzero(&client, sizeof(client));
		client.sin_family= AF_INET;
		client.sin_addr.s_addr=inet_addr(pref);
		client.sin_port=htons(80);

		if(connect(socket_id1,(struct sockaddr *)&client,sizeof(client))<0)
			print();
		if(send(socket_id1,head1,strlen(head1),0)<0)
			print();
		rep=recv(socket_id1,webpage,9999,0);
		printf("~~~~~~\n");
		printf("%s",webpage);
		printf("~~~~~~\n");
		doHttp(conn,webpage);
		//Sends http request
 		strncpy(ipaddr, "", sizeof(ipaddr));
 		close(conn);
 		} 
	close(socket_id);
	return 0;
}
