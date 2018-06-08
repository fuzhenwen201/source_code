package cn.fintechstar.admin.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.synch.SecurityOfSymbol;

@Repository
@Transactional
public interface ISecurityDao {
	
	void updateSecurity(Security security);

	void saveSecurity(Security security);

	Security getSecurityById(int securityId);

	void deleteSecurityById(int securityId);

	List<Security> getSecurities();

	List<SecurityOfSymbol> getBoSecuritiesOfGroup();

	List<SecurityOfSymbol> getFxSecuritiesOfGroup();
}
