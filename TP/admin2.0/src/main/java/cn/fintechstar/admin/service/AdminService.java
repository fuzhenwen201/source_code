package cn.fintechstar.admin.service;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import com.google.gson.Gson;

import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.LoggedInAdmin;
import cn.fintechstar.admin.model.admin.OnlineAdmin;
import cn.fintechstar.admin.model.admin.message.BasicAdminAns;
import cn.fintechstar.admin.model.admin.message.BasicLoginAns;
import cn.fintechstar.admin.model.admin.message.BasicLoginReq;
import cn.fintechstar.admin.model.admin.message.BasicLogoutAns;
import cn.fintechstar.admin.model.admin.message.BasicLogoutReq;
import cn.fintechstar.admin.model.admin.message.ChangePasswordAns;
import cn.fintechstar.admin.model.admin.message.ChangePasswordReq;
import cn.fintechstar.admin.model.admin.message.CreateAdminReq;
import cn.fintechstar.admin.model.admin.message.DeleteAdminAns;
import cn.fintechstar.admin.model.admin.message.DeleteAdminReq;
import cn.fintechstar.admin.model.admin.message.GetAdminsAns;
import cn.fintechstar.admin.model.admin.message.GetAdminsReq;
import cn.fintechstar.admin.model.admin.message.UpdateAdminReq;
import cn.fintechstar.admin.service.admin.AdminCommonService;
import cn.fintechstar.admin.service.admin.LoggedInAdminService;
import cn.fintechstar.admin.service.admin.OnlineAdminService;
import cn.fintechstar.admin.util.PasswordGeneratorUtil;

@Service
public class AdminService {

	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(AdminService.class);
	
	@Autowired
	private AdminCommonService adminCommonService;
	
	@Autowired
	private LoggedInAdminService loggedInAdminService;
	
	@Autowired
	private OnlineAdminService onlineAdminService;
	
	public BasicLoginAns login(String body) {
		BasicLoginReq loginReq = GSON.fromJson(body, BasicLoginReq.class);
		BasicLoginAns loginAns = getBasicLoginAns(loginReq);
		return loginAns;
	}

	private BasicLoginAns getBasicLoginAns(BasicLoginReq basicLoginReq) {
		BasicLoginAns basicLoginAns = new BasicLoginAns();
		basicLoginAns.setReqId(basicLoginReq.getReqId());
		basicLoginAns.setSession(basicLoginReq.getSession());
		basicLoginAns.setSgId(basicLoginReq.getSgId());
		basicLoginAns.setAdminId(basicLoginReq.getAdminId());
		try{
			Admin admin = adminCommonService.getAdminByAdminId(basicLoginReq.getAdminId());
			if(admin != null && admin.getPassword().equals(PasswordGeneratorUtil.doMd5Hashing(basicLoginReq.getPassword()))){
				basicLoginAns.setSuperAdmin(admin.getIsSuperAdmin());
				// force logout other online user
				List<BasicLogoutReq> forceLogoutReqs = onlineAdminService.getForceLogoutReqs();
				if(forceLogoutReqs.size() > 1){
					LOGGER.error("More than 1 admin are online!");
				}
				for(BasicLogoutReq forceLogoutReq : forceLogoutReqs){
					getLogoutAns(forceLogoutReq);
				}
				// record login info
				LoggedInAdmin loggedInAdmin = loggedInAdminService.saveLoggedInAdmin(basicLoginReq);
				OnlineAdminService.adminLoginMap.put(basicLoginReq.getAdminId(), loggedInAdmin.getId());
				onlineAdminService.addOnlineAdmin(admin, basicLoginReq);
				basicLoginAns.setLoginStatus(true);
			}else{
				basicLoginAns.setLoginStatus(false);
				basicLoginAns.setError("Invalid User Name and Password");
			}
		}catch (Exception e) {
			e.printStackTrace();
			basicLoginAns.setLoginStatus(false);
			basicLoginAns.setError("Encountered an internal error, please try again");
			LOGGER.error("Login request : " + GSON.toJson(basicLoginReq));
		}
		return basicLoginAns;
	}

	public BasicLogoutAns logout(String body) {
		BasicLogoutReq logoutReq = GSON.fromJson(body, BasicLogoutReq.class);
		BasicLogoutAns logoutAns = getLogoutAns(logoutReq);
		return logoutAns;
	}

	private BasicLogoutAns getLogoutAns(BasicLogoutReq logoutReq) {
		BasicLogoutAns logoutAns = new BasicLogoutAns();
		logoutAns.setReqId(logoutReq.getReqId());
		logoutAns.setSession(logoutReq.getSession());
		logoutAns.setSgId(logoutReq.getSgId());
		logoutAns.setAdminId(logoutReq.getAdminId());
		try{
			if(onlineAdminService.isOnlineAdmin(logoutReq.getAdminId())){
				loggedInAdminService.logout(logoutReq.getAdminId());
				logoutAns.setStatus(true);
				onlineAdminService.removeFromOnline(logoutReq.getAdminId());
			}else{
				logoutAns.setStatus(false);
    			logoutAns.setError("Not able to logout because Admin is not online !!");
			}
		}catch (Exception e) {
			e.printStackTrace();
			logoutAns.setStatus(false);
			logoutAns.setError("Encountered an internal error, please try again");
			LOGGER.error("Logout request is : " + GSON.toJson(logoutReq));
		}
		return logoutAns;
	}

	public ChangePasswordAns changePassword(String body) {
		ChangePasswordReq changePasswordReq = GSON.fromJson(body, ChangePasswordReq.class);
		ChangePasswordAns changePasswordAns = getChangePasswordAns(changePasswordReq);
		return changePasswordAns;
	}

	private ChangePasswordAns getChangePasswordAns(ChangePasswordReq changePasswordReq) {
		ChangePasswordAns changePasswordAns = new ChangePasswordAns();
		changePasswordAns.setReqId(changePasswordReq.getReqId());
		changePasswordAns.setSession(changePasswordReq.getReqId());
		changePasswordAns.setSgId(changePasswordReq.getSgId());
		changePasswordAns.setAdminId(changePasswordReq.getAdminid());
		changePasswordAns.setPassword(changePasswordReq.getPassword());
		try{
			Admin admin = adminCommonService.getAdminByAdminId(changePasswordReq.getAdminid());
			if(admin.getPassword().equals(PasswordGeneratorUtil.doMd5Hashing(changePasswordReq.getOldPassword()))){
				adminCommonService.updateChangePasswordByAdminId(changePasswordReq.getAdminid(), PasswordGeneratorUtil.doMd5Hashing(admin.getPassword()));
				changePasswordAns.setStatus(true);
			}else{
				changePasswordAns.setErrorMessage("Incorrect Current Password");
				changePasswordAns.setStatus(false);
			}
		}catch (Exception e) {
			e.printStackTrace();
			changePasswordAns.setStatus(false);
			changePasswordAns.setErrorMessage("Encounter an internal error");
			LOGGER.error("Change password request is : " + GSON.toJson(changePasswordReq));
		}
		return changePasswordAns;
	}

	public BasicAdminAns createAdmin(String body) {
		CreateAdminReq createAdminReq = GSON.fromJson(body, CreateAdminReq.class);
		BasicAdminAns createAdminAns = getAdminAns(createAdminReq);
		return createAdminAns;
	}

	private BasicAdminAns getAdminAns(CreateAdminReq createAdminReq) {
		BasicAdminAns createAdminAns = new BasicAdminAns();
		createAdminAns.setReqId(createAdminReq.getReqId());
		createAdminAns.setSession(createAdminReq.getSession());
		createAdminAns.setSgId(createAdminReq.getSgId());
		createAdminAns.setName(createAdminReq.getName());
		createAdminAns.setAddress(createAdminReq.getAddress());
		createAdminAns.setPhoneNo(createAdminReq.getPhoneNo());
		createAdminAns.setSuperAdminId(createAdminReq.getSuperAdminId());
		try{
			if(adminCommonService.isSuperAdmin(createAdminReq.getSuperAdminId())){
				Admin admin = getAdmin(createAdminReq);
				adminCommonService.saveAdmin(admin);
				createAdminAns.setAdminId(admin.getAdminId());
				createAdminAns.setStatus(true);
			}else{
				createAdminAns.setStatus(false);
				createAdminAns.setError("You can't create admin for you are not super admin");
			}
		}catch (Exception e) {
			e.printStackTrace();
			createAdminAns.setStatus(false);
			createAdminAns.setError("Encounter an internal error");
			LOGGER.error("Create admin request is : " + GSON.toJson(createAdminReq));
		}
		return createAdminAns;
	}

	private Admin getAdmin(CreateAdminReq createAdminReq) {
		Admin admin = new Admin();
		admin.setName(createAdminReq.getName());
		admin.setPassword(createAdminReq.getPassword());
		admin.setAddress(createAdminReq.getAddress());
		admin.setPhoneNo(createAdminReq.getPhoneNo());
		return admin;
	}

	public BasicAdminAns updateAdmin(String body) {
		UpdateAdminReq updateAdminReq = GSON.fromJson(body, UpdateAdminReq.class);
		BasicAdminAns updateAdminAns = getUpdateAdminAns(updateAdminReq);
		return updateAdminAns;
	}

	private BasicAdminAns getUpdateAdminAns(UpdateAdminReq updateAdminReq) {
		BasicAdminAns updateAdminAns = new BasicAdminAns();
		updateAdminAns.setReqId(updateAdminReq.getReqId());
		updateAdminAns.setSession(updateAdminReq.getSession());
		updateAdminAns.setSgId(updateAdminReq.getSgId());
		updateAdminAns.setAdminId(updateAdminReq.getAdminId());
		updateAdminAns.setName(updateAdminReq.getName());
		updateAdminAns.setAddress(updateAdminReq.getAddress());
		updateAdminAns.setPhoneNo(updateAdminReq.getPhoneNo());
		updateAdminAns.setSuperAdminId(updateAdminReq.getSuperAdminId());
		try{
			if(adminCommonService.isSuperAdmin(updateAdminReq.getAdminId())){
				Admin admin = getAdmin(updateAdminReq);
				adminCommonService.updateAdmin(admin);
				updateAdminAns.setStatus(true);
			}else{
				updateAdminAns.setStatus(false);
				updateAdminAns.setError("You can't update admin for you are not super admin");	
			}
		}catch (Exception e) {
			e.printStackTrace();
			updateAdminAns.setStatus(false);
			updateAdminAns.setError("Encounter an internal error");
			LOGGER.error("Update admin request is : " + GSON.toJson(updateAdminReq));
		}
		return updateAdminAns;
	}

	private Admin getAdmin(UpdateAdminReq updateAdminReq) {
		Admin admin = new Admin();
		admin.setAdminId(updateAdminReq.getAdminId());
		admin.setName(updateAdminReq.getName());
		admin.setPassword(updateAdminReq.getPassword());
		admin.setAddress(updateAdminReq.getAddress());
		admin.setPhoneNo(updateAdminReq.getPhoneNo());
		admin.setIsDeleted(false);
		admin.setIsSuperAdmin(false);
		return admin;
	}

	public DeleteAdminAns deleteAdmin(String body) {
		DeleteAdminReq deleteAdminReq = GSON.fromJson(body, DeleteAdminReq.class);
		DeleteAdminAns deleteAdminAns = getDeleteAdminAns(deleteAdminReq);
		return deleteAdminAns;
	}

	private DeleteAdminAns getDeleteAdminAns(DeleteAdminReq deleteAdminReq) {
		DeleteAdminAns deleteAdminAns = new DeleteAdminAns();
		deleteAdminAns.setReqId(deleteAdminReq.getReqId());
		deleteAdminAns.setSession(deleteAdminReq.getSession());
		deleteAdminReq.setSgId(deleteAdminReq.getSgId());
		deleteAdminAns.setAdminId(deleteAdminReq.getAdminId());
		deleteAdminAns.setAdminIds(deleteAdminReq.getAdminIds());
		try{
			if(adminCommonService.isSuperAdmin(deleteAdminReq.getAdminId())){
				// force logout online admin
				List<OnlineAdmin> onlineAdmins = onlineAdminService.isOnlineAdmin(deleteAdminReq.getAdminIds());
				if(!CollectionUtils.isEmpty(onlineAdmins)){
					List<BasicLogoutReq> forceLogoutReqs = onlineAdminService.getForceLogoutReqs(onlineAdmins);
					getLogoutAns(forceLogoutReqs);
				}
				adminCommonService.deleteAdmins(deleteAdminReq.getAdminIds());
				deleteAdminAns.setStatus(true);
			}else{
				deleteAdminAns.setStatus(false);
				deleteAdminAns.setError("You can't delete admin for you are not super admin");	
			}
		}catch (Exception e) {
			e.printStackTrace();
			deleteAdminAns.setStatus(false);
			deleteAdminAns.setError("Encounter an internal error");
			LOGGER.error("Delete admin request is : " + GSON.toJson(deleteAdminReq));
		}
		return deleteAdminAns;
	}

	private void getLogoutAns(List<BasicLogoutReq> forceLogoutReqs) {
		for(BasicLogoutReq forceLogoutReq : forceLogoutReqs){
			getLogoutAns(forceLogoutReq);
		}
	}

	public GetAdminsAns getAdmins(String body) {
		GetAdminsReq getAdminsReq = GSON.fromJson(body, GetAdminsReq.class);
		GetAdminsAns getAdminsAns = getAdminsAns(getAdminsReq);
		return getAdminsAns;
	}

	private GetAdminsAns getAdminsAns(GetAdminsReq getAdminsReq) {
		GetAdminsAns getAdminsAns = new GetAdminsAns();
		getAdminsAns.setReqId(getAdminsReq.getReqId());
		getAdminsAns.setSession(getAdminsReq.getSession());
		getAdminsAns.setSgId(getAdminsReq.getSgId());
		getAdminsAns.setAdminId(getAdminsReq.getAdminId());
		getAdminsAns.setSuperAdmin(getAdminsReq.isSuperAdmin());
		try{
			if(adminCommonService.isSuperAdmin(getAdminsReq.getAdminId())){
				getAdminsAns.setAdmins(adminCommonService.getAdmins());
				getAdminsAns.setStatus(true);
			}
		}catch (Exception e) {
			e.printStackTrace();
			getAdminsAns.setStatus(false);
			getAdminsAns.setError("Encounter an internal error");
			LOGGER.error("Get admin request is : " + GSON.toJson(getAdminsReq));
		}
		return getAdminsAns;
	}

}
