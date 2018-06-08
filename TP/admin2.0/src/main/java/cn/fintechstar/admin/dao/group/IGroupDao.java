package cn.fintechstar.admin.dao.group;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupDetailsVo;

@Repository
@Transactional
public interface IGroupDao {

	void saveGroup(GroupCommon groupCommon);

	void updateGroup(GroupCommon groupCommon);

	GroupCommon getGroupCommonByGroupId(int groupId);

	List<GroupDetailsVo> getGroups();

}
