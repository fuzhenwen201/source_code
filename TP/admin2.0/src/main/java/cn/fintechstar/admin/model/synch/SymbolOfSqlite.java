package cn.fintechstar.admin.model.synch;

public class SymbolOfSqlite {

	private int id;
	private String symbol;
	private int symbolSpread;
	private int operatorId;
	private String modifyTime;
	private String describe;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getSymbol() {
		return symbol;
	}

	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}

	public int getSymbolSpread() {
		return symbolSpread;
	}

	public void setSymbolSpread(int symbolSpread) {
		this.symbolSpread = symbolSpread;
	}

	public int getOperatorId() {
		return operatorId;
	}

	public void setOperatorId(int operatorId) {
		this.operatorId = operatorId;
	}

	public String getModifyTime() {
		return modifyTime;
	}

	public void setModifyTime(String modifyTime) {
		this.modifyTime = modifyTime;
	}

	public String getDescribe() {
		return describe;
	}

	public void setDescribe(String describe) {
		this.describe = describe;
	}
}
