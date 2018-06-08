package cn.fintechstar.admin.service.admin;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.admin.IAdminDao;
import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.AdminVo;

@Service
public class AdminCommonService {
	
	@Autowired
	private IAdminDao adminDao;

	public Admin getAdminByAdminId(int adminId) {
		return adminDao.getAdminByAdminId(adminId);
	}

	public void updateChangePasswordByAdminId(int adminid, String password) {
		adminDao.updateChangePasswordByAdminId(adminid, password);
	}

	public boolean isSuperAdmin(int adminId) {
		int isSuperAdmin = adminDao.isSuperAdmin(adminId);
		return isSuperAdmin == 0 ? false : true;
	}

	public void saveAdmin(Admin admin) {
		adminDao.saveAdmin(admin);
	}

	public void updateAdmin(Admin admin) {
		adminDao.updateAdmin(admin);
	}

	public void deleteAdmins(List<Integer> adminIds) {
		adminDao.deleteAdmins(adminIds);
	}

	public List<AdminVo> getAdmins() {
		return adminDao.getAdmins();
	}

}
