package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.FincoinUsdDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class FincoinService {

	@Autowired
	private FincoinUsdDao fincoinUsdDao;
	
	public void saveFincoinUsd(InstantSignal instantSignal){
		fincoinUsdDao.saveFincoinUsd(instantSignal);
	}
	
}
