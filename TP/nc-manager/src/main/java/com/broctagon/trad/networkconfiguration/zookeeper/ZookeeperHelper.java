package com.broctagon.trad.networkconfiguration.zookeeper;

import org.apache.curator.framework.CuratorFramework;
import org.apache.log4j.Logger;
import org.apache.zookeeper.CreateMode;
import org.springframework.stereotype.Component;


@Component
public class ZookeeperHelper {	
	
	private Logger logger = Logger.getLogger(Logger.class);

	private CuratorFramework zfc;

	public ZookeeperHelper(CuratorFramework zfc){				
		this.zfc = zfc;
		zfc.start();
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
	
	public byte[] getData(String path) throws Exception{	
		byte[] data = zfc.getData().forPath(path);
		return data;
	}
	
	public void close(){
		if(zfc!=null){
			 zfc.close();
		}
	}
   
/*	 public static void main( String[] args )
	 {		
		 ZookeeperHelper zkHelper = new ZookeeperHelper();	 
		 
		 try {
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
