package com.broctagon.trad.reportserver.spring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Component;

import com.broctagon.trad.reportserver.service.MT4ReportService;

@Component
public class Instantiation implements ApplicationListener<ContextRefreshedEvent> {

/*	@Autowired
	private ScheduleService scheduleService;*/
	@Autowired
	private MT4ReportService reportService;
	
	public void onApplicationEvent(ContextRefreshedEvent event) {
		  System.out.println("event.getApplicationContext()");
	      if(event.getApplicationContext().getParent() == null){
	    	  System.out.println("event.getApplicationContext().getParent() == null");
	    	  reportService.redisToMT4Schedule();
	
	      }
	      else{
	    	  System.out.println("event.getApplicationContext().getParent() = " + event.getApplicationContext().getParent());
	      }
	      
	}
	
}
