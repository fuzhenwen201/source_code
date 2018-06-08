#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "xlogger.h"
#include "tnode.h"


using namespace snetwork_xservice_tnode;




TNode::TNode(const STTnodeConfig &tnodeconfig)
{
	m_objTnodeconfig.mq_vhost 	= tnodeconfig.mq_vhost ;
	m_objTnodeconfig.mq_exchange_group	= tnodeconfig.mq_exchange_group ;
	m_objTnodeconfig.mq_host	= tnodeconfig.mq_host ;
	m_objTnodeconfig.mq_port	= tnodeconfig.mq_port ;
	m_objTnodeconfig.mq_user	= tnodeconfig.mq_user ;
	m_objTnodeconfig.mq_passwd	= tnodeconfig.mq_passwd ;

	
	pthread_mutex_init(&m_TnodeLock,NULL);

	m_isStopConsumer = false;
	m_isRunConsumer = false;

}

TNode::~TNode()
{
	pthread_mutex_destroy(&m_TnodeLock);
}


int TNode::AddTNodeConsumer( TNodeConsumer *pConsumer)
{
	assert(pConsumer!=NULL);
	pthread_mutex_lock(&m_TnodeLock);
	const char *queuename = pConsumer->GetQueueName().c_str() ;
	const char *exchange = m_objTnodeconfig.mq_exchange_group.c_str() ;


	const char *bindingkey = pConsumer->GetBindingkey().c_str();

	
	 amqp_queue_declare(m_MQConn, 1, amqp_cstring_bytes(queuename), 
                         0, 0, 0, 1,amqp_empty_table);
	if (-1 == CheckAmqpError(amqp_get_rpc_reply(m_MQConn),"queue_declare") ){pthread_mutex_unlock(&m_TnodeLock); return -1; }
		

   	amqp_queue_bind(m_MQConn, 1, amqp_cstring_bytes(queuename), amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey), amqp_empty_table);
	if (-1 == CheckAmqpError(amqp_get_rpc_reply(m_MQConn),"queue_bind bindingkey") ){pthread_mutex_unlock(&m_TnodeLock); return -1; }

	
	amqp_bytes_t consumer;
	consumer.len = 128;
	consumer.bytes = malloc(consumer.len);
	pConsumer->SetSpace((char * )consumer.bytes);
	memset(consumer.bytes, 0 ,consumer.len);
	sprintf((char *)(consumer.bytes),"%ld",pConsumer);
	amqp_basic_consume(m_MQConn, 1, amqp_cstring_bytes(queuename), consumer, 0, 1, 0, amqp_empty_table);
	if (-1 == CheckAmqpError(amqp_get_rpc_reply(m_MQConn),"amqp_basic_consume") )
	{
		pthread_mutex_unlock(&m_TnodeLock);
		free(consumer.bytes);
		return -1; 
	}
	
	m_ConsumerList.push_back(pConsumer) ;

	pthread_mutex_unlock(&m_TnodeLock);

	XINFO("AddTNodeConsumer Sucess: exchange:%s, bindingkey:%s, queue:%s  ...\n",exchange,bindingkey,queuename);

	return 0;
}





int TNode::RemoveTNodeConsumer( TNodeConsumer *pConsumer)
{
	assert(pConsumer!=NULL);
	amqp_bytes_t consumer;
	consumer.len = 32;
	consumer.bytes = pConsumer->GetSpace();
	amqp_basic_cancel(m_MQConn, 1, consumer);
	free(consumer.bytes);
	
	pthread_mutex_lock(&m_TnodeLock);
	m_ConsumerList.remove(pConsumer);
	pthread_mutex_unlock(&m_TnodeLock);

	return 0;
}




int TNode::PublishToMQ(const char * pRoutingKey,  char *pBuf, unsigned int nBufLen,const char *pExchange)				
{
	assert(pRoutingKey != NULL);
	assert(pBuf != NULL);

	const char *pExchange_tmp = NULL;
	if(NULL == pExchange)
	{
		pExchange_tmp = m_objTnodeconfig.mq_exchange_group.c_str() ;
	}
	else
	{
		pExchange_tmp = pExchange ;
	}
	
	if( nBufLen  > TNODE_PACKET_MAXLEN  )
	{
		XERROR("PublishToMQ: packet is too big, more than %d",TNODE_PACKET_MAXLEN) ;
		return -2;
	}
	
	amqp_bytes_t senddata;
	senddata.bytes = (void *)(pBuf) ;
	senddata.len = nBufLen ;

	pthread_mutex_lock(&m_TnodeLock);
	int i = 0;
	i++;
	do
	{
		if(10 == i)i=1;
		int ret = amqp_basic_publish(m_MQConn,
	                                    1,
	                                    //amqp_cstring_bytes(m_objTnodeconfig.mq_exchange_group.c_str()),
	                                    amqp_cstring_bytes(pExchange_tmp),
	                                    amqp_cstring_bytes(pRoutingKey),
	                                    0,
	                                    0,
	                                    &m_props,
	                                    senddata
	                      );
		
		
		if (AMQP_STATUS_OK == ret)break;
		
		XERROR("send to mq fail, pls check !");
		Close();
		
		do
		{
			if(-1 == Init() )
			{
				XERROR("pusher tnode reconnect fail , pls check ! \n");
				sleep(i*10);
				continue;
			}
			break;
		}while(1);

		
	}while(1);
	pthread_mutex_unlock(&m_TnodeLock);

	
	return 0;

	
}










int TNode::CheckAmqpError(amqp_rpc_reply_t x, char const *context) 
{
  switch (x.reply_type) 
  {
	  case AMQP_RESPONSE_NORMAL:
	    return 0;

	  case AMQP_RESPONSE_NONE:
	    XERROR("%s: missing RPC reply type!\n", context);
	    break;

	  case AMQP_RESPONSE_LIBRARY_EXCEPTION:
	    XERROR("%s: %s\n", context, amqp_error_string2(x.library_error));
	    break;

	  case AMQP_RESPONSE_SERVER_EXCEPTION:
	    switch (x.reply.id)
		{
			    case AMQP_CONNECTION_CLOSE_METHOD: 
				{
			      amqp_connection_close_t *m = (amqp_connection_close_t *) x.reply.decoded;
			      XERROR("%s: server connection error %d, message: %.*s\n",
			              context,
			              m->reply_code,
			              (int) m->reply_text.len, (char *) m->reply_text.bytes);
			      break;
			    }
			    case AMQP_CHANNEL_CLOSE_METHOD: 
				{
			      amqp_channel_close_t *m = (amqp_channel_close_t *) x.reply.decoded;
			      XERROR("%s: server channel error %d, message: %.*s\n",
			              context,
			              m->reply_code,
			              (int) m->reply_text.len, (char *) m->reply_text.bytes);
			      break;
			    }
			    default:
			      XERROR("%s: unknown server error, method id 0x%08X\n", context, x.reply.id);
			      break;
		  }
		
		  break;
  	}

  return -1;
}




int TNode::Init()
{
	
	amqp_socket_t *socket = NULL;
	

	int status=0;
	
	m_MQConn = amqp_new_connection();
	if (NULL == m_MQConn)
	{
		XERROR("amqp_new_connection fail ");
		return -1;
	}
	
	socket = amqp_tcp_socket_new(m_MQConn);
	if (NULL == socket)
	{
		XERROR("amqp_tcp_socket_new fail ");
		return -1;
	}

	
	status = amqp_socket_open(socket, m_objTnodeconfig.mq_host.c_str(), m_objTnodeconfig.mq_port);
	if (AMQP_STATUS_OK != status) 
	{
		XERROR("amqp_socket_open fail ");
		return -1;
	}
	
  
	amqp_rpc_reply_t rpcReply = amqp_login(m_MQConn, m_objTnodeconfig.mq_vhost.c_str(), 0, 131072, 0, 
		AMQP_SASL_METHOD_PLAIN, m_objTnodeconfig.mq_user.c_str(), m_objTnodeconfig.mq_passwd.c_str());
	if(-1 == CheckAmqpError(rpcReply, "Logging in")) return -1 ;
	
	amqp_channel_open(m_MQConn, 1);
	rpcReply = amqp_get_rpc_reply(m_MQConn);
	if(-1 == CheckAmqpError(rpcReply, "Opening channel") ) return -1;


	m_props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    m_props.content_type = amqp_cstring_bytes("text/plain");
    m_props.delivery_mode = 2; /* persistent delivery mode */
	
	amqp_exchange_declare(m_MQConn, 1, amqp_cstring_bytes(m_objTnodeconfig.mq_exchange_group.c_str()), 
						amqp_cstring_bytes("topic"),
                        0, 0, 0, 0, amqp_empty_table);

	if(-1 == CheckAmqpError(amqp_get_rpc_reply(m_MQConn),"exchange_declare") )return -1;

	XINFO("Tnode Init sucess ...\n");

	return 0;
	
}


int TNode::Close()
{
	amqp_channel_close(m_MQConn, 1, AMQP_REPLY_SUCCESS);
  	amqp_connection_close(m_MQConn, AMQP_REPLY_SUCCESS);
  	amqp_destroy_connection(m_MQConn);

	XINFO("TNode Close ...\n");

	return 0;
}




