package cn.fintechstar.admin.service.group;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.group.IGroupArchieveDao;
import cn.fintechstar.admin.model.group.GroupArchieve;

@Service
public class GroupArchieveService {
	
	@Autowired
	private IGroupArchieveDao groupArchieveDao;

	public void saveGroupArchieve(int groupId, GroupArchieve groupArchieve) {
		if(groupArchieve != null){
			groupArchieve.setGroupId(groupId);
			groupArchieveDao.saveGroupArchieve(groupArchieve);
		}
	}

	public void updateGroupArchieve(GroupArchieve groupArchieve) {
		if(groupArchieve != null){
			groupArchieveDao.updateGroupArchieve(groupArchieve);
		}
	}

	public GroupArchieve getGroupArchieveByGroupId(int groupId) {
		return groupArchieveDao.getGroupArchieveByGroupId(groupId);
	}

}
