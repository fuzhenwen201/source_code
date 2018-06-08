package cn.fintechstar.admin.service.manager;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import com.google.gson.Gson;

import cn.fintechstar.admin.dao.manager.IManagerDao;
import cn.fintechstar.admin.model.manager.Manager;
import cn.fintechstar.admin.model.manager.message.BasicManager;
import cn.fintechstar.admin.model.manager.message.DeleteManager;
import cn.fintechstar.admin.model.manager.message.DeleteManagerStatus;

@Service
public class ManagerCommonService {
	
	private static final Logger LOGGER = Logger.getLogger(ManagerCommonService.class);
	
	private static final Gson GSON = new Gson();
	
	@Autowired
	private IManagerDao managerDao;	

	public int saveOrUpdateMananger(BasicManager createOrUpdateManagerReq) {
		Manager manager = getManager(createOrUpdateManagerReq);
		if(manager.getManagerId() == 0){
			managerDao.saveManager(manager);
		}else{
			managerDao.updateManager(manager);
			if(manager.getIsEnable()){
				// TODO force logout manager from CMT
			}
		}
		return manager.getManagerId();
	}

	private Manager getManager(BasicManager createOrUpdateManagerReq) {
		Manager manager = new Manager();
		manager.setManagerId(createOrUpdateManagerReq.getManagerId());
		manager.setGroupId(createOrUpdateManagerReq.getGroupId());
		manager.setPassword(createOrUpdateManagerReq.getPassword());
		manager.setFirstName(createOrUpdateManagerReq.getFirstName());
		manager.setLastName(createOrUpdateManagerReq.getLastName());
		manager.setEmail(createOrUpdateManagerReq.getEmail());
		manager.setAddress(createOrUpdateManagerReq.getAddress());
		manager.setCity(createOrUpdateManagerReq.getCity());
		manager.setState(createOrUpdateManagerReq.getState());
		manager.setCountry(createOrUpdateManagerReq.getCountry());
		manager.setPin(createOrUpdateManagerReq.getPin());
		manager.setIsEnable(createOrUpdateManagerReq.isEnable());
		return manager;
	}

	public DeleteManager deleteManager(DeleteManager deleteManagerReq) {
		if(CollectionUtils.isEmpty(deleteManagerReq.getDeleteManagerStatus())){
			for(DeleteManagerStatus deleteManagerStatus : deleteManagerReq.getDeleteManagerStatus()){
				try{
					managerDao.deleteManager(deleteManagerStatus.getManagerId());
					deleteManagerStatus.setStatus(true);
					deleteManagerStatus.setResponseMsg("Deleted Successfully");
				}catch (Exception e) {
					deleteManagerStatus.setStatus(false);
					e.printStackTrace();
					LOGGER.error("DeleteManager request is : " + GSON.toJson(deleteManagerReq));
				}	
			}
		}
		return deleteManagerReq;
	}

	public void enableManagerByManagerId(int managerId, boolean enable) {
		managerDao.enableManagerByManagerId(managerId, enable);
	}

	public List<BasicManager> getManagers() {
		List<Manager> managers = managerDao.getManagers();
		List<BasicManager> managerVos = getManagers(managers);
		return managerVos;
	}

	private List<BasicManager> getManagers(List<Manager> managers) {
		List<BasicManager> managerVos = new ArrayList<>();
		for(Manager manager : managers){
			BasicManager managerVo = new BasicManager();
			managerVo.setManagerId(manager.getManagerId());
			managerVo.setGroupId(manager.getGroupId());
			managerVo.setFirstName(manager.getFirstName());
			managerVo.setLastName(manager.getLastName());
			managerVo.setEmail(manager.getEmail());
			managerVo.setAddress(manager.getAddress());
			managerVo.setCity(manager.getCity());
			managerVo.setState(manager.getState());
			managerVo.setCountry(manager.getCountry());
			managerVo.setPin(manager.getPin());
			managerVo.setEnable(manager.getIsEnable());
			managerVos.add(managerVo);
		}
		return managerVos;
	}

	public List<BasicManager> getManagerByManagerId(int managerId) {
		List<Manager> managers = managerDao.getManagerByManagerId(managerId);
		List<BasicManager> managerVos = getManagers(managers);
		return managerVos;
	}

}
