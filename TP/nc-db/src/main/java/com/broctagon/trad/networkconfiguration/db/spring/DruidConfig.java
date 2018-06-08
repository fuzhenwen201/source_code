package com.broctagon.trad.networkconfiguration.db.spring;

import java.sql.SQLException;

import javax.sql.DataSource;

import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.boot.autoconfigure.SpringBootVFS;
import org.mybatis.spring.mapper.MapperScannerConfigurer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;

import com.alibaba.druid.pool.DruidDataSource;

@ComponentScan(basePackages="com.broctagon.trad.networkconfiguration.db")
@Configuration
//@PropertySource("file:${configFilePath}")
public class DruidConfig implements EnvironmentAware{
	
	@Autowired
	private Environment environment;
	
	@Bean
	public DataSource dataSource() throws SQLException{	
				
		DruidDataSource dataSource = new DruidDataSource();
		dataSource.setDriverClassName(environment.getProperty("db.driverClassName"));
		dataSource.setUrl(environment.getProperty("db.url"));
		dataSource.setDbType(environment.getProperty("db.dbType"));
/*		dataSource.setUsername(environment.getProperty("db.username"));
		dataSource.setPassword(environment.getProperty("db.password"));*/
		dataSource.setInitialSize(Integer.parseInt(environment.getProperty("db.initialSize")));
		dataSource.setMinIdle(Integer.parseInt(environment.getProperty("db.minIdle")));
		dataSource.setMaxActive(Integer.parseInt(environment.getProperty("db.maxActive")));
		dataSource.setFilters(environment.getProperty("db.filters"));
		return dataSource;
	}
	
	@Bean
	public SqlSessionFactory sqlSessionFactory() throws Exception{
		SqlSessionFactoryBean sessionFactoryBean = new SqlSessionFactoryBean();
		sessionFactoryBean.setDataSource(dataSource());
		sessionFactoryBean.setVfs(SpringBootVFS.class);
		sessionFactoryBean.setTypeAliasesPackage("com.broctagon.trad.networkconfiguration.db.model");	
		Resource[] mapperLocations = new PathMatchingResourcePatternResolver().getResources("classpath*:/mapper/*xml");
		sessionFactoryBean.setMapperLocations(mapperLocations);
	//	sessionFactoryBean.setMapperLocations(new PathMatchingResourcePatternResolver().getResources("/mapper/*.xml"));
	//	Resource mybaitsResource = new ClassPathResource("mybatisConfig.xml");
	//	sessionFactoryBean.setConfigLocation(mybaitsResource);
		return sessionFactoryBean.getObject();
	}
	
	@Bean
	public MapperScannerConfigurer mapperScannerConfigurer(){
		MapperScannerConfigurer mapperScannerConfigurer = new MapperScannerConfigurer();
		mapperScannerConfigurer.setBasePackage("com.broctagon.trad.networkconfiguration.db.dao");
		mapperScannerConfigurer.setSqlSessionFactoryBeanName("sqlSessionFactory");
		return mapperScannerConfigurer;
	}
	
	@Bean
	public DataSourceTransactionManager transactionManager() throws SQLException{
		DataSourceTransactionManager transactionManager = new DataSourceTransactionManager();
		transactionManager.setDataSource(dataSource());
		return transactionManager;
	}
	
	public void setEnvironment(Environment environment) {
		this.environment = environment;
	}

}
