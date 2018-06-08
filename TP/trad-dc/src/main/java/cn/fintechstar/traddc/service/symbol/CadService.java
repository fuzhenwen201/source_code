package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.cad.CadChfDao;
import cn.fintechstar.traddc.dao.instant.cad.CadHkdDao;
import cn.fintechstar.traddc.dao.instant.cad.CadJpyDao;
import cn.fintechstar.traddc.dao.instant.cad.CadSgdDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class CadService {

	@Autowired
	private CadChfDao cadChfDao;
	
	@Autowired
	private CadHkdDao cadHkdDao;
	
	@Autowired
	private CadJpyDao cadJpyDao;
	
	@Autowired
	private CadSgdDao cadSgdDao;
	
	
	public void saveCadChf(InstantSignal instantSignal){
		cadChfDao.saveCadChf(instantSignal);
	}
	
	public void saveCadHkd(InstantSignal instantSignal) {
		cadHkdDao.saveCadHkd(instantSignal);
	}
	
	public void saveCadJpy(InstantSignal instantSignal){
		cadJpyDao.saveCadJpy(instantSignal);
	}
	
	public void saveCadSgd(InstantSignal instantSignal){
		cadSgdDao.saveCadSgd(instantSignal);
	}
	
}
