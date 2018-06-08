package cn.fintechstar.traddc.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import cn.fintechstar.traddc.dao.HistoricalSignalDao;
import cn.fintechstar.traddc.model.Instrument;

@EnableJpaRepositories
@Repository
public class HistoricalSignalDaoImpl implements HistoricalSignalDao {

	@Autowired
	@Qualifier("groupTemplate")
	private JdbcTemplate jdbcTemplate;

	@Override
	public int getGroupSpread(int groupId, String requestTime, Instrument instrument) {
		// SELECT SPREAD, (SELECT max(ID) FROM GROUP_1 WHERE julianday(julianday("2017-08-22 10:36:00") - datetime(CREATE_TIME)) > 0) AS TMPID 
		// FROM GROUP_1 WHERE TMPID = ID AND SYMBOLS LIKE "%SGD_JPY%"
		String sql = "SELECT SPREAD, (SELECT max(ID) FROM GROUP_" + groupId + " WHERE julianday(julianday(\""
				+ requestTime + "\") - datetime(CREATE_TIME)) > 0) AS TMPID FROM GROUP_" + groupId
				+ " WHERE TMPID = ID AND SYMBOLS LIKE \"%" + instrument + "%\"";
		int spread = jdbcTemplate.query((conn) -> {
			PreparedStatement ps = conn.prepareStatement(sql);
			return ps;
		}, (rs) -> {
			if (rs.next()) {
				return rs.getInt(0);
			}
			return 0;
		});
		return spread;
	}

}
