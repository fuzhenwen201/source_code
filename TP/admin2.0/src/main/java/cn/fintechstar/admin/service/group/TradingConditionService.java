package cn.fintechstar.admin.service.group;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import cn.fintechstar.admin.dao.group.ITimePeriodDao;
import cn.fintechstar.admin.dao.group.ITradingConditionDao;
import cn.fintechstar.admin.model.group.TimePeriod;
import cn.fintechstar.admin.model.group.TradCondition;

@Service
public class TradingConditionService {
	
	@Autowired
	private ITradingConditionDao tradingConditionDao;
	
	@Autowired
	private ITimePeriodDao timePeriodDao;

	public void saveTradingCondition(int groupId, TradCondition tradCondition) {
		if(tradCondition !=  null){
			tradCondition.setGroupId(groupId);
			tradingConditionDao.saveTradingCondition(tradCondition);
			saveTimePeriod(groupId, tradCondition.getTimePeriods());
		}
	}

	private void saveTimePeriod(int groupId, List<TimePeriod> timePeriods) {
		if(!CollectionUtils.isEmpty(timePeriods)){
			for(TimePeriod timePeriod : timePeriods){
				timePeriod.setGroupId(groupId);
			}
			timePeriodDao.saveTimePeriods(timePeriods);
		}
	}

	public void updateTradingCondition(int groupId, TradCondition tradCondition) {
		if(tradCondition !=  null){
			tradingConditionDao.updateTradingCondition(tradCondition);
			updateTimePeriod(groupId, tradCondition.getTimePeriods());
		}
	}

	private void updateTimePeriod(int groupId, List<TimePeriod> timePeriods) {
		timePeriodDao.deleteTimePeriodsByGroupId(groupId);
		if(!CollectionUtils.isEmpty(timePeriods)){
			timePeriodDao.saveTimePeriods(timePeriods);
		}
	}

	public TradCondition getTradingConditionByGroupId(int groupId) {
		TradCondition tradCondition =  tradingConditionDao.getTradingConditionByGroupId(groupId);
		tradCondition.setTimePeriods(timePeriodDao.getTimePeriodsByGroupId(groupId));
		return tradCondition;
	}

}
