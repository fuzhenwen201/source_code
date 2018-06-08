package cn.fintechstar.admin.dao.group;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.group.GroupSymbol;

@Repository
@Transactional
public interface IGroupSymbolDao {

	void saveGroupSymbols(List<GroupSymbol> groupSymbols);

	List<GroupSymbol> getGroupSymbolsByGroupId(int groupId);

	void deleteGroupSymbolsByGroupId(int groupId);

}
