package cn.fintechstar.admin.service;


import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.model.symbol.message.BasicSymbolAns;
import cn.fintechstar.admin.model.symbol.message.CreateOrUpdateSymbolReq;
import cn.fintechstar.admin.model.symbol.message.GetSymbolByIdAns;
import cn.fintechstar.admin.model.symbol.message.GetSymbolsAns;
import cn.fintechstar.admin.model.symbol.message.GetSymbolsReq;
import cn.fintechstar.admin.service.symbol.BoSymbolService;
import cn.fintechstar.admin.service.symbol.SymbolCalculationService;
import cn.fintechstar.admin.service.symbol.SymbolCommonSerivce;
import cn.fintechstar.admin.service.symbol.SymbolFilterationService;
import cn.fintechstar.admin.service.symbol.SymbolSessionService;
import cn.fintechstar.admin.service.symbol.SymbolSwapService;

@Service
public class SymbolService {
	
	private static final Logger LOGGER = Logger.getLogger(SymbolService.class);
	
	private static final Gson GSON = new Gson();

	@Autowired
	private SymbolCommonSerivce symbolCommonSerivce;
	
	@Autowired
	private SymbolFilterationService symbolFilterationService;
	
	@Autowired
	private SymbolCalculationService symbolCalculationService;
	
	@Autowired
	private SymbolSwapService symbolSwapServcie;
	
	@Autowired
	private SymbolSessionService symbolSessionService;
	
	@Autowired
	private BoSymbolService boSymbolService;
	
	public BasicSymbolAns createOrUpdateSymbol(String body) {
		CreateOrUpdateSymbolReq createSymbolRequest = GSON.fromJson(body, CreateOrUpdateSymbolReq.class);
		BasicSymbolAns createSymbolAns = getCreateOrUpdateSymbolAns(createSymbolRequest);
		return createSymbolAns;
	}

	private BasicSymbolAns getCreateOrUpdateSymbolAns(CreateOrUpdateSymbolReq createOrUpdateSymbolReq) {
		BasicSymbolAns createOrUpdateSymbolAns = new BasicSymbolAns();
		createOrUpdateSymbolAns.setReqId(createOrUpdateSymbolReq.getReqId());
		createOrUpdateSymbolAns.setSession(createOrUpdateSymbolReq.getSession());
		createOrUpdateSymbolAns.setSgId(createOrUpdateSymbolReq.getSgId());
		createOrUpdateSymbolAns.setAdminId(createOrUpdateSymbolReq.getAdminId());
		try {
			if(createOrUpdateSymbolReq.getSymbolDetail().getSymbolId() == 0){
				symbolCommonSerivce.saveSymbol(createOrUpdateSymbolReq.getSymbolDetail());
				int symbolId = createOrUpdateSymbolReq.getSymbolDetail().getSymbolId();
				if(symbolId != 0){
					symbolFilterationService.saveSymbolFilteration(symbolId, createOrUpdateSymbolReq.getSymbolFilteration());
					symbolCalculationService.saveSymbolCalculation(symbolId, createOrUpdateSymbolReq.getSymbolCalculation());
					symbolSwapServcie.saveSymbolSwap(symbolId, createOrUpdateSymbolReq.getSymbolSwap());
					symbolSessionService.saveSymbolSession(symbolId, createOrUpdateSymbolReq.getSymbolSessions());
					boSymbolService.saveBoSymbol(symbolId, createOrUpdateSymbolReq.getBoSymbol());
				}else{
					throw new Exception("Error create symbol! Request is : " + GSON.toJson(createOrUpdateSymbolReq));
				}
			}else{
				symbolCommonSerivce.updateSymbol(createOrUpdateSymbolReq.getSymbolDetail());
				symbolFilterationService.updateSymbolFilteration(createOrUpdateSymbolReq.getSymbolFilteration());
				symbolCalculationService.updateSymbolCalculation(createOrUpdateSymbolReq.getSymbolCalculation());
				symbolSwapServcie.updateSymbolSwap(createOrUpdateSymbolReq.getSymbolSwap());
				symbolSessionService.updateSymbolSession(createOrUpdateSymbolReq.getSymbolSessions());
				boSymbolService.updateBoSymbol(createOrUpdateSymbolReq.getBoSymbol(), createOrUpdateSymbolReq.getSymbolId());
			}
			createOrUpdateSymbolAns.setSymbolId(createOrUpdateSymbolReq.getSymbolDetail().getSymbolId());
			createOrUpdateSymbolAns.setStatus(true);
		} catch (Exception e) {
			e.printStackTrace();
			createOrUpdateSymbolAns.setStatus(false);
			LOGGER.error("CreateOrUpdate symbol request is : " + GSON.toJson(createOrUpdateSymbolReq));
		}
		return createOrUpdateSymbolAns;
	}

	public GetSymbolsAns getSymbols(String body) {
		GetSymbolsReq getSymbolsReq = GSON.fromJson(body, GetSymbolsReq.class);
		GetSymbolsAns getSymbolsAns = getSymbolsAns(getSymbolsReq);
		return getSymbolsAns;
	}

	private GetSymbolsAns getSymbolsAns(GetSymbolsReq getSymbolsReq) {
		GetSymbolsAns getSymbolsAns = new GetSymbolsAns();
		getSymbolsAns.setReqId(getSymbolsReq.getReqId());
		getSymbolsAns.setSession(getSymbolsReq.getSession());
		getSymbolsAns.setSgId(getSymbolsReq.getSgId());
		getSymbolsAns.setAdminId(getSymbolsReq.getAdminId());
		try{
			getSymbolsAns.setSymbolDetails(symbolCommonSerivce.getSymbols());
			getSymbolsAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			getSymbolsAns.setStatus(false);
			LOGGER.error("Get symbols request is : " + GSON.toJson(getSymbolsReq));
		}
		return getSymbolsAns;
	}

	public GetSymbolByIdAns getSymbolById(String body) {
		GetSymbolsReq getSymbolsReq = GSON.fromJson(body, GetSymbolsReq.class); 
		GetSymbolByIdAns getSymbolByIdAns = getSymbolByIdAns(getSymbolsReq);
		return getSymbolByIdAns;
	}

	private GetSymbolByIdAns getSymbolByIdAns(GetSymbolsReq getSymbolsReq) {
		GetSymbolByIdAns getSymbolByIdAns = new GetSymbolByIdAns();
		int symbolId = getSymbolsReq.getSymbolId();
		getSymbolByIdAns.setReqId(getSymbolsReq.getReqId());
		getSymbolByIdAns.setSession(getSymbolsReq.getSession());
		getSymbolByIdAns.setSgId(getSymbolsReq.getSgId());
		getSymbolByIdAns.setAdminId(getSymbolsReq.getAdminId());
		getSymbolByIdAns.setSymbolId(symbolId);
		try{
			getSymbolByIdAns.setSymbolDetails(symbolCommonSerivce.getSymbolBySymbolId(symbolId));
			getSymbolByIdAns.setSymbolFilteration(symbolFilterationService.getSymbolFilterationBySymbolId(symbolId));
			getSymbolByIdAns.setSymbolCalculation(symbolCalculationService.getSymbolCalculationBySymbolId(symbolId));
			getSymbolByIdAns.setSymbolSwap(symbolSwapServcie.getSymbolSwapBySymbolId(symbolId));
			getSymbolByIdAns.setSymbolSessions(symbolSessionService.getSymbolSessionsBySymbolId(symbolId));
			getSymbolByIdAns.setBoSymbol(boSymbolService.getBoSymbolBySymbolId(symbolId));
			getSymbolByIdAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			getSymbolByIdAns.setStatus(false);
			LOGGER.error("Get symbol by symbol id request is : " + GSON.toJson(getSymbolsReq));
		}
		return getSymbolByIdAns;
	}

}
