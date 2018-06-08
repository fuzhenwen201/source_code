package cn.fintechstar.admin.service.account;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.account.IAccountDao;
import cn.fintechstar.admin.model.account.Account;

@Service
public class AccountCommonService {

	@Autowired
	private IAccountDao accountDao;
	
	public void saveOrUpdateAccount(Account account) {
		if(account.getAccountId() == 0){
			accountDao.saveAccount(account);
		}else{
			accountDao.updateAccount(account);
		}
			
	}

}
