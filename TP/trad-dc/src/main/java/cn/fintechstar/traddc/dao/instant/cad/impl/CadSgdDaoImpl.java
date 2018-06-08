package cn.fintechstar.traddc.dao.instant.cad.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Scope;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import cn.fintechstar.traddc.dao.instant.cad.CadSgdDao;
import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.util.CommonUtil;
import cn.fintechstar.traddc.util.DateUtil;

@Repository
@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
public class CadSgdDaoImpl implements CadSgdDao {

	@Autowired
	@Qualifier("cadSgdTemplate")
	private JdbcTemplate jdbcTemplate;
 
	@Override
	public void saveCadSgd(InstantSignal instantSignal) {
		String sql = "INSERT INTO RAW_CAD_SGD(STATUS, BID, ASK, 'TIMESTAMP', TIME_FORMAT) VALUES(?, ?, ?, ?, ?)";
		jdbcTemplate.update((con) ->{
			PreparedStatement ps = con.prepareStatement(sql);
			ps.setString(1, instantSignal.getInstrument().toString());
			ps.setDouble(2, CommonUtil.intToDoubleByNumberFormat(instantSignal.getBid()));
			ps.setDouble(3, CommonUtil.intToDoubleByNumberFormat(instantSignal.getAsk()));
			ps.setLong(4, instantSignal.getTimestamp());
			ps.setString(5, DateUtil.getCurrentDateTimeStr(instantSignal.getTimestamp()));
			return ps;
		});
	}

}
