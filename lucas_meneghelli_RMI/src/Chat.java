
/** Chat.java **/
import java.rmi.*;

public interface Chat extends Remote {

   public String hello() throws RemoteException;

   public int connect(String nickName) throws RemoteException;

   public void sendChat(Message message) throws RemoteException;

   public int getMessageIndex() throws RemoteException;

   public Message receiveChat(int messageIndex) throws RemoteException;
}
