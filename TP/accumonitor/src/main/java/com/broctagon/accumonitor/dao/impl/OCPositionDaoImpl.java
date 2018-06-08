package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.OCPositionDao;

@Repository
public class OCPositionDaoImpl implements OCPositionDao {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final String GET_ID = "SELECT ID FROM LOGIN_MONITOR WHERE REQUEST_ID = ?";
	
	private static final String UPDATE_OCPOSITION = "UPDATE LOGIN_MONITOR SET OC_POSITION = ? WHERE ID = ?";
	
	private static final String ADD_POSITION = "INSERT INTO LOGIN_MONITOR(REQUEST_ID, OC_POSITION) VALUES(?, ?)";

	@Override
	public void addOCPosition(final String requestId, final String message) {
		
		final Integer id = jdbcTemplate.query((con) ->{
			PreparedStatement ps = con.prepareStatement(GET_ID);
			ps.setString(1, requestId);
			return ps;
		}, (rs) -> {return rs.next() ? rs.getInt(1) : null;});
				
		if(id != null){
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(UPDATE_OCPOSITION);
				ps.setString(1, message);
				ps.setInt(2, id);
				return ps;
			});
		}else{
			jdbcTemplate.update((con) ->{
				PreparedStatement ps = con.prepareStatement(ADD_POSITION);
				ps.setString(1, requestId);
				ps.setString(2, message);
				return ps;
			});
		}
		
	}

}
