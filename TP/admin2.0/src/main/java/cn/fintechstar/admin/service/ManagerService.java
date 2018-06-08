package cn.fintechstar.admin.service;


import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.model.manager.AccessRight;
import cn.fintechstar.admin.model.manager.message.AddManagerToGroupAns;
import cn.fintechstar.admin.model.manager.message.AddManagerToGroupReq;
import cn.fintechstar.admin.model.manager.message.BasicManager;
import cn.fintechstar.admin.model.manager.message.BasicManagerAns;
import cn.fintechstar.admin.model.manager.message.DeleteManager;
import cn.fintechstar.admin.model.manager.message.EnableManagerAns;
import cn.fintechstar.admin.model.manager.message.EnableManagerReq;
import cn.fintechstar.admin.model.manager.message.GetManagerAccessRightAns;
import cn.fintechstar.admin.model.manager.message.GetManagerAccessRightReq;
import cn.fintechstar.admin.model.manager.message.GetManagersAns;
import cn.fintechstar.admin.model.manager.message.GetManagersReq;
import cn.fintechstar.admin.model.manager.message.UpdateManagerAccessRightReq;
import cn.fintechstar.admin.service.manager.ManagerCommonService;
import cn.fintechstar.admin.service.manager.ManagerRightService;

@Service
public class ManagerService {
	
	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(ManagerService.class);
	
	@Autowired
	private ManagerCommonService managerCommonService;
	
	@Autowired
	private ManagerRightService managerRightService;

	public BasicManagerAns createOrUpdateManager(String body) {
		BasicManager createOrUpdateManagerReq = GSON.fromJson(body, BasicManager.class);
		BasicManagerAns createOrUpdateManagerAns = getCreateOrUpdateManager(createOrUpdateManagerReq);
		return createOrUpdateManagerAns;
	}

	private BasicManagerAns getCreateOrUpdateManager(BasicManager createOrUpdateManagerReq) {
		BasicManagerAns createOrUpdateManagerAns = new BasicManagerAns();
		createOrUpdateManagerAns.setReqId(createOrUpdateManagerReq.getReqId());
		createOrUpdateManagerAns.setSession(createOrUpdateManagerReq.getSession());
		createOrUpdateManagerAns.setSgId(createOrUpdateManagerReq.getSgId());
		createOrUpdateManagerAns.setAdminId(createOrUpdateManagerReq.getAdminId());
		try{
			int managerId = managerCommonService.saveOrUpdateMananger(createOrUpdateManagerReq);
			createOrUpdateManagerAns.setManagerId(managerId);
			createOrUpdateManagerAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("CreateOrUpdate manager request is : " + GSON.toJson(createOrUpdateManagerReq));
			createOrUpdateManagerAns.setStatus(false);
		}
		return createOrUpdateManagerAns;
	}

	public AddManagerToGroupAns addManagerToGroup(String body) {
		AddManagerToGroupReq addManagerToGroupReq = GSON.fromJson(body, AddManagerToGroupReq.class);
		AddManagerToGroupAns addManagerToGroupsAns = getAddManagerToGroupAns(addManagerToGroupReq);
		return addManagerToGroupsAns;
	}

	private AddManagerToGroupAns getAddManagerToGroupAns(AddManagerToGroupReq addManagerToGroupReq) {
		AddManagerToGroupAns addManagerToGroupsAns = new AddManagerToGroupAns();
		addManagerToGroupsAns.setReqId(addManagerToGroupReq.getReqid());
		addManagerToGroupsAns.setSession(addManagerToGroupReq.getSession());
		addManagerToGroupsAns.setSgId(addManagerToGroupReq.getSgId());
		addManagerToGroupsAns.setAdminId(addManagerToGroupReq.getAdminId());
		try{
			
			addManagerToGroupsAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			addManagerToGroupsAns.setStatus(false);
		}
		return addManagerToGroupsAns;
	}

	public BasicManagerAns updateManagerAccessRight(String body) {
		UpdateManagerAccessRightReq managerAccessRightReq = GSON.fromJson(body, UpdateManagerAccessRightReq.class);
		BasicManagerAns managerAccessRightAns = getUpdateManagerAccessRightAns(managerAccessRightReq);
		return managerAccessRightAns;
	}

	private BasicManagerAns getUpdateManagerAccessRightAns(UpdateManagerAccessRightReq managerAccessRightReq) {
		BasicManagerAns managerAccessRightAns = new BasicManagerAns();
		managerAccessRightAns.setReqId(managerAccessRightReq.getReqId());
		managerAccessRightAns.setSession(managerAccessRightReq.getSession());
		managerAccessRightAns.setSgId(managerAccessRightReq.getSgId());
		managerAccessRightAns.setAdminId(managerAccessRightReq.getAdminId());
		managerAccessRightAns.setManagerId(managerAccessRightReq.getManagerId());
		try {
			managerRightService.updateManagerAccessRight(managerAccessRightReq);
			managerAccessRightAns.setStatus(true);
		} catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("UpdateManangerAccessRight request is : " + GSON.toJson(managerAccessRightReq));
			managerAccessRightAns.setStatus(false);
		}
		return managerAccessRightAns;
	}

	public DeleteManager deleteManager(String body) {
		DeleteManager deleteManagerReq = GSON.fromJson(body, DeleteManager.class);
		DeleteManager deleteManagerAns = getDeleleteManagerAns(deleteManagerReq);
		return deleteManagerAns;
	}

	private DeleteManager getDeleleteManagerAns(DeleteManager deleteManagerReq) {
		return managerCommonService.deleteManager(deleteManagerReq);
	}

	public EnableManagerAns enableManager(String body) {
		EnableManagerReq enableManagerReq = GSON.fromJson(body, EnableManagerReq.class);
		EnableManagerAns enableManagerAns = getEnableManagerAns(enableManagerReq);
		return enableManagerAns;
	}

	private EnableManagerAns getEnableManagerAns(EnableManagerReq enableManagerReq) {
		EnableManagerAns enableManagerAns = new EnableManagerAns();
		enableManagerAns.setReqId(enableManagerReq.getReqId());
		enableManagerAns.setSession(enableManagerReq.getSession());
		enableManagerAns.setSgId(enableManagerReq.getSgId());
		enableManagerAns.setAdminId(enableManagerReq.getAdminId());
		enableManagerAns.setManagerId(enableManagerReq.getManagerId());
		try {
			managerCommonService.enableManagerByManagerId(enableManagerReq.getManagerId(), enableManagerReq.isEnable());
		} catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("EnableManager request is : " + GSON.toJson(enableManagerReq));
			enableManagerAns.setStatus(false);
			enableManagerAns.setError("Encounter an internal error");
		}
		// TODO force logout manager from CMT
		return enableManagerAns;
	}

	public GetManagersAns getManagers(String body) {
		GetManagersReq getManagersReq = GSON.fromJson(body, GetManagersReq.class);
		GetManagersAns getManagersAns = getManagersAns(getManagersReq);
		return getManagersAns;
	}

	private GetManagersAns getManagersAns(GetManagersReq getManagersReq) {
		GetManagersAns getManagersAns = new GetManagersAns();
		getManagersAns.setReqId(getManagersReq.getReqId());
		getManagersAns.setSession(getManagersReq.getSession());
		getManagersAns.setSgId(getManagersReq.getSgId());
		getManagersAns.setAdminId(getManagersReq.getAdminId());
		try{
			// TODO need to check why manager id is 0
			if(getManagersReq.getManagerId() == 0){
				getManagersAns.setManagers(managerCommonService.getManagers());
			}else{
				getManagersAns.setManagers(managerCommonService.getManagerByManagerId(getManagersReq.getManagerId()));;
			}
		}catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("GetManagers request is : " + GSON.toJson(getManagersReq));
		}
		return getManagersAns;
	}

	public GetManagerAccessRightAns getManagerAccessRights(String body) {
		GetManagerAccessRightReq getManagerAccessRightReq = GSON.fromJson(body, GetManagerAccessRightReq.class);
		GetManagerAccessRightAns getManagerAccessRightAns = getManagerAccessRightsAns(getManagerAccessRightReq);
		return getManagerAccessRightAns;
	}

	private GetManagerAccessRightAns getManagerAccessRightsAns(GetManagerAccessRightReq getManagerAccessRightReq) {
		GetManagerAccessRightAns getManagerAccessRightAns = new GetManagerAccessRightAns();
		getManagerAccessRightAns.setReqId(getManagerAccessRightReq.getReqId());
		getManagerAccessRightAns.setSession(getManagerAccessRightReq.getSession());
		getManagerAccessRightAns.setSgId(getManagerAccessRightReq.getSgId());
		getManagerAccessRightAns.setAdminId(getManagerAccessRightReq.getAdminId());
		getManagerAccessRightAns.setManagerId(getManagerAccessRightReq.getManagerId());
		try{
			AccessRight accessRight = managerRightService.getManagerAccessRightByManagerId(getManagerAccessRightReq.getManagerId());
			getManagerAccessRightAns.setCreateUser(accessRight.getCreateUser());
			getManagerAccessRightAns.setDeleteTrader(accessRight.getDeleteTrader());
			getManagerAccessRightAns.setUpdateTrader(accessRight.getUpdateTrader());
			getManagerAccessRightAns.setViewTrader(accessRight.getViewTrader());
			getManagerAccessRightAns.setViewTraderDetails(accessRight.getViewTraderDetails());
			getManagerAccessRightAns.setCreateManager(accessRight.getCreateManager());
			getManagerAccessRightAns.setDeleteManager(accessRight.getDeleteManager());
			getManagerAccessRightAns.setUpdateManager(accessRight.getUpdateManager());
			getManagerAccessRightAns.setViewManager(accessRight.getViewManager());
			getManagerAccessRightAns.setViewManagerDetails(accessRight.getViewManagerDetails());
			getManagerAccessRightAns.setModifyManagerAccess(accessRight.getModifyManagerAccess());
			getManagerAccessRightAns.setCredit(accessRight.getCredit());
			getManagerAccessRightAns.setBalance(accessRight.getBalance());
			getManagerAccessRightAns.setOpenTrades(accessRight.getOpenTrades());
			getManagerAccessRightAns.setBoMonitoring(accessRight.getBoMonitoring());
		}catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("GetManagerAccessRight request is : " + GSON.toJson(getManagerAccessRightReq));
		}
		return getManagerAccessRightAns;
	}	
	
}
