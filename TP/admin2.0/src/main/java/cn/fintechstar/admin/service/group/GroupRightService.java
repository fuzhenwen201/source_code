package cn.fintechstar.admin.service.group;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.group.IGroupRightDao;
import cn.fintechstar.admin.model.group.GroupRight;

@Service
public class GroupRightService {

	@Autowired
	private IGroupRightDao groupRightDao;

	public void saveGroupRight(int groupId, GroupRight groupRight) {
		if(groupRight != null){
			groupRight.setGroupId(groupId);
			groupRightDao.saveGroupRight(groupRight);
		}
	}

	public void updateGroupRight(GroupRight groupRight) {
		if(groupRight != null){
			groupRightDao.updateGroupRight(groupRight);
		}
	}

	public GroupRight getGroupRightByGroupId(int groupId) {
		return groupRightDao.getGroupRightByGroupId(groupId);
	}
}
