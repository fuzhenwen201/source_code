package cn.fintechstar.admin.service;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.dao.ISecurityDao;
import cn.fintechstar.admin.dao.group.IGroupSecurityDao;
import cn.fintechstar.admin.dao.group.ITimePeriodDao;
import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.listener.SynchDataListener;
import cn.fintechstar.admin.model.group.TimePeriod;
import cn.fintechstar.admin.model.symbol.SymbolVo;
import cn.fintechstar.admin.model.synch.GroupOfSqlite;
import cn.fintechstar.admin.model.synch.SecurityOfGroupBO;
import cn.fintechstar.admin.model.synch.SecurityOfSqlite;
import cn.fintechstar.admin.model.synch.SecurityOfSymbol;
import cn.fintechstar.admin.model.synch.SymbolOfSqlite;
import cn.fintechstar.admin.model.synch.message.SynchDataAns;
import cn.fintechstar.admin.model.synch.message.SynchDataReq;
import cn.fintechstar.admin.synch.dao.IBoGroupDao;
import cn.fintechstar.admin.synch.dao.IBoSecurityDao;
import cn.fintechstar.admin.synch.dao.IFxGroupDao;
import cn.fintechstar.admin.synch.dao.IFxSecurityDao;
import cn.fintechstar.admin.synch.dao.ISymbolOfSqliteDao;

@Service
public class SynchDataService {

	private static final Logger LOGGER = Logger.getLogger(SynchDataListener.class);
	
	private static final Gson GSON = new Gson();
	
	@Value("${default.timePeriod}")
	private String defaultTimePeriod;
	
	@Autowired
	private IBoGroupDao boGroupDao;
	
	@Autowired
	private IFxGroupDao fxGroupDao;	
	
	@Autowired
	private IBoSecurityDao boSecurityDao;
	
	@Autowired
	private IFxSecurityDao fxSecurityDao;
	
	@Autowired
	private ISymbolOfSqliteDao symbolDaoOfSqlite;
	
	@Autowired
	private IGroupSecurityDao groupSecurityDao;
	
	@Autowired
	private ITimePeriodDao timePeriodDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	public SynchDataAns getSynchDataAns(String body) {
		SynchDataReq synchDataReq = GSON.fromJson(body, SynchDataReq.class);
		SynchDataAns synchDataAns = getSynchDataAns(synchDataReq);
		return synchDataAns;
	}

	private SynchDataAns getSynchDataAns(SynchDataReq synchDataReq) {
		SynchDataAns synchDataAns = new SynchDataAns();
		synchDataAns.setReqId(synchDataReq.getReqId());
		synchDataAns.setSession(synchDataReq.getSession());
		synchDataAns.setSgId(synchDataReq.getSgId());
		synchDataAns.setAdminId(synchDataReq.getAdminId());
		try{
			clearSqliteDatabase();
			addDataFromMysqlToSqlite();
		}catch (Exception e) {
			e.printStackTrace();
			synchDataAns.setStatus(false);
			LOGGER.error("Synchronize data from mysql to slite request is : " + GSON.toJson(synchDataReq));
		}
		return synchDataAns;
	}

	private void addDataFromMysqlToSqlite() {
		boGroupDao.clearTable();
		fxGroupDao.clearTable();
		boSecurityDao.clearTable();
		fxSecurityDao.clearTable();
		symbolDaoOfSqlite.clearTable();
	}

	private void clearSqliteDatabase() {
		List<SecurityOfGroupBO> boSecurityOfGroupBOs = groupSecurityDao.getBoGroups();
		if(!boSecurityOfGroupBOs.isEmpty()){
			List<GroupOfSqlite> boGroups = getGroups(boSecurityOfGroupBOs);
			boGroupDao.saveBoGroups(boGroups);
		}
		List<SecurityOfGroupBO> fxSecurityOfGroupBOs = groupSecurityDao.getFxGroups();
		if(!fxSecurityOfGroupBOs.isEmpty()){
			List<GroupOfSqlite> fxGroups = getGroups(fxSecurityOfGroupBOs);
			fxGroupDao.saveFxGroups(fxGroups);
		}
		List<SecurityOfSymbol> boSecurityOfSymbolBos = securityDao.getBoSecuritiesOfGroup();
		if(!boSecurityOfSymbolBos.isEmpty()){
			List<SecurityOfSqlite> boSecurities = getSecurities(boSecurityOfSymbolBos);
			boSecurityDao.saveBoSecurities(boSecurities);
		}
		List<SecurityOfSymbol> fxSecurityOfSymbolBos = securityDao.getFxSecuritiesOfGroup();
		if(!fxSecurityOfSymbolBos.isEmpty()){
			List<SecurityOfSqlite> fxSecurities = getSecurities(fxSecurityOfSymbolBos);
			fxSecurityDao.saveFxSecurities(fxSecurities);
		}
		List<SymbolVo> allSymbols = symbolDao.getSymbols();
		if(!allSymbols.isEmpty()){
			List<SymbolOfSqlite> symbols = getFxSymbols(allSymbols);
			symbolDaoOfSqlite.saveSymbols(symbols);
		}
	}
	

	private List<SymbolOfSqlite> getFxSymbols(List<SymbolVo> allSymbols) {
		List<SymbolOfSqlite> symbolOfSqlites = new ArrayList<>();
		for(SymbolVo symbol : allSymbols){
			if(!symbol.getSymbolName().toLowerCase().endsWith("bo")){
				SymbolOfSqlite symbolOfSqlite = new SymbolOfSqlite();
				symbolOfSqlite.setSymbol(symbol.getSymbolName());
				symbolOfSqlite.setSymbolSpread(Integer.parseInt(symbol.getSpread()));
				symbolOfSqlites.add(symbolOfSqlite);
			}
		}
		return symbolOfSqlites;
	}

	private List<SecurityOfSqlite> getSecurities(List<SecurityOfSymbol> securityOfSymbolBos) {
		List<SecurityOfSqlite> securityOfSqlites = new ArrayList<>();
		for(int i = 0; i < securityOfSymbolBos.size(); i ++){
			SecurityOfSymbol securityOfSymbolBO = securityOfSymbolBos.get(i);
			if(securityOfSqlites.isEmpty()){
				SecurityOfSqlite securityOfSqlite = getSecurityOfSqlite(securityOfSymbolBO);
				securityOfSqlites.add(securityOfSqlite);
			}else{
				SecurityOfSqlite securityOfSqlite = securityOfSqlites.get(securityOfSqlites.size() -1);
				if((securityOfSymbolBO.getSecurityId() == securityOfSqlite.getSecurityId())){
					securityOfSqlite.setSymbolList(securityOfSqlite.getSymbolList() + securityOfSymbolBO.getSymbolName() + "_" + securityOfSymbolBO.getDigits() + ";" );
				}else{
					securityOfSqlites.add(getSecurityOfSqlite(securityOfSymbolBO));
				}
			}
		}
		return securityOfSqlites;
	}

	private List<GroupOfSqlite> getGroups(List<SecurityOfGroupBO> securityOfGroupBOs) {
		List<GroupOfSqlite> groups = new ArrayList<>();
		for(SecurityOfGroupBO securityOfGroupBO : securityOfGroupBOs){
			GroupOfSqlite group = new GroupOfSqlite();
			group.setGroupId(securityOfGroupBO.getGroupId());
			group.setGroupName(securityOfGroupBO.getGroupName());
			group.setSecurityId(securityOfGroupBO.getSecurityId());
			group.setSecuritySpread(securityOfGroupBO.getSpread());
			group.setMaximumAmounts(securityOfGroupBO.getMaximumBoAmount());
			group.setMinimumAmounts(securityOfGroupBO.getMinimumBoAmount());
			group.setProfit(securityOfGroupBO.getPayout());
			group.setModifyTime(securityOfGroupBO.getModifyTime());
			group.setTimePeriod(getTimePeriod(securityOfGroupBO.getGroupId()));
			groups.add(group);
		}
		return groups;
	}

	private String getTimePeriod(int groupId) {
		StringBuilder sb = new StringBuilder();
		List<TimePeriod> timePeriods = timePeriodDao.getTimePeriodsByGroupId(groupId);
		for(TimePeriod timePeriod : timePeriods){
			sb.append(timePeriod.getPeriod()).append(";");
		}
		return sb.toString().length() == 0 ? defaultTimePeriod : sb.toString().substring(0, sb.toString().length()- 1);
	}


	private SecurityOfSqlite getSecurityOfSqlite(SecurityOfSymbol securityOfSymbolBO) {
		SecurityOfSqlite securityOfSqlite = new SecurityOfSqlite();
		securityOfSqlite.setSecurityId(securityOfSymbolBO.getSecurityId());
		securityOfSqlite.setSecurityName(securityOfSymbolBO.getSecurityName());
		securityOfSqlite.setModifyTime(securityOfSymbolBO.getModifyTime());
		securityOfSqlite.setDescription(securityOfSymbolBO.getDescription());
		securityOfSqlite.setSymbolList(securityOfSymbolBO.getSymbolName() + "_" + securityOfSymbolBO.getDigits() + ";");
		return securityOfSqlite;
	}

}
