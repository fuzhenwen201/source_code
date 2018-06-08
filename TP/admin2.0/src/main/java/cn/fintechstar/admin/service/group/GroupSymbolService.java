package cn.fintechstar.admin.service.group;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.CollectionUtils;

import cn.fintechstar.admin.dao.group.IGroupSymbolDao;
import cn.fintechstar.admin.model.group.GroupSymbol;

@Service
public class GroupSymbolService {
	
	@Autowired
	private IGroupSymbolDao groupSymbolDao;

	public void saveGroupSymbols(int groupId, List<GroupSymbol> groupSymbols) {
		if(!CollectionUtils.isEmpty(groupSymbols)){
			for(GroupSymbol groupSymbol : groupSymbols){
				groupSymbol.setGroupId(groupId);
			}
			groupSymbolDao.saveGroupSymbols(groupSymbols);
		}
	}

	public void updateGroupSymbols(int groupId, List<GroupSymbol> groupSymbols) {
		groupSymbolDao.deleteGroupSymbolsByGroupId(groupId);
		if(!CollectionUtils.isEmpty(groupSymbols)){
			groupSymbolDao.saveGroupSymbols(groupSymbols);
		}
	}

	public List<GroupSymbol> getGroupSymbolsByGroupId(int groupId) {
		return groupSymbolDao.getGroupSymbolsByGroupId(groupId);
	}

}
