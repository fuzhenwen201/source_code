package cn.fintechstar.admin.service;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import com.google.gson.Gson;

import cn.fintechstar.admin.dao.ISecurityDao;
import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.security.SecuritySymbol;
import cn.fintechstar.admin.model.security.message.BasicSecurityAns;
import cn.fintechstar.admin.model.security.message.CreateOrUpdateSecurityReq;
import cn.fintechstar.admin.model.security.message.DeleteSecurityReq;
import cn.fintechstar.admin.model.security.message.GetSecuritiesAns;
import cn.fintechstar.admin.model.security.message.GetSecuritiesReq;

@Service
public class SecurityService {
	
	private static final Logger LOGGER = Logger.getLogger(SecurityService.class);
	
	private static final Gson GSON = new Gson();

	@Autowired
	private ISecurityDao securityDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	public BasicSecurityAns createOrUpdateSecurity(String body) {
		CreateOrUpdateSecurityReq createOrUpdateSecurityReq = GSON.fromJson(body, CreateOrUpdateSecurityReq.class);
		BasicSecurityAns createOrUpdateSecurityAns = getCreateOrUpdateSecurityAns(createOrUpdateSecurityReq);
		return createOrUpdateSecurityAns;
	}

	private BasicSecurityAns getCreateOrUpdateSecurityAns(CreateOrUpdateSecurityReq createOrUpdateSecurityReq) {
		BasicSecurityAns createOrUpdateSecurityAns = new BasicSecurityAns();
		createOrUpdateSecurityAns.setAdminId(createOrUpdateSecurityReq.getAdminId());
		createOrUpdateSecurityAns.setReqId(createOrUpdateSecurityReq.getReqId());
		createOrUpdateSecurityAns.setSession(createOrUpdateSecurityReq.getSession());
		createOrUpdateSecurityAns.setSgId(createOrUpdateSecurityReq.getSgId());
		try{
			Security security = getSecurityFromCreateOrUpdateSecurityReq(createOrUpdateSecurityReq);
			if(security.getSecurityId() != 0){
				securityDao.updateSecurity(security);
			}else{
				securityDao.saveSecurity(security);
			}
			createOrUpdateSecurityAns.setSecurityId(security.getSecurityId());
			createOrUpdateSecurityAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			createOrUpdateSecurityAns.setStatus(false);
			LOGGER.error("CreateOrUpdateSecurity request is : " + GSON.toJson(createOrUpdateSecurityReq));
		}
		return createOrUpdateSecurityAns;
	}

	private Security getSecurityFromCreateOrUpdateSecurityReq(CreateOrUpdateSecurityReq createOrUpdateSecurityReq) {
		Security security = new Security();
		security.setSecurityId(createOrUpdateSecurityReq.getSecurityId());
		security.setSecurityName(createOrUpdateSecurityReq.getSecurityName());
		security.setDescription(createOrUpdateSecurityReq.getDescription());
		security.setType(createOrUpdateSecurityReq.getType());
		security.setIsEnable(1);
		security.setIsDeleted(0);
		security.setCreatedTime(new Date());
		security.setModifiedTime(new Date());
		return security;
	}

	public GetSecuritiesAns getSecurities(String body) {
		GetSecuritiesReq getSecuritiesReq = GSON.fromJson(body, GetSecuritiesReq.class);
		GetSecuritiesAns getSecuritiesAns = getSecurities(getSecuritiesReq);
		return getSecuritiesAns;
	}

	private GetSecuritiesAns getSecurities(GetSecuritiesReq getSecuritiesReq) {
		GetSecuritiesAns getSecuritiesAns = new GetSecuritiesAns();
		getSecuritiesAns.setReqId(getSecuritiesReq.getReqId());
		getSecuritiesAns.setSession(getSecuritiesReq.getSession());
		getSecuritiesAns.setSgId(getSecuritiesReq.getSgId());
		getSecuritiesAns.setAdminId(getSecuritiesReq.getAdminId());
		try{
			List<Security> securities = securityDao.getSecurities();
			List<SecuritySymbol> securitySymbols = new ArrayList<>();
			for(Security security : securities){
				SecuritySymbol securitySymbol = new SecuritySymbol();
				List<String> symbols = symbolDao.getSymbolNamesBySecurityId(security.getSecurityId());
				securitySymbol.setSecurityId(security.getSecurityId());
				securitySymbol.setSecurityName(security.getSecurityName());
				securitySymbol.setType(security.getType());
				securitySymbol.setEnable(security.getIsEnable() == 1 ? true : false);
				securitySymbol.setDeleted(security.getIsDeleted() == 1 ? true : false);
				securitySymbol.setDescription(security.getDescription());
				securitySymbol.setModifiedTime(security.getModifiedTime());
				securitySymbol.setCreatedTime(security.getCreatedTime());
				if(!CollectionUtils.isEmpty(symbols)){
					securitySymbol.setSymbolList(getSymbolNames(symbols));
				}
				securitySymbols.add(securitySymbol);
			}
			getSecuritiesAns.setSecuritySymbols(securitySymbols);
			getSecuritiesAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			getSecuritiesAns.setStatus(false);
			LOGGER.error("Get all securities request is : " + GSON.toJson(getSecuritiesReq));
		}
		return getSecuritiesAns;
	}

	private String getSymbolNames(List<String> symbols) {
		StringBuilder sb = new StringBuilder();
		sb.append(symbols.size()).append(": ");
		for(String symbol : symbols){
			sb.append(symbol).append(",");
		}
		return sb.toString().substring(0, sb.length() -1);
	}

	public BasicSecurityAns deleteSecurityById(String body) {
		DeleteSecurityReq deleteSecurityReq = GSON.fromJson(body, DeleteSecurityReq.class);
		BasicSecurityAns deleteSecurityAns = getDeleteSecurityAns(deleteSecurityReq);
		return deleteSecurityAns;
	}

	private BasicSecurityAns getDeleteSecurityAns(DeleteSecurityReq deleteSecurityReq) {
		BasicSecurityAns deleteSecurityAns = new BasicSecurityAns();
		deleteSecurityAns.setAdminId(deleteSecurityReq.getAdminId());
		deleteSecurityAns.setReqId(deleteSecurityReq.getReqId());
		deleteSecurityAns.setSession(deleteSecurityReq.getSession());
		deleteSecurityAns.setSgId(deleteSecurityReq.getSgId());
		try {
			securityDao.deleteSecurityById(deleteSecurityReq.getSecurityId());
			deleteSecurityAns.setStatus(true);
		} catch (Exception e) {
			e.printStackTrace();
			deleteSecurityAns.setStatus(false);
			LOGGER.error("DeleteSecurity request is : " + GSON.toJson(deleteSecurityReq));
		}
		return deleteSecurityAns;
	}

}
