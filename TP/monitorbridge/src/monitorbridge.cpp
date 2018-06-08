//
// Created by kevin on 7/6/17.
//

#include "monitorbridge.h"
#include <vector>
DEFINE_string(loggerDir, "../logs", "logger directory");
DEFINE_string(loggerFile, "serverconfig", "logger file name");
DEFINE_string(sgdip, "127.0.0.1", "sgdistributor ip");
DEFINE_string(sgdport, "5031", "sgdistributor port");
DEFINE_string(mqvhost, "/demo", "rabbitmq vhost");
DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
DEFINE_string(mqAddress, "localhost", "rabbitmq ip address");
DEFINE_string(mqPort, "5672", "rabbitmq port");
DEFINE_string(mqUser, "demo", "rabbitmq user");
DEFINE_string(mqPassword, "demo", "rabbitmq password");

DEFINE_string(Invalid, "Invalid", "Invalid input parameters!");

static int32_t  socket_fd = 0;
bool g_connect_flags = false;
static std::vector<STINServiceGWBody> local_body;
bool reconnect_send = false;

static void socket_read_callback(struct ev_loop *loop, struct ev_io *watcher, int revents){
    STInServiceAck msgack;
    ssize_t read;

    XINFO("callback socket_read_callback successful!");
    if(EV_ERROR & revents) {
        XERROR("error event in read");
        return ;
    }

    //正常的recv
    read = recv(watcher->fd, &msgack, 1024, 0);
    if(read < 0) {
        XERROR("read error");
        return;
    } else if(read == 0) {
        XERROR("client disconnected.");

        //如果客户端断开连接，释放响应的资源，并且关闭监听
        close(watcher->fd);
        ev_io_stop(loop, watcher);
        g_connect_flags = false;
        reconnect_send = true;
        return;
    } else {
        XINFO("receive message from SGD successful! %ld,%ld",read, sizeof(STInServiceAck));
    }
}

MonitorBridge::MonitorBridge() {
}

MonitorBridge::~MonitorBridge() {
}

void MonitorBridge::ParseCommandLine(int* argc, char*** argv, bool cv) {
    gflags::ParseCommandLineFlags(argc, argv, cv);
}

const std::string& MonitorBridge::Getconfig(const std::string& str) {
    if(str == "loggerdir")
        return FLAGS_loggerDir;
    else if(str == "loggerfile")
        return FLAGS_loggerFile;
    else if(str == "sgdip")
        return FLAGS_sgdip;
    else if(str == "sgdport")
        return FLAGS_sgdport;
    else if(str == "mqvhost")
        return FLAGS_mqvhost;
    else if(str == "mqExchange")
        return FLAGS_mqExchange;
    else if(str == "mqExchangeGroup")
        return FLAGS_mqExchangeGroup;
    else if(str == "mqAddress")
        return FLAGS_mqAddress;
    else if(str == "mqPort")
        return FLAGS_mqPort;
    else if(str == "mqUser")
        return FLAGS_mqUser;
    else if(str == "mqPassword")
        return FLAGS_mqPassword;
     else {
        return FLAGS_Invalid;
    }
}

MonitorConsumer::MonitorConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel) : channel_(channel){
    instance_id_ = instance_id;
}

MonitorConsumer::~MonitorConsumer(){

}

std::string MonitorConsumer::GetEventName(){
    return instance_id_;
}

uint32_t MonitorConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen) {
    assert(event != nullptr);

    if (0xA007 != tag || 0 == eventlen) {
        XERROR("Get the message of tag is %d and eventlen is %d", tag, eventlen);
        return -1;
    }

    //printf("Received Monitor Msg: %s\n", event);
    do {
        rapidjson::Document doc;
        doc.Parse((const char *) event);
        if (doc.HasParseError()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("json format error, parse code %d", code);
            break;
        }

        STINServiceGWMsg GateWayMsg;

        rapidjson::Value::ConstMemberIterator it;

        if ((it = doc.FindMember("reqid")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no reqid members or it's not string type in the message!");
            break;
        }
        strncpy(GateWayMsg.msgheader.requestid, it->value.GetString(), 32);

        if ((it = doc.FindMember("mid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("There are no mid members or it's not uint type in the message!");
            break;
        }
        GateWayMsg.monitor_id = htons(it->value.GetUint());

        if ((it = doc.FindMember("total")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("There are no total members or it's not uint type in the message!");
            break;
        }
        uint32_t number = it->value.GetUint();
        GateWayMsg.numbers = htons(number);

        if ((it = doc.FindMember("body")) == doc.MemberEnd() || !it->value.IsArray()){
            XERROR("There are no body members or it's not an array in the message!");
            break;
        }

        uint32_t sendlen = sizeof(GateWayMsg) + number * sizeof(STINServiceGWBody);
        GateWayMsg.msgheader.msgid = htonl(0x0010);
        GateWayMsg.msgheader.msglen = htons(sendlen);
        GateWayMsg.msgheader.version = htons(1);
        GateWayMsg.msgheader.starter = htonl(0x008F);
        GateWayMsg.inclient_type = htons(1);


        char * buff = (char *)malloc(sendlen);
        memset(buff, 0 ,sendlen);

        memcpy(buff, &GateWayMsg, sizeof(GateWayMsg));
        char *tmp = buff + sizeof(GateWayMsg);
        bool bool_send = false;
        rapidjson::Value& valArray = doc["body"];
        for (rapidjson::SizeType i = 0; i < number; ++i){
            STINServiceGWBody body;
            rapidjson::Value::ConstMemberIterator ita;
            if((ita = valArray[i].FindMember("type")) == valArray[i].MemberEnd() || !ita->value.IsUint()){
                XERROR("There are no type members or it's not uint type in the body array!");
                continue;
            }
            body.op_type = htons(ita->value.GetUint());

            if((ita = valArray[i].FindMember("ip")) == valArray[i].MemberEnd() || !ita->value.IsString()){
                XERROR("There are no ip members or it's not string type in the body array!");
                continue;
            }
            body.gwip = inet_addr(ita->value.GetString());

            if((ita = valArray[i].FindMember("port")) == valArray[i].MemberEnd() || !ita->value.IsUint()){
                XERROR("There are no type members or it's not uint type in the body array!");
                continue;
            }
            body.gwport = htonl(ita->value.GetUint());

            if((ita = valArray[i].FindMember("curruser")) == valArray[i].MemberEnd() || !ita->value.IsUint()){
                XERROR("There are no curruser members or it's not uint type in the body array!");
                continue;
            }
            body.curusercount = htonl(ita->value.GetUint());

            if((ita = valArray[i].FindMember("maxuser")) == valArray[i].MemberEnd() || !ita->value.IsUint()){
                XERROR("There are no maxuser members or it's not uint type in the body array!");
                continue;
            }
            body.maxusercount = htonl(ita->value.GetUint());
            std::vector<STINServiceGWBody>::iterator it;
            bool tmp_bool = false;
            bool ip_port = false;
            if (!reconnect_send) {
                for (it = local_body.begin(); it != local_body.end();) {
                    if (body.gwip == it->gwip && body.gwport == it->gwport) {
                        ip_port = true;
                    }

                    if (body.op_type == it->op_type && body.curusercount == it->curusercount &&
                        body.maxusercount == it->maxusercount) {
                        tmp_bool = true;
                    }

                    if (ip_port && !tmp_bool) {
                        it = local_body.erase(it);
                        break;
                    } else if (ip_port) {
                        break;
                    } else {
                        ++it;
                    }
                }

                if (ip_port && tmp_bool) {
                    //printf("**************\n");
                    continue;
                }
                local_body.push_back(body);
            }
            XINFO("Send massage to SGD:ip %s  port %d  curusercount %d maxusercount %d op_type %d\n",
                  inet_ntoa(*((struct in_addr*)&body.gwip)),ntohl(body.gwport),ntohl(body.curusercount),ntohl(body.maxusercount),ntohs(body.op_type));
            memcpy(tmp, &body, sizeof(STINServiceGWBody));
            tmp += sizeof(STINServiceGWBody);
            bool_send = true;
            reconnect_send = false;
        }

        if(!bool_send){
            free(buff);
            return -1;
        }

        const char* cp = buff;
        uint32_t remain = sendlen;
        ssize_t written = 0;
        while (remain) {
            written = send(socket_fd, cp, remain, 0);
            if (written <= 0) {
                break;
            }
            remain -= written;
            cp += written;
        }
        if (written <= 0) {
            free(buff);
            XERROR("Send massage to SGD failure!");
             break;
        }
        free(buff);
        //printf("Send massage to SGD successful!sendlen:%d,written:%d",sendlen,written);
        return 0;

    } while (false);

    return -1;
}

uint32_t MonitorConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("\nERRNO %d, No implement\n", -1);
    return -1;
}

int32_t MonitorConsumer::ConnectService(const char* ip, uint16_t port){
    int ret = 0;
    if (-1 == (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        XERROR("Create socket error!");
        return -1;
    }
    XINFO("create socket successful:%s",ip);

    int flags = fcntl(socket_fd, F_GETFL, 0);

    if (-1 == fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK)) {
        XERROR("set client socket non-blocking");
        return -1;
    }
    XINFO("set non-blocking successful.");
    struct sockaddr_in socketaddr;
    bzero(&socketaddr, sizeof(socketaddr));
    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(port);
    socketaddr.sin_addr.s_addr = inet_addr(ip);
    int32_t res = connect(socket_fd, (struct sockaddr *) &socketaddr, sizeof(socketaddr));
    if (0 == res) {
        XINFO("socket connect succeed immediately.");
        ret = 0;
    } else {
        XINFO("get the connect result by select().");
        if (errno == EINPROGRESS) {
            int times = 0;
            while (times++ < 5) {
                fd_set rfds, wfds;
                struct timeval tv;

                FD_ZERO(&rfds);
                FD_ZERO(&wfds);
                FD_SET(socket_fd, &rfds);
                FD_SET(socket_fd, &wfds);

                /* set select() time out */
                tv.tv_sec = 10;
                tv.tv_usec = 0;
                int selres = select(FD_SETSIZE, &rfds, &wfds, NULL, &tv);
                switch (selres) {
                    case -1:
                        XERROR("select error");
                        ret = -1;
                        break;
                    case 0:
                        XERROR("select time out");
                        ret = -1;
                        break;
                    default: {
                        connect(socket_fd, (struct sockaddr *) &socketaddr, sizeof(struct sockaddr_in));
                        int err = errno;
                        if (err == EISCONN) {
                            ret = 0;
                        } else {
                          //  XERROR("connect failed. errno:%d, FD_ISSET(sock_fd,&rfds):%d,FD_ISSET(sock_fd,&wfds):%d",
                           //        errno,FD_ISSET(socket_fd, &rfds), FD_ISSET(socket_fd, &wfds));
                            ret = errno;
                        }
                        break;
                    }
                }
                if (-1 != selres && (ret != 0))
                {
                    XINFO("check connect result again... %d", times);
                    continue;
                } else {
                    break;
                }
            }
        } else {
            XERROR("connect to host %s:%d failed.", ip, port);
            ret = errno;
        }
    }

    if (0 == ret) {
        XERROR("connect finished succeed!");
        auto *loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);

        // initialize the connect callback so that it starts the stdin asap
        struct ev_io *receive_watcher = (struct ev_io *) malloc(sizeof(struct ev_io));
        if (receive_watcher == NULL) {
            XERROR("malloc error in receive_watcher!");
            return -1;
        }

        ev_io_init(receive_watcher, socket_read_callback, socket_fd, EV_READ);
        ev_io_start(loop, receive_watcher);

        ev_run(loop, 0);
        ev_loop_destroy(loop);
    }
    close(socket_fd);
    return ret;
}
