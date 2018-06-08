package com.broctagon.trad.reportserver.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.trad.reportserver.model.BalanceUser;
import com.broctagon.trad.reportserver.model.ShortUser;
import com.broctagon.trad.reportserver.model.User;

@Repository
@Transactional
public interface UserDao {

	public User getUserByLoginId(Integer loginId);
	public List<User> getUsersByLoginIds(List<Integer> loginIds);
	
	public int addUser(User user);
	public int addUsers(List<User> users);
	public int updateShortUsers(List<ShortUser> shortUsers);
	public int updateBalanceUser(BalanceUser balanceUser);
	public int updateBalanceUsers(List<BalanceUser> balanceUsers);
	
	public int updateUser(User user);
	public int updateUsers(List<User> users);
	 
	public int deleteUserByLoginId(Integer loginId);
	public int deleteUsersByLoginIds(List<Integer> loginIds);
	
}
