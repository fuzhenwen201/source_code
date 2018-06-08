package cn.fintechstar.admin.config;

import org.mybatis.spring.mapper.MapperScannerConfigurer;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MybatisConfig {
	
	@Bean
	public MapperScannerConfigurer mysqlMapperScannerConfigurer(){
		MapperScannerConfigurer mapperScanner = new MapperScannerConfigurer();
		mapperScanner.setBasePackage("cn.fintechstar.admin.dao");
		mapperScanner.setSqlSessionFactoryBeanName("mysqlSqlSessionFactory");
		return mapperScanner;
	}
	
	@Bean
	public MapperScannerConfigurer sqliteMapperScannerConfigurer(){
		MapperScannerConfigurer mapperScanner = new MapperScannerConfigurer();
		mapperScanner.setBasePackage("cn.fintechstar.admin.synch.dao");
		mapperScanner.setSqlSessionFactoryBeanName("sqliteSqlSessionFactory");
		return mapperScanner;
	}
}
