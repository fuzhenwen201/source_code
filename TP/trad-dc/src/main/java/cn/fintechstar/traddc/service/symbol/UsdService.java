package cn.fintechstar.traddc.service.symbol;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.usd.UsdCadDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdChfDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdCnhDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdCzkDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdDkkDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdHkdDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdHufDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdInrDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdJpyDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdMxnDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdNokDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdPlnDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdSarDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdSekDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdSgdDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdThbDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdTryDao;
import cn.fintechstar.traddc.dao.instant.usd.UsdZarDao;
import cn.fintechstar.traddc.model.InstantSignal;

@Service
public class UsdService {

	@Autowired
	private UsdCadDao usdCadDao;
	
	@Autowired
	private UsdChfDao usdChfDao;
	
	@Autowired
	private UsdCnhDao usdCnhDao;
	
	@Autowired
	private UsdCzkDao usdCzkDao;
	
	@Autowired
	private UsdDkkDao usdDkkDao;
	
	@Autowired
	private UsdHkdDao usdHkdDao;
	
	@Autowired
	private UsdHufDao usdHufDao;
	
	@Autowired
	private UsdInrDao usdInrDao;
	
	@Autowired
	private UsdJpyDao usdJpyDao;
	
	@Autowired
	private UsdMxnDao usdMxnDao;
	
	@Autowired
	private UsdNokDao usdNokDao;
	
	@Autowired
	private UsdPlnDao usdPlnDao;
	
	@Autowired
	private UsdSarDao usdSarDao;
	
	@Autowired
	private UsdSekDao usdSekDao;
	
	@Autowired
	private UsdSgdDao usdSgdDao;
	
	@Autowired
	private UsdThbDao usdThbDao;
	
	@Autowired
	private UsdTryDao usdTryDao;
	
	@Autowired
	private UsdZarDao usdZarDao;
	
	
	public void saveUsdCad(InstantSignal instantSignal){
		usdCadDao.saveUsdCad(instantSignal);
	}
	
	public void saveUsdChf(InstantSignal instantSignal){
		usdChfDao.saveUsdChf(instantSignal);
	}
	
	public void saveUsdCnh(InstantSignal instantSignal){
		usdCnhDao.saveUsdCnh(instantSignal);
	}
	
	public void saveUsdCzk(InstantSignal instantSignal){
		usdCzkDao.saveUsdCzk(instantSignal);
	}
	
	public void saveUsdDkk(InstantSignal instantSignal){
		usdDkkDao.saveUsdDkk(instantSignal);
	}
	
	public void saveUsdHkd(InstantSignal instantSignal){
		usdHkdDao.saveUsdHkd(instantSignal);
	}
	
	public void saveUsdHuf(InstantSignal instantSignal){
		usdHufDao.saveUsdHuf(instantSignal);
	}
	
	public void saveUsdInr(InstantSignal instantSignal){
		usdInrDao.saveUsdInr(instantSignal);
	}
	
	public void saveUsdJpy(InstantSignal instantSignal){
		usdJpyDao.saveUsdJpy(instantSignal);
	}
	
	public void saveUsdMxn(InstantSignal instantSignal){
		usdMxnDao.saveUsdMxn(instantSignal);
	}
	
	public void saveUsdNok(InstantSignal instantSignal){
		usdNokDao.saveUsdNok(instantSignal);
	}
	
	public void saveUsdPln(InstantSignal instantSignal){
		usdPlnDao.saveUsdPln(instantSignal);
	}
	
	public void saveUsdSar(InstantSignal instantSignal){
		usdSarDao.saveUsdSar(instantSignal);
	}
	
	public void saveUsdSek(InstantSignal instantSignal){
		usdSekDao.saveUsdSek(instantSignal);
	}
	
	public void saveUsdSgdDao(InstantSignal instantSignal){
		usdSgdDao.saveUsdSgd(instantSignal);
	}
	
	public void saveUsdThb(InstantSignal instantSignal){
		usdThbDao.saveUsdThb(instantSignal);
	}
	
	public void saveUsdTry(InstantSignal instantSignal){
		usdTryDao.saveUsdTry(instantSignal);
	}
	
	public void saveUsdZar(InstantSignal instantSignal){
		usdZarDao.saveUsdZar(instantSignal);
	}
}
