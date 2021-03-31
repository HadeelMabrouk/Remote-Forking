#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>   
#include<sys/ioctl.h>
#include<unistd.h>  
#include<iostream>
#include<fstream>
#include<errno.h>
#include <cstring>

#define PORT 8080
using namespace std;


int restore(int pid, string ip)
{

  int socket_desc;
  struct sockaddr_in server;
  char *parray;


  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);

  if (socket_desc == -1) {
  printf("Could not create socket");
  }

  memset(&server,0,sizeof(server));
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( PORT );

  //Connect to remote server
  if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
  cout<<strerror(errno);
  close(socket_desc);
  puts("Connect Error");
  return 1;
  }

  puts("Connected\n");
  send(socket_desc, "D",strlen("D"),0);
usleep(1000000);
	send(socket_desc, &pid,sizeof(pid),0);
usleep(1000000);
	int IPSize = ip.size();
	send(socket_desc, &IPSize,sizeof(IPSize),0);
usleep(1000000);	
	send(socket_desc, ip.c_str(),strlen(ip.c_str()),0);

  close(socket_desc);
	fflush(stdout);
  return 0;
}


int myfork(string ip)
{
	int pid;
	pid = fork();
	if(pid < 0) return -1;
	else if(pid == 0)
	{
		if(restore(getpid(),ip)==0)
		{
			usleep(20000000);
			return 0;
		}
		else
			return 2;
	}
	else return 1;
}


int main()
{
	string s;
	cout << "Please, enter the IP of the remote machine: ";
	cin >> s;
	int isParent = myfork(s);
	if(isParent == 1) cout << "I'm Parent!" << endl;
	else if (isParent == 2) cout << "Error: Connection Failed." << endl;
	else if (isParent == -1) cout << "Error: Forking Failed." << endl;
	else cout << "I'm Child!" << endl;
	return 0;
}

