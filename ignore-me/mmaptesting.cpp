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

    MapMemory::MapMemory();
    MapMemory::~MapMemory();

    void* allocate(int size);
    void deallocate();

private:
    int fd;
    int truncated;
    int demap;
};

MapMemory::MapMemory()
{
    fd = open("./aFile", O_RDWR | O_CREAT, S_IRWXU);
    if(fd == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    SIZE = sysconf(_SC_PAGE_SIZE);
}

MapMemory::~MapMemory()
{

}

void* MapMemory::allocate(int size)
{
    SIZE = SIZE * (size + 1);

    truncated = truncate("./aFile", SIZE);
    if(truncated == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    ADDRESS = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ADDRESS == MAP_FAILED) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    return ADDRESS;
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

    std::cout << map.allocate(1) << "\n";
    return 0;
}
