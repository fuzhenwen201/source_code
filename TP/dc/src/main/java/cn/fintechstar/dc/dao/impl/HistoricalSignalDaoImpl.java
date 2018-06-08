package cn.fintechstar.dc.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import cn.fintechstar.dc.dao.HistoricalSignalDao;

@EnableJpaRepositories
@Repository
public class HistoricalSignalDaoImpl implements HistoricalSignalDao {

	@Autowired
	private JdbcTemplate jdbcTemplate;

	@Override
	public int getGroupSpread(int groupId, String instrument) {
		String sql = "SELECT SECURITY_SPREAD FROM GROUP_FX G INNER JOIN SECURITY_FX S ON G.SECURITY_ID = S.ID WHERE G.GROUP_ID = " +  groupId + " AND S.SYMBOL_LIST LIKE \"%" + instrument + "%\" ;";
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
