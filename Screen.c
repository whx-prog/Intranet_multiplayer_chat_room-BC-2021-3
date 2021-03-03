#include <My_head.h>
struct msg{
    char buf[128];
    int buf_count;
    char name[128];
    int Cur_Online;
    short flag;
    int ID;
};

struct msgsend{
    long mtype;
    struct msg msg_box;

};

int i=0;
int main(int argc,char *argv[])
{ //  ARGS_CHECK(argc,2);
    int msgid=msgget(2333,0666|IPC_CREAT);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgsend m;
    struct msgsend Old_m;
    memset(&m,0,sizeof(m));
    Old_m.mtype=0;
    Old_m.msg_box.ID=0;

    while(1){
        int ret=msgrcv(msgid,&m,sizeof(struct msgsend),0,0);//类型1是自己的输入
        if(m.msg_box.ID==Old_m.msg_box.ID&&m.msg_box.buf[0]==Old_m.msg_box.buf[0]&&m.msg_box.buf[1]==Old_m.msg_box.buf[1]
           &&m.msg_box.buf_count==Old_m.msg_box.buf_count)
            continue;
        i++;
        //printf("读取次数为:%d\n",i);
        ERROR_CHECK(ret,-1,"msgrcv");
        if(m.mtype=2){
            if(m.msg_box.flag==1){  
                //有人加入聊天室
                printf("有人加入了聊天室，在线人数更新为：%d\n",m.msg_box.Cur_Online);
                printf("%s:%s\n",m.msg_box.name,m.msg_box.buf);
                printf("A");

            }
            else if(m.msg_box.flag==0){               //正常输入输出
                //printf("有人加入了条天室，在线人数更新为：%d\n",msg_box.Cur_Online);

                printf("%s说：%s\n",m.msg_box.name,m.msg_box.buf);
                printf("B");
            }
        }
        else if(m.mtype==3){
            int ret=msgctl(msgid,IPC_RMID,NULL);
            ERROR_CHECK(ret,-1,"msgctl");
            puts("聊天结束");
           // pid_t pid;
          //i  pid_t ppid;
            // pid=getpid();
            //     //ppid=getppid();
            //         //kill(pid,SIGKILL);
            //             //kill(ppid,SIGKILL);
            break;
            //                     
        }

        memcpy(&Old_m,&m,sizeof(struct msgsend));
    }return 0;
}
