package cn.fintechstar.admin.synch.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.synch.GroupOfSqlite;

@Repository
@Transactional
public interface IFxGroupDao {

	void clearTable();

	void saveFxGroups(List<GroupOfSqlite> fxGroups);

}
