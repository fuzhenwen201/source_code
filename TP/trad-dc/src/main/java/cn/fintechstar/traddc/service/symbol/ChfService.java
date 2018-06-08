package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.chf.ChfHkdDao;
import cn.fintechstar.traddc.dao.instant.chf.ChfJpyDao;
import cn.fintechstar.traddc.dao.instant.chf.ChfZarDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class ChfService {

	@Autowired
	private ChfHkdDao chfHkdDao;
	
	@Autowired
	private ChfJpyDao chfJpyDao;
	
	@Autowired
	private ChfZarDao chfZarDao;
	
	
	public void saveChfHkd(InstantSignal instantSignal){
		chfHkdDao.saveChfHkd(instantSignal);
	}
	
	public void saveChfJpy(InstantSignal instantSignal){
		chfJpyDao.saveChfJpy(instantSignal);
	}
	
	public void saveChfZar(InstantSignal instantSignal){
		chfZarDao.saveChfZar(instantSignal);
	}
	
}
