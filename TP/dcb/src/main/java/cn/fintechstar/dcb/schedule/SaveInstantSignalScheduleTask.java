package cn.fintechstar.dcb.schedule;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import cn.fintechstar.dcb.constant.SignalConstant;
import cn.fintechstar.dcb.model.InstantSignal;
import cn.fintechstar.dcb.service.InstantSignalService;

@Service
public class SaveInstantSignalScheduleTask {

	@Autowired
	private InstantSignalService instantSignalService;
	
	@Scheduled(cron="*/1 * * * * ?")
	public void saveInstantSignal(){
		while(!SignalConstant.ALL_SIGNALS.isEmpty()){
			StringBuilder sb = new StringBuilder("BEGIN TRANSACTION;");
			for(int i = 0; i < 100 ;i ++){
				InstantSignal instantSignal = SignalConstant.ALL_SIGNALS.poll();
				if(instantSignal != null){
					sb.append(instantSignalService.getSaveInstantSignalSql(instantSignal));
				}
			}
			if(!"BEGIN TRANSACTION;".equals(sb.toString())){
				sb.append("COMMIT;");
				instantSignalService.saveInstantSignal(sb.toString());
			}
		}
	}
}
