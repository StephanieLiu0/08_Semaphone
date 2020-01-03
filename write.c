#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define SHMKEY 1502
#define SEMKEY 1503
#define SIZE 100

int main() {
  printf("trying to get in...\n");
  int semd = semget(SEMKEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1);

  int shmd = shmget(SHMKEY, SIZE, 0644);
  int fd = open("story.txt", O_WRONLY | O_APPEND);
  char * last = shmat(shmd, 0, 0);
  printf("last addition: %s\n", last);

  printf("your addition: ");
  char buff[SIZE];
  fgets(buff, SIZE, stdin);
  write(fd, buff, SIZE);
  close(fd);

  strcpy(last, buff);
  shmdt(last);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}
