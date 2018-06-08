package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.SymboButtlDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class SymbolService {

	@Autowired
	private SymboButtlDao symboButtlDao;
	
	public void saveSymbolButt(InstantSignal instantSignal){
		symboButtlDao.saveSymbolButt(instantSignal);
	}
}
