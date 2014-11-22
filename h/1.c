#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
    int *      flag = (int *)my_shm_create("h_sync", sizeof(int));
    *flag = 0;
    int i = 1;
    for (; i < 6; i+=2) {
        while (*flag != 0) {
          
        }
        printf("%i\n", i);
        *flag = 1;
    }
    while (*flag != 0) {} //wait for exit
    
}

