package com.broctagon.trad.rm.obj.spring;

import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Component;


@Component
public class Instantiation implements ApplicationListener<ContextRefreshedEvent> {

//	@Autowired
//	private ScheduleService scheduleService;

	public void onApplicationEvent(ContextRefreshedEvent event) {
		System.out.println("event.getApplicationContext()");
		if (event.getApplicationContext().getParent() == null) {
			System.out.println("event.getApplicationContext().getParent() == null");

		//	scheduleService.schedule();
		} else {
			System.out.println(
					"event.getApplicationContext().getParent() = " + event.getApplicationContext().getParent());
		}

	}

}
