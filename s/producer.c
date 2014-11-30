#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static int  shm_fd = -1;

static int *
my_shm_create (char * shm_name, int size)
{
    int     rtnval;
    int *  shm_addr;
    
    printf ("Calling shm_open('%s')\n", shm_name);
    shm_fd = shm_open (shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
    printf ("shm_open() returned %d\n", shm_fd);
                
    printf ("Calling ftrucate(%d,%d)\n", shm_fd, size);
    rtnval = ftruncate (shm_fd, size);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed");
    }
    printf ("ftruncate() returned %d\n", rtnval);
                
    printf ("Calling mmap(len=%d,fd=%d)\n", size, shm_fd);
    shm_addr = (int *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    printf ("mmap() returned %p\n", shm_addr);

    return (shm_addr);
}

void main(void) {
	int bufferSize = 100;
    unsigned long *      b = (unsigned long *)my_shm_create("s_buffer", sizeof(unsigned long) * bufferSize);
    
    //sem_t *s = sem_open("s", O_CREAT | O_EXCL, 0600, 1);
    sem_t *n = sem_open("n", O_CREAT | O_EXCL, 0600, 0);
    sem_t *e = sem_open("e", O_CREAT | O_EXCL, 0600, bufferSize);
    
    unsigned long value = 0;
    int out = 0;
    
    while (1) {
    	//produce
    	usleep(100000LL);
    	value++;
    	printf("produced: %li\n", value);
    	
    	sem_wait(e);
    	//sem_wait(s);
    	//append
    	if (b[out] == 0) {
       		b[out] = value;
    		out = (out + 1) % bufferSize;
    	} else {
    		printf("Error, buffer is not 0\n");
    		exit(-1);
    	}
    	//sem_post(s);
    	sem_post(n);
    }
    
}

