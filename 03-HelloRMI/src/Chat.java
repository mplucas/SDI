
/** Chat.java **/
import java.rmi.*;

public interface Chat extends Remote {

   public String hello() throws RemoteException;

   public int connect(String nickname) throws RemoteException;
}
