package cn.fintechstar.admin.service.group;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.group.IGroupDao;
import cn.fintechstar.admin.dao.group.IGroupSecurityDao;
import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupDetailsVo;

@Service
public class GroupCommonService {
	
	@Autowired
	private IGroupDao groupDao;

	@Autowired
	private IGroupSecurityDao groupSecurityDao;
	
	public void saveGroup(GroupCommon groupCommon) {
		if(groupCommon != null){
			groupDao.saveGroup(groupCommon);
		}
	}

	public void updateGroup(GroupCommon groupCommon) {
		if(groupCommon != null){
			groupDao.updateGroup(groupCommon);	
		}
	}

	public GroupCommon getGroupCommonByGroupId(int groupId) {
		return groupDao.getGroupCommonByGroupId(groupId);
	}

	public List<GroupDetailsVo> getGroups() {
		List<GroupDetailsVo> groupDetailsVos = groupDao.getGroups();
		for(GroupDetailsVo groupDetailsVo : groupDetailsVos){
			groupDetailsVo.setSecurityList(groupSecurityDao.getSecurityNamesByGroupId(groupDetailsVo.getGroupId()));
		}
		return groupDetailsVos;
	}
}
