package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.nzd.NzdCadDao;
import cn.fintechstar.traddc.dao.instant.nzd.NzdChfDao;
import cn.fintechstar.traddc.dao.instant.nzd.NzdHkdDao;
import cn.fintechstar.traddc.dao.instant.nzd.NzdJpyDao;
import cn.fintechstar.traddc.dao.instant.nzd.NzdSdgDao;
import cn.fintechstar.traddc.dao.instant.nzd.NzdUsdDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class NzdService {

	@Autowired
	private NzdCadDao nzdCadDao;
	
	@Autowired
	private NzdChfDao nzdChfDao;
	
	@Autowired
	private NzdHkdDao nzdHkdDao;
	
	@Autowired
	private NzdJpyDao nzdJpyDao;
	
	@Autowired
	private NzdSdgDao nzdSdgDao;
	
	@Autowired
	private NzdUsdDao nzdUsdDao;
	
	
	public void saveNzdCad(InstantSignal instantSignal){
		nzdCadDao.saveNzdCad(instantSignal);
	}
	
	public void saveNzdChf(InstantSignal instantSignal){
		nzdChfDao.saveNzdChf(instantSignal);
	}
	
	public void saveNzdHkd(InstantSignal instantSignal){
		nzdHkdDao.saveNzdHkd(instantSignal);
	}
	
	public void saveNzdJpy(InstantSignal instantSignal){
		nzdJpyDao.saveNzdJpy(instantSignal);
	}
	
	public void saveNzdSgd(InstantSignal instantSignal){
		nzdSdgDao.saveNzdSgd(instantSignal);
	}
	
	public void saveNzdUsd(InstantSignal instantSignal){
		nzdUsdDao.saveNzdUsd(instantSignal);
	}
	
}
