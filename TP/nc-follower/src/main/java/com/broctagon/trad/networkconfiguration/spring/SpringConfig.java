
package com.broctagon.trad.networkconfiguration.spring;

import org.apache.curator.RetryPolicy;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.retry.ExponentialBackoffRetry;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.EnableAspectJAutoProxy;
import org.springframework.core.env.Environment;

import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperHelper;

/**
* @auther: Water
* @time: Oct 13, 2017 6:14:06 PM
* 
*/

@EnableAspectJAutoProxy
@Configuration
public class SpringConfig {
	
	@Autowired
	private Environment environment;
	
	@Bean
	public CuratorFramework curatorFramework(){		
		RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000,3);
		String hostPort = environment.getProperty("zk.hostport");
		System.out.println("hostPort:" + hostPort);
		CuratorFramework zfc = CuratorFrameworkFactory.newClient(hostPort, retryPolicy);				
		return zfc;		
	}
	
	@Bean
	public ZookeeperHelper zookeeperHelper(){
		CuratorFramework zfc = curatorFramework();
		ZookeeperHelper zookeeperHelper = new ZookeeperHelper(zfc);
		return zookeeperHelper;
	}
	
	
}
