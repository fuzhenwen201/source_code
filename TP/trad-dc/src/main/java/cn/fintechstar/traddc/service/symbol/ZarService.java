package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.ZarJpyDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class ZarService {

	@Autowired
	private ZarJpyDao zarJpyDao;
	
	public void saveZarJpy(InstantSignal instantSignal){
		zarJpyDao.saveZarJpy(instantSignal);
	}
}
