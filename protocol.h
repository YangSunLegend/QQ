#ifndef PROTOCOL_H
#define PROTOCOL_H

#define DEFAULT_SERVER_IP   "192.168.1.211"
/*
    registerInfo
*/
#define REGISTER_SERVER_IP          DEFAULT_SERVER_IP
#define REGISTER_SERVER_PORT     6666
#define REGISTER_STATUS_OK         0
#define REGISTER_STATUS_ERROR   1
#define COUNT_SIZE                        64


struct register_st {
    char count[COUNT_SIZE];
    char passwd[COUNT_SIZE];
    int status;
}__attribute__((packed));

/*登录*/
#define LOGIN_SERVER_IP         DEFAULT_SERVER_IP
#define LOGIN_SERVER_PORT    7777
#define LOGIN_STATUS_OK         0
#define LOGIN_STATUS_ERROR   1

struct login_st {
    char count[COUNT_SIZE];
    char passwd[COUNT_SIZE];
    int status;
}__attribute__((packed));

/*添加好友*/
#define ADDFRIEND_SERVER_IP         DEFAULT_SERVER_IP
#define ADDFRIEND_SERVER_PORT    8888
#define ADD_STATUS_OK                   0
#define ADD_STATUS_ERROR             1
#define ADD_STATUS_EXISTS             2

struct addfrd_st {
    char src_count[COUNT_SIZE];
    char dst_count[COUNT_SIZE];
    int status;
}__attribute__((packed));


/*记住地址*/
#define REMEMBER_ADDR_IP        DEFAULT_SERVER_IP
#define REMEMBER_ADDR_PORT   11111

struct remember_st {
    char count[COUNT_SIZE];
}__attribute__((packed));


/*聊天*/
#define CHAT_SERVER_IP             DEFAULT_SERVER_IP
#define CHAT_SERVER_PORT        9999
#define CHAT_STATUS_MSG         0
#define CHAT_STATUS_LIST          1

struct chat_st {
    char src_count[COUNT_SIZE];
    char dst_count[COUNT_SIZE];
    char msg[COUNT_SIZE];
    int status;
}__attribute__((packed));


#endif // PROTOCOL_H
