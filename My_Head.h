#ifndef _HEAD_
#define _HEAD_
#define _GNU_SOURCE
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<wait.h>
#include <time.h>
#include <syslog.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include<sys/time.h>
#include<pthread.h>
#include<malloc.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include<memory.h>
//#include <tcp_net_socket.h>
#define ARGS_CHECK(argc,num) { if(argc!=num){\
        fprintf(stderr,"error\n");\
        return -1;\
}}
#define ERROR_CHECK(ret,num,msg) {if(ret==num){\
        perror(msg);\
        printf("行号：%d\n",__LINE__);\
        return -1;\
} }
#define THREAD_ERROR_CHECK(ret,funcname) {if(ret!=0) {printf("%s failed %s\n",funcname,strerror(ret));return -1;}}


#endif

