package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.UserInfoDao;

@Repository
public class UserInfoDaoImpl implements UserInfoDao {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final String GET_ID = "SELECT ID FROM LOGIN_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String ADD_USERINFO = "INSERT INTO LOGIN_MONITOR(REQUEST_ID, USER_INFO) VALUES (?, ?)";
	
	private static final String UPDATE_USERINFO = "UPDATE LOGIN_MONITOR SET USER_INFO = ? WHERE ID = ?";


	@Override
	public void addUserInfo(String requestId, String message) {
		final Integer id = jdbcTemplate.query((con) ->{
			PreparedStatement ps = con.prepareStatement(GET_ID);
			ps.setString(1, requestId);
			return ps;
		}, (rs) -> {return rs.next() ? rs.getInt(1) : null;});
		
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_USERINFO);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_USERINFO);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
	}

}
