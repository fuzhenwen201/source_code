package cn.fintechstar.admin.dao.group;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupReport;

@Repository
@Transactional
public interface IGroupReportDao {

	void saveGroupReport(GroupReport groupReport);

	void updateGroupReport(GroupReport groupReport);

	GroupReport getGroupReportByGroupId(int groupId);

}
