package com.broctagon.accumonitor.util;

import org.junit.Assert;
import org.junit.Test;


public class ConstantUtilTest {

	String str = "{\"reqid\":\"51b3a2924f1446959ee163d989ccd0be\", \"groupid\":1,\"clientid\":72,\"sgid\":5,\"status\":1,\"sessionid\":\"f595259a5b1b11e7bf8eba9c61387542\"} ";
	
	@Test
	public void testGetRequestId(){
		Assert.assertEquals(ConstantUtil.getMessageFromRequest(str, "reqid"), "51b3a2924f1446959ee163d989ccd0be");
		str = "{\"reqid\":\"197e35aa9a4d40a196b918cddb19bc65\",\"groupid\":1,\"clientid\":72,\"error\":\"Password does not match1\",\"sgid\":5,\"status\":0,\"sessionid\":\"f618958c5b1c11e7bf8e3576a289cb41\"}";
		Assert.assertEquals(ConstantUtil.getMessageFromRequest(str, "reqid"), "197e35aa9a4d40a196b918cddb19bc65");
	}
	
	@Test
	public void testGetSgId(){
		Assert.assertSame(ConstantUtil.getSgId(str), 5);
	}
	
	@Test
	public void testGetRoutingKey(){
		String str = "TEST.#";
		Assert.assertEquals(ConstantUtil.getRoutingKey(str), "TEST.$");
	}
}
