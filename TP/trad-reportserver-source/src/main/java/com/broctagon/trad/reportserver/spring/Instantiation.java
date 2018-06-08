package com.broctagon.trad.reportserver.spring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Component;

import com.broctagon.trad.reportserver.service.RealTimeSocketService;
import com.broctagon.trad.reportserver.service.SocketService;

@Component
public class Instantiation implements ApplicationListener<ContextRefreshedEvent> {

	@Autowired
	private RealTimeSocketService realTimeSocketService;
	@Autowired
	private SocketService socketService;
	
	public void onApplicationEvent(ContextRefreshedEvent event) {
		  System.out.println("event.getApplicationContext()");
	      if(event.getApplicationContext().getParent() == null){
	    	  System.out.println("event.getApplicationContext().getParent() == null");
	
	    //	  realTimeSocketService.init();
	    	  socketService.init();
	      }
	      else{
	    	  System.out.println("event.getApplicationContext().getParent() = " + event.getApplicationContext().getParent());
	      }
	      
	}
	
}
