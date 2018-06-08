package cn.fintechstar.dcb.dao.impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import cn.fintechstar.dcb.dao.InstantSignalDao;

@Repository
public class InstantSignalDaoImpl implements InstantSignalDao{

	@Autowired 
	private JdbcTemplate jdbcTemplate;
	
	@Override
	public void saveInstantSignal(String sql) {
		jdbcTemplate.update(sql);
	}

}
