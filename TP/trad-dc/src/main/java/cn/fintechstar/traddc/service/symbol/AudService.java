package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.aud.AudCadDao;
import cn.fintechstar.traddc.dao.instant.aud.AudChfDao;
import cn.fintechstar.traddc.dao.instant.aud.AudHkdDao;
import cn.fintechstar.traddc.dao.instant.aud.AudJpyDao;
import cn.fintechstar.traddc.dao.instant.aud.AudNzdDao;
import cn.fintechstar.traddc.dao.instant.aud.AudSgdDao;
import cn.fintechstar.traddc.dao.instant.aud.AudUsdDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class AudService {

	@Autowired
	private AudCadDao audCadDao;
	
	@Autowired
	private AudChfDao audChfDao;
	
	@Autowired
	private AudHkdDao audHkdDao;
	
	@Autowired
	private AudJpyDao audJpyDao;
	
	@Autowired
	private AudNzdDao audNzdDao;
	
	@Autowired
	private AudUsdDao audUsdDao;
	
	@Autowired
	private AudSgdDao augSgdDao;
	
	
	public void saveAudCad(InstantSignal instantSignal){
		audCadDao.saveAudCad(instantSignal);
	}
	
	public void saveAudChf(InstantSignal instantSignal){
		audChfDao.saveAudChf(instantSignal);
	}
	
	public void saveAudHkd(InstantSignal instantSignal){
		audHkdDao.saveAudHkd(instantSignal);
	}
		
	public void saveAudJpy(InstantSignal instantSignal){
		audJpyDao.saveAudJpy(instantSignal);
	}
	
	public void saveAudNzd(InstantSignal instantSignal){
		audNzdDao.saveAudNzd(instantSignal);
	}
	
	public void saveAudUsd(InstantSignal instantSignal){
		audUsdDao.saveAudUsd(instantSignal);
	}
	
	public void saveAudSgd(InstantSignal instantSignal){
		augSgdDao.saveAudSgd(instantSignal);
	}
}
