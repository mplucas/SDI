
/** Server.java **/

import java.rmi.*;
import java.rmi.server.*;
import java.util.ArrayList;
import java.rmi.registry.*;

public class Server implements Chat {

   private ArrayList<String> nicknameList;

   public Server() {

      nicknameList = new ArrayList<String>();

   }

   public static void main(String[] args) {

      try {
         // Instancia o objeto servidor e a sua stub
         Server server = new Server();
         Chat stub = (Chat) UnicastRemoteObject.exportObject(server, 0);

         // Registra a stub no RMI Registry para que ela seja obtAida pelos clientes
         Registry registry = LocateRegistry.createRegistry(6600);

         registry.bind("Chat", stub);
         System.out.println("Servidor pronto");

      } catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   @Override
   public String hello() throws RemoteException {
      System.out.println("Executando hello()");
      return "Hello!!!";
   }

   @Override
   public int connect(String nickname) throws RemoteException {

      System.out.println(nickname + " solicitou conexâo.");

      if (!nicknameList.contains(nickname)) {

         nicknameList.add(nickname);
         int newClientID = nicknameList.size();

         System.out.println(nickname + " registrado com id " + newClientID + ".");

         return newClientID;
      
      } else {

         System.out.println(nickname + " recusado pois já existe cliente conectado com mesmo id.");
         
         return -1;
      }
   }
}
