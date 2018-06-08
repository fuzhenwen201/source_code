package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.eur.EurAudDao;
import cn.fintechstar.traddc.dao.instant.eur.EurCadDao;
import cn.fintechstar.traddc.dao.instant.eur.EurChfDao;
import cn.fintechstar.traddc.dao.instant.eur.EurCzkDao;
import cn.fintechstar.traddc.dao.instant.eur.EurDkkDao;
import cn.fintechstar.traddc.dao.instant.eur.EurGbpDao;
import cn.fintechstar.traddc.dao.instant.eur.EurHkdDao;
import cn.fintechstar.traddc.dao.instant.eur.EurHufDao;
import cn.fintechstar.traddc.dao.instant.eur.EurJpyDao;
import cn.fintechstar.traddc.dao.instant.eur.EurNokDao;
import cn.fintechstar.traddc.dao.instant.eur.EurNzdDao;
import cn.fintechstar.traddc.dao.instant.eur.EurPlnDao;
import cn.fintechstar.traddc.dao.instant.eur.EurSekDao;
import cn.fintechstar.traddc.dao.instant.eur.EurSgdDao;
import cn.fintechstar.traddc.dao.instant.eur.EurTryDao;
import cn.fintechstar.traddc.dao.instant.eur.EurUsdDao;
import cn.fintechstar.traddc.dao.instant.eur.EurZarDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class EurService {

	@Autowired
	private EurAudDao eurAudDao;
	
	@Autowired
	private EurCadDao eurCadDao;
	
	@Autowired
	private EurChfDao eurChfDao;
	
	@Autowired
	private EurCzkDao eurCzkDao;
	
	@Autowired
	private EurDkkDao eurDkkDao;
	
	@Autowired
	private EurGbpDao eurGbpDao;
	
	@Autowired
	private EurHkdDao eurHkdDao;
	
	@Autowired
	private EurHufDao eurHufDao;
	
	@Autowired
	private EurJpyDao eurJpyDao;
	
	@Autowired
	private EurNokDao eurNokDao;
	
	@Autowired
	private EurNzdDao eurNzdDao;
	
	@Autowired
	private EurPlnDao eurPlnDao;
	
	@Autowired
	private EurSekDao eurSekDao;
	
	@Autowired
	private EurSgdDao eurSgdDao;
	
	@Autowired
	private EurTryDao eurTryDao;
	
	@Autowired
	private EurUsdDao eurUsdDao;
	
	@Autowired
	private EurZarDao eurZarDao;
	
	
	public void saveEurAud(InstantSignal instantSignal){
		eurAudDao.saveEurAud(instantSignal);
	}
	
	public void saveEurCad(InstantSignal instantSignal){
		eurCadDao.saveEurCad(instantSignal);
	}
	
	public void saveEurChf(InstantSignal instantSignal){
		eurChfDao.saveEurChf(instantSignal);
	}
	
	public void saveEurCzk(InstantSignal instantSignal){
		eurCzkDao.saveEurCzk(instantSignal);
	}
	
	public void saveEurDkk(InstantSignal instantSignal){
		eurDkkDao.saveEurDkk(instantSignal);
	}
	
	public void saveEurGbp(InstantSignal instantSignal){
		eurGbpDao.saveEurGbp(instantSignal);
	}
	
	public void saveEurHkd(InstantSignal instantSignal){
		eurHkdDao.saveEurHkd(instantSignal);
	}
	
	public void saveEurHuf(InstantSignal instantSignal){
		eurHufDao.saveEurHuf(instantSignal);
	}
	
	public void saveEurJpy(InstantSignal instantSignal){
		eurJpyDao.saveEurJpy(instantSignal);
	}
	
	public void saveEurNok(InstantSignal instantSignal){
		eurNokDao.saveEurNok(instantSignal);
	}
	
	public void saveEurNzd(InstantSignal instantSignal){
		eurNzdDao.saveEurNzd(instantSignal);
	}
	
	public void saveEurPln(InstantSignal instantSignal){
		eurPlnDao.saveEurPln(instantSignal);
	}
	
	public void saveEurSek(InstantSignal instantSignal){
		eurSekDao.saveEurSek(instantSignal);
	}
	
	public void saveEurSgd(InstantSignal instantSignal){
		eurSgdDao.saveEurSgd(instantSignal);
	}
	
	public void saveEurTry(InstantSignal instantSignal){
		eurTryDao.saveEurTry(instantSignal);
	}
	
	public void saveEurUsd(InstantSignal instantSignal){
		eurUsdDao.saveEurUsd(instantSignal);
	}
	
	public void saveEurZar(InstantSignal instantSignal){
		eurZarDao.saveEurZar(instantSignal);
	}
}
