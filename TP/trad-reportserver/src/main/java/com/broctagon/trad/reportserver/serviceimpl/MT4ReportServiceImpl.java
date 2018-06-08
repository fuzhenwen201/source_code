package com.broctagon.trad.reportserver.serviceimpl;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.annotation.PostConstruct;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Service;

import com.broctagon.trad.reportserver.common.CommonConstants;
import com.broctagon.trad.reportserver.common.CommonUtil;
import com.broctagon.trad.reportserver.common.Instrument;
import com.broctagon.trad.reportserver.service.MT4PriceService;
import com.broctagon.trad.reportserver.service.MT4ReportService;
import com.broctagon.trad.reportserver.service.MT4TradeService;
import com.broctagon.trad.reportserver.service.MT4UserService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;

@Service("reportService")
public class MT4ReportServiceImpl implements MT4ReportService{

	private Logger logger = Logger.getLogger(MT4ReportServiceImpl.class);
	
	public static final int SCHEDULE_TIME_INIT = 3000; // milliseconds
	public static final int SCHEDULE_PERIOD = 1000; // milliseconds
	
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	@Autowired
	private MT4UserService userService;
	@Autowired
	private MT4TradeService tradeService;
	@Autowired
	private MT4PriceService priceService;
	@Autowired
	private Environment environment;
	
	private Integer write_max_length = 10000;
	
	@PostConstruct
	public void init(){
 		String maxLengthStr = environment.getProperty("write.max.length");
 		if(!CommonUtil.isEmptyString(maxLengthStr)){
 			write_max_length = Integer.parseInt(maxLengthStr.trim());
 			logger.info("write_max_length:"+write_max_length);
 		}
 		else{
 			logger.info("write.max.length is empty.");
 		}
	}
		
	public void redisToMT4Schedule(){
		logger.info("start");
		
		try{
			writeToMT4User(CommonConstants.REDIS_KEY_REPORT_SERVER_INIT_TRADERS);
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
		
		ScheduledExecutorService service = Executors.newScheduledThreadPool(10);
		Runnable newTask = new Runnable(){
			@Override
			public void run() {		
				try{
					writeToMT4User(CommonConstants.REDIS_KEY_REPORT_SERVER_NEW_TRADERS);
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}
			}
		};
		
		Runnable updateBalanceUserTask = new Runnable(){
			@Override
			public void run() {		
				try{
					writeToMT4User(CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_TRADER_BALANCE);
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}
			}
		};
		
		Runnable updateUserTask = new Runnable(){
			@Override
			public void run() {		
				try{
					writeToMT4User(CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_TRADERS);
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}
			}
		};
		
		Runnable tradeTask = new Runnable(){
			@Override
			public void run() {	
				try{
					writeToMT4Trade(CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_POSITION);
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}
				
			}
		};
		
		Runnable priceTask = new Runnable(){
			@Override
			public void run() {	
				try{
					writeToMT4Price();
				}
				catch(Exception ex){
					logger.error(ex.getMessage());
				}			
			}
		};
			
		try{
			writeToMT4Trade(CommonConstants.REDIS_KEY_REPORT_SERVER_INIT_POSITIONS);
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
		
		service.scheduleAtFixedRate(newTask,				SCHEDULE_TIME_INIT,	 SCHEDULE_PERIOD,  TimeUnit.MILLISECONDS);
		service.scheduleAtFixedRate(updateUserTask, 		SCHEDULE_TIME_INIT,	 SCHEDULE_PERIOD,  TimeUnit.MILLISECONDS);
		service.scheduleAtFixedRate(updateBalanceUserTask,	SCHEDULE_TIME_INIT,	 SCHEDULE_PERIOD,  TimeUnit.MILLISECONDS);
		service.scheduleAtFixedRate(tradeTask,				SCHEDULE_TIME_INIT,	 SCHEDULE_PERIOD,  TimeUnit.MILLISECONDS);	
		service.scheduleAtFixedRate(priceTask,				SCHEDULE_TIME_INIT,	 SCHEDULE_PERIOD,  TimeUnit.MILLISECONDS);
	
	}
	
	public List<String> writeToMT4User(String redisKey){		
		ShardedJedis shardedJedis = null;	
		try{
			shardedJedis = shardedJedisPool.getResource();
			long len=shardedJedis.llen(redisKey);
			if(len==0){
			//	logger.info("redisKey:" + redisKey + ",len==0");
				return Collections.emptyList();
			}
			
			long start = System.currentTimeMillis();				
			List<String> traderStrList = null;			
			traderStrList = shardedJedis.lrange(redisKey, 0, (write_max_length-1));
								
			if(traderStrList!=null&&!traderStrList.isEmpty()){
				shardedJedis.ltrim(redisKey, write_max_length, len-1);	
			//	long end = System.currentTimeMillis();
				logger.info("lrange: traderStrList.size():" + traderStrList.size());
				if(CommonConstants.REDIS_KEY_REPORT_SERVER_UPDATE_TRADER_BALANCE.equals(redisKey)){
					userService.updateBalanceUsers(traderStrList);
				}
				else{
					userService.addMT4Users(traderStrList);
				}			
			}
			else{
				logger.info("write_max_length:" + write_max_length + ",len:" + len + "," + "traderStrList is empty.");
			}			
											
			return Collections.emptyList();
		}
		catch(Exception e){
			logger.error(e.getMessage());
			return Collections.emptyList();
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();	
			}			
		}	
	}
	
	public List<String> writeToMT4Trade(String redisTradeKey){
		ShardedJedis shardedJedis = null;	
		try{
			shardedJedis = shardedJedisPool.getResource();
			long len=shardedJedis.llen(redisTradeKey);
			if(len==0){
			//	logger.info("redisTradeKey:" + redisTradeKey + ",len==0");
				return Collections.emptyList();
			}
			
			long start = System.currentTimeMillis();
			List<String> orderStrList = shardedJedis.lrange(redisTradeKey, 0, write_max_length-1);
						
			if(orderStrList!=null&&!orderStrList.isEmpty()){
				shardedJedis.ltrim(redisTradeKey, write_max_length, len-1);
			//	long end = System.currentTimeMillis();
			//	logger.info("lrange: orderStrList.size():" + orderStrList.size() + ",duration:" + (end-start));								
				tradeService.addMT4Trades(orderStrList);
			}
			else{
				logger.info("write_max_length:" + write_max_length + ",len:" + len + "," +"orderStrList is empty.");
			}
			
			return orderStrList;
		}
		catch(Exception e){
			logger.error(e.getMessage());
			return Collections.emptyList();
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();	
			}			
		}
	}
	
	public List<String> writeToMT4Price(){
		ShardedJedis shardedJedis = null;	
		try{
			shardedJedis = shardedJedisPool.getResource();
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
						
			for(Instrument instrument: Instrument.values()){			
				pipeline.get(instrument.toString());
			}
			List<Object> realtimeObjList = pipeline.syncAndReturnAll();		
	/*		for(Instrument instrument: Instrument.values()){			
				pipeline.del(instrument.toString());
			}
			pipeline.sync();*/
			
			if(realtimeObjList==null||realtimeObjList.size()==0){
			//	logger.info("realtimeObjList==null||realtimeObjList.size()==0");
				return Collections.emptyList();
			}
			priceService.addMT4Prices(realtimeObjList);					
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();	
			}			
		}
		return Collections.emptyList();
	}
	
}
