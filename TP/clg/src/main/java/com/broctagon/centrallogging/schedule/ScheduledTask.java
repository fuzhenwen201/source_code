package com.broctagon.centrallogging.schedule;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.broctagon.centrallogging.receive.ErrorMessageListener;
import com.broctagon.centrallogging.service.ErrorMessageService;
import com.broctagon.centrallogging.service.MailService;
import com.broctagon.tradtnode.rabbit.message.Message;

@Component
public class ScheduledTask {
	
	@Autowired
	@Qualifier("ds1")
	private DataSource dataSource;
	
	@Autowired
	@Qualifier("ds2")
	private DataSource dataSource2;
	
	@Autowired
	private ErrorMessageService errorMessageService;
	
	@Autowired
	private MailService mailService;
	
	private static ArrayList<com.broctagon.tradtnode.rabbit.message.Message> tnodeMessages1;
	
	private static ArrayList<com.broctagon.tradtnode.rabbit.message.Message> tnodeMessages2 ;
	
	@SuppressWarnings("unchecked")
	@Scheduled(cron = "*/1 * * * * *")
	public void saveErrorMessage1() throws Exception{
		if(!ErrorMessageListener.circleArray1.isEmpty()){
			tnodeMessages1 = (ArrayList<Message>) errorMessageService.getTnodeMessage(Arrays.asList(ErrorMessageListener.circleArray1.getAll()));
			errorMessageService.saveErrorMessages(tnodeMessages1, dataSource);
			new Thread(new Runnable() {
				public void run() {
					try {
						mailService.sendMails((List<Message>)tnodeMessages1.clone());
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}).start();;
		}
	}
	
	@SuppressWarnings("unchecked")
	@Scheduled(cron = "*/1 * * * * *")
	public void saveErrorMessage2() throws Exception{
		if(!ErrorMessageListener.circleArray2.isEmpty()){
			tnodeMessages2 = (ArrayList<Message>) errorMessageService.getTnodeMessage(Arrays.asList(ErrorMessageListener.circleArray2.getAll()));
			errorMessageService.saveErrorMessages(tnodeMessages2, dataSource2);
			new Thread(new Runnable() {
				public void run() {
					try {
						mailService.sendMails((List<Message>) tnodeMessages2.clone());
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}).start();
 		}
	}


}
