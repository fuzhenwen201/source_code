package com.broctagon.trad.rm.obj;

import java.util.Date;

import org.apache.log4j.Logger;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.CommonUtil;

@SpringBootApplication
//@EnableScheduling
@ComponentScan(basePackages="com.broctagon.trad.rm")
public class SpringBootApp implements CommandLineRunner{
	private static Logger LOGGER = Logger.getLogger(SpringBootApp.class);
	public static void main(String[] args){
		SpringApplication.run(SpringBootApp.class, args);
	}

	@Override
	public void run(String... arg0) throws Exception {
		LOGGER.info("RUN CALLED");
		CommonUtil.rmStartTimeMap.put(CommonConstants.RM_START_TIME, new Date(System.currentTimeMillis()).getTime());
	}
	
}
