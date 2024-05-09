#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define MAXFILE2 11

static uint64 next = 1;

uint64 rand(void) {
  next = next * 1103515245 + 12345;
  return (uint64)(next/65536);
}

void srand(uint64 seed) {
  next = seed;
}

#define BUFSIZE (BSIZE / sizeof(uint64))
uint64 buf[BUFSIZE];

int main() {
  int fd = open("big", O_CREATE|O_RDWR);
  if(fd < 0){
    printf("error: creat big failed!\n");
    exit(1);
  }

  int cnt = 0;
  srand(1);
  int x;
  for(int i = 0; i < MAXFILE2 * BUFSIZE; ++i){
    rand();
    buf[cnt++] = next;
    cnt %= BUFSIZE;
    if(!cnt) {	
      if ((x = write(fd, buf, BUFSIZE)) != BUFSIZE){
      	printf("error: write big file failed, writed: %d\n", x);
      	exit(1);
      }
      else if (i / BUFSIZE % 1000 == 0) printf("%d/%d block writed!\n", i / BUFSIZE, MAXFILE2);
    }
  }

  close(fd);

  fd = open("big", O_RDONLY);
  if(fd < 0){
    printf("error: open big failed!\n");
    exit(1);
  }

  cnt = 0;
  srand(1);
  for(int i = 0; i < MAXFILE2; ++i){
    if (read(fd, buf, BUFSIZE) != BUFSIZE){
 	  printf("error: write big file failed\n", i);
      exit(1);
    } else {
      for (int j = 0; j < BUFSIZE; ++j) {
      	rand();
      	printf("%d == %d\n", next, buf[j]);
        // if (next != buf[j]) {
          // printf("test failed!\n");
          // exit(0);
        // }
      }
    }
  }
  
  close(fd);
}
