package cn.fintechstar.admin.dao.group;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.TradCondition;

@Repository
@Transactional
public interface ITradingConditionDao {

	void saveTradingCondition(TradCondition tradCondition);

	void updateTradingCondition(TradCondition tradCondition);

	TradCondition getTradingConditionByGroupId(int groupId);

}
