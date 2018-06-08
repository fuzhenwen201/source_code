package cn.fintechstar.admin.dao.account;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.account.Trader;
import cn.fintechstar.admin.model.account.TraderVo;

@Repository
@Transactional
public interface ITraderDao {

	void updateTrader(Trader trader);

	void saveTrader(Trader trader);

	List<TraderVo> getTraders();

	Trader getTraderById(int clientId);

}
