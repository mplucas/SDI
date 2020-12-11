
/** Server.java **/

import java.rmi.*;
import java.rmi.server.*;
import java.util.ArrayList;
import java.rmi.registry.*;

public class Server implements Chat {

   private ArrayList<String> nickNameList;
   private ArrayList<String> savedFilesNameList;

   public Server() {

      nickNameList = new ArrayList<String>();
      savedFilesNameList = new ArrayList<String>();

   }

   public static void main(String[] args) {

      try {

         validateArgs(args);

         System.setProperty("java.rmi.server.hostname", args[0]);
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

   private static void validateArgs(String[] args) throws Exception {

      if (args.length != 1) {
         throw new Exception(
               "Argumentos de chamada errados. Tente: java Server <host> (IMPORTANTE: para acessar esse server por um cliente de outra máquina, informe o ip, nao simplesmente 'localhost'. Ex: java Server 10.20.221.230.)");
      }
   }

   @Override
   public String hello() throws RemoteException {
      System.out.println("Executando hello()");
      return "Hello!!!";
   }

   @Override
   public int connect(String nickName) throws RemoteException {

      System.out.println(nickName + " solicitou conexâo.");

      if (!nickNameList.contains(nickName)) {

         nickNameList.add(nickName);
         int newClientID = nickNameList.size();

         System.out.println(nickName + " registrado com id " + newClientID + ".");

         return newClientID;

      } else {

         System.out.println(nickName + " recusado pois já existe cliente conectado com mesmo id.");

         return -1;
      }
   }

   @Override
   public void sendChat(Message message) throws RemoteException {

      System.out.println("Recebido mensagem de " + message.getNickName() + ".");

      String strMessageIndex = "0" + getCurrentMessageIndex();
      strMessageIndex = strMessageIndex.substring(strMessageIndex.length() - 2, strMessageIndex.length());

      String fileName = message.getNickName() + "-" + strMessageIndex + ".serv";

      FileManager fileManager = new FileManager();
      fileManager.writeContentInFile(message.getContent(), fileName);

      savedFilesNameList.add(fileName);

      System.out.println("Mensagem salva em " + fileName + ".");

   }

   private int getCurrentMessageIndex() {
      return savedFilesNameList.size() + 1;
   }

   @Override
   public int getMessageIndex() throws RemoteException {
      return getCurrentMessageIndex();
   }

   @Override
   public Message receiveChat(int messageIndex) throws RemoteException {

      System.out.println("Recebido requisição da mensagem " + messageIndex + ".");

      int messageIndexInSavedFilesNameList = messageIndex - 1;

      if (messageIndexInSavedFilesNameList >= 0 && messageIndexInSavedFilesNameList < savedFilesNameList.size()) {

         String fileName = savedFilesNameList.get(messageIndexInSavedFilesNameList);
         FileManager fileManager = new FileManager();
         System.out.println("Retornada mensagem " + messageIndex + ": " + fileName + ".");
         return fileManager.readMessageIn(fileName);

      } else {

         System.out.println("ID da mensagem inválido.");
         return null;

      }

   }
}
