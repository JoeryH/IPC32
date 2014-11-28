#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(void) {
	
	sem_open ("k_sem_a", O_CREAT | O_EXCL, 0600, 0);
	sem_open ("k_sem_b", O_CREAT | O_EXCL, 0600, 0);
	sem_open ("k_sem_c", O_CREAT | O_EXCL, 0600, 0);
	sem_open ("k_sem_d", O_CREAT | O_EXCL, 0600, 1);
	
	return 0;
}