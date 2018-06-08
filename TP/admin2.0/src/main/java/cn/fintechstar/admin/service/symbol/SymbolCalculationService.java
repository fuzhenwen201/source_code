package cn.fintechstar.admin.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.symbol.ISymbolCalculationDao;
import cn.fintechstar.admin.model.symbol.SymbolCalculation;

@Service
public class SymbolCalculationService {
	
	@Autowired
	private ISymbolCalculationDao symbolCalculationDao;

	public void saveSymbolCalculation(int symbolId, SymbolCalculation symbolCalculation) {
		if(symbolCalculation != null){
			symbolCalculation.setSymbolId(symbolId);
			symbolCalculationDao.saveSymbolCalculation(symbolCalculation);
		}
	}

	public void updateSymbolCalculation(SymbolCalculation symbolCalculation) {
		if(symbolCalculation != null){
			symbolCalculationDao.updateSymbolCalculation(symbolCalculation);
		}	
	}

	public SymbolCalculation getSymbolCalculationBySymbolId(int symbolId) {
		return symbolCalculationDao.getSymbolCalculationBySymbolId(symbolId);
	}

}
