package cn.fintechstar.admin.service.group;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import cn.fintechstar.admin.dao.group.IGroupSecurityDao;
import cn.fintechstar.admin.model.group.GroupSecurity;

@Service
public class GroupSecurityService {
	
	@Autowired
	private IGroupSecurityDao groupSecurityDao;

	public void saveGroupSecurities(int groupId, List<GroupSecurity> groupSecurities) {
		if(!CollectionUtils.isEmpty(groupSecurities)){
			for(GroupSecurity groupSecurity : groupSecurities){
				groupSecurity.setGroupId(groupId);
			}
			groupSecurityDao.saveGroupSecurities(groupSecurities);
		}
	}

	public void updateGroupSecurities(int groupId, List<GroupSecurity> groupSecurities) {
		groupSecurityDao.deleteGroupSecuritiesByGroupId(groupId);
		if(!CollectionUtils.isEmpty(groupSecurities)){
			groupSecurityDao.saveGroupSecurities(groupSecurities);
		}
	}

	public List<GroupSecurity> getGroupSecuritiesByGroupId(int groupId) {
		return groupSecurityDao.getGroupSecuritiesByGroupId(groupId);
	}

}
