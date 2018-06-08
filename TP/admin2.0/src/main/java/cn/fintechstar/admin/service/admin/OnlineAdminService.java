package cn.fintechstar.admin.service.admin;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArraySet;
import java.util.stream.Collectors;

import org.springframework.stereotype.Service;

import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.OnlineAdmin;
import cn.fintechstar.admin.model.admin.message.BasicLoginReq;
import cn.fintechstar.admin.model.admin.message.BasicLogoutReq;

@Service
public class OnlineAdminService {
	
	public static Set<OnlineAdmin> onlineAdmins = new CopyOnWriteArraySet<>();

	public static Map<Integer, Integer> adminLoginMap = new ConcurrentHashMap<>();

	
	public List<BasicLogoutReq> getForceLogoutReqs() {
		List<BasicLogoutReq> forceLogoutReqs = new ArrayList<>();
		if(!onlineAdmins.isEmpty()){
			for(OnlineAdmin onlineAdmin : onlineAdmins){
				BasicLogoutReq forceLogoutReq = getForceLogoutReq(onlineAdmin);
				forceLogoutReqs.add(forceLogoutReq);
			}
		}
		return forceLogoutReqs;
	}

	private BasicLogoutReq getForceLogoutReq(OnlineAdmin onlineAdmin) {
		BasicLogoutReq forceLogoutReq = new BasicLogoutReq();
		forceLogoutReq.setAdminId(onlineAdmin.getAdmin().getAdminId());
		forceLogoutReq.setReqId(onlineAdmin.getReqId());
		forceLogoutReq.setSession(onlineAdmin.getSession());
		forceLogoutReq.setSgId(onlineAdmin.getSgId());
		return forceLogoutReq;
	}

	public void addOnlineAdmin(Admin admin, BasicLoginReq basicLoginReq) {
		OnlineAdmin onlineAdmin = getOnlineAdmin(admin, basicLoginReq);
		onlineAdmins.add(onlineAdmin);
	}

	private OnlineAdmin getOnlineAdmin(Admin admin, BasicLoginReq basicLoginReq) {
		OnlineAdmin onlineAdmin = new OnlineAdmin();
		onlineAdmin.setAdmin(admin);
		onlineAdmin.setSgId(basicLoginReq.getSgId());
		onlineAdmin.setReqId(basicLoginReq.getReqId());
		onlineAdmin.setIpAddress(basicLoginReq.getIpAdress());
		onlineAdmin.setHardDiskSerialno(basicLoginReq.getHardDiskSerialno());
		onlineAdmin.setSession(basicLoginReq.getSession());
		onlineAdmin.setMacAddress(basicLoginReq.getMacAddress());
		return onlineAdmin;
	}

	public boolean isOnlineAdmin(int adminId) {
		boolean flag = false;
		List<OnlineAdmin> onlineAdmin = onlineAdmins.stream().filter(p->p.getAdmin().getAdminId()==adminId).collect(Collectors.toList());
		if(!onlineAdmin.isEmpty()){
			flag = true;
		}
		return flag;
	}

	public void removeFromOnline(int adminId) {
		OnlineAdmin onlineAdmin = getOnlineAdminByAdminId(adminId);
		onlineAdmins.remove(onlineAdmin);
	}

	private OnlineAdmin getOnlineAdminByAdminId(int adminId) {
		List<OnlineAdmin> onlineAdmin = onlineAdmins.stream().filter(p->p.getAdmin().getAdminId()==adminId).collect(Collectors.toList());
		if(!onlineAdmin.isEmpty()){
			return onlineAdmin.get(0);
		}else{
			return null;
		}
	}

	public List<OnlineAdmin> isOnlineAdmin(List<Integer> adminIds) {
		List<OnlineAdmin> onlineAdmins = new ArrayList<>();
		for(Integer adminId : adminIds){
			if(isOnlineAdmin(adminId)){
				onlineAdmins.add(getOnlineAdminByAdminId(adminId));
			}
		}
		return onlineAdmins;
	}

	public List<BasicLogoutReq> getForceLogoutReqs(List<OnlineAdmin> onlineAdminList) {
		List<BasicLogoutReq> logoutReqs = new ArrayList<>();
		for(OnlineAdmin onlineAdmin : onlineAdminList){
			logoutReqs.add(getForceLogoutReq(onlineAdmin));
		}
		return logoutReqs;
	}
}
