package com.broctagon.trad.networkconfiguration.zookeeper;

import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.api.CuratorEvent;
import org.apache.curator.framework.api.CuratorEventType;
import org.apache.curator.framework.api.CuratorListener;
import org.apache.log4j.Logger;
import org.apache.zookeeper.Watcher.Event;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class MyCuratorListener implements CuratorListener{
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ZookeeperDispatcher zookeeperDispatcher;
	
	public void eventReceived(CuratorFramework client, CuratorEvent event) throws Exception {
		CuratorEventType curatorEventType = event.getType();		
		logger.info("event:" + event);		
		logger.info("event.getType():" + event.getType());	
				
		switch(curatorEventType){
		case WATCHED:
			logger.info("case WATCHED");
			routingWatchEvent(client, event);			
			break;
		default:
			logger.info("switch(curatorEventType): defaultm type:");		
		}		
	}
	
	public void routingWatchEvent(CuratorFramework client, CuratorEvent event) throws Exception{
		Event.EventType eventType = event.getWatchedEvent().getType();
		switch(eventType){
		case NodeDataChanged:			
			nodeDataChangeAction(client,event);
			break;
		default:
			logger.info("routingWatchEvent: default");
			break;
		}	
	}
	
	public void nodeDataChangeAction(CuratorFramework client, CuratorEvent event) throws Exception{
		String path =  event.getPath();
		byte[] data = client.getData().watched().forPath(path);
		zookeeperDispatcher.dispatch(path, data);		
	}

}
