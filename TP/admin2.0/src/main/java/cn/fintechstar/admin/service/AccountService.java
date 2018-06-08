package cn.fintechstar.admin.service;


import java.util.Date;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.constant.Tag;
import cn.fintechstar.admin.model.account.Account;
import cn.fintechstar.admin.model.account.Trader;
import cn.fintechstar.admin.model.account.message.BasicAccount;
import cn.fintechstar.admin.model.account.message.BasicAccountAns;
import cn.fintechstar.admin.model.account.message.GetAccountByIdAns;
import cn.fintechstar.admin.model.account.message.GetAccountReq;
import cn.fintechstar.admin.model.account.message.GetAccountsAns;
import cn.fintechstar.admin.service.account.AccountCommonService;
import cn.fintechstar.admin.service.account.TraderService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;
import cn.fintechstar.admin.util.PasswordGeneratorUtil;

@Service
public class AccountService {

	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(Logger.class);
	
	@Value("${rabbit.cm.exchange}")
	private String cmExchange;
	
	@Value("${rabbit.cm.routingKey}")
	private String cmRoutingKey;
	
	@Autowired
	private TraderService traderService;
	
	@Autowired
	private AccountCommonService accountCommonService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	public BasicAccountAns createOrUpdateAccount(String body) {
		BasicAccount createOrUpdateAccountReq = GSON.fromJson(body, BasicAccount.class);
		BasicAccountAns createOrUpdateAccountAns = getCreateOrUpdateAccountAns(createOrUpdateAccountReq);
		return createOrUpdateAccountAns;
	}

	private BasicAccountAns getCreateOrUpdateAccountAns(BasicAccount createOrUpdateAccountReq) {
		BasicAccountAns createOrUpdateAccountAns = new BasicAccountAns();
		createOrUpdateAccountAns.setReqId(createOrUpdateAccountReq.getReqId());
		createOrUpdateAccountAns.setSession(createOrUpdateAccountReq.getSession());
		createOrUpdateAccountAns.setSgId(createOrUpdateAccountReq.getSgId());
		createOrUpdateAccountAns.setAdminId(createOrUpdateAccountReq.getAdminId());
		createOrUpdateAccountAns.setClientId(createOrUpdateAccountReq.getClientId());
		try{
			if(createOrUpdateAccountReq.getBalance() == 0 && createOrUpdateAccountReq.getMargin() == 0 && createOrUpdateAccountReq.getFreeMargin() == 0 && 
					createOrUpdateAccountReq.getPrevBalance() == 0 && createOrUpdateAccountReq.getChangeDeposit() == 0 && createOrUpdateAccountReq.getCloseBalance() == 0){
				Trader trader = getTrader(createOrUpdateAccountReq);
				traderService.saveOrUpdateTrader(trader);
				Account account = getAccount(createOrUpdateAccountReq, trader.getClientId());
				accountCommonService.saveOrUpdateAccount(account);
				// SEND MSG to CM
				if(createOrUpdateAccountReq.getAccountId() != 0 && trader.getDirtyBit() == 1){
					createOrUpdateAccountReq.setIsAdmin(true);
					TNode sendMessageToCm = TNodeCodec.getTNode(Tag.SEND_TO_CM_WHEN_DIRTY_BIT, GSON.toJson(createOrUpdateAccountReq));
					rabbitTemplate.send(cmExchange, cmRoutingKey, new Message(TNodeCodec.encodeMessage(sendMessageToCm), new MessageProperties()));
				}
				createOrUpdateAccountAns.setClientId(trader.getClientId());
				createOrUpdateAccountAns.setAccountId(account.getAccountId());
				createOrUpdateAccountAns.setStatus(true);
			}else{
				createOrUpdateAccountAns.setStatus(false);
				createOrUpdateAccountAns.setErrorMessage("Please balance/amount related info everthing should be 0 amount !!");
			}
		}catch (Exception e) {
			e.printStackTrace();
			createOrUpdateAccountAns.setErrorMessage("Encounter an internal error!");
			createOrUpdateAccountAns.setStatus(false);
			LOGGER.error("CreateOrUpdateAccount request is : " + GSON.toJson(createOrUpdateAccountAns));
		}
		return createOrUpdateAccountAns;
	}

	private Account getAccount(BasicAccount createOrUpdateAccountReq, int clientId) {
		Account account = new Account();
		account.setAccountId(createOrUpdateAccountReq.getAccountId());
		account.setClientId(clientId);
		account.setGroupId(createOrUpdateAccountReq.getGroupId());
		account.setBalance(createOrUpdateAccountReq.getBalance());
		account.setPrevBalance(createOrUpdateAccountReq.getPrevBalance());
		account.setChangeDeposit(createOrUpdateAccountReq.getChangeDeposit());
		account.setMargin(createOrUpdateAccountReq.getMargin());
		account.setFreeMargin(createOrUpdateAccountReq.getFreeMargin());
		account.setCloseBalance(createOrUpdateAccountReq.getCloseBalance());
		account.setDirtyBit(createOrUpdateAccountReq.getDirtyBit());
		account.setBoAccount(createOrUpdateAccountReq.getBoAccount());
		account.setFxAccount(createOrUpdateAccountReq.getFxAccount());
		account.setModifiedTime(new Date());
		return account;
	}

	private Trader getTrader(BasicAccount createOrUpdateAccountReq) {
		Trader trader = new Trader();
		trader.setAddress(createOrUpdateAccountReq.getAddress());
		trader.setEmail(createOrUpdateAccountReq.getEmail());
		trader.setFirstName(createOrUpdateAccountReq.getFirstname());
		trader.setGroupId(createOrUpdateAccountReq.getGroupId());
		trader.setLastName(createOrUpdateAccountReq.getLastname());
		trader.setPassword(PasswordGeneratorUtil.doMd5Hashing(createOrUpdateAccountReq.getPassword()));
		trader.setCity(createOrUpdateAccountReq.getCity());
		trader.setCountry(createOrUpdateAccountReq.getCountry());
		trader.setState(createOrUpdateAccountReq.getState());
		trader.setPin(createOrUpdateAccountReq.getPin());
		trader.setIsEnable(createOrUpdateAccountReq.getIsEnable());
		//For future use
		trader.setTraderComment(createOrUpdateAccountReq.getTraderComment());
		trader.setStatus(createOrUpdateAccountReq.getStatus());
		trader.setLeadSource(createOrUpdateAccountReq.getLeadSource());
		trader.setInvestorPassword(createOrUpdateAccountReq.getInvestorPassword());
		trader.setPhone(createOrUpdateAccountReq.getPhone());
		trader.setIdNumber(createOrUpdateAccountReq.getIdNumber());
		trader.setPhonePassword(createOrUpdateAccountReq.getPhonePassword());
		trader.setLeverage(createOrUpdateAccountReq.getLeverage());
		trader.setTaxRate(createOrUpdateAccountReq.getTaxRate());
		trader.setAgentAccount(createOrUpdateAccountReq.getAgentAccount());
		trader.setIsAllowChangePassword(createOrUpdateAccountReq.getIsAllowChangePassword());
		trader.setIsEnableOTP(createOrUpdateAccountReq.getIsEnableOtp());
		trader.setIsReadOnly(createOrUpdateAccountReq.getIsReadonly());
		trader.setIsSendReport(createOrUpdateAccountReq.getIsSendReport());
		trader.setCreatedDate(new Date(System.currentTimeMillis()));
		trader.setUpdatedDate(new Date(System.currentTimeMillis()));
		return trader;
	}

	public GetAccountsAns getAccounts(String body) {
		GetAccountReq getAccountsReq = GSON.fromJson(body, GetAccountReq.class);
		GetAccountsAns getAccountsAns = getAccountsAns(getAccountsReq);
		return getAccountsAns;
	}

	private GetAccountsAns getAccountsAns(GetAccountReq getAccountsReq) {
		GetAccountsAns getAccountsAns = new GetAccountsAns();
		getAccountsAns.setReqId(getAccountsReq.getReqId());
		getAccountsAns.setSession(getAccountsReq.getSession());
		getAccountsAns.setSgId(getAccountsReq.getSgId());
		getAccountsAns.setAdminId(getAccountsReq.getAdminId());
		try{
			getAccountsAns.setAccounts(traderService.getTraders());
		}catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("GetAccounts request is : " + GSON.toJson(getAccountsReq));
			getAccountsAns.setStatus(true);
		}
		return getAccountsAns;
	}

	public GetAccountByIdAns getAccountById(String body) {
		GetAccountReq getAccountReq = GSON.fromJson(body, GetAccountReq.class);
		GetAccountByIdAns getAccountByIdAns = getAccountByIdAns(getAccountReq);
		return getAccountByIdAns;
	}

	private GetAccountByIdAns getAccountByIdAns(GetAccountReq getAccountReq) {
		GetAccountByIdAns getAccountByIdAns = new GetAccountByIdAns();
		getAccountByIdAns.setReqId(getAccountReq.getReqId());
		getAccountByIdAns.setSession(getAccountReq.getSession());
		getAccountByIdAns.setSgId(getAccountReq.getSgId());
		getAccountByIdAns.setAdminId(getAccountReq.getAdminId());
		try{
			getAccountByIdAns.setTrader(traderService.getTraderById(getAccountReq.getClientId()));
			getAccountByIdAns.setStatus(true);
		}catch (Exception e) {
			e.printStackTrace();
			LOGGER.error("GetAccountById request is : " + GSON.toJson(getAccountReq));
			getAccountByIdAns.setStatus(false);
		}
		return getAccountByIdAns;
	}

}
