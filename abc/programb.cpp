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
my_shm_open (char * shm_name)
{
    int     size;
    char *  shm_addr;
    
    printf ("Calling shm_open('%s')\n", shm_name);
    shm_fd = shm_open (shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
    printf ("shm_open() returned %d\n", shm_fd);
                
    printf ("Calling lseek(fd=%d,SEEK_END)\n", shm_fd);
    size = lseek (shm_fd, 0, SEEK_END);
    printf ("lseek() returned %d\n", size);
                
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

    while (shm_addr == (char *) MAP_FAILED || *shm_addr != 'a') {
        //Wait until memory is created and a is present
        shm_addr = my_shm_open ("shmb");
    }
    //Read the data.
    printf ("data (@ %#x): '%s'\n", (unsigned int) shm_addr, shm_addr);

    //Change first to capital A
    *shm_addr = 'A';
    return (0);
}
