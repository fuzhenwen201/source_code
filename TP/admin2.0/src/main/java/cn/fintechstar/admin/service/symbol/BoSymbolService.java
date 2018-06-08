package cn.fintechstar.admin.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.symbol.IBoSymbolDao;
import cn.fintechstar.admin.model.symbol.BoSymbol;
import cn.fintechstar.admin.model.symbol.BoSymbolVo;

@Service
public class BoSymbolService {
	
	@Autowired
	private IBoSymbolDao boSymbolDao;

	public void saveBoSymbol(int symbolId, BoSymbolVo boSymbolVo) {
		if(boSymbolVo != null){
			BoSymbol boSymbol = getBoSymbol(boSymbolVo, symbolId);
			boSymbolDao.saveBoSymbol(boSymbol);
		}
	}

	private BoSymbol getBoSymbol(BoSymbolVo boSymbolVo, int symbolId) {
		BoSymbol boSymbol = new BoSymbol();
		boSymbol.setSymbolId(symbolId);
		boSymbol.setBoCalType(boSymbolVo.getCalType());
		boSymbol.setReturnRate(boSymbol.getReturnRate());
		return boSymbol;
	}

	public void updateBoSymbol(BoSymbolVo boSymbolVo, int symbolId) {
		if(boSymbolVo != null){
			BoSymbol boSymbol = getBoSymbol(boSymbolVo, symbolId);
			boSymbolDao.updateBoSymbol(boSymbol);
		}
	}

	public BoSymbol getBoSymbolBySymbolId(int symbolId) {
		return boSymbolDao.getBoSymbolBySymbolId(symbolId);
	}

}
