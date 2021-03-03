#include <My_head.h>
int ret;
char buf[128]={0};
int i=0;

struct msg{
   char buf[128];
   int buf_count;
    char name[128];
    int Cur_Online;
    short flag;
    int ID;
};
pthread_t pthid;

struct msgsend{
    long mtype;
    struct msg msg_box;
};

int My_msgsend(struct msg *msg_box,int msgtype){
    int msgid=msgget(2333,IPC_CREAT|0666);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgsend msg;
    memset(&msg,0,sizeof(struct msgsend));
    msg.mtype=msgtype;
   // printf("客户端收到：%s %s\n",msg_box->name,msg_box->buf);
    memcpy(&msg.msg_box,msg_box,sizeof(struct msg));
  //  printf("客户端收到：%s %s\n",msg.msg_box.name,msg.msg_box.buf);
    int ret=msgsnd(msgid,&msg,sizeof(msg.msg_box),0);
    i++;
//    printf("发送msg次数%d\n",i);
    ERROR_CHECK(ret,1,"msgsnd");
}

void sigdel(int signum){
    struct msg send;
    strcpy(send.buf,"聊天关闭");
    strcpy(send.buf,"客户端");
    send.flag=0;
    send.Cur_Online=-1;
    My_msgsend(&send,3);
}

int main(int argc,char *argv[])
{ //  ARGS_CHECK(argc,2);
    ARGS_CHECK(argc,3);
    struct msg msg_box;
    memset(&msg_box,0,sizeof(struct msg));
    signal(SIGINT,sigdel);
    //char *buf=(char *)malloc(sizeof(msg_box));
   // memset(buf,0,sizeof(buf));

    int sfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    struct sockaddr_in ser_addr;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(atoi(argv[2]));
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);
    ret=connect(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"connect");
    fd_set rdset;
    printf("请输入您的用户名：\n");
   // close(STDOUT_FILENO);
    while(1){
    FD_ZERO(&rdset);
    FD_SET(sfd,&rdset);
    FD_SET(STDIN_FILENO,&rdset);
    ret=select(sfd+1,&rdset,NULL,NULL,NULL);
    ERROR_CHECK(ret,-1,"select");
    //------------------------------------------------------------------------------------------------------------------------
    
    if(FD_ISSET(STDIN_FILENO,&rdset)){      //STDIN 读入消息
    memset(buf,0,128);
    ret=read(STDIN_FILENO,buf,128);
    ERROR_CHECK(ret,-1,"read");
    if(ret==0){
        My_msgsend(&msg_box,3);
        printf("结束聊天成功\n");
        break;
    }
    ret=send(sfd,buf,128,0);
    ERROR_CHECK(ret,-1,"send");
    printf("\n");
    }
    //----------------------------------------------------------------------------------------------------------------------------1

    if(FD_ISSET(sfd,&rdset)){               //接受消息
    memset(&msg_box,0,sizeof(msg_box));
    ret=recv(sfd,&msg_box,sizeof(msg_box),0);
    ERROR_CHECK(ret,-1,"recv");
   // i++;
  //  printf("从服务器读取消息次数:%d\n",i);
   // memcpy(&msg_box,buf,sizeof(buf));
    if(ret==0){
      //  My_msgsend(&msg_box,3);
        printf("服务器断开链接\n");         //服务器端断开连接
        break;
    }
    
    My_msgsend(&msg_box,2);

    



   /* if(msg_box.flag==1){  
                                           //有人加入聊天室
        printf("有人加入了聊天室，在线人数更新为：%d\n",msg_box.Cur_Online);
         printf("%s:%s\n",msg_box.name,msg_box.buf);
         
    }
    else if(msg_box.flag==0){               //正常输入输出
        //printf("有人加入了条天室，在线人数更新为：%d\n",msg_box.Cur_Online);
      /* while(1){
          int ret; 
          char str[128]={0};
        if((ret=read(STDIN_FILENO,str,128))==0)
            goto goal;
       }
goal:*/
      //  printf("%s说：%s\n",msg_box.name,msg_box.buf);
         
   // }


    }
 }
    close(sfd);
    return 0;
}

