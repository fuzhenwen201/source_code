package cn.fintechstar.admin.synch.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.synch.SymbolOfSqlite;

@Repository
@Transactional
public interface ISymbolOfSqliteDao {

	void clearTable();

	void saveSymbols(List<SymbolOfSqlite> symbols);

}
