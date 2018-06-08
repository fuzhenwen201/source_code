package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.LogoutDao;

@Repository
public class LogoutDaoImpl implements LogoutDao{

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final String GET_ID = "SELECT ID FROM LOGOUT_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String UPDATE_LOGOUTREQ = "UPDATE LOGOUT_MONITOR SET LOGOUT_REQ = ? WHERE ID = ?";
	
	private static final String ADD_LOGOUTREQ = "INSERT INTO LOGOUT_MONITOR(REQUEST_ID, LOGOUT_REQ) VALUES(?, ?)";
	
	private static final String UPDATE_LOGOUTANS = "UPDATE LOGOUT_MONITOR SET LOGOUT_ANS = ? WHERE ID = ?";
	
	private static final String ADD_LOGOUTANS = "INSERT INTO LOGOUT_MONITOR(REQUEST_ID, LOGOUT_ANS) VALUES(?, ?)";
	
	private static final String DELETE_LOGOUT = "DELETE FROM LOGOUT_MONITOR WHERE REQUEST_ID IN ( ? ) ";
	
	@Override
	public void addLogoutReq(String requestId, String body) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) -> {
				PreparedStatement ps = con.prepareStatement(UPDATE_LOGOUTREQ);
				ps.setString(1, body);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) -> {
				PreparedStatement ps = con.prepareStatement(ADD_LOGOUTREQ);
				ps.setString(1, requestId);
				ps.setString(2, body);
				return ps;
			});
		}
	}
	
	@Override
	public void addLogoutAns(final String requestId, final String body) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) -> {
				PreparedStatement ps = con.prepareStatement(UPDATE_LOGOUTANS);
				ps.setString(1, body);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) -> {
				PreparedStatement ps = con.prepareStatement(ADD_LOGOUTANS);
				ps.setString(1, requestId);
				ps.setString(2, body);
				return ps;
			});
		}
	}

	private Integer getId(String requestId) {
		return jdbcTemplate.query((con) -> {
			PreparedStatement ps = con.prepareStatement(GET_ID);
			ps.setString(1, requestId);
			return ps;
		}, (rs) -> {return rs.next() ? rs.getInt(1) : null;});
	}

	@Override
	public void detelteLogoutByRequestId(String requestIds) {
		jdbcTemplate.update((con) ->{
			PreparedStatement ps = con.prepareStatement(DELETE_LOGOUT);
			ps.setString(1, requestIds);
			return ps;
		});
	}

}
