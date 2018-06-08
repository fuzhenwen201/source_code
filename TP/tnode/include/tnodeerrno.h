//
// Created by alex on 12/16/16.
//

#ifndef TNODE_ERRNO_H
#define TNODE_ERRNO_H


#define UNIVERSAL_TNODE                                         0x00010000

#define DEFINED_TNODE_ERRNO(errno)                              ((UNIVERSAL_TNODE) | (errno))

#define UNIVERSAL_TNODE_SUCCESS                                 0x0
#define UNIVERSAL_TNODE_UNKNOWN                                 DEFINED_TNODE_ERRNO(1)
#define UNIVERSAL_TNODE_PARAMETER_ERR                           DEFINED_TNODE_ERRNO(2)
#define UNIVERSAL_TNODE_NULL_PTR_ERR                            DEFINED_TNODE_ERRNO(3)
#define UNIVERSAL_TNODE_GROUP_REPEAT_ERR                        DEFINED_TNODE_ERRNO(4)
#define UNIVERSAL_TNODE_CONSUMER_REPEAT_ERR                     DEFINED_TNODE_ERRNO(5)
#define UNIVERSAL_TNODE_CHANNEL_EXCHANGE_DEFINE_ERR             DEFINED_TNODE_ERRNO(6)
#define UNIVERSAL_TNODE_CHANNEL_EXCHANGE_GRP_DEFINE_ERR         DEFINED_TNODE_ERRNO(7)
#define UNIVERSAL_TNODE_CHANNEL_UNKNOWN_ERR                     DEFINED_TNODE_ERRNO(8)
#define UNIVERSAL_TNODE_NOT_READY_ERR                           DEFINED_TNODE_ERRNO(9)
#define UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR                    DEFINED_TNODE_ERRNO(10)
#define UNIVERSAL_TNODE_INVALID_SERVICE_MSG_ERR                 DEFINED_TNODE_ERRNO(11)
#define UNIVERSAL_TNODE_DECLARE_QUEUE_ERR                       DEFINED_TNODE_ERRNO(12)
#define UNIVERSAL_TNODE_RUN_TIMEOUT_ERR                         DEFINED_TNODE_ERRNO(13)
#define UNIVERSAL_TNODE_BOUND_ERR                               DEFINED_TNODE_ERRNO(14)
#define UNIVERSAL_TNODE_UNBIND_ERR                              DEFINED_TNODE_ERRNO(15)


#endif //TNODE_ERRNO_H
