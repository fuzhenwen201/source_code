package com.broctagon.accumonitor.dao;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class PullDaoTest {

	@Autowired
	private PullDao dao;
	
//	@Test
	public void addPullRequestTest(){
		dao.addPullRequest("reqid", "pull request");
	}
	
//	@Test
	public void addPullResponseTest(){
		dao.addPullResponse("reqid", "pull response");
	}
}
