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
	void allocate(int size);
    void deallocate();

private:

};

// constructor definition
MapMemory::MapMemory(int size)
{
    allocate(size);
}
// deconstructor definition
MapMemory::~MapMemory()
{
    deallocate();
}

// method definition
void MapMemory::allocate(int size)
{
    fd = fopen("afile.txt", "wr");
    ftruncate(fileno(fd), 4096*size);
    fseek(fd, 0L, SEEK_END);
    SIZE = ftell(fd);

    ADDRESS = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
};

void MapMemory::deallocate()
{
    //manmap
    remove("myfile.txt");
};

int main()
{
	// constructor called automatically when creating a class variable (object)
	MapMemory map(1);

    std::cout << &map.ADDRESS << " ";
	return 0;
}
