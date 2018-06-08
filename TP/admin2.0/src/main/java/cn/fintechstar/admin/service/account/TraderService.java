package cn.fintechstar.admin.service.account;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.account.ITraderDao;
import cn.fintechstar.admin.model.account.Trader;
import cn.fintechstar.admin.model.account.TraderVo;

@Service
public class TraderService {

	@Autowired
	private ITraderDao traderDao;
	
	public void saveOrUpdateTrader(Trader trader) {
		if(trader.getClientId() != 0){
			traderDao.updateTrader(trader);
		}else{
			traderDao.saveTrader(trader);
		}
	}

	public List<TraderVo> getTraders() {
		return traderDao.getTraders();
	}

	public Trader getTraderById(int clientId) {
		return traderDao.getTraderById(clientId);
	}

}
