package com.broctagon.trad.rm.obj.scheduler.serviceimpl;

import org.apache.log4j.Logger;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.CommonUtil;


/**
 * @author imdadullah
 *
 */
@Component
public class CronScheduler {
	private static Logger LOGGER = Logger.getLogger(CronScheduler.class);
	/*
	 * @Autowired static Environment environment; private static final String
	 * CRON_TIME = environment.getProperty("cron_time");
	 */

	@Scheduled(cron = "${cron.time}")
	public void updateFlushFlag() {
		LOGGER.info("###### GOING TO RUN CRON JOB ######");
		CommonUtil.flushFlag = true;
	}
}
