package cn.fintechstar.receivetest.receive.schedule;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import cn.fintechstar.receivetest.receive.Receive;

@Component
public class ScheduleTask {

	@Scheduled(cron="*/1 * * * * *")
	public void print(){
		System.err.println(Receive.count);
	}
}
