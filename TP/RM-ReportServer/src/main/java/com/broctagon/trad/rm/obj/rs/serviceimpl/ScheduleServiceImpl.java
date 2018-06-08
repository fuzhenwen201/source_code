package com.broctagon.trad.rm.obj.rs.serviceimpl;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.obj.rs.service.PositionService;
import com.broctagon.trad.rm.obj.rs.service.ScheduleService;
import com.broctagon.trad.rm.obj.rs.service.UpdateTraderService;

@Service
public class ScheduleServiceImpl implements ScheduleService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private UpdateTraderService updateTraderService;
	@Autowired private PositionService positionService;

	public void schedule(){
			
		ScheduledExecutorService scheduledExecutor = Executors.newScheduledThreadPool(5);
		
		Runnable updateTradersTask = new Runnable(){
			public void run() {
				try{
					updateTraderService.broadcastUpdateTrader();
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}			
			}	
		};
		
		Runnable updatePositionsTask = new Runnable(){
			public void run() {
				try{
					positionService.updatePositions();	
				}
				catch(Exception e){
					logger.error(e.getMessage());
				}											
			}	
		};
				
		scheduledExecutor.scheduleAtFixedRate(updateTradersTask, 0, 1000, TimeUnit.MILLISECONDS);
		scheduledExecutor.scheduleAtFixedRate(updatePositionsTask, 0, 1000, TimeUnit.MILLISECONDS);
				
	}
	
}
