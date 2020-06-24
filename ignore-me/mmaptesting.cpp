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
    int file_counter;
    int fd;
    int truncated;
    int demap;
};

MapMemory::MapMemory()
{   // Create a file
    fd = open("./file" + to_string(file_counter), O_RDWR | O_CREAT, S_IRWXU);
    if(fd == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }
    file_counter = 1;

    // Set page size to computer specific
    SIZE = sysconf(_SC_PAGE_SIZE);

    truncated = truncate("./aFile", SIZE);
    if(truncated == -1) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    ADDRESS = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ADDRESS == MAP_FAILED) { std::cerr << "Error: " << strerror(errno) << '\n'; }

    std::cout << "Allocated 1 page of memory. Size: " << SIZE << "\n";
}

MapMemory::~MapMemory()
{

}

void MapMemory::allocate(int size)
{

    // Set wanted page size to be the size of the length * the desired amount
    SIZE = (SIZE * size) + SIZE;
    file_counter++;

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
    // New File of allocation
    map.allocate(1);
    std::cout << "Map Address: "<< map.ADDRESS << " Size: "<< map.SIZE << "\n";
    return 0;
}
