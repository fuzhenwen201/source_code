package cn.fintechstar.admin.dao.symbol;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.SymbolCalculation;

@Repository
@Transactional
public interface ISymbolCalculationDao {

	void saveSymbolCalculation(SymbolCalculation symbolCalculation);

	void updateSymbolCalculation(SymbolCalculation symbolCalculation);

	SymbolCalculation getSymbolCalculationBySymbolId(int symbolId);

}
