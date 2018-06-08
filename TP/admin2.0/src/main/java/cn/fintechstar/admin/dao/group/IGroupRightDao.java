package cn.fintechstar.admin.dao.group;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupRight;

@Repository
@Transactional
public interface IGroupRightDao {

	void saveGroupRight(GroupRight groupRight);

	void updateGroupRight(GroupRight groupRight);

	GroupRight getGroupRightByGroupId(int groupId);

}
