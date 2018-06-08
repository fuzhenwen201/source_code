package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.LoginDao;

@Repository
public class LoginDaoImpl implements LoginDao {

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final String GET_ID = "SELECT ID FROM LOGIN_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String ADD_LOGINREQ = "INSERT INTO LOGIN_MONITOR(REQUEST_ID, LOGIN_REQ) VALUES (?, ?)";
	
	private static final String UPDATE_LOGINREQ = "UPDATE LOGIN_MONITOR SET LOGIN_REQ = ? WHERE ID = ?";
	
	private static final String ADD_LOGINANS = "INSERT INTO LOGIN_MONITOR(REQUEST_ID, LOGIN_ANS) VALUES (?, ?)";
	
	private static final String UPDATE_LOGINANS = "UPDATE LOGIN_MONITOR SET LOGIN_ANS = ? WHERE ID = ?";
	
	private static final String GET_LOGIN = "SELECT LOGIN_REQ, LOGIN_ANS FROM LOGIN_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String DELETE_MESSAGE = "DELETE FROM LOGIN_MONITOR WHERE REQUEST_ID = ?";
	
	@Override
	public void addLoginReq(String requestId, String message) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_LOGINREQ);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_LOGINREQ);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
	}
	
	@Override
	public void addLoginAns(final String requestId, final String message) {
		final Integer id = getId(requestId);
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_LOGINANS);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_LOGINANS);
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
	public List<String> getLoginInfoByRequestId(String requestId) {
		List<String> logins = new ArrayList<>();
		jdbcTemplate.query((con) -> {
			PreparedStatement ps = con.prepareStatement(GET_LOGIN);
			ps.setString(1, requestId);
			return ps;
		}, (rs) -> {
			if(rs.next()){
				logins.add(rs.getString(1));
				logins.add(rs.getString(2));
			}
			return rs;
		});
		return logins;
	}

	@Override
	public void deleteMessageByRequestId(String requestId) {
		jdbcTemplate.update((con) ->{
			PreparedStatement ps = con.prepareStatement(DELETE_MESSAGE);
			ps.setString(1, requestId);
			return ps;
		});
	}

}
