package cn.fintechstar.admin.dao.symbol;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.SymbolSession;

@Repository
@Transactional
public interface ISymbolSessionDao {

	void saveSymbolSessions(List<SymbolSession> symbolSessions);

	void updateSymbolSessions(List<SymbolSession> symbolSessions);

	List<SymbolSession> getSymbolSessionsBySymbolId(int symbolId);

}
