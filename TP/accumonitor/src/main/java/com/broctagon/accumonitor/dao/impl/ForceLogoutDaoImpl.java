package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.ForceLogoutDao;

@Repository
public class ForceLogoutDaoImpl implements ForceLogoutDao {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final String GET_ID = "SELECT ID FROM FORCELOGOUT_MONITOR WHERE REQUEST_ID = ?";

	private static final String ADD_FORCELOGOUTREQ = "INSERT INTO FORCELOGOUT_MONITOR(REQUEST_ID, FORCE_LOGOUT_REQ) VALUES (?, ?)";
	
	private static final String UPDATE_FORCELOGOUTREQ = "UPDATE FORCELOGOUT_MONITOR SET FORCE_LOGOUT_REQ = ? WHERE ID = ?";
	
	private static final String ADD_FORCELOGOUTANS = "INSERT INTO FORCELOGOUT_MONITOR(REQUEST_ID, FORCE_LOGOUT_ANS) VALUES (?, ?)";
	
	private static final String UPDATE_FORCELOGOUTANS = "UPDATE FORCELOGOUT_MONITOR SET FORCE_LOGOUT_ANS = ? WHERE ID = ?";

	private static final String GET_REQUESTID_BY_EMPTY_FORCELOGINREQ = "SELECT REQUEST_ID FROM FORCELOGOUT_MONITOR WHERE FORCE_LOGOUT_REQ IS NULL";
	
	private static final String ADD_LOGININFO = "UPDATE FORCELOGOUT_MONITOR SET LOGIN_REQ = ?, LOGIN_ANS = ? WHERE REQUEST_ID = ?";
	
	@Override
	public void addForceLogoutReq(String requestId, String message) {
	final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_FORCELOGOUTREQ);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_FORCELOGOUTREQ);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
	}

	@Override
	public void addForceLogoutAns(String requestId, String message) {
	final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_FORCELOGOUTANS);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_FORCELOGOUTANS);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
	}
	
	private Integer getId(String requestId) {
		return jdbcTemplate.query((con) ->{
			PreparedStatement ps = con.prepareStatement(GET_ID);
			ps.setString(1, requestId);
			return ps;
		}, (rs) -> {return rs.next() ? rs.getInt(1) : null;});
	}

	@Override
	public List<String> getRequestIdByEmptyForceLoginReq() {
		return jdbcTemplate.queryForList(GET_REQUESTID_BY_EMPTY_FORCELOGINREQ, String.class);
	}

	@Override
	public void addLoginInfo(String requestId, String loginReq, String loginAns){
		jdbcTemplate.update((con) -> {
			PreparedStatement ps = con.prepareStatement(ADD_LOGININFO);
			ps.setString(1, loginReq);
			ps.setString(2, loginAns);
			ps.setString(3, requestId);
			return ps;
		});
	}


}
