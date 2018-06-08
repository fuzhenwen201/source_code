package cn.fintechstar.admin.service.symbol;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import cn.fintechstar.admin.dao.symbol.ISymbolSessionDao;
import cn.fintechstar.admin.model.symbol.SymbolSession;
import cn.fintechstar.admin.model.symbol.SymbolSessionVo;
import cn.fintechstar.admin.util.DateUtil;

@Service
public class SymbolSessionService {
	
	@Autowired
	private ISymbolSessionDao symbolSessionDao;

	public void saveSymbolSession(int symbolId, List<SymbolSessionVo> symbolSessionVos) throws ParseException {
		if(!CollectionUtils.isEmpty(symbolSessionVos)){
			List<SymbolSession> symbolSessions = getSymbolSessions(symbolSessionVos, symbolId);
			symbolSessionDao.saveSymbolSessions(symbolSessions);
		}
	}

	private List<SymbolSession> getSymbolSessions(List<SymbolSessionVo> symbolSessionVos, int symbolId) throws ParseException {
		List<SymbolSession> symbolSessions = new ArrayList<>();
		for(SymbolSessionVo symbolSessionVo : symbolSessionVos){
			SymbolSession symbolSession = new SymbolSession();
			symbolSession.setSymbolId(symbolId);
			symbolSession.setDay(symbolSessionVo.getDay());
			symbolSession.setOpenQuote(DateUtil.convertStringtoTime(symbolSessionVo.getOpenQuote()));
			symbolSession.setCloseQuote(DateUtil.convertStringtoTime(symbolSessionVo.getCloseQuote()));
			symbolSession.setOpenTrade(DateUtil.convertStringtoTime(symbolSessionVo.getOpenTrade()));
			symbolSession.setCloseTrade(DateUtil.convertStringtoTime(symbolSessionVo.getCloseTrade()));
			symbolSessions.add(symbolSession);
		}
		return symbolSessions;
	}

	public void updateSymbolSession(List<SymbolSessionVo> symbolSessionVos) throws ParseException {
		if(!CollectionUtils.isEmpty(symbolSessionVos)){
			List<SymbolSession> symbolSessions = getSymbolSessions(symbolSessionVos, symbolSessionVos.get(0).getSymbolId());
			symbolSessionDao.updateSymbolSessions(symbolSessions);
		}
	}

	public List<SymbolSession> getSymbolSessionsBySymbolId(int symbolId) {
		return symbolSessionDao.getSymbolSessionsBySymbolId(symbolId);
	}

}
