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

// union semun {
//   int              val;    /* Value for SETVAL */
//   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//   unsigned short  *array;  /* Array for GETALL, SETALL */
//   struct seminfo  *__buf;  /* Buffer for IPC_INFO
//                               (Linux-specific) */
// };

int create() {
  int shmd = shmget(SHMKEY, SIZE, IPC_CREAT | 0644);
  printf("shared memory created\n");

  int semd = semget(SEMKEY, SIZE, IPC_CREAT | 0644);
  if (semd < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  else {
    union semun us;
    us.val = 1;
    semctl(semd, 0, SETVAL, us);
    printf("semaphore created\n");
  }

  int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  close(fd);
  printf("file created\n");

  return 0;
}

int view() {
  printf("The story so far:\n");
  int fd = open("story.txt", O_RDONLY, 0644);
  if (fd < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  char buff[SIZE];
  while (read(fd, buff, SIZE) > 0)
      printf("%s\n", buff);
  close(fd);

  return 0;
}

void rem() {
  int shmd = shmget(SHMKEY, SIZE, IPC_CREAT | 0644);
  shmctl(shmd, IPC_RMID, 0);
  printf("shared memory removed\n");

  int semd = semget(SEMKEY, SIZE, IPC_CREAT | 0644);
  semctl(semd, IPC_RMID, 0);
  printf("semaphore removed\n");

  remove("story.txt");
  printf("file removed\n");
}

int main(int argc, char *argv[]) {
  if (argc == 2) {
    if (strcmp(argv[1], "-c") == 0)
      create();
    else if (strcmp(argv[1], "-r") == 0)
      rem();
    else if (strcmp(argv[1], "-v") == 0)
      view();
    else
      printf("Enter command line argument -c, -r, or -v\n");
  }
  else
    printf("Enter command line argument -c, -r, or -v\n");

  return 0;
}
