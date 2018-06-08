package com.broctagon.trad.reportserver.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.trad.reportserver.model.Price;

@Repository
@Transactional
public interface PriceDao {

	public Price getPriceBySymbol(String symbol);
	public List<Price> getPricesBySymbols(List<String> symbols);
	
	public int addPrice(Price price);
	public int addPrices(List<Price> prices);
	
	public int updatePrice(Price price);
	public int updatePrices(List<Price> prices);
	
	public int deletePriceBySymbol(String symbol);
	public int deletePricesBySymbols(List<String> symbols);
	
}
