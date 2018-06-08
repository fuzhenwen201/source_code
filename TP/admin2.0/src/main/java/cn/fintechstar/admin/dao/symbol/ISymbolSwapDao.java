package cn.fintechstar.admin.dao.symbol;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.SymbolSwap;

@Repository
@Transactional
public interface ISymbolSwapDao {

	void saveSymbolSwap(SymbolSwap symbolSwap);

	void updateSymbolSwap(SymbolSwap symbolSwap);

	SymbolSwap getSymbolSwapBySymbolId(int symbolId);

}
