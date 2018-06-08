package cn.fintechstar.admin.dao.manager;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.manager.Manager;

@Repository
@Transactional
public interface IManagerDao {

	void saveManager(Manager manager);

	void updateManager(Manager manager);

	void deleteManager(int managerId);

	void enableManagerByManagerId(int managerId, boolean enable);

	List<Manager> getManagers();

	List<Manager> getManagerByManagerId(int managerId);

}
