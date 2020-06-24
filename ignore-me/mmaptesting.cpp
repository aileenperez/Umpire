#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

class MapMemory
{
public:
    void* ADDRESS;
    int SIZE;

    MapMemory();
    ~MapMemory();

    void allocate(int size);
    void deallocate();

private:
    int fd;
    int truncated;
    int demap;
};

MapMemory::MapMemory()
{   // Create a file 
    fd = open("./aFile", O_RDWR | O_CREAT, S_IRWXU);
    if(fd == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    // Set page size to computer specific
    SIZE = sysconf(_SC_PAGE_SIZE);

    truncated = truncate("./aFile", SIZE);
    if(truncated == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    ADDRESS = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ADDRESS == MAP_FAILED) { std::cerr << "Error: " << strerror(errno) << '\n'; }
}

MapMemory::~MapMemory()
{

}

void MapMemory::allocate(int size)
{
    // Set wanted page size to be the size of the length * the desired amount
    SIZE = SIZE * (size + 1);

    truncated = truncate("./aFile", SIZE);
    if(truncated == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    ADDRESS = mmap(ADDRESS, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ADDRESS == MAP_FAILED) { std::cerr << "Error: " << strerror(errno) << '\n'; }

};

void MapMemory::deallocate()
{
    int demap = munmap(ADDRESS, SIZE);
    if(demap == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    truncated = truncate("./aFile", 0);
    if(truncated == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    close(fd);
    remove("./aFile");
};

int main()
{
    MapMemory map;

    std::cout << "Map Address: "<< map.ADDRESS << " Size: "<< map.SIZE << "\n";
    map.allocate(1);
    std::cout << "Map Address: "<< map.ADDRESS << " Size: "<< map.SIZE << "\n";
    return 0;
}
