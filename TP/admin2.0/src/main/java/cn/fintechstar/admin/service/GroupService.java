package cn.fintechstar.admin.service;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.model.group.message.BasicGroupAns;
import cn.fintechstar.admin.model.group.message.BasicGroupReq;
import cn.fintechstar.admin.model.group.message.GetGroupsAns;
import cn.fintechstar.admin.model.group.message.GroupDetailsMessage;
import cn.fintechstar.admin.service.group.GroupArchieveService;
import cn.fintechstar.admin.service.group.GroupCommonService;
import cn.fintechstar.admin.service.group.GroupMarginService;
import cn.fintechstar.admin.service.group.GroupReportService;
import cn.fintechstar.admin.service.group.GroupRightService;
import cn.fintechstar.admin.service.group.GroupSecurityService;
import cn.fintechstar.admin.service.group.GroupSymbolService;
import cn.fintechstar.admin.service.group.TradingConditionService;

@Service
public class GroupService {

	@Autowired
	private GroupCommonService groupCommonService;
	
	@Autowired
	private GroupRightService groupRightService;
	
	@Autowired
	private GroupArchieveService groupArchieveService;
	
	@Autowired
	private GroupMarginService groupMarginService;
	
	@Autowired
	private GroupReportService groupReportService;
	
	@Autowired
	private GroupSecurityService groupSecurityService;
	
	@Autowired
	private GroupSymbolService groupSymbolService;
	
	@Autowired
	private TradingConditionService tradingConditionService;
	
	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(GroupService.class);
	
	public BasicGroupAns createOrUpdateGroup(String body) {
		GroupDetailsMessage createOrUpdateGroupReq = GSON.fromJson(body, GroupDetailsMessage.class);
		BasicGroupAns createOrUpdateGroupAns = getCreateOrUpdateGroupAns(createOrUpdateGroupReq);
		return createOrUpdateGroupAns;
	}

	private BasicGroupAns getCreateOrUpdateGroupAns(GroupDetailsMessage createOrUpdateGroupReq) {
		BasicGroupAns createOrUpdateGroupAns = new BasicGroupAns();
		createOrUpdateGroupAns.setReqId(createOrUpdateGroupReq.getReqId());
		createOrUpdateGroupAns.setSession(createOrUpdateGroupReq.getSession());
		createOrUpdateGroupAns.setSgId(createOrUpdateGroupReq.getSgId());
		createOrUpdateGroupAns.setAdminId(createOrUpdateGroupReq.getAdminId());
		try {
			if(createOrUpdateGroupReq.getGroupId() == 0){
				groupCommonService.saveGroup(createOrUpdateGroupReq.getGroupCommon());
				int groupId = createOrUpdateGroupReq.getGroupCommon().getGroupId();
				if(groupId != 0){
					groupRightService.saveGroupRight(groupId, createOrUpdateGroupReq.getGroupRight());
					groupArchieveService.saveGroupArchieve(groupId, createOrUpdateGroupReq.getGroupArchieve());
					groupMarginService.saveGroupMargin(groupId, createOrUpdateGroupReq.getGroupMargin());
					groupReportService.saveGroupReport(groupId, createOrUpdateGroupReq.getGroupReport());
					groupSecurityService.saveGroupSecurities(groupId, createOrUpdateGroupReq.getGroupSecurities());
					groupSymbolService.saveGroupSymbols(groupId, createOrUpdateGroupReq.getGroupSymbols());
					tradingConditionService.saveTradingCondition(groupId, createOrUpdateGroupReq.getTradCondition());
				}else{
					throw new Exception("Error create group! Request is : " + GSON.toJson(createOrUpdateGroupReq));
				}
			}else{
				groupCommonService.updateGroup(createOrUpdateGroupReq.getGroupCommon());
				groupRightService.updateGroupRight(createOrUpdateGroupReq.getGroupRight());
				groupArchieveService.updateGroupArchieve(createOrUpdateGroupReq.getGroupArchieve());
				groupMarginService.updateGroupMargin(createOrUpdateGroupReq.getGroupMargin());
				groupReportService.updateGroupReport(createOrUpdateGroupReq.getGroupReport());
				groupSecurityService.updateGroupSecurities(createOrUpdateGroupReq.getGroupId(), createOrUpdateGroupReq.getGroupSecurities());
				groupSymbolService.updateGroupSymbols(createOrUpdateGroupReq.getGroupId(), createOrUpdateGroupReq.getGroupSymbols());
				tradingConditionService.updateTradingCondition(createOrUpdateGroupReq.getGroupId(), createOrUpdateGroupReq.getTradCondition());
			}
			createOrUpdateGroupAns.setGroupId(createOrUpdateGroupReq.getGroupCommon().getGroupId());
			createOrUpdateGroupAns.setStatus(true);
		} catch (Exception e) {
			e.printStackTrace();
			createOrUpdateGroupAns.setStatus(false);
			LOGGER.error("CreateOrUpdateGroup request is : " + GSON.toJson(createOrUpdateGroupReq));
		}
		return createOrUpdateGroupAns;
	}

	public GroupDetailsMessage getGroupById(String body) {
		BasicGroupReq getGroupByIdReq = GSON.fromJson(body, BasicGroupReq.class);
		GroupDetailsMessage getGroupByIdAns = getGroupByIdAns(getGroupByIdReq);
		return getGroupByIdAns;
	}

	private GroupDetailsMessage getGroupByIdAns(BasicGroupReq getGroupByIdReq) {
		GroupDetailsMessage getGroupByIdAns = new GroupDetailsMessage();
		int groupId = getGroupByIdReq.getGroupId();
		getGroupByIdAns.setReqId(getGroupByIdReq.getReqId());
		getGroupByIdAns.setSession(getGroupByIdReq.getSession());
		getGroupByIdAns.setSgId(getGroupByIdReq.getSgId());
		getGroupByIdAns.setAdminId(getGroupByIdReq.getAdminId());
		getGroupByIdAns.setGroupId(groupId);
		try{
			getGroupByIdAns.setGroupCommon(groupCommonService.getGroupCommonByGroupId(groupId));
			getGroupByIdAns.setGroupRight(groupRightService.getGroupRightByGroupId(groupId));
			getGroupByIdAns.setGroupArchieve(groupArchieveService.getGroupArchieveByGroupId(groupId));
			getGroupByIdAns.setGroupMargin(groupMarginService.getGroupMarginByGroupId(groupId));
			getGroupByIdAns.setGroupSecurities(groupSecurityService.getGroupSecuritiesByGroupId(groupId));
			getGroupByIdAns.setGroupSymbols(groupSymbolService.getGroupSymbolsByGroupId(groupId));
			getGroupByIdAns.setTradCondition(tradingConditionService.getTradingConditionByGroupId(groupId));
			getGroupByIdAns.setGroupReport(groupReportService.getGroupReportByGroupId(groupId));
			getGroupByIdAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			getGroupByIdAns.setStatus(false);
			LOGGER.error("GetGroupById request is : " + GSON.toJson(getGroupByIdReq));
		}
		return getGroupByIdAns;
	}

	public GetGroupsAns getGroups(String body) {
		BasicGroupReq getGroupsReq = GSON.fromJson(body, BasicGroupReq.class);
		GetGroupsAns getGroupsAns = getGroupsAns(getGroupsReq);
		return getGroupsAns;
	}

	private GetGroupsAns getGroupsAns(BasicGroupReq getGroupsReq) {
		GetGroupsAns getGroupsAns = new GetGroupsAns();
		getGroupsAns.setReqId(getGroupsReq.getReqId());
		getGroupsAns.setSession(getGroupsReq.getSession());
		getGroupsAns.setSgId(getGroupsReq.getSgId());
		getGroupsAns.setAdminId(getGroupsReq.getAdminId());
		try{
			getGroupsAns.setGroupDetails(groupCommonService.getGroups());
			getGroupsAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			getGroupsAns.setStatus(false);
			LOGGER.error("GetGroups request is : " + GSON.toJson(getGroupsReq));
		}
		return getGroupsAns;
	}

}
