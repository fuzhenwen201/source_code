package cn.fintechstar.admin.service.group;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.group.IGroupMarginDao;
import cn.fintechstar.admin.model.group.GroupMargin;

@Service
public class GroupMarginService {

	@Autowired
	private IGroupMarginDao groupMarginDao;
	
	public void saveGroupMargin(int groupId, GroupMargin groupMargin) {
		if(groupMargin != null){
			groupMargin.setGroupId(groupId);
			groupMarginDao.saveGroupMargin(groupMargin);
		}
	}

	public void updateGroupMargin(GroupMargin groupMargin) {
		if(groupMargin != null){
			groupMarginDao.updateGroupMargin(groupMargin);
		}
	}

	public GroupMargin getGroupMarginByGroupId(int groupId) {
		return groupMarginDao.getGroupMarginByGroupId(groupId);
	}

}
