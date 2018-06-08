package cn.fintechstar.admin.dao.symbol;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.SymbolFilteration;

@Repository
@Transactional
public interface ISymbolFilterationDao {

	void saveSymbolFilteration(SymbolFilteration symbolFilteration);

	void updateSymbolFilteration(SymbolFilteration symbolFilteration);

	SymbolFilteration getSymbolFilterationBySymbolId(int symbolId);
}
