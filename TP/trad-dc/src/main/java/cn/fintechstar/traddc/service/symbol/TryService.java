package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.TryJpyDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class TryService {

	@Autowired
	private TryJpyDao tryJpyDao;
	
	public void saveTryJpy(InstantSignal instantSignal){
		tryJpyDao.saveTryJpy(instantSignal);
	}
}
