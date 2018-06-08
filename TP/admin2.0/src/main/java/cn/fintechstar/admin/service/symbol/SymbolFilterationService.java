package cn.fintechstar.admin.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.symbol.ISymbolFilterationDao;
import cn.fintechstar.admin.model.symbol.SymbolFilteration;

@Service
public class SymbolFilterationService {
	
	@Autowired
	private ISymbolFilterationDao symbolFilterationDao;

	public void saveSymbolFilteration(int symbolId, SymbolFilteration symbolFilteration) {
		if(symbolFilteration != null){
			symbolFilteration.setSymbolId(symbolId);
			symbolFilterationDao.saveSymbolFilteration(symbolFilteration);
		}
	}

	public void updateSymbolFilteration(SymbolFilteration symbolFilteration) {
		if(symbolFilteration != null){
			symbolFilterationDao.updateSymbolFilteration(symbolFilteration);	
		}
	}

	public SymbolFilteration getSymbolFilterationBySymbolId(int symbolId) {
		return symbolFilterationDao.getSymbolFilterationBySymbolId(symbolId);
	}
}
