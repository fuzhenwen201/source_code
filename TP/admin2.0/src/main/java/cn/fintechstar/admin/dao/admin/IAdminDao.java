package cn.fintechstar.admin.dao.admin;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.AdminVo;

@Repository
@Transactional
public interface IAdminDao {

	Admin getAdminByAdminId(int adminId);

	void updateChangePasswordByAdminId(int adminid, String password);

	int isSuperAdmin(int adminId);

	void saveAdmin(Admin admin);

	void updateAdmin(Admin admin);

	void deleteAdmins(List<Integer> adminIds);

	List<AdminVo> getAdmins();

}
