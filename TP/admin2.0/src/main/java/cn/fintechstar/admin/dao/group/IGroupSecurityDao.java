package cn.fintechstar.admin.dao.group;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupSecurity;
import cn.fintechstar.admin.model.synch.SecurityOfGroupBO;

@Repository
@Transactional
public interface IGroupSecurityDao {

	void saveGroupSecurities(List<GroupSecurity> groupSecurities);

	List<GroupSecurity> getGroupSecuritiesByGroupId(int groupId);

	List<String> getSecurityNamesByGroupId(int groupId);

	void deleteGroupSecuritiesByGroupId(int groupId);

	List<SecurityOfGroupBO> getBoGroups();

	List<SecurityOfGroupBO> getFxGroups();

}
