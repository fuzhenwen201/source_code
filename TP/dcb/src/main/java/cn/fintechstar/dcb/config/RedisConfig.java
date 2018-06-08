package cn.fintechstar.dcb.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.CacheManager;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.data.redis.cache.RedisCacheManager;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.serializer.StringRedisSerializer;

import redis.clients.jedis.JedisPoolConfig;

@Configuration
@EnableCaching
public class RedisConfig {

	@Autowired
	private Environment environment;
	
	@Bean
	public JedisConnectionFactory redisConnectionFactory(){
		JedisConnectionFactory connectionFactory = new JedisConnectionFactory();
		connectionFactory.setUsePool(true);
		connectionFactory.setHostName(environment.getProperty("spring.redis.host"));
		connectionFactory.setPort(Integer.parseInt(environment.getProperty("spring.redis.port")));
		connectionFactory.setPassword(environment.getProperty("spring.redis.password"));
		connectionFactory.setDatabase(Integer.parseInt(environment.getProperty("spring.redis.database")));
		JedisPoolConfig poolConfig=new JedisPoolConfig();  
	    poolConfig.setMaxIdle(Integer.parseInt(environment.getProperty("spring.redis.pool.max-active")));  
	    poolConfig.setMinIdle(Integer.parseInt(environment.getProperty("spring.redis.pool.min-idle"))); 
	    connectionFactory.setPoolConfig(poolConfig);
		return connectionFactory;
	}
	
	@Bean
	public RedisTemplate<String, String> redisTemplate(){
		RedisTemplate<String, String> redisTemplate = new RedisTemplate<String, String>();
		redisTemplate.setConnectionFactory(redisConnectionFactory());
		redisTemplate.setDefaultSerializer(new StringRedisSerializer());
		return redisTemplate;
	}
	
	@Bean
	public CacheManager cacheManager(){
		return new RedisCacheManager(redisTemplate());
	}
}
