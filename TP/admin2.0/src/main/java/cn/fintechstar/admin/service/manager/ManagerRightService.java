package cn.fintechstar.admin.service.manager;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.admin.dao.manager.IManagerRightDao;
import cn.fintechstar.admin.model.manager.AccessRight;
import cn.fintechstar.admin.model.manager.message.UpdateManagerAccessRightReq;

@Service
public class ManagerRightService {
	
	@Autowired
	private IManagerRightDao managerRightDao;

	public void updateManagerAccessRight(UpdateManagerAccessRightReq managerAccessRightReq) {
		AccessRight accessRight = managerRightDao.getManagerAccessRightByManagerId(managerAccessRightReq.getManagerId());
		if(accessRight != null){
			managerRightDao.deleteManagerAccessRightByManagerId(managerAccessRightReq.getManagerId());
		}
		managerRightDao.saveManagerAccessRight(getManagerAccess(managerAccessRightReq));
	}

	private AccessRight getManagerAccess(UpdateManagerAccessRightReq managerAccessRightReq) {
		AccessRight accessRight = new AccessRight();
		accessRight.setCreateUser(managerAccessRightReq.getCreateUser());
		accessRight.setDeleteTrader(managerAccessRightReq.getDeleteTrader());
		accessRight.setUpdateTrader(managerAccessRightReq.getUpdateTrader());
		accessRight.setViewTrader(managerAccessRightReq.getViewTrader());
		accessRight.setViewTraderDetails(managerAccessRightReq.getViewTraderDetails());
		accessRight.setCreateManager(managerAccessRightReq.getCreateManager());
		accessRight.setDeleteManager(managerAccessRightReq.getDeleteManager());
		accessRight.setUpdateManager(managerAccessRightReq.getUpdateManager());
		accessRight.setViewManager(managerAccessRightReq.getViewManager());
		accessRight.setViewManagerDetails(managerAccessRightReq.getViewManagerDetails());
		accessRight.setModifyManagerAccess(managerAccessRightReq.getModifyManagerAccess());
		accessRight.setCredit(managerAccessRightReq.getCredit());
		accessRight.setBalance(managerAccessRightReq.getBalance());
		accessRight.setOpenTrades(managerAccessRightReq.getOpenTrades());
		accessRight.setBoMonitoring(managerAccessRightReq.getBoMonitoring());
		return accessRight;
	}

	public AccessRight getManagerAccessRightByManagerId(int managerId) {
		return managerRightDao.getManagerAccessRightByManagerId(managerId);
	}
}
