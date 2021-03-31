
   #include<stdio.h>
   #include<string.h>
   #include<sys/socket.h>
   #include<arpa/inet.h>   
   #include<unistd.h>  
   #include<iostream>
   #include<fstream>
   #include<errno.h>
	 #include <cstring>

#define PORT 8080

   using namespace std;

int receive_image(int socket, string filename)
{ // Start function 

int buffersize = 0, recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;

char imagearray[10241],verify = '1';
FILE *image;

//Find the size of the image
do{
stat = read(socket, &size, sizeof(int));
}while(stat<0);

printf("Packet received.\n");
printf("Packet size: %i\n",stat);
printf("Image size: %i\n",size);
printf(" \n");

char buffer[] = "Got it";

//Send our verification signal
do{
stat = write(socket, &buffer, sizeof(int));
}while(stat<0);

printf("Reply sent\n");
printf(" \n");

image = fopen(filename.c_str(), "w");

if( image == NULL) {
printf("Error has occurred. Image file could not be opened\n");
return -1; }

//Loop while we have not received the entire file yet


int need_exit = 0;
struct timeval timeout = {10,0};

fd_set fds;
int buffer_fd, buffer_out;

while(recv_size < size) {
//while(packet_index < 2){

    FD_ZERO(&fds);
    FD_SET(socket,&fds);

    buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

    if (buffer_fd < 0)
       printf("error: bad file descriptor set.\n");

    if (buffer_fd == 0)
       printf("error: buffer read timeout expired.\n");

    if (buffer_fd > 0)
    {
        do{
               read_size = read(socket,imagearray, 10241);
            }while(read_size <0);

            printf("Packet number received: %i\n",packet_index);
        printf("Packet size: %i\n",read_size);


        //Write the currently read data into our image file
         write_size = fwrite(imagearray,1,read_size, image);
         printf("Written image size: %i\n",write_size); 

             if(read_size !=write_size) {
                 printf("error in read write\n");    }


             //Increment the total number of bytes read
             recv_size += read_size;
             packet_index++;
             printf("Total received image size: %i\n",recv_size);
             printf(" \n");
             printf(" \n");
    }

}


  fclose(image);
  printf("Image successfully Received!\n");
  return 1;
  }


 int send_image(int socket,string filename){

   FILE *picture;
   int size, read_size, stat, packet_index;
   char send_buffer[10240], read_buffer[256];
   packet_index = 1;

   picture = fopen(filename.c_str(), "r");
   printf("Getting Picture Size\n");   

   if(picture == NULL) {
        printf("Error Opening Image File"); } 

   fseek(picture, 0, SEEK_END);
   size = ftell(picture);
   fseek(picture, 0, SEEK_SET);
   printf("Total Picture size: %i\n",size);

   //Send Picture Size
   printf("Sending Picture Size\n");
   write(socket, (void *)&size, sizeof(int));

   //Send Picture as Byte Array
   printf("Sending Picture as Byte Array\n");

   do { //Read while we get errors that are due to signals.
      stat=read(socket, &read_buffer , 255);
      printf("Bytes read: %i\n",stat);
   } while (stat < 0);

   printf("Received data in socket\n");
   printf("Socket data: %s\n", read_buffer);

   while(!feof(picture)) {
   //while(packet_index = 1){
      //Read from the file into our send buffer
      read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

      //Send data through our socket 
      do{
        stat = write(socket, send_buffer, read_size);  
      }while (stat < 0);

      printf("Packet Number: %i\n",packet_index);
      printf("Packet Size Sent: %i\n",read_size);     
      printf(" \n");
      printf(" \n");


      packet_index++;  

      //Zero out our send buffer
      bzero(send_buffer, sizeof(send_buffer));
     }
    }

int main(int argc , char *argv[])
{
      int socket_desc , new_socket , c, read_size,buffer = 0;
      struct sockaddr_in server , client;
      char *readin;

      //Create socket
      socket_desc = socket(AF_INET , SOCK_STREAM , 0);
      if (socket_desc == -1)
      {
         printf("Could not create socket");
      }

      //Prepare the sockaddr_in structure
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port = htons( PORT );

      //Bind
			 if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
			 {
			   puts("bind failed");
			   return 1;
			 }

     puts("bind done");

     //Listen
     listen(socket_desc , 3);

      //Accept and incoming connection
      puts("Waiting for incoming connections...");
      c = sizeof(struct sockaddr_in);

     if((new_socket = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&c))){
puts("Connection accepted");
         }

    fflush(stdout);

    if (new_socket<0)
    {
      perror("Accept Failed");
      return 1;
    }
	char myBuffer[4];
	bzero(myBuffer,sizeof(myBuffer));
	int valread = read(new_socket, myBuffer, 4);
	printf("%s\n", myBuffer);
	
	

	
	
	if(myBuffer[0] == 'D')
	{
		int myPID;
	  valread = read(new_socket, &myPID, sizeof(myPID));
	  printf("%d\n", myPID);

		int IPSize;
	  valread = read(new_socket, &IPSize, sizeof(IPSize));

		char myIP[20];
	  valread = read(new_socket, myIP, 20);
	  printf("%s\n", myIP);

	  int socket_desc;
	  struct sockaddr_in server;
	  char *parray;

	  //Create socket (client mode to connect to the remote node)
	  int socket_desc2 = socket(AF_INET , SOCK_STREAM , 0);

	  if (socket_desc2 == -1) {
	  printf("Could not create socket");
	  }

	  memset(&server,0,sizeof(server));

		char *myIP2 = new char[IPSize+1];
		memcpy(myIP2, myIP,IPSize);
		myIP2[IPSize] = '\0';
		printf("%s\n", myIP2);
	  server.sin_addr.s_addr = inet_addr(myIP2);
	  server.sin_family = AF_INET;
	  server.sin_port = htons( PORT );

	  //Connect to remote server
	  if (connect(socket_desc2 , (struct sockaddr *)&server , sizeof(server)) < 0) {
	  cout<<strerror(errno);
	  close(socket_desc2);
	  puts("Connect Error");
	  return 1;
	  }

	  puts("Connected\n");
	  send(socket_desc2, "R",strlen("R"),0);

		system("mkdir checkpoint");
		string command = "sudo criu dump -t "+to_string(myPID)+" -D checkpoint/ -j -v4";			
		printf("%s\n", command.c_str());
		system(command.c_str());
		system("zip -r checkpoint.zip checkpoint");
	  
    		send_image(socket_desc2,"checkpoint.zip");	
		close(socket_desc2);
		fflush(stdout);	
	}
	if(myBuffer[0] == 'R')
	{
		receive_image(new_socket, "checkpoint.zip");
		system("unzip checkpoint.zip");
		system("sudo criu restore -d -D checkpoint/ -j");
		close(new_socket);
		fflush(stdout);
	}
    close(socket_desc);
    fflush(stdout);
    return 0;
}

