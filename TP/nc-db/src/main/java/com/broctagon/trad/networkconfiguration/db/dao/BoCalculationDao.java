package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.BoCalculationModel;

public interface BoCalculationDao {

	public List<BoCalculationModel> queryBoCalculationModels();
	
	public int addBoCalculationModel(BoCalculationModel boCalculationModel);
	public int addBoCalculationModels(List<BoCalculationModel> boCalculationModels);
	
	public int deleteAll();
}
