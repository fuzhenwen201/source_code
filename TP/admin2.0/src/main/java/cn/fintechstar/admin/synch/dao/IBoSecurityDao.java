package cn.fintechstar.admin.synch.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.synch.SecurityOfSqlite;

@Repository
@Transactional
public interface IBoSecurityDao {

	void clearTable();

	void saveBoSecurities(List<SecurityOfSqlite> boSecurities);

}
