package cn.fintechstar.admin.synch.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.synch.GroupOfSqlite;

@Repository
@Transactional
public interface IBoGroupDao {

	void clearTable();
	
	void saveBoGroups(List<GroupOfSqlite> boGroups);
}
