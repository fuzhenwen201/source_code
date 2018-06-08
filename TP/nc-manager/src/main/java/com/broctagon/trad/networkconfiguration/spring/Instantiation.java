package com.broctagon.trad.networkconfiguration.spring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperHelper;

@Component
public class Instantiation implements ApplicationListener<ContextRefreshedEvent> {

	@Autowired private ZookeeperHelper zkHelper;
	
	public void onApplicationEvent(ContextRefreshedEvent event) {
		  System.out.println("event.getApplicationContext()");
	      if(event.getApplicationContext().getParent() == null){
	    	  System.out.println("event.getApplicationContext().getParent() == null");
	    	      	  					    	  
	      }
	      else{
	    	  System.out.println("event.getApplicationContext().getParent() = " + event.getApplicationContext().getParent());
	      }
	      
	}
	
}
