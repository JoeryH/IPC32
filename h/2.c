#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int  shm_fd = -1;

//Silent here so it doesnt ruin the output of the other terminal :)
static int *
my_shm_open (char * shm_name)
{
    int     size;
    int *  shm_addr;
    
    //printf ("Calling shm_open('%s')\n", shm_name);
    shm_fd = shm_open (shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm#include <stdio.h>_open() failed");
    }
    //printf ("shm_open() returned %d\n", shm_fd);
                
    //printf ("Calling lseek(fd=%d,SEEK_END)\n", shm_fd);
    size = lseek (shm_fd, 0, SEEK_END);
    //printf ("lseek() returned %d\n", size);
                
    //printf ("Calling mmap(len=%d,fd=%d)\n", size, shm_fd);
    shm_addr = (int *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    //printf ("mmap() returned %p\n", shm_addr);

    return (shm_addr);
}

void main(void) {
    int *      flag = my_shm_open("h_sync");
    int i = 2;
    for (; i < 7; i+=2) {
        while (*flag != 1) {
          
        }
        printf("%i\n", i);
        *flag = 0;
    }
    shm_unlink("h_sync");
}
