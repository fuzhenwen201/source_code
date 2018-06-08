package com.broctagon.tradstnode.rabbit.enums;

import java.nio.ByteBuffer;

import com.broctagon.tradstnode.rabbit.enums.TNodeCnServiceHeader;

public class TNodeCnServiceHeader implements java.io.Serializable {

	private static final long serialVersionUID = 1L;

	public byte flag;
	public byte msg_type;
	public short tag;
	public int queue_name_len;
	public int msg_len;

	public byte[] getBytes() {
		byte[] tagbytes = new byte[2];
		ByteBuffer buffer = ByteBuffer.allocate(tagbytes.length);

		buffer.putShort(tag);
		tagbytes = buffer.array();
		byte[] lenbyte = new byte[4];
		ByteBuffer lbuffer = ByteBuffer.allocate(lenbyte.length);

		lbuffer.putInt(queue_name_len);
		lenbyte = lbuffer.array();
		byte[] menbyte = new byte[4];
		ByteBuffer mbuffer = ByteBuffer.allocate(menbyte.length);

		mbuffer.putInt(msg_len);
		menbyte = mbuffer.array();
		byte[] header = new byte[12];
		header[0] = flag;
		header[1] = msg_type;
		header[2] = tagbytes[0];
		header[3] = tagbytes[1];
		for (int i = 0; i < 4; i++) {
			header[i + 4] = lenbyte[i];
		}
		for (int i = 0; i < 4; i++) {
			header[i + 8] = menbyte[i];
		}
		return header;
	}

	public static TNodeCnServiceHeader deserialize(byte[] incoming) {
		TNodeCnServiceHeader header = new TNodeCnServiceHeader();
		header.flag = incoming[0];
		header.msg_len = incoming[1];
		byte[] tbyte = new byte[2];
		tbyte[0] = incoming[2];
		tbyte[1] = incoming[3];
		ByteBuffer bb = ByteBuffer.allocate(2);

		bb.put(tbyte);
		header.tag = bb.getShort(0);
		byte[] lbyte = new byte[4];
		for (int i = 0; i < 4; i++) {
			lbyte[i] = incoming[i + 4];
		}
		ByteBuffer lbb = ByteBuffer.wrap(lbyte);

		header.queue_name_len = lbb.getInt();
		byte[] mbyte = new byte[4];
		for (int i = 0; i < 4; i++) {
			mbyte[i] = incoming[i + 8];
		}
		ByteBuffer mbb = ByteBuffer.wrap(mbyte);

		header.msg_len = mbb.getInt();
		return header;
	}
}
