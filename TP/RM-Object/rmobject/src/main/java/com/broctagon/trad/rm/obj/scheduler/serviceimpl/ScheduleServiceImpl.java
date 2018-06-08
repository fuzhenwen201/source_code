package com.broctagon.trad.rm.obj.scheduler.serviceimpl;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.obj.service.BoService;
import com.broctagon.trad.rm.obj.service.ClosedTradDaoAdminService;
import com.broctagon.trad.rm.obj.service.ScheduleService;

@Service
public class ScheduleServiceImpl implements ScheduleService {

	@Autowired
	private BoService boService;
	@Autowired
	private ClosedTradDaoAdminService closedTradDaoAdminService;
	@Autowired
	private Environment environment;

	public void schedule() {
		final long ADT_TIME = environment.getRequiredProperty("schedule.adt.time", Long.class);
		final long RM_TIME = environment.getRequiredProperty("schedule.rm.time", Long.class);
		ScheduledExecutorService scheduledExecutor = Executors.newScheduledThreadPool(5);

		Runnable processTraderTask = new Runnable() {
			public void run() {
				boService.fetchOrderAnsAndProcess();
			}
		};

		Runnable updateClosedTradToAdmin = new Runnable() {
			public void run() {
				closedTradDaoAdminService.processAndSend();
			}
		};
		scheduledExecutor.scheduleAtFixedRate(processTraderTask, 0, RM_TIME, TimeUnit.MILLISECONDS);
		scheduledExecutor.scheduleAtFixedRate(updateClosedTradToAdmin, 0, ADT_TIME, TimeUnit.MILLISECONDS);
	}

}
