package cn.fintechstar.admin.service.group;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.group.IGroupReportDao;
import cn.fintechstar.admin.model.group.GroupReport;

@Service
public class GroupReportService {

	@Autowired
	private IGroupReportDao groupReportDao;
	
	public void saveGroupReport(int groupId, GroupReport groupReport) {
		if(groupReport != null){
			groupReport.setGroupId(groupId);
			groupReportDao.saveGroupReport(groupReport);
		}
	}

	public void updateGroupReport(GroupReport groupReport) {
		if(groupReport != null){
			groupReportDao.updateGroupReport(groupReport);
		}
	}

	public GroupReport getGroupReportByGroupId(int groupId) {
		return groupReportDao.getGroupReportByGroupId(groupId);
	}

}
