#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void error_handing(char* message);
    
int main()
{
  //文件描述符
  //open 函数原型 int open(const char* path, int flag)
  //成功时返回文件描述符，失返回-1
  int fd;
  char buf[] = "Let's go!\n";
  fd = open("data.txt", O_CREAT | O_RDWR | O_TRUNC);
  if(fd == -1)
  {
    error_handing("open() error!");  
  }
  printf("file descriptor: %d\n", fd);
  if((write(fd, buf, sizeof(buf)) == -1))
  {
    error_handing("write() error!"); 
  }
  

  close(fd);
  return 0;
}

void error_handing(char* message)
{
  printf("run this\n");
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
