package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.HkdJpyDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class HkdService {

	@Autowired
	private HkdJpyDao hkdJpyDao;
	
	public void saveHkdJpy(InstantSignal instantSignal){
		hkdJpyDao.saveHkdJpy(instantSignal);
	}
}
