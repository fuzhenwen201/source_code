package com.broctagon.trad.rm.obj.req.message;
/**
 * @author imdadullah
 *
 */
public class CustomBoReqMsg {

	private int assetid;
	private String reqid;
	private int managerid;
	private int direction;
	private int sgid;
	private String session;
	private boolean isStop = false;
	
	
	public boolean isStop() {
		return isStop;
	}
	public void setStop(boolean isStop) {
		this.isStop = isStop;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public int getAssetid() {
		return assetid;
	}
	public void setAssetid(int assetid) {
		this.assetid = assetid;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + assetid;
		result = prime * result + direction;
		result = prime * result + managerid;
		return result;
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (!(obj instanceof CustomBoReqMsg))
			return false;
		CustomBoReqMsg other = (CustomBoReqMsg) obj;
		if (assetid != other.assetid)
			return false;
		if (direction != other.direction)
			return false;
		if (managerid != other.managerid)
			return false;
		return true;
	}
	
}
