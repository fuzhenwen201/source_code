package cn.fintechstar.admin.dao.group;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupMargin;

@Repository
@Transactional
public interface IGroupMarginDao {

	void saveGroupMargin(GroupMargin groupMargin);

	void updateGroupMargin(GroupMargin groupMargin);

	GroupMargin getGroupMarginByGroupId(int groupId);

}
