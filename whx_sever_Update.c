#include <My_head.h>
int The_say_onefd;//说话哪个人的sock——fd
int sfd;//服务器sock流的fd
fd_set rdset,rd_box;//rd_box用于更新新加入客户
struct sockaddr_in client_addr[128];//用户sock流结构体
int new_fd=-2;//accpet所返回的用户sock流fd
char out[128]={0};
int client_box[128]={0};//存储所有客户fd的数组
int i=0;
int count=0;//普通计数
int num=0;//在线人数
//char *name[128]={0};
struct msg{             //服务器每次转发的消息包
    char buf[128];
    int buf_count;
    char name[128];
    int Cur_Online;
    short flag;
    int ID;
};
void sigdel(int signum){    //暂时用不上
    close(sfd);
    close(new_fd);
}
int main(int argc,char *argv[])
{  // ARGS_CHECK(argc,2);
    //   signal(SIGTSTP,sigdel);
    ARGS_CHECK(argc,3);
    struct msg msg_box[128];            
    memset(&msg_box,0,sizeof(msg_box)); //初始化信息包
    //  char *buf=(char *)malloc(sizeof(struct msg));
    //  memset(buf,0,sizeof(buf));
    /* printf("**************         *******************");
       printf("******************************************");
       printf("****       ********************       ****");
       printf("***    ***********************************");
       printf("*     ******             *****************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************");
       printf("************* ******** *******************"); */
   /* if(fork())      
        exit(0);
    setsid();           //创建守护进程，使服务器后台运行，只需打开虚拟机即可
    chdir("/");
    umask(0);
    int p;
    for(p=0;p<3;p++)
        close(p);*/
//===============================================================================================    
    sfd=socket(AF_INET,SOCK_STREAM,0);  //创建服务器socket流
    ERROR_CHECK(sfd,-1,"socket");
//===============================================================================================
    struct sockaddr_in ser_addr;            //socket结构体，用于存储服务器开放的端口以及ip地址
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;            //协议
    ser_addr.sin_port=htons(atoi(argv[2]));     //端口
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);    //ip地址
    int ret=bind(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"bind");         //绑定ip地址与端口到服务器socket流上
 //===============================================================================================
    listen(sfd,128);//开始监听服务器socket流

    socklen_t addrlen=sizeof(client_addr);//初始化客户socket结构体
    for(int p=0;p<128;p++){                 //监听容量128，初始化128个客户sock结构体
        memset(&client_addr[p],0,sizeof(client_addr[0]));
    }

    FD_ZERO(&rd_box);           //初始化监听集合(用于更新)
    FD_SET(sfd,&rd_box);        //将服务器与标准输入缓冲加入监听集合
    FD_SET(STDIN_FILENO,&rd_box);

 //==============================================================================================
    while(1){               //服务器进入工作状态
        FD_ZERO(&rdset);    //用于运行
        memcpy(&rdset,&rd_box,sizeof(fd_set));//更新集合
        ret=select(128,&rdset,NULL,NULL,NULL);//将集合加入分选位图
        ERROR_CHECK(ret,-1,"select");
 //===============================================================================================        
        if(FD_ISSET(sfd,&rdset)){       //当服务器sock流有用户流连接时
            new_fd=accept(sfd,(struct sockaddr*)&client_addr[count],&addrlen);//接受连接
            ERROR_CHECK(new_fd,-1,"accept");
            printf("检测道客户端ip为:%s,端口:%d\n",inet_ntoa(client_addr[count].sin_addr),ntohs(client_addr[count].sin_port));
            num++;//在线用户人数增加
            printf("在线人数为:%d\n",num);
            FD_SET(new_fd,&rd_box);//将新来的用户sockfd加入更新集合
            client_box[i++]=new_fd;//将新来的用户sockfd加入客户fd数组（方便转发消息遍历）
            count++;//更换用于存储客户信息的用户sock结构体结点
            msg_box[i].flag=1;//表示有新用户加入
            msg_box[i].ID=rand();//设置用户ID
        }
//===================================================================================================        }
        for(int k=0;k<count;k++){   //遍历用户sockfd数组
            if(client_box[k]!=0&&FD_ISSET(client_box[k],&rdset)){//仅当fd！=0并且该fd中有数据到达
                memset(msg_box[k].buf,0,128);//初始化消息包
               // memset(buf,0,sizeof(buf));
                if(*(msg_box[k].name)=='\0'){//用户第一次进入时输入用户名
                    ret= recv(client_box[k],msg_box[k].name,128,0);//接受用户名
                    ERROR_CHECK(ret,-1,"recv");
                   // continue;
                  // printf("输入任意一条消息进入聊天室\n");
                }
//==============================================================================================================
                ret= recv(client_box[k],msg_box[k].buf,128,0);//不是第一次进入则接受用户消息
                ERROR_CHECK(ret,-1,"recv");
                msg_box[k].buf_count=strlen(msg_box[k].buf);//计算用户输入消息长度并存储
                The_say_onefd=k;//标记说话的用户，便于锁定客户sockfd数组下标
                // strcpy(msg_box[k].buf,buf); 
                //strcat(out,buf);
                // strcat(buf,"whx");
                msg_box[k].Cur_Online=num;//将更新后的在线人数存储
//=============================================================================================================================
                if(ret==0){ //如果用户断开连接
                    //printf("客户端断开链接\n");
                    FD_CLR(client_box[k],&rd_box);//清空分选池该用户对应的fd
                    client_box[k]=0;    //清空该用户在客户数组的fd
                    num--;//在线人数减少
                    memset(&msg_box[k],0,sizeof(struct msg));//清空该客户对应socket结构体结点内容
                    continue;//跳过
                }
//==============================================================================================================================
                printf("服务器接收到客户端信息:%s\n",msg_box[k].buf);//用于检测客户对话
                printf("开始转发信息。。。。\n");//测试否转发成功
                for(int j=0;j<count;j++){          //将这次说话用户的消息转发给所有其它客户
                    //  memset(buf,0,sizeof(buf));
                    // memcpy(buf,&msg_box[j],sizeof(buf));
                    if(client_box[j]==0)//跳过那些已经退出的客户
                        continue;
                    ret=send(client_box[j],&msg_box[The_say_onefd],sizeof(struct msg),0);//转发消息包
                    ERROR_CHECK(ret,-1,"send");
                    printf("回信成功,回信内容为:%s\n",msg_box[The_say_onefd].buf);//转发成功
                    msg_box[j].flag=0;//正常状态标记
                }

            }
        }
   // }
}
    close(sfd);
    close(new_fd);
    return 0;

}
