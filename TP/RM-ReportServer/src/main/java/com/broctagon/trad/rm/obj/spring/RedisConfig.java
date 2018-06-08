package com.broctagon.trad.rm.obj.spring;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;

import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.ShardedJedisPool;

@Configuration
public class RedisConfig {
	
	@Autowired private Environment environment;

	@Bean
	public ShardedJedisPool shardedJedisPool(){
		JedisPoolConfig jedisPoolConfig = jedisPoolConfig();
		List<JedisShardInfo> jedisShardInfoList = jedisShardInfoList();
		ShardedJedisPool shardedJedisPool = new ShardedJedisPool(jedisPoolConfig,jedisShardInfoList);
		return shardedJedisPool;
	}
	
	@Bean
	public JedisPoolConfig jedisPoolConfig(){
		JedisPoolConfig jedisPoolConfig = new JedisPoolConfig();		
		jedisPoolConfig.setMaxIdle(Integer.parseInt(environment.getProperty("redis.pool.maxIdle")));
		jedisPoolConfig.setMaxTotal(Integer.parseInt(environment.getProperty("redis.pool.maxTotal")));	
		jedisPoolConfig.setMaxWaitMillis(Integer.parseInt(environment.getProperty("redis.pool.maxWait")));
		jedisPoolConfig.setTestOnBorrow(Boolean.parseBoolean(environment.getProperty("redis.pool.testOnBorrow")));
		return jedisPoolConfig;
	}
	
	private List<JedisShardInfo> jedisShardInfoList(){
		List<JedisShardInfo> jedisShardInfoList = new ArrayList<JedisShardInfo>();
		
		String host = environment.getProperty("redis.host");
		int port = Integer.parseInt(environment.getProperty("redis.port"));
		int timeout = Integer.parseInt(environment.getProperty("redis.timeout"));
		JedisShardInfo jedisShardInfo = new JedisShardInfo(host, port, timeout);
		String password = environment.getProperty("redis.password");	
		if(password!=null&&!"".equals(password.trim())){
			jedisShardInfo.setPassword(password);
		}
		else{
			System.out.println("password:"+password);
		}
		
		jedisShardInfoList.add(jedisShardInfo);
		
		return jedisShardInfoList;
	}
	
}

