#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<time.h>
#include<sys/timeb.h>
#include<sys/time.h>
#include<unistd.h>

char request[65535],path[100];
char host[1024],name[1024],pref[100];
char port[20];

//displays error
void print(char fn[])
{
 printf("Function:%s\n",fn);
 perror("Error : ");
 exit(0);
}

//dns
char* dns(char addr[])
{
 strcpy(port," ");
 int errcode=0,min=99999,total_t;
 char *s;
 int sockfd,i=0,k=0;
 char *pref,addrstr[100],addr1[100];
 struct addrinfo *res,hints;
 struct timeval start,end;
 pref= (char*)malloc(sizeof(char));
 struct sockaddr_in client1;

 memset(&hints,0,sizeof(hints));
 hints.ai_family=AF_INET;
 hints.ai_socktype=SOCK_STREAM;
 hints.ai_protocol = 6;

 s=strchr(name,':');
 if(s)
 {
  for(i=1;i<strlen(s);i++)
  {
   port[k++]=s[i];
  }
  port[k++]='\0';
  k=0;
  for(i=0;addr[i]!=':';i++)
    addr1[k++]=addr[i];
  addr1[k++]='\0';
 }
 else
 {
 strcpy(port,"80");
 strcpy(addr1,addr);
 }
 errcode = getaddrinfo(addr1,"http",&hints,&res);
 if(errcode!=0)
   strcpy(pref,"No IP Found");
 else
 {
   while(res){
        inet_ntop(res->ai_family,&((struct sockaddr_in *)res->ai_addr)->sin_addr,addrstr,INET_ADDRSTRLEN);
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	  print("Dns socket");
        bzero(&client1,sizeof(client1));
        client1.sin_family = AF_INET;
        client1.sin_port=htons(atoi(port));
 
 	if(inet_pton(AF_INET,addrstr,&client1.sin_addr)<0)
              print("inet_pton error");
	gettimeofday(&start,NULL);
	//connect(sockfd,res->ai_addr,res->ai_addrlen);
	if((connect(sockfd,(struct sockaddr *)&client1,sizeof(client1))<0))
	   print("Dns Connect");
        printf("DNS: %s\n",addrstr);
	gettimeofday(&end,NULL);
	total_t  = (int)(end.tv_usec - start.tv_usec);
	if(total_t < min){
		min=total_t;
		strcpy(pref,addrstr);
	}
	res=res->ai_next;
   }
 }
 return pref;
}

//doParse
void doParse(char url[])
{
 int i=0,j=0,k=0;
 //request --> First Line
 while(url[i]!='\n')
 {
  request[j++]=url[i];
  i++;
 }
 request[j++]='\0';
 i=4;
 j=0;
 //host ----> except Get
 while(request[i]!=' '){
  host[j++]=request[i];
  i++;
  }
  host[j++]='\0';
  // name -->hostname
  i=7;
  j=0;
  while(host[i]!='/'){
   name[j++]=host[i++];
  }
  name[j++]='\0';
  k=strlen(name);
  i=11+k;
  j=0;
  while(request[i]!=' ')
     path[j++]=request[i++];
  path[j++]='\0';
// printf("Host---:%s\n",host);
 // printf("Name---:%s\n",name);
  //printf("Path---:%s\n",path);
  return;
}

int block( char hostname[],char filename[])//host,filename
{
 int len=strlen(hostname);
 char temp[len];
 FILE *fp;
 int flag=-2;
 char *res;
 fp=fopen(filename,"r");
 if(fp==NULL)//no file exist
   return -1;
 while(!feof(fp))
 {
  //fgets(temp,sizeof(temp),fp);
  fscanf(fp,"%s",temp);
  res=strstr(hostname,temp);
  if(res)
  {
     flag=1;//invalid req
     break;
  }
  else
     flag= 0;//valid req
 }
 if(flag==1)
   return 1;//invalid req
 else
   return 0;//valid req
}

void errorPage(int conn,char err[],char err_code[])
{
  char header[60]="<!DOCTYPE html><html><head></head><body><center>";
  char footer[40]="</center></body></html>";
  char type1[40]="HTTP/1.0 200 OK\n";
  //err_code
  char type2[40]="Content-Type: text/html\n\n";
  
  send(conn,type1,strlen(type1),0);//header
  send(conn,type2,strlen(type2),0);
  send(conn,err_code,strlen(err_code),0);//Err_code
  send(conn,header,strlen(header),0);//header <html>
  send(conn,err,strlen(err),0);//error msg to be displayed
  //send(conn,nextLine,strlen(nextLine),0);
  send(conn,footer,strlen(footer),0);//footer </html>
  close(conn);
  return;
}

//doHttp
void doHttp(int con,char pref[])
{
 char head[100];
 strcpy(head,"GET ");
 strcat(head,path);
 strcat(head," HTTP/1.1\r\nHost: ");
 strcat(head,name);
 strcat(head,"\r\n\r\n");
 int socket_id1,response_len;
 struct sockaddr_in client;
 char webpage[65535]; 
 if((socket_id1=socket(AF_INET,SOCK_STREAM,0))==-1)
    print("DoHttp Socket");
 bzero(&client,sizeof(client));
 client.sin_family = AF_INET;
 client.sin_addr.s_addr=inet_addr(pref);
 //client.sin_port=htons(80);
 client.sin_port=htons(atoi(port));
 if(connect(socket_id1,(struct sockaddr*)&client,sizeof(client))<0)
 	print("Dohttp connect");
 if(send(socket_id1,head,strlen(head),0)<0)
 	print("Dohttp Send");
 do
 {
 response_len=recv(socket_id1,webpage,65535,0);
// printf("Response Length: %d\n",response_len);
 send(con,webpage,response_len,0);
 strcpy(webpage," ");
 }while(response_len!=0);
 return;
}


int main(int argc, char **argv)
{
 char *ip;
 int socketid,conn,i=0;
 struct sockaddr_in servaddr,client_addr;
 socklen_t client_len=sizeof(client_addr);
 int request_len=-1;
 char request_buffer[65535],line[65535];;
 FILE *block_list;
 int flag;
 ip= (char* )malloc(sizeof(char));
 if(argc<3)//object_file,port,blocked_file
 {
  printf("Insufficient Input\n");
  exit(0);
 }
 if(( block_list = fopen(argv[2],"r"))==NULL)
 {
  printf("Given File not Found\n");
  //fclose(block_list);
  exit(1);
 }
 if((socketid=socket(AF_INET,SOCK_STREAM,0))==-1)
   print("Main Socket creation");
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family=AF_INET;
 servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 servaddr.sin_port=htons(atoi(argv[1]));
 if((bind(socketid,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0)
    print("Main Bind");
 printf("Stage 3 program by SK2745, Listening in port %d\n",atoi(argv[1]));
 if(listen(socketid,100)<0)
   print("Main Listen");
   while(1){
    printf("Listening.....\n");
    bzero(&client_addr,sizeof(client_addr));
    //Connection
    if((conn=accept(socketid, (struct sockaddr *)&client_addr,&client_len))<0)
       print("Main connect");
    request_len = recv(conn,line,65535,0);
    if(request_len<=0){
  //    continue;
    }
    if(request_len >65535)
    {
     //send larger request
     errorPage(conn,"Invalid Larger Request","413 Request Too Large");
    // continue;
    }
    for(i=0;i<request_len-2;i++)
    {
      request_buffer[i]=line[i];
    }
    request_buffer[i]='\0';//has the entire request from the browser
   doParse(request_buffer);//returns firstline in request and hostname in host
   if(strlen(request)>=3 && request[0] =='G' && request[1]=='E' && request[2]=='T'){
     printf("Request:%s\n",request);
   }
   if(strcmp(host,"")==0|| strlen(host)>253)
   {
    //  printf("$$$ Long Request / no request $$$\n");
      errorPage(conn,"400 Bad Request","400 Bad Request");
   }
   flag = block(host,argv[2]);//check where hostname is blocked or not
   if(flag==-1)
   { 
       printf("$$$$ Invalid Request $$$$$\n");
       errorPage(conn,"403 Forbidden Blocked Website","Error:403");
   close(conn);
   }
   else if(flag==1)
   { 
       printf("$$$$ Invalid Request $$$$$\n");
       errorPage(conn,"403 Forbidden Blocked Website","Error:403");
   close(conn);
   }
   else
   {
    //dns function
    ip=dns(name);
    if((strcmp(ip,"No IP Found"))==0)
       errorPage(conn,"404 Error: File Not Found","Error:404");
      // printf("DNS Error\n");
    else
    {
       //doHttp
       strcpy(pref,ip);
       doHttp(conn,pref);
   }
   }
   strcpy(request," ");
   close(conn);
   
   }
   return 0;
}
