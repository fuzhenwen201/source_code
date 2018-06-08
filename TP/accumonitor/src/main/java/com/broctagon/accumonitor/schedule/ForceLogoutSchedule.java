package com.broctagon.accumonitor.schedule;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.broctagon.accumonitor.dao.ForceLogoutDao;
import com.broctagon.accumonitor.dao.LoginDao;
import com.broctagon.accumonitor.dao.LogoutDao;


@Component
public class ForceLogoutSchedule {

	@Autowired
	private LoginDao loginDao;
	
	@Autowired
	private LogoutDao logoutDao;
	
	@Autowired
	private ForceLogoutDao forceLogoutDao;
	
	/**
	 * A user login when another one use the same account logined, it will force logout the pre-user logined.
	 * Assert login or forceLogout by whether forcelogin_req is null or not.
	 * If force logout, We need remove login_req from the table login_monitor and add the login_req to the table forcelogout_monitor.
	 **/
	@Scheduled(cron = "*/1 * * * * * ")
	public void updateForceLogout(){
		List<String> requestIds = forceLogoutDao.getRequestIdByEmptyForceLoginReq();
		Map<String, List<String>> loginReqMap = getLoginReqMap(requestIds);
		loginReqMap.entrySet().parallelStream().forEach(map -> {
			if(map.getValue().size() > 0){
				forceLogoutDao.addLoginInfo(map.getKey(), map.getValue().get(0), map.getValue().get(1));
			}
		});
		loginReqMap.entrySet().parallelStream().forEach(map -> loginDao.deleteMessageByRequestId(map.getKey()));

	}
	
	@Scheduled(cron = "*/1 * * * * *")
	public void updateLogout(){
		List<String> requestIds = forceLogoutDao.getRequestIdByEmptyForceLoginReq();
		String sql = getSql(requestIds);
		if(sql != null){
			logoutDao.detelteLogoutByRequestId(sql);
		}
	}

	private String getSql(List<String> requestIds) {
		if(requestIds.size() > 0){
			StringBuilder sb = new StringBuilder();
			for(String requestId : requestIds){
				sb.append("'").append(requestId).append("', ");
			}
			return sb.toString().substring(0, sb.length() - 2);
		}
		return null;
	}

	private Map<String, List<String>> getLoginReqMap(List<String> requestIds) {
		Map<String, List<String>> loginReqMap = new HashMap<>();
		for(String requestId : requestIds){
			if (!StringUtils.isEmpty(requestId)) {
				List<String> loginReq = loginDao.getLoginInfoByRequestId(requestId);
				loginReqMap.put(requestId, loginReq);
			}
		}
		return loginReqMap;
	}
}
