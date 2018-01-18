/******************************************************************************
*     File Name           :     x.c                                           *
*     Created By          :     Klas Segeljakt <klasseg@kth.se>               *
*     Creation Date       :     [2017-12-08 11:33]                            *
*     Last Modified       :     [2017-12-12 14:56]                            *
*     Description         :     Send code over net, what could go wrong?      *
******************************************************************************/
#include <stdlib.h>     // malloc
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>     // read, write, close
/*****************************************************************************/
#define PORT 5000
#define BACKLOG 5
#define IP "127.0.0.1"
/*****************************************************************************/
static void transfer(char *filename);
/*****************************************************************************/
int main(const int argc, const char *argv[]) {
  printf("Transfering...\n");
  transfer("myfile.c");
  printf("Done!\n");
}
/*---------------------------------------------------------------------------*/
static void transfer(char *filename) {
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(IP),
    .sin_port = htons(PORT),
  };
  /* Connect */
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr))) {
    exit(EXIT_FAILURE);
  }
  /* Transfer file */
  int fd = open(filename, O_RDONLY);
  off_t offset = 0;
  off_t len = 0;
  printf("  Sending %s\n", filename);
  write(sock, filename, sizeof(char)*strlen(filename));
  sendfile(fd, sock, offset, &len, NULL, 0);
  close(sock);
}
