package cn.fintechstar.admin.dao.symbol;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.BoSymbol;

@Repository
@Transactional
public interface IBoSymbolDao {

	void saveBoSymbol(BoSymbol boSymbol);

	void updateBoSymbol(BoSymbol boSymbol);

	BoSymbol getBoSymbolBySymbolId(int symbolId);

}
