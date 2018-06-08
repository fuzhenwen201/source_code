package com.broctagon.trad.reportserver.serviceimpl;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.common.CommonConstants;
import com.broctagon.trad.reportserver.common.CommonUtil;
import com.broctagon.trad.reportserver.common.Instrument;
import com.broctagon.trad.reportserver.model.RealtimeData;
import com.broctagon.trad.reportserver.model.RealtimeDatas;
import com.broctagon.trad.reportserver.sbe.RealtimedataUtil;
import com.broctagon.trad.reportserver.model.SocketMessage;
import com.broctagon.trad.reportserver.service.RealTimeSocketService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;

import javax.annotation.PreDestroy;
import javax.annotation.PostConstruct;

@Service("realTimeSocketService")
public class RealTimeSocketServiceImpl implements RealTimeSocketService{

	private static Logger logger = Logger.getLogger(RealTimeSocketServiceImpl.class);
	
	public static final int REQ_ID_LENGTH = 36;
	public static final int PRICE_REQ_HEAD_LENGTH = 60;
	public static final int PRICE_OK_HEAD_LENGTH = 60;
	public static final int PRICE_ANS_HEAD_LENGTH = 66;
	
	public static final short REAL_TIME_VERSION = 1;
	public static final short OPERATION_TYPE = 1;

	@Autowired
	private Environment environment;
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	
	private Socket realTimeSocket;
//	@Autowired @Qualifier("priceInputStream") 
	private InputStream priceInputStream;
//	@Autowired @Qualifier("priceOutputStream")
	private OutputStream priceOutputStream;
	
	private ExecutorService fixedThreadPool;
		
//	 @PostConstruct  
	 public void  init(){  
	    System.out.println("I'm  init  method");  		    	  
	   
	    buildConnection();
	    	    
	    fixedThreadPool = Executors.newFixedThreadPool(30);
	    Thread priceThread = new Thread(){
			@Override
			public void run(){
			priceSubscribe();					
			}
		};
		priceThread.start();						  
	}
	 
	public void buildConnection(){		
		while(true){
			try {
				logger.info("build connection");
				if(realTimeSocket!=null){
/*					boolean connected = realTimeSocket.isConnected();
					boolean closed = realTimeSocket.isClosed();
					logger.info("connected:"+connected);
					logger.info("closed:"+closed);*/
					try{
						realTimeSocket.close();
						realTimeSocket=null;
						priceInputStream.close();
						priceOutputStream.close();
					}
					catch(Exception e){
						e.printStackTrace();
					}
				}
				realTimeSocket = new Socket(environment.getProperty("realtime.socket.ip"), new Integer(environment.getProperty("realtime.socket.port")));	
				logger.info("realTimeSocket.hashCode():" + realTimeSocket.hashCode());
				
				priceInputStream = realTimeSocket.getInputStream();
				priceOutputStream = realTimeSocket.getOutputStream();
				sendPriceReq();
				if(readRealTimeErrNo()){
					logger.info("the realTimeSocket connect is built successully.");
					break;
				}
				Thread.sleep(5000);				
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void priceSubscribe(){		
		while(true){
			try {
				if(realTimeSocket!=null){
					boolean connected = realTimeSocket.isConnected();
					if(connected==false){
						logger.info("connected:"+connected);
					}					
				}
				SocketMessage socketMsg=readRealTimeAns();	
				if(socketMsg==null){
					buildConnection();
				}
								
				Runnable target = new Runnable(){
					@Override
					public void run(){
						routing(socketMsg);
					}
				};	
			//	new Thread(target).start();
				fixedThreadPool.execute(target);	
			//	Thread.sleep(100);
			} 
			catch (Exception e) {
				e.printStackTrace();
			}
						
		}	
	}
	
	public void routing(SocketMessage socketMsg){
		int tag = socketMsg.getTag();
		byte[] body = socketMsg.getBody();
		ShardedJedis shardedJedis = null;
	//	logger.info("tag:"+tag);
		long start = System.currentTimeMillis();
		
		try{
			shardedJedis = shardedJedisPool.getResource();
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			switch(tag){					
				case CommonConstants.REALTIME_PRICE_ANS_TAG:				
					RealtimeDatas realtimeDatas = RealtimedataUtil.decode(body);
					List<RealtimeData> realtimeDataList = realtimeDatas.getRealtimeDatas();
					logger.info("realtimeDataList.size():" + realtimeDataList.size());
		//			logger.debug(JSON.toJSONString(realtimeDataList.get(0)));
					if(realtimeDataList!=null){
						for(RealtimeData realtimeData : realtimeDataList){
							String realtimeDataStr = JSON.toJSONString(realtimeData);
							pipeline.set(realtimeData.getInstrument(), realtimeDataStr);
						// pipeline.expire(realtimeData.getInstrument(), 1);
						}
					}
					break;
				default:logger.error("error tag:"+tag);
			}
			pipeline.sync();
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
			
	public boolean readRealTimeErrNo(){
		byte[] head = new byte[PRICE_REQ_HEAD_LENGTH];
		try {
			priceInputStream.read(head);
			int errno = CommonUtil.bytesToIntBig(head, 56);		
			logger.info("real real time errno :" + errno);
			if(errno==0){
				return true;
			}
		} catch (IOException e) {
			logger.error(e.getMessage());
			e.printStackTrace();
		}

		return false;		
	}
	
	public SocketMessage readRealTimeAns(){
		byte[] head = new byte[PRICE_ANS_HEAD_LENGTH];
		int count=-1;
		try {
			count = priceInputStream.read(head);
		} catch (IOException e) {
			logger.error("priceInputStream.read(head):"+e.getMessage());
			e.printStackTrace();
			return null;
		}
		if(count<0){
			logger.error("priceInputStream.read(head)<0");
			return null;
		}
		short length = CommonUtil.bytesToShortBig(head, 4);
		short bodyLength = (short) (length- PRICE_ANS_HEAD_LENGTH);
		int tag = CommonUtil.bytesToIntBig(head, 8);
		logger.debug("bodyLength:"+bodyLength + ",tag:" + tag);
		
		byte[] body=null;
		try {
			body = readBody(bodyLength,priceInputStream);
		} catch (IOException e) {
			logger.error("readBody(bodyLength,priceInputStream):"+e.getMessage());
			e.printStackTrace();
			return null;
		}
		final SocketMessage socketMsg = new SocketMessage();
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
			
	private byte[] sendPriceReq() throws IOException{
			
		StringBuffer instrumentBuf = new StringBuffer();
		boolean flag = false;
		for(Instrument instrument: Instrument.values()){
			if(flag==true){
				instrumentBuf.append(";");
			}
			else{
				flag=true;
			}
			instrumentBuf.append(instrument.toString());
		}
		String instrumentStr = instrumentBuf.toString();
		logger.debug("instrumentStr:" + instrumentStr);
		byte[] instrumentByte = instrumentStr.getBytes();
		byte[] totalByte = new byte[instrumentByte.length+PRICE_REQ_HEAD_LENGTH];
		
		byte[] startByte = CommonUtil.intToBytesBig(CommonConstants.REAL_TIME_START);
		System.arraycopy(startByte, 0, totalByte, 0, startByte.length);		
		
		byte[] lengthByte = CommonUtil.shortToBytesBig((short)(instrumentByte.length+PRICE_REQ_HEAD_LENGTH));
		System.arraycopy(lengthByte, 0, totalByte, startByte.length, lengthByte.length);	
		
		byte[] versionByte = CommonUtil.shortToBytesBig(REAL_TIME_VERSION);	
		System.arraycopy(versionByte, 0, totalByte, startByte.length+lengthByte.length, versionByte.length);	
		
		byte[] msgidByte = CommonUtil.intToBytesBig(CommonConstants.REALTIME_PRICE_REQ_TAG);		
		System.arraycopy(msgidByte, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length, msgidByte.length);		
		
		String reqId = "52c7afeffce149e4a84b2e7e55731da3";
		byte[] reqIdByte = reqId.getBytes();		
		System.arraycopy(reqIdByte, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length+msgidByte.length, reqIdByte.length);		
		
		byte[] operationTypeByte = CommonUtil.shortToBytesBig(OPERATION_TYPE);
		System.arraycopy(operationTypeByte, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length+msgidByte.length+REQ_ID_LENGTH, operationTypeByte.length);		
		
		byte[] coinNum = CommonUtil.shortToBytesBig((short)Instrument.values().length);				
		System.arraycopy(coinNum, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length+msgidByte.length+REQ_ID_LENGTH+operationTypeByte.length, 
				coinNum.length);			
		
		int groupid = 100;
		byte[] groupidByte = CommonUtil.intToBytesBig(groupid);
		System.arraycopy(groupidByte, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length+msgidByte.length+REQ_ID_LENGTH+operationTypeByte.length+coinNum.length, 
				groupidByte.length);			
		
		int clientid = 200;
		byte[] clientidByte = CommonUtil.intToBytesBig(clientid);
		System.arraycopy(clientidByte, 0, totalByte, 
				startByte.length+lengthByte.length+versionByte.length+msgidByte.length+REQ_ID_LENGTH
				+operationTypeByte.length+coinNum.length+groupidByte.length, 
				clientidByte.length);
		
		System.arraycopy(instrumentByte, 0, totalByte, PRICE_REQ_HEAD_LENGTH, instrumentByte.length);
					
		priceOutputStream.write(totalByte);
		priceOutputStream.flush();
		
		return totalByte;
	}
	

	@PreDestroy
	private void destory(){
		
		System.out.println("I'm  destory  method  using  @@PreDestroy....");  
			
		if(priceInputStream!=null){
			try {
				priceInputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		if(priceOutputStream!=null){
			try {
				priceOutputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}

}
