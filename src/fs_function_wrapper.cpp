#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <wups.h>

static OpenFunction new_open_ptr __attribute__((section(".data"))) = NULL;
static CloseFunction new_close_ptr __attribute__((section(".data"))) = NULL;
static WriteFunction new_write_ptr __attribute__((section(".data"))) = NULL;
static ReadFunction new_read_ptr __attribute__((section(".data"))) = NULL;
static LSeekFunction new_lseek_ptr __attribute__((section(".data"))) = NULL;
static StatFunction new_stat_ptr __attribute__((section(".data"))) = NULL;
static FStatFunction new_fstat_ptr __attribute__((section(".data"))) = NULL;
static OpenDirFunction new_opendir_ptr __attribute__((section(".data"))) = NULL;
static CloseDirFunction new_closedir_ptr __attribute__((section(".data"))) = NULL;
static ReadDirFunction new_readdir_ptr __attribute__((section(".data"))) = NULL;
static MKDirFunction new_mkdir_ptr __attribute__((section(".data"))) = NULL;

#ifdef __cplusplus
extern "C" {
#endif
void WUPS_InitFS(wups_loader_init_fs_args_t args) {
    new_open_ptr =      args.open_repl;
    new_close_ptr =     args.close_repl;
    new_write_ptr =     args.write_repl;
    new_read_ptr =      args.read_repl;
    new_lseek_ptr =     args.lseek_repl;
    new_stat_ptr =      args.stat_repl;
    new_fstat_ptr =     args.fstat_repl;
    new_opendir_ptr =   args.opendir_repl;
    new_closedir_ptr =  args.closedir_repl;
    new_readdir_ptr =   args.readdir_repl;
    new_mkdir_ptr =     args.mkdir_repl;
}

int __real_open(const char *pathname, int flags);
int __wrap_open(const char *pathname, int flags) {
    if(new_open_ptr == NULL) return __real_open(pathname,flags);
    return new_open_ptr(pathname,flags);
}
int __real_close(int fd);
int __wrap_close(int fd) {
    if(new_close_ptr == NULL) return __real_close(fd);
    return new_close_ptr(fd);
}
ssize_t __real_write(int fd, const void *buf, size_t count);
ssize_t __wrap_write(int fd, const void *buf, size_t count) {
    if(new_write_ptr == NULL) return __real_write(fd,buf,count);
    return new_write_ptr(fd,buf,count);
}
ssize_t __real_read(int fd, void *buf, size_t count);
ssize_t __wrap_read(int fd, void *buf, size_t count) {
    if(new_read_ptr == NULL) return __real_read(fd,buf,count);
    return new_read_ptr(fd,buf,count);
}
off_t __real_lseek(int fd, off_t offset, int whence);
off_t __wrap_lseek(int fd, off_t offset, int whence) {
    if(new_lseek_ptr == NULL) return __real_lseek(fd, offset, whence);
    return new_lseek_ptr(fd, offset, whence);
}
int __real_stat(const char *pathname, struct stat *statbuf);
int __wrap_stat(const char *pathname, struct stat *statbuf) {
    if(new_stat_ptr == NULL) return __real_stat(pathname,statbuf);
    return new_stat_ptr(pathname,statbuf);
}
int __real_fstat(int fd, struct stat *statbuf);
int __wrap_fstat(int fd, struct stat *statbuf) {
    if(new_fstat_ptr == NULL) return __real_fstat(fd,statbuf);
    return new_fstat_ptr(fd,statbuf);
}
DIR* __real_opendir(const char * arg);
DIR* __wrap_opendir(const char * arg) {
    if(new_opendir_ptr == NULL) return __real_opendir(arg);
    return new_opendir_ptr(arg);
}
int __real_closedir(DIR *dirp);
int __wrap_closedir(DIR *dirp) {
    if(new_closedir_ptr == NULL) return __real_closedir(dirp);
    return new_closedir_ptr(dirp);
}
struct dirent * __real_readdir(DIR *dirp);
struct dirent * __wrap_readdir(DIR *dirp) {
    if(new_readdir_ptr == NULL) return __real_readdir(dirp);
    return new_readdir_ptr(dirp);
}
int __real_mkdir(const char *path, mode_t mode);
int __wrap_mkdir(const char *path, mode_t mode) {
    if(new_mkdir_ptr == NULL) return __real_mkdir(path, mode);
    return new_mkdir_ptr(path, mode);
}

#ifdef __cplusplus
}
#endif
