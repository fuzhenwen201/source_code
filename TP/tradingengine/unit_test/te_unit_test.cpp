

#include "gtest/gtest.h"
#include "tnodeconfig_v0.h"
#include "tradingengine.h"
#include "tnode.h"

using namespace snetwork_tradingengine;
static snetwork_xservice_tnode::TNodeConfigV0 cfg;


TEST(tradingengineTest, TradingBo_SetPayRatio) {
	TradingBO Bo;
	EXPECT_EQ(UNIVERSAL_TRADING_NO_INIT_ERR, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)2,80));

	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, Bo.Init());
	EXPECT_EQ(UNIVERSAL_TRADING_REPEAT_INIT_ERR, Bo.Init());

	EXPECT_EQ(UNIVERSAL_TRADING_SYMBOL_ERR, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)72,80));
	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)71,80));
	EXPECT_EQ(UNIVERSAL_TRADING_SYMBOL_ERR, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)73,80));
	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)0,80));
	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)1,80));	

	EXPECT_EQ(UNIVERSAL_TRADING_CONFIG_PARAMETER_ERR, Bo.SetPayRatio((snetwork_signalengine_marketdata::MarketSymbol)10,101));	
}

TEST(tradingengineTest, TradingBo_OrderConsumer_EventHandle) {
	TradingBO Bo;
	snetwork_xservice_tnode::TNode tnode;
	uint32_t res;

	tnode.Config(cfg);
	tnode.Bind(1);
	tnode.Run();

	char event[400] = {};
	const std::string instance_id("1");
	const char *str = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf0\",\"groupid\":1,\"clientid\":75,\"assetid\":0,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"sgid\":1}";
    strcpy(event, str);
	snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);
    TradingBO::OrderConsumer *order_consumer = new TradingBO::OrderConsumer(instance_id, channel, "TRADINGENGIN");
	res = order_consumer->EventHandle(0x11, event, 0);
	EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

	res = order_consumer->EventHandle(0x10, event, 20);
	EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

	res = order_consumer->EventHandle(0x11, event, 0);
	EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

	res = order_consumer->EventHandle(0x11, event, 183);
	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, res);

	const char *event1 = "{\"groupid\":1,\"clientid\":75,\"assetid\":0,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"sgid\":1}";
	memset(event, 0, 400 * sizeof(char));
    strcpy(event, event1);
	res = order_consumer->EventHandle(0x11, event,140);
	EXPECT_EQ(UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR, res);

	const char *event2 = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf0000\",\"groupid\":1,\"clientid\":75,\"assetid\":0,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"sgid\":1}";
	memset(event, 0, 400 * sizeof(char));
	strcpy(event, event2);
	res = order_consumer->EventHandle(0x11, event,185);
	EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, res);
}

TEST(tradingengineTest, TradingBo_ValidateConsumer_EventHandle0) {
    TradingBO Bo;
    snetwork_xservice_tnode::TNode tnode;

    uint32_t res;

    tnode.Config(cfg);
    tnode.Bind(1);
    tnode.Run();

    const std::string id("1");
    snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);
    TradingBO::ValidateConsumer *validate_consumer = new TradingBO::ValidateConsumer(id, channel);

    char event[400] = {};
    const char *str = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf0\",\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"groupid\":1,\"clientid\":75,\"accountid\":0,\"boaccountid\":0,\"assetid\":80,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sgid\":1,\"orderstatus\":0,\"margin\":100,\"freemargin\":99880,\"profit\":-20,\"status\":1,\"prevbalance\":0,\"changed\":100000,\"closebalance\":0}";

    strcpy(event, str);

    //assetid >=72
    res = validate_consumer->EventHandle(VALIDATE, event, 335);
    EXPECT_EQ(UNIVERSAL_TRADING_SYMBOL_ERR, res);

    const char *str1 = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf0\",\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"groupid\":1,\"clientid\":75,\"accountid\":0,\"boaccountid\":0,\"assetid\":0,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sgid\":1,\"orderstatus\":0,\"margin\":100,\"freemargin\":99880,\"profit\":-20,\"status\":1,\"prevbalance\":0,\"changed\":100000,\"closebalance\":0}";
    memset(event, 0, 400 * sizeof(char));
    strcpy(event, str1);
    res = validate_consumer->EventHandle(VALIDATE, event, 335);
    EXPECT_EQ(UNIVERSAL_TRADING_MARKET_DATA_INVALID_ERR, res);
}

TEST(tradingengineTest, TradingBo_ValidateConsumer_EventHandle1) {
    TradingBO Bo;
    snetwork_xservice_tnode::TNode tnode;

    uint32_t res;

    tnode.Config(cfg);
    tnode.Bind(1);
    tnode.Run();

    const std::string id("1");
    snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);
    TradingBO::ValidateConsumer *validate_consumer = new TradingBO::ValidateConsumer(id, channel);

    char event[400] = {};
    const char *str = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf1\",\"sessionid\":\"d2936f0909bd4c258b9e7000d4d74092\",\"groupid\":1,\"clientid\":75,\"accountid\":0,\"boaccountid\":0,\"assetid\":1,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sgid\":1,\"orderstatus\":1,\"margin\":100,\"freemargin\":99880,\"profit\":-20,\"status\":1,\"prevbalance\":0,\"changed\":100000,\"closebalance\":0}";
    strcpy(event, str);

    res = validate_consumer->EventHandle(VALIDATE, event, 335);
    EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, res);

    //tag != VALIDATE
    res = validate_consumer->EventHandle(0x4, event, 335);
    EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

    //eventlen == 0
    res = validate_consumer->EventHandle(VALIDATE, event, 0);
    EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

    //no reqid member
    const char *str1 = "{\"sessionid\":\"d2936f0909bd4c258b9e7000d4d7409c\",\"groupid\":1,\"clientid\":75,\"accountid\":0,\"boaccountid\":0,\"assetid\":0,\"direction\":1,\"expiryperiod\":5,\"amount\":100,\"sgid\":1,\"orderstatus\":0,\"margin\":100,\"freemargin\":99880,\"profit\":-20,\"status\":1,\"prevbalance\":0,\"changed\":100000,\"closebalance\":0}";
    memset(event, 0, 400 * sizeof(char));
    strcpy(event, str1);
    res = validate_consumer->EventHandle(VALIDATE, event, 297);
    EXPECT_EQ(UNIVERSAL_TRADING_VALIDATE_MSG_ERR, res);
}

TEST(tradingengineTest, TradingBo_AcknowledgeConsumer_EventHandle) {
    TradingBO Bo;
    snetwork_xservice_tnode::TNode tnode;

    uint32_t res;

    tnode.Config(cfg);
    tnode.Bind(1);
    tnode.Run();

    const std::string id("1");
    snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);
    TradingBO::AcknowledgeConsumer *acknowledge_consumer = new TradingBO::AcknowledgeConsumer(id, channel);

    char event[400] = {};
    const char *str = "{\"reqid\":\"b7ce6d971d974e9a8657e799e76f5bf0\",\"reqstatus\":5,\"sgid_teid\":1}";
    strcpy(event, str);

    res = acknowledge_consumer->EventHandle(VALIDATE, event, 73);
    EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

    res = acknowledge_consumer->EventHandle(ACKNOWLEDGE, event, 0);
    EXPECT_EQ(UNIVERSAL_TRADING_MESSAGE_DATA_ERR, res);

    res = acknowledge_consumer->EventHandle(ACKNOWLEDGE, event, 73);
    EXPECT_EQ(UNIVERSAL_TRADING_SUCCESS, res);

    const char *str1 = "{\"reqstatus\":5,\"sgid_teid\":1}";
    memset(event, 0, 400 * sizeof(char));
    strcpy(event, str1);
    res = acknowledge_consumer->EventHandle(ACKNOWLEDGE, event, 40);
    EXPECT_EQ(UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR, res);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");

	return RUN_ALL_TESTS();
}



