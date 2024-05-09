#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

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

int main(int argc, char* argv[]) {
  if (argc < 3) {
  	fprintf(2, "utilite must have 2 arguments!\n");
  	exit(1);
  }

  uint64 seed = atoi(argv[1]);
  uint64 size = atoi(argv[2]);
  
  printf("bigfile test is running!\n");
  int fd = open("big", O_CREATE|O_RDWR);
  if(fd < 0){
    fprintf(2, "error: creat big failed!\n");
    exit(1);
  }

  int cnt = 0;
  srand(seed);
  for(int i = 0; i < size * BUFSIZE; ++i){
    rand();
    buf[cnt++] = next;
    cnt %= BUFSIZE;
    if(!cnt) {	
      if (write(fd, buf, BSIZE) != BSIZE){
      	fprintf(2, "error: write big file failed!\n");
      	exit(1);
      }
      else if (i / BUFSIZE % 1000 == 0) printf("%d/%d blocks writed!\n", i / BUFSIZE, size);
    }
  }

  close(fd);

  fd = open("big", O_RDONLY);
  if(fd < 0){
    fprintf(2, "error: open big failed!\n");
    exit(seed);
  }

  cnt = 0;
  srand(1);
  for(int i = 0; i < size; ++i){
    if (read(fd, buf, BSIZE) != BSIZE){
 	  fprintf(2, "error: write big file failed\n", i);
      exit(1);
    } else {
      for (int j = 0; j < BUFSIZE; ++j) {
      	rand();
        if (next != buf[j]) {
          printf("status: test failed!\n");
          exit(0);
        }
      }
    }
  }

  printf("status: OK\n");
  close(fd);
}
