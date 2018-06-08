package cn.fintechstar.admin.service.symbol;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolVo;

@Service
public class SymbolCommonSerivce {
	
	@Autowired
	private ISymbolDao symbolDao;

	public void saveSymbol(SymbolCommon symbol) {
		if(symbol != null){
			symbolDao.saveSymbol(symbol);
		}
	}

	public void updateSymbol(SymbolCommon symbol) {
		if(symbol != null){
			symbolDao.updateSymbol(symbol);
		}
	}

	public List<SymbolVo> getSymbols() {
		return symbolDao.getSymbols();
	}

	public SymbolCommon getSymbolBySymbolId(int symbolId) {
		return symbolDao.getSymbolBySymbolId(symbolId);
	}

}
