package cn.fintechstar.admin.dao.group;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupArchieve;

@Repository
@Transactional
public interface IGroupArchieveDao {

	void saveGroupArchieve(GroupArchieve groupArchieve);

	void updateGroupArchieve(GroupArchieve groupArchieve);

	GroupArchieve getGroupArchieveByGroupId(int groupId);

}
