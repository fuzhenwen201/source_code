package cn.fintechstar.admin.config;

import java.io.IOException;

import javax.sql.DataSource;

import org.mybatis.spring.SqlSessionFactoryBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.core.env.Environment;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;

import com.zaxxer.hikari.HikariConfig;
import com.zaxxer.hikari.HikariDataSource;

@Configuration
public class DatabaseConfig {
	
	@Autowired
	private Environment env;
	
	@Bean
	public HikariConfig mysqlConfig(){
		HikariConfig hikariConfig = new HikariConfig();
		hikariConfig.setJdbcUrl(env.getProperty("dataSource.mysql.jdbcUrl"));
		hikariConfig.setDriverClassName(env.getProperty("dataSource.mysql.driverClassName"));
		hikariConfig.setUsername(env.getProperty("dataSource.mysql.userName"));
		hikariConfig.setPassword(env.getProperty("dataSource.mysql.password"));
		return hikariConfig;
	}
	
	@Bean
	@Primary
	public DataSource mysqlDataSource(){
		DataSource dataSource = new HikariDataSource(mysqlConfig());
		return dataSource;
	}
	
	@Bean
	@Primary
	public DataSourceTransactionManager mysqlTransactionManager(){
		DataSourceTransactionManager transactionManager = new DataSourceTransactionManager();
		transactionManager.setDataSource(mysqlDataSource());
		return transactionManager;
	}
	
	@Bean
	@Primary
	public SqlSessionFactoryBean mysqlSqlSessionFactory() throws IOException{
		SqlSessionFactoryBean sqlSessionFactory = new SqlSessionFactoryBean();
		sqlSessionFactory.setDataSource(mysqlDataSource());
		sqlSessionFactory.setTypeAliasesPackage("cn.fintechstar.admin.model");
		ResourcePatternResolver resourcePatternResolver = new PathMatchingResourcePatternResolver();
		sqlSessionFactory.setMapperLocations(resourcePatternResolver.getResources("/mapper/mysql/*.xml"));
		return sqlSessionFactory;
	}

	@Bean
	public DataSource sqliteDataSource(){
		return new HikariDataSource(sqliteConfig());
	}
	
	@Bean
	public HikariConfig sqliteConfig(){
		HikariConfig hikariConfig = new HikariConfig();
		hikariConfig.setJdbcUrl(env.getProperty("dataSource.sqlite.jdbcUrl"));
		hikariConfig.setDriverClassName(env.getProperty("dataSource.sqlite.driverClassName"));
		return hikariConfig;
	}
	
	@Bean
	public DataSourceTransactionManager sqliteTransactionManager(){
		DataSourceTransactionManager transactionManager = new DataSourceTransactionManager();
		transactionManager.setDataSource(sqliteDataSource());
		return transactionManager;
	}
	
	@Bean
	public SqlSessionFactoryBean sqliteSqlSessionFactory() throws IOException{
		SqlSessionFactoryBean sqlSessionFactory = new SqlSessionFactoryBean();
		sqlSessionFactory.setDataSource(sqliteDataSource());
		sqlSessionFactory.setTypeAliasesPackage("cn.fintechstar.admin.model.synch");
		ResourcePatternResolver resourcePatternResolver = new PathMatchingResourcePatternResolver();
		sqlSessionFactory.setMapperLocations(resourcePatternResolver.getResources("/mapper/sqlite/*.xml"));
		return sqlSessionFactory;
	}
}
