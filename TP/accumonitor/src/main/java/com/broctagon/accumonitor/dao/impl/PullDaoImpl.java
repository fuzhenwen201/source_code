package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.PullDao;

@Repository
public class PullDaoImpl implements PullDao {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;

	private static final String GET_ID = "SELECT ID FROM LOGOUT_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String UPDATE_PULLREQ = "UPDATE LOGOUT_MONITOR SET SYN_POSITION_REQ = ? WHERE ID = ?";
	
	private static final String ADD_PULLREQ = "INSERT INTO LOGOUT_MONITOR(REQUEST_ID, SYN_POSITION_REQ) VALUES(?, ?)";
	
	private static final String UPDATE_PULLANS = "UPDATE LOGOUT_MONITOR SET POSITION_RES = ? WHERE ID = ?";
	
	private static final String ADD_PULLANS = "INSERT INTO LOGOUT_MONITOR(REQUEST_ID, POSITION_RES) VALUES(?, ?)";
	
	@Override
	public void addPullRequest(String requestId, String message) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_PULLREQ);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_PULLREQ);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
	}

	@Override
	public void addPullResponse(String requestId, String message) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_PULLANS);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_PULLANS);
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

}
