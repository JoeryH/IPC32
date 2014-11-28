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

	//init code
	sem_t *count, *verify, *syncCount;
	if (argc == 2) {
		count = sem_open("l_count", O_CREAT, 0600, 0);
		verify = sem_open("l_verify", O_CREAT, 0600, 0);
		syncCount = sem_open("l_syncCount", O_CREAT, 0600, 1);
		printf("creating\n");
		if (count == SEM_FAILED || verify == SEM_FAILED || syncCount == SEM_FAILED) {
			perror("ERROR: sem_open() failed");
			return -1;
		}
	} else {
		count = sem_open("l_count", 0);
		verify = sem_open("l_verify", 0);
		syncCount = sem_open("l_syncCount", 0);
		printf("opening\n");
		if (count == SEM_FAILED || verify == SEM_FAILED || syncCount == SEM_FAILED) {
			perror("ERROR: sem_open() failed");
			return -1;
		}
	}

	//statement 1
	printf("%i: statement 1\n", getpid());
	
	//sync code
	sem_wait(syncCount);
	sem_post(count);
	int value;
	sem_getvalue(count, &value); // semaphore only!! :')
	sem_post(syncCount);
	if (value == 4) {
		for (int i = 0; i < 4; i++) {
			sem_post(verify);
		}
	} else {
		sem_wait(verify);
	}

	//statement 2
	printf("%i: statement 2\n", getpid());
}