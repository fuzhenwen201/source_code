package cn.fintechstar.admin.dao.group;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.TimePeriod;

@Repository
@Transactional
public interface ITimePeriodDao {

	void saveTimePeriods(List<TimePeriod> timePeriods);

	void deleteTimePeriodsByGroupId(int groupId);

	List<TimePeriod> getTimePeriodsByGroupId(int groupId);

}
