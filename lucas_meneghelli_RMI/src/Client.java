
/** Client.java **/
import java.io.File;
import java.rmi.registry.*;

public class Client {

   private static int NULL_CLIENT_ID = -1;
   private static int FIVE_SECONDS = 5000;

   private static Chat stub;
   private static String host;
   private static String nickName;
   private static int clientID;

   private static int sendMessageIndex = 1;
   private static int receiveMessageIndex = 1;

   public static void incrementSendMessageIndex() {
      Client.sendMessageIndex++;
   }

   public static void incrementReceiveMessageIndex() {
      Client.receiveMessageIndex++;
   }

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

            while (true) {

               trySendFile();

               poll();

               Thread.sleep(FIVE_SECONDS);

            }

         } else {
            System.out.println("Não conectou no servidor porque já existe um cliente com mesmo nickname: " + nickName);
         }

      } catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   private static void readArgs(String[] args) throws Exception {

      try {

         validateArgs(args);
         host = args[0];
         nickName = args[1];

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
         clientID = stub.connect(nickName);
      } catch (Exception ex) {
         throw ex;
      }
   }

   private static boolean isConnectedToServer() {
      return clientID != NULL_CLIENT_ID;
   }

   private static void trySendFile() throws Exception {

      try {

         boolean hasMessagesToSend = true;

         while (hasMessagesToSend) {

            String strMessageIndex = "0" + sendMessageIndex;
            strMessageIndex = strMessageIndex.substring(strMessageIndex.length() - 2, strMessageIndex.length());

            String fileName = nickName + "-" + strMessageIndex + ".chat";

            System.out.println("Verificando se existe o arquivo " + fileName + " para ser enviado.");

            if (fileExistsWith(fileName)) {

               FileManager fileManager = new FileManager();

               Message message = new Message();
               message = fileManager.readMessageIn(fileName);

               stub.sendChat(message);

               incrementSendMessageIndex();

               System.out.println("Arquivo " + fileName + " enviado com sucesso.");

            } else {

               System.out.println("Arquivo " + fileName + " não encontrado.");
               hasMessagesToSend = false;

            }
         }

      } catch (Exception ex) {
         throw ex;
      }

   }

   private static boolean fileExistsWith(String fileName) {

      File f = new File(fileName);
      return f.exists() && !f.isDirectory();

   }

   private static void poll() throws Exception {

      try {

         System.out.println("Verificando se há novas mensagens no server (poll).");
         int serverMessageIndex = stub.getMessageIndex();

         if (serverMessageIndex <= receiveMessageIndex) {
            System.out.println("Nenhuma mensagem nova no server.");
         }

         while (receiveMessageIndex < serverMessageIndex) {

            System.out.println("Encontrado mensagem " + receiveMessageIndex + ". Realizando requisição.");

            Message message = stub.receiveChat(receiveMessageIndex);

            if (message.getNickName().equals(nickName)) {

               System.out
                     .println("Mensagem " + receiveMessageIndex + " é uma mensagem enviada por você. Não será salva.");

            } else {

               System.out.println(message.getNickName() + " enviou:\n" + message.getContent() + "\n");

               String strMessageIndex = "0" + receiveMessageIndex;
               strMessageIndex = strMessageIndex.substring(strMessageIndex.length() - 2, strMessageIndex.length());
               String strClientID = "0" + clientID;
               strClientID = strClientID.substring(strClientID.length() - 2, strClientID.length());

               String fileName = nickName + "-" + strMessageIndex + ".client" + strClientID;

               FileManager fileManager = new FileManager();
               fileManager.writeContentInFile(message.getContent(), fileName);

               System.out.println("Salva mensagem " + receiveMessageIndex + " em " + fileName + ".");
            }

            incrementReceiveMessageIndex();

         }

      } catch (Exception ex) {
         throw ex;
      }

   }
}
