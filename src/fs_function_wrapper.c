#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <wups.h>

static void * new_open_ptr __attribute__((section(".data"))) = NULL; 
static void * new_close_ptr __attribute__((section(".data"))) = NULL; 
static void * new_write_ptr __attribute__((section(".data"))) = NULL; 
static void * new_read_ptr __attribute__((section(".data"))) = NULL; 
static void * new_lseek_ptr __attribute__((section(".data"))) = NULL; 
static void * new_stat_ptr __attribute__((section(".data"))) = NULL; 
static void * new_fstat_ptr __attribute__((section(".data"))) = NULL; 
static void * new_opendir_ptr __attribute__((section(".data"))) = NULL; 
static void * new_closedir_ptr __attribute__((section(".data"))) = NULL; 
static void * new_readdir_ptr __attribute__((section(".data"))) = NULL; 

#ifdef __cplusplus
    extern "C" {
#endif    
    void WUPS_InitFS(wups_loader_init_args_t* args){
        if(args != NULL){
            new_open_ptr =      (void*) args->fs_wrapper.open_repl;
            new_close_ptr =     (void*) args->fs_wrapper.close_repl;
            new_write_ptr =     (void*) args->fs_wrapper.write_repl;
            new_read_ptr =      (void*) args->fs_wrapper.read_repl;
            new_lseek_ptr =     (void*) args->fs_wrapper.lseek_repl;
            new_stat_ptr =      (void*) args->fs_wrapper.stat_repl;
            new_fstat_ptr =     (void*) args->fs_wrapper.fstat_repl;
            new_opendir_ptr =   (void*) args->fs_wrapper.opendir_repl;
            new_closedir_ptr =  (void*) args->fs_wrapper.closedir_repl;
            new_readdir_ptr =   (void*) args->fs_wrapper.readdir_repl;
        }
    } 

    int __real_open(const char *pathname, int flags);
    int __wrap_open(const char *pathname, int flags){
        if(new_open_ptr == NULL) return __real_open(pathname,flags); 
        return ( (int (*)(const char *,int))((unsigned int*)new_open_ptr) )(pathname,flags);
    }
    int __real_close(int fd);
    int __wrap_close(int fd){
        if(new_close_ptr == NULL) return __real_close(fd); 
        return ( (int (*)(int))((unsigned int*)new_close_ptr) )(fd);
    }
    ssize_t __real_write(int fd, const void *buf, size_t count);
    ssize_t __wrap_write(int fd, const void *buf, size_t count){
        if(new_write_ptr == NULL) return __real_write(fd,buf,count); 
        return ( (ssize_t (*)(int, const void *, size_t))((unsigned int*)new_write_ptr) )(fd,buf,count);
    }
    ssize_t __real_read(int fd, const void *buf, size_t count);
    ssize_t __wrap_read(int fd, const void *buf, size_t count){
        if(new_read_ptr == NULL) return __real_read(fd,buf,count); 
        return ( (ssize_t (*)(int, const void *, size_t))((unsigned int*)new_read_ptr) )(fd,buf,count);
    }
    off_t __real_lseek(int fd, off_t offset, int whence);
    off_t __wrap_lseek(int fd, off_t offset, int whence){
        if(new_lseek_ptr == NULL) return __real_lseek(fd, offset, whence); 
        return ( (off_t (*)(int, off_t, int))((unsigned int*)new_lseek_ptr) )(fd, offset, whence);
    }
    int __real_stat(const char *pathname, struct stat *statbuf);
    int __wrap_stat(const char *pathname, struct stat *statbuf){
        if(new_stat_ptr == NULL) return __real_stat(pathname,statbuf); 
        return ( (int (*)(const char *, struct stat *))((unsigned int*)new_stat_ptr) )(pathname,statbuf);
    }
    int __real_fstat(int fd, struct stat *statbuf);
    int __wrap_fstat(int fd, struct stat *statbuf){
        if(new_fstat_ptr == NULL) return __real_fstat(fd,statbuf); 
        return ( (int (*)(int, struct stat *))((unsigned int*)new_fstat_ptr) )(fd,statbuf);
    }
    DIR* __real_opendir(const char * arg);
    DIR* __wrap_opendir(const char * arg){
        if(new_opendir_ptr == NULL) return __real_opendir(arg); 
        return ( (DIR* (*)(const char *))((unsigned int*)new_opendir_ptr) )(arg);
    }
    int __real_closedir(DIR *dirp);
    int __wrap_closedir(DIR *dirp){
        if(new_closedir_ptr == NULL) return __real_closedir(dirp); 
        return ( (int (*)(DIR *))((unsigned int*)new_closedir_ptr) )(dirp);
    }
    struct dirent * __real_readdir(DIR *dirp);
    struct dirent * __wrap_readdir(DIR *dirp){
        if(new_readdir_ptr == NULL) return __real_readdir(dirp); 
        return ( (struct dirent * (*)(DIR *))((unsigned int*)new_readdir_ptr) )(dirp);
    }
    
#ifdef __cplusplus
    }
#endif