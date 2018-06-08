package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.sgd.SgdChfDao;
import cn.fintechstar.traddc.dao.instant.sgd.SgdHkdDao;
import cn.fintechstar.traddc.dao.instant.sgd.SgdJpyDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class SgdService {

	@Autowired
	private SgdChfDao sgdChfDao;
	
	@Autowired
	private SgdHkdDao sgdHkdDao;
	
	@Autowired
	private SgdJpyDao sgdJpyDao;
	
	
	public void saveSgdChf(InstantSignal instantSignal){
		sgdChfDao.saveSgdChf(instantSignal);
	}
	
	public void saveSgdHkd(InstantSignal instantSignal){
		sgdHkdDao.saveSgdHkd(instantSignal);
	}
	
	public void saveSgdJpy(InstantSignal instantSignal){
		sgdJpyDao.saveSgdJpy(instantSignal);
	}
	
}
