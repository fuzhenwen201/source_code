package cn.fintechstar.admin.dao.symbol;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolVo;

@Repository
@Transactional
public interface ISymbolDao {

	List<String> getSymbolNamesBySecurityId(int securityId);

	void saveSymbol(SymbolCommon symbol);

	void updateSymbol(SymbolCommon symbol);

	List<SymbolVo> getSymbols();

	SymbolCommon getSymbolBySymbolId(int symbolId);

}
