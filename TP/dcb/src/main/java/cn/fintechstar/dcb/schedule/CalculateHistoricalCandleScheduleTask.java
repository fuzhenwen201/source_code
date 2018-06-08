package cn.fintechstar.dcb.schedule;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import cn.fintechstar.dcb.constant.SignalConstant;
import cn.fintechstar.dcb.model.InstantSignal;
import cn.fintechstar.dcb.service.InstantSignalService;

@Service
public class CalculateHistoricalCandleScheduleTask {
	
	@Autowired
	private InstantSignalService instantSignalService;
	
	@Scheduled(cron="*/1 * * * * ?")
	public void calculateHistorialCandle(){	
		new Thread(() -> {
			while(!SignalConstant.GROUP_0.isEmpty()){
				InstantSignal instantSignal = SignalConstant.GROUP_0.poll();
				instantSignalService.getHistoricalCandles(instantSignal);
			}
		}).start();
		new Thread(() -> {
			while(!SignalConstant.GROUP_1.isEmpty()){
				InstantSignal instantSignal = SignalConstant.GROUP_1.poll();	
				instantSignalService.getHistoricalCandles(instantSignal);
			}
		}).start();
		new Thread(() -> {
			while(!SignalConstant.GROUP_2.isEmpty()){
				InstantSignal instantSignal = SignalConstant.GROUP_2.poll();
				instantSignalService.getHistoricalCandles(instantSignal);
			}
		}).start();
		new Thread(() -> {
			while(!SignalConstant.GROUP_3.isEmpty()){
				InstantSignal instantSignal = SignalConstant.GROUP_3.poll();
				instantSignalService.getHistoricalCandles(instantSignal);
			}
		}).start();
		new Thread(() -> {
			while(!SignalConstant.GROUP_4.isEmpty()){
				InstantSignal instantSignal = SignalConstant.GROUP_4.poll();
				instantSignalService.getHistoricalCandles(instantSignal);
			}
		}).start();
		while(!SignalConstant.GROUP_5.isEmpty()){
			InstantSignal instantSignal = SignalConstant.GROUP_5.poll();
			instantSignalService.getHistoricalCandles(instantSignal);
		}
	}

	
}
