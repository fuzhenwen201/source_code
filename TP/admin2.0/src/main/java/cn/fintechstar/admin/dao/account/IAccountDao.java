package cn.fintechstar.admin.dao.account;

import cn.fintechstar.admin.model.account.Account;

public interface IAccountDao {

	void saveAccount(Account account);

	void updateAccount(Account account);


}
