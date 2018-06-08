package com.broctagon.trad.reportserver.serviceimpl;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.common.CommonConstants;
import com.broctagon.trad.reportserver.common.CommonUtil;
import com.broctagon.trad.reportserver.common.DataUtil;
import com.broctagon.trad.reportserver.model.OrdersAns;
import com.broctagon.trad.reportserver.model.OrdersReq;
import com.broctagon.trad.reportserver.model.TradersReq;
import com.broctagon.trad.reportserver.model.LoginAns;
import com.broctagon.trad.reportserver.model.LoginReq;
import com.broctagon.trad.reportserver.model.Order;
import com.broctagon.trad.reportserver.model.SocketMessage;
import com.broctagon.trad.reportserver.model.Trader;
import com.broctagon.trad.reportserver.model.BalanceTrader;
import com.broctagon.trad.reportserver.service.SocketService;
import com.broctagon.trad.reportserver.socket.SocketUtil;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;

import javax.annotation.PreDestroy;


@Service("socketService")
public class SocketServiceImpl implements SocketService{

	private static Logger logger = Logger.getLogger(SocketServiceImpl.class);
	
	public static final int headLength = 8;
	private Socket socket;
	private InputStream inputStream;
	private OutputStream outputStream;

	@Autowired
	private Environment environment;
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	
	private ExecutorService fixedThreadPool;
	
	private LoginAns loginAns;
	public LoginAns getLoginAns(){
		return loginAns;
	}
	
	 public void  init(){  
	      System.out.println("I'm  init  method ");  
	      
	      buildConnection();	
	      
	      fixedThreadPool = Executors.newFixedThreadPool(30);
	      Thread thread = new Thread(){
				@Override
				public void run(){
					subscribe();
				}
			};
		  thread.start();
     
	 }
	 
	 public void buildConnection(){		
		 while(true){
			 try {
					logger.info("start build connection");
					if(socket!=null){
						try{
							socket.close();
							socket=null;
							inputStream.close();
							outputStream.close();
						}
						catch(Exception e){
							e.printStackTrace();
						}
					}
					socket = new Socket(environment.getProperty("socket.ip"), new Integer(environment.getProperty("socket.port")));	
					logger.info("socket.hashCode():" + socket.hashCode());
					
					inputStream = socket.getInputStream();
					outputStream = socket.getOutputStream();
					this.loginAns = postLoginReq();
					if(loginAns!=null&&(loginAns.isLoginstatus()||loginAns.isStatus())){
						sendTradersReq(loginAns);
						sendOrdersReq(loginAns);
						logger.info("the socket connect is built successully.");
						break;
					}
					Thread.sleep(5000);				
				} catch (UnknownHostException e) {
					logger.error(e.getMessage());
					e.printStackTrace();
				} catch (IOException e) {
					logger.error(e.getMessage());
					e.printStackTrace();
				} catch (InterruptedException e) {
					logger.error(e.getMessage());
					e.printStackTrace();
				}
		}
	}
	 	 	 
	public LoginAns postLoginReq() throws IOException{		
		LoginReq loginReq = new LoginReq();
		loginReq.setReqid("b48f1e0db0f546f8a5e983952aabd56e");	
		
		String managerid = environment.getProperty("login.managerid", "1");
		String password = environment.getProperty("login.password", "123");
		logger.info("login, managerid:" + managerid + ",password:" + password);
		
		loginReq.setManagerid(Integer.parseInt(managerid));
		loginReq.setPassword(password);
		loginReq.setMacaddress("2C1B0D4CD495");
		loginReq.setHarddiskserialno("3035323042363237393430333130303030303349");
		String loginBodyStr = JSON.toJSONString(loginReq);		
		logger.info("loginBodyStr:" + loginBodyStr);
		
		byte[] loginReqByte = SocketUtil.prepareEntireBody(CommonConstants.TAG_MGSG_LOGIN_REQ, loginBodyStr);				
		outputStream.write(loginReqByte);
		outputStream.flush();
		String bodyStr = SocketUtil.readBodyStr(inputStream);
		logger.info("bodyStr1=" + bodyStr);
		
		loginAns = JSON.parseObject(bodyStr,LoginAns.class);
		
		return loginAns;
	}
	
	public void sendTradersReq(LoginAns loginAns) throws IOException{
		TradersReq tradersReq = new TradersReq();
		tradersReq.setClientid(0);
		tradersReq.setManagerid(loginAns.getManagerid());
		tradersReq.setReqid("52c7afeffce149e4a84b2e7e55731da3");
		tradersReq.setSession(loginAns.getSession());
		tradersReq.setSgid(2021);
		String tradersReqStr = JSON.toJSONString(tradersReq);
				
		byte[] tradersReqByte = prepareEntireBody(CommonConstants.GET_TRADERS_REQ_TAG, tradersReqStr);	
		String tradersReqByteStr = new String(tradersReqByte);
		logger.debug("tradersReqByteStr="+ tradersReqByteStr);
		outputStream.write(tradersReqByte);
		outputStream.flush();
	}
	
	public void sendOrdersReq(LoginAns loginAns) throws IOException{
		OrdersReq ordersReq = new OrdersReq();
		ordersReq.setClientid(0);
		ordersReq.setManagerid(loginAns.getManagerid());
		ordersReq.setReqid("52c7afeffce149e4a84b2e7e55731da3");
		ordersReq.setSession(loginAns.getSession());
		ordersReq.setTypesoftrade(0);
		String orderSReqStr = JSON.toJSONString(ordersReq);
		
		byte[] ordersReqByte = prepareEntireBody(CommonConstants.TAG_GET_POSITIONS_REQ, orderSReqStr);	
		String ordersReqByteStr = new String(ordersReqByte);
		logger.debug("ordersReqByteStr="+ ordersReqByteStr);
		outputStream.write(ordersReqByte);
		outputStream.flush();
	}
		
	public void subscribe(){
		
		while(true){
			try {
				SocketMessage socketMsg = readContent();
				if(socketMsg==null){
					buildConnection();
					continue;
				}		
						
				Runnable target = new Runnable(){
					@Override
					public void run(){
						try{
							routing(socketMsg);
						}catch (Exception e) {
							e.printStackTrace();
							logger.info("routing(socketMsg):"+e.getMessage());
						}
						
					}
				};	
				new Thread(target).start();
			//	fixedThreadPool.execute(target);
			//	Thread.sleep(10);
			} catch (Exception e) {
				e.printStackTrace();
				logger.info(e.getMessage());
			}
		}	
	}
	
	private class TraderComparator implements Comparator<Trader>{
		@Override
		public int compare(Trader o1, Trader o2) {
			if(o1.getClientid()<o2.getClientid()){
				return -1;
			}
			else{
				return 1;
			}								
		}
	}
	
	public void routing(SocketMessage socketMsg){
		int tag = socketMsg.getTag();
		byte[] body = socketMsg.getBody();
		ShardedJedis shardedJedis = null;
		
		long start = System.currentTimeMillis();
		try{
			shardedJedis = shardedJedisPool.getResource();
			
			logger.info("tag:"+tag);
			String redisKey="";
			switch(tag){
				case CommonConstants.TAG_CM_FORCELOGOUT_ANS:
					logger.info("Force logout. " + ",new String(body):" + new String(body));
					if(socket!=null){
						socket.close();
					}					
					break;
				case CommonConstants.GET_TRADERS_ANS_TAG:
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_INIT_TRADERS;
					storeTraders(body, redisKey, shardedJedis);
					break;
				case CommonConstants.NEW_TRADERS_ANS_TAG:
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_NEW_TRADERS;
					storeTraders(body, redisKey, shardedJedis);
					break;
				case CommonConstants.UPDATE_TRADERS_ANS_TAG:	
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_TRADERS;
					storeTraders(body, redisKey, shardedJedis);
					break;
				case CommonConstants.TAG_UPDATE_TRADER_BALANCE:
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_TRADER_BALANCE;
					storeBalanceTraders(body,redisKey,shardedJedis);					
					break;
				case CommonConstants.TAG_GET_POSITIONS_ANS:
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_INIT_POSITIONS;
					storePositions(body, redisKey, shardedJedis);
					break;		
				case CommonConstants.TAG_UPDATE_POSITION_ONROOT:
					redisKey=CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_POSITION;
					storePositions(body, redisKey, shardedJedis);								
					break;					
				default:logger.error("error tag:"+tag + ",new String(body):" + new String(body));
			}
			
			long end = System.currentTimeMillis();
			long interval = end-start;
			logger.info("tag:" + tag + ",interval:" +interval);
		}
		catch(Exception e){
			e.printStackTrace();
			logger.error(e.getMessage());
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}
		
	}
	
	private void storeTraders(byte[] body, String redisKey, ShardedJedis shardedJedis){
		List<Trader> traderList = DataUtil.getTraderList(body); 					
		if(traderList!=null&&traderList.size()!=0){
			logger.info("traderList.size():" + traderList.size());
			TraderComparator c = new TraderComparator();
			Collections.sort(traderList, c);
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			for(Trader trader: traderList){
				 String traderStr = JSON.toJSONString(trader);
				 pipeline.rpush(redisKey, traderStr);
			}
			pipeline.sync();
		}
		else{
			logger.error("the traderList is empty, the redisKey is " + redisKey);
		}
	}
	
	private void storeBalanceTraders(byte[] body, String redisKey, ShardedJedis shardedJedis){
		List<BalanceTrader> traderBalanceList = DataUtil.getTraderBalanceList(body);
		if(traderBalanceList!=null&&traderBalanceList.size()!=0){
			logger.info("traderBalanceList.size():" + traderBalanceList.size());
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			for(BalanceTrader trader: traderBalanceList){
				 String traderStr = JSON.toJSONString(trader);
				 pipeline.rpush(redisKey, traderStr);
			}
			pipeline.sync();
		}
		else{
			logger.error("the traderList is empty, the redisKey is " + redisKey);
		}
	}
	
	private void storePositions(byte[] body, String redisKey, ShardedJedis shardedJedis){
		String bodyStr = new String(body);
		OrdersAns ordersAns = JSON.parseObject(bodyStr, OrdersAns.class);
		if(ordersAns!=null){
			List<Order> orderList = ordersAns.getBody();
			logger.info("orderList.size():" + orderList.size());
			if(orderList!=null&&orderList.size()!=0){
				ShardedJedisPipeline pipeline = shardedJedis.pipelined();
				for(Order order: orderList){
					String orderStr = JSON.toJSONString(order);
					pipeline.rpush(redisKey, orderStr);
				}
				pipeline.sync();
			}	
		}
		else{
			logger.error("the ordersAns is null. " + " redisKey:" + redisKey + ", bodyStr:" + bodyStr);
		}
	}
					
    private SocketMessage readContent(){
		byte[] head = new byte[headLength];									
		int count=-1;
		try {
			count = inputStream.read(head);
		} catch (IOException e) {
			logger.error("inputStream.read(head):"+e.getMessage());
			e.printStackTrace();
			return null;
		}
		if(count<0){
			logger.error("inputStream.read(head)<0");
			return null;
		}
		
		 byte[] tagByte = new byte[2];
		 for(int i=0; i<2; i++){
			tagByte[i] = head[2+i];
		 }
		short tag = CommonUtil.bytesToShortBig(head, 2);
		int bodyLength = CommonUtil.bytesToIntBig(head, 4);
		
		byte[] body=null;
		try {
			body = readBody(bodyLength,inputStream);
		} catch (IOException e) {
			logger.error("readBody(bodyLength,inputStream):"+e.getMessage());
			e.printStackTrace();
			return null;
		}
		
				
		SocketMessage socketMsg = new SocketMessage();
		socketMsg.setTag(tag);
		socketMsg.setBody(body);
			
		return socketMsg;
	}
    
	private byte[] readBody(int bodyLength, InputStream inputStream) throws IOException{
		
		byte[] body = new byte[bodyLength];
		byte[] readContent = new byte[4096];
		int totalCount=0;
		int readCount=0;
		
		while(bodyLength-totalCount>0){
			if(bodyLength-totalCount>readContent.length){
				readCount = inputStream.read(body,totalCount,readContent.length);
			}
			else{
				readCount = inputStream.read(body,totalCount,bodyLength-totalCount);
			}
			totalCount += readCount;
		}
		
		return body;
	}
		
	private byte[] prepareEntireBody(short tag, String sourceBody){
		
		sourceBody=sourceBody+'\0';
		byte[] bodyByte = sourceBody.getBytes();		
		byte[] outByte = new byte[bodyByte.length+headLength];
					
		outByte[0] = (byte)0x8f;
		
		byte[] shortByte = CommonUtil.shortToBytesBig(tag);			
		outByte[2] = shortByte[0];
		outByte[3] = shortByte[1];
		byte[] intByte = CommonUtil.intToBytesBig(bodyByte.length);
		outByte[4] = intByte[0];
		outByte[5] = intByte[1];
		outByte[6] = intByte[2];
		outByte[7] = intByte[3];
		
		for(int i=0; i<bodyByte.length;i++){
			outByte[headLength+i] = bodyByte[i];
		}
		
		return outByte;	
	}
		
	@PreDestroy
	private void destory(){
		
		System.out.println("I'm  destory  method  using  @@PreDestroy....");  
		
		if(inputStream!=null){
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		if(outputStream!=null){
			try {
				outputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}	
		
	}

}
