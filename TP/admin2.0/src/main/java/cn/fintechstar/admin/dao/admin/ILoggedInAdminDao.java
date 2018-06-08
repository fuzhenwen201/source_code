package cn.fintechstar.admin.dao.admin;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.admin.LoggedInAdmin;

@Repository
@Transactional
public interface ILoggedInAdminDao {

	void saveLoggedInAdmin(LoggedInAdmin loggedInAdmin);

	LoggedInAdmin getLoggedInAdminById(int id);

	void updateLoggedInAdmin(LoggedInAdmin loggedInAdmin);

}
