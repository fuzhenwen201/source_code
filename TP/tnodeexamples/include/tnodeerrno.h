//
// Created by alex on 12/16/16.
//

#ifndef TNODE_ERRNO_H
#define TNODE_ERRNO_H

namespace snetwork_xservice_tnode
{

#define UNIVERSAL_TNODE                                         0x00010000

#define DEFINED_ERRNO(errno)                                    ((UNIVERSAL_TNODE) | (errno))

#define UNIVERSAL_TNODE_SUCCESS                                 0x0
#define UNIVERSAL_TNODE_UNKNOWN                                 DEFINED_ERRNO(1)
#define UNIVERSAL_TNODE_PARAMETER_ERR                           DEFINED_ERRNO(2)
#define UNIVERSAL_TNODE_NULL_PTR_ERR                            DEFINED_ERRNO(3)
#define UNIVERSAL_TNODE_GROUP_REPEAT_ERR                        DEFINED_ERRNO(4)
#define UNIVERSAL_TNODE_CONSUMER_REPEAT_ERR                     DEFINED_ERRNO(5)
#define UNIVERSAL_TNODE_CHANNEL_EXCHANGE_DEFINE_ERR             DEFINED_ERRNO(6)
#define UNIVERSAL_TNODE_CHANNEL_EXCHANGE_GRP_DEFINE_ERR         DEFINED_ERRNO(7)
#define UNIVERSAL_TNODE_CHANNEL_UNKNOWN_ERR                     DEFINED_ERRNO(8)
#define UNIVERSAL_TNODE_NOT_READY_ERR                           DEFINED_ERRNO(9)
#define UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR                    DEFINED_ERRNO(10)
#define UNIVERSAL_TNODE_INVALID_SERVICE_MSG_ERR                 DEFINED_ERRNO(11)
#define UNIVERSAL_TNODE_DECLARE_QUEUE_ERR                       DEFINED_ERRNO(12)

}

#endif //TNODE_ERRNO_H
