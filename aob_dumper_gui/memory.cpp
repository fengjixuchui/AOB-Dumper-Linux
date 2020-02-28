#include "memory.h"

int Mem::GetProcessID(std::string processName)
{
    int pid = INVALID_PID;
    DIR* pdir = opendir("/proc");
    if(!pdir)
        return INVALID_PID;

    struct dirent* pdirent;
    while(pid < 0 && (pdirent = readdir(pdir)))
    {
        int id = atoi(pdirent->d_name);
        if(id > 0)
        {
            std::string cmdpath = std::string("/proc/") + pdirent->d_name + "/cmdline";
            std::ifstream cmdfile(cmdpath.c_str());
            std::string cmdline;
            getline(cmdfile, cmdline);
            size_t pos = cmdline.find('\0');
            if(!cmdline.empty())
            {
                if (pos != std::string::npos)
                    cmdline = cmdline.substr(0, pos);
                pos = cmdline.rfind('/');
                if (pos != std::string::npos)
                    cmdline = cmdline.substr(pos + 1);
                if (processName == cmdline.c_str())
                    pid = id;
            }
        }
    }
    closedir(pdir);
    return pid;
}

void Mem::ReadBuffer(int pid, off_t address, void* buffer, size_t size)
{
    char file[MAX_FILENAME];
    sprintf(file, "/proc/%ld/mem", (long)pid);
    int fd = open(file, O_RDWR);
    ptrace(PTRACE_ATTACH, pid, 0, 0);
    waitpid(pid, NULL, 0);
    pread(fd, buffer, size, address);
    ptrace(PTRACE_DETACH, pid, 0, 0);
    close(fd);
}

void Mem::WriteBuffer(int pid, off_t address, void* value, size_t size)
{
    char file[MAX_FILENAME];
    sprintf(file, "/proc/%ld/mem", (long)pid);
    int fd = open(file, O_RDWR);
    ptrace(PTRACE_ATTACH, pid, 0, 0);
    waitpid(pid, NULL, 0);
    pwrite(fd, value, size, address);
    ptrace(PTRACE_DETACH, pid, 0, 0);
    close(fd);
}

bool Mem::IsProcessRunning(int pid)
{
    char dirbuf[MAX_FILENAME];
    sprintf(dirbuf, "/proc/%ld", (long)pid);
    struct stat status;
    stat(dirbuf, &status);
    return status.st_mode & S_IFDIR != 0;
}

unsigned int Mem::FileToArrayOfBytes(std::string filepath, char** pbuffer)
{
    std::ifstream filestream(filepath, std::ios::binary | std::ios::ate);
    if(filestream.fail()) return BAD_RETURN;
    
	unsigned int size = filestream.tellg();
    *pbuffer = new char(size);
	filestream.seekg(0, std::ios::beg);
	filestream.read((char*)*pbuffer, size);
	filestream.close();
    return size;
}