#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int shm_fd = -1;

static int *
my_shm_open(char * shm_name) {
	int size;
	int * shm_addr;

	printf ("Calling shm_open('%s')\n", shm_name);
	shm_fd = shm_open(shm_name, O_RDWR, 0600);
	if (shm_fd == -1) {
		perror("ERROR: shm#include <stdio.h>_open() failed");
	}
	printf ("shm_open() returned %d\n", shm_fd);

	printf ("Calling lseek(fd=%d,SEEK_END)\n", shm_fd);
	size = lseek(shm_fd, 0, SEEK_END);
	printf ("lseek() returned %d\n", size);

	printf ("Calling mmap(len=%d,fd=%d)\n", size, shm_fd);
	shm_addr = (int *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
			shm_fd, 0);
	if (shm_addr == MAP_FAILED) {
		perror("ERROR: mmap() failed");
	}
	printf ("mmap() returned %p\n", shm_addr);

	return (shm_addr);
}

void main(void) {
	int bufferSize = 100;
	unsigned long * b = (unsigned long *) my_shm_open("s_buffer");

	//sem_t *s = sem_open("s", 0);
	sem_t *n = sem_open("n", 0);
	sem_t *e = sem_open("e", 0);

	unsigned long value = 0;
	int in = 0;

	while (1) {
		sem_wait(n);
		//sem_wait(s);
		//take
		value = b[in];
		b[in] = 0;
		in = (in + 1) % bufferSize;
		//sem_post(s);
		sem_post(e);

		//consume
		printf("consumed: %li\n", value);
		usleep(80000LL);
	}
}
