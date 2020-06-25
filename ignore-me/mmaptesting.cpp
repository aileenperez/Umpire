#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
class MapMemory
{
public:
    void* ADDRESS;
    int SIZE;
    const char* file;
    MapMemory();
    ~MapMemory();

    void * allocate(int size);
    void deallocate();

private:
    int file_counter;
    int fd;
    int truncated;
    int demap;
};

MapMemory::MapMemory()
{   // Create a file
    file_counter = 0;
    ADDRESS = NULL;
    SIZE = 0;
}

MapMemory::~MapMemory()
{
    for(int i = 0; i <= file_counter; i++)
    {
        remove(("./file" + std::to_string(i)).c_str());
    }   
}

void * MapMemory::allocate(int size)
{   
    file_counter++;
    file = ("./file" + std::to_string(file_counter)).c_str();

    fd = open(file, O_RDWR | O_CREAT, S_IRWXU);
    if(fd == -1) { std::cerr << "\nLook a Error: " << strerror(errno) << '\n'; }

    truncated = truncate(file, sysconf(_SC_PAGE_SIZE) * size);
    if(truncated == -1) { std::cerr << "\nLook a Error: " << strerror(errno) << '\n'; }

    ADDRESS = mmap(ADDRESS, sysconf(_SC_PAGE_SIZE) * size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ADDRESS == MAP_FAILED) { std::cerr << "\nLook a Error: " << strerror(errno) << '\n'; }

    struct stat srcinfo;
    stat(file, &srcinfo);

    std::cout << "\nMap Address: " << ADDRESS << "\nSize: " << srcinfo.st_size;

    return ADDRESS;
};

void MapMemory::deallocate()
{
    int demap = munmap(ADDRESS, SIZE);
    if(demap == -1) { std::cerr << "Look a Error: " << strerror(errno) << '\n'; }

    truncated = truncate("./aFile", 0);
    if(truncated == -1) { std::cerr << "Look a Error: " << strerror(errno) << '\n'; }
};

int main()
{
    MapMemory map;
    
    // User needs the pointer location and file name

    std::cout << "\nUser Returned Map Address: " << map.allocate(1);

    // New File of allocation
    std::cout << "\nUser Returned Map Address: " << map.allocate(1);

    return 0;
}
