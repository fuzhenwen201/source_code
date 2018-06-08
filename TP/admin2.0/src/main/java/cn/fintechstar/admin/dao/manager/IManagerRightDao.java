package cn.fintechstar.admin.dao.manager;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.manager.AccessRight;

@Repository
@Transactional
public interface IManagerRightDao {

	AccessRight getManagerAccessRightByManagerId(int managerId);

	void deleteManagerAccessRightByManagerId(int managerId);

	void saveManagerAccessRight(AccessRight managerAccessRightReq);

}
