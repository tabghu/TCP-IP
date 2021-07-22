#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int fd;
  char buf[] = "dadnakda\n";
  fd = open("1.txt", O_CREAT | O_RDWR | O_TRUNC);
  write(fd, buf,sizeof(buf)-1);
  close(fd);
  return 0;
}
