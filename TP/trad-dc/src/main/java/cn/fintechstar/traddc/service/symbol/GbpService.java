package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.gbp.GbpAudDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpCadDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpChfDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpHkdDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpJpyDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpNzdDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpPlnDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpSgdDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpUsdDao;
import cn.fintechstar.traddc.dao.instant.gbp.GbpZarDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class GbpService {

	@Autowired
	private GbpAudDao gbpAudDao;
	
	@Autowired
	private GbpCadDao gbpCadDao;
	
	@Autowired
	private GbpChfDao gbpChfDao;
	
	@Autowired
	private GbpHkdDao gbpHkdDao;
	
	@Autowired
	private GbpJpyDao gbpJpyDao;
	
	@Autowired
	private GbpNzdDao gbpNzdDao;
	
	@Autowired
	private GbpPlnDao gbpPlnDao;
	
	@Autowired
	private GbpSgdDao gbpSgdDao;
	
	@Autowired
	private GbpUsdDao gbpUsdDao;
	
	@Autowired
	private GbpZarDao gbpZarDao;
	
	
	public void saveGbpAud(InstantSignal instantSignal){
		gbpAudDao.saveGbpAud(instantSignal);
	}
	
	public void saveGbpCad(InstantSignal instantSignal){
		gbpCadDao.saveGbpCad(instantSignal);
	}
	
	public void saveGbpChf(InstantSignal instantSignal){
		gbpChfDao.saveGbpChf(instantSignal);
	}
	
	public void saveGbpHkd(InstantSignal instantSignal){
		gbpHkdDao.saveGbpHkd(instantSignal);
	}
	
	public void saveGbpJpy(InstantSignal instantSignal){
		gbpJpyDao.saveGbpJpy(instantSignal);
	}
	
	public void saveGbpNzd(InstantSignal instantSignal){
		gbpNzdDao.saveGbpNzd(instantSignal);
	}
	
	public void saveGbpPln(InstantSignal instantSignal){
		 gbpPlnDao.saveGbpPln(instantSignal);
	}
	
	public void saveGbpSgd(InstantSignal instantSignal){
		gbpSgdDao.saveGbpSgd(instantSignal);
	}
	
	public void saveGbpUsd(InstantSignal instantSignal){
		gbpUsdDao.saveGbpUsd(instantSignal);
	}
	
	public void saveGbpZar(InstantSignal instantSignal){
		gbpZarDao.saveGbpZar(instantSignal);
	}
	
}
