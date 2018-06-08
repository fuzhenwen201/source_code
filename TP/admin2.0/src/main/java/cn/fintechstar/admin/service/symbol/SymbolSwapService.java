package cn.fintechstar.admin.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.symbol.ISymbolSwapDao;
import cn.fintechstar.admin.model.symbol.SymbolSwap;

@Service
public class SymbolSwapService {
	
	@Autowired
	private ISymbolSwapDao symbolSwapDao;

	public void saveSymbolSwap(int symbolId, SymbolSwap symbolSwap) {
		if(symbolSwap != null){
			symbolSwap.setSymbolId(symbolId);
			symbolSwapDao.saveSymbolSwap(symbolSwap);
		}
	}

	public void updateSymbolSwap(SymbolSwap symbolSwap) {
		if(symbolSwap != null){
			symbolSwapDao.updateSymbolSwap(symbolSwap);
		}
	}

	public SymbolSwap getSymbolSwapBySymbolId(int symbolId) {
		return symbolSwapDao.getSymbolSwapBySymbolId(symbolId);
	}

}
