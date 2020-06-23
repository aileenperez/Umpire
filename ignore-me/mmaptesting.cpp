#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

// class definition
class MapMemory
{
public:
    // data members
    FILE *fd;
    void *ADDRESS;
    int SIZE;
    // constructor
    MapMemory(int size);
    // deconstuctor
    ~MapMemory();

    // method
    void allocate();
    void deallocate();

private:

};

// constructor definition
MapMemory::MapMemory(int size)
{
    SIZE = 4096 * size;
    allocate();
}
// deconstructor definition
MapMemory::~MapMemory()
{
    deallocate();
}

void MapMemory::allocate()
{
    int fd = open("./aFile", O_RDWR | O_CREAT, S_IRWXU);
    if(fd == -1)
    {
	perror("Error: ")
	printf("Error Value: %s\n", )
    }
    truncate("./aFile", SIZE);

    ADDRESS = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
};

void MapMemory::deallocate()
{
    //manmap
    remove("myfile.txt");
};

int main()
{
    MapMemory map(1);

    std::cout << &map.ADDRESS << " ";
    return 0;
}
