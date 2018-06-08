#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include <errno.h>

#include "xlogger.h"
#include "tnode.h"

using namespace std;
using namespace snetwork_xservice_tnode;



int TNode::Consumer(struct timeval *tv)
{
	
	amqp_frame_t frame;
	amqp_rpc_reply_t ret;

	amqp_envelope_t  _envelope;
	amqp_maybe_release_buffers(m_MQConn);


	ret = amqp_consume_message(m_MQConn, &_envelope, NULL, 0);
	if (AMQP_RESPONSE_NORMAL != ret.reply_type) 
	{
		//printf("take non normal msg from rabbitmq!\n");
		if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error)
		{
	    	if (AMQP_STATUS_OK != amqp_simple_wait_frame(m_MQConn, &frame))
			{
				XERROR("Get MSG,MQ connect status is not AMQP_STATUS_OK\n");
				m_isConnMQ = false;
				return -1;
	    	}

			if (AMQP_FRAME_METHOD == frame.frame_type) 
			{
					XERROR("Internal error, AMQP_FRAME_METHOD == frame.frame_type !!!");
					switch (frame.payload.method.id)
					{
						/* if we've turned publisher confirms on, and we've published a message
			             * here is a message being confirmed
			            */
						case AMQP_BASIC_ACK_METHOD:
			          	break;
						
						/* if a published message couldn't be routed and the mandatory flag was set
			           * this is what would be returned. The message then needs to be read.
			           */
						case AMQP_BASIC_RETURN_METHOD:
						{
							amqp_message_t message;
							ret = amqp_read_message(m_MQConn, frame.channel, &message, 0);
							if (AMQP_RESPONSE_NORMAL != ret.reply_type) 
							{
								XERROR("Get MSG,AMQP_BASIC_RETURN_METHOD error \n");
								m_isConnMQ = false;
								return -1;
							}
							
							amqp_destroy_message(&message);
						}
			          	break;

						/* a channel.close method happens when a channel exception occurs, this
			           * can happen by publishing to an exchange that doesn't exist for example
			           *
			           * In this case you would need to open another channel redeclare any queues
			           * that were declared auto-delete, and restart any consumers that were attached
			           * to the previous channel
			           */
						case AMQP_CHANNEL_CLOSE_METHOD:
						{
							m_isConnMQ = false;
							XERROR("Get Msg,AMQP_CHANNEL_CLOSE_METHOD ");
							return -1;
						}
						

						/* a connection.close method happens when a connection exception occurs,
			           * this can happen by trying to use a channel that isn't open for example.
			           *
			           * In this case the whole connection must be restarted.
			           */
						case AMQP_CONNECTION_CLOSE_METHOD:
						{
							m_isConnMQ = false;
							XERROR("Get Msg,AMQP_CONNECTION_CLOSE_METHOD ");
							return -1;
						}
						

						default:
						XERROR("Get MSG,An unexpected method was received %d\n", frame.payload.method.id);
						m_isConnMQ = false;
						return -1;
					}
					
			}
			XERROR("Internal error,AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error !!!");
		}

		XERROR("Internal error, packet loss, Big packet cause !!!");
		
		return -1;

	}	

	if(0==_envelope.message.body.len || NULL == _envelope.message.body.bytes)
	{
		return 0;
	}

	
	TNodeConsumer *pConsumer = (TNodeConsumer *) atol((char *)_envelope.consumer_tag.bytes);

	STTnodeDataPacketHeader *pHeader = (STTnodeDataPacketHeader *)_envelope.message.body.bytes;
	if(htonl(TNODE_STARTER)!=pHeader->starter)
	{
		amqp_destroy_envelope(&_envelope);
		XERROR("Consumer recv Wrong package: starter is not 0x%04x, discarded",TNODE_STARTER);
		return 0;
	}


	pHeader->starter = ntohl(pHeader->starter) ;
	pHeader->tag = ntohs(pHeader->tag);
	pHeader->version = ntohs(pHeader->version);
	pHeader->bodylen = ntohl(pHeader->bodylen);
	
	char *pBody = (char *)_envelope.message.body.bytes + m_TnodeHeaderLen ;

	//char c = pBody[pHeader->bodylen];
	//pBody[pHeader->bodylen]='\0';
	pConsumer->ConsumerData(pHeader,pBody);
	//pBody[pHeader->bodylen] = c;
	amqp_destroy_envelope(&_envelope);

	
	
	return 0;
	
	
}




void *ConsumerThread(void *arg)
{
	assert(arg!=NULL);
	TNode *tnode = (TNode *)arg;

	int ret = 0;
	struct timeval tv;
	tv.tv_sec = 1;

	XINFO("run consumer ...\n");
	while(!tnode->isStopConsumer())
	{

		if(0 == ret){ret = tnode->Consumer(&tv) ;}

		if(-1 == ret )
		{
			if( -1 == (ret = tnode->ReStartConsumer()) )
			{
				sleep(20);
			}
			
		}
	}
	
	XINFO("stop consumer ...\n");

}



void TNode::RunConsumer()
{
	// 3,创建接收线程在这里
	pthread_mutex_lock(&m_TnodeLock);
	if(m_isRunConsumer)
	{
		pthread_mutex_unlock(&m_TnodeLock);
		return ;
	}
	m_isRunConsumer = true;
	pthread_mutex_unlock(&m_TnodeLock);

	pthread_t nThreadID ;
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	if (0 != pthread_create(&nThreadID, &attr, ConsumerThread, this) )
	{
		XERROR("pthread_create error:%s\n",strerror(errno));
		sleep(1);
		exit(-1);
	}

	m_isStopConsumer = false;
}



void 	TNode::StopConsumer()
{
	m_isStopConsumer = true;
}




int		TNode::ReStartConsumer()
{
	
	XERROR("ReStartConsumer begin...\n");
	TNode::Close();

	list<TNodeConsumer *>	ConsumerList;	
	m_ConsumerList.swap(ConsumerList);
	printf("consumer:%d, %d\n",ConsumerList.size(),m_ConsumerList.size());
	m_ConsumerList.clear();
	
	if(-1 == Init() )
	{
		XERROR("ReStartConsumer tnode init fail , pls check ! \n");
		m_ConsumerList.swap(ConsumerList);
		return -1;
	}

	
	list<TNodeConsumer *>::iterator itr;
	for (itr=ConsumerList.begin(); itr!=ConsumerList.end(); itr++)
	{
		TNodeConsumer *pConsumer = (*itr);
		if(-1 == AddTNodeConsumer(pConsumer))
		{
			m_ConsumerList.swap(ConsumerList);
			ConsumerList.clear();
			return -1;
		}
	}

	ConsumerList.clear();
	XERROR("ReStartConsumer end...\n");

	return 0;

}







