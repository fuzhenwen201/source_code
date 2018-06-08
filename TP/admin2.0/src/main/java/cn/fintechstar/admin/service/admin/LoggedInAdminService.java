package cn.fintechstar.admin.service.admin;

import java.util.Date;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.admin.ILoggedInAdminDao;
import cn.fintechstar.admin.model.admin.LoggedInAdmin;
import cn.fintechstar.admin.model.admin.message.BasicLoginReq;

@Service
public class LoggedInAdminService {
	
	@Autowired
	private ILoggedInAdminDao loggedInAdminDao;

	public LoggedInAdmin saveLoggedInAdmin(BasicLoginReq basicLoginReq) {
		LoggedInAdmin loggedInAdmin = getLoggedInAdmin(basicLoginReq);
		loggedInAdminDao.saveLoggedInAdmin(loggedInAdmin);
		return loggedInAdmin;
	}

	private LoggedInAdmin getLoggedInAdmin(BasicLoginReq basicLoginReq) {
		LoggedInAdmin loggedInAdmin = new LoggedInAdmin();
		loggedInAdmin.setAdminId(basicLoginReq.getAdminId());
		loggedInAdmin.setIpAddress(basicLoginReq.getIpAdress());
		loggedInAdmin.setMacAddress(basicLoginReq.getMacAddress());
		loggedInAdmin.setHardDiskSerialNo(basicLoginReq.getHardDiskSerialno());
		loggedInAdmin.setReqId(basicLoginReq.getReqId());
		loggedInAdmin.setSessionId(basicLoginReq.getSession());
		loggedInAdmin.setLoginTime(new Date());
		return loggedInAdmin;
	}

	public void logout(int adminId) {
		int adminIdFromOnlineMap = OnlineAdminService.adminLoginMap.get(adminId);
		if(adminIdFromOnlineMap != 0){
			LoggedInAdmin loggedInAdmin = loggedInAdminDao.getLoggedInAdminById(adminId);
			if(loggedInAdmin != null){
				loggedInAdmin.setLogoutTime(new Date());
				loggedInAdminDao.updateLoggedInAdmin(loggedInAdmin);
				OnlineAdminService.adminLoginMap.remove(adminId);
			}
		}
	}

}
