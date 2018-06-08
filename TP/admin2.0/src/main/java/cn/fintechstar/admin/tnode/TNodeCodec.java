package cn.fintechstar.admin.tnode;

import cn.fintechstar.admin.util.BytesUtil;

public class TNodeCodec {

	private static final int TNODE_FLAG_LEN = 4, TAG_LEN = 2, VERSION_LEN = 2, REQ_ID_LEN = 17, SESSION_ID_LEN = 17, RESERVED_LEN = 2, BODY_LEN = 4;
	private static final int HEADER_LEN = TNODE_FLAG_LEN + TAG_LEN + VERSION_LEN + REQ_ID_LEN + SESSION_ID_LEN + RESERVED_LEN + BODY_LEN;
	
	public static byte[] encodeMessage(TNode tNode){
		byte[] body = tNode.getBody().getBytes();
		
		byte[] message = new byte[HEADER_LEN + body.length];		
		byte[] tnodeFlag = BytesUtil.intToBytesBig(tNode.gettNodeFlag());
		byte[] tag = BytesUtil.shortToBytesBig(tNode.getTag());
		byte[] version = BytesUtil.shortToBytesBig(tNode.getVersion());
//		byte[] requestId = tNode.getRequestId().getBytes(); // reserved 
//		byte[] sessionId = tNode.getSessionId().getBytes(); // reserved
		byte[] bodyLen = BytesUtil.intToBytesBig(tNode.getBody().getBytes().length);
		
		
		for(int i = 0; i < TNODE_FLAG_LEN; i++){
			message[i] = tnodeFlag[i];
		}
		
		int cursor = TNODE_FLAG_LEN;
		for(int i = 0; i < TAG_LEN; i++){
			message[cursor + i] = tag[i];
		}
		
		cursor = cursor + TAG_LEN;
		for(int i = 0; i < VERSION_LEN; i++){
			message[cursor + i] = version[i];
		}
		
		cursor = cursor + VERSION_LEN;
//		for(int i = 0; i < requestId.length; i++){
//			message[cursor + i] = requestId[i];
//		}
		
		cursor = cursor + REQ_ID_LEN;
//		for(int i = 0; i < sessionId.length; i++){
//			message[cursor + i] = sessionId[i];
//		}
		
		cursor = cursor + SESSION_ID_LEN + RESERVED_LEN;
		
		for(int i = 0; i < bodyLen.length; i++){
			message[cursor + i] = bodyLen[i];
		}
		
		cursor = cursor + BODY_LEN;
		for(int i = 0; i < body.length; i++){
			message[cursor + i] = body[i];
		}

		return message;
	}
	
	public static TNode decodeMessage(byte[] message){
		TNode tNode = new TNode();
		byte[] tNodeFlag = new byte[TNODE_FLAG_LEN];
		byte[] tag = new byte[TAG_LEN];
		byte[] version = new byte[VERSION_LEN];
		byte[] requestId = new byte[REQ_ID_LEN];
		byte[] sessionId = new byte[SESSION_ID_LEN];
		byte[] reserved = new byte[RESERVED_LEN];
		byte[] bodyLength = new byte[BODY_LEN];
		byte[] body = new byte[message.length - HEADER_LEN];
		
		int cursor = TNODE_FLAG_LEN;
		for(int i = 0; i < TNODE_FLAG_LEN; i++){
			tNodeFlag[i] = message[i];
		}
		
		for(int i = 0; i < TAG_LEN; i++){
			tag[i] = message[cursor + i];
		}
		
		cursor = cursor + TAG_LEN;
		for(int i = 0; i < VERSION_LEN; i++){
			version[i] = message[cursor + i];
		}
		
		cursor = cursor + VERSION_LEN;
		for(int i = 0; i < REQ_ID_LEN; i++){
			requestId[i] = message[cursor + i];
		}
		
		cursor = cursor + REQ_ID_LEN;
		for(int i = 0; i < SESSION_ID_LEN; i++){
			sessionId[i] = message[cursor + i];
		}
		
		cursor = cursor + SESSION_ID_LEN;
		for(int i = 0; i < RESERVED_LEN; i ++){
			reserved[i] = message[cursor + i];
		}
		
		cursor =cursor + RESERVED_LEN;
		for(int i = 0; i < BODY_LEN; i++){
			bodyLength[i] = message[cursor + i];
		}
		
		cursor = cursor + BODY_LEN;
		for(int i = 0; i < message.length - HEADER_LEN; i++){
			body[i] = message[cursor + i];
		}
		
		tNode.settNodeFlag(BytesUtil.bytesToIntBig(tNodeFlag));
		tNode.setTag(BytesUtil.bytesToShort(tag));
		tNode.setVersion(BytesUtil.bytesToShort(version));
		tNode.setRequestId(new String(requestId).trim());
		tNode.setSessionId(new String(sessionId).trim());
		tNode.setReserved(new String(reserved).trim());
		tNode.setBodyLength(BytesUtil.bytesToIntBig(bodyLength));
		tNode.setBody(new String(body).trim());

		return tNode;
	}
	
	public static TNode getTNode(short tag, String message){
		TNode tNode = new TNode();
		tNode.setTag(tag);
		tNode.setBody(message);
		return tNode;
	}
}
