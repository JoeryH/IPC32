#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Please enter A-D as argumeasdnt\n");
		return -1;
	}
	sem_t * semWait;
	sem_t * semPost;
	int i = 0;
	if (*argv[1] == 'A') {
		semWait = sem_open("k_sem_d", 0);
		semPost = sem_open("k_sem_a", 0);
		i = 1;
	} else if (*argv[1] == 'B') {
		semWait = sem_open("k_sem_a", 0);
		semPost = sem_open("k_sem_b", 0);
		i = 2;
	} else if (*argv[1] == 'C') {
		semWait = sem_open("k_sem_b", 0);
		semPost = sem_open("k_sem_c", 0);
		i = 3;
	} else if (*argv[1] == 'D') {
		semWait = sem_open("k_sem_c", 0);
		semPost = sem_open("k_sem_d", 0);
		i = 4;
	} else {
		printf("Please enter A-D as argument\n");
		return -1;
	}
	for (; i < 9; i+=4) {
		sem_wait(semWait);
		printf("%i\n", i);
		sem_post(semPost);
	}

	return 0;
}