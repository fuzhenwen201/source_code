package com.broctagon.trad.networkconfiguration.zookeeper;

import javax.annotation.PostConstruct;

import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.api.CuratorListener;
import org.apache.log4j.Logger;
import org.apache.zookeeper.CreateMode;
import org.springframework.beans.factory.annotation.Autowired;


public class ZookeeperHelper {	
	
	private Logger logger = Logger.getLogger(Logger.class);
	
	@Autowired 
	private CuratorListener curatorListener;
	@Autowired
	private ZookeeperDispatcher zookeeperDispatcher;

	private CuratorFramework zfc;
	
	public ZookeeperHelper(CuratorFramework zfc){				
		this.zfc = zfc;
		zfc.start();
	}
	
	@PostConstruct
	public void init(){
		logger.info("ZookeeperHelper init ");
				
		addListener(curatorListener);					
				
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_SYMBOLSPREAD);
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_SGIPURLMAP);
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_SECURITYSPREAD);
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_GROUPSPREAD);
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_BOCALCULATION);
		initDataAndWatchPath(ZookeeperConstants.ZK_PATH_GROUPBOSYMBOL);					
	}
	
	public void addListener(CuratorListener curatorListener){
		zfc.getCuratorListenable().addListener(curatorListener);
	}
	
	public void initDataAndWatchPath(String path){					
		try {				
			byte[] data = getDataAndWatchPath(path);
			zookeeperDispatcher.dispatch(path, data);
		} catch (Exception e) {
			logger.error(e.getMessage());
			e.printStackTrace();
		}	
	}
	
	public byte[] getDataAndWatchPath(String path) throws Exception{
		if(zfc.checkExists().forPath(path)==null){
			logger.info("path :" + path + "，do not exist， create. " );
			zfc.create().withMode(CreateMode.PERSISTENT).forPath(path,"".getBytes());						
		}		
		byte[] data = zfc.getData().watched().forPath(path);
		return data;
	}
	
	public byte[] getData(String path) throws Exception{	
		byte[] data = zfc.getData().forPath(path);
		return data;
	}
	
	public void setData(String path, byte[] data) throws Exception{		
		try {	
			if(zfc.checkExists().forPath(path)==null){
				logger.info("path :" + path + "，do not exist， create. " );
				zfc.create().withMode(CreateMode.PERSISTENT).forPath(path,"".getBytes());						
			}	
			zfc.setData().forPath(path, data);
		} catch (Exception e) {
			logger.error(e.getMessage());
			e.printStackTrace();
		}	
	}
		
	public void close(){
		if(zfc!=null){
			 zfc.close();
		}
	}
	
	public void test(){
  		addListener(curatorListener);
		try {
			getDataAndWatchPath(ZookeeperConstants.ZK_PATH_SGIPURLMAP);
		} catch (Exception e) {
			e.printStackTrace();
		}
    	  
		try {
			Thread.sleep(3000);
			setData(ZookeeperConstants.ZK_PATH_SGIPURLMAP, "6666sstttyyyttuuuuu".getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}	
	}
	
    
	/* public static void main( String[] args )
	 {		
		 ZookeeperHelper zkHelper = new ZookeeperHelper();	 
		 CuratorListener curatorListener =  new MyCuratorListener();
		 zkHelper.addListener(curatorListener);
		 zkHelper.init();
		 
		 try {
			zkHelper.watchPath("/workers3");
			zkHelper.setData("/workers3", "6666sstttyyytt".getBytes());
			Thread.sleep(3000);
			
		} catch (Exception e) {		
			e.printStackTrace();
		}
		finally{
			zkHelper.close();
		}
	 }*/
	 
}
