
/** Client.java **/
import java.rmi.registry.*;

public class Client {
   public static void main(String[] args) {
      String host = (args.length < 1) ? null : args[0];
      try {
         // Obtém uma referência para o registro do RMI
         Registry registry = LocateRegistry.getRegistry(host, 6600);

         // Obtém a stub do servidor
         Chat stub = (Chat) registry.lookup("Hello");

         // Chama o método do servidor e imprime a mensagem
         String msg = stub.hello();
         System.out.println("Mensagem do Servidor: " + msg);
      } catch (Exception ex) {
         ex.printStackTrace();
      }
   }
}
