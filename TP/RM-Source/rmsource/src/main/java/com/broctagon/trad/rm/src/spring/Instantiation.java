package com.broctagon.trad.rm.src.spring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.src.service.OnlineUserSetService;


@Component
public class Instantiation implements ApplicationListener<ContextRefreshedEvent> {

	@Autowired
	private OnlineUserSetService onlineUserSetService;
	
	public void onApplicationEvent(ContextRefreshedEvent event) {
		  System.out.println("event.getApplicationContext()");
	      if(event.getApplicationContext().getParent() == null){
	    	  System.out.println("event.getApplicationContext().getParent() == null");
	    	  
	    	  onlineUserSetService.requestAllOnlineUsers();
	
	      }
	      else{
	    	  System.out.println("event.getApplicationContext().getParent() = " + event.getApplicationContext().getParent());
	      }
	      
	}
	
}
