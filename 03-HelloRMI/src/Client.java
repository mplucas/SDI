
/** Client.java **/
import java.rmi.registry.*;

public class Client {

   private static int NULL_CLIENT_ID = -1;

   private static Chat stub;
   private static String host;
   private static String nickname;
   private static int clientID;

   public static void main(String[] args) {

      // String host = (args.length < 1) ? null : args[0];

      try {

         // verificar parametros - OK
         // conectar e pegar id - OK

         // while
         // enviar mensagens
         // polling

         readArgs(args);

         createStub();

         connectToServer();

         if (isConnectedToServer()) {

            System.out.println("Conectado no servidor com o id: " + clientID + ".");

         } else {

            System.out.println("Não conectou no servidor porque já existe um cliente com mesmo nickname: " + nickname);

         }

      } catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   private static void readArgs(String[] args) throws Exception {

      try {

         validateArgs(args);
         host = args[0];
         nickname = args[1];

      } catch (Exception ex) {
         throw ex;
      }
   }

   private static void validateArgs(String[] args) throws Exception {

      if (args.length != 2) {
         throw new Exception("Argumentos de chamada errados. Tente: java Client <host> <nickname>");
      }
   }

   private static void createStub() throws Exception {

      try {

         // Obtém uma referência para o registro do RMI
         Registry registry = LocateRegistry.getRegistry(host, 6600);
         // Obtém a stub do servidor
         stub = (Chat) registry.lookup("Chat");

      } catch (Exception ex) {
         throw ex;
      }
   }

   private static void connectToServer() throws Exception {

      try {
         clientID = stub.connect(nickname);
      } catch (Exception ex) {
         throw ex;
      }
   }

   private static boolean isConnectedToServer() {
      return clientID != NULL_CLIENT_ID;
   }
}
