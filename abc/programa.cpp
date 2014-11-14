#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

static int  shm_fd = -1;

static char *
my_shm_create (char * shm_name, int size)
{
    int     rtnval;
    char *  shm_addr;
    
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
    shm_addr = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    printf ("mmap() returned %p\n", shm_addr);

    return (shm_addr);
}

int main(void)
{
    char *      shm_addr = (char *) MAP_FAILED;
    int         size = 56;

    shm_addr = my_shm_create ("shmb", size);

    char * pointerToLetter = shm_addr;
    //Write a letter every .25s
    for (char i = 'a'; i <= 'z'; i++) {
        *pointerToLetter = i;
        pointerToLetter++;
        printf("wrote %c\n", i);
        usleep(250000LL);
    }

    while(*shm_addr != 'A') {
        //Wait until capital A is present
    }

    printf("deleting\n");
    shm_unlink("shmb");
    return (0);
}
