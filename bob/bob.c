/******************************************************************************
*     File Name           :     x.c                                           *
*     Created By          :     Klas Segeljakt <klasseg@kth.se>               *
*     Creation Date       :     [2017-12-08 13:17]                            *
*     Last Modified       :     [2017-12-08 14:20]                            *
*     Description         :     Recv code over net, Very safe... right?       *
******************************************************************************/
#include <stdio.h>      // printf, sprintf
#include <stdlib.h>     // exit, system
#include <arpa/inet.h>  // network
#include <unistd.h>     // read, write, close
#include <string.h>     // strcpy, strlen
#include <dlfcn.h>      // dlopen, dlsym, dlclose
/*****************************************************************************/
#define PORT 5000
#define IP "127.0.0.1"
#define BACKLOG 1
/*****************************************************************************/
typedef int (*func_f)(int);
/*****************************************************************************/
static void receive(char filename[]);
static void compile(char libname[], char filename[]);
static void execute(char libname[], char *funcname, int arg);
/*****************************************************************************/
int main(int argc, char *argv[]) {
  char filename[BUFSIZ] = {0};
  char libname[BUFSIZ] = {0};
  printf("Receiving...\n");
  receive(filename);
  printf("Compiling...\n");
  compile(libname, filename);
  printf("Executing...\n");
  execute(libname, "my_function", 2);
  printf("Done!\n");
  return 0;
}
/*---------------------------------------------------------------------------*/
static void receive(char filename[]) {
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(IP),
    .sin_port = htons(PORT),
  };
  /* Setup socket which listens for incoming connections */
  int ssock = socket(AF_INET, SOCK_STREAM, 0);
  if(bind(ssock, (struct sockaddr*)&addr, sizeof(struct sockaddr))) {
    exit(EXIT_FAILURE);
  }
  if(listen(ssock, BACKLOG)) {
    exit(EXIT_FAILURE);
  }
  socklen_t sock_len = sizeof(struct sockaddr_in);
  /* Accept incoming connection */
  int csock = accept(ssock, (struct sockaddr*)&addr, &sock_len);
  /* Receive metadata */
  read(csock, filename, BUFSIZ); // Filename
  FILE *fid = fopen(filename, "w+");
  /* Receive file */
  printf("  Receiving %s\n", filename);
  int len = 0;
  char buf[BUFSIZ] = {0};
  while((len = read(csock, buf, BUFSIZ)) > 0) {
    fwrite(buf, sizeof(char), len, fid);
  }
  fclose(fid);
  close(ssock);
  close(csock);
}
/*---------------------------------------------------------------------------*/
static void compile(char libname[], char filename[]) {
  char cmd[BUFSIZ] = {0};
  strcpy(libname, filename);
  int len = strlen(filename);
  libname[len-1] = 's';
  libname[len]   = 'o';
  libname[len+1] = '\0';
  sprintf(cmd, "cc -shared -fPIC -o %s %s", libname, filename);
  printf("  %s\n", cmd);
  system(cmd);
}
/*---------------------------------------------------------------------------*/
static void execute(char libname[], char *funcname, int arg) {
  void *handle = dlopen(libname, RTLD_NOW);
  func_f func = dlsym(handle, funcname); // Execute function with name: funcname
  printf("  %s(%d): %d\n", funcname, arg, func(arg));
  dlclose(handle);
}
