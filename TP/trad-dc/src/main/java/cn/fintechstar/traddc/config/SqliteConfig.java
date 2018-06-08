package cn.fintechstar.traddc.config;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.jdbc.core.JdbcTemplate;

@Configuration
public class SqliteConfig {
	
	@Bean(name="groupTemplate")
	@Primary
	public JdbcTemplate groupTemplate(@Qualifier("dataSource") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}

	@Bean(name="audCadTemplate")
	public JdbcTemplate audCadTemplate(@Qualifier("AUD_CAD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audChfTemplate")
	public JdbcTemplate audChfTemplate(@Qualifier("AUD_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audHkdTemplate")
	public JdbcTemplate audHkdTemplate(@Qualifier("AUD_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audJpyTemplate")
	public JdbcTemplate audJpyTemplate(@Qualifier("AUD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audNzdTemplate")
	public JdbcTemplate audNzdTemplate(@Qualifier("AUD_NZD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audSgdTemplate")
	public JdbcTemplate audSgdTemplate(@Qualifier("AUD_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="audUsdTemplate")
	public JdbcTemplate audUsdTemplate(@Qualifier("AUD_USD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="cadChfTemplate")
	public JdbcTemplate cadChfTemplate(@Qualifier("CAD_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="cadHkdTemplate")
	public JdbcTemplate cadHkdTemplate(@Qualifier("CAD_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="cadJpyTemplate")
	public JdbcTemplate cadJpyTemplate(@Qualifier("CAD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="cadSgdTemplate")
	public JdbcTemplate cadSgdTemplate(@Qualifier("CAD_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="chfHkdTemplate")
	public JdbcTemplate chfHkdTemplate(@Qualifier("CHF_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="chfJpyTemplate")
	public JdbcTemplate chfJpyTemplate(@Qualifier("CHF_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="chfZarTemplate")
	public JdbcTemplate chfZarTemplate(@Qualifier("CHF_ZAR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurAudTemplate")
	public JdbcTemplate eurAudTemplate(@Qualifier("EUR_AUD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurCadTemplate")
	public JdbcTemplate eurCadTemplate(@Qualifier("EUR_CAD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurChfTemplate")
	public JdbcTemplate eurChfTemplate(@Qualifier("EUR_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurCzkTemplate")
	public JdbcTemplate eurCzkTemplate(@Qualifier("EUR_CZK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurDkkTemplate")
	public JdbcTemplate eurDkkTemplate(@Qualifier("EUR_DKK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurGbpTemplate")
	public JdbcTemplate eurGbpTemplate(@Qualifier("EUR_GBP") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurHkdTemplate")
	public JdbcTemplate eurHkdTemplate(@Qualifier("EUR_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurHufTemplate")
	public JdbcTemplate eurHufTemplate(@Qualifier("EUR_HUF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurJpyTemplate")
	public JdbcTemplate eurJpyTemplate(@Qualifier("EUR_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurNokTemplate")
	public JdbcTemplate eurNokTemplate(@Qualifier("EUR_NOK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurNzdTemplate")
	public JdbcTemplate eurNzdTemplate(@Qualifier("EUR_NZD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurPlnTemplate")
	public JdbcTemplate eurPlnTemplate(@Qualifier("EUR_PLN") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurSekTemplate")
	public JdbcTemplate eurSekTemplate(@Qualifier("EUR_SEK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurSgdTemplate")
	public JdbcTemplate eurSgdTemplate(@Qualifier("EUR_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	@Bean(name="eurTryTemplate")
	public JdbcTemplate eurTrtTemplate(@Qualifier("EUR_TRY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurUsdTemplate")
	public JdbcTemplate eurUsdTemplate(@Qualifier("EUR_USD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="eurZarTemplate")
	public JdbcTemplate eurZarTemplate(@Qualifier("EUR_ZAR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpAudTemplate")
	public JdbcTemplate gbpAudTemplate(@Qualifier("GBP_AUD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpCadTemplate")
	public JdbcTemplate gbpCadTemplate(@Qualifier("GBP_CAD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpChfTemplate")
	public JdbcTemplate gbpChfTemplate(@Qualifier("GBP_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpHkdTemplate")
	public JdbcTemplate gbpHkdTemplate(@Qualifier("GBP_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpJpyTemplate")
	public JdbcTemplate gbpJpyTemplate(@Qualifier("GBP_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpNzdTemplate")
	public JdbcTemplate gbpNzdTemplate(@Qualifier("GBP_NZD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpPlnTemplate")
	public JdbcTemplate gbpPlnTemplate(@Qualifier("GBP_PLN") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpSgdTemplate")
	public JdbcTemplate gbpSgdTemplate(@Qualifier("GBP_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpUsdTemplate")
	public JdbcTemplate gbpUsdTemplate(@Qualifier("GBP_USD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="gbpZarTemplate")
	public JdbcTemplate gbpZarTemplate(@Qualifier("GBP_ZAR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdCadTemplate")
	public JdbcTemplate nzdCadTemplate(@Qualifier("NZD_CAD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdChfTemplate")
	public JdbcTemplate nzdChfTemplate(@Qualifier("NZD_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdHkdTemplate")
	public JdbcTemplate nzdHkdTemplate(@Qualifier("NZD_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdJpyTemplate")
	public JdbcTemplate nzdJpyTemplate(@Qualifier("NZD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdSgdTemplate")
	public JdbcTemplate nzdSgdTemplate(@Qualifier("NZD_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="nzdUsdTemplate")
	public JdbcTemplate nzdUsdTemplate(@Qualifier("NZD_USD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="sgdChfTemplate")
	public JdbcTemplate sgdChfTemplate(@Qualifier("SGD_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="sgdHkdTemplate")
	public JdbcTemplate sgdHkdTemplate(@Qualifier("SGD_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="sgdJpyTemplate")
	public JdbcTemplate sgdJpyTemplate(@Qualifier("SGD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="fincoinUsdTemplate")
	public JdbcTemplate fincoinUsdTemplate(@Qualifier("FINCOIN_USD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="hkdJpyTemplate")
	public JdbcTemplate hkdJpyTemplate(@Qualifier("HKD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="symbolButtTemplate")
	public JdbcTemplate symbolButtTemplate(@Qualifier("SymbolBUTT") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="tryJpyTemplate")
	public JdbcTemplate tryJpyTemplate(@Qualifier("TRY_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="zarJpyTemplate")
	public JdbcTemplate zarJpyTemplate(@Qualifier("ZAR_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}
	
	@Bean(name="usdCadTemplate")
	public JdbcTemplate usdCadTemplate(@Qualifier("USD_CAD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdChfTemplate")
	public JdbcTemplate usdChfTemplate(@Qualifier("USD_CHF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdCnhTemplate")
	public JdbcTemplate usdCnhTemplate(@Qualifier("USD_CNH") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdCzkTemplate")
	public JdbcTemplate usdCzkTemplate(@Qualifier("USD_CZK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdDkkTemplate")
	public JdbcTemplate usdDkkTemplate(@Qualifier("USD_DKK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdHkdTemplate")
	public JdbcTemplate usdHkdTemplate(@Qualifier("USD_HKD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdHufTemplate")
	public JdbcTemplate usdHufTemplate(@Qualifier("USD_HUF") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdInrTemplate")
	public JdbcTemplate usdInrTemplate(@Qualifier("USD_INR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdJpyTemplate")
	public JdbcTemplate usdJpyTemplate(@Qualifier("USD_JPY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdMxnTemplate")
	public JdbcTemplate usdMxnTemplate(@Qualifier("USD_MXN") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdNokTemplate")
	public JdbcTemplate usdNokTemplate(@Qualifier("USD_NOK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdPlnTemplate")
	public JdbcTemplate usdPlnTemplate(@Qualifier("USD_PLN") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdSarTemplate")
	public JdbcTemplate usdSarTemplate(@Qualifier("USD_SAR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdSekTemplate")
	public JdbcTemplate usdSekTemplate(@Qualifier("USD_SEK") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdSgdTemplate")
	public JdbcTemplate usdSgdTemplate(@Qualifier("USD_SGD") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdThbTemplate")
	public JdbcTemplate usdThbTemplate(@Qualifier("USD_THB") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdTryTemplate")
	public JdbcTemplate usdTryTemplate(@Qualifier("USD_TRY") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
	@Bean(name="usdZarTemplate")
	public JdbcTemplate usdZarTemplate(@Qualifier("USD_ZAR") DataSource dataSource){
		return new JdbcTemplate(dataSource);
	}	
	
}
