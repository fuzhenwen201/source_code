/*******************************************************************************************

*Department:	TP

*Decrible:  	new tnode v2.0 , base on amqp-c 
				关于内存分配,释放: 谁分配谁释放 的原则			

*Auhor:			Savin Chen

*Createdtime:	2017-08-08

*ModifyTime:	


********************************************************************************************/

#ifndef NEW_TNODE_V2_PUBLIC
#define NEW_TNODE_V2_PUBLIC


#include "amqp_tcp_socket.h"
#include "amqp.h"
#include "amqp_framing.h"

#include <assert.h>

#include <string>
#include <list>


using namespace std;


#define TNODE_STARTER 0x009F
#define	TNODE_PACKET_MAXLEN 20*1024*1024
#define REQUESTIDLEN 16
#define	SESSIONIDLEN 16


#define TNODE_VERSION 2


typedef struct _STTnodeConfig
{
	std::string 	mq_vhost;
	std::string		mq_exchange_group;
	std::string		mq_host;
	int				mq_port;
	std::string		mq_user;
	std::string		mq_passwd;

}STTnodeConfig ;




void *ConsumerThread(void *arg);


namespace snetwork_xservice_tnode
{

	class TNodeConsumer 
	{
    public:
        TNodeConsumer(){}

        virtual ~TNodeConsumer(){}

        virtual string GetQueueName() = 0;
		virtual string GetBindingkey() = 0;

		//pHeader, pBody 内存空间由TNode内部管理，ConsumerData调用结束内部回收
	    virtual unsigned int ConsumerData(char *pMsg,int nMsgLen) = 0;
		

		// inner call 
		char *	GetSpace(){ return pSpace ; }
		void	SetSpace(char *space){ pSpace =  space; }

	protected:
		char *pSpace;
      
    };

	
	class TNode
	{
	
	public:
		TNode(const STTnodeConfig &tnodeconfig);
		~TNode();
		
		
		int 	Init();
		int 	Close();

		// if no Consumer then don't call this funcation
		int 	AddTNodeConsumer( TNodeConsumer *pConsumer);
		int 	RemoveTNodeConsumer( TNodeConsumer *pConsumer);
		
		// start consumer  
		void 	RunConsumer();
		// stop consumer 
		void 	StopConsumer();

		

		// if no PublishToMQ then don't call this funcation
		int 	PublishToMQ(const char * pRoutingKey,  char *pBuf, unsigned int nBufLen)	;			


		//  inner call
		int 	Consumer(struct timeval *tv);
		bool	isStopConsumer(){ return m_isStopConsumer ;}
		int		ReStartConsumer();

	private:

		
		int 	CheckAmqpError(amqp_rpc_reply_t x, char const *context) ; 
		

	


	private:
		STTnodeConfig 				m_objTnodeconfig ;
		
		
		amqp_connection_state_t 	m_MQConn;
		amqp_basic_properties_t 	m_props;

		bool 						m_isConnMQ ;
		bool						m_isStopConsumer;

		list<TNodeConsumer*>		m_ConsumerList;
		pthread_mutex_t 			m_TnodeLock;
		
		bool						m_isRunConsumer;
		
	};

}


#endif

